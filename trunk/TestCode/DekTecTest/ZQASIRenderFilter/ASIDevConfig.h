#pragma once
#include "IASIDevPreConfig.h"
class CASIDevConfig :
	public IASIDevPreConfig
{
public:
	CASIDevConfig(void);
	~CASIDevConfig(void);

	DECLARE_IUNKNOWN

	STDMETHODIMP set_param (
		devparam *paramIn      //
		) PURE;

	STDMETHODIMP get_param (
		devparam *paramOut      // 
		) PURE;

	devparam m_DevParam;
};
