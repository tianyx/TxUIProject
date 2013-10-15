
// AotoFillDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AotoFill.h"
#include "AotoFillDlg.h"
#include "ZQGlobal.h"
#include "CApplication.h"
#include <vector>
#include "CRange.h"
#include "io.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAotoFillDlg �Ի���




CAotoFillDlg::CAotoFillDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAotoFillDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAotoFillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_log);
	DDX_Control(pDX, IDC_CHECK_SUBDIR, m_chkSubDir);
}

BEGIN_MESSAGE_MAP(CAotoFillDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CAotoFillDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_Merge, &CAotoFillDlg::OnBnClickedBtnMerge)
END_MESSAGE_MAP()


// CAotoFillDlg ��Ϣ�������

BOOL CAotoFillDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	g_hwndLog = this->GetSafeHwnd();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CAotoFillDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAotoFillDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAotoFillDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAotoFillDlg::OnBnClickedBtnStart()
{
	m_log.ResetContent();

	BOOL bWithSubDir = m_chkSubDir.GetCheck() == BST_CHECKED;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFolder = g_GlobalInfo.szAppPath.c_str();
	VECFiles vFiles;
	GetFilesInFolder(strFolder, vFiles, bWithSubDir);
	if (vFiles.size() == 0)
	{
		CFWriteLog(TEXT("��ת���ļ�����Ϊ0"));
		return;
	}
	
	CTime tm(time(NULL));
	CString strTmpDir = strFolder;
	strTmpDir.Format(TEXT("%s\\Converted%2d%02d%02d"), strFolder, tm.GetHour(),tm.GetMinute(), tm.GetSecond());
	CreateDirectory(strTmpDir, NULL);
	CFWriteLog(TEXT("���Ŀ¼%s"), strTmpDir);
	CApplication objApp;
	if (!objApp.CreateDispatch(TEXT("Excel.Application"), NULL))
	{
		AfxMessageBox(TEXT("δ�ҵ�excel.exe"));
		return;
	}
	objApp.put_AlertBeforeOverwriting(FALSE);
	//objApp.put_DisplayAlerts(FALSE);
	for (size_t i = 0 ; i <vFiles.size(); i++)
	{
		CString strSrc, strDes;
		strSrc = vFiles[i];
		int nPos = strFolder.GetLength();
		CString strDesName = strSrc.Mid(nPos+1);
		strDes.Format(TEXT("%s\\%s"), strTmpDir, strDesName);
		if (_access(strSrc, 04)!= 0)
		{
			CFWriteLog(TEXT("�ļ��޷����ʣ� file=%s"), strSrc);
			continue;
		}
			
		CFWriteLog(TEXT("��ʼת����%d���ļ�(��%d��)"), i+1, vFiles.size());
		ConvertExcellFiles(strSrc, strDes, objApp);
	}

	objApp.ReleaseDispatch();
	CFWriteLog(TEXT("ȫ��ת��������ɣ�"));

	
}

void CAotoFillDlg::GetFilesInFolder( CString& strPath, VECFiles& vOut, BOOL bSearchInSubFolder /*= TRUE*/)
{
	CFileFind finder;
	// build a string with wildcards
	CString strWildcard(strPath);
	strWildcard.TrimRight(TEXT("\\"));
	strWildcard += TEXT("\\*.*");

	CString strAppPath = GetAppPath().c_str();
	strAppPath.TrimRight(TEXT("\\"));

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if (finder.IsDots())
			continue;
		// if it's a directory, recursively search it
		if (finder.IsDirectory() && bSearchInSubFolder)
		{
			CString strPath = finder.GetFilePath();

			CString strPreSubPath = strPath;
			strPreSubPath += TEXT("\\");
			strPreSubPath += finder.GetFileName();
			strPreSubPath += TEXT("\\");
			GetFilesInFolder(strPath, vOut, TRUE);
		}

		CString strName = finder.GetFileName();
		int nPos = strName.ReverseFind(L'.');
		if (!(nPos < 0))
		{
			CString strType = strName.Mid(nPos+1);
			strType.MakeLower();
			if (strType.Find(TEXT("xls"))!= -1)
			{
				CString strFull = strPath;
				strFull +=TEXT("\\");
				strFull += strName;
				vOut.push_back(strFull);
			}
		}
	}

	finder.Close();


}

