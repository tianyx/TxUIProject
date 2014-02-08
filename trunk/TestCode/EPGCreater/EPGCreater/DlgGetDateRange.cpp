// DlgGetDateRange.cpp : 实现文件
//

#include "stdafx.h"
#include "EPGCreater.h"
#include "DlgGetDateRange.h"


// CDlgGetDateRange 对话框

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


// CDlgGetDateRange 消息处理程序

void CDlgGetDateRange::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dtStart.GetTime(m_tmStart);
	m_dtEnd.GetTime(m_tmEnd);
	if (m_tmStart > m_tmEnd)
	{
		AfxMessageBox("起始时间大于结束时间！");
		return;
	}

	OnOK();
}

BOOL CDlgGetDateRange::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CTime tmNow(time(NULL));
	m_dtEnd.SetTime(&tmNow);
	m_dtStart.SetTime(&tmNow);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
