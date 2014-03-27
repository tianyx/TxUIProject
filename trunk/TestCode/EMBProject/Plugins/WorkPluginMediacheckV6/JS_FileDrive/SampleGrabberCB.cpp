// SampleGrabberCB.cpp: implementation of the CSampleGrabberCB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SampleGrabberCB.h"
#include <stdio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSampleGrabberCB::CSampleGrabberCB()
{
	m_GraberSampleCounts = 0;
}

CSampleGrabberCB::~CSampleGrabberCB()
{
}

// Support querying for ISampleGrabberCB interface
HRESULT CSampleGrabberCB::QueryInterface(REFIID iid, void**ppv)
{
	if (NULL == ppv) return E_POINTER;
	if (iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(this);
		return S_OK;
	}
	if (iid == __uuidof(ISampleGrabberCB))
	{
		*ppv = static_cast<ISampleGrabberCB*>(this);
		return S_OK;
	}
	return E_NOINTERFACE;
}
STDMETHODIMP CSampleGrabberCB::BufferCB(double SampleTime, BYTE *pBuffer, long nBufferLen)
{

	HRESULT hr = S_OK;
	MyMediaSample *MS = NULL;
	///////////////////////////////////////////////////
// 	if(m_GraberSampleCounts > 0)
// 	{
//         CString strPath;
// 		strPath.Format("C:\\BMP\\%ld.Dat",m_GraberSampleCounts);
// 		FILE* fp=fopen(strPath, "wb");
// 		if (fp!=NULL)
// 		{
// 			fwrite(pBuffer, nBufferLen, 1, fp);
// 			fclose(fp);
// 		}
// 		BITMAPINFOHEADER BIH;
// 		BIH.biBitCount = 24;
// 		BIH.biClrImportant = 0;
// 		BIH.biClrUsed = 0;
// 		BIH.biCompression = 0;
// 		BIH.biHeight = 576;
// 		BIH.biPlanes = 0;
// 		BIH.biSize = sizeof(BITMAPINFOHEADER);
// 		BIH.biSizeImage = 736*576*3;
// 		BIH.biWidth = 736;
// 		BIH.biXPelsPerMeter = 0;
// 		BIH.biYPelsPerMeter = 0;
// 		BYTE* p = YUY22BMP(pBuffer,736,576);
// 		strPath.Format("C:\\BMP\\%ld.bmp",m_GraberSampleCounts);
// 		SaveAsBmp(p,strPath,&BIH);
// 		delete []p;
// 	}
// 	m_GraberSampleCounts++;

	/////////////////////////////////////////////////////
//	m_CritSec.Lock();
//	TRACE1("Grabber:%d\n",m_GraberSampleCounts);
	MS = m_pSampleList->GetEmptyObject();
	if(MS)
	{
		hr = MS->SetData(pBuffer,nBufferLen);
        if (SUCCEEDED(hr))
		{
			MS->SetTime(SampleTime);
		    m_pSampleList->GetEmptyObjectComplete();
		}
// 		m_GraberSampleCounts++;
// 		CString strLog = "";
// 		CString strTime;
// 		SYSTEMTIME systemtime;
// 		GetLocalTime(&systemtime);
// 		strTime.Format("CurrentTime = %d-%d-%d %d:%d:%d:%d",systemtime.wYear,systemtime.wMonth,systemtime.wDay,systemtime.wHour,systemtime.wMinute,systemtime.wSecond,systemtime.wMilliseconds);
// 		strLog.Format("TestKKK: Decode: %s nFrame = %d",strTime,m_GraberSampleCounts - 1);
// 		OutputDebugString(strLog);
	}
	MS = NULL;
	
//	m_CritSec.Unlock();

	return hr;
}
STDMETHODIMP CSampleGrabberCB::SampleCB(double SampleTime, IMediaSample *pSample)
 { 
	HRESULT hr = S_OK;
	MyMediaSample *MS = NULL;
	long nBufferLen = pSample->GetActualDataLength();
	BYTE* pBuffer   = NULL;
	pSample->GetPointer(&pBuffer);

	/*//////////////////////////////////////////////////
	AM_MEDIA_TYPE* pAMT;
    pSample->GetMediaType(&pAMT);
	CString strPath;
	strPath.Format("C:\\BMP\\%ld.Dat",m_GraberSampleCounts);
	FILE* fp=fopen(strPath, "wb");
    if (fp!=NULL)
    {
        fwrite(pBuffer, nBufferLen, 1, fp);
        fclose(fp);
    }

	BITMAPINFOHEADER BIH;
	BIH.biBitCount = 24;
	BIH.biClrImportant = 0;
	BIH.biClrUsed = 0;
	BIH.biCompression = 0;
	BIH.biHeight = 576;
	BIH.biPlanes = 0;
	BIH.biSize = sizeof(BITMAPINFOHEADER);
	BIH.biSizeImage = 736*576*3;
	BIH.biWidth = 736;
	BIH.biXPelsPerMeter = 0;
	BIH.biYPelsPerMeter = 0;
	BYTE* p = YUY22BMP(pBuffer,736,576);
	strPath.Format("C:\\BMP\\%ld.bmp",m_GraberSampleCounts);
	SaveAsBmp(p,strPath,&BIH);
	delete []p;
	m_GraberSampleCounts++;
	*/////////////////////////////////////////////////////

    MS = m_pSampleList->GetEmptyObject();
	if(MS)
	{
		MS->SetData(pBuffer,nBufferLen);
		MS->SetTime(SampleTime);
		m_pSampleList->GetEmptyObjectComplete();
	}
	MS = NULL;
	return hr;
 }

