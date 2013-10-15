#pragma once

/*
������1������-> ʹ��mfc��->typelib �е�mfc��
#import "msxml4.dll" rename_namespace("ADXML")
���򹤳������ӷ�װ�õ�ͷ�ļ�.

������2����ֱ�ӵ���dll�ļ�
#import "msxml4.dll" rename_namespace("ADXML")
ADXML::IXMLDOMDocumentPtr* pDoc = NULL;
HRESULT hr = CoCreateInstance(__uuidof(ADXML::DOMDocument), NULL, CLSCTX_INPROC_SERVER, __uuidof(ADXML::IXMLDOMDocument), reinterpret_cast<void**>(&pDoc));
if (hr != S_OK)
{
ASSERT(FALSE);
}

*/

#include <map>
#include <vector>
using namespace std;

struct ST_ADITEMDATA
{
	int nIdx;
	CString ProgramName;
	int nSom;
	int nDuration;
	int nClipType;
	int nTCType;
};

typedef map<int, ST_ADITEMDATA> MAPADITEM;

struct ST_ADPACKDATA
{
	CString strSegmentName;
	CString strCreater;
	MAPADITEM mapAdItem;
};

typedef vector<ST_ADPACKDATA> VECADPACKDATA;


struct ST_ADPACK
{
	CString strDate;
	CString strPkTypeName; //PackageTypeName
	CString strChName;		//ChannelName

	VECADPACKDATA vecAdPackData;
};




class CADXmlLoader
{
public:
	CADXmlLoader(void);
	~CADXmlLoader(void);

	BOOL LoadADPackListFromFile(CString& strXmlFileIn, ST_ADPACK& dataOut);
private:
};

