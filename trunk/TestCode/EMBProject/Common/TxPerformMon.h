#pragma once

#include <string>
#include <vector>
#include "AutoCritSec.h"
using namespace std;

enum ENUMRESMONTYPE
{
	restype_Processor = 1,
	restype_PhysicalDisk,
	restype_Memory,
	restype_Network
};

#define PFOBJ_Network		TEXT("\\Network Interface")
#define PFOBJ_PhysicalDisk	TEXT("\\PhysicalDisk")
#define PFOBJ_Memory		TEXT("\\Memory")
#define PFOBJ_Processor		TEXT("\\Processor")

#define PFCNAME_ProcessorIdle TEXT("\\Processor(_Total)\\% Idle Time")
#define PFCNAME_NetworkBandwidth TEXT("\\Network Interface(*)\\Current Bandwidth")
#define PFCNAME_NetworkRealIO TEXT("\\Network Interface(*)\\Bytes Total/sec")
#define PFCNAME_DiskIdle TEXT("\\PhysicalDisk(_Total)\\% Idle Time")
#define PFCNAME_MemoryAvailableMBytes TEXT("\\Memory\\Available MBytes")


typedef vector<CString> VECOBJNAME;
typedef vector<CString> VECSUBITEMNAME;

class CTxPerformMon
{
public:
	CTxPerformMon(void);
	~CTxPerformMon(void);
	
	BOOL Run();
	BOOL Stop();

	BOOL GetUsage(ENUMRESMONTYPE typeIn);

public:
	BOOL PerfRunLoop();
private:
	HANDLE m_hThreadPerf;
	HANDLE m_hEnentQuit;
	
	CAutoCritSec m_csRes;
	int m_nProcessorUsage;
	int m_nDiskUsage;
	int m_nNetUsage;
	int m_nMemUsage;

	DWORDLONG  m_MemPhysicalSize;
	
public:
	static void GetAllObjName(VECOBJNAME& vObjOut);
	static void GetAllSubItem(LPCTSTR strObjIn, VECSUBITEMNAME& vItemsOut);

};
