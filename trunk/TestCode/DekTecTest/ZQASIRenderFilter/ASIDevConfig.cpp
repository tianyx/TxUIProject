#include "StdAfx.h"
#include "ASIDevConfig.h"

CASIDevConfig::CASIDevConfig(void)
{
}

CASIDevConfig::~CASIDevConfig(void)
{
}

STDMETHODIMP CASIDevConfig::set_param( devparam *paramIn /* */ )
{
	if (!paramIn)
	{
		return E_FAIL;
	}
	m_DevParam = *paramIn;
	return S_OK;
}

STDMETHODIMP CASIDevConfig::get_param( devparam *paramOut /* */ )
{
	if (!paramIn)
	{
		return E_FAIL;
	}
	*paramOut = m_DevParam;
	return S_OK;
}
