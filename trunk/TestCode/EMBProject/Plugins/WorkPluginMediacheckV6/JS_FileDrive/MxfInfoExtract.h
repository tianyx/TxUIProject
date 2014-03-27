#pragma once
#include "FileInfoExtract.h"
#include "JS_FileDrive.h"
////////////////////////////////////////////////////////////////////
//CMxfInfoExtract class设计目的是为了从MXF 文件中得到一些视频和音频信息以及文件比较.
//对MXF的操作是基于开源MXFLIB 1.0.0开发包实现的。
//wang jun-----fouder 2008.04.11
////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#pragma comment(lib, "mxflib_D.lib")
#else
#pragma comment(lib, "mxflib.lib")
#endif

#include "mxflib-1.0.0/mxflib/mxflib.h"
using namespace std;
using namespace mxflib;
class _JS_Fix CMxfInfoExtract:public CFileInfoExtract
{
public:
	bool GetVideoInfo(MDObjectPtr Desp);
	CMxfInfoExtract(void);
    virtual ~CMxfInfoExtract(void);
	virtual BOOL    AnalyzeFile();

protected:
	bool BuildEssenceInfo(mxflib::PartitionPtr &pPartition);
//	void DumpObject(MDObjectPtr Object, std::string Prefix);
	long FindStartCode(const BYTE * pBuffer,long Lentgh,DWORD code);
protected:
	string ConvertEssenceCoding2String(const UInt8* pData);
	bool GetAudioInfo(MDObjectPtr Desp);
//{added by tianyx  13:6:2013   14:17
	bool GetAFDInfo(std::string& strFileIn, byte& outInfo);
	//}end added by tianyx

//! DM Dictionaries
	typedef std::list<std::string> DMFileList;
    DMFileList           m_DMDicts;
//	MXFFilePtr           m_MxfFilePtr;
	UInt8              m_ui8VideoCoderFormat;
	UInt8              m_ui8AudioCoderFormat;
};