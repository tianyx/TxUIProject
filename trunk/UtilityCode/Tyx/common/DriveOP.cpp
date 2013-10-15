#include "StdAfx.h"
#include "DriveOP.h"

CDriveOP::CDriveOP(void)
:m_dwLastDrivers(0)
,m_bNeedRefresh(true)
{
}

CDriveOP::~CDriveOP(void)
{
}

bool CDriveOP::Refresh()
{
	char Disk[26];
	for (int i=0;i<26;i++)
		Disk[i] = 'A' + i;


	DWORD dwRet =  GetLogicalDrives();

	if (dwRet == 0)
	{
		DWORD err = GetLastError();
		g_log.Print(1, "GetLogicalDrives() err %d\r\n", err);
		return false;
	}


	m_vecDrivers.clear();
	for (int i=0;i<26;i++)
	{
		DWORD bit = 1 << i;
		if ( (dwRet & bit) == bit )
		{
			DriveItem drive;
			drive.DiskName = Disk[i];
			m_vecDrivers.push_back(drive);
		}
	}

	int size = m_vecDrivers.size();
	for(int i=0; i < size; i++)
	{
		char cPath[MAX_PATH];
		sprintf(cPath, "%c:\\",m_vecDrivers[i].DiskName);
		UINT nType = GetDriveType(cPath);
		m_vecDrivers[i].DiskType = nType;

		g_log.Print(1, "当前盘符：%c:\\  属性：%d\r\n", m_vecDrivers[i].DiskName, m_vecDrivers[i].DiskType);
	}

	m_bNeedRefresh = false;
	
	g_log.Print(1, "当前盘符数字%d, 上一次盘符数字%d\r\n", dwRet,m_dwLastDrivers);
	m_dwLastDrivers = dwRet;

	return true;
}

bool CDriveOP::GetCDROM(LPSTR lpCDROMs)
{
	return GetSpecialDisks(DRIVE_CDROM, lpCDROMs);
}

bool CDriveOP::GetRemovable( LPSTR lpRemovables )
{
	return GetSpecialDisks(DRIVE_REMOVABLE, lpRemovables);
}

bool CDriveOP::GetSpecialDisks( UINT flag, LPSTR lpDisks )
{
	if (m_dwLastDrivers == 0)
		return false;

	int size = m_vecDrivers.size();
	int pos = 0;

	bool bRet = false;

	for (int i=0; i < size; i++)
	{
		if (m_vecDrivers[i].DiskType == flag)
		{
			if (m_vecDrivers[i].DiskName == 'A' ||m_vecDrivers[i].DiskName == 'B')
			{
				continue;
			}
			bRet = true;
			pos = sprintf(lpDisks+pos, "%c:\\",m_vecDrivers[i].DiskName );
			pos++;
		}
	}
	sprintf(lpDisks+pos, "\0");

	return bRet;

}