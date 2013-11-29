/********************************************************************
	created:	2013/11/22
	created:	$2013:11:22
	filename: 	EMBMessageDef.h
	file base:	EMBMessageDef
	file ext:	h
	author:		tianyx
	
	purpose:	ST_EMBTRANSMSG 消息传递类
				CEMBAutoBuffer 自动字符串转换类，用于自动创建适应消息类对象的buffer，析构时释放
*********************************************************************/
#pragma once
struct ST_EMBTRANSMSG :public ST_TXMSGBASE
{
	CString strGuid;
	CString strData;
	ST_EMBTRANSMSG(int nMsgTypeIn)
	{
		nMsgType = nMsgTypeIn;
	}

	virtual BOOL operator <<( CTxSerialize& ar) const
	{
		ST_TXMSGBASE::operator <<(ar);
		ar << strGuid;
		ar << strData;
		return TRUE;
	}

	virtual BOOL operator >>(CTxSerialize& ar)
	{
		ST_TXMSGBASE::operator >>(ar);
		ar >> strGuid;
		ar >> strData;
		return TRUE;
	}
};

class CEMBAutoBuffer
{
public:
	CEMBAutoBuffer(ST_EMBTRANSMSG& msgIn)
	{
		//calc the needed size
#ifdef _UNICODE
		m_nBuffSize = 256+(msgIn.strGuid.GetLength()+msgIn.strData.GetLength())*2;
#else
		m_nBuffSize = 256+msgIn.strGuid.GetLength()+msgIn.strData.GetLength();

#endif
		m_pBuff = new char[m_nBuffSize];

	}

	CEMBAutoBuffer(int nLen)
	{
		m_pBuff = new char[nLen];
	}


	~CEMBAutoBuffer()
	{
		if (m_pBuff)
		{
			delete[] m_pBuff;
			m_pBuff = NULL;
		}
	}

	operator char*(){return m_pBuff;}
	operator LONG_PTR (){return (LONG_PTR)m_pBuff;}
	int GetSize(){return m_nBuffSize;}
private:
	int m_nBuffSize;
	char* m_pBuff;
};