HRESULT CSampleGrabberCB::SetBuffer(void *pA)
{
//	if(!IsBadReadPtr(pA,sizeof(CSList)))
	if(!pA)
	{
		return E_FAIL;
	}
	m_pSampleList =(CSList*)pA;
	return S_OK;
}

BYTE* CSampleGrabberCB::YUY22BMP(BYTE* pBuffer,DWORD Width,DWORD Height)
{
	DWORD* p=(DWORD*)pBuffer;
	BYTE cY,cU,cV,cR,cG,cB;
	BYTE* ppLine=new BYTE[Height*Width*3];
	DWORD stride = Width*3;
	long uLine=0;
	for (uLine=0;uLine<(long)Height;uLine++)
	{
		for (WORD uPixel=0;uPixel<stride;uPixel+=6)
		{	
			cY=BYTE(*p&0x000000FF);
			cU=BYTE((*p&0x0000FF00)>>8);
			cV=BYTE((*p&0xFF000000)>>24);
			ConverToRGB24(cY,cU,cV,&cR,&cG,&cB);
			ppLine[uLine*stride+uPixel  ]=cB;
			ppLine[uLine*stride+uPixel+1]=cG;
			ppLine[uLine*stride+uPixel+2]=cR;
			
			cY=BYTE((*p&0x00FF0000)>>16);
			ConverToRGB24(cY,cU,cV,&cR,&cG,&cB);
			
			ppLine[uLine*stride+uPixel+3]=cB;
			ppLine[uLine*stride+uPixel+4]=cG;
			ppLine[uLine*stride+uPixel+5]=cR;
			p++;
		}
	}
	return ppLine;
}
HBITMAP CSampleGrabberCB::YV122BMP(BYTE* pBuffer,DWORD Width,DWORD Height)
{
/*	BITMAPV4HEADER* dibInfo = (BITMAPV4HEADER *)new BITMAPV4HEADER;
	dibInfo->bV4Size = sizeof(BITMAPV4HEADER);
	dibInfo->bV4Width = Width;
	dibInfo->bV4Height = Height;
	dibInfo->bV4Planes = color_planes;
	dibInfo->bV4BitCount = (WORD) bit_per_pixel *color_planes;
	dibInfo->bV4V4Compression = BI_RGB;
	
	BYTE* pByte;
	HBITMAP hBitmap = ::CreateDIBSection (NULL, (BITMAPINFO *)dibInfo, DIB_RGB_COLORS,
		(VOID **) (&pByte),NULL, 0);
	if(hBitmap ==NULL) MessageBox("error!","hBitmap Create",MB_OK);
	
	WORD* p_y=(WORD*)pBuffer;
	BYTE*  p_v = pBuffer+Height*Width;
	BYTE*  p_u = p_v+ Height*Width/4;
	BYTE cY,cU,cV,cR,cG,cB;

	BYTE** ppLine=new BYTE*[Height];
	DWORD stride = Width*bit_per_pixel/8;
	long uLine=0;

	for (uLine=Height-1;uLine>=0;uLine--)
	{
		ppLine[uLine]=pByte+(stride*(Height-uLine-1));
	}
	DWORD key = 0;
	for (uLine=0;uLine<(long)Height;uLine++)
	{
		for (WORD uPixel=0;uPixel<stride;uPixel+=6)
		{
			cY=BYTE( *p_y&0x00ff);
			cU=*p_u;
			cV=*p_v;
			ConverToRGB24(cY,cU,cV,&cR,&cG,&cB);
			ppLine[uLine][uPixel  ]=cB;
			ppLine[uLine][uPixel+1]=cG;
			ppLine[uLine][uPixel+2]=cR;			
			cY=BYTE((*p_y&0xff00)>>8);
			ConverToRGB24(cY,cU,cV,&cR,&cG,&cB);			
			ppLine[uLine][uPixel+3]=cB;
			ppLine[uLine][uPixel+4]=cG;
			ppLine[uLine][uPixel+5]=cR;
			p_y++;
			p_u++;
			p_v++;
		}	
		if (key%2==0)
		{
			p_u-=Width/2;
			p_v-=Width/2;
		}
		key++;
	}
	delete[] pBuffer;
	delete[] ppLine;
	return hBitmap;
	*/
	return NULL;

}
void CSampleGrabberCB::ConverToRGB24(BYTE Y,BYTE U,BYTE V,BYTE* R,BYTE* G,BYTE* B)
{	
	long E_R=0,E_G=0,E_B=0,E_Y= Y,E_U=U,E_V = V;
				
				
	E_B = ((E_Y - 16) * 76284 + (E_U - 128) * 132252);
	E_G = ((E_Y - 16) * 76284 - (E_V - 128) *  53281 - (E_U-128)*25625 );
	E_R = ((E_Y - 16) * 76284 + (E_V - 128) * 104595);
				
	E_B=E_B<0 ? 0 : E_B;
	E_G=E_G<0 ? 0 : E_G;
	E_R=E_R<0 ? 0 : E_R;
				
	*R = BYTE(E_R >> 16);
	*G = BYTE(E_G >> 16);
	*B = BYTE(E_B >> 16);
}

