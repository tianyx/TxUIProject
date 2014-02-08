// DlgGetDateRange.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EPGCreater.h"
#include "DlgGetDateRange.h"


// CDlgGetDateRange �Ի���

IMPLEMENT_DYNAMIC(CDlgGetDateRange, CDialog)

CDlgGetDateRange::CDlgGetDateRange(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGetDateRange::IDD, pParent)
{

}

CDlgGetDateRange::~CDlgGetDateRange()
{
}

void CDlgGetDateRange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dtStart);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dtEnd);
}


BEGIN_MESSAGE_MAP(CDlgGetDateRange, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgGetDateRange::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgGetDateRange ��Ϣ�������

void CDlgGetDateRange::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_dtStart.GetTime(m_tmStart);
	m_dtEnd.GetTime(m_tmEnd);
	if (m_tmStart > m_tmEnd)
	{
		AfxMessageBox("��ʼʱ����ڽ���ʱ�䣡");
		return;
	}

	OnOK();
}

BOOL CDlgGetDateRange::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CTime tmNow(time(NULL));
	m_dtEnd.SetTime(&tmNow);
	m_dtStart.SetTime(&tmNow);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
