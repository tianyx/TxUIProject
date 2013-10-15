#pragma once

#include <vector>


class CDriveOP
{


	typedef struct _DriveItem
	{
		char DiskName;
		UINT DiskType;

	}DriveItem, *PDriveItem;



public:
	CDriveOP(void);
public:
	~CDriveOP(void);


	bool Refresh();
	void SetChanged() { m_bNeedRefresh = true; }
	bool IsChanged() const	{ return m_bNeedRefresh; }


	bool GetSpecialDisks(UINT flag, LPSTR lpDisks);

	// 参数字符串返回光驱的盘符。如果有多个光驱，则返回所有光驱的盘符，中间用0字符间隔，字符串末尾用两个0字符标记结束。
	bool GetCDROM(LPSTR lpCDROMs);

	// 参数用法同GetCDROM()
	bool GetRemovable(LPSTR lpRemovables);

private:
	DWORD m_dwLastDrivers;		// 上一次的盘符数字
	bool m_bNeedRefresh;		// 是否需要重新获取盘符
	std::vector<DriveItem> m_vecDrivers;		// 盘符数组
};