//±£´æ³Ébmp
BOOL CSampleGrabberCB::SaveAsBmp(LPBYTE pDIB,CString strPath,BITMAPINFOHEADER *BIH)
{
	ASSERT(pDIB!=NULL);
	
	BITMAPINFOHEADER bih;
	BYTE* Bits=NULL;
	if(BIH == NULL)
	{
		RtlMoveMemory(&bih.biSize, pDIB, sizeof(BITMAPINFOHEADER));
		Bits=new BYTE[bih.biSizeImage];
		RtlMoveMemory(Bits, pDIB + sizeof(BITMAPINFOHEADER), bih.biSizeImage);
		
	}
	else
	{
		RtlMoveMemory(&bih.biSize, BIH, sizeof(BITMAPINFOHEADER));
		Bits=new BYTE[bih.biSizeImage];
		RtlMoveMemory(Bits, pDIB , bih.biSizeImage);
	}
	//now get the bitmap bits
    if (bih.biSizeImage < 1)
	{
		AfxMessageBox("BITMAPINFOHEADER.biSizeImage=0",MB_OK);
        return FALSE;
	}
    //and BitmapInfo variable-length UDT
    BYTE memBitmapInfo[40];
    RtlMoveMemory(memBitmapInfo, &bih, sizeof(bih));
	
    BITMAPFILEHEADER bfh;
    bfh.bfType=19778;    //BM header
    bfh.bfSize=55 + bih.biSizeImage;
    bfh.bfReserved1=0;
    bfh.bfReserved2=0;
    bfh.bfOffBits=sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER); //54
    
   
    FILE* fp=fopen(strPath, "wb");
    if (fp!=NULL)
    {
        fwrite(&bfh, sizeof(bfh), 1, fp);
        fwrite(&memBitmapInfo, sizeof(memBitmapInfo), 1, fp);
        fwrite(Bits, bih.biSizeImage, 1, fp);
        fclose(fp);
    }
    else
    {
        TRACE0(_T("Error writing the bitmap file"));
        return FALSE;
    }
    delete []Bits;
    return TRUE;
}