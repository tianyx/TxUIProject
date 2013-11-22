#include "StdAfx.h"
#include "MBCEndObj.h"
#include <algorithm>
#include "FGlobal.h"
#include "GlobalDef.h"

typedef INT64 int64_t;
typedef unsigned char uint8_t;

#define TOMILLSENDPARM_TM 27
bool GetPCR(BYTE* data, int size, __int64* pcr, int *data_parse );
static int parse_pcr(int64_t *ppcr_high,  int *ppcr_low, const uint8_t *packet)
{
	int afc, len, flags;
	const uint8_t *p;
	unsigned int v;

	afc = (packet[3] >> 4) & 3;
	if (afc <= 1)
		return -1;

	p = packet + 4;
	len = p[0];
	p++;
	if (len == 0)
		return -1;

	flags = *p++;
	len--;
	if (!(flags & 0x10))
		return -1;

	if (len < 6)
		return -1;

	v = ((int)(p[0]) << 24) | ((int)(p[1]) << 16) | ((int)(p[2])<<8) | p[3];

	*ppcr_high = ((int64_t)v << 1) | (p[4] >> 7);
	*ppcr_low = ((p[4] & 1) << 8) | p[5];
	return 0;
}

bool GetPCR(BYTE* data, int size, __int64* pcr, int *data_parse )
{
	*data_parse = 0;

	int64_t hipcr;
	int lopcr;


	if( size == 188 && data[0] == 0x47 )
	{
		if( parse_pcr(&hipcr, &lopcr, data) >= 0 )
		{
			(*data_parse) += 188;
			*pcr = (__int64)((hipcr*300+lopcr));
			return true;
		}

	}


	return false;
}

DWORD __stdcall CalcBitrateProc(void* lparam)
{
	CMBCEndObj* pObj = (CMBCEndObj*) lparam;
	ASSERT(pObj);
	int nSec =0;
	ULONG64 nTotal = 0;
	while(TRUE)
	{
		if (pObj->m_bQuitCalcBitrateThread)
		{
			pObj->ClearRecvAddrTracer();
			pObj->ClearRelayAddrs();
			return 0;
		}
		//clear src trace list
		pObj->ClearRecvAddrTracer();
		pObj->m_nRecvByte = 0;
		nSec = GetTickCount();
		Sleep(g_nEndBitRateCheckInterval);
		nSec = GetTickCount()-nSec;
		if (nSec == 0)
		{
			continue;
		}
		//TRACE("\n time =%d, recvbit = %d", nSec, pObj->m_nRecvByte);
		nTotal+= pObj->m_nRecvByte;
		if (pObj->m_nRecvByte == 0)
		{
			//TRACE("\n---total = %x", nTotal);
			nTotal = 0;
		}
		pObj->m_nBitRate = ((double)(pObj->m_nRecvByte* 8)) /nSec; //add padding bit
		pObj->CheckBitrate();

		
	}
}

DWORD __stdcall RelayDataProc(void* lparam)
{
	CMBCEndObj* pObj = (CMBCEndObj*) lparam;
	pObj->CacheExSendProc();
	return 0;

}

DWORD __stdcall UDPSockLoopProc( void* lparam )
{
	CMBCEndObj* pObj = (CMBCEndObj*)lparam;
	SOCKET sockRecv = *(pObj->m_pSockBase);
	char szRecvbuff[MAX_RECVBUFF];
	while(TRUE)
	{
		if (pObj->m_bQuitUdpThread)
		{
			pObj->m_bQuitUdpThread = FALSE;
			return 0;
		}
		int ret = 0;
		SOCKADDR_IN from;
		int addrlen = sizeof(from);
		char* pPos = szRecvbuff;
		int nRead = recvfrom(sockRecv,pPos,MAX_RECVBUFF,0,(struct sockaddr*)&from, &addrlen);
		int nTotal = 0;
		int nLoopCount = 0;
		if (pObj->m_bQuitUdpThread)
		{
			pObj->m_bQuitUdpThread = FALSE;
			return 0;
		}
		if(nRead != SOCKET_ERROR)
		{
			
			pObj->m_nRecvByte += nRead;
			//RelayData(m_szrecvbuff, nRead);
			//TRACE("\n read end err = %d, total = %d, loop = %d",WSAGetLastError(), nTotal, nLoopCount);

			if (pObj->m_nObjType == MBCOBJTYPE_ENDBACK)
			{
				pObj->CopyCacheData(szRecvbuff, nRead);
			}
			else
			{
				pObj->AddRecvAddrTracer(from);

			}
			

		}
		else
		{
			HRESULT hr = WSAGetLastError();
			if (hr == WSAEMSGSIZE)
			{
				TRACE("\nmsg too large");
			}
			else if (hr == WSAEFAULT)
			{
				TRACE("\n buffer error");
			}
			else
			{
				//reconnect;
				pObj->m_pSockBase->SockMsgCallback(MAKEWORD(WSAENETDOWN, FD_CONNECT), (LPARAM)(pObj->m_pSockBase->m_hSock));
			}
		}
		
	}
}

