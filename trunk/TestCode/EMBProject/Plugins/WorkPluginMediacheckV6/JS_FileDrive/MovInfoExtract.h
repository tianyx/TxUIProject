#pragma once
#include "FileInfoExtract.h"
#include "JS_FileDrive.h"
////////////////////////////////////////////////////////////////////
//CMOVInfoExtract class设计目的是为了从MOV 文件中得到一些视频和音频信息以及文件比较.
//zhaoXiaolong founder 2012-07-13
////////////////////////////////////////////////////////////////////

using namespace std;

class _JS_Fix CMOVInfoExtract:public CFileInfoExtract
{
public:
	CMOVInfoExtract(void);
    virtual ~CMOVInfoExtract(void);
	virtual BOOL AnalyzeFile();
};