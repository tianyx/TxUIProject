// FileTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "FileTreeCtrl.h"
#include "resource.h"
//#include "shfolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileTreeCtrl

LPSHELLFOLDER CFileTreeCtrl::m_lpsfDesktop=NULL;


CFileTreeCtrl::CFileTreeCtrl()
{
	SHGetDesktopFolder(&CFileTreeCtrl::m_lpsfDesktop);
}

CFileTreeCtrl::~CFileTreeCtrl()
{
	m_imgList.Detach();
}

BOOL CFileTreeCtrl::DisplayTree()
{
	DWORD dwStyle = GetStyle();   // read the windowstyle
    LPITEMIDLIST  lpi=NULL;
	LPMALLOC lpMalloc=NULL;
	
	CString Name;

	// Load Image Lists 
	if ( !GetSysImgList() )	return FALSE;
	m_imgState.Create( IDB_FILE_LIST_STATE, 16, 1, RGB(255,255,255) );
	SetImageList( &m_imgState, TVSIL_STATE );


	if (FAILED(SHGetMalloc(&lpMalloc))) return FALSE;
	
	if ( dwStyle & TVS_EDITLABELS ) 
	{
		// Don't allow the user to edit ItemLabels
		ModifyStyle( TVS_EDITLABELS , 0 );
	}
	
	// Initialize the tree view control to be empty.
	DeleteAllItems();
		
	// Add Desktop as Root Item
	SHGetSpecialFolderLocation (m_hWnd, CSIDL_DESKTOP, &lpi);
	GetName(m_lpsfDesktop, lpi, SHGDN_NORMAL, Name);
	m_hDesktop = CTreeCtrl::InsertItem( Name, GetIcon (lpi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON), 
								 GetIcon (lpi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON));


	// Fill in the tree view control from the root.
	FillTreeView(m_lpsfDesktop, NULL, m_hDesktop);
	SetItemState(m_hDesktop,NOSTATE, NONE);
	Expand(m_hDesktop,TVE_EXPAND);

	if (lpi) lpMalloc->Free(lpi);
	if (!lpMalloc) lpMalloc->Release();
	return TRUE;
}

BOOL CFileTreeCtrl::RefreshTree()
{
	
	return SetItemState(m_hDesktop, REFRESH, ALL);
}


BOOL CFileTreeCtrl::GetSysImgList()
{
	SHFILEINFO shFinfo;
	HIMAGELIST hImgList = NULL;
	
	if ( GetImageList( TVSIL_NORMAL ) )
		m_imgList.Detach();
	
	hImgList = (HIMAGELIST)SHGetFileInfo( TEXT("C:\\"),
		0,
		&shFinfo,
		sizeof( shFinfo ),
		SHGFI_SYSICONINDEX | 
		SHGFI_SMALLICON );
	if ( !hImgList )
	{
		m_strError = "Cannot retrieve the Handle of SystemImageList!";
		return FALSE;
	}
	
	m_imgList.m_hImageList = hImgList;    
    
	SetImageList( &m_imgList, TVSIL_NORMAL );
	return TRUE;   // OK
}

