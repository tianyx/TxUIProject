਍⼀⼀ 䔀洀戀吀攀猀琀䐀氀最⸀挀瀀瀀 㨀 鸀끛蝳ൎഀ
//਍ഀഀ
#include "stdafx.h"਍⌀椀渀挀氀甀搀攀 ∀䔀洀戀吀攀猀琀⸀栀∀ഀഀ
#include "EmbTestDlg.h"਍⌀椀渀挀氀甀搀攀 ∀吀砀倀愀爀愀洀匀琀爀椀渀最⸀栀∀ഀഀ
਍⌀椀昀搀攀昀 开䐀䔀䈀唀䜀ഀഀ
#define new DEBUG_NEW਍⌀攀渀搀椀昀ഀഀ
਍ഀഀ
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框਍ഀഀ
class CAboutDlg : public CDialog਍笀ഀഀ
public:਍ऀ䌀䄀戀漀甀琀䐀氀最⠀⤀㬀ഀഀ
਍⼀⼀ 豈�䚋灨湥ൣഀ
	enum { IDD = IDD_ABOUTBOX };਍ഀഀ
	protected:਍ऀ瘀椀爀琀甀愀氀 瘀漀椀搀 䐀漀䐀愀琀愀䔀砀挀栀愀渀最攀⠀䌀䐀愀琀愀䔀砀挀栀愀渀最攀⨀ 瀀䐀堀⤀㬀    ⼀⼀ 䐀䐀堀⼀䐀䐀嘀 ⼀ťൣഀ
਍⼀⼀ 鸀끛൳ഀ
protected:਍ऀ䐀䔀䌀䰀䄀刀䔀开䴀䔀匀匀䄀䜀䔀开䴀䄀倀⠀⤀ഀഀ
};਍ഀഀ
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)਍笀ഀഀ
}਍ഀഀ
void CAboutDlg::DoDataExchange(CDataExchange* pDX)਍笀ഀഀ
	CDialog::DoDataExchange(pDX);਍紀ഀഀ
਍䈀䔀䜀䤀一开䴀䔀匀匀䄀䜀䔀开䴀䄀倀⠀䌀䄀戀漀甀琀䐀氀最Ⰰ 䌀䐀椀愀氀漀最⤀ഀഀ
END_MESSAGE_MAP()਍ഀഀ
਍⼀⼀ 䌀䔀洀戀吀攀猀琀䐀氀最 豈�䚋൨ഀ
਍ഀഀ
਍ഀഀ
CEmbTestDlg::CEmbTestDlg(CWnd* pParent /*=NULL*/)਍ऀ㨀 䌀䐀椀愀氀漀最⠀䌀䔀洀戀吀攀猀琀䐀氀最㨀㨀䤀䐀䐀Ⰰ 瀀倀愀爀攀渀琀⤀ഀഀ
{਍ऀ洀开栀䤀挀漀渀 㴀 䄀昀砀䜀攀琀䄀瀀瀀⠀⤀ⴀ㸀䰀漀愀搀䤀挀漀渀⠀䤀䐀刀开䴀䄀䤀一䘀刀䄀䴀䔀⤀㬀ഀഀ
}਍ഀഀ
void CEmbTestDlg::DoDataExchange(CDataExchange* pDX)਍笀ഀഀ
	CDialog::DoDataExchange(pDX);਍紀ഀഀ
਍䈀䔀䜀䤀一开䴀䔀匀匀䄀䜀䔀开䴀䄀倀⠀䌀䔀洀戀吀攀猀琀䐀氀最Ⰰ 䌀䐀椀愀氀漀最⤀ഀഀ
	ON_WM_SYSCOMMAND()਍ऀ伀一开圀䴀开倀䄀䤀一吀⠀⤀ഀഀ
	ON_WM_QUERYDRAGICON()਍ऀ⼀⼀紀紀䄀䘀堀开䴀匀䜀开䴀䄀倀ഀഀ
	ON_BN_CLICKED(IDC_BUTTON1, &CEmbTestDlg::OnBnClickedButton1)਍䔀一䐀开䴀䔀匀匀䄀䜀䔀开䴀䄀倀⠀⤀ഀഀ
