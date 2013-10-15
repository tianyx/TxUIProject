#pragma once

/*
方法：1增加类-> 使用mfc类->typelib 中的mfc类
#import "msxml4.dll" rename_namespace("ADXML")
会向工程内增加封装好的头文件.

方法：2或者直接导入dll文件
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