BOOL CAotoFillDlg::ConvertExcellFiles( CString& strSrc, CString& strDes ,  CApplication& appIn)
{
	CString strDesPath = strDes;
	int nPos = strDesPath.ReverseFind('\\');
	strDesPath = strDesPath.Mid(0, nPos);
	CreateDirectory(strDesPath, NULL);
	if (!CopyFile(strSrc, strDes, FALSE))
	{
		CFWriteLog(TEXT("�ļ�����ʧ��! file = %s"), strDes);
		return FALSE;
	}

	CWorkbooks books;
	CWorkbook book;
	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	try
	{
	
	books = appIn.get_Workbooks();
	if (!books.m_lpDispatch)
	{
		CFWriteLog(TEXT("�ܹ�������ʧ��"));
		return FALSE;
	}
	book = books.Open(strDes, VOptional, _variant_t(false), VOptional, VOptional,  VOptional,     VOptional, VOptional, VOptional,  VOptional, VOptional, VOptional, VOptional,  VOptional, VOptional);
	if (!book.m_lpDispatch)
	{
		CFWriteLog(TEXT("��������ʧ��"));
		return FALSE;
	}

	CWorksheets sheets;
	sheets = book.get_Worksheets();
	if (!sheets.m_lpDispatch)
	{
		CFWriteLog(TEXT("�����ܱ��ʧ��"));
		return FALSE;
	}

	int nSheetCount = sheets.get_Count();
	int nChangedNum = 0;
	for (int i = 1; i <= nSheetCount; i++)
	{
		COleVariant vIdx((long)i);
		
		CWorksheet st =sheets.get_Item(vIdx);
		if (!st.m_lpDispatch)
		{
			CFWriteLog(TEXT("��%d��ʧ��"), i);
			continue;
		}
		
		CExcellRange rgn;
		rgn = st.get_UsedRange();
		CExcellRange rgnRow = rgn.get_Rows();
		CExcellRange rgnCol = rgn.get_Columns();
		long nRowCount= rgnRow.get_Count();
		long nColCount = rgnCol.get_Count();
		for (long irow = 1; irow  <= nRowCount; ++irow)
		{
			COleVariant valAge, ValAddr, valToFill; 
			COleVariant vRow((long)irow);
			COleVariant vCol((long)g_GlobalInfo.nKeyColAge);
			CExcellRange ageRgn, fillRgn, addrRgn;
			ageRgn.AttachDispatch(rgn.get_Item(COleVariant(irow), COleVariant(g_GlobalInfo.nKeyColAge)).pdispVal);
			fillRgn.AttachDispatch(rgn.get_Item(COleVariant(irow), COleVariant(g_GlobalInfo.nKeyColFill)).pdispVal);
			addrRgn.AttachDispatch(rgn.get_Item(COleVariant(irow), COleVariant(g_GlobalInfo.nKeyColAddr)).pdispVal);
			valAge = ageRgn.get_Text();
			valToFill = fillRgn.get_Text();
			ValAddr = addrRgn.get_Text();
			CString strFill;
			CString strAddr;
			CString strAge;
			if (valAge.vt != VT_NULL)
			{
				strAge = CString(valAge.bstrVal);	
			}
			if (valToFill.vt != VT_NULL)
			{
				strFill = valToFill.bstrVal;
			}
			if (ValAddr.vt != VT_NULL)
			{
				strAge = valAge.bstrVal;
			}

			BOOL bChanged = FALSE;
			if (strFill.GetLength()< 3)
			{
				if (!strAge.IsEmpty())
				{
					int nAge = atoi(strAge.GetBuffer());
					if (nAge <= g_GlobalInfo.nAgeMax && nAge >= g_GlobalInfo.nAgeMin)
					{
						int nHp = GenRand()%(g_GlobalInfo.nDetaHp) + g_GlobalInfo.nMinHp;
						int nMp = GenRand()%(g_GlobalInfo.nDetaMp) + g_GlobalInfo.nMinMp;
						strFill.Format(TEXT("%d/%d"), nHp, nMp);
						rgn.put_Item(COleVariant(irow), COleVariant(g_GlobalInfo.nKeyColFill), COleVariant(strFill));	
						bChanged = TRUE;
					}
				}
			}

			if (strAddr.IsEmpty())
			{
				int nAddrSize = g_GlobalInfo.vAddress.size();
				strAddr = g_GlobalInfo.vAddress[GenRand()%nAddrSize].c_str();
				rgn.put_Item(COleVariant(irow), COleVariant(g_GlobalInfo.nKeyColAddr), COleVariant(strAddr));	
				bChanged = TRUE;
			}

			if (bChanged)
			{
				nChangedNum++;
			}
			
		}
		
	}

	CFWriteLog(TEXT("ת�����:��%d����������ת��%d��"),nSheetCount, nChangedNum);
	book.Save();
	book.Close(VOptional,COleVariant(strDes),VOptional); 
	books.Close();

	}
	catch (const _com_error& e)
	{
		CFWriteLog(TEXT("ת������:%s"), e.ErrorMessage());
		book.Close(VOptional,COleVariant(strDes),VOptional); 
		books.Close();

	}
	catch(...)
	{
		CFWriteLog(TEXT("ת������"));
	}
	return TRUE;

}

