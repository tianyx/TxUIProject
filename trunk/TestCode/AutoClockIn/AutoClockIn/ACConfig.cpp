#include "StdAfx.h"
#include "ACConfig.h"
#include "IMSerialize.h"


#define AC_VER 918

HWND g_hwndLog = NULL;

#define STR_ACSORTCUT_FILENAME TEXT("\\AutoClockIn.lnk")
#define STR_ACCONFIG_FILE TEXT("\\AcConfig.dat")


CACConfig* CACConfig::m_spAcCfg = NULL;
int CACConfig::m_snRef = 0;

CACConfig::CACConfig(void)
{
	CWinApp* pApp = AfxGetApp();

	CString strAppPath = pApp->m_pszHelpFilePath;
	CString strPath = GetFileName(strAppPath);
	ST_CLOCKINDATA data;
	for (int i = 0; i < 4; i++)
	{
		data.netData.strUrl.Format(TEXT("/_layouts/Founder/Check/Check.aspx?Type=%d"), i+1) ;
		data.netData.strUser = TEXT("");
		data.netData.strPw = TEXT("Founder@2011!");
		data.netData.strSvr = TEXT("itsoft.hold.founder.com");
		data.netData.strFile.Format( TEXT("%sClockInLog%d.txt"), strPath, i+1);
		m_vNetData.push_back(data);
	}

	m_vNetData[0].nHour = 9;
	m_vNetData[0].nMinute = 0;
	m_vNetData[1].nHour = 11;
	m_vNetData[1].nMinute = 31;
	m_vNetData[2].nHour = 11;
	m_vNetData[2].nMinute = 31;
	m_vNetData[3].nHour = 18;
	m_vNetData[3].nMinute = 3;

}

CACConfig::~CACConfig(void)
{
}

BOOL CACConfig::CreateStatrupShortcut()
{
	TCHAR szMyDir[MAX_PATH];   
	ZeroMemory(szMyDir, sizeof(szMyDir));
	::SHGetSpecialFolderPath(NULL,szMyDir,CSIDL_STARTUP,0); 
	CString strPathStartUp = szMyDir;

	::GetModuleFileName(NULL, szMyDir, sizeof(szMyDir));
	CString strAppName = szMyDir;
	CString strSortcutFile = strPathStartUp + STR_ACSORTCUT_FILENAME;
	CFileFind ffind;
	BOOL bFind = ffind.FindFile(strSortcutFile, 0);
	if (bFind)
	{
		ACWriteLog(TEXT("find old startup link, delete first."));
		BOOL bDel = DeleteFile(strSortcutFile);
		if (!bDel)
		{
			ASSERT(FALSE);
			ACWriteLog(TEXT("failed to delete startup link!"));
		}
	}

	ACWriteLog(TEXT("creating startup link..."));
	ffind.Close();

	BOOL bSuc =CreateLink(strAppName, strSortcutFile);
	if (bSuc)
	{
		ACWriteLog(TEXT("creating startup successfully."));

	}
	else
	{
		ACWriteLog(TEXT("creating startup failed!"));

	}
	return bSuc;
}

CACConfig* CACConfig::GetACConfig()
{
	if (!m_spAcCfg)
	{
		m_spAcCfg = new CACConfig;
	}

	m_snRef++;
	return m_spAcCfg;
}

BOOL CACConfig::LoadData()
{
	CString strCfgFile;
	strCfgFile = GetAppPath() + STR_ACCONFIG_FILE;
	CFile file;
	BOOL bOpen = file.Open(strCfgFile, CFile::modeRead, NULL);
	if (bOpen)
	{
		CIMSerialize ar(&file);
		file.Close();
		int nVer = 0;
		ar >> nVer;
		if (nVer == AC_VER)
		{
			ar >> m_cfgData;
			ar >> m_vNetData;
		}
		else
		{
			ACWriteLog(TEXT("failed to load config, use default."));
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

CString CACConfig::GetAppPath()
{

	CString strAppPath = AfxGetApp()->m_pszHelpFilePath;
	CString strPath = GetFileName(strAppPath);
	return strPath;

}

BOOL CACConfig::SaveData()
{

	//pre action
	if (m_cfgData.bAutoRun)
	{
		CreateStatrupShortcut();
	}

	CString strCfgFile = GetAppPath() + STR_ACCONFIG_FILE;
	CFile file;
	BOOL bOpen = file.Open(strCfgFile, CFile::modeCreate|CFile::modeWrite, NULL);
	if (bOpen)
	{
		CIMSerialize ar;
		ar << (int)AC_VER;
		ar << m_cfgData;
		ar << m_vNetData;
		ar.SerializeFileOut(&file);
		file.Flush();
		file.Close();
	}
	else
	{
		ACWriteLog(TEXT("failed to save config!"));
		return FALSE;
	}

	return TRUE;
}

void CACConfig::Release()
{
	m_snRef--;
	if (m_snRef <= 0)
	{
		delete m_spAcCfg;
		m_spAcCfg = NULL;

	}
}

CString GetFileName( CString& strFileIn )
{
	CString strExt = strFileIn;
	int nPos = strFileIn.ReverseFind('\\');
	if(nPos > 0)
	{
		strExt = strFileIn.Left(nPos+1);
	}
	return strExt;
}

BOOL CreateLink(LPCTSTR szPath,//快捷方式的目标应用程序名
				LPCTSTR szLink)//快捷方式的数据文件名(*.lnk)
{
	HRESULT hres;
	IShellLink* psl;
	IPersistFile* ppf;
	////初始化COM
	CoInitialize(NULL);
	//创建一个IShellLink实例
	hres = CoCreateInstance(CLSID_ShellLink,NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&psl);
	if(FAILED(hres))
	{
		CoUninitialize();
		return FALSE;
	}
	//设置目标应用程序
	psl->SetPath(szPath);
	//设置快捷键(此处设为Shift+Ctrl+'R')
	// psl -> SetHotkey( MAKEWORD( 'R',
	// HOTKEYF_SHIFT |HOTKEYF_CONTROL)) ;
	CString csWorkDir(szPath);
	int idx = csWorkDir.ReverseFind('\\');
	csWorkDir = csWorkDir.Left(idx);
	psl->SetWorkingDirectory(csWorkDir);
	//从IShellLink获取其IPersistFile接口
	//用于保存快捷方式的数据文件 (*.lnk)
	hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
	if(FAILED(hres))
	{
		CoUninitialize();
		return FALSE;
	}
	//调用IPersistFile::Save
	//保存快捷方式的数据文件 (*.lnk)
	hres = ppf ->Save(szLink, STGM_READWRITE);
	//释放IPersistFile和IShellLink接口
	ppf -> Release( ) ;
	psl -> Release( ) ;
	CoUninitialize();
	return TRUE;
}

void ACWriteLog(LPCTSTR lpFmt, ...)
{
	va_list va; va_start(va,lpFmt);
	CString strLog;
	::_vsnwprintf(strLog.GetBuffer(256),255,lpFmt,va);
	strLog.ReleaseBuffer();

	if (g_hwndLog)
	{
		::SendMessage(g_hwndLog, MSG_AC_WRITELOG, (WPARAM)&strLog, 0);
	}
}