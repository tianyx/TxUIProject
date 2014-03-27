/********************************************************************
 Created: 2006/11/15 
 File name: mpeg2mux_guid.h
 Purpose: GUIDs for MPEG-2 Multiplexer DS Filter

 Copyright (c) 2005-2008 MainConcept AG. All rights reserved.

 This software is the confidential and proprietary information of
 MainConcept AG and may be used only in accordance with the terms of
 your license from MainConcept AG.

*********************************************************************/

#ifndef __MPG2MUX_GUID_H__
#define __MPG2MUX_GUID_H__

////////////////////////////////////////////////////////////

#undef COMPANY_HIDEICON
#undef STD_EDITION

/*****************************************
	Full-edition Filter GUIDs
*****************************************/
// {cfd28198-115c-4606-bd64-27a7e0x51d2a}
static const GUID CLSID_EM2Mux = 
{0xcfd28198, 0x115c, 0x4606, {0xbd, 0x64, 0x27, 0xa7, 0xe0, 0x5, 0x1d, 0x2a}};

//Property pages
static const GUID CLSID_EM2Mux_AboutPropertyPage =  
{0xd99b76ed, 0x726e, 0x45e9, {0xab, 0x37, 0xb9, 0xd6, 0x63, 0x6c, 0x83, 0x7a}};

static const GUID CLSID_EM2Mux_MainPropertyPage = 
{0x6724cb1e, 0x9eda, 0x4a81, {0xb5, 0x6f, 0x31, 0xe3, 0x5e, 0x44, 0xe9, 0x2b}};


#endif // __MPG2MUX_GUID_H__

