#pragma once



// CMBCWinApp

class CMBCWinApp : public CWinApp
{
	DECLARE_DYNCREATE(CMBCWinApp)

public:
	CMBCWinApp();           // 动态创建所使用的受保护的构造函数
	virtual ~CMBCWinApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


