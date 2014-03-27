/********************************************************************
	created:	2013/12/02
	filename: 	SystemResourceInfo.h
	author:		zhangjianchao
	purpose:	使用windows性能计数器 获取本机 CPU, DISK I/O, NET, MEMORY 信息 
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

	// 获得资源信息
	virtual bool GetInfor(){return true;}
	double GetCounter(LPCSTR pPath, DWORD nFormat, bool bTwoSample = true);

public:
	SYSTEM_RES	m_eRes;			// 资源类型
	CString     m_strName;      // 名称
	int			m_nUsedPercent; // 使用百分比
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
	int m_Available; // 可用内存  Mbytes
	int m_PhysicalSize; // 物理内存大小Mbytes
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