/****************************************************************************
*
*  FUNCTION: FillTreeView( LPSHELLFOLDER lpsf,
*                          LPITEMIDLIST  lpifq,
*                          HTREEITEM     hParent)
*
*  PURPOSE: Fills a branch of the TreeView control.  Given the
*           shell folder, enumerate the subitems of this folder,
*           and add the appropriate items to the tree.
*
*  PARAMETERS:
*    lpsf         - Pointer to shell folder that we want to enumerate items 
*    lpifq        - Fully qualified item id list to the item that we are enumerating
*                   items for.  In other words, this is the PIDL to the item
*                   identified by the lpsf parameter.
*    hParent      - Parent node
*
*  COMMENTS:
*    This function enumerates the items in the folder identifed by lpsf.
*
****************************************************************************/
bool CFileTreeCtrl::FillTreeView (LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, HTREEITEM hParent)
{
    TV_ITEM         tvi;            // tree view item
    TV_INSERTSTRUCT tvins;          // tree view insert structure
    HTREEITEM       hPrev = NULL;   // previous item added
    LPSHELLFOLDER   lpsf2=NULL;
    LPENUMIDLIST    lpe=NULL;
    LPITEMIDLIST    lpi=NULL, lpiTemp=NULL, lpifqThisItem;
    LPTVITEMDATA    lptvid=NULL, lptvid1=NULL;
    LPMALLOC        lpMalloc=NULL;
    ULONG           ulFetched;
    UINT            uCount=0;
    HRESULT         hr;						   
    CString         Name,Path;
	TV_SORTCB       tvscb;
	bool			bProcess;
	bool			bHasChildren;
	bool			bResult=FALSE;
	bool			bRefresh=FALSE;


    // Allocate a shell memory object. 
    hr=::SHGetMalloc(&lpMalloc);
    if (FAILED(hr))
		return FALSE;
	
	// Get the IEnumIDList object for the given folder.
	hr=lpsf->EnumObjects(m_hWnd, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &lpe);
	
	if (SUCCEEDED(hr))
	{
		lptvid1 = (LPTVITEMDATA) GetItemData(hParent);
				
		if (hParent == m_hDesktop || (lptvid1 && lptvid1->nNumFiles!=-1)) 
		{
			bRefresh=TRUE;
			HTREEITEM hOldItem;
			LPTVITEMDATA lpItemData;
			if (hParent) hOldItem = GetChildItem(hParent);
			else hOldItem = GetNextItem(NULL,TVGN_ROOT);
			while (hOldItem)
			{
				lpItemData = (LPTVITEMDATA) GetItemData(hOldItem);
				if (lpItemData) lpItemData->bRefresh = FALSE;
				hOldItem = GetNextSiblingItem(hOldItem);
			}
		}

		if (lptvid1) 
		{
			lptvid1->nNumDirs = 0;
			lptvid1->nNumFiles = 0;
			lptvid1->nSelDirs = 0;
			lptvid1->nPartSelDirs = 0;
			lptvid1->nSelFiles = 0;
		}
		// Enumerate throught the list of folder and nonfolder objects.
		while (S_OK==lpe->Next(1, &lpi, &ulFetched))
		{
			// Get the friendly name to 
			// put in the tree view control.
			if (!GetName(lpsf, lpi, SHGDN_NORMAL, Name)) goto Done; // Error - could not get friendly name
			if (!GetName(lpsf, lpi, SHGDN_FORPARSING, Path)) goto Done;
			
			// Determine what type of object you have.
			ULONG ulAttrs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER ;
			lpsf->GetAttributesOf (1, (const struct _ITEMIDLIST **)&lpi, &ulAttrs);
			
			// Check whether the item needs to be inserted and has children
			bHasChildren=FALSE;
			bProcess=FALSE;
			if (ulAttrs & SFGAO_FOLDER) 
			{
				if (!IsHidFolder(Path)) {
					bProcess=TRUE;
					bResult=TRUE;
					if (ulAttrs & SFGAO_HASSUBFOLDER) bHasChildren=TRUE;
					else bHasChildren=CheckForMatchedFiles(Path);
				}
			}
			else if (MatchExtension(Path)) 
			{
				bProcess=TRUE;
				bResult=TRUE;
			}
			
			// Insert the item
			if (bProcess) 
			{ 

				tvi.mask= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
				
				if (bHasChildren)
				{
					// This item has subfolders, so put a plus sign in
					// the tree view control. The first time the user clicks
					// the item, you should populate the sub-folders.
					tvi.cChildren=1;
					tvi.mask |= TVIF_CHILDREN;
				}
				
				// Get some memory for the ITEMDATA structure.
				lptvid = (LPTVITEMDATA)lpMalloc->Alloc(sizeof(TVITEMDATA));
				if (!lptvid) goto Done;  // Error - could not allocate memory.

				lptvid->FullName = new CString(Path);
				lptvid->sPath = new CString(Name);
				
				tvi.pszText    = Name.GetBuffer(10); 
				Name.ReleaseBuffer();
				tvi.cchTextMax = MAX_PATH;
				
				lpifqThisItem=ConcatPidls(lpifq, lpi);
				
				// Now make a copy of the ITEMIDLIST
				lptvid->lpi=CopyITEMID(lpMalloc, lpi);
				
				GetNormalAndSelectedIcons(lpifqThisItem, &tvi);
				
				lptvid->lpsfParent=lpsf;    // store the parent folder's SF
				lpsf->AddRef();
				
				lptvid->lpifq=ConcatPidls(lpifq, lpi);
				lptvid->nNumDirs = -1;
				lptvid->nNumFiles = -1;
				lptvid->nSelDirs = -1;
				lptvid->nPartSelDirs = -1;
				lptvid->nSelFiles = -1;

				if (ulAttrs & SFGAO_FOLDER) 
				{
					lptvid->IsFolder=TRUE; 
					if (bHasChildren) { if (lptvid1) lptvid1->nNumDirs++; }
					else 
					{
						lptvid->nNumDirs = 0;
						lptvid->nNumFiles = 0;
						lptvid->nSelDirs = 0;
						lptvid->nPartSelDirs = 0;
						lptvid->nSelFiles = 0;
						tvi.mask |= TVIF_STATE;
						tvi.state = INDEXTOSTATEIMAGEMASK(DISABLED);
						tvi.stateMask = TVIS_STATEIMAGEMASK;
					}
				}
				else 
				{
					lptvid->IsFolder=FALSE;
					if (lptvid1) lptvid1->nNumFiles++;
				}
				
				tvi.lParam = (LPARAM)lptvid;

				// Populate the tree view insert structure.
				tvins.item         = tvi;
				tvins.hInsertAfter = TVI_FIRST;
				tvins.hParent      = hParent;
				
				// Add the item to the tree.
				hPrev = InsertItem(&tvins, bRefresh);

				// Free the task allocator for this item.
				lpMalloc->Free(lpifqThisItem);  
				lpifqThisItem=0;
			}
			
			lpMalloc->Free(lpi);  // free PIDL the shell gave you
			lpi=0;
					
		}
		if (lptvid1 && lptvid1->nNumFiles==0 && lptvid1->nNumDirs==0) SetItemState(hParent, DISABLED, PARENTS);
		/*if (bRefresh) 
		{
			HTREEITEM hOldItem;
			LPTVITEMDATA lpItemData;
			if (hParent) hOldItem = GetChildItem(hParent);
			else hOldItem = GetNextItem(NULL,TVGN_ROOT);
			while (hOldItem)
			{
				hPrev = GetNextSiblingItem(hOldItem); 
				lpItemData = (LPTVITEMDATA) GetItemData(hOldItem);
				if (lpItemData && !lpItemData->bRefresh) 
				{
					if (lpItemData->IsFolder) DeleteChildren(hOldItem);
					DeleteItem(hOldItem);
				}
				hOldItem = hPrev;
			}
		}*/
		tvscb.hParent     = hParent;
		tvscb.lParam      = 0;
		tvscb.lpfnCompare = TreeViewCompareProc;
		
		SortChildrenCB(&tvscb);
		if (!bRefresh) SetItemState (hParent, REFRESH, CHILDREN); 
			else SetItemState(hParent, REFRESH);
	}
	
Done:

	if (lpe) lpe->Release();
	
	// The following two if statements will be TRUE only if you got here
	// on an error condition from the goto statement.  Otherwise, free 
	// this memory at the end of the while loop above.
	if (lpi && lpMalloc)           
		lpMalloc->Free(lpi);
	if (lpifqThisItem && lpMalloc) 
		lpMalloc->Free(lpifqThisItem);  
	
	if (lpMalloc) 
		lpMalloc->Release();

	return bResult;
}

