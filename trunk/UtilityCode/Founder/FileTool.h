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
	//				����lpszRoot��ָ���ĸ�Ŀ¼���õ����·�� ,���� "example\\test"
	//				���lpszRootΪNULL����Ӧ�ó������ڵ�Ŀ¼����Ϊ�Ǹ�Ŀ¼ 
	CString			GetRelativePath(LPCTSTR lpszRoot = NULL);
	
	//				�õ�����·���������ļ��� ,���� "c:\\windows\\win.ico"
	CString			GetFullPathName();
	
	//				�õ��ļ���������չ��, ���� "win.ico"
	CString			GetFileName();
	
	//				�õ��ļ�����������չ��, ���� "win"
	CString			GetFileNameWithoutExt();
	
	//				�õ�����·�����������ļ���, ���� "c:\\windows"
	CString			GetFilePath();
	
	//				�õ��ļ���չ�������� "ico"
	CString			GetFileExt();
	
public:	
	
	//				�������·����������ԭȫ·��Ϊ c:/windows/test.exe
	//				SetRelativePathName(_T("/anotherfolder/change.exe"))); �������ȫ·����
	//				Ϊ c:/windows/anotherfolder/change.exe
	BOOL			SetRelativePathName( LPCTSTR lpszRelativePathName);
	
	//				��������ȫ·���� SetFullPathName( _T("c:/windows\\test.exe") );
	BOOL			SetFullPathName( LPCTSTR lpszFullPathName);
	
	//				�ж�ȫ·������ָ����ļ���Ŀ¼�Ƿ����
	BOOL			IsExist();	
	
	//				�жϾ���·������ָ����ļ���Ŀ¼�Ƿ����windows�ı�׼
	BOOL			IsPathNameValid();

	//				strFileFullPath : ����·��
	static BOOL		IsPathNameValid( const CString& strFileFullPath );

	//				�ж� strPath ��ָ���·���Ƿ�Ϊ����·��
	static BOOL		IsAbsolutePath(const CString& strPath);
	
public:
	// ���캯����ϵ������
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

	//				�õ�ϵͳ����������"c:\\"; "d:\\"
	HRESULT			GetDiskPartition(CStringArray& strPartitionArray, DWORD dwFlag = 0);


	//				�õ�ָ���������ļ������ strWildcardû��ָ��·��������������ϵͳ
	//				���� "*.dll" ��ʾ��������ϵͳ�����е� dll �ļ��� "c:\\*.dll" ��ʾ����C���µ�dll�ļ���
	HRESULT			GetFiles( const CString& strWildcard, CStringArray& strArrayFiles, DWORD dwFlag = SEARCH_SUB_FOLDER );


	//				����ָ��Ŀ¼�µ�������Ŀ¼���ڷ��ص�Ŀ¼�����У�����������"\\/"
	//				��� strRoot Ϊ��:
	//						dwFlag = DONOT_SEARCH_SUB_FOLDER, �򷵻�ϵͳ��������"c:\\";"d:\\";"e:\\"�ȵ�
	//						dwFlag = SEARCH_SUB_FOLDER, ϵͳ����Ŀ¼
	HRESULT			GetFolders(const CString& strRoot, CStringArray& strArrayFolders, DWORD dwFlag = SEARCH_SUB_FOLDER );

	//				��ʾһ��Ŀ¼�Ի��򣬹��û�ѡ��Ŀ¼
	HRESULT			SelectFolder(CString& strFolder);
};

#endif // !defined(AFX_FILETOOL_H__9FF23256_9EA2_480A_A17B_437ED64A083F__INCLUDED_)