void CMBCEndObj::CopyCacheData( char* buffIn, const int nLenIn )
{
	if(nLenIn <= 0 || nLenIn%TS_PACKET_SIZE != 0)
	{
		ASSERT(FALSE);
		return;
	}
	static int snPackCount = 0;
	static int snPcrCount = 0;
	static int snTicUse = GetTickCount();
	int nTsPackSize = nLenIn/TS_PACKET_SIZE;
	char* p = buffIn;
	int64_t pcr = -1;
	int dataparse = 0;

	BOOL bWait = ((m_deqCache.m_nMaxSize - m_deqCache.m_nUsed) < (TS_PACKWITHPCR_SIZE*nTsPackSize));
	if (bWait)
	{
		//real stream can't wait!!!
		TRACE("\ncache overflow!!!");
		return;
		//WaitForSingleObject(m_lockCacheWrite, INFINITE);
	}

	if ( m_bSendFirstReadSignal && m_deqCache.m_nUsed > g_GlobalInfo.nCacheBeforeRelay )
	{
		ReleaseSemaphore(m_hFirstReadSignal, 1, 0);
		m_bSendFirstReadSignal = FALSE;
	}

	CAutoLock lock(&m_crtCache);

	for (int i = 0; i < nTsPackSize; ++i)
	{
		snPackCount++;
		if (!GetPCR((BYTE*)p, TS_PACKET_SIZE, &pcr, &dataparse))
		{
			pcr = -1;
		}
		else
		{
			snPcrCount++;

		}

	
		int nLenCopy = TS_PACKWITHPCR_SIZE;
		m_deqCache.WriteBuff((char*)&pcr, 8);
		m_deqCache.WriteBuff(p, TS_PACKET_SIZE);
		p+=TS_PACKET_SIZE;
	}

	if (m_deqCache.m_nUsed >0)
	{
		//_RPT0(0,"\nrelease read");
		ReleaseSemaphore(m_lockCacheRead, 1, NULL);
	}
}

void CMBCEndObj::CopyExchangeData()
{

	//copy in
	m_deqCacheExchange.Clear();

	m_crtCache.Lock();
	BOOL bwaite =m_deqCache.m_nUsed < TS_PACKWITHPCR_SIZE;
	BOOL bReleaseWrite = m_deqCache.m_nUsed > 0;
	m_crtCache.Unlock();
	if (bwaite)
	{
		_RPT0(0,"\nwait read");
		WaitForSingleObject(m_lockCacheRead, 1000);

	}
	if (m_deqCache.m_nUsed < TS_PACKWITHPCR_SIZE)
	{
		return;
	}
	CAutoLock lock(&m_crtCache);
	CAutoLock lock2(&m_crtCacheExchange);



	int nNextLen =TS_PACKWITHPCR_SIZE;
	char* pDes = m_deqCacheExchange.pszbuff;
	while(nNextLen > 0 && (m_deqCacheExchange.m_nMaxSize- m_deqCacheExchange.m_nUsed) >= nNextLen)
	{

		m_deqCache.ReadBuff(pDes, nNextLen);
		pDes += nNextLen;
		m_deqCacheExchange.m_nUsed += nNextLen;
		m_deqCacheExchange.m_nCurrWritePos += nNextLen;
		nNextLen = m_deqCache.m_nUsed >=TS_PACKWITHPCR_SIZE? TS_PACKWITHPCR_SIZE:0;
	}
	if (bReleaseWrite)
	{
		//_RPT0(0,"\release write");
		ReleaseSemaphore(m_lockCacheWrite, 1,0);
	}
}


