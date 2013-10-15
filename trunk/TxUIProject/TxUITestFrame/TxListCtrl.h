#pragma once

#include "TxScrollView.h"
// CTxListCtrl

struct st_TxLstHeaderInfo 
{
	UINT m_nHeaderId; //not order number.
	CSize m_szHeaderSize;
	CString m_strTitle;

};

struct st_TxLstItemInfo
{

};

typedef std::list<st_TxLstHeaderInfo> LSTHEADERS;

class CTxListCtrl : public CTxScrollView
{
	DECLARE_DYNAMIC(CTxListCtrl)

public:
	CTxListCtrl();
	virtual ~CTxListCtrl();

private:
	//header
	LSTHEADERS m_headers;

};