LRESULT CAotoFillDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (message == MSG_LOGWRITE)
	{
		char* line = (char*)wParam;
		m_log.AddString(line);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CAotoFillDlg::OnBnClickedBtnMerge()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_log.ResetContent();

	BOOL bWithSubDir = m_chkSubDir.GetCheck() == BST_CHECKED;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFolder = g_GlobalInfo.szAppPath.c_str();
	VECFiles vFiles;
	GetFilesInFolder(strFolder, vFiles, bWithSubDir);
	if (vFiles.size() == 0)
	{
		CFWriteLog(TEXT("��ת���ļ�����Ϊ0"));
		return;
	}

	CString strTemplateFile = strFolder;
	strTemplateFile +=TEXT("\\wgyTemplate.xlt");
	if (_access(strTemplateFile, 04)!= 0)
	{
		CFWriteLog(TEXT("ģ���ļ��޷����ʣ� file=%s"), strTemplateFile);
		return;
	}
	CTime tm(time(NULL));
	CString strTmpDir = strFolder;
	strTmpDir.Format(TEXT("%s\\Merged%2d%02d%02d.xls"), strFolder, tm.GetHour(),tm.GetMinute(), tm.GetSecond());

	CFWriteLog(TEXT("�ϲ����ļ���%s"), strTmpDir);
	CApplication objApp;
	if (!objApp.CreateDispatch(TEXT("Excel.Application"), NULL))
	{
		AfxMessageBox(TEXT("δ�ҵ�excel.exe"));
		return;
	}
	objApp.put_AlertBeforeOverwriting(FALSE);

	CWorkbooks books = objApp.get_Workbooks();
	if (!books.m_lpDispatch)
	{
		objApp.DetachDispatch();
		CFWriteLog(TEXT("�ܹ�������ʧ��"));
		return ;
	}
	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	CWorkbook bookDes = books.Add(COleVariant(strTemplateFile));
	if (!bookDes.m_lpDispatch)
	{
		CFWriteLog(TEXT("�½�xlsʧ��"));
		objApp.DetachDispatch();
		return;
	}
	try
	{


		//objApp.put_DisplayAlerts(FALSE);
		for (size_t i = 0 ; i <vFiles.size(); i++)
		{
			CString strSrc, strDes;
			strSrc = vFiles[i];
			int nPos = strFolder.GetLength();
			CString strDesName = strSrc.Mid(nPos+1);
			strDes.Format(TEXT("%s\\%s"), strTmpDir, strDesName);
			if (_access(strSrc, 04)!= 0)
			{
				CFWriteLog(TEXT("�ļ��޷����ʣ� file=%s"), strSrc);
				continue;
			}

			CWorkbook book = books.Open(strSrc, VOptional, _variant_t(false), VOptional, VOptional,  VOptional,     VOptional, VOptional, VOptional,  VOptional, VOptional, VOptional, VOptional,  VOptional, VOptional);
			if (!book.m_lpDispatch)
			{
				CFWriteLog(TEXT("��������ʧ��:%s"), strSrc);
				continue;
			}

			CFWriteLog(TEXT("��ʼ�ϲ���%d���ļ�(��%d��)"), i+1, vFiles.size());
			MergeExcelFiles(book, bookDes);
			book.Close(_variant_t(false), VOptional, VOptional);
		}
		bookDes.SaveAs(COleVariant(strTmpDir), VOptional,VOptional,VOptional,VOptional,VOptional, 1,VOptional,VOptional,VOptional,VOptional,VOptional);

	}
	catch (...)
	{
		CFWriteLog(TEXT("�ϲ�ʧ��"));
		objApp.ReleaseDispatch();
	}
	
	objApp.ReleaseDispatch();
	CFWriteLog(TEXT("�ϲ�������ɣ�"));
}

BOOL CAotoFillDlg::MergeExcelFiles( CWorkbook&bookSrc, CWorkbook& bookDes )
{
	CWorksheet stSrc = bookSrc.get_ActiveSheet();
	CWorksheet stDes = bookDes.get_ActiveSheet();
	CExcellRange rgnSrc = stSrc.get_UsedRange();
	stSrc.Copy(COleVariant("A1"), COleVariant("BA10"));

	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	stDes.Paste(COleVariant("A1"), VOptional);

	return TRUE;
}