/****************************************************************************
*
*    FUNCTION: GetIcon(LPITEMIDLIST lpi, UINT uFlags)
*
*    PURPOSE:  Gets the index for the current icon.  Index is index into system
*              image list.
*
*  PARAMETERS:
*    lpi    - Fully qualified item id list for current item.
*    uFlags - Flags for SHGetFileInfo()
*
*  RETURN VALUE:
*    Icon index for current item.
****************************************************************************/
int CFileTreeCtrl::GetIcon(LPITEMIDLIST lpi, UINT uFlags)
{
	SHFILEINFO    sfi;
	
	SHGetFileInfo((LPCTSTR)lpi, 0, &sfi, sizeof(SHFILEINFO), uFlags);
	
	return sfi.iIcon;
}

/****************************************************************************
*
*    FUNCTION: GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
*
*    PURPOSE:  Gets the index for the normal and selected icons for the current item.
*
*    PARAMETERS:
*    lpifq    - Fully qualified item id list for current item.
*    lptvitem - Pointer to treeview item we are about to add to the tree.
*
****************************************************************************/
void CFileTreeCtrl::GetNormalAndSelectedIcons (LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
{
	// Don't check the return value here. 
	// If IGetIcon() fails, you're in big trouble.
	lptvitem->iImage = GetIcon (lpifq, 
		SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	
	lptvitem->iSelectedImage = GetIcon (lpifq, 
		SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
	
	return;
}



/****************************************************************************
*
*    FUNCTION: TreeViewCompareProc(LPARAM, LPARAM, LPARAM)
*
*    PURPOSE:  Callback routine for sorting the tree 
*
****************************************************************************/
int CALLBACK CFileTreeCtrl::TreeViewCompareProc(LPARAM lparam1, 
												LPARAM lparam2, LPARAM lparamSort)
{
    LPTVITEMDATA lptvid1=(LPTVITEMDATA)lparam1;
    LPTVITEMDATA lptvid2=(LPTVITEMDATA)lparam2;
	if (lptvid1->IsFolder && !lptvid2->IsFolder) return -1;
	if (!lptvid1->IsFolder && lptvid2->IsFolder) return 1;

	HRESULT h = m_lpsfDesktop->CompareIDs(lparamSort, lptvid1->lpifq,lptvid2->lpifq);
	return (short)HRESULT_CODE(h);
}	


// 
// FUNCTIONS THAT DEAL WITH PIDLs
//
/****************************************************************************
*
*    FUNCTION: Next(LPCITEMIDLIST pidl)
*
*    PURPOSE:  Gets the next PIDL in the list 
*
****************************************************************************/
LPITEMIDLIST CFileTreeCtrl::Next(LPCITEMIDLIST pidl)
{
	LPSTR lpMem=(LPSTR)pidl;
	
	lpMem+=pidl->mkid.cb;
	
	return (LPITEMIDLIST)lpMem;
}

/****************************************************************************
*
*    FUNCTION: GetSize(LPCITEMIDLIST pidl)
*
*    PURPOSE:  Gets the size of the PIDL 
*
****************************************************************************/
UINT CFileTreeCtrl::GetSize(LPCITEMIDLIST pidl)
{
    UINT cbTotal = 0;
    if (pidl)
    {
        cbTotal += sizeof(pidl->mkid.cb);       // Null terminator
        while (pidl->mkid.cb)
        {
            cbTotal += pidl->mkid.cb;
            pidl = Next(pidl);
        }
    }
	
    return cbTotal;
}

/****************************************************************************
*
*    FUNCTION: Create(UINT cbSize)
*
*    PURPOSE:  Allocates a PIDL 
*
****************************************************************************/
LPITEMIDLIST CFileTreeCtrl::Create(UINT cbSize)
{
    LPMALLOC lpMalloc;
    HRESULT  hr;
    LPITEMIDLIST pidl=NULL;
	
    hr=SHGetMalloc(&lpMalloc);
	
    if (FAILED(hr))
		return 0;
	
    pidl=(LPITEMIDLIST)lpMalloc->Alloc(cbSize);
	
    if (pidl)
        memset(pidl, 0, cbSize);      // zero-init for external task   alloc
	
    if (lpMalloc) lpMalloc->Release();
	
    return pidl;
}

/****************************************************************************
*
*    FUNCTION: ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
*
*    PURPOSE:  Concatenates two PIDLs 
*
****************************************************************************/
LPITEMIDLIST CFileTreeCtrl::ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
    LPITEMIDLIST pidlNew;
    UINT cb1;
    UINT cb2;
	
    if (pidl1)  //May be NULL
		cb1 = GetSize(pidl1) - sizeof(pidl1->mkid.cb);
    else
		cb1 = 0;
	
    cb2 = GetSize(pidl2);
	
    pidlNew = Create(cb1 + cb2);
    if (pidlNew)
    {
        if (pidl1)
			memcpy(pidlNew, pidl1, cb1);
        memcpy(((LPSTR)pidlNew) + cb1, pidl2, cb2);
    }
    return pidlNew;
}

/****************************************************************************
*
*    FUNCTION: CopyITEMID(LPMALLOC lpMalloc, LPITEMIDLIST lpi)
*
*    PURPOSE:  Copies the ITEMID 
*
****************************************************************************/
LPITEMIDLIST CFileTreeCtrl::CopyITEMID(LPMALLOC lpMalloc, LPITEMIDLIST lpi)
{
	LPITEMIDLIST lpiTemp;
	
	lpiTemp=(LPITEMIDLIST)lpMalloc->Alloc(lpi->mkid.cb+sizeof(lpi->mkid.cb));
	CopyMemory((PVOID)lpiTemp, (CONST VOID *)lpi, lpi->mkid.cb+sizeof(lpi->mkid.cb));
	
	return lpiTemp;
}

/****************************************************************************
*
*    FUNCTION: GetName(LPSHELLFOLDER lpsf,LPITEMIDLIST  lpi,DWORD dwFlags,
*             LPSTR         lpFriendlyName)
*
*    PURPOSE:  Gets the friendly name for the folder 
*
****************************************************************************/
BOOL CFileTreeCtrl::GetName (LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, 
							 DWORD dwFlags, CString &strName)
{
	BOOL   bSuccess=TRUE;
	STRRET str;
	
	if (NOERROR==lpsf->GetDisplayNameOf(lpi,dwFlags, &str))
	{
		LPTSTR lpStr;
		StrRetToStr(&str,lpi,&lpStr);
		strName = lpStr;
		CoTaskMemFree(lpStr);
	}
	else
		bSuccess = FALSE;
	
	return bSuccess;
}

/****************************************************************************
*
*    FUNCTION: GetFullyQualPidl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi)
*
*    PURPOSE:  Gets the Fully qualified Pidls for the folder 
*
****************************************************************************/
LPITEMIDLIST CFileTreeCtrl::GetFullyQualPidl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi)
{
	CString Name;
	LPOLESTR szOleStr;
	LPITEMIDLIST  lpifq;
	ULONG ulEaten, ulAttribs;
	HRESULT hr;
	
	if (!GetName(lpsf, lpi, SHGDN_FORPARSING, Name))
			return NULL;
	
	szOleStr = Name.AllocSysString();

	hr=m_lpsfDesktop->ParseDisplayName(NULL,
		NULL,
		szOleStr,
		&ulEaten,
		&lpifq,
		&ulAttribs);

//	Name.ReleaseBuffer();	
	
	if (FAILED(hr))
		return NULL;
	
	return lpifq;
}

bool CFileTreeCtrl::MatchExtension(CString file)
{
	return TRUE;
//	CString strTemp = file.Right(4);
//  if (!strTemp.CollateNoCase(".EXE")) return TRUE;
//	return FALSE;
}

bool CFileTreeCtrl::CheckForMatchedFiles(CString Path)
{
	CFileFind finder;
	int bWorking;
	int iStartTime;

	Path = Path + TEXT("\\*.*");
    bWorking = finder.FindFile(Path);
	iStartTime=GetTickCount();

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if ( !finder.IsDirectory() && MatchExtension(finder.GetFileName()) || GetTickCount()-iStartTime>500)
		{
			finder.Close();
			return TRUE;
		}
	}
	
	finder.Close();
	return FALSE;
}


