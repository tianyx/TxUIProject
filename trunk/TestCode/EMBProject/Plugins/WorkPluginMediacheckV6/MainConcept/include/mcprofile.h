/* ----------------------------------------------------------------------------
 * File: mcprofile.h
 *
 * Desc: profile/preset defines for default codec settings
 *
 * Copyright (c) 2009, MainConcept GmbH. All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * MainConcept GmbH and may be used only in accordance with the terms of
 * your license from MainConcept GmbH.
 * ----------------------------------------------------------------------------
 */
 
#if !defined (__MC_PROFILE_INCLUDED__)
#define __MC_PROFILE_INCLUDED__  
 
/////////////////////////////////////////////////////////////////
//
// returned strings of the default setting functions should contain the profile name 
// and some codec spezific data terminated by '\0' if the profile is known by the codec
// otherwise return NULL
//
// samples:
// MCPROFILE_VCD:   "VCD, PAL, MPEG-2 Video\0"      for MPEG video encoder
// MCPROFILE_DVD:   "DVD, NTSC, MPEG-2 Video\0"     for MPEG video encoder
// 
// MCPROFILE_VCD:   "VCD, MPEG-1 Audio Layer 2\0"   for MPEG audio encoder
// MCPROFILE_DVD:   "DVD, MPEG-1 Audio Layer 2\0"   for MPEG audio encoder
// a.s.o
//
//
/////////////////////////////////////////////////////////////////


#define MCPROFILE_DEFAULT                   0x00000000       // default setting of codec


/////////////////////////////////////////////////////////////////
//
// reserved ranges
//
// 0x0001 - 0x0FFF   reserved for private codec profiles
//
/////////////////////////////////////////////////////////////////


#define MCPROFILE_VCD                       0x00001000       // MPEG-1 VideoCD
#define MCPROFILE_SVCD                      0x00001001       // MPEG-2 SuperVCD
#define MCPROFILE_DVD                       0x00001010       // MPEG-2 DVD-Video
#define MCPROFILE_DVD_MPEG1                 0x00001011       // MPEG-1 DVD-Video
#define MCPROFILE_DVD_DVR                   0x00001012       // MPEG-2 DVD-VR
#define MCPROFILE_DVD_DVR_MPEG1             0x00001013       // MPEG-1 DVD-VR
#define MCPROFILE_DVD_PVR                   0x00001014       // MPEG-2 DVD+VR, not yet implemented!
#define MCPROFILE_DVD_PVR_MPEG1             0x00001015       // MPEG-1 DVD+VR, not yet implemented!

#define MCPROFILE_DVB                       0x00001020       // DVB
#define MCPROFILE_MMV                       0x00001021       // MPEG-2 MICROMV
#define MCPROFILE_DVHS                      0x00001022       // DVHS
#define MCPROFILE_ATSC                      0x00001023       // MPEG-2 ATSC normal data rate
#define MCPROFILE_ATSCHI                    0x00001024       // MPEG-2 ATSC high data rate
#define MCPROFILE_CABLELABS                 0x00001025       // CableLabs MPEG-2 MP @ ML (3.18MBit) or MP @ HL (18.1MBit)

#define MCPROFILE_HDV_HD1                   0x00001030       // MPEG-2 HDV HD1
#define MCPROFILE_HDV_HD2                   0x00001031       // MPEG-2 HDV HD2


#define MCPROFILE_D10                       0x00001040       // D-10 MPEG-2 4:2:2P @ ML (25 MBit)
#define MCPROFILE_D10_25                    0x00001040       // D-10 MPEG-2 4:2:2P @ ML (25 MBit)
#define MCPROFILE_D10_30                    0x00001041       // D-10 MPEG-2 4:2:2P @ ML (30 MBit)
#define MCPROFILE_D10_40                    0x00001042       // D-10 MPEG-2 4:2:2P @ ML (40 MBit)
#define MCPROFILE_D10_50                    0x00001043       // D-10 MPEG-2 4:2:2P @ ML (50 MBit)


#define MCPROFILE_HD_DVD                    0x00002000       // HD DVD

#define MCPROFILE_AVCHD                     0x00002800       // AVCHD
#define MCPROFILE_AVC_INTRA                 0x00002801       // AVC Intra
#define MCPROFILE_AVC_INTRA_CLASS_50        0x00002802       // AVC Intra (Class 50)
#define MCPROFILE_AVC_INTRA_CLASS_100       0x00002803       // AVC Intra (Class 100)


#define MCPROFILE_BD                        0x00003000       // Blu Ray Disc
#define MCPROFILE_BD_HDMV                   0x00003001       // Blu Ray Disc (Main Video)

#define MCPROFILE_PSP                       0x00004000       // Play Station Portable by Sony
#define MCPROFILE_PSP_480x270               0x00004001       // Play Station Portable 480x270 by Sony
#define MCPROFILE_PSP_640x480               0x00004002       // Play Station Portable 640x480 by Sony

#define MCPROFILE_IPOD                      0x00005000       // iPod by Apple
#define MCPROFILE_IPOD_640x480              0x00005001       // iPod 640x480 by Apple

#define MCPROFILE_3GPP                      0x00006000       // 3GPP 
#define MCPROFILE_3GPP2                     0x00006100       // 3GPP2
#define MCPROFILE_ISMA                      0x00006200       // ISMA


