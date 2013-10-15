
#include "stdafx.h"
#include "PIDL.h"

LPSHELLFOLDER CpidlWrapper::m_sfDesktop  = NULL; 
LPMALLOC      CpidlWrapper::m_pAllocator = NULL; 

CpidlWrapper::pidl_initializer CpidlWrapper::m_initializer; 


////////////////////////////////////////////////////////////////////
// Initialization object
////////////////////////////////////////////////////////////////////

// pidl_initializer is used to initialize the static data.  The 
// constructor and destructor are automatically called for us when
// the program starts/ends.

CpidlWrapper::pidl_initializer::pidl_initializer()
{
    SHGetDesktopFolder(&m_sfDesktop); // cache d'top folder obj ptr 
    SHGetMalloc(&m_pAllocator);       // cache sys allocator obj ptr
}

CpidlWrapper::pidl_initializer::~pidl_initializer()
{
    m_sfDesktop->Release();
    m_pAllocator->Release();
}


////////////////////////////////////////////////////////////////////
// CpidlWrapper Implementation
////////////////////////////////////////////////////////////////////

CpidlWrapper::~CpidlWrapper()
{
    Free();  // just free used memory
}


////////////////////////////////////////////////////////////////////
// Assignment Functions

HRESULT CpidlWrapper::Set(const CpidlWrapper& cpidl)
{
    return MakeCopyOf(cpidl.m_pidl);
}

HRESULT CpidlWrapper::Set(LPITEMIDLIST pidl)
{
    Free();
    m_pidl = pidl;
    return ERROR_SUCCESS;
}

HRESULT CpidlWrapper::Set(LPCSTR szPath, LPSHELLFOLDER psf)
{
    OLECHAR olePath[MAX_PATH];
    ULONG   chEaten;
    ULONG   dwAttributes;
    
    Free();
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, 
        olePath, MAX_PATH);
    return psf->ParseDisplayName(NULL, NULL, olePath, &chEaten, 
        &m_pidl, &dwAttributes);
}

HRESULT CpidlWrapper::MakeCopyOf(LPITEMIDLIST pidl)
{
    Free();
    if (pidl) {
        ULONG_PTR sz = m_pAllocator->GetSize((LPVOID)pidl);
        AllocMem(sz);
        CopyMemory((LPVOID)m_pidl, (LPVOID)pidl, sz);
    }
    return ERROR_SUCCESS;
}

HRESULT CpidlWrapper::MakeAbsPIDLOf(LPSHELLFOLDER psf, LPITEMIDLIST pidl)
{
    STRRET  str;
    
    HRESULT hr = psf->GetDisplayNameOf(pidl, SHGDN_FORPARSING, &str);
    if (SUCCEEDED(hr)) {
        ExtractCStr(str);
        hr = Set(str.cStr);
    }
    return hr;
}


////////////////////////////////////////////////////////////////////
// CpidlWrapper Operations

void CpidlWrapper::Free()
{
    if (m_pidl) {
        m_pAllocator->Free(m_pidl);
        m_pidl = NULL;
    }
}

#define CB_SIZE  (sizeof(piid->cb))  // size of termination item

UINT CpidlWrapper::GetSize() const
{
    UINT        cbTotal = 0;
    LPSHITEMID  piid    = GetFirstItemID();
    
    if (piid) {
        do {
            cbTotal += piid->cb;
            GetNextItemID(piid);
        } while (piid->cb);
        cbTotal += CB_SIZE; // count the terminator
    }
    
    return cbTotal;
}

void CpidlWrapper::Split(CpidlWrapper& parent, CpidlWrapper& obj) const
{
    int         size = 0;
    SHITEMID    *piid, *piidLast;
    
    // find last item-id and calculate total size of pidl
    piid = piidLast = &m_pidl->mkid;
    while (piid->cb)
    {
        piidLast = piid;
        size += (piid->cb);
        piid =  (SHITEMID *)((LPBYTE)piid + (piid->cb));
    }
    
    // copy parent folder portion
    size -= piidLast->cb;  // don't count "object" item-id
    parent.AllocMem(size + CB_SIZE);
    CopyMemory(parent.m_pidl, m_pidl, size);
    ZeroMemory((LPBYTE)parent.m_pidl + size, CB_SIZE); // terminator
    
    // copy "object" portion
    size = piidLast->cb + CB_SIZE;
    obj.AllocMem(size);
    CopyMemory(obj.m_pidl, piidLast, size);
}

