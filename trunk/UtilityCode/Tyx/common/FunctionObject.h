// FunctionObject.h: interface for the CFunctionObject class.
// 
// USAGE:
// The user of this class is a C++ class (CYourClass) that must:
//		
// 1. Implement a member function on CYourClass of the form:
//				
// void MyEventCallback(DISPID id, VARIANT* pVarResult);
// 
// This function will be called with the dispid set in
// CreateEventFunctionObject. One event handler function can service
// multiple event function objects.
// 
// 2. When setting function object event handlers on MSHTML, use code of
// this form:
// 
// _variant_t varFO =
//    CFunctionObject<CYourClass>::CreateEventFunctionObject(this,
//       MyEventCallback, DISPID_MOUSEDOWN);
// [HTML object interface]->put_onmousedown(varFO);
// 
// This syntax requires VC++ COM compiler support for the _variant_t class. 
// Otherwise, pass into the put_mousedown (or other MSHTML event prop)
// a VARIANT containing the LPDISPATCH returned from
// CreateEventFunctionObject.
// 
// 3. Set all MSHTML function object event handlers to NULL before the
// MSHTML document is destroyed:
// 
//   VARIANT varNull; varNull.vt = VT_NULL;
//   [HTML object interface]->put_onmousedown(varNull);
// 
// At minimum, do this in an event handler for the HTML window object's 
// "onbeforeunload" event or WebBrowser control BeforeNavigate2. The 
// function objects will be stranded in memory if this isn't done before
// the HTML document is unloaded.If you set the handlers to NULL in 
// BeforeNavigate2 then you should also account for the last case when 
// the application is being shutdown since BeforeNavigate2 will not be
// called at that time. 


////////////////////////////////////////////////////////////////////// 

template <class T> class CFunctionObject : public IDispatch  
{
	typedef void (T::*EVENTFUNCTIONCALLBACK)(DISPID id, VARIANT* pVarResult);
	
public:
	CFunctionObject() { m_cRef = 0; }
	
	~CFunctionObject() {}
	
	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject)
	{
		*ppvObject = NULL;
		
		if (IsEqualGUID(riid, IID_IUnknown))
			*ppvObject = reinterpret_cast<void **> (this);
		
		if (IsEqualGUID(riid, IID_IDispatch))
			*ppvObject = reinterpret_cast<void **> (this);
		
		if (*ppvObject)
		{
			((IUnknown*)*ppvObject)->AddRef();
			return S_OK;
		}
		else return E_NOINTERFACE;
	}
	
	DWORD __stdcall AddRef()
	{
		return InterlockedIncrement(&m_cRef);
	}
	
	DWORD __stdcall Release()
	{
		if (InterlockedDecrement(&m_cRef) == 0)
		{
			delete this;
			return 0;
		}
		return m_cRef;
	}
	
	STDMETHOD(GetTypeInfoCount)(unsigned int FAR* pctinfo)
	{ return E_NOTIMPL; }
	STDMETHOD(GetTypeInfo)(unsigned int iTInfo, LCID  lcid,
		ITypeInfo FAR* FAR*  ppTInfo)
	{ return E_NOTIMPL; }
	STDMETHOD(GetIDsOfNames)(REFIID riid, OLECHAR FAR* FAR* rgszNames,
		unsigned int cNames, LCID lcid, DISPID FAR* rgDispId)
	{ return S_OK; }
	STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid,
		WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult,
		EXCEPINFO * pExcepInfo, UINT * puArgErr)
	{
		if (DISPID_VALUE == dispIdMember && m_pFunc != NULL && m_pT != NULL)
			(m_pT->*m_pFunc)(m_id, pVarResult);
		else
			TRACE(_T("CFO Invoke dispid = %d\n"), dispIdMember);
		
		return S_OK;
	}
	
public:
	static LPDISPATCH CreateEventFunctionObject(T* pT,
		EVENTFUNCTIONCALLBACK pFunc, DISPID id)
	{
		CFunctionObject<T>* pFO = new CFunctionObject<T>;
		pFO->m_pT = pT;
		pFO->m_pFunc = pFunc;
		pFO->m_id = id;
		return reinterpret_cast<LPDISPATCH> (pFO);
	}
	
protected:
	T* m_pT;
	EVENTFUNCTIONCALLBACK m_pFunc;
	DISPID m_id;
	long m_cRef;
};