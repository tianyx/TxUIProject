// RuntimeClassFactory.cpp: implementation of the CRuntimeClassFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RuntimeClassFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRuntimeClassFactory* m_pRuntimeClassFactory = NULL;

CRuntimeClassFactory* GetRunTimeClassFactory()
{
	if (m_pRuntimeClassFactory == NULL)
	{
		m_pRuntimeClassFactory = new CRuntimeClassFactory();
	}
	return m_pRuntimeClassFactory;
}

CRuntimeClassFactory::CRuntimeClassFactory()
{
	
}

CRuntimeClassFactory::~CRuntimeClassFactory()
{

}

void CRuntimeClassFactory::Register_Class(CString cClassName,void* p)
{
	cClassName.TrimLeft();
	cClassName.TrimRight();
	cClassName.MakeUpper();
	tagRunTimeClassInfo ClassInfo;
	ClassInfo.strClassName = cClassName;
	ClassInfo.pClass = (RUNTIME_CREATE_REF)p;
	m_ClassArr.Add(ClassInfo);
}

void* CRuntimeClassFactory::CreateByName(CString cClassName)
{
	cClassName.TrimRight();
	cClassName.TrimLeft();
	cClassName.MakeUpper();
	for (int i = 0 ; i < m_ClassArr.GetSize() ; i ++)
	{
		tagRunTimeClassInfo ClassInfo;
		ClassInfo = m_ClassArr.GetAt(i);
		if (ClassInfo.strClassName == cClassName)
		{
			return ClassInfo.pClass();
		}
	}
	return NULL;
}

void CRuntimeClassFactory::DieOut()
{
	delete[] m_pRuntimeClassFactory;
}