//FUNCTIONS FOR HIDDEN FOLDERS LIST

void CFileTreeCtrl::FreeHidFoldersList()
{
	m_HidFolders.RemoveAll();
}

void CFileTreeCtrl::AddHidFolder(int nFolder, CString strPath)
{
	LPITEMIDLIST pidl=NULL;
	LPMALLOC lpMalloc=NULL;
	LPSHELLFOLDER lpsf=NULL;

	if (nFolder) 
	{

		if (FAILED(SHGetMalloc(&lpMalloc)))	return;
		if (FAILED(SHGetSpecialFolderLocation(NULL,nFolder,&pidl))) goto Done;
		if (FAILED(SHGetDesktopFolder(&lpsf))) goto Done;
		if (!GetName(lpsf, pidl, SHGDN_FORPARSING, strPath)) goto Done;
		
	}
	
	m_HidFolders.Add(strPath);
	
Done:

	if (lpsf) lpsf->Release();
	if (pidl) lpMalloc->Free(pidl);
	if (lpMalloc) lpMalloc->Release();
}


BOOL CFileTreeCtrl::IsHidFolder(CString Name)
{
	int N=m_HidFolders.GetUpperBound();
	
	if (N!=-1) {
		// Make sure it is a REAL folder, not a fake (like Zip-folders in XP)
		DWORD iAttr=GetFileAttributes(Name);
		if (!(FILE_ATTRIBUTE_DIRECTORY & iAttr)) return TRUE;

		// Check if it is not in Hidden Folders list
		for (int i=0; i<=N; i++) 
			if (Name == m_HidFolders[i]) return TRUE;
	}
	return FALSE;
}


// FUNCTIONS FOR INSERTING & DELETING ITEMS

HTREEITEM CFileTreeCtrl::InsertItem(LPTVINSERTSTRUCT lpInsertStruct, bool bRefresh)
{
	return CTreeCtrl::InsertItem(lpInsertStruct);
}

UINT CFileTreeCtrl::DeleteChildren (HTREEITEM hItem)
{
	UINT nCount = 0;
	HTREEITEM hChild = GetChildItem(hItem);

	while (hChild)
	{
		HTREEITEM hNextItem = GetNextSiblingItem(hChild);
		DeleteItem(hChild);
		hChild = hNextItem;
		nCount++;
	}
	return nCount;
}