਍ഀഀ
// CEmbTestDlg 消息处理程序਍ഀഀ
BOOL CEmbTestDlg::OnInitDialog()਍笀ഀഀ
	CDialog::OnInitDialog();਍ഀഀ
	// 将“关于...”菜单项添加到系统菜单中。਍ഀഀ
	// IDM_ABOUTBOX 必须在系统命令范围内。਍ऀ䄀匀匀䔀刀吀⠀⠀䤀䐀䴀开䄀䈀伀唀吀䈀伀堀 ☀ 　砀䘀䘀䘀　⤀ 㴀㴀 䤀䐀䴀开䄀䈀伀唀吀䈀伀堀⤀㬀ഀഀ
	ASSERT(IDM_ABOUTBOX < 0xF000);਍ഀഀ
	CMenu* pSysMenu = GetSystemMenu(FALSE);਍ऀ椀昀 ⠀瀀匀礀猀䴀攀渀甀 ℀㴀 一唀䰀䰀⤀ഀഀ
	{਍ऀऀ䈀伀伀䰀 戀一愀洀攀嘀愀氀椀搀㬀ഀഀ
		CString strAboutMenu;਍ऀऀ戀一愀洀攀嘀愀氀椀搀 㴀 猀琀爀䄀戀漀甀琀䴀攀渀甀⸀䰀漀愀搀匀琀爀椀渀最⠀䤀䐀匀开䄀䈀伀唀吀䈀伀堀⤀㬀ഀഀ
		ASSERT(bNameValid);਍ऀऀ椀昀 ⠀℀猀琀爀䄀戀漀甀琀䴀攀渀甀⸀䤀猀䔀洀瀀琀礀⠀⤀⤀ഀഀ
		{਍ऀऀऀ瀀匀礀猀䴀攀渀甀ⴀ㸀䄀瀀瀀攀渀搀䴀攀渀甀⠀䴀䘀开匀䔀倀䄀刀䄀吀伀刀⤀㬀ഀഀ
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);਍ऀऀ紀ഀഀ
	}਍ഀഀ
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动਍ऀ⼀⼀  最䱢撈쵫層൏ഀ
	SetIcon(m_hIcon, TRUE);			// 设置大图标਍ऀ匀攀琀䤀挀漀渀⠀洀开栀䤀挀漀渀Ⰰ 䘀䄀䰀匀䔀⤀㬀ऀऀ⼀⼀ 븀溋ཿ﹜ݖ൨ഀ
਍ऀ⼀⼀ 吀伀䐀伀㨀 ⠀摗ﭫꁭ鵒ᚘ葙ᵶ쭒ᙙŎ൸ഀ
਍ऀ爀攀琀甀爀渀 吀刀唀䔀㬀  ⼀⼀ 搀庖ڗ♜륱빰溋みꝒ౎⛿ᥔ푒�⁖吀刀唀䔀ഀഀ
}਍ഀഀ
void CEmbTestDlg::OnSysCommand(UINT nID, LPARAM lParam)਍笀ഀഀ
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)਍ऀ笀ഀഀ
		CAboutDlg dlgAbout;਍ऀऀ搀氀最䄀戀漀甀琀⸀䐀漀䴀漀搀愀氀⠀⤀㬀ഀഀ
	}਍ऀ攀氀猀攀ഀഀ
	{਍ऀऀ䌀䐀椀愀氀漀最㨀㨀伀渀匀礀猀䌀漀洀洀愀渀搀⠀渀䤀䐀Ⰰ 氀倀愀爀愀洀⤀㬀ഀഀ
	}਍紀ഀഀ
