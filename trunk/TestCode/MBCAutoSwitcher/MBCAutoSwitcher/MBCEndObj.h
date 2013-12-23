/********************************************************************
	created:	2013/08/02
	created:	2013:8
	filename: 	MBCEndObj.h
	file base:	MBCEndObj
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once
#include "mbcbaseobj.h"
#include "AutoCritSec.h"
#include "TxTimer.h"

#define MAX_RELAYBUFF 1316*1000
#define MAX_RECVBUFF 1316*100

#define TS_PACKET_SIZE			(188)
#define TS_PACKWITHPCR_SIZE (196)
#define TS_FRAME_SIZE (TS_PACKET_SIZE * 7)
#define MAXEXCHANGECACHE 0x27FF78  //2.5m
#define MAXSENDCACHE (MAXEXCHANGECACHE*10)  //10m

struct ST_FRAMESENDBUFF
{
	char szbuff[TS_FRAME_SIZE];
	int nUsed;
	ST_FRAMESENDBUFF()
	{
		nUsed = 0;
	}
};

struct ST_TSSENDCACHE 
{
	char* pszbuff;  //total frame 0x16DB6D
	int m_nUsed;
	int m_nCurrPos;
	int m_nCurrWritePos;
	int m_nMaxSize;
	ST_TSSENDCACHE(int nMaxSizeIn =MAXSENDCACHE)
	{
		pszbuff = 0;
		Init(nMaxSizeIn);
	}
	void Clear()
	{
		m_nUsed = 0;
		m_nCurrPos = 0;
		m_nCurrWritePos = 0;

	}
	BOOL Init(int nMaxSizeIn)
	{
		Clear();
		if (pszbuff)
		{
			delete pszbuff;
			pszbuff = NULL;
			m_nMaxSize = 0;
		}
		m_nMaxSize = nMaxSizeIn;
		if (m_nMaxSize > 0)
		{
			pszbuff = new char[m_nMaxSize];
			ASSERT(pszbuff);
		}
		return pszbuff != NULL;
	}

	int NextPackSize()
	{
		if (m_nUsed < 4)
		{
			return 0;
		}
		int nNextLen = 0;
		char* p = (char*)&nNextLen;
		char* pSrc = pszbuff;
		pSrc += m_nCurrPos;
		for (int i = 0; i < 4; ++i)
		{
			*p = *pSrc;
			if (m_nCurrPos +i+1 == m_nMaxSize)
			{
				pSrc = pszbuff;
			}
			else
			{
				++pSrc;
			}
			++p;
		}

		return nNextLen;
	}

	BOOL WriteBuff(char* pSrc, const int nLentoCopy)
	{
		if (m_nMaxSize - m_nUsed < nLentoCopy)
		{
			return FALSE;
		}
		int nEndRemain = m_nMaxSize - m_nCurrWritePos;
		if (nEndRemain < nLentoCopy)
		{

			char* pDes = pszbuff;
			pDes += m_nCurrWritePos;
			memcpy(pDes, pSrc, nEndRemain);
			pSrc += nEndRemain;
			pDes = pszbuff;
			memcpy(pDes, pSrc, nLentoCopy-nEndRemain);
			m_nCurrWritePos = nLentoCopy-nEndRemain;
		}
		else
		{
			char* pDes = pszbuff;
			pDes += m_nCurrWritePos;
			memcpy(pDes, pSrc, nLentoCopy);
			m_nCurrWritePos += nLentoCopy;
			if (m_nCurrWritePos == m_nMaxSize)
			{
				m_nCurrWritePos = 0;
			}
		}
		m_nUsed += nLentoCopy;

		return TRUE;
	}

	BOOL ReadBuff(char* pDes, const int nLenToCopy)
	{
		if (m_nUsed < nLenToCopy)
		{
			return FALSE;
		}
		int nEndRemain = m_nMaxSize - m_nCurrPos;
		char* pSrc = pszbuff;
		if (nEndRemain < nLenToCopy)
		{
			//len been devided.
			pSrc+= m_nCurrPos;
			char* pTmp = pDes;
			memcpy(pTmp, pSrc, nEndRemain);
			pTmp += nEndRemain;
			pSrc = pszbuff;
			memcpy(pTmp, pSrc, nLenToCopy-nEndRemain);
			m_nCurrPos = nLenToCopy-nEndRemain;
		}
		else
		{
			pSrc += m_nCurrPos;
			memcpy(pDes, pSrc, nLenToCopy);
			m_nCurrPos += nLenToCopy;
			if (m_nCurrPos >= m_nMaxSize)
			{
				m_nCurrPos = 0;
			}
		}
		m_nUsed -= nLenToCopy;
		return TRUE;
	}
};



typedef std::vector<CMBCSocket*> VRELAYSOCKS;
typedef std::vector<SOCKADDR_IN> VECRECVSRCADDRS;
typedef std::vector<SOCKADDR_IN> VECRELAYADDRS;
class CMBCEndObj :
	public CMBCBaseObj
{
public:
	CMBCEndObj(ENUM_MBCOBJTYPE type);
	virtual ~CMBCEndObj(void);

	CAutoCritSec m_lockrelaysock;
	CAutoCritSec m_lockRecvAddrs;

	CString m_strCHID;

	virtual HRESULT Run();
	virtual HRESULT Stop();

	HRESULT StartUDPSockLoop();
	HRESULT StopUDPSockLoop();

	HRESULT StartCalcBitrate();
	HRESULT StopCalcBitrate();
	HRESULT CheckBitrate();

	HRESULT StartRelayThd();
	HRESULT StopRelayThd();

	//for bitrate calc
	HANDLE m_hCalbitrateThd;
	HANDLE m_hRelayProcThd;
	HANDLE m_hRelaySphone;
	HANDLE m_hUDPLoopThd;
	int m_nStreamDownCount; //set error if >10
	INT64 m_nRecvByte;
	int  m_nBitRate;
	 
	VECRECVSRCADDRS m_vRecvAddrs;
	BOOL AddRecvAddrTracer(SOCKADDR_IN& addrIn);
	BOOL ClearRecvAddrTracer();
	BOOL GetRecvAddrs(VECRECVSRCADDRS& vaddrsOut);

	VRELAYSOCKS m_vRelaySocks;
	BOOL AddRelayAddr(SOCKADDR_IN& addrIn);
	BOOL RemoveRelayAddr(SOCKADDR_IN& addrIn);
	BOOL ClearRelayAddrs();
	BOOL GetRelayedAddrs(VECRELAYADDRS& vaddrOut);
	int IsLocalAddrOfRelaySock(SOCKADDR_IN& addrRelayTo, SOCKADDR_IN& addrLocal);

	BOOL CanRelayData();
	BOOL PreRelayData();
	HRESULT RelayData(const char* buffer, int nLen);
	void PutRelayBuff(char* buffIn, int nLen);
	void CopyCacheBuff(char* buffOut, const int nMaxLen, int& nLenCopyed);
	void CopyRelayBuff(char* buffOut, const int nMaxLen, int& nLenCopyed);

	virtual HRESULT	GetStateInfo(ST_OBJSTATEINFO& infoOut);

	BOOL ResetCache( int nCacheSize, int nExchangeCacheSize);
	void CacheExSendProc();
	void CopyExchangeData();
	void CopyCacheData(char* buffIn, const int nLenIn);

	CAutoCritSec m_crtCache;
	CAutoCritSec m_crtCacheExchange;

	HANDLE m_lockCacheRead;
	HANDLE m_lockCacheWrite;
	ST_TSSENDCACHE m_deqCache;
	ST_TSSENDCACHE m_deqCacheExchange;
	BOOL m_bQuitUdpThread;
	BOOL m_bQuitCalcBitrateThread;
	HANDLE m_hFirstReadSignal;
	ST_FRAMESENDBUFF m_frameSendbuff;
	BOOL m_bSendFirstReadSignal;
	BOOL m_bActiveRelay;
#ifdef _DEBUG
	CFile fileOut;
#endif // _DEBUG
};