//FUNCTIONS FOR HANDLING CHECKBOXES

// nAction tells what to update and can be NONE, CHILDREN, PARENTS, ALL

BOOL CFileTreeCtrl::SetItemState(HTREEITEM hItem, int newState, int nAction)
{
	
	if( hItem == NULL ) 
	{
		m_strError = "NULL item in CFileTreeCtrl::SetItemState!";
		return FALSE;
	}

	if (newState != REFRESH && newState != UNCHECKED && newState != UNCHECKED_REAL 
		&& newState != CHILD_FILE && newState != CHILD_ALLFILES && newState != CHILD_NOFILE_FOLDER
		&& newState != FILE && newState != CHILD_FILE_FOLDER && newState != CHILD_ALLFILES_FOLDER 
		&& newState != DISABLED && newState != NOSTATE && newState != CHILD_NOFILE_ALLFOLDERS
		&& newState != CHILD_FILE_ALLFOLDERS && newState != CHILD_ALL) return FALSE;
	
			
	LPTVITEMDATA fInfo;
	UINT oldParentState, newParentState;
	int oldState = GetItemState(hItem, &fInfo);
		
	if (oldState == newState) return FALSE;
	if (fInfo && fInfo->IsFolder && newState == FILE) return FALSE;
	if (fInfo && !fInfo->IsFolder && newState != FILE && newState != UNCHECKED && newState != DISABLED && newState !=REFRESH) return FALSE;
	
	if (fInfo) 
	{
		if (fInfo->nNumDirs==-1 && fInfo->IsFolder) 
		{
			CString sName=TEXT("");
			switch (newState)
			{
			case CHILD_ALLFILES:
				sName+="f"; 
				sName+=*(fInfo->FullName);
				SelFolders.SetAt(hItem,sName);
				break;
				
			case CHILD_NOFILE_ALLFOLDERS:
				sName+="F";
				sName+=*(fInfo->FullName);
				SelFolders.SetAt(hItem,sName);
				break;
				
			case CHILD_ALL:
				sName+="A"; 
				sName+=*(fInfo->FullName);
				SelFolders.SetAt(hItem,sName);
				break;
				
			case UNCHECKED:
				SelFolders.RemoveKey(hItem);
			}
		}
		
		if (!fInfo->IsFolder) 
		{
			if (newState==FILE) SelFiles.SetAt(hItem,*(fInfo->FullName));
			if (newState==UNCHECKED) SelFiles.RemoveKey(hItem);
		}
	}

	// SET THE STATE OF THE ITEM
	if (newState != REFRESH)  // The item doesn't need to be refreshed
	{
		int nState = newState;
		
		if (nState == NOSTATE) nState = 0;
		else if (nState == UNCHECKED) nState = UNCHECKED_REAL;
		
		if (!CTreeCtrl::SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK )) return FALSE;
	} 
	
	else
		// If... Child items are refreshed according to parent's state
		if (nAction == CHILDREN && oldState != UNCHECKED) newState = oldState; 

		// If... Parent item is refreshed according to children's states
		else if (nAction == ALL) 
		{
			if (fInfo && fInfo->nNumFiles!=-1) 
			{
				fInfo->nNumDirs=0;
				fInfo->nNumFiles=0;
				fInfo->nPartSelDirs=0;
				fInfo->nSelDirs=0;
				fInfo->nSelFiles=0;
			}

		}
		else return FALSE;
		
	if (nAction != NONE) 
	{
		// UPDATE CHILDREN OF THE FOLDER
		if ((nAction == CHILDREN || nAction == ALL) && 
			(newState == UNCHECKED || newState == CHILD_ALLFILES 
			|| newState == CHILD_NOFILE_ALLFOLDERS || newState == CHILD_ALL
			|| newState == REFRESH || newState == DISABLED || newState == NOSTATE))
		{
			HTREEITEM hChild = GetChildItem(hItem);

			while (hChild)
			{
				
				LPTVITEMDATA fChildInfo;
				int ChildState = GetItemState(hChild, &fChildInfo);
				
				if (ChildState!=DISABLED && ChildState!=NOSTATE) 
				{
					switch (newState)
					{
					case UNCHECKED: SetItemState(hChild, UNCHECKED); break;
					case CHILD_ALLFILES:
						if (fChildInfo->IsFolder) SetItemState(hChild, UNCHECKED);
						else SetItemState(hChild, FILE); break;
					case CHILD_NOFILE_ALLFOLDERS:
						if (fChildInfo->IsFolder) 
						{
							if (fChildInfo->nNumDirs) SetItemState(hChild, CHILD_ALL);
							else SetItemState(hChild, CHILD_ALLFILES);
						}
						else SetItemState(hChild, UNCHECKED); break;
					case CHILD_ALL:
						if (fChildInfo->IsFolder) SetItemState(hChild, CHILD_ALL);
						else SetItemState(hChild, FILE); break;
					case DISABLED:
					case NOSTATE:
						SetItemState(hChild,newState);
						break;
					}
				} 
				if (newState == REFRESH) 
				{
					if (fChildInfo->IsFolder) 
					{
						SetItemState(hChild, REFRESH, ALL);
						if (fInfo) 
						{
							int ChildState = GetItemState(hChild, &fChildInfo);
							if (ChildState != DISABLED)
							{
								fInfo->nNumDirs++;
								if (SelectedCompletely(ChildState, fChildInfo)) fInfo->nSelDirs++;
								else if (ChildState != UNCHECKED) fInfo->nPartSelDirs++;
							}
						}
					}
					else if (fInfo && ChildState != DISABLED) 
					{
						fInfo->nNumFiles++;
						if (ChildState == FILE) fInfo->nSelFiles++;
					}
				}
				hChild = GetNextItem(hChild, TVGN_NEXT);
			}
			
			if (fInfo && newState == REFRESH) 
			{
				

				if (!fInfo->nNumDirs && !fInfo->nNumFiles) 
				{ 
					newState = DISABLED;
					goto Quit1;
				}
				
				if (fInfo->nNumFiles == -1) 
				{
					if (fInfo->IsFolder && oldState != UNCHECKED 
						&& oldState != CHILD_ALLFILES && oldState != CHILD_NOFILE_ALLFOLDERS
						&& oldState != CHILD_ALL) return ToggleForward(hItem);
					return TRUE;
				}

				newState = 0;

				if (fInfo->nNumFiles) 
				{
					if (fInfo->nNumFiles == fInfo->nSelFiles) newState |= 3;
					else if (fInfo->nSelFiles) newState |= 2;
				}
				else if (fInfo->nSelDirs == fInfo->nNumDirs) newState |= 3;
				
				if (fInfo->nSelDirs || fInfo->nPartSelDirs)
				{
					if (fInfo->nSelDirs == fInfo->nNumDirs) newState |= 12;
					else newState |= 4;
				}
Quit1:
				if (oldState != newState) 
				{
					SetItemState(hItem, newState, NONE);
				}	
				return TRUE;
			} 
		}
		
		// UPDATE PARENTS OF THE FOLDER
		if (nAction == PARENTS || nAction == ALL)
		{
			
			HTREEITEM hParent = GetParentItem(hItem);
			if (!hParent) return FALSE;
			if (hParent == m_hDesktop) return TRUE; // The end of update recursion
			
			LPTVITEMDATA lptvid;
			oldParentState = GetItemState(hParent, &lptvid);
			if (oldParentState == NOSTATE) return FALSE;
			if (!lptvid) return FALSE;
			
			// CORRECT NUMBER OF SELECTED FOLDERS AND FILES IN PARENT FOLDER
			
			if (newState == DISABLED) // Folder is disabled
			{
				if (!fInfo->IsFolder) 
				{
					lptvid->nNumFiles--;
					if (oldState == FILE) lptvid->nSelFiles--;
				}
				else 
				{
					lptvid->nNumDirs--;
					if (SelectedCompletely(oldState,lptvid)) lptvid->nSelDirs--;
					else if (oldState != UNCHECKED) lptvid->nPartSelDirs--;
				}
				newParentState = oldParentState;
				goto Done;
			}
			
			if (oldState == UNCHECKED)  // Item was unchecked 
			{
				// File selected
				if (newState == FILE) lptvid->nSelFiles++;
				
				// Folder got selected completely
				else if (SelectedCompletely(newState,fInfo)) lptvid->nSelDirs++;
				
				// Folder got selected partially
				else lptvid->nPartSelDirs++;
			}
			
			else // Item was already selected 
			{
				// File unchecked
				if (oldState == FILE) lptvid->nSelFiles--;
				
				// Folder got selected completely
				else if (!SelectedCompletely(oldState,fInfo) && SelectedCompletely(newState,fInfo))
				{
					lptvid->nPartSelDirs--;
					lptvid->nSelDirs++;
				}
				
				// Folder got unchecked...
				else if (newState == UNCHECKED) 
				{
					// ...after being selected completely
					if (SelectedCompletely(oldState, fInfo)) lptvid->nSelDirs--;
					// ...after being selected partially
					else lptvid->nPartSelDirs--;
				}
				
				// Folder got selected partially after being selected completely
				else if (SelectedCompletely(oldState, fInfo) && !SelectedCompletely(newState, fInfo))
				{
					lptvid->nPartSelDirs++;
					lptvid->nSelDirs--;
				}
			}
			
			// UPDATE PARENT ITEM STATE
			
			newParentState = 0;
			
Done:	
			
			if (newState == DISABLED && !lptvid->nNumDirs && !lptvid->nNumFiles) 
			{ 
				newParentState = DISABLED;
				goto Quit;
			}
			
			if (lptvid->nNumFiles) 
			{
				if (lptvid->nNumFiles == lptvid->nSelFiles) newParentState |= 3;
				else if (lptvid->nSelFiles) newParentState |= 2;
			}
			else if (lptvid->nSelDirs == lptvid->nNumDirs) newParentState |= 3;
			
			if (lptvid->nSelDirs || lptvid->nPartSelDirs)
			{
				if (lptvid->nSelDirs == lptvid->nNumDirs) newParentState |= 12;
				else newParentState |= 4;
			}
Quit:
			if (oldParentState != newParentState) SetItemState(hParent, newParentState, PARENTS);
		}
	}

	return TRUE;
}