#define MCPROFILE_JP2K_LOSSLESS             0x00007000       // JPEG 2000 loss less
#define MCPROFILE_JP2K_LOSSY                0x00007001       // JPEG 2000 lossy
#define MCPROFILE_JP2K_DCI_2K               0x00007100       // JPEG 2000 DCI 2K
#define MCPROFILE_JP2K_DCI_4K               0x00007101       // JPEG 2000 DCI 4K
#define MCPROFILE_MJ2                       0x00007102       // JPEG 2000 file format

#define MCPROFILE_DIVX_QMOBILE              0x00008000       // QMOBILE DivX Profile
#define MCPROFILE_DIVX_MOBILE               0x00008010       // MOBILE DivX Profile
#define MCPROFILE_DIVX_HOME_THEATER         0x00008020       // HOME_THEATER DivX Profile
#define MCPROFILE_DIVX_HDTV                 0x00008030       // HDTV DivX Profile
#define MCPROFILE_DIVX_1080                 0x00008040       // 1080 DivX Profile
#define MCPROFILE_DIVX_UNCONSTRAINED        0x00008100       // Unconstrained DivX Profile

#define MCPROFILE_DIVXPLUS                  0x00008500       // DivX-Plus

#define MCPROFILE_XDCAM_IMX                 0x00010000       // Sony XDCAM IMX
#define MCPROFILE_XDCAM_IMX_25              0x00010000       // Sony XDCAM IMX MPEG-2 4:2:2P @ ML (25 MBit)
#define MCPROFILE_XDCAM_IMX_30              0x00010001       // Sony XDCAM IMX MPEG-2 4:2:2P @ ML (30 MBit)
#define MCPROFILE_XDCAM_IMX_40              0x00010002       // Sony XDCAM IMX MPEG-2 4:2:2P @ ML (40 MBit)
#define MCPROFILE_XDCAM_IMX_50              0x00010003       // Sony XDCAM IMX MPEG-2 4:2:2P @ ML (50 MBit)

#define MCPROFILE_XDCAM_DV                  0x00010050       // Sony XDCAM DV25

#define MCPROFILE_XDCAM_HD                  0x00010100       // Sony XDCAM HD
#define MCPROFILE_XDCAM_1080_CBR_25         0x00010100       // Sony XDCAM MP@H-14 (25 Mbit CBR)
#define MCPROFILE_XDCAM_1080_VBR_17_5       0x00010101       // Sony XDCAM MP@HL (17.5 Mbit VBR)
#define MCPROFILE_XDCAM_1080_VBR_35         0x00010102       // Sony XDCAM MP@HL (35 Mbit VBR)
#define MCPROFILE_XDCAM_540_CBR_12_5        0x00010103       // Sony XDCAM MP@H-14 (12.5 Mbit CBR)
#define MCPROFILE_XDCAM_540_VBR_8_75        0x00010104       // Sony XDCAM MP@HL (8.75 Mbit VBR)
#define MCPROFILE_XDCAM_540_VBR_17_5        0x00010105       // Sony XDCAM MP@HL (17.5 Mbit VBR)
#define MCPROFILE_XDCAM_EX_1920             0x00010110       // Sony XDCAM EX MP@HL   4:2:0 1920x1080 (35 Mbit VBR)
#define MCPROFILE_XDCAM_EX_1440             0x00010111       // Sony XDCAM EX MP@H-14 4:2:0 1440x1080 (25 Mbit CBR)
#define MCPROFILE_XDCAM_EX_1280             0x00010112       // Sony XDCAM EX MP@HL   4:2:0 1280x720  (35 Mbit VBR)
#define MCPROFILE_XDCAM_HD_420_1440         0x00010113       // Sony XDCAM HD MP@H-14 4:2:0 1440x1080 (25 Mbit CBR)
#define MCPROFILE_XDCAM_HD_420_1280         0x00010114       // Sony XDCAM HD MP@HL   4:2:0 1280x720  (35 Mbit VBR)
#define MCPROFILE_XDCAM_HD_422_1920         0x00010115       // Sony XDCAM HD MP@HL   4:2:2 1920x1080 (50 Mbit CBR)
#define MCPROFILE_XDCAM_HD_422_1280         0x00010116       // Sony XDCAM HD MP@HL   4:2:2 1280x720  (50 Mbit CBR)
#define MCPROFILE_XDCAM_HD_420_1280_CBR_25  0x00010117		 // Sony XDCAM HD MP@HL   4:2:0 1280x720  (25 Mbit CBR)

#define MCPROFILE_PANASONIC_P2              0x00011000       // outdated, do not use - Panasonic P2 DVCPRO compatible MXF multiplex (video: SMPTE 383M, audio SMPTE 382M, clipwrapped)
#define MCPROFILE_PANASONIC_P2_DVCPRO       0x00011001       // Panasonic P2 DVCPRO compatible MXF multiplex (video: SMPTE 383M, audio SMPTE 382M, clipwrapped)
#define MCPROFILE_PANASONIC_P2_AVCI         0x00011002       // Panasonic P2 AVCI compatible MXF multiplex (video: SMPTE 383M, audio SMPTE 382M, clipwrapped)

#define MCPROFILE_FLASH_LOWRES              0x00011010       // Adobe Flash (Low Resolution Web Content)
#define MCPROFILE_FLASH_HIGHRES             0x00011011       // Adobe Flash (High Resolution Web Content)


/////////////////////////////////////////////////////////////////


#endif // #if !defined (__MC_PROFILE_INCLUDED__)

