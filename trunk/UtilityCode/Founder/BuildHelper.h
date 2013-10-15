// BuildHelper.h: interface for the CBuildVersionHelper class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_BUILDHELPER_H__11BB42BD_0B61_4E14_9BB4_9EA42E969766__INCLUDED_)
#define AFX_BUILDHELPER_H__11BB42BD_0B61_4E14_9BB4_9EA42E969766__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBuildVersionHelper  
{
public:
	CBuildVersionHelper();
	virtual ~CBuildVersionHelper();

public:
	operator CTime()
	{
		
		CTime tBuildTime( _ttoi(m_strBuildTime_Year), _ttoi(m_strBuildTime_Month), _ttoi(m_strBuildTime_Day), 
			_ttoi(m_strBuildTime_Hour), _ttoi(m_strBuildTime_Minute), _ttoi(m_strBuildTime_Second));
		return tBuildTime;
	}

	//		%y, %m, %d,  %H, %M, %S   ( year, month, day, hour, minute, second)
	CString Format(const CString& strFormat);
		
	CString m_strBuildTime_Hour;
	CString m_strBuildTime_Minute;
	CString m_strBuildTime_Second;

	CString m_strBuildTime_Year;
	CString m_strBuildTime_Month;
	CString m_strBuildTime_Day;

	// �汾��Ϣ
	CString m_strFileDescription;				// �ļ�����
	CString m_strInternalName;					// �ڲ�����
	CString m_strLegalTradeMarks;				// �̱�
	CString m_strOriginalFileName;				// ԭʼ�ļ���
	CString m_strProductName;					// ��Ʒ����
	CString m_strProductVersion;				// ��Ʒ�汾
	CString m_strCompanyName;					// ��˾����
	CString m_strLegalCopyright;				// ��Ȩ����
	CString m_strPrivateBuild;
	CString m_strSpecialBuild;
	CString m_strFileVersion;
	
	CString m_strComments;						//						

};

#endif // !defined(AFX_BUILDHELPER_H__11BB42BD_0B61_4E14_9BB4_9EA42E969766__INCLUDED_)
