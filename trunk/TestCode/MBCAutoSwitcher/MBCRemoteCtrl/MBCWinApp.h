#pragma once



// CMBCWinApp

class CMBCWinApp : public CWinApp
{
	DECLARE_DYNCREATE(CMBCWinApp)

public:
	CMBCWinApp();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMBCWinApp();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


