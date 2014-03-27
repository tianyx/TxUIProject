/********************************************************************
	created:	2013/12/02
	filename: 	SystemResourceInfo.h
	author:		zhangjianchao
	purpose:	ʹ��windows���ܼ����� ��ȡ���� CPU, DISK I/O, NET, MEMORY ��Ϣ 
    History: 
*********************************************************************/

#pragma once

enum SYSTEM_RES
{
	CPU = 1,
	MEMORY = 2,
	DISK   = 3,
	NET    = 4
};

class CSystemResourceInfo
{
public:
	CSystemResourceInfo(void);
	~CSystemResourceInfo(void);

	// �����Դ��Ϣ
	virtual bool GetInfor(){return true;}
	double GetCounter(LPCSTR pPath, DWORD nFormat, bool bTwoSample = true);

public:
	SYSTEM_RES	m_eRes;			// ��Դ����
	CString     m_strName;      // ����
	int			m_nUsedPercent; // ʹ�ðٷֱ�
	int			m_nIO;      // kbps
};

class CCpuRes : public CSystemResourceInfo
{
public:
	CCpuRes();
	~CCpuRes();

	virtual bool GetInfor();
};

class CMemoryRes : public CSystemResourceInfo
{
public:
	CMemoryRes();
	~CMemoryRes();

	virtual bool GetInfor();

public:
	int m_Available; // �����ڴ�  Mbytes
	int m_PhysicalSize; // �����ڴ��СMbytes
};

class CDiskRes : public CSystemResourceInfo
{
public:
	CDiskRes();
	~CDiskRes();

	virtual bool GetInfor();
};

class CNetRes : public CSystemResourceInfo
{
public:
	CNetRes();
	~CNetRes();

	virtual bool GetInfor();
};