BOOL CMBCEndObj::ResetCache( int nCacheSize, int nExchangeCacheSize)
{
	CAutoLock lock(&m_crtCache);
	CAutoLock lock2(&m_crtCacheExchange);
	m_deqCache.Init(nCacheSize);
	m_deqCacheExchange.Init(nCacheSize);

	return TRUE;
}

void CMBCEndObj::CacheExSendProc()
{
	while(WaitForSingleObject(m_hFirstReadSignal, 100)!= WAIT_OBJECT_0)
	{
		if (!m_bRuning)
		{
			return;
		}
	}
	LARGE_INTEGER nFrequency ;
	QueryPerformanceFrequency(&nFrequency);
	LARGE_INTEGER nPfTmp ;
	QueryPerformanceCounter(&nPfTmp);

	char strDbg[1024];
	int nTickcount = GetTickCount();
	LARGE_INTEGER npfTickcount = nPfTmp;
	UINT64 nSendPackCount = 0;
	int64_t nPfReference = 0;
	int64_t nPCRReference = 0;

	int64_t nPfPre= 0;
	int64_t nPCRPre = 0;

	int64_t nSendPCRDeta = 0;
	int64_t nSndDetaOnce = 0;

	int64_t nCmpPCRDlta = 0;
	int64_t nCmpPfDlta =0;
	int64_t npfDeta = 0;
	int64_t nPfNow = 0;
	int64_t nSleepTime = 0;
	m_deqCacheExchange.Clear();

	char* p = NULL;
	int64_t* pnPCRTm = 0;
	BOOL bForceSend = FALSE;
	while(TRUE)
	{
		if (!m_bRuning)
		{
			return;
		}
		if (m_deqCacheExchange.m_nUsed== 0)
		{
			CopyExchangeData();
		}

		if (m_deqCacheExchange.m_nUsed == 0)
		{
			if (!m_bRuning)
			{
				return;
			}
			OutputDebugString("stream down-- nodata to send");
			m_bSendFirstReadSignal = TRUE;
			while(WaitForSingleObject(m_hFirstReadSignal, 100)!= WAIT_OBJECT_0)
			{
				if (!m_bRuning)
				{
					return;
				}
			}
			continue;
		}
		CAutoLock lock(&m_crtCacheExchange);

		p = m_deqCacheExchange.pszbuff;
		p += m_deqCacheExchange.m_nCurrPos;
		pnPCRTm = (int64_t*)p;
		p+= 8;


		if (*pnPCRTm > 0)
		{
			//
			if (nPCRReference == 0)
			{
				//first pcr
				RelayData(p, TS_PACKET_SIZE);
				m_deqCacheExchange.m_nCurrPos +=TS_PACKWITHPCR_SIZE;
				m_deqCacheExchange.m_nUsed -= TS_PACKWITHPCR_SIZE;
				nPCRReference = *pnPCRTm;
				QueryPerformanceCounter(&nPfTmp);
				nPfReference = nPfTmp.QuadPart*1000000/nFrequency.QuadPart;
				nPCRPre = *pnPCRTm;
				nPfPre = nPfReference;
				nSendPackCount++;

			}
			else
			{
				QueryPerformanceCounter(&nPfTmp);
				nPfNow = nPfTmp.QuadPart*1000000/(nFrequency.QuadPart);
				npfDeta =  nPfNow - nPfReference;
				nSendPCRDeta = (*pnPCRTm - nPCRReference)/TOMILLSENDPARM_TM;
				nSndDetaOnce =nSendPCRDeta - npfDeta;

				bForceSend = FALSE;
				nCmpPCRDlta = (*pnPCRTm - nPCRPre)/TOMILLSENDPARM_TM;
				nCmpPfDlta = nPfNow - nPfPre;
				if (nCmpPfDlta > 200000 || nCmpPfDlta < 0)
				{
					//__asm{int 3};
					bForceSend = TRUE;
					sprintf(strDbg,"****found pfDeta error = %I64d", npfDeta);
					OutputDebugString(strDbg);
				}
				if (nCmpPCRDlta >1000000 || nCmpPCRDlta < 0)
				{
					//__asm{int 3};
					bForceSend = TRUE;
					sprintf(strDbg, "****found PCRDeta error = %I64d", nSendPCRDeta);
					OutputDebugString(strDbg);
				}

				if (nSndDetaOnce < 10 || bForceSend)
				{
					//send 
					//_RPT3(0, "\n +++pcr send = %I64d, deta = %I64d, presend = %d", *pnPCRTm,nSendPCRDeta - npfDeta, nSendPackCount);
					RelayData(p, TS_PACKET_SIZE);
					m_deqCacheExchange.m_nCurrPos +=TS_PACKWITHPCR_SIZE;
					m_deqCacheExchange.m_nUsed -= TS_PACKWITHPCR_SIZE;
					nSendPackCount++;

					nPCRPre = *pnPCRTm;
					nPfPre = nPfNow;

					if (bForceSend)
					{
						//reset pcr reference pos
						nPCRReference = *pnPCRTm;
						QueryPerformanceCounter(&nPfTmp);
						nPfReference = nPfTmp.QuadPart*1000000/nFrequency.QuadPart;
					}
				}
				else
				{
					//wait
					//_RPT1(0, "\n deta = %d", nSendPCRDeta);
					nSleepTime = nSndDetaOnce/1000;
					if (nSleepTime > 1 )
					{
						Sleep(nSleepTime-1);
					}
				}
			}

		}
		else
		{
			//_RPT0(0, "\n ---nopcrsend");
			RelayData(p, TS_PACKET_SIZE);
			m_deqCacheExchange.m_nCurrPos +=TS_PACKWITHPCR_SIZE;
			m_deqCacheExchange.m_nUsed -= TS_PACKWITHPCR_SIZE;
			nSendPackCount++;
		}

		// 		if (nSendPackCount == 1742*10)
		// 		{
		// // 			QueryPerformanceCounter(&nPfTmp);
		// // 			int64_t ncoutpf = (nPfTmp.QuadPart- npfTickcount.QuadPart)*1000/(nFrequency.QuadPart);
		// // 			char szbuff[1024];
		// // 			sprintf(szbuff, "\nout 25mbit in 1s, pftime= %I64d, usetime = %dms", ncoutpf, GetTickCount() - nTickcount);
		// // 			OutputDebugString(szbuff);
		// // 			QueryPerformanceCounter(&npfTickcount);
		// // 			nTickcount = GetTickCount();
		// 			nSendPackCount = 0;
		// 		}
	}
}



