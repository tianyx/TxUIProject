#pragma once
#include "FileInfoExtract.h"
#include "JS_FileDrive.h"
////////////////////////////////////////////////////////////////////
//CMOVInfoExtract class���Ŀ����Ϊ�˴�MOV �ļ��еõ�һЩ��Ƶ����Ƶ��Ϣ�Լ��ļ��Ƚ�.
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