#pragma once


// CTxWinApp

class CTxWinApp : public CWinAppEx
{
	DECLARE_DYNAMIC(CTxWinApp)

public:
	CTxWinApp();
	virtual ~CTxWinApp();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnIdle(LONG lCount);
};