int CFileTreeCtrl::GetItemState(HTREEITEM hItem, LPTVITEMDATA* fInfo)
{
	int nState;
	if ( hItem == NULL ) {
		m_strError = "NULL item in CFileTreeCtrl::GetItemState!";
		return -1;
	}
	nState = CTreeCtrl::GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
	if (nState == 0) nState = NOSTATE;
	if (nState == UNCHECKED_REAL) nState = UNCHECKED;
	if (fInfo)	*fInfo = (LPTVITEMDATA) GetItemData(hItem);
	return nState;
}

BOOL CFileTreeCtrl::CheckItem(HTREEITEM hItem)
{
	if( hItem == NULL ) return FALSE;

	UINT newState;
	LPTVITEMDATA lptvid;

	UINT nState = GetItemState(hItem, &lptvid);
	if (nState == NOSTATE || nState == DISABLED) return FALSE;
		
	// Determine the new check state

	if(nState == FILE || nState == CHILD_ALL) return TRUE;
	
	switch( nState ) 
	{
		case CHILD_FILE:	
		case CHILD_FILE_FOLDER:
		case CHILD_FILE_ALLFOLDERS: 
			newState = nState + 1; 
			break;

		case CHILD_NOFILE_FOLDER:
		case CHILD_NOFILE_ALLFOLDERS: 
			newState = nState + 3; 
			break;

		case UNCHECKED:
			if (lptvid->IsFolder) newState = CHILD_ALLFILES;
			else newState = FILE;
			break;

		case CHILD_ALLFILES_FOLDER: 
		case CHILD_ALLFILES:
			newState = CHILD_ALL;
			break;
	}

	SetItemState(hItem, newState, ALL); 
	return TRUE;
}

