/********************************************************************
 Created: 2006/11/15 
 File name: l2adec_guid.h
 Purpose: GUIDs for MPEG audio decoder DS Filter

 Copyright (c) 2005-2008 MainConcept AG. All rights reserved.

 This software is the confidential and proprietary information of
 MainConcept AG and may be used only in accordance with the terms of
 your license from MainConcept AG.

*********************************************************************/

#ifndef __MPEG2AUDIO_GUID_H__
#define __MPEG2AUDIO_GUID_H__

////////////////////////////////////////////////////////////

#undef  L2ADCOMPANY_HIDEICON
#define L2ADCOMPANY_DS_MERIT   MERIT_NORMAL

////////////////////////////////////////////////////////////
#ifndef RC_INVOKED // don't let resource compiler see this part

// {2F75E451-A88C-4939-BFE5-D92D48C102F2}
DEFINE_GUID(CLSID_ELMPGLayer2AudioDecoder,
0x2f75e451, 0xa88c, 0x4939, 0xbf, 0xe5, 0xd9, 0x2d, 0x48, 0xc1, 0x2, 0xf2);

// {A1BDD89E-DC7C-41fb-AD2D-A7D6C3B531F6}
DEFINE_GUID(CLSID_ELMPGLayer2AudioDecoder_SettingsPropertyPage,
0xa1bdd89e, 0xdc7c, 0x41fb, 0xad, 0x2d, 0xa7, 0xd6, 0xc3, 0xb5, 0x31, 0xf6);

// {8566B372-D0F6-4136-8C5E-7E368EBC85B4}
DEFINE_GUID(CLSID_ELMPGLayer2AudioDecoder_AboutPropertyPage,
0x8566b372, 0xd0f6, 0x4136, 0x8c, 0x5e, 0x7e, 0x36, 0x8e, 0xbc, 0x85, 0xb4);

#endif // RC_INVOKED
#endif //__MPEG2AUDIO_GUID_H__
