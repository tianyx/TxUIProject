// DlgStgSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "EmbTaskOwl.h"
#include "DlgStgSetting.h"


// CDlgStgSetting 对话框

IMPLEMENT_DYNAMIC(CDlgStgSetting, CDialog)

CDlgStgSetting::CDlgStgSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStgSetting::IDD, pParent)
{

}

CDlgStgSetting::~CDlgStgSetting()
{
}

void CDlgStgSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STG, m_lstStg);
	DDX_Control(pDX, IDC_BTN_NEW, m_btnNew);
	DDX_Control(pDX, IDC_BTN_COPY, m_btnCopy);
	DDX_Control(pDX, IDC_BTN_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_BTN_MOVEUP, m_btnMoveUp);
	DDX_Control(pDX, IDC_BTN_MOVEDOWN, m_btnMoveDown);
	DDX_Control(pDX, IDC_BTN_ENABLE, m_btnEnable);
	DDX_Control(pDX, IDC_EDIT1, m_edtContent);
	DDX_Control(pDX, IDC_EDIT_STGCONTENT, m_edtStgContent);
}


BEGIN_MESSAGE_MAP(CDlgStgSetting, CDialog)
	ON_NOTIFY(HDN_ITEMCHANGED, 0, &CDlgStgSetting::OnHdnItemchangedListStg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_STG, &CDlgStgSetting::OnNMCustomdrawListStg)
END_MESSAGE_MAP()


// CDlgStgSetting 消息处理程序

BOOL CDlgStgSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_lstStg.SetExtendedStyle( (m_lstStg.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_SHOWSELALWAYS) &(~LVS_EX_TRACKSELECT));
	// TODO:  在此添加额外的初始化
	m_lstStg.InsertColumn(0, TEXT("序号"), LVCFMT_LEFT, 50);
	m_lstStg.InsertColumn(1, TEXT("策略名称"), LVCFMT_LEFT, 100);
	m_lstStg.InsertColumn(2, TEXT("策略文件"), LVCFMT_LEFT, 100);

	m_lstStg.InsertColumn(3, TEXT("映射模板"), LVCFMT_LEFT, 100);
	
	LoadFromFile();
	UpdateCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgStgSetting::LoadFromFile()
{

}

void CDlgStgSetting::UpdateCtrl()
{
	POSITION pos = m_lstStg.GetFirstSelectedItemPosition();
	int nItem = -1;
	if (pos)
	{
		nItem = m_lstStg.GetNextSelectedItem(pos);
	}
	m_btnEdit.EnableWindow(nItem >= 0);
	m_btnCopy.EnableWindow(nItem >= 0);
	m_btnMoveDown.EnableWindow(nItem >= 0 && (nItem < m_lstStg.GetItemCount()-1));
	m_btnMoveUp.EnableWindow(nItem > 0);
	m_btnEnable.EnableWindow(nItem > 0);


}
void CDlgStgSetting::OnHdnItemchangedListStg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateCtrl();
	*pResult = 0;
}

void CDlgStgSetting::OnNMCustomdrawListStg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	LPNMLVCUSTOMDRAW plvCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult =CDRF_NOTIFYITEMDRAW;
		return;
	}

	if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
	{

		int nId = pNMCD->dwItemSpec;
		if( m_lstStg.GetItemState(nId, LVIS_SELECTED) & LVIS_SELECTED)
		{
			plvCD->clrTextBk = RGB(53,152,255);
			plvCD->clrText = RGB(255,255,255);
			*pResult = 0;
			return;
		}
	}
}