BOOL CFileTreeCtrl::UncheckItem(HTREEITEM hItem)
{
	UINT newState;
	if( hItem == NULL ) return FALSE;

	LPTVITEMDATA lptvid;
	UINT nState = GetItemState( hItem, &lptvid);
	if (nState == NOSTATE || nState == DISABLED) return FALSE;
		
	// Determine the new check state

	if(nState == UNCHECKED)  return TRUE;
	
	switch( nState ) 
	{
		case CHILD_FILE:	
		case CHILD_FILE_FOLDER:
		case CHILD_FILE_ALLFOLDERS: 
			newState = nState - 2; 
			break;

		case CHILD_ALLFILES:
		case CHILD_ALLFILES_FOLDER:
		case CHILD_ALL: 
			newState = nState - 3; 
			break;

		case CHILD_NOFILE_FOLDER:
		case CHILD_NOFILE_ALLFOLDERS: 
		case FILE:
			newState = UNCHECKED;
			break;
	}

	SetItemState( hItem, newState, ALL); 
	return TRUE;
}

BOOL CFileTreeCtrl::ToggleForward (HTREEITEM hItem)
{
	if( hItem == NULL ) return FALSE;

	UINT newState;
	LPTVITEMDATA lptvid;

	UINT nState = GetItemState( hItem, &lptvid );
	if (nState == NOSTATE || nState == DISABLED) return FALSE;

	if (nState == FILE) return UncheckItem(hItem);
	
	switch (nState) 
	{
		case UNCHECKED: if (lptvid->IsFolder) newState = CHILD_ALLFILES; 
			else return CheckItem(hItem); break;
		case CHILD_ALLFILES: if (lptvid->nNumDirs) newState = CHILD_ALL;
			else newState = UNCHECKED; break;
		case CHILD_ALL:	newState = CHILD_NOFILE_ALLFOLDERS; break;
		case CHILD_NOFILE_ALLFOLDERS: newState = UNCHECKED; break;
		default: return CheckItem(hItem);
	}

	SetItemState( hItem, newState, ALL); 
	return TRUE;
}
	

BOOL CFileTreeCtrl::ToggleBackward (HTREEITEM hItem)
{
	if( hItem == NULL ) return FALSE;

	UINT newState;
	LPTVITEMDATA lptvid;
	
	UINT nState = GetItemState( hItem, &lptvid);
	if (nState == NOSTATE || nState == DISABLED) return FALSE;

	if (nState == UNCHECKED && !lptvid->IsFolder) return CheckItem(hItem);
	
	switch (nState) 
	{
		case UNCHECKED: if (lptvid->nNumDirs) newState = CHILD_NOFILE_ALLFOLDERS; 
			else newState = CHILD_ALL; break;
		case CHILD_NOFILE_ALLFOLDERS: newState = CHILD_ALL; break;
		case CHILD_ALL:	newState = CHILD_ALLFILES; break;
		case CHILD_ALLFILES: newState = UNCHECKED; break;
		default: return UncheckItem(hItem);
	}
	
	SetItemState( hItem, newState, ALL); 
	return TRUE;
}

BOOL CFileTreeCtrl::SelectedCompletely(int nState, LPTVITEMDATA lptvid)
{
	if ((nState == CHILD_ALL) || (nState == CHILD_ALLFILES && !lptvid->nNumDirs)) return TRUE;
	return FALSE;
}


BOOL CFileTreeCtrl::RefreshItem (HTREEITEM hItem)
{
	return SetItemState(hItem, REFRESH, ALL);
}

BEGIN_MESSAGE_MAP(CFileTreeCtrl, CTreeCtrl)
//{{AFX_MSG_MAP(CFileTreeCtrl)
ON_WM_DESTROY()
ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteitem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileTreeCtrl message handlers

void CFileTreeCtrl::OnDestroy() 
{
    FreeHidFoldersList();
	if (m_lpsfDesktop) m_lpsfDesktop->Release();

	CTreeCtrl::OnDestroy();
}

