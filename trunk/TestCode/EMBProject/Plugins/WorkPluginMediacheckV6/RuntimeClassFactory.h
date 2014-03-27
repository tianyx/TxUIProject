// RuntimeClassFactory.h: interface for the CRuntimeClassFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNTIMECLASSFACTORY_H__20AEABAA_3956_45E0_B63D_0EEEA217878F__INCLUDED_)
#define AFX_RUNTIMECLASSFACTORY_H__20AEABAA_3956_45E0_B63D_0EEEA217878F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>

template<class T>
struct ClassObject 
{
	ClassObject(CString cClassName)
	{
		GetRunTimeClassFactory()->Register_Class(cClassName,&ClassObject<T>::CreateObject);
	}
	static void* CreateObject()
	{
		void* p = new T;
		return p;
	}
};


#define REGISTER_CLASS(X) ClassObject<X> __class_help_##X(_T(#X));
#define REGISTER_CLASS_BY_NAME(X,CLASSNAME) ClassObject<X> __class_Help_##X(_T(CLASSNAME));

class CRuntimeClassFactory  
{
private:
	typedef void* (*RUNTIME_CREATE_REF)();
protected:
	struct tagRunTimeClassInfo 
	{
		CString strClassName;
		CString strType;
		CString strDevID;
		RUNTIME_CREATE_REF pClass;
	};
	CArray<tagRunTimeClassInfo,tagRunTimeClassInfo> m_ClassArr;
public:
	  void Register_Class(CString cClassName,void* p);
	  void* CreateByName(CString cClassName);
	  void DieOut();
public:
	CRuntimeClassFactory();
	virtual ~CRuntimeClassFactory();

};

CRuntimeClassFactory* GetRunTimeClassFactory();

#endif // !defined(AFX_RUNTIMECLASSFACTORY_H__20AEABAA_3956_45E0_B63D_0EEEA217878F__INCLUDED_)
