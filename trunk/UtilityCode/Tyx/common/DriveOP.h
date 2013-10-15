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

	// �����ַ������ع������̷�������ж���������򷵻����й������̷����м���0�ַ�������ַ���ĩβ������0�ַ���ǽ�����
	bool GetCDROM(LPSTR lpCDROMs);

	// �����÷�ͬGetCDROM()
	bool GetRemovable(LPSTR lpRemovables);

private:
	DWORD m_dwLastDrivers;		// ��һ�ε��̷�����
	bool m_bNeedRefresh;		// �Ƿ���Ҫ���»�ȡ�̷�
	std::vector<DriveItem> m_vecDrivers;		// �̷�����
};