CpidlWrapper CpidlWrapper::operator + (CpidlWrapper& pidl) const
{
    CpidlWrapper ret;
    Concat(*this, pidl, ret);
    return ret;
}

void CpidlWrapper::Concat(const CpidlWrapper &a, const CpidlWrapper& b, CpidlWrapper& result)
{
    result.Free();
    
    // both empty->empty | a empty->return b | b empty->return a
    if (a.m_pidl == NULL  &&  b.m_pidl == NULL) return;
    if (a.m_pidl == NULL) { result.Set(b); return; }
    if (a.m_pidl == NULL) { result.Set(a); return; }
    
    UINT cb1 = a.GetSize() - sizeof(a.m_pidl->mkid.cb);
    UINT cb2 = b.GetSize(); 
    result.AllocMem(cb1 + cb2); // allocate enough memory 
    CopyMemory(result.m_pidl, a.m_pidl, cb1);                 // 1st
    CopyMemory(((LPBYTE)result.m_pidl) + cb1, b.m_pidl, cb2); // 2nd
}

HRESULT CpidlWrapper::GetUIObjectOf(REFIID riid, LPVOID *ppvOut, 
    HWND hWnd /*= NULL*/, LPSHELLFOLDER psf /*= m_sfDesktop*/)
{
    CpidlWrapper           parent, obj;
    LPSHELLFOLDER   psfParent;
    
    Split(parent, obj);    
    
    HRESULT hr = psf->BindToObject(parent, NULL, IID_IShellFolder, (LPVOID *)&psfParent); // get the IShellFolder of the parent
    if (SUCCEEDED(hr)) 
	 {
        hr = psfParent->GetUIObjectOf(hWnd, 1, obj, riid, 0, ppvOut);
        psfParent->Release();
    }
	 else
	 {
		  return psf->GetUIObjectOf(hWnd, 1, obj, riid, 0, ppvOut);
	 }
    return hr;
}

void CpidlWrapper::ExtractCStr(STRRET& strRet) const
{
	switch (strRet.uType) 
	{
	case STRRET_WSTR:
		{
		// pOleStr points to a WCHAR string - convert it to ANSI
		LPWSTR pOleStr = strRet.pOleStr;
		WideCharToMultiByte(CP_ACP, 0, pOleStr, -1, strRet.cStr, MAX_PATH, NULL, NULL);
		m_pAllocator->Free(pOleStr);
		}
		break;

	case STRRET_OFFSET:

		// The string lives inside the pidl, so copy it out.
		lstrcpyn(strRet.cStr, (LPSTR)((LPBYTE)m_pidl + strRet.uOffset), MAX_PATH);
		break;
	}
}


////////////////////////////////////////////////////////////////////
// CpidlWrapper Private Operations

void CpidlWrapper::AllocMem(ULONG_PTR iAllocSize)
{
    Free();
    m_pidl = (LPITEMIDLIST)m_pAllocator->Alloc(iAllocSize);
}




///////////////////////////////////////////
// global functions