਍⼀⼀ 舀鱙ᅧ凜�䚋ﭨꁭRཧᙜ॓깣ಔ᧿R膗உ扎蒗Ŏ൸ഀ
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，਍⼀⼀  �ڏㅜ䙵뙨ꢁ豒ၛɢരഀ
਍瘀漀椀搀 䌀䔀洀戀吀攀猀琀䐀氀最㨀㨀伀渀倀愀椀渀琀⠀⤀ഀഀ
{਍ऀ椀昀 ⠀䤀猀䤀挀漀渀椀挀⠀⤀⤀ഀഀ
	{਍ऀऀ䌀倀愀椀渀琀䐀䌀 搀挀⠀琀栀椀猀⤀㬀 ⼀⼀ ⠀蹵�㙾葒빶ދ൙上下文਍ഀഀ
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);਍ഀഀ
		// 使图标在工作区矩形中居中਍ऀऀ椀渀琀 挀砀䤀挀漀渀 㴀 䜀攀琀匀礀猀琀攀洀䴀攀琀爀椀挀猀⠀匀䴀开䌀堀䤀䌀伀一⤀㬀ഀഀ
		int cyIcon = GetSystemMetrics(SM_CYICON);਍ऀऀ䌀刀攀挀琀 爀攀挀琀㬀ഀഀ
		GetClientRect(&rect);਍ऀऀ椀渀琀 砀 㴀 ⠀爀攀挀琀⸀圀椀搀琀栀⠀⤀ ⴀ 挀砀䤀挀漀渀 ⬀ ㄀⤀ ⼀ ㈀㬀ഀഀ
		int y = (rect.Height() - cyIcon + 1) / 2;਍ഀഀ
		// 绘制图标਍ऀऀ搀挀⸀䐀爀愀眀䤀挀漀渀⠀砀Ⰰ 礀Ⰰ 洀开栀䤀挀漀渀⤀㬀ഀഀ
	}਍ऀ攀氀猀攀ഀഀ
	{਍ऀऀ䌀䐀椀愀氀漀最㨀㨀伀渀倀愀椀渀琀⠀⤀㬀ഀഀ
	}਍紀ഀഀ
਍⼀⼀匀⡟㝵홢ꡢRཧᙜ靓ﭥ�;⢌摵ﵫ灑홥靓䥟ݑ൨ഀ
//显示。਍䠀䌀唀刀匀伀刀 䌀䔀洀戀吀攀猀琀䐀氀最㨀㨀伀渀儀甀攀爀礀䐀爀愀最䤀挀漀渀⠀⤀ഀഀ
{਍ऀ爀攀琀甀爀渀 猀琀愀琀椀挀开挀愀猀琀㰀䠀䌀唀刀匀伀刀㸀⠀洀开栀䤀挀漀渀⤀㬀ഀഀ
}਍ഀഀ
਍瘀漀椀搀 䌀䔀洀戀吀攀猀琀䐀氀最㨀㨀伀渀䈀渀䌀氀椀挀欀攀搀䈀甀琀琀漀渀㄀⠀⤀ഀഀ
{਍ऀ䌀䘀椀氀攀 昀椀氀攀㬀ഀഀ
	file.Open("f:\\dd.xml", CFile::modeRead, NULL);਍ऀ挀栀愀爀 猀稀戀甀昀昀嬀㈀　㐀㠀崀㬀ഀഀ
	file.Read(szbuff, 2048);਍ऀ昀椀氀攀⸀䌀氀漀猀攀⠀⤀㬀ഀഀ
	CTxParamString txStr(szbuff);਍ऀ椀渀琀 渀䤀搀 㴀 琀砀匀琀爀⸀䜀攀琀䄀琀琀爀椀戀嘀愀氀⠀吀䔀堀吀⠀∀∀⤀Ⰰ 吀䔀堀吀⠀∀䤀䐀∀⤀⤀⸀䜀攀琀䄀猀䤀渀琀⠀⤀㬀ഀഀ
	txStr.GoToPath(TEXT(".\\SpotListInfo"));਍ऀ䌀匀琀爀椀渀最 猀琀爀䐀愀琀攀 㴀 琀砀匀琀爀⸀䜀攀琀䔀氀攀洀嘀愀氀⠀吀䔀堀吀⠀∀䐀愀琀攀∀⤀⤀⸀䜀攀琀䄀猀匀琀爀椀渀最⠀⤀㬀ഀഀ
	txStr.SetElemVal(TEXT("Date"), CTxStrConvert(TEXT("2013-11-11")));਍ऀ琀砀匀琀爀⸀匀攀琀䄀琀琀爀椀戀嘀愀氀⠀吀䔀堀吀⠀∀䐀愀琀攀∀⤀Ⰰ 吀䔀堀吀⠀∀䐀䐀∀⤀Ⰰ 䌀吀砀匀琀爀䌀漀渀瘀攀爀琀⠀∀琀攀猀琀䄀琀琀爀∀⤀⤀㬀ഀഀ
	txStr.UpdateData();਍ഀഀ
}਍