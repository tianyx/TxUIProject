/********************************************************************
 Created: 2006/11/15 
 File name: l2aenc_guid.h
 Purpose: GUIDs for MPEG audio encoder DS Filter

 Copyright (c) 2005-2008 MainConcept AG. All rights reserved.

 This software is the confidential and proprietary information of
 MainConcept AG and may be used only in accordance with the terms of
 your license from MainConcept AG.

*********************************************************************/

////////////////////////////////////////////////////////////

#undef  COMPANY_HIDEICON

#define L2AECOMPANY_DS_MERIT   MERIT_SW_COMPRESSOR

////////////////////////////////////////////////////////////
#ifndef RC_INVOKED // don't let resource compiler see this part

#if !defined(L2_ENCODER_GUID)
#define L2_ENCODER_GUID

// {15BEBB32-5BB5-42b6-B45A-BA49F78BA19F}
static const GUID CLSID_EL2AudioEncoder =
{ 0x15bebb32, 0x5bb5, 0x42b6, {0xb4, 0x5a, 0xba, 0x49, 0xf7, 0x8b, 0xa1, 0x9f}};

// {15BEBB32-A1C8-471c-99B4-8D9B13F9419F}
static const GUID CLSID_EL2AudioEncoder_About =
{ 0x15bebb32, 0xa1c8, 0x471c, {0x99, 0xb4, 0x8d, 0x9b, 0x13, 0xf9, 0x41, 0x9f}};

// {15BEBB32-6FF7-4098-992F-08356D7A0EC9}
static const GUID CLSID_EL2AudioEncoder_PropertyPageSettings =
{ 0x15bebb32, 0x6ff7, 0x4098, {0x99, 0x2f, 0x8, 0x35, 0x6d, 0x7a, 0xe, 0xc9}};

#endif // L2_ENCODER_GUID
#endif // RC_INVOKED
