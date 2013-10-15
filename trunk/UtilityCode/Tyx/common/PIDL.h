
#pragma once

#include "comdef.h"
#include <vector>

HIMAGELIST GetSystemImageList(BOOL fSmall);


#define PPIDL_FROM_LPARAM(l)		(reinterpret_cast<CpidlWrapper*>(l))
#define PIDL_FROM_LPARAM(l)			(*reinterpret_cast<CpidlWrapper*>(l))

class CpidlWrapper
{
public:
    LPITEMIDLIST  m_pidl;

// == Construction/Destruction == //

    CpidlWrapper() : m_pidl(NULL) {}

    // Copy constructor
    CpidlWrapper(const CpidlWrapper& cpidl) : m_pidl(NULL) { Set(cpidl); }

    // From path (szPath relative to the folder psf) - see Set()
    CpidlWrapper(LPCSTR szPath, LPSHELLFOLDER psf = m_sfDesktop) : 
        m_pidl(NULL) { Set(szPath, psf); }

    // From a list ptr - *doesn't* copy the actual data - see Set()
    CpidlWrapper(LPITEMIDLIST pidl) : m_pidl(pidl) {}

    virtual ~CpidlWrapper();


// == Assignment == //

    // Make a copy of cpidl's list data
    HRESULT Set(const CpidlWrapper& cpidl);

    // Set by path: szPath relative to the folder psf.
    HRESULT Set(LPCSTR szPath, LPSHELLFOLDER psf = m_sfDesktop);

    // Points the CpidlWrapper to an existing item list: does *not* copy
    // the actual data - just the pointer (unlike MakeCopyOf()).
    HRESULT Set(LPITEMIDLIST pidl);

    // Special Assignment: Copies the data of an exisiting list.
    HRESULT MakeCopyOf(LPITEMIDLIST pidl);

    // Special Assignment: Makes a PIDL rooted at the desktop.
    HRESULT MakeAbsPIDLOf(LPSHELLFOLDER psf, LPITEMIDLIST pidl);


// == Item Access == //

    // Returns a pointer to the first item in the list
    LPSHITEMID GetFirstItemID() const { return (LPSHITEMID)m_pidl; }

    // Points to the next item in the list
    void GetNextItemID(LPSHITEMID& pid) const 
        { (LPBYTE &)pid += pid->cb; }


// == General Operations == //

    void Free();          // Frees the memory used by the item id list
    UINT GetSize() const; // Counts the actual memory in use

    // Split into direct parent and object pidls
    void Split(CpidlWrapper& parent, CpidlWrapper& obj) const;

    // Concatenation
    CpidlWrapper operator + (CpidlWrapper& pidl) const;  // using + operator
    static void Concat(const CpidlWrapper &a, const CpidlWrapper& b, 
        CpidlWrapper& result);                    // result = a+b (faster)


// == Shell Name-space Access Helper Functions == //

    // 1) Won't always work: psf->GetUIObjectOf(pidl, ... )
    // 2) Will always work:  pidl.GetUIObjectOf(..., psf)
    HRESULT GetUIObjectOf(REFIID riid, LPVOID *ppvOut, 
        HWND hWnd = NULL, LPSHELLFOLDER psf = m_sfDesktop);

    // Places the STRRET string in the cStr field.  
    void ExtractCStr(STRRET& strRet) const;


// == Conversion Operators == //

    operator LPITEMIDLIST&  () { return m_pidl; }
    operator LPITEMIDLIST * () { return &m_pidl; }
    operator LPCITEMIDLIST  () const { return m_pidl; }
    operator LPCITEMIDLIST* () const 
        { return (LPCITEMIDLIST *)&m_pidl; }

	CpidlWrapper& operator = (const CpidlWrapper& other)
	{
		if (this == &other)
		{
			return *this;
		}
		this->MakeCopyOf(other.m_pidl);
		return *this;
	}

protected:
    static LPSHELLFOLDER    m_sfDesktop;    // desktop object
    static LPMALLOC         m_pAllocator;   // system allocator

    // allocate memory for the pidl using the system allocator
    void AllocMem(ULONG_PTR iAllocSize);

    // initializer (used for automatic initialization)
    static struct pidl_initializer {
        pidl_initializer();
        ~pidl_initializer();
    } m_initializer;
    friend struct pidl_initializer;
};



typedef struct tagFolderTreeInsertStruct : private TVINSERTSTRUCT
{
	char m_szBuff[_MAX_PATH];
	char m_szPath[_MAX_PATH];

	DWORD m_dwAttributes;

	tagFolderTreeInsertStruct(HTREEITEM hParentSet = NULL)
	{
		hParent					 = hParentSet;
		itemex.lParam			 = 0;
		itemex.cChildren		 = 0;
		itemex.iImage			 = 0;
		itemex.iSelectedImage = 0;
		m_szBuff[0]				 = 0x00;
		m_szPath[0]				 = 0x00;
	}

	void Set(CpidlWrapper* ppidl, char* szText, DWORD dwAttributes, INT iImage, INT iSelectedImage, DWORD dwState = 0)
	{
		lstrcpy(m_szBuff, szText);
		itemex.lParam			 = reinterpret_cast<LPARAM>(ppidl);
		itemex.cChildren		 = dwAttributes & SFGAO_HASSUBFOLDER ? 1 : 0;
		itemex.iImage			 = iImage;
		itemex.iSelectedImage = iSelectedImage;
		itemex.state			 = dwState;
		itemex.stateMask		 = dwState;
		m_dwAttributes        = dwAttributes;
	}

	void DestroyPidl()
	{
		delete reinterpret_cast<CpidlWrapper*>(itemex.lParam);
		itemex.lParam = 0;
	}

	TVINSERTSTRUCT* GetTVINSERTSTRUCT(BOOL bIcons)
	{
		hInsertAfter	= TVI_LAST;
		itemex.mask		= TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN | TVIF_STATE | (bIcons ? TVIF_IMAGE | TVIF_SELECTEDIMAGE : 0);
		itemex.pszText	= m_szBuff;
		return static_cast<TVINSERTSTRUCT*>(this);
	}

} SFolderTreeInsertStruct;

typedef std::vector<SFolderTreeInsertStruct> V_InsertStruct;