CMBCEndObj::CMBCEndObj(ENUM_MBCOBJTYPE type)
{
	m_nObjType = type;
	m_hCalbitrateThd = NULL;
	m_hRelayProcThd = NULL;
	m_nRecvByte = 0;
	m_vRelaySocks.reserve(10);
	m_vRecvAddrs.reserve(5);
	m_hRelaySphone = CreateSemaphore(0, 0, 1, NULL);
	m_hUDPLoopThd = NULL;
	m_hFirstReadSignal = NULL;
	m_bRuning = FALSE;
	if (m_nObjType == MBCOBJTYPE_ENDBACK)
	{
		ResetCache(MAXSENDCACHE, MAXEXCHANGECACHE);
		m_lockCacheRead = CreateSemaphore(NULL, 0,1, NULL);
		m_lockCacheWrite = CreateSemaphore(NULL, 0,1, NULL);
		m_hFirstReadSignal = CreateSemaphore(NULL, 0, 1, NULL);
	}
}

CMBCEndObj::~CMBCEndObj(void)
{
	ResetCache(0,0);
	if (m_hRelaySphone)
	{
		CloseHandle(m_hRelaySphone);
		m_hRelaySphone = NULL;
	}
	
	if (m_hFirstReadSignal)
	{
		CloseHandle(m_hFirstReadSignal);
		m_hFirstReadSignal = NULL;
	}
}

HRESULT CMBCEndObj::Run()
{
	HRESULT hr = S_OK;
	Stop();

	MCBSOCKADDRS addrs;
	addrs.addrLocal.sin_family = AF_INET;
	addrs.addrLocal.sin_port = htons(0);
	addrs.addrLocal.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrs.addrRemote = m_addrRemote;
	m_pSockBase = CMBCSocket::CreateUDPSocket(addrs, FD_READ|FD_CLOSE|FD_CONNECT, this, MBCSOCKTYPE_UDP_MBC_RECVER|MBCSOCKTYPE_AUTORECONNECT);
	if (m_pSockBase == NULL)
	{
		ASSERT(FALSE);
		hr =  E_FAIL;
	}
	StartCalcBitrate();
	m_nSelfState = MBCSTATE_OK;
#ifdef _DEBUG
	if (m_nObjType == MBCOBJTYPE_ENDBACK
		||m_nObjType == MBCOBJTYPE_ENDMASTER)
	{
// 		CString strName;
// 		strName.Format("f:\\outtest-%d-%s.ts", m_pSockBase->m_hSock, m_nObjType == MBCOBJTYPE_ENDBACK? "bak":"master");
// 		fileOut.Open(strName, CFile::modeCreate|CFile::modeWrite, NULL);
	}
#endif // _DEBUG
	StartUDPSockLoop();
	if (m_nObjType ==MBCOBJTYPE_ENDBACK)
	{
		StartRelayThd();
	}
	return CMBCBaseObj::Run();
}