HIMAGELIST GetSystemImageList(BOOL fSmall)
{
	// get system image list:
	SHFILEINFO sfi;
	HIMAGELIST himl = (HIMAGELIST)::SHGetFileInfo(TEXT("C:\\"), 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | (fSmall ? SHGFI_SMALLICON : SHGFI_LARGEICON));

	// Do a version check first because you only need to use this code on
	// Windows NT version 4.0.
	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(vi);
	::GetVersionEx(&vi);
	if (VER_PLATFORM_WIN32_WINDOWS == vi.dwPlatformId)
		return himl;

	// You need to create a temporary, empty .lnk file that you can use to
	// pass to IShellIconOverlay::GetOverlayIndex. You could just enumerate
	// down from the Start Menu folder to find an existing .lnk file, but
	// there is a very remote chance that you will not find one. By creating
	// your own, you know this code will always work.
	HRESULT           hr;
	IShellFolder      *psfDesktop = NULL;
	IShellFolder      *psfTempDir = NULL;
	IMalloc           *pMalloc = NULL;
	LPITEMIDLIST      pidlTempDir = NULL;
	LPITEMIDLIST      pidlTempFile = NULL;
	TCHAR             szTempDir[MAX_PATH];
	TCHAR             szTempFile[MAX_PATH] = TEXT("");
	TCHAR             szFile[MAX_PATH];
	HANDLE            hFile;
	int               i;
	DWORD             dwAttributes;
	DWORD             dwEaten;
	IShellIconOverlay *psio = NULL;
	int               nIndex;

	// Get the desktop folder.
	hr = ::SHGetDesktopFolder(&psfDesktop);
	if (FAILED(hr)) goto exit;

	// Get the shell's allocator.
	hr = ::SHGetMalloc(&pMalloc);
	if (FAILED(hr)) goto exit;

	// Get the TEMP directory.
	if (!::GetTempPath(MAX_PATH, szTempDir))
	{
		// There might not be a TEMP directory. If this is the case, use the
		// Windows directory.
		if (!::GetWindowsDirectory(szTempDir, MAX_PATH))
		{
			hr = E_FAIL;
			goto exit;
		}
	}

	// Create a temporary .lnk file.
	if (szTempDir[lstrlen(szTempDir) - 1] != '\\')
		lstrcat(szTempDir, TEXT("\\"));
	for (i = 0, hFile = INVALID_HANDLE_VALUE ; INVALID_HANDLE_VALUE == hFile ; i++)
	{
		lstrcpy(szTempFile, szTempDir);
		wsprintf(szFile, TEXT("temp%d.lnk"), i);
		lstrcat(szTempFile, szFile);

		hFile = ::CreateFile(szTempFile,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		// Do not try this more than 100 times.
		if (i > 100)
		{
			hr = E_FAIL;
			goto exit;
		}
	}

	// Close the file you just created.
	::CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;

	// Get the PIDL for the directory.
	hr = psfDesktop->ParseDisplayName(  NULL,
		NULL,
		_bstr_t(szTempDir),
		&dwEaten,
		&pidlTempDir,
		&dwAttributes);
	if (FAILED(hr))
		goto exit;

	// Get the IShellFolder for the TEMP directory.
	hr = psfDesktop->BindToObject(   pidlTempDir,
		NULL,
		IID_IShellFolder,
		(LPVOID*)&psfTempDir);
	if (FAILED(hr))
		goto exit;


	// Get the IShellIconOverlay interface for this folder. If this fails,
	// it could indicate that you are running on a pre-Internet Explorer 4.0
	// shell, which doesn't support this interface. If this is the case, the
	// overlay icons are already in the system image list.
	hr = psfTempDir->QueryInterface(IID_IShellIconOverlay, (LPVOID*)&psio);
	if (FAILED(hr)) goto exit;

	// Get the PIDL for the temporary .lnk file.
	hr = psfTempDir->ParseDisplayName(  NULL,
		NULL,
		_bstr_t(szFile),//szOle,
		&dwEaten,
		&pidlTempFile,
		&dwAttributes);
	if (FAILED(hr))
		goto exit;

	// Get the overlay icon for the .lnk file. This causes the shell
	// to put all of the standard overlay icons into your copy of the system
	// image list.
	hr = psio->GetOverlayIndex(pidlTempFile, &nIndex);

exit:

	// Delete the temporary file.
	::DeleteFile(szTempFile);

	if (psio)
		psio->Release();

	if (INVALID_HANDLE_VALUE != hFile)
		CloseHandle(hFile);

	if (psfTempDir)
		psfTempDir->Release();

	if (pMalloc)
	{
		if (pidlTempFile)
			pMalloc->Free(pidlTempFile);

		if (pidlTempDir)
			pMalloc->Free(pidlTempDir);

		pMalloc->Release();
	}

	if (psfDesktop)
		psfDesktop->Release();

	return himl;

	return 0;
} 