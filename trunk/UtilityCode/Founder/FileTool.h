// FileTool.h: interface for the CFileTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILETOOL_H__9FF23256_9EA2_480A_A17B_437ED64A083F__INCLUDED_)
#define AFX_FILETOOL_H__9FF23256_9EA2_480A_A17B_437ED64A083F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFilePath  
{
public:
	enum { CURRENT_MODULE, EXE_MODULE };
	// c:\\windows\\..\\..\\test.h = c:\\test.h
public:
	//				根据lpszRoot所指定的根目录来得到相对路径 ,例如 "example\\test"
	//				如果lpszRoot为NULL，则应用程序所在的目录被认为是根目录 
	CString			GetRelativePath(LPCTSTR lpszRoot = NULL);
	
	//				得到绝对路径，包括文件名 ,例如 "c:\\windows\\win.ico"
	CString			GetFullPathName();
	
	//				得到文件名，带扩展名, 例如 "win.ico"
	CString			GetFileName();
	
	//				得到文件名，不带扩展名, 例如 "win"
	CString			GetFileNameWithoutExt();
	
	//				得到绝对路径，不包括文件名, 例如 "c:\\windows"
	CString			GetFilePath();
	
	//				得到文件扩展名，例如 "ico"
	CString			GetFileExt();
	
public:	
	
	//				设置相对路径名，例如原全路径为 c:/windows/test.exe
	//				SetRelativePathName(_T("/anotherfolder/change.exe"))); 则变更后的全路径名
	//				为 c:/windows/anotherfolder/change.exe
	BOOL			SetRelativePathName( LPCTSTR lpszRelativePathName);
	
	//				重新设置全路径名 SetFullPathName( _T("c:/windows\\test.exe") );
	BOOL			SetFullPathName( LPCTSTR lpszFullPathName);
	
	//				判断全路径名所指向的文件或目录是否存在
	BOOL			IsExist();	
	
	//				判断绝对路径名所指向的文件或目录是否符合windows的标准
	BOOL			IsPathNameValid();

	//				strFileFullPath : 绝对路径
	static BOOL		IsPathNameValid( const CString& strFileFullPath );

	//				判断 strPath 所指向的路径是否为绝对路径
	static BOOL		IsAbsolutePath(const CString& strPath);
	
public:
	// 构造函数与系构函数
	CFilePath( DWORD nModuleFlag = CURRENT_MODULE );
	CFilePath( LPCTSTR lpszPathName);
	virtual ~CFilePath();
	
protected:
	CString			m_strFullPathName;
	CString			m_strFilePath;
	CString			m_strFileName;
};

class CFileSystemTool  
{
public:	
	
	enum			{ DONOT_SEARCH_SUB_FOLDER = 0, SEARCH_SUB_FOLDER = 1};

	//				得到系统分区。例如"c:\\"; "d:\\"
	HRESULT			GetDiskPartition(CStringArray& strPartitionArray, DWORD dwFlag = 0);


	//				得到指定条件的文件。如果 strWildcard没有指明路径，则搜索整个系统
	//				例如 "*.dll" 表示搜索整个系统中所有的 dll 文件。 "c:\\*.dll" 表示搜索C盘下的dll文件。
	HRESULT			GetFiles( const CString& strWildcard, CStringArray& strArrayFiles, DWORD dwFlag = SEARCH_SUB_FOLDER );


	//				返回指定目录下的所有子目录。在返回的目录名称中，不包括最后的"\\/"
	//				如果 strRoot 为空:
	//						dwFlag = DONOT_SEARCH_SUB_FOLDER, 则返回系统分区。如"c:\\";"d:\\";"e:\\"等等
	//						dwFlag = SEARCH_SUB_FOLDER, 系统所有目录
	HRESULT			GetFolders(const CString& strRoot, CStringArray& strArrayFolders, DWORD dwFlag = SEARCH_SUB_FOLDER );

	//				显示一个目录对话框，供用户选择目录
	HRESULT			SelectFolder(CString& strFolder);
};

#endif // !defined(AFX_FILETOOL_H__9FF23256_9EA2_480A_A17B_437ED64A083F__INCLUDED_)