HRESULT CMBCEndObj::Stop()
{
	m_bQuitUdpThread = TRUE;
	StopCalcBitrate();
	StopUDPSockLoop();
#ifdef _DEBUG
	if (fileOut.m_hFile != INVALID_HANDLE_VALUE)
	{
		fileOut.Close();
	}
#endif // _DEBUG
	if (m_nObjType ==MBCOBJTYPE_ENDBACK)
	{
		ClearRelayAddrs();
		StopRelayThd();

	}
	//StopRelayThd();
	
	
	return CMBCBaseObj::Stop();
}

BOOL CMBCEndObj::AddRelayAddr( SOCKADDR_IN& addrIn )
{
	BOOL bFind = FALSE;
	CAutoLock lock(&m_lockrelaysock);
	for (size_t i = 0; i < m_vRelaySocks.size(); ++i)
	{
		CMBCSocket* pSock = m_vRelaySocks[i];
		if (pSock && IsEqualAddr(pSock->m_addrs.addrRemote, addrIn))
		{
			bFind = TRUE;
			break;
		}
	}

	if (bFind)
	{
		return TRUE;
	}
	else
	{
		MCBSOCKADDRS addrs;
		addrs.addrLocal.sin_family = AF_INET;
		addrs.addrLocal.sin_port = htons(0);
		addrs.addrLocal.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		addrs.addrRemote = addrIn;
		CMBCSocket* pSockCtrl = CMBCSocket::CreateUDPSocket(addrs, FD_CLOSE|FD_CONNECT, this, MBCSOCKTYPE_UDP_MBC_SENDER|MBCSOCKTYPE_AUTORECONNECT);
		if (pSockCtrl == NULL)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		m_vRelaySocks.push_back(pSockCtrl);
		if (m_vRelaySocks.size() ==1)
		{
			//start relay proc
			//StartRelayThd();

		}
	}

	return TRUE;
}


BOOL CMBCEndObj::RemoveRelayAddr( SOCKADDR_IN& addrIn )
{
	CAutoLock lock(&m_lockrelaysock);
	BOOL bFind = FALSE;
	CMBCSocket* pSock = NULL;
	VRELAYSOCKS::iterator itb = m_vRelaySocks.begin();
	VRELAYSOCKS::iterator ite = m_vRelaySocks.end();

	for (; itb != ite; ++itb)
	{
		CMBCSocket* pSock = *itb;
		if (pSock && IsEqualAddr(pSock->m_addrs.addrRemote, addrIn))
		{
			bFind = TRUE;
			m_vRelaySocks.erase(itb);
			CMBCSocket::ReleaseSock(pSock);
			break;
		}
	}

	if (m_vRelaySocks.size() == 0 && m_hRelayProcThd)
	{
		StopRelayThd();
	}

	return bFind;
}

HRESULT CMBCEndObj::GetStateInfo( ST_OBJSTATEINFO& infoOut )
{
	CAutoLock lock(&m_lockrelaysock);
	infoOut.addrLocal = m_addrLocal;
	infoOut.addrRemote = m_addrRemote;
	infoOut.remoteInfo.vCHInfo.clear();
	infoOut.nSelfState = m_nSelfState;
	infoOut.nObjType = m_nObjType;
	infoOut.strName = m_strObjName;
	infoOut.strChId = m_strCHID;
	infoOut.remoteInfo.nRemoteState = m_nSelfState;
	infoOut.remoteInfo.vCHInfo.clear();
	if (m_nObjType != MBCOBJTYPE_ENDBACK &&  m_nSelfState != MBCSTATE_OK)
	{
		return S_OK;
	}
	for (size_t i = 0; i < m_vRelaySocks.size(); ++i)
	{
		CMBCSocket* pSock = m_vRelaySocks[i];
		if (pSock )
		{
			ST_MBCCHANNELINFO chinfo;
			chinfo.addrDest = pSock->m_addrs.addrRemote; 
			infoOut.remoteInfo.vCHInfo.push_back(chinfo);
		}
	}
	
	return S_OK;
}

