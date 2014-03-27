#include "StdAfx.h"
#include "SystemResourceInfo.h"
#include "pdh.h"
#include "list"
#include "string"

using namespace std;

#pragma comment(lib, "Pdh.lib")

CSystemResourceInfo::CSystemResourceInfo(void)
{
	m_nUsedPercent = 0;
	m_nIO = 0;
}

CSystemResourceInfo::~CSystemResourceInfo(void)
{
}


double CSystemResourceInfo::GetCounter( LPCSTR pPath, DWORD nFormat,bool bTwoSampl )
{
	double ret = 0;
	// 要使用性能计数器的基本步骤是: 
	// 1.打开计数器PdhOpenQuery； 
	// 2.为计数器句柄分配空间； 
	// 3.把感兴趣的计数器添加进来PdhAddCounter； 
	// 4.收集数据PdhCollectQueryData 
	// 4.得到计数器的数值PdhGetFormattedCounterValue； 
	// 5.关闭计数器PdhCloseQuery。

	// open counter
	HQUERY hQuery = NULL;
	PDH_STATUS pdhStatus;
	HCOUNTER* pCounterHandle = NULL;

	try
	{
		pdhStatus = PdhOpenQuery(0, 0, &hQuery);

		if (ERROR_SUCCESS == pdhStatus)
		{
			pCounterHandle = (HCOUNTER*)GlobalAlloc(GPTR, sizeof(HCOUNTER));
		}
	}
	catch (...)
	{
	}

	// 创建计数器
	PDH_FMT_COUNTERVALUE fmtValue;
	DWORD dwctrType;
	try
	{
		pdhStatus = PdhAddCounter(hQuery, pPath, 0, pCounterHandle);

		if (ERROR_SUCCESS == pdhStatus)
		{
			pdhStatus = PdhCollectQueryData(hQuery);
			if (bTwoSampl)
			{
				Sleep(1000);
			}
			pdhStatus = PdhCollectQueryData(hQuery);
			// get value
			pdhStatus = PdhGetFormattedCounterValue(*pCounterHandle, nFormat, &dwctrType, &fmtValue);

			if (ERROR_SUCCESS == pdhStatus)
			{
				ret = fmtValue.doubleValue;
			}
			else
			{
				printf("--PdhGetFormattedCounterValue(%s) %X \n", pPath, pdhStatus);
			}
		}
	}
	catch (...)
	{
	}

	// free mem
	if (pCounterHandle)
	{
		::GlobalFree(pCounterHandle);
	}

	// close
	pdhStatus = PdhCloseQuery(hQuery);

	return ret;
}


CCpuRes::CCpuRes()
{
	m_eRes = CPU;
	m_strName = "cpu";
}

CCpuRes::~CCpuRes()
{

}

bool CCpuRes::GetInfor()
{
	m_nUsedPercent = int(GetCounter("\\Processor(_Total)\\% Processor Time", PDH_FMT_DOUBLE | PDH_FMT_NOCAP100));
	
	return true;
}

CMemoryRes::CMemoryRes()
{
	m_eRes = MEMORY;
	m_strName = "memory";

	// 物理内存大小
	MEMORYSTATUSEX mStatus={0}; 
	mStatus.dwLength = sizeof(mStatus);    
	GlobalMemoryStatusEx(&mStatus);
	m_PhysicalSize = mStatus.ullTotalPhys / 1024 / 1024;
}

CMemoryRes::~CMemoryRes()
{

}

bool CMemoryRes::GetInfor()
{
	m_Available = (int)GetCounter("\\Memory\\Available Mbytes", PDH_FMT_DOUBLE, false);
	m_nUsedPercent = (m_PhysicalSize - m_Available) * 100 / m_PhysicalSize; // 内存使用率

	return true;
}

CDiskRes::CDiskRes()
{
	m_eRes = DISK;
	m_strName = "disk";
}

CDiskRes::~CDiskRes()
{
}

bool CDiskRes::GetInfor()
{
	m_nIO = (int)GetCounter("\\PhysicalDisk(_Total)\\Disk Bytes/sec", PDH_FMT_DOUBLE) / 1024;
	
	return true;
}

CNetRes::CNetRes()
{
	m_eRes = NET;
	m_strName = "net";
}

CNetRes::~CNetRes()
{

}

bool CNetRes::GetInfor()
{
	const char* netface = "Network Interface";  
	const char* total_speed = "Bytes Total/sec";  

	list< HCOUNTER> m_counters;  
	HQUERY m_hQuery;  
	
	string counter_list;  
	string instance_list;  
	DWORD clistlen = 0;  
	DWORD ilistlen = 0;   

	PdhEnumObjectItemsA(0, 0, netface, 0, &clistlen, 0, &ilistlen, PERF_DETAIL_WIZARD, 0);  
	counter_list.assign(clistlen, 0);  
	instance_list.assign(ilistlen, 0);  
	if (ERROR_SUCCESS != PdhEnumObjectItemsA(0, 0, netface, &counter_list[0], &clistlen, &instance_list[0], &ilistlen, PERF_DETAIL_WIZARD, 0)) 
	{
		return false;
	} 

	PDH_STATUS pdhStatus;  
	pdhStatus = PdhOpenQuery (0, 0, &m_hQuery);  
	if ( pdhStatus != ERROR_SUCCESS )  
	{
		return false;
	}  

	char* tmpsz = &instance_list[0];  
	// 枚举网卡  
	for (; *tmpsz!=0; tmpsz+=(strlen(tmpsz)+1))  
	{  
		string query_obj_in = string("\\") + netface + "(" + tmpsz + ")" + "\\"  + total_speed;    

		HCOUNTER tmpcounter;                  
		pdhStatus = PdhAddCounterA( m_hQuery , query_obj_in.c_str(), 0 , &tmpcounter ) ;  
		if ( pdhStatus != ERROR_SUCCESS )  
		{  
			PdhCloseQuery(m_hQuery) ;  
			return false;  
		}  
		m_counters.push_back(tmpcounter);   
	}  

	// 统计流量
	PDH_FMT_COUNTERVALUE fmtValue;  
	DWORD dwctrType;  
	pdhStatus = PdhCollectQueryData(m_hQuery); 
	Sleep(500);
	pdhStatus = PdhCollectQueryData(m_hQuery);

	if (pdhStatus == ERROR_SUCCESS)  
	{  
		double t_bps = 0.0;   

		// 遍历网卡
		for (list< HCOUNTER>::iterator iter = m_counters.begin();  iter != m_counters.end();  iter++)  
		{  
			pdhStatus = PdhGetFormattedCounterValue(*iter , PDH_FMT_DOUBLE , &dwctrType, &fmtValue ) ;  
			if ( pdhStatus == ERROR_SUCCESS )  
			{  
				t_bps += fmtValue.doubleValue;  
			}    
		}

		m_nIO = int(t_bps / 1024);
	} 

	PdhCloseQuery(m_hQuery);

	return true;
}
