#pragma once


typedef LRESULT (CALLBACK *FNTXCONTAINERWNDPROC)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


class CTxUIContainer
{
private:
	CTxUIContainer(void);
	virtual ~CTxUIContainer(void);
	
public:
	HWND GetHwnd(){return m_hwnd;}

public:
	virtual LRESULT OnContainerWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	BOOL Init(HWND hwnd);
	BOOL UnInit();
	FNTXCONTAINERWNDPROC m_pOldWndProc;

private:
	HWND m_hwnd;
	static CTxUIContainer* m_spContainer;
public:
	static CTxUIContainer* AttachUIWnd(HWND hwnd);
	static CTxUIContainer* GetContainer();

};

