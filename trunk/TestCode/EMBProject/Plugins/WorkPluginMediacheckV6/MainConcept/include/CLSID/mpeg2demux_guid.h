/********************************************************************
 Created: 2006/11/15 
 File name: mpeg2demux_guid.h
 Purpose: GUIDs for MPEG-2 Demultiplexer DS Filter

 Copyright (c) 2005-2008 MainConcept AG. All rights reserved.

 This software is the confidential and proprietary information of
 MainConcept AG and may be used only in accordance with the terms of
 your license from MainConcept AG.

*********************************************************************/

#ifndef __MPGDMX_GUID_H__
#define __MPGDMX_GUID_H__

////////////////////////////////////////////////////////////


#undef  COMPANY_HIDEICON

#define COMPANY_DS_MERIT_DEMULTIPLEXER (MERIT_PREFERRED+0x100)
#define COMPANY_DS_MERIT_PARSER        (MERIT_UNLIKELY)


////////////////////////////////////////////////////////////


// {136DCBF5-3874-4b70-AE3E-15997D6334F7} 
static const GUID CLSID_MPGDMX = 
{ 0x136dcbf5, 0x3874, 0x4b70, { 0xae, 0x3e, 0x15, 0x99, 0x7d, 0x63, 0x34, 0xf7 } };

// {4578D46F-1AAF-4fa6-AD9C-401A97CFA291} 
static const GUID CLSID_MPGDMXPropMain = 
{ 0x4578d46f, 0x1aaf, 0x4fa6, { 0xad, 0x9c, 0x40, 0x1a, 0x97, 0xcf, 0xa2, 0x91 } };

// {A8786FBA-B1E0-41d8-9A3E-F56D4E226A51} 
static const GUID CLSID_MPGDMXPropAbout = 
{ 0xa8786fba, 0xb1e0, 0x41d8, { 0x9a, 0x3e, 0xf5, 0x6d, 0x4e, 0x22, 0x6a, 0x51 } };

// {DEE56715-7081-4d57-91A7-984AE2712268}
static const GUID CLSID_MPGDMX_PARSER = 
{ 0xdee56715, 0x7081, 0x4d57, { 0x91, 0xa7, 0x98, 0x4a, 0xe2, 0x71, 0x22, 0x68 } };

// {F7EF9722-1DEA-4430-B830-C54B382FC90C} 
static const GUID CLSID_MPGDMXPropAboutParser = 
{ 0xf7ef9722, 0x1dea, 0x4430, { 0xb8, 0x30, 0xc5, 0x4b, 0x38, 0x2f, 0xc9, 0xc } };


////////////////////////////////////////////////////////////

#endif //__MPGDMX_GUID_H__

