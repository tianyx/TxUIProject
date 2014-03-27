/********************************************************************
 Created: 2006/11/15 
 File name: sf_guid.h
 Purpose: GUIDs for DS Sink Filter

 Copyright (c) 2005-2008 MainConcept AG. All rights reserved.

 This software is the confidential and proprietary information of
 MainConcept AG and may be used only in accordance with the terms of
 your license from MainConcept AG.

*********************************************************************/

#ifndef __SINKFILTER_GUID_H__
#define __SINKFILTER_GUID_H__

////////////////////////////////////////////////////////////

#define SFCOMPANY_DS_MERIT   MERIT_DO_NOT_USE
#undef	SFCOMPANY_HIDEICON

////////////////////////////////////////////////////////////

// {CF2521A7-4029-4cc1-8C6E-F82BD82BB343}
DEFINE_GUID(CLSID_ESF,
0xcf2521a7, 0x4029, 0x4cc1, 0x8c, 0x6e, 0xf8, 0x2b, 0xd8, 0x2b, 0xb3, 0x43);

// {A539FCC4-AB2E-4307-BFBD-634DE69ACD78}
DEFINE_GUID(CLSID_ESF_AboutPage,
0xa539fcc4, 0xab2e, 0x4307, 0xbf, 0xbd, 0x63, 0x4d, 0xe6, 0x9a, 0xcd, 0x78);

// {A539FCC5-AB2E-4307-BFBD-634DE69ACD78}
DEFINE_GUID(CLSID_ESF_PropertyPage,
0xa539fcc5, 0xab2e, 0x4307, 0xbf, 0xbd, 0x63, 0x4d, 0xe6, 0x9a, 0xcd, 0x78);

////////////////////////////////////////////////////////////

#endif //__SINKFILTER_GUID_H__
