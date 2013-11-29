/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:09
	filename: 	TxAutoComPtr.h
	author:		tianyx
	
	purpose:	auto ptr for ITxUnknow
*********************************************************************/
#pragma  once

template <class T>
class CTxAutoComPtr
{
	T* p;
public:
	CTxAutoComPtr() throw()
	{
		p = NULL;
	}
	CTxAutoComPtr(_In_opt_ T* lp) throw()
	{
		p = lp;
		if (p != NULL)
			p->AddRef();
	}
public:
	typedef T _PtrClass;
	~CTxAutoComPtr() throw()
	{
		if (p)
			p->Release();
	}
	operator T*() const throw()
	{
		return p;
	}
	T& operator*() const
	{
		ATLENSURE(p!=NULL);
		return *p;
	}

	T* operator->() const
	{
		ATLENSURE(p!=NULL);
		return p;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&() throw()
	{
		return &p;
	}
	
	bool operator!() const throw()
	{
		return (p == NULL);
	}
	
	bool operator!=(_In_opt_ T* pT) const
	{
		return !operator==(pT);
	}
	bool operator==(_In_opt_ T* pT) const throw()
	{
		return p == pT;
	}

	// Release the interface and set to NULL
	void Release() throw()
	{
		T* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}
	
	// Attach to an existing interface (does not AddRef)
	void Attach(_In_opt_ T* p2) throw()
	{
		if (p)
			p->Release();
		p = p2;
	}
	// Detach the interface (does not Release)
	T* Detach() throw()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}
		
};