HRESULT CMBCEndObj::CheckBitrate()
{
	if (m_nBitRate == 0)
	{
		++m_nStreamDownCount;
		if (m_nStreamDownCount > 0
			&& m_nSelfState == MBCSTATE_OK)
		{
			ChangeSelfState(MBCSTATE_STREAMDOWN);
			CFWriteLog(TEXT("notify:stream down at %s"), Addr2String(m_addrRemote).c_str());
			ClearRelayAddrs();
		}
	}
	else
	{
		ChangeSelfState(MBCSTATE_OK);
		m_nStreamDownCount = 0;
	}

	return S_OK;
}

HRESULT CMBCEndObj::StartCalcBitrate()
{
	StopCalcBitrate();
	m_bQuitCalcBitrateThread = FALSE;
	m_hCalbitrateThd = CreateThread(NULL, 0, CalcBitrateProc, (LPVOID)this, 0,0);
	return S_OK;
}

HRESULT CMBCEndObj::StopCalcBitrate()
{
	m_nStreamDownCount = 0;
	m_nBitRate = 0;
	m_nRecvByte = 0;
	m_bQuitCalcBitrateThread = TRUE;
	if (m_hCalbitrateThd)
	{
		WaitForSingleObject(m_hCalbitrateThd, INFINITE);
		CloseHandle(m_hCalbitrateThd);
		m_hCalbitrateThd = NULL;
	}

	return S_OK;
}

BOOL CMBCEndObj::AddRecvAddrTracer( SOCKADDR_IN& addrIn )
{
	CAutoLock lock(&m_lockRecvAddrs);
	VECRECVSRCADDRS::iterator itf = std::find(m_vRecvAddrs.begin(), m_vRecvAddrs.end(), addrIn);
	if (itf == m_vRecvAddrs.end())
	{
		m_vRecvAddrs.push_back(addrIn);
	}

	if (m_vRecvAddrs.size() >1  && m_nObjType == MBCOBJTYPE_ENDMASTER)
	{
		//ASSERT(FALSE);
	}
	return TRUE;
}

BOOL CMBCEndObj::ClearRecvAddrTracer()
{
	CAutoLock lock(&m_lockRecvAddrs);
	m_vRecvAddrs.clear();

	return TRUE;
}

HRESULT CMBCEndObj::RelayData( const char* buffer, int nLen )
{
	if (buffer == NULL || nLen <= 0)
	{
		return E_FAIL;
	}

	char* p = m_frameSendbuff.szbuff;
	p += m_frameSendbuff.nUsed;
	m_frameSendbuff.nUsed += nLen;
	memcpy(p, buffer, nLen);
	if (m_frameSendbuff.nUsed <TS_FRAME_SIZE )
	{
		return S_OK;
	}

	if (m_vRelaySocks.size() == 0)
	{
		m_frameSendbuff.nUsed = 0;
		return S_OK;
	}

	
	CAutoLock lock(&m_lockrelaysock);
	CMBCSocket* pRelaySock = NULL;
	for (size_t i = 0 ; i < m_vRelaySocks.size(); ++i)
	{
			pRelaySock = m_vRelaySocks[i];
			if (pRelaySock != NULL)
			{
				HRESULT hr = sendto(*pRelaySock, m_frameSendbuff.szbuff, TS_FRAME_SIZE, 0, (sockaddr*)&(pRelaySock->m_addrs.addrRemote), sizeof(SOCKADDR_IN));
// 				if(hr == SOCKET_ERROR)
// 				{
// 					TRACE("\nrelay err = %d", WSAGetLastError());
// 				}
			}
	}
	
	m_frameSendbuff.nUsed = 0;

	return S_OK;
}

BOOL CMBCEndObj::ClearRelayAddrs()
{
	CAutoLock lock(&m_lockrelaysock);
	BOOL bFind = FALSE;
	CMBCSocket* pSock = NULL;
	VRELAYSOCKS::iterator itb = m_vRelaySocks.begin();
	VRELAYSOCKS::iterator ite = m_vRelaySocks.end();

	for (; itb != ite; ++itb)
	{
		CMBCSocket* pSock = *itb;
		if (pSock)
		{
			CMBCSocket::ReleaseSock(pSock);
		}
	}
	m_vRelaySocks.clear();

	return bFind;
}