void CFileTreeCtrl::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	// TODO: Add your control notification handler code here
	
	LPTVITEMDATA   lptvid;  //Long pointer to TreeView item data
	HRESULT        hr;
	LPSHELLFOLDER  lpsf2=NULL;

	*pResult = 0;

	// Prevent root item (Desktop) from collapsing
	if (pNMTreeView->action == TVE_COLLAPSE)
	{
		if (pNMTreeView->itemNew.hItem == m_hDesktop) 
		{
			//FillTreeView(m_lpsfDesktop, NULL, m_hDesktop);
			*pResult = 1;
		}
		return;
	}

	if (!(pNMTreeView->itemNew.state & TVIS_EXPANDEDONCE))
	{
		// Otherwise fill the contents of the expanding folder
		lptvid=(LPTVITEMDATA)pNMTreeView->itemNew.lParam;
		if (lptvid)
		{
			hr=lptvid->lpsfParent->BindToObject(lptvid->lpi,
				0, IID_IShellFolder,(LPVOID *)&lpsf2);
			
			if (SUCCEEDED(hr))
			{
				if (!FillTreeView(lpsf2, lptvid->lpifq,	pNMTreeView->itemNew.hItem)) 
				{
					pNMTreeView->itemNew.cChildren=0;
					SetItem(&(pNMTreeView->itemNew));
				}
				else SelFolders.RemoveKey(pNMTreeView->itemNew.hItem); 
			}
			
		}
	}
	
}

int CFileTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	DisplayTree();

	return 0;
}

void CFileTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UINT uFlags=0;
	HTREEITEM hItem = HitTest(point,&uFlags);
	
	if( uFlags & TVHT_ONITEMSTATEICON )
	{
		if (nFlags & MK_SHIFT) ToggleBackward(hItem);
		else ToggleForward(hItem);

		NMTREEVIEW* pnmh = new NMTREEVIEW;
		pnmh->hdr.hwndFrom = m_hWnd;
		pnmh->hdr.idFrom = GetDlgCtrlID();
		pnmh->hdr.code = TVN_STATECHANGED;
		GetParent()->SendMessage(WM_NOTIFY, (WPARAM) GetDlgCtrlID(), (LPARAM)pnmh);
		delete(pnmh);

		return;
	}
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CFileTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	if( nChar == VK_SPACE )
	{
		HTREEITEM hItem = GetSelectedItem();
		ToggleForward(hItem);
		return;
	}
	
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CFileTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
/*	UINT uFlags=0;
	HTREEITEM hItem = HitTest(point,&uFlags);
	
	if( uFlags & TVHT_ONITEMSTATEICON )
	{
		LPTVITEMDATA fInfo;
		int State = GetItemState(hItem, &fInfo);
		int newState;
		do
			newState = 15.0*rand()/RAND_MAX;
		while (newState == NOSTATE || !SetItemState(hItem, newState, NONE));
		return;
	}
	
	CTreeCtrl::OnLButtonDown(nFlags, point);*/
}

void CFileTreeCtrl::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HRESULT        hr;
	LPMALLOC       lpMalloc;
	LPTVITEMDATA   lptvid;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;
	
	//Let's free the memory for the TreeView item data...
	hr=SHGetMalloc(&lpMalloc);
	if (FAILED(hr)) return;
	
	lptvid=(LPTVITEMDATA)pNMTreeView->itemOld.lParam;
	if (lptvid) 
	{
		if (lptvid->FullName) delete lptvid->FullName;
		if (lptvid->sPath) delete lptvid->sPath;

		lptvid->lpsfParent->Release();
		lpMalloc->Free(lptvid->lpi);  
		lpMalloc->Free(lptvid->lpifq);  
		lpMalloc->Free(lptvid); 
	}
	lpMalloc->Release();
}

CString CFileTreeCtrl::GetInfo(HTREEITEM hItem)
{
	CString res = TEXT("No info");
	if (hItem)
	{
		LPTVITEMDATA lptvid;
		lptvid = (LPTVITEMDATA) GetItemData(hItem);
		if (lptvid) res.Format(TEXT("Files: %d;   Dirs: %d;   SelFiles: %d;   SelDirs: %d;   PartDirs: %d"),
			lptvid->nNumFiles, lptvid->nNumDirs,
			lptvid->nSelFiles, lptvid->nSelDirs, lptvid->nPartSelDirs);
	}
	return res;
}

CString CFileTreeCtrl::GetSelectedFile()
{
	HTREEITEM hItem=GetSelectedItem();
	LPTVITEMDATA lptvid;
	lptvid = (LPTVITEMDATA) GetItemData(hItem);
	if (lptvid && !lptvid->IsFolder) return *(lptvid->FullName);
	else return TEXT("");
}

int compare( void* arg1, void* arg2 )
{
   CString *s1 = (CString*) arg1;
   CString *s2 = (CString*) arg2;
   return s1->CollateNoCase(*s2);
}

int compare1( void* arg1, void* arg2 )
{
   CString *s1 = (CString*) arg1;
   CString *s2 = (CString*) arg2;
   return s1->CollateNoCase(*s2);
}

int CFileTreeCtrl::GetSelectedFiles(CString* &Files)
{
	int N=SelFiles.GetCount();

	if (Files) delete[] Files;
	
	if (N) 
	{
		Files = new CString[N];
		
		int i=0;
		POSITION pos = SelFiles.GetStartPosition();
		HTREEITEM Key;
		while (pos != NULL)
		{
			SelFiles.GetNextAssoc( pos, Key, Files[i] );
			i++;
		}
	}

	qsort( &Files[0], (size_t)N, sizeof(CString), (int(*)(const void*, const void*))compare);

	return N;
}


int CFileTreeCtrl::GetSelectedFolders(CString* &Files)
{
	int N=SelFolders.GetCount();

	if (Files) delete[] Files;
	
	if (N) 
	{
		Files = new CString[N];
		
		int i=0;
		POSITION pos = SelFolders.GetStartPosition();
		HTREEITEM Key;
		while (pos != NULL)
		{
			SelFolders.GetNextAssoc( pos, Key, Files[i] );
			i++;
		}
	}

	qsort( &Files[0], (size_t)N, sizeof(CString), (int(*)(const void*, const void*))compare1);

	return N;
}
