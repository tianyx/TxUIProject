/********************************************************************
 Created: 2006/11/15 
 File name: mpeg2enc_guid.h
 Purpose: GUIDs for MPEG-2 Video Encoder DS Filter

 Copyright (c) 2005-2008 MainConcept AG. All rights reserved.

 This software is the confidential and proprietary information of
 MainConcept AG and may be used only in accordance with the terms of
 your license from MainConcept AG.

*********************************************************************/

////////////////////////////////////////////////////////////

#undef  COMPANY_HIDEICON
#undef  STD_EDITION

#define M2VECOMPANY_DS_MERIT    MERIT_DO_NOT_USE

////////////////////////////////////////////////////////////
#ifndef RC_INVOKED // don't let resource compiler see this part

#ifndef __M2VE_FILTER_GUIDS_H__
#define __M2VE_FILTER_GUIDS_H__

/*****************************************
	Full-edition Filter GUIDs
*****************************************/

// {00098205-76CC-497E-98A1-6EF10D0BF26C}
static const GUID CLSID_EM2VE = 
	{ 0x00098205, 0x76cc, 0x497e, { 0x98, 0xa1, 0x6e, 0xf1, 0x0d, 0x0b, 0xf2, 0x6c } };

// {17E6F4EC-48B7-4094-AFA9-546A1DCF698E}
static const GUID CLSID_EM2VE_AboutPropertyPage = 
	{ 0x17e6f4ec, 0x48b7, 0x4094, { 0xaf, 0xa9, 0x54, 0x6a, 0x1d, 0xcf, 0x69, 0x8e } };

// {D0088987-2FFF-4e1e-8AE2-A1607BD20240}
static const GUID CLSID_EM2VE_MainPropertyPage = 
	{ 0xd0088987, 0x2fff, 0x4e1e, { 0x8a, 0xe2, 0xa1, 0x60, 0x7b, 0xd2, 0x2, 0x40 } };

// {1CDD1AD7-A1CC-4cb8-B81E-8AAFE09AC292}
static const GUID CLSID_EM2VE_AdvancedPropertyPage = 
	{ 0x1cdd1ad7, 0xa1cc, 0x4cb8, { 0xb8, 0x1e, 0x8a, 0xaf, 0xe0, 0x9a, 0xc2, 0x92 } };

#endif __M2VE_FILTER_GUIDS_H__

#endif // RC_INVOKED