BOOL CMBCEndObj::GetRecvAddrs( VECRECVSRCADDRS& vaddrsOut )
{
	CAutoLock lock(&m_lockRecvAddrs);
	vaddrsOut = m_vRecvAddrs;
	return TRUE;
}

int CMBCEndObj::IsLocalAddrOfRelaySock( SOCKADDR_IN& addrRelayTo, SOCKADDR_IN& addrLocal )
{
	CAutoLock lock(&m_lockrelaysock);
	int bFind = 0;
	CMBCSocket* pSock = NULL;
	VRELAYSOCKS::iterator itb = m_vRelaySocks.begin();
	VRELAYSOCKS::iterator ite = m_vRelaySocks.end();

	for (; itb != ite; ++itb)
	{
		CMBCSocket* pSock = *itb;
		if (pSock && addrRelayTo == pSock->m_addrs.addrRemote)
		{
			bFind = 1;

			char szCSBuff[256];
			CSADDR_INFO* pcsInfo = (CSADDR_INFO*)szCSBuff;
			int noptLen = 256;
			HRESULT hRet = getsockopt(*pSock, SOL_SOCKET, SO_BSP_STATE, (char*)pcsInfo, &noptLen);
			SOCKADDR_IN* paddrLocal =NULL;
			if (hRet != SOCKET_ERROR)
			{
				ASSERT(pcsInfo->LocalAddr.iSockaddrLength > 0);
				paddrLocal = (SOCKADDR_IN*)pcsInfo->LocalAddr.lpSockaddr;
			}
			else
			{
				paddrLocal =  &pSock->m_addrs.addrLocal;
			}
			if (*paddrLocal == addrLocal)
			{
				bFind = 2;
			}
			break;
		}
	}

	return bFind;
}

HRESULT CMBCEndObj::StartRelayThd()
{
	m_deqCache.Clear();
	m_deqCacheExchange.Clear();
	ReleaseSemaphore(m_lockCacheRead, 1, NULL);
	WaitForSingleObject(m_lockCacheRead, 1);
	ReleaseSemaphore(m_lockCacheWrite, 1, NULL);
	WaitForSingleObject(m_lockCacheWrite, 1);
	WaitForSingleObject(m_hFirstReadSignal, 1);
	m_frameSendbuff.nUsed = 0;
	m_bQuitRelayThread = FALSE;
	m_bSendFirstReadSignal = TRUE;
	m_bRuning = TRUE;
	m_hRelayProcThd = CreateThread(NULL, 0, RelayDataProc, (LPVOID)this,  0, 0);
	ASSERT(m_hRelayProcThd != NULL);
	ASSERT(SetThreadPriority(m_hRelayProcThd, THREAD_PRIORITY_HIGHEST));

	return S_OK;
}

HRESULT CMBCEndObj::StopRelayThd()
{
	m_bQuitRelayThread = TRUE;
	if (m_hRelayProcThd)
	{
		m_bRuning = FALSE;
		WaitForSingleObject(m_hRelayProcThd, 10000);
		CloseHandle(m_hRelayProcThd);
		m_hRelayProcThd = NULL;
		//m_crtCache.ResetLock();
		//m_crtCacheExchange.ResetLock();
	}
	return S_OK;
}

HRESULT CMBCEndObj::StartUDPSockLoop()
{
	StopUDPSockLoop();
	m_bQuitUdpThread = FALSE;
	m_hUDPLoopThd = CreateThread(NULL, 0, UDPSockLoopProc, (LPVOID)this,  0, 0);
	ASSERT(m_hUDPLoopThd != NULL);
	ASSERT(SetThreadPriority(m_hUDPLoopThd, THREAD_PRIORITY_TIME_CRITICAL));
	return S_OK;
}

HRESULT CMBCEndObj::StopUDPSockLoop()
{
	m_bQuitUdpThread = TRUE;
	if ( m_hUDPLoopThd)
	{
		WaitForSingleObject(m_hUDPLoopThd, 100);
		if (m_bQuitUdpThread)
		{
			TerminateThread(m_hUDPLoopThd, 0);
			m_bQuitUdpThread= FALSE;
		}
		CloseHandle(m_hUDPLoopThd);
		m_hUDPLoopThd = NULL;
	}

	return S_OK;
}

