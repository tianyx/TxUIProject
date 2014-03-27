 /********************************************************************
 Created: 2001/05/14 
 File name: mpegdef.h
 Purpose: MPEG definitions and constants

 Copyright (c) 2001-2008 MainConcept AG. All rights reserved.

 This software is the confidential and proprietary information of
 MainConcept AG and may be used only in accordance with the terms of
 your license from MainConcept AG.

*********************************************************************/

#ifndef _MPEGDEF_H_
#define _MPEGDEF_H_

#include "mcfourcc.h"

/* maximum GOP size limits */
#define MAXM               8      // distance between I/P frames
#define MAXN             128      // number of frames in Group of Pictures

/* MPEG types */
#define MPEG_MPEG1                 0       // generic ISO/IEC 11172-1/2
#define MPEG_VCD                   1       // MPEG-1 VideoCD
#define MPEG_MPEG2                 2       // generic ISO/IEC 13818-1/2
#define MPEG_SVCD                  3       // MPEG-2 SuperVCD
#define MPEG_DVD                   4       // MPEG-2 DVD-Video
#define MPEG_TS                    5       // generic MPEG-2 transport stream, PAT and PMT only
#define MPEG_PVA                   6       // PVA stream type, strictly for decoding, do not use for encoding
#define MPEG_NONE                  7       // indicates no muxing
#define MPEG_DVB                   8       // DVB
#define MPEG_MMV                   9       // MPEG-2 MICROMV
#define MPEG_DVHS                  10      // DVHS
#define MPEG_ATSC                  11      // MPEG-2 ATSC normal data rate
#define MPEG_ATSCHI                12      // MPEG-2 ATSC high data rate
#define MPEG_DVD_MPEG1             13      // MPEG-1 DVD-Video
#define MPEG_DVD_DVR               14      // MPEG-2 DVD-VR
#define MPEG_DVD_DVR_MPEG1         15      // MPEG-1 DVD-VR
#define MPEG_DVD_PVR               16      // MPEG-2 DVD+VR, not yet implemented!
#define MPEG_DVD_PVR_MPEG1         17      // MPEG-1 DVD+VR, not yet implemented!
#define MPEG_HDV_HD1               18      // MPEG-2 HDV HD1
#define MPEG_HDV_HD2               19      // MPEG-2 HDV HD2
#define MPEG_D10_25                20      // D-10 MPEG-2 4:2:2P @ ML (25 MBit)
#define MPEG_D10_30                21      // D-10 MPEG-2 4:2:2P @ ML (30 MBit)
#define MPEG_D10_40                22      // D-10 MPEG-2 4:2:2P @ ML (40 MBit)
#define MPEG_D10_50                23      // D-10 MPEG-2 4:2:2P @ ML (50 MBit)
#define MPEG_CABLELABS             24      // CableLabs MPEG-2 MP @ ML (3.18MBit) or MP @ HL (18.1MBit)
#define MPEG_HD_DVD                25      // HD DVD
#define MPEG_BD                    26      // Blu Ray Disc
#define MPEG_BD_HDMV               27      // Blu Ray Disc (Main Video)
#define MPEG_XDCAM_IMX             28      // XDCAM IMX
#define MPEG_XDCAM_IMX_25          28      // XDCAM IMX MPEG-2 4:2:2P @ ML (25 MBit)
#define MPEG_XDCAM_IMX_30          29      // XDCAM IMX MPEG-2 4:2:2P @ ML (30 MBit)
#define MPEG_XDCAM_IMX_40          30      // XDCAM IMX MPEG-2 4:2:2P @ ML (40 MBit)
#define MPEG_XDCAM_IMX_50          31      // XDCAM IMX MPEG-2 4:2:2P @ ML (50 MBit)
#define MPEG_XDCAM_HD              32     // XDCAM HD
#define MPEG_XDCAM_1080_CBR_25     32	    // XDCAM HD MP@H-14 4:2:0 1440x1080  (25 Mbit CBR)
#define MPEG_XDCAM_1080_VBR_17_5   33	    // XDCAM HD MP@HL   4:2:0 1440x1080  (17.5 Mbit VBR)
#define MPEG_XDCAM_1080_VBR_35     34	    // XDCAM HD MP@HL   4:2:0 1440x1080  (35 Mbit VBR)
#define MPEG_XDCAM_540_CBR_12_5    35	    // XDCAM HD MP@H-14 4:2:0 1440x540	  (12.5 Mbit CBR)
#define MPEG_XDCAM_540_VBR_8_75    36	    // XDCAM HD MP@HL   4:2:0 1440x540	  (8.75 Mbit VBR)
#define MPEG_XDCAM_540_VBR_17_5    37	    // XDCAM HD MP@HL   4:2:0 1440x540   (17.5 Mbit VBR)
#define MPEG_XDCAM_HD_420_1280     38     // XDCAM HD MP@HL   4:2:0 1280x720   (35 Mbit VBR)
#define MPEG_XDCAM_HD_422_1920     39     // XDCAM HD 422P@HL 4:2:2 1920x1080  (50 Mbit CBR)
#define MPEG_XDCAM_HD_422_1280     40     // XDCAM HD 422P@HL 4:2:2 1280x720   (50 Mbit CBR)
#define MPEG_XDCAM_EX              41     // XDCAM EX
#define MPEG_XDCAM_EX_1920         41     // XDCAM EX MP@HL   4:2:0 1920x1080  (35 Mbit VBR)
#define MPEG_XDCAM_EX_1440         42     // XDCAM EX MP@H-14 4:2:0 1440x1080  (25 Mbit CBR)
#define MPEG_XDCAM_EX_1280         43     // XDCAM EX MP@HL   4:2:0 1280x720   (35 Mbit VBR)
#define MPEG_AVCHD				   44	   // AVCHD, muxer and pcm encoder type only!
#define MPEG_LAST_MPEG_TYPE        44

/* audio types */
#define NO_AUDIO               0       // no audio
#define MPEG_AUDIO_LAYER1      1       // ISO/IEC 11172-3 layer 1
#define MPEG_AUDIO_LAYER2      2       // ISO/IEC 11172-3 layer 2
#define MPEG_AUDIO_LAYER3      3       // ISO/IEC 11172-3 layer 3
#define PCM_AUDIO              4       // Linear PCM
#define AC3_AUDIO              5       // Dolby digital 
#define MPEG2_AAC_AUDIO        6       // MPEG-2 AAC
#define MPEG4_AAC_AUDIO        7       // MPEG-4 AAC
#define DTS_AUDIO              8       // Digital Theater Sound
#define SDDS_AUDIO             9       // Sony Dynamic Digital Sound
#define AES3_302M_AUDIO        10      // AES3 PCM audio as per SMPTE 302M, MPEG-2 transport stream only 
#define AES3_331M_AUDIO        11      // AES3 PCM audio as per SMPTE 331M, MXF only
#define AES3_382M_AUDIO        12      // AES3 PCM audio as per SMPTE 382M, MXF only
#define HDMV_LPCM_AUDIO		   13	   // HDMV LPCM audio, Blu Ray and AVCHD
#define TWOS_LPCM_AUDIO		   14	   // TWOS LPCM audio, Sony Professional Media Card
#define AMR_AUDIO			   15	   // AMR audio

// stream types reported by mpegin
#define STREAM_ELEMENTARY      0       // audio or video elementary stream
#define STREAM_MPEG1_PROGRAM   1       // mpeg-1 program stream
#define STREAM_MPEG2_PROGRAM   2       // mpeg-2 program stream
#define STREAM_MPEG2_TRANSPORT 3       // mpeg-2 transport stream
#define STREAM_PVA             4       // pva stream
#define STREAM_XDCAM_HD        5       // XDCAM HD MXF stream
#define STREAM_XDCAM_IMX       6       // XDCAM IMX MXF stream
#define STREAM_MP4             7	   // MP4 stream

// options for the XXXChkSettings functions
#define CHECK_MPEG_ONLY       0x00000001  // only do MPEG checks, do not check for VCD/SVCD/DVD compliance
#define CHECK_ALL             0x00000002  // do all checks, normally the check functions return after the first error found

/* default sector sizes */
#define VIDEOCD_SECTOR_SIZE   2324	   // VideoCD sector size 
#define SVCD_SECTOR_SIZE      2324     // Super VideoCD sector size 
#define DVD_SECTOR_SIZE       2048     // DVD sector size     
#define TS_SECTOR_SIZE        188      // transport stream sector size
#define MAX_SECTOR_SIZE       4096

/* VCD video bitrate */
#define MIN_VCD_VIDEO_BITRATE    1140000
#define MAX_VCD_VIDEO_BITRATE    1151929

/* maximum SVCD video bitrate, Note, this assumes no audio! */
/* If audio is used, subtract the audio bitrate from this   */
/* number, i.e. if the audio is 224 kbps, then the video    */
/* bitrate would be 2422848 bps (2646848 - 224000)          */
#define MAX_SVCD_VIDEO_BITRATE           2646848

/* maximum DVD video bitrate */
#define MAX_DVD_VIDEO_BITRATE            9800000

// maximum MicroMV video bitrate
#define MAX_MMV_VIDEO_BITRATE           12000000
 
// maximum ATSC video bitrate (normal)
#define MAX_ATSC_VIDEO_BITRATE          19400000

// maximum ATSC video bitrate (high data rate)
#define MAX_ATSC_HIGH_VIDEO_BITRATE     38800000

// maximum HDV HD1 NTSC video bitrate
#define MAX_HDV_HD1_NTSC_VIDEO_BITRATE  19830000

// maximum HDV HD1 PAL video bitrate
#define MAX_HDV_HD1_PAL_VIDEO_BITRATE   19850000

// maximum HDV HD2 video bitrate
#define MAX_HDV_HD2_VIDEO_BITRATE       25000000

// maximum CableLabs video SD bitrate
#define MAX_CABLELABS_VIDEO_SD_BITRATE   3180000

// maximum CableLabs video HD bitrate
#define MAX_CABLELABS_VIDEO_HD_BITRATE  18100000

/* minimum video bitrate */
#define MIN_VIDEO_BITRATE                 192000

/* maximum MPEG2 video bitrates based on profile/level */
/* high profile */
#define MAX_HP_HL_BITRATE              100000000  // high level
#define MAX_HP_H1440_BITRATE            80000000  // high 1440 level
#define MAX_HP_ML_BITRATE               20000000  // main level
#define MAX_HP_LL_BITRATE                      0  // low level, not allowed

/* main profile */
#define MAX_MP_HL_BITRATE               80000000  // high level
#define MAX_MP_H1440_BITRATE            60000000  // high 1440 level
#define MAX_MP_ML_BITRATE               15000000  // main level
#define MAX_MP_LL_BITRATE                4000000  // low level

/* simple profile */
#define MAX_SP_HL_BITRATE                      0  // high level, not allowed
#define MAX_SP_H1440_BITRATE                   0  // high 1440 level, not allowed
#define MAX_SP_ML_BITRATE               15000000  // main level
#define MAX_SP_LL_BITRATE                      0  // low level, not allowed

/* 4:2:2 profile */
#define MAX_422_HL_BITRATE             300000000  // high level
#define MAX_422_H1440_BITRATE                  0  // high 1440 level, not allowed
#define MAX_422_ML_BITRATE              50000000  // main level
#define MAX_422_LL_BITRATE                     0  // low level, not allowed

/* plain maximums for levels */
#define MAX_HIGH_LEVEL_BITRATE         300000000  // high level
#define MAX_HIGH_1440_BITRATE           80000000  // high 1440 level
#define MAX_MAIN_LEVEL_BITRATE          50000000  // main level
#define MAX_LOW_LEVEL_BITRATE            4000000  // low level

/* maximum MPEG1 video bitrates */
//#define MAX_MPEG1_VIDEO_BITRATE  104857200 // in theory, lower bitrates probably work much better
#define MAX_MPEG1_VIDEO_BITRATE  MAX_MP_ML_BITRATE    // MPEG-2 main profile/main level limit?

/* the maximum MPEG-1 constrained parameters bitrate */
#define MAX_MPEG1_CONSTR_BITRATE         1856000

/* mux rates in 50 byte/sec units */
#define VCD_MUX_RATE                 3528       //  1411200 bps
#define SVCD_MUX_RATE                6972       //  2788800 bps
#define DVD_MUX_RATE                25200       // 10080000 bps

#define MAX_DVB_SDTV_MUX_RATE       22500       //  9000000 bps
#define MAX_DVB_HDTV_MUX_RATE       70000       // 28000000 bps

#define MAX_ATSC_MUX_RATE           48481       // 19392400 bps
#define MAX_ATSC_MUX_RATE_BPS    19392658       // exact bitrate
#define MAX_ATSCHI_MUX_RATE         96963       // 38785200 bps
#define MAX_ATSCHI_MUX_RATE_BPS  38785316       // exact bitrate

#define MAX_HD1_NTSC_MUX_RATE       49575       // 19830000 bps
#define MAX_HD1_PAL_MUX_RATE        49625       // 19850000 bps
#define MAX_HD2_MUX_RATE            82500       // 33000000 bps

// for 1 video and 1 audio stream, if additional audio streams
// the maximum can be higher
#define MAX_CABLELABS_SD_MUX_RATE    9375       //  3750000 bps
#define MAX_CABLELABS_HD_MUX_RATE   47500       // 19000000 bps

/* supported frame rates */
#define FRAMERATE0        0       // use default frame rate
#define FRAMERATE23       1       // 23.976 frames per second
#define FRAMERATE24       2       // 24.0 frames per second
#define FRAMERATE25       3       // 25.0 frames per second
#define FRAMERATE29       4       // 29.97 frames per second
#define FRAMERATE30       5       // 30.0 frames per second
#define FRAMERATE50       6       // 50.0 frames per second
#define FRAMERATE59       7       // 59.94 frames per second
#define FRAMERATE60       8       // 60.0 frames per second

/* NTSC pulldown flags */
#define PULLDOWN_NONE        0
#define PULLDOWN_23          1    // 23.976/24 played as 29.97/30
#define PULLDOWN_32          2    // 23.976/24 played as 29.97/30
#define PULLDOWN_AUTO        3
#define PULLDOWN_23_PROGSEQ  4    // 23.976/24 played as 59.94/60
#define PULLDOWN_32_PROGSEQ  5    // 23.976/24 played as 59.94/60
/* PAL pulldown flags */
#define PULLDOWN_22					 6			// 25 fps played as 50fps 
#define PULLDOWN_22_PROGSEQ	 7			// 25 fps played as 50fps

/* mux rate computation methods */
#define COMPBITRATE_NONE  0
#define COMPBITRATE_AVG   1
#define COMPBITRATE_MAX   2

/* muxer frame/timestamp combinations */
#define TIMESTAMPS_ALL    0
#define TIMESTAMPS_IPONLY 1
#define TIMESTAMPS_IONLY  2

/* profile IDs, not original MPEG standard value */
#define PROFILE_MULTIVIEW 8  // currently not supported
#define PROFILE_422       7
#define PROFILE_SIMPLE    5
#define PROFILE_MAIN      4
#define PROFILE_SNR       3  // currently not supported
#define PROFILE_SPAT      2  // currently not supported
#define PROFILE_HIGH      1

/* level IDs */
#define LEVEL_LOW       10
#define LEVEL_MAIN       8
#define LEVEL_HIGH1440   6
#define LEVEL_HIGH       4
#define LEVEL_UNSPECIFIED 15


/* video start codes */
#define PICTURE_START_CODE        0x100L
#define SLICE_MIN_START           0x101L
#define SLICE_MAX_START           0x1AFL
#define USER_START_CODE           0x1B2L
#define SEQ_START_CODE            0x1B3L
#define EXT_START_CODE            0x1B5L
#define SEQ_END_CODE              0x1B7L
#define GOP_START_CODE            0x1B8L

/* system start codes */
#define ISO_END_CODE              0x1B9L
#define PACK_START_CODE           0x1BAL
#define SYSTEM_START_CODE         0x1BBL

#define PROGRAM_STREAM_MAP        0x1BCL
#define PRIVATE_STREAM_1          0x1BDL
#define PADDING_STREAM            0x1BEL
#define PRIVATE_STREAM_2          0x1BFL

#define AUDIO_ELEMENTARY_STREAM   0x1C0L
#define AUDIO_ELEMENTARY_STREAM0  0x1C0L
#define AUDIO_ELEMENTARY_STREAM1  0x1C1L
#define AUDIO_ELEMENTARY_STREAM2  0x1C2L
#define AUDIO_ELEMENTARY_STREAM3  0x1C3L
#define AUDIO_ELEMENTARY_STREAM4  0x1C4L
#define AUDIO_ELEMENTARY_STREAM5  0x1C5L
#define AUDIO_ELEMENTARY_STREAM6  0x1C6L
#define AUDIO_ELEMENTARY_STREAM7  0x1C7L
#define AUDIO_ELEMENTARY_STREAM8  0x1C8L
#define AUDIO_ELEMENTARY_STREAM9  0x1C9L
#define AUDIO_ELEMENTARY_STREAM10 0x1CAL
#define AUDIO_ELEMENTARY_STREAM11 0x1CBL
#define AUDIO_ELEMENTARY_STREAM12 0x1CCL
#define AUDIO_ELEMENTARY_STREAM13 0x1CDL
#define AUDIO_ELEMENTARY_STREAM14 0x1CEL
#define AUDIO_ELEMENTARY_STREAM15 0x1CFL
#define AUDIO_ELEMENTARY_STREAM16 0x1D0L
#define AUDIO_ELEMENTARY_STREAM17 0x1D1L
#define AUDIO_ELEMENTARY_STREAM18 0x1D2L
#define AUDIO_ELEMENTARY_STREAM19 0x1D3L
#define AUDIO_ELEMENTARY_STREAM20 0x1D4L
#define AUDIO_ELEMENTARY_STREAM21 0x1D5L
#define AUDIO_ELEMENTARY_STREAM22 0x1D6L
#define AUDIO_ELEMENTARY_STREAM23 0x1D7L
#define AUDIO_ELEMENTARY_STREAM24 0x1D8L
#define AUDIO_ELEMENTARY_STREAM25 0x1D9L
#define AUDIO_ELEMENTARY_STREAM26 0x1DAL
#define AUDIO_ELEMENTARY_STREAM27 0x1DBL
#define AUDIO_ELEMENTARY_STREAM28 0x1DCL
#define AUDIO_ELEMENTARY_STREAM29 0x1DDL
#define AUDIO_ELEMENTARY_STREAM30 0x1DEL
#define AUDIO_ELEMENTARY_STREAM31 0x1DFL

#define VIDEO_ELEMENTARY_STREAM   0x1E0L
#define VIDEO_ELEMENTARY_STREAM0  0x1E0L
#define VIDEO_ELEMENTARY_STREAM1  0x1E1L
#define VIDEO_ELEMENTARY_STREAM2  0x1E2L
#define VIDEO_ELEMENTARY_STREAM3  0x1E3L
#define VIDEO_ELEMENTARY_STREAM4  0x1E4L
#define VIDEO_ELEMENTARY_STREAM5  0x1E5L
#define VIDEO_ELEMENTARY_STREAM6  0x1E6L
#define VIDEO_ELEMENTARY_STREAM7  0x1E7L
#define VIDEO_ELEMENTARY_STREAM8  0x1E8L
#define VIDEO_ELEMENTARY_STREAM9  0x1E9L
#define VIDEO_ELEMENTARY_STREAM10 0x1EAL
#define VIDEO_ELEMENTARY_STREAM11 0x1EBL
#define VIDEO_ELEMENTARY_STREAM12 0x1ECL
#define VIDEO_ELEMENTARY_STREAM13 0x1EDL
#define VIDEO_ELEMENTARY_STREAM14 0x1EEL
#define VIDEO_ELEMENTARY_STREAM15 0x1EFL

#define ECM_STREAM                0x1F0L
#define EMM_STREAM                0x1F1L
#define ITU_T_H2220_STREAM        0x1F2L
#define ISO_IEC_13522_STREAM      0x1F3L
#define ITU_T_H2221_TYPEA_STREAM  0x1F4L
#define ITU_T_H2221_TYPEB_STREAM  0x1F5L
#define ITU_T_H2221_TYPEC_STREAM  0x1F6L
#define ITU_T_H2221_TYPED_STREAM  0x1F7L
#define ITU_T_H2221_TYPEE_STREAM  0x1F8L
#define ANCILLARY_STREAM          0x1F9L

#define RESERVED_DATA_STREAM0     0x1FAL
#define RESERVED_DATA_STREAM1     0x1FBL
#define RESERVED_DATA_STREAM2     0x1FCL
#define RESERVED_DATA_STREAM3     0x1FDL
#define RESERVED_DATA_STREAM4     0x1FEL

#define PROGRAM_STREAM_DIRECTORY  0x1FFL
#define SYSTEM_START_CODE_MAX     0x1FFL

/* DVD substream defines */
#define PCI_NAVIGATION            0x00
#define DSI_NAVIGATION            0x01

/* subpicture streams */
#define SUBPICTURE_STREAM0        0x20
#define SUBPICTURE_STREAM1        0x21
#define SUBPICTURE_STREAM2        0x22
#define SUBPICTURE_STREAM3        0x23
#define SUBPICTURE_STREAM4        0x24
#define SUBPICTURE_STREAM5        0x25
#define SUBPICTURE_STREAM6        0x26
#define SUBPICTURE_STREAM7        0x27
#define SUBPICTURE_STREAM8        0x28
#define SUBPICTURE_STREAM9        0x29
#define SUBPICTURE_STREAM10       0x2A
#define SUBPICTURE_STREAM11       0x2B
#define SUBPICTURE_STREAM12       0x2C
#define SUBPICTURE_STREAM13       0x2D
#define SUBPICTURE_STREAM14       0x2E
#define SUBPICTURE_STREAM15       0x2F
#define SUBPICTURE_STREAM16       0x30
#define SUBPICTURE_STREAM17       0x31
#define SUBPICTURE_STREAM18       0x32
#define SUBPICTURE_STREAM19       0x33
#define SUBPICTURE_STREAM20       0x34
#define SUBPICTURE_STREAM21       0x35
#define SUBPICTURE_STREAM22       0x36
#define SUBPICTURE_STREAM23       0x37
#define SUBPICTURE_STREAM24       0x38
#define SUBPICTURE_STREAM25       0x39
#define SUBPICTURE_STREAM26       0x3A
#define SUBPICTURE_STREAM27       0x3B
#define SUBPICTURE_STREAM28       0x3C
#define SUBPICTURE_STREAM29       0x3D
#define SUBPICTURE_STREAM30       0x3E 
#define SUBPICTURE_STREAM31       0x3F

/* AC3 streams */
#define AC3_STREAM0               0x80
#define AC3_STREAM1               0x81
#define AC3_STREAM2               0x82
#define AC3_STREAM3               0x83
#define AC3_STREAM4               0x84
#define AC3_STREAM5               0x85
#define AC3_STREAM6               0x86
#define AC3_STREAM7               0x87

/* DTS streams */
#define DTS_STREAM0               0x88
#define DTS_STREAM1               0x89
#define DTS_STREAM2               0x8A
#define DTS_STREAM3               0x8B
#define DTS_STREAM4               0x8C
#define DTS_STREAM5               0x8D
#define DTS_STREAM6               0x8E
#define DTS_STREAM7               0x8F

/* SDDS streams */
#define SDDS_STREAM0              0x90
#define SDDS_STREAM1              0x91
#define SDDS_STREAM2              0x92
#define SDDS_STREAM3              0x93
#define SDDS_STREAM4              0x94
#define SDDS_STREAM5              0x95
#define SDDS_STREAM6              0x96
#define SDDS_STREAM7              0x97

/* PCM streams */
#define PCM_STREAM0               0xA0
#define PCM_STREAM1               0xA1
#define PCM_STREAM2               0xA2
#define PCM_STREAM3               0xA3
#define PCM_STREAM4               0xA4
#define PCM_STREAM5               0xA5
#define PCM_STREAM6               0xA6
#define PCM_STREAM7               0xA7

/* other system defines */
#define MIN_MPEG2_AFTER_PACKET_LENGTH  3
#define MIN_MPEG1_AFTER_PACKET_LENGTH  1

#define MAX_MPEG2_AFTER_PACKET_LENGTH  16
#define MAX_MPEG1_AFTER_PACKET_LENGTH  12

#define MPEG1_PACK_HEADER_SIZE         12
#define MPEG2_PACK_HEADER_SIZE	       14

#define PACKET_HEADER_SIZE	           6

/* picture structure */
#define TOP_FIELD     1
#define BOTTOM_FIELD  2
#define FRAME_PICTURE 3

/* extension start code IDs */
#define SEQ_ID       1
#define DISP_ID      2
#define QUANT_ID     3
#define SEQSCAL_ID   5
#define PANSCAN_ID   7
#define CODING_ID    8
#define SPATSCAL_ID  9
#define TEMPSCAL_ID 10

/* picture coding type */
#define I_TYPE 1
#define P_TYPE 2
#define B_TYPE 3
#define D_TYPE 4

/* macroblock type */
#define MB_INTRA    1
#define MB_PATTERN  2
#define MB_BACKWARD 4
#define MB_FORWARD  8
#define MB_QUANT    16

/* motion_type */
#define MC_FIELD 1
#define MC_FRAME 2
#define MC_16X8  2
#define MC_DMV   3

/* mv_format */
#define MV_FIELD 0
#define MV_FRAME 1

/* chroma_format */
#define CHROMA420 1
#define CHROMA422 2
#define CHROMA444 3

/* muxer flags */
#define SEQHDR_FLAG             0x01
#define GOPHDR_FLAG             0x02
#define SVCD_USER_BLOCK         0x04
#define SEQ_USER_BLOCK          0x08
#define PIC_USER_BLOCK          0x10
#define SPLIT_FILE_FLAG         0x20
#define AU_FLAG                 0x40

/* audio_mode */
#define MPG_MD_STEREO           0
#define MPG_MD_JOINT_STEREO     1
#define MPG_MD_DUAL_CHANNEL     2
#define MPG_MD_MONO             3

/* mpeg output return codes */
#define mpegOutErrNone    0 	  // no error
#define mpegOutCancel  	  1       // user canceled settings
#define mpegOutError      2       // undefined error
#define mpegOutDecline    3       // unable to use input settings, resolution, audio etc.
#define mpegOutEOFReached 4       // returned by SR, when mpegin returns mpegInEOF

/* mpeg input return codes */
#define mpegInErrNone     0 	  // no error
#define mpegInError       1       // undefined error
#define mpegInEOF         2       // end of file error

// access modes for mpgvdec and mpgadec
// accurate is only needed if the PTS's of a program stream are not
// contiguous. it is always used when reading an elemental stream
#define ACCESS_ACCURATE   0  // parse entire file when opening
#define ACCESS_FAST       1  // read first/last PTS's and use time stamps for seeking

// for mpegOutVideoDefaults(mpeg_v_settings *set, int video_type, int PAL);
#define VM_NTSC           0
#define VM_PAL            1

// MPEG-1 layer 1 bit rates
#define L1_AUDIOBITRATE32    1
#define L1_AUDIOBITRATE64    2
#define L1_AUDIOBITRATE96    3
#define L1_AUDIOBITRATE128   4
#define L1_AUDIOBITRATE160   5
#define L1_AUDIOBITRATE192   6
#define L1_AUDIOBITRATE224   7
#define L1_AUDIOBITRATE256   8
#define L1_AUDIOBITRATE288   9
#define L1_AUDIOBITRATE320   10
#define L1_AUDIOBITRATE352   11
#define L1_AUDIOBITRATE384   12
#define L1_AUDIOBITRATE416   13
#define L1_AUDIOBITRATE448   14

// MPEG-1 layer 2 bit rates
#define L2_AUDIOBITRATE32    1
#define L2_AUDIOBITRATE48    2
#define L2_AUDIOBITRATE56    3
#define L2_AUDIOBITRATE64    4
#define L2_AUDIOBITRATE80    5
#define L2_AUDIOBITRATE96    6
#define L2_AUDIOBITRATE112   7
#define L2_AUDIOBITRATE128   8
#define L2_AUDIOBITRATE160   9
#define L2_AUDIOBITRATE192   10
#define L2_AUDIOBITRATE224   11
#define L2_AUDIOBITRATE256   12
#define L2_AUDIOBITRATE320   13
#define L2_AUDIOBITRATE384   14

// MPEG-2 LSF layer 1 bit rates
#define LSF_L1_AUDIOBITRATE32    1
#define LSF_L1_AUDIOBITRATE48    2
#define LSF_L1_AUDIOBITRATE56    3
#define LSF_L1_AUDIOBITRATE64    4
#define LSF_L1_AUDIOBITRATE80    5
#define LSF_L1_AUDIOBITRATE96    6
#define LSF_L1_AUDIOBITRATE112   7
#define LSF_L1_AUDIOBITRATE128   8
#define LSF_L1_AUDIOBITRATE144   9
#define LSF_L1_AUDIOBITRATE160   10
#define LSF_L1_AUDIOBITRATE176   11
#define LSF_L1_AUDIOBITRATE192   12
#define LSF_L1_AUDIOBITRATE224   13
#define LSF_L1_AUDIOBITRATE256   14

// MPEG-2 LSF layer 2 bit rates
#define LSF_L2_AUDIOBITRATE8     1
#define LSF_L2_AUDIOBITRATE16    2
#define LSF_L2_AUDIOBITRATE24    3
#define LSF_L2_AUDIOBITRATE32    4
#define LSF_L2_AUDIOBITRATE40    5
#define LSF_L2_AUDIOBITRATE48    6
#define LSF_L2_AUDIOBITRATE56    7
#define LSF_L2_AUDIOBITRATE64    8
#define LSF_L2_AUDIOBITRATE80    9
#define LSF_L2_AUDIOBITRATE96    10
#define LSF_L2_AUDIOBITRATE112   11
#define LSF_L2_AUDIOBITRATE128   12
#define LSF_L2_AUDIOBITRATE144   13
#define LSF_L2_AUDIOBITRATE160   14

// video encoder performance level
#define VIDEO_LEVEL_MIN   0
#define VIDEO_LEVEL_MAX   31

// defines for variable/constant bitrate
// used for both the video encoder and the multiplexer
#define VBR_CONSTANT      1
#define VBR_VARIABLE1     0
//#define VBR_VARIABLE2     2   // was never really used
//#define VBR_VARIABLE3     3   // was never really used
#define VBR_CQT           2
#define VBR_CQT_STRICT    3

// PCM audio defines
#define PCM_1CHANNEL      0
#define PCM_2CHANNEL      1
#define PCM_3CHANNEL      2
#define PCM_4CHANNEL      3
#define PCM_5CHANNEL      4
#define PCM_6CHANNEL      5
#define PCM_7CHANNEL      6
#define PCM_8CHANNEL      7

#define PCM_EMPHASIS_OFF  0
#define PCM_EMPHASIS_ON   1

#define PCM_MUTE_OFF      0
#define PCM_MUTE_ON       1

#define PCM_16BITS        0
#define PCM_20BITS        1  // currently AES3 only
#define PCM_24BITS        2  // currently AES3 only

#define PCM_NO_DYNAMIC_RANGE_CONTROL  0x80

// AC3 audio defines
#define AC3_CHANNEL_LEFT		0
#define AC3_CHANNEL_CENTER		1
#define AC3_CHANNEL_RIGHT		2
#define AC3_CHANNEL_LEFT_SUR	3
#define AC3_CHANNEL_RIGHT_SUR	4
#define AC3_CHANNEL_LFE			5

#define AC3_AUDIOMODE_DSC	0	// Dolby Surround compatible (Lt,Rt)
#define AC3_AUDIOMODE_1_1   0   // (L,R)		2 channels total
#define AC3_AUDIOMODE_1_0   1   // (C)			1 channel total
#define AC3_AUDIOMODE_2_0   2   // (L,R)		2 channels total
#define AC3_AUDIOMODE_3_0   3   // (L,C,R)		3 channels total
#define AC3_AUDIOMODE_2_1   4   // (L,R,l)		3 channels total
#define AC3_AUDIOMODE_3_1   5   // (L,C,R,l)	4 channels total
#define AC3_AUDIOMODE_2_2   6   // (L,R,l,r)	4 channels total
#define AC3_AUDIOMODE_3_2   7   // (L,C,R,l,r)	5 channels total

#define AC3_BITRATE_64      4
#define AC3_BITRATE_80      5
#define AC3_BITRATE_96      6
#define AC3_BITRATE_112     7
#define AC3_BITRATE_128     8
#define AC3_BITRATE_160     9
#define AC3_BITRATE_192    10
#define AC3_BITRATE_224    11
#define AC3_BITRATE_256    12
#define AC3_BITRATE_320    13
#define AC3_BITRATE_384    14
#define AC3_BITRATE_448    15
#define AC3_BITRATE_512    16
#define AC3_BITRATE_576    17
#define AC3_BITRATE_640    18


// defines for VCD conformance check errors
#define INV_VCD_VID_ERRORS             0x00000100
#define INV_VCD_VID_FRAMERATE          INV_VCD_VID_ERRORS + 0
#define INV_VCD_VID_DEF_HSIZE          INV_VCD_VID_ERRORS + 1
#define INV_VCD_VID_DEF_VSIZE          INV_VCD_VID_ERRORS + 2
#define INV_VCD_VID_CBR                INV_VCD_VID_ERRORS + 3
#define INV_VCD_VID_BITRATE            INV_VCD_VID_ERRORS + 4

#define INV_VCD_AUD_ERRORS             0x00000200
#define INV_VCD_AUD_LAYER              INV_VCD_AUD_ERRORS + 0
#define INV_VCD_AUD_MODE               INV_VCD_AUD_ERRORS + 1
#define INV_VCD_AUD_BITRATE            INV_VCD_AUD_ERRORS + 2
#define INV_VCD_AUD_SAMPLE_RATE        INV_VCD_AUD_ERRORS + 3
#define INV_VCD_AUD_TYPE							 INV_VCD_AUD_ERRORS + 4

#define INV_VCD_MUX_ERRORS             0x00000400
#define INV_VCD_MUX_SECTOR_SIZE        INV_VCD_MUX_ERRORS + 0
#define INV_VCD_MUX_PACKETS_PER_PACK   INV_VCD_MUX_ERRORS + 1
#define INV_VCD_MUX_AUDIO1_BSIZE       INV_VCD_MUX_ERRORS + 2
#define INV_VCD_MUX_VIDEO_BSIZE        INV_VCD_MUX_ERRORS + 3
#define INV_VCD_MUX_MUX_RATE           INV_VCD_MUX_ERRORS + 4
#define INV_VCD_MUX_TIMESTAMPS         INV_VCD_MUX_ERRORS + 5
#define INV_VCD_MUX_VBR_MULTIPLEX      INV_VCD_MUX_ERRORS + 6
#define INV_VCD_MUX_NOVCD_MUX          INV_VCD_MUX_ERRORS + 7

// defines for SVCD conformance check errors
#define INV_SVCD_VID_ERRORS            0x00000800
#define INV_SVCD_VID_FRAMERATE         INV_SVCD_VID_ERRORS + 0
#define INV_SVCD_VID_MPEGN             INV_SVCD_VID_ERRORS + 1
#define INV_SVCD_VID_DEF_VSIZE         INV_SVCD_VID_ERRORS + 2
#define INV_SVCD_VID_DEF_HSIZE         INV_SVCD_VID_ERRORS + 3
#define INV_SVCD_VID_MPEGM             INV_SVCD_VID_ERRORS + 4
#define INV_SVCD_VID_PROFILE           INV_SVCD_VID_ERRORS + 5
#define INV_SVCD_VID_LEVEL             INV_SVCD_VID_ERRORS + 6
#define INV_SVCD_VID_DATA_BLOCKS       INV_SVCD_VID_ERRORS + 7
#define INV_SVCD_VID_PSEQ              INV_SVCD_VID_ERRORS + 8
#define INV_SVCD_VID_ARATIO            INV_SVCD_VID_ERRORS + 9
#define INV_SVCD_VID_BITRATE           INV_SVCD_VID_ERRORS + 10
#define INV_SVCD_VID_MAXBITRATE        INV_SVCD_VID_ERRORS + 11
#define INV_SVCD_VID_VBVBUFFER         INV_SVCD_VID_ERRORS + 12

#define INV_SVCD_AUD_ERRORS            0x00001000
#define INV_SVCD_AUD_LAYER             INV_SVCD_AUD_ERRORS + 0
#define INV_SVCD_AUD_EPROTECTION       INV_SVCD_AUD_ERRORS + 1 
#define INV_SVCD_AUD_EMPHASIS          INV_SVCD_AUD_ERRORS + 2 
#define INV_SVCD_AUD_BITRATE           INV_SVCD_AUD_ERRORS + 3 
#define INV_SVCD_AUD_SAMPLE_RATE       INV_SVCD_AUD_ERRORS + 4 
#define INV_SVCD_AUD_TYPE							 INV_SVCD_AUD_ERRORS + 5

#define INV_SVCD_MUX_ERRORS            0x00002000
#define INV_SVCD_MUX_SECTOR_SIZE       INV_SVCD_MUX_ERRORS + 0
#define INV_SVCD_MUX_PACKETS_PER_PACK  INV_SVCD_MUX_ERRORS + 1
#define INV_SVCD_MUX_AUDIO1_BSIZE      INV_SVCD_MUX_ERRORS + 2
#define INV_SVCD_MUX_AUDIO2_BSIZE      INV_SVCD_MUX_ERRORS + 3
#define INV_SVCD_MUX_VIDEO_BSIZE       INV_SVCD_MUX_ERRORS + 4
#define INV_SVCD_MUX_MUX_RATE          INV_SVCD_MUX_ERRORS + 5
#define INV_SVCD_MUX_SCAN_OFFSETS      INV_SVCD_MUX_ERRORS + 6
#define INV_SVCD_MUX_NOSVCD_MUX        INV_SVCD_MUX_ERRORS + 7

// defines for DVD conformance check errors
#define INV_DVD_VID_ERRORS             0x00004000
#define INV_DVD_VID_FRAMERATE          INV_DVD_VID_ERRORS + 0
#define INV_DVD_VID_MPEGN              INV_DVD_VID_ERRORS + 1
#define INV_DVD_VID_DEF_HSIZE          INV_DVD_VID_ERRORS + 2
#define INV_DVD_VID_DEF_VSIZE          INV_DVD_VID_ERRORS + 3
#define INV_DVD_VID_TRANSFER_CHAR      INV_DVD_VID_ERRORS + 4
#define INV_DVD_VID_COLOR_PRIM         INV_DVD_VID_ERRORS + 5
#define INV_DVD_VID_DEF_DISP_VSIZE     INV_DVD_VID_ERRORS + 6
#define INV_DVD_VID_PROFILE            INV_DVD_VID_ERRORS + 7
#define INV_DVD_VID_LEVEL              INV_DVD_VID_ERRORS + 8
#define INV_DVD_VID_PSEQ               INV_DVD_VID_ERRORS + 9
#define INV_DVD_VID_ARATIO             INV_DVD_VID_ERRORS + 10
#define INV_DVD_VID_BITRATE            INV_DVD_VID_ERRORS + 11 
#define INV_DVD_VID_MAXBITRATE         INV_DVD_VID_ERRORS + 12
#define INV_DVD_VID_DEF_DISP_HSIZE     INV_DVD_VID_ERRORS + 13
#define INV_DVD_VID_DEF_HSIZE_HCENTER  INV_DVD_VID_ERRORS + 14
#define INV_DVD_VID_VCENTER            INV_DVD_VID_ERRORS + 15
#define INV_DVD_VID_MATRIX_COEFF       INV_DVD_VID_ERRORS + 16
#define INV_DVD_VID_SEQ_HDR            INV_DVD_VID_ERRORS + 17
#define INV_DVD_VID_MPEG1_DEF_HSIZE    INV_DVD_VID_ERRORS + 18
#define INV_DVD_VID_MPEG1_ARATIO       INV_DVD_VID_ERRORS + 19
#define INV_DVD_VID_MPEG1_DEF_VSIZE    INV_DVD_VID_ERRORS + 20
#define INV_DVD_VID_DVR_DISP_HSIZE     INV_DVD_VID_ERRORS + 21
#define INV_DVD_VID_DVR_HSIZE_HCENTER  INV_DVD_VID_ERRORS + 22
#define INV_DVD_VID_MPEG1_CPARMS       INV_DVD_VID_ERRORS + 23

#define INV_DVD_AUD_ERRORS             0x00008000
#define INV_DVD_AUD_LAYER              INV_DVD_AUD_ERRORS + 0
#define INV_DVD_AUD_EPROTECTION        INV_DVD_AUD_ERRORS + 1
#define INV_DVD_AUD_EMPHASIS           INV_DVD_AUD_ERRORS + 2
#define INV_DVD_AUD_PRIVATEBIT         INV_DVD_AUD_ERRORS + 3
#define INV_DVD_AUD_BITRATE            INV_DVD_AUD_ERRORS + 4
#define INV_DVD_AUD_MODE               INV_DVD_AUD_ERRORS + 5
#define INV_DVD_AUD_SAMPLE_RATE        INV_DVD_AUD_ERRORS + 6
#define INV_DVD_AUD_DVR_PCM_CHANNELS   INV_DVD_AUD_ERRORS + 7
#define INV_DVD_AUD_DVR_PCM_QUANT      INV_DVD_AUD_ERRORS + 8
#define INV_DVD_AUD_DVR_PCM_FREQUENCY  INV_DVD_AUD_ERRORS + 9
#define INV_DVD_AUD_TYPE			   INV_DVD_AUD_ERRORS + 10
#define INV_DVD_AUD_ENDIAN			   INV_DVD_AUD_ERRORS + 11

#define INV_DVD_MUX_ERRORS             0x00010000
#define INV_DVD_MUX_SECTOR_SIZE        INV_DVD_MUX_ERRORS + 0
#define INV_DVD_MUX_PACKETS_PER_PACK   INV_DVD_MUX_ERRORS + 1
#define INV_DVD_MUX_AUDIO1_BSIZE       INV_DVD_MUX_ERRORS + 2
#define INV_DVD_MUX_AUDIO2_BSIZE       INV_DVD_MUX_ERRORS + 3
#define INV_DVD_MUX_VIDEO_BSIZE        INV_DVD_MUX_ERRORS + 4
#define INV_DVD_MUX_MUX_RATE           INV_DVD_MUX_ERRORS + 5
#define INV_DVD_MUX_ALIGN_SH           INV_DVD_MUX_ERRORS + 6
#define INV_DVD_MUX_PRIVATE2           INV_DVD_MUX_ERRORS + 7
#define INV_DVD_MUX_NODVD_MUX          INV_DVD_MUX_ERRORS + 8
#define INV_DVD_MUX_MPEG1_VIDEO_BSIZE  INV_DVD_MUX_ERRORS + 9
#define INV_DVD_MUX_NODVD_MPEG1_MUX    INV_DVD_MUX_ERRORS + 10
#define INV_DVD_MUX_NODVD_DVR_MUX      INV_DVD_MUX_ERRORS + 11
#define INV_DVD_MUX_NODVD_DVR_MPEG1_MUX INV_DVD_MUX_ERRORS + 12

// defines for DVB conformance check errors
#define INV_DVB_VID_ERRORS             0x00012000
#define INV_DVB_VID_PROFILE            INV_DVB_VID_ERRORS + 0
#define INV_DVB_VID_DEF_VSIZE          INV_DVB_VID_ERRORS + 1
#define INV_DVB_VID_FRAMERATE          INV_DVB_VID_ERRORS + 2
#define INV_DVB_VID_CHROMA             INV_DVB_VID_ERRORS + 3
#define INV_DVB_VID_SEQ_HDR            INV_DVB_VID_ERRORS + 4
#define INV_DVB_VID_ARATIO             INV_DVB_VID_ERRORS + 5
#define INV_DVB_VID_DEF_HSIZE          INV_DVB_VID_ERRORS + 6

#define INV_DVB_AUD_ERRORS             0x00012100
#define INV_DVB_AUD_LAYER              INV_DVB_AUD_ERRORS + 0
#define INV_DVB_AUD_EPROTECTION        INV_DVB_AUD_ERRORS + 1
#define INV_DVB_AUD_EMPHASIS           INV_DVB_AUD_ERRORS + 2
#define INV_DVB_AUD_TYPE							 INV_DVB_AUD_ERRORS + 3

#define INV_DVB_MUX_ERRORS             0x00012200
#define INV_DVB_MUX_NODVB_MUX          INV_DVB_MUX_ERRORS + 0
#define INV_DVB_MUX_MUX_RATE           INV_DVB_MUX_ERRORS + 1

// defines for MICROMV conformance check errors
#define INV_MMV_VID_ERRORS             0x00013000
#define INV_MMV_VID_PROFILE            INV_MMV_VID_ERRORS + 0
#define INV_MMV_VID_LEVEL              INV_MMV_VID_ERRORS + 1
#define INV_MMV_VID_MPEGN              INV_MMV_VID_ERRORS + 2
#define INV_MMV_VID_DEF_VSIZE          INV_MMV_VID_ERRORS + 3
#define INV_MMV_VID_FRAMERATE          INV_MMV_VID_ERRORS + 4
#define INV_MMV_VID_MPEGM              INV_MMV_VID_ERRORS + 5
#define INV_MMV_VID_SEQ_HDR            INV_MMV_VID_ERRORS + 6
#define INV_MMV_VID_ARATIO             INV_MMV_VID_ERRORS + 7
#define INV_MMV_VID_DEF_HSIZE          INV_MMV_VID_ERRORS + 8

#define INV_MMV_AUD_ERRORS             0x00013100
#define INV_MMV_AUD_LAYER              INV_MMV_AUD_ERRORS + 0
#define INV_MMV_AUD_EPROTECTION        INV_MMV_AUD_ERRORS + 1
#define INV_MMV_AUD_EMPHASIS           INV_MMV_AUD_ERRORS + 2
#define INV_MMV_AUD_SAMPLE_RATE        INV_MMV_AUD_ERRORS + 3
#define INV_MMV_AUD_BITRATE            INV_MMV_AUD_ERRORS + 4
#define INV_MMV_AUD_TYPE							 INV_MMV_AUD_ERRORS + 5

#define INV_MMV_MUX_ERRORS             0x00013200
#define INV_MMV_MUX_NOMMV_MUX          INV_MMV_MUX_ERRORS + 0

// defines for DVHS conformance check errors
#define INV_DVHS_VID_ERRORS            0x00014000
#define INV_DVHS_VID_PROFILE           INV_DVHS_VID_ERRORS + 0
#define INV_DVHS_VID_LEVEL             INV_DVHS_VID_ERRORS + 1
#define INV_DVHS_VID_DEF_VSIZE         INV_DVHS_VID_ERRORS + 2
#define INV_DVHS_VID_FRAMERATE         INV_DVHS_VID_ERRORS + 3
#define INV_DVHS_VID_CHROMA            INV_DVHS_VID_ERRORS + 4
#define INV_DVHS_VID_SEQ_HDR           INV_DVHS_VID_ERRORS + 5
#define INV_DVHS_VID_DEF_HSIZE         INV_DVHS_VID_ERRORS + 6
#define INV_DVHS_VID_ARATIO            INV_DVHS_VID_ERRORS + 7
#define INV_DVHS_VID_PROGSEQ           INV_DVHS_VID_ERRORS + 8

#define INV_DVHS_AUD_ERRORS            0x00014100
#define INV_DVHS_AUD_LAYER             INV_DVHS_AUD_ERRORS + 0
#define INV_DVHS_AUD_EPROTECTION       INV_DVHS_AUD_ERRORS + 1
#define INV_DVHS_AUD_EMPHASIS          INV_DVHS_AUD_ERRORS + 2
#define INV_DVHS_AUD_BITRATE           INV_DVHS_AUD_ERRORS + 3
#define INV_DVHS_PCM_CHANNELS          INV_DVHS_AUD_ERRORS + 4
#define INV_DVHS_PCM_EMPHASIS          INV_DVHS_AUD_ERRORS + 5
#define INV_DVHS_PCM_BITDEPTH          INV_DVHS_AUD_ERRORS + 6
#define INV_DVHS_PCM_FREQUENCY         INV_DVHS_AUD_ERRORS + 7
#define INV_DVHS_AUD_SAMPLE_RATE       INV_DVHS_AUD_ERRORS + 8
#define INV_DVHS_AUD_CMODE             INV_DVHS_AUD_ERRORS + 9
#define INV_DVHS_AUD_TYPE							 INV_DVHS_AUD_ERRORS + 10

#define INV_DVHS_MUX_ERRORS            0x00014200
#define INV_DVHS_MUX_NODVHS_MUX        INV_DVHS_MUX_ERRORS + 0
#define INV_DVHS_MUX_MUX_RATE          INV_DVHS_MUX_ERRORS + 1

// defines for ATSC conformance check errors
#define INV_ATSC_VID_ERRORS            0x00015000
#define INV_ATSC_VID_PROFILE           INV_ATSC_VID_ERRORS + 0
#define INV_ATSC_VID_DEF_VSIZE         INV_ATSC_VID_ERRORS + 1
#define INV_ATSC_VID_FRAMERATE         INV_ATSC_VID_ERRORS + 2
#define INV_ATSC_VID_BITRATE           INV_ATSC_VID_ERRORS + 3
#define INV_ATSC_VID_CHROMA            INV_ATSC_VID_ERRORS + 4
#define INV_ATSC_VID_VBVBUFFER         INV_ATSC_VID_ERRORS + 5
#define INV_ATSC_VID_SEQ_HDR           INV_ATSC_VID_ERRORS + 6
#define INV_ATSC_VID_DEF_HSIZE         INV_ATSC_VID_ERRORS + 7
#define INV_ATSC_VID_ARATIO            INV_ATSC_VID_ERRORS + 8
#define INV_ATSC_VID_PROGSEQ           INV_ATSC_VID_ERRORS + 9

#define INV_ATSC_AUD_ERRORS            0x00015100
#define INV_ATSC_AUD_TYPE							 INV_ATSC_AUD_ERRORS + 0

#define INV_ATSC_MUX_ERRORS            0x00015200
#define INV_ATSC_MUX_NOATSC_MUX        INV_ATSC_MUX_ERRORS + 0
#define INV_ATSC_MUX_MUX_RATE          INV_ATSC_MUX_ERRORS + 1
#define INV_ATSC_MUX_HI_MUX_RATE       INV_ATSC_MUX_ERRORS + 2

// defines for HDV conformance check errors
#define INV_HDV_VID_ERRORS             0x00016000
#define INV_HDV_VID_PROFILE            INV_HDV_VID_ERRORS + 0
#define INV_HDV_VID_LEVEL              INV_HDV_VID_ERRORS + 1
#define INV_HDV_VID_MPEGN              INV_HDV_VID_ERRORS + 2
#define INV_HDV_VID_DEF_VSIZE          INV_HDV_VID_ERRORS + 3
#define INV_HDV_VID_FRAMERATE          INV_HDV_VID_ERRORS + 4
#define INV_HDV_VID_SEQ_HDR            INV_HDV_VID_ERRORS + 5
#define INV_HDV_VID_ARATIO             INV_HDV_VID_ERRORS + 6
#define INV_HDV_VID_DEF_HSIZE          INV_HDV_VID_ERRORS + 7
#define INV_HDV_VID_PROGSEQ            INV_HDV_VID_ERRORS + 8
#define INV_HDV_VID_VBR                INV_HDV_VID_ERRORS + 9
#define INV_HDV_VID_BITRATE            INV_HDV_VID_ERRORS + 10
#define INV_HDV_VID_CP                 INV_HDV_VID_ERRORS + 11
#define INV_HDV_VID_TC                 INV_HDV_VID_ERRORS + 12
#define INV_HDV_VID_MC                 INV_HDV_VID_ERRORS + 13
#define INV_HDV_VID_SEC                INV_HDV_VID_ERRORS + 14

#define INV_HDV_AUD_ERRORS             0x00016100
#define	INV_HDV_AUD_LAYER              INV_HDV_AUD_ERRORS + 0
#define INV_HDV_AUD_EPROTECTION        INV_HDV_AUD_ERRORS + 1
#define INV_HDV_AUD_EMPHASIS           INV_HDV_AUD_ERRORS + 2
#define INV_HDV_AUD_SAMPLE_RATE        INV_HDV_AUD_ERRORS + 3
#define INV_HDV_AUD_BITRATE            INV_HDV_AUD_ERRORS + 4
#define INV_HDV_PCM_AUDIO              INV_HDV_AUD_ERRORS + 5
#define INV_HDV_AUD_TYPE							 INV_HDV_AUD_ERRORS + 6

#define INV_HDV_MUX_ERRORS             0x00016200
#define INV_HDV_MUX_NOHDV_MUX          INV_HDV_MUX_ERRORS + 0
#define INV_HDV_MUX_HD1_MUX_RATE       INV_HDV_MUX_ERRORS + 1
#define INV_HDV_MUX_HD2_MUX_RATE       INV_HDV_MUX_ERRORS + 2


// defines for CableLabs conformance check errors
#define INV_CLABS_VID_ERRORS           0x00017000
#define INV_CLABS_VID_PROFILE          INV_CLABS_VID_ERRORS + 0
#define INV_CLABS_VID_LEVEL            INV_CLABS_VID_ERRORS + 1
#define INV_CLABS_VID_ARATIO           INV_CLABS_VID_ERRORS + 2
#define INV_CLABS_VID_FRAMERATE        INV_CLABS_VID_ERRORS + 3
#define INV_CLABS_VID_MPEGN            INV_CLABS_VID_ERRORS + 4
#define INV_CLABS_VID_MPEGM            INV_CLABS_VID_ERRORS + 5
#define INV_CLABS_VID_VBR              INV_CLABS_VID_ERRORS + 6
#define INV_CLABS_VID_BITRATE          INV_CLABS_VID_ERRORS + 7
#define INV_CLABS_VID_SEQ_HDR          INV_CLABS_VID_ERRORS + 8
#define INV_CLABS_VID_PIC_STRUCT       INV_CLABS_VID_ERRORS + 9
#define INV_CLABS_VID_DEF_HSIZE        INV_CLABS_VID_ERRORS + 10
#define INV_CLABS_VID_DEF_VSIZE        INV_CLABS_VID_ERRORS + 11

#define INV_CLABS_AUD_ERRORS           0x00017100
#define INV_CLABS_AUD_SAMPLE_RATE      INV_CLABS_AUD_ERRORS + 0
#define INV_CLABS_AUD_CMODE            INV_CLABS_AUD_ERRORS + 1
#define INV_CLABS_AUD_BITRATE          INV_CLABS_AUD_ERRORS + 2

#define INV_CLABS_MUX_ERRORS           0x00017200
#define INV_CLABS_MUX_RATE             INV_CLABS_MUX_ERRORS + 0

// defines for XDCAM HD conformance check errors
#define INV_XDCAM_VID_ERRORS           0x00018000
#define INV_XDCAM_VID_PROFILE          INV_XDCAM_VID_ERRORS + 0
#define INV_XDCAM_VID_LEVEL            INV_XDCAM_VID_ERRORS + 1
#define INV_XDCAM_VID_VBV              INV_XDCAM_VID_ERRORS + 2
#define INV_XDCAM_VID_HORZ             INV_XDCAM_VID_ERRORS + 3
#define INV_XDCAM_VID_DISPHORZ         INV_XDCAM_VID_ERRORS + 4
#define INV_XDCAM_VID_FRAMERATE        INV_XDCAM_VID_ERRORS + 5
#define INV_XDCAM_VID_1080VERT         INV_XDCAM_VID_ERRORS + 6
#define INV_XDCAM_VID_1080DISPVERT     INV_XDCAM_VID_ERRORS + 7
#define INV_XDCAM_VID_540VERT          INV_XDCAM_VID_ERRORS + 8
#define INV_XDCAM_VID_540DISPVERT      INV_XDCAM_VID_ERRORS + 9
#define INV_XDCAM_VID_GOP              INV_XDCAM_VID_ERRORS + 10
#define INV_XDCAM_VID_PROG             INV_XDCAM_VID_ERRORS + 11
#define INV_XDCAM_VID_TOPFIRST         INV_XDCAM_VID_ERRORS + 12
#define INV_XDCAM_VID_CHROMA           INV_XDCAM_VID_ERRORS + 13
#define INV_XDCAM_VID_CBR              INV_XDCAM_VID_ERRORS + 14
#define INV_XDCAM_VID_VBR              INV_XDCAM_VID_ERRORS + 15
#define INV_XDCAM_VID_BITRATE          INV_XDCAM_VID_ERRORS + 16

// defines for D10 and XDCAM IMX conformance check errors
#define INV_D10_VID_ERRORS             0x00018500
#define INV_D10_VID_PROFILE            INV_D10_VID_ERRORS + 0
#define INV_D10_VID_LEVEL              INV_D10_VID_ERRORS + 1
#define INV_D10_VID_FRAMERATE          INV_D10_VID_ERRORS + 2
#define INV_D10_VID_GOP                INV_D10_VID_ERRORS + 3
#define INV_D10_VID_CHROMA             INV_D10_VID_ERRORS + 4
#define INV_D10_VID_CBR                INV_D10_VID_ERRORS + 5
#define INV_D10_VID_BITRATE            INV_D10_VID_ERRORS + 6
#define INV_D10_VID_DCPREC             INV_D10_VID_ERRORS + 7
#define INV_D10_VID_PROGSEQ            INV_D10_VID_ERRORS + 8
#define INV_D10_VID_PROGFRAME          INV_D10_VID_ERRORS + 9
#define INV_D10_VID_TOPFIRST           INV_D10_VID_ERRORS + 10
#define INV_D10_VID_REPEATFIRST        INV_D10_VID_ERRORS + 11
#define INV_D10_VID_FRAMEPRED          INV_D10_VID_ERRORS + 12
#define INV_D10_VID_QSCALE             INV_D10_VID_ERRORS + 13
#define INV_D10_VID_INTRAVLC           INV_D10_VID_ERRORS + 14
#define INV_D10_VID_ALTSCAN            INV_D10_VID_ERRORS + 15
#define INV_D10_VID_HORZ               INV_D10_VID_ERRORS + 16
#define INV_D10_VID_NTSCVERT           INV_D10_VID_ERRORS + 17
#define INV_D10_VID_PALVERT            INV_D10_VID_ERRORS + 18


// defines for HD_DVD conformance check errors
#define INV_HD_DVD_VID_ERRORS          0x00019000

// defines for BD_HDMV conformance check errors
#define INV_BD_HDMV_VID_ERRORS         0x00019500

// defines for MPEG conformance check errors
#define INV_MPEG_ERRORS                0x00020000

// defines for MPEG video conformance check errors
#define INV_VID_ERRORS                 INV_MPEG_ERRORS
#define INV_VID_DEF_HORZ_MPEG1         INV_MPEG_ERRORS + 0
#define INV_VID_DEF_VERT_MPEG1         INV_MPEG_ERRORS + 1
#define INV_VID_DEF_HORZ_MPEG2         INV_MPEG_ERRORS + 2
#define INV_VID_DEF_VERT_MPEG2         INV_MPEG_ERRORS + 3
#define INV_VID_DEF_HORZ_LEVEL         INV_MPEG_ERRORS + 4
#define INV_VID_DEF_VERT_LEVEL         INV_MPEG_ERRORS + 5
#define INV_VID_DEF_HV_FR_LEVEL        INV_MPEG_ERRORS + 6
#define INV_VID_ASPECT_RATIO           INV_MPEG_ERRORS + 7
#define INV_VID_FRAME_RATE             INV_MPEG_ERRORS + 8
#define INV_VID_BITRATE                INV_MPEG_ERRORS + 9
#define INV_VID_MAX_BITRATE            INV_MPEG_ERRORS + 10
#define INV_VID_AVG_BITRATE            INV_MPEG_ERRORS + 11
#define INV_VID_MIN_BITRATE            INV_MPEG_ERRORS + 12
#define INV_VID_VBVBUFFER              INV_MPEG_ERRORS + 13
#define INV_VID_CHROMA                 INV_MPEG_ERRORS + 14
#define INV_VID_VIDEO_FORMAT           INV_MPEG_ERRORS + 15
#define INV_VID_COLOR_PRIM             INV_MPEG_ERRORS + 16
#define INV_VID_TRANS_CHAR             INV_MPEG_ERRORS + 17
#define INV_VID_MATRIX_COEFF           INV_MPEG_ERRORS + 18
#define INV_VID_DISP_HORZ              INV_MPEG_ERRORS + 19
#define INV_VID_DISP_VERT              INV_MPEG_ERRORS + 20
#define INV_VID_DC_PREC                INV_MPEG_ERRORS + 21
#define INV_VID_PULLDOWN               INV_MPEG_ERRORS + 22
#define INV_VID_FH_FCODE               INV_MPEG_ERRORS + 23
#define INV_VID_FV_FCODE               INV_MPEG_ERRORS + 24
#define INV_VID_SXF                    INV_MPEG_ERRORS + 25
#define INV_VID_SYF                    INV_MPEG_ERRORS + 26
#define INV_VID_BH_FCODE               INV_MPEG_ERRORS + 27
#define INV_VID_BV_FCODE               INV_MPEG_ERRORS + 28
#define INV_VID_SXB                    INV_MPEG_ERRORS + 29
#define INV_VID_SYB                    INV_MPEG_ERRORS + 30
#define INV_VID_PROFILE                INV_MPEG_ERRORS + 31
#define INV_VID_LEVEL                  INV_MPEG_ERRORS + 32
#define INV_VID_PROFILE_LEVEL          INV_MPEG_ERRORS + 33
#define INV_VID_PROFILE_M              INV_MPEG_ERRORS + 34
#define INV_VID_PROFILE_CHROMA         INV_MPEG_ERRORS + 35
#define INV_VID_CHROMA_PROFILE         INV_MPEG_ERRORS + 36
#define INV_VID_PROFILE_FRATE_RFIRST   INV_MPEG_ERRORS + 37
#define INV_VID_PROF_FRATE_PSEQ_RFIRST INV_MPEG_ERRORS + 38
#define INV_VID_PROFILE_DC_PREC        INV_MPEG_ERRORS + 39
#define INV_VID_LEVEL_FRATE            INV_MPEG_ERRORS + 40
#define INV_VID_DEF_HORZ_PROFILE_LEVEL INV_MPEG_ERRORS + 41
#define INV_VID_DEF_VERT_PROFILE_LEVEL INV_MPEG_ERRORS + 42
#define INV_VID_DEF_HV_FRATE           INV_MPEG_ERRORS + 43
#define INV_VID_TYPE                   INV_MPEG_ERRORS + 44
#define INV_VID_MPEGN                  INV_MPEG_ERRORS + 45
#define INV_VID_MPEGM                  INV_MPEG_ERRORS + 46
#define INV_VID_MPEGN_M                INV_MPEG_ERRORS + 47
#define INV_VID_CONSTRPARMS            INV_MPEG_ERRORS + 48
#define INV_VID_PROG_SEQ_MPEG1         INV_MPEG_ERRORS + 49
#define INV_VID_CHROMA_MPEG1           INV_MPEG_ERRORS + 50
#define INV_VID_DC_PREC_MPEG1          INV_MPEG_ERRORS + 51
#define INV_VID_PULLDOWN_MPEG1         INV_MPEG_ERRORS + 52
#define INV_VID_QSCALE_MPEG1           INV_MPEG_ERRORS + 53
#define INV_VID_IVLC_MPEG1             INV_MPEG_ERRORS + 54
#define INV_VID_ALTSCAN_MPEG1          INV_MPEG_ERRORS + 55
#define INV_VID_CONSTPARMS_MPEG2       INV_MPEG_ERRORS + 56
#define INV_VID_PROG_SEQ_FRAME         INV_MPEG_ERRORS + 57
#define INV_VID_PFRAME_FIELDPIC        INV_MPEG_ERRORS + 58
#define INV_VID_PFRAME_RFIRST          INV_MPEG_ERRORS + 59
#define INV_VID_PFRAME_FRAME_PRED      INV_MPEG_ERRORS + 60 
#define INV_VID_PSEQ_RFIRST_TFIRST     INV_MPEG_ERRORS + 61
#define INV_VID_CBRVBR_FIXED_VBV       INV_MPEG_ERRORS + 62
#define INV_VID_FH_FCODE_SXF           INV_MPEG_ERRORS + 63
#define INV_VID_FV_FCODE_SYF           INV_MPEG_ERRORS + 64
#define INV_VID_BH_FCODE_SXB           INV_MPEG_ERRORS + 65
#define INV_VID_BV_FCODE_SYB           INV_MPEG_ERRORS + 66

// defines for MPEG audio conformance check errors
#define INV_AUD_ERRORS                 0x00040000
#define INV_AUD_MODE                   INV_AUD_ERRORS + 0
#define INV_AUD_LAYER                  INV_AUD_ERRORS + 1 
#define INV_AUD_PSYCH                  INV_AUD_ERRORS + 2 
#define INV_AUD_BITRATE                INV_AUD_ERRORS + 3 
#define INV_AUD_EMPHASIS               INV_AUD_ERRORS + 4
#define INV_AUD_PRIVATEBIT             INV_AUD_ERRORS + 5
#define INV_AUD_ERROR_PROT             INV_AUD_ERRORS + 6
#define INV_AUD_COPYRIGHT              INV_AUD_ERRORS + 7
#define INV_AUD_ORIGINAL               INV_AUD_ERRORS + 8
#define INV_AUD_SAMPLE_RATE            INV_AUD_ERRORS + 9

// defines for MPEG muxer conformance check errors
#define INV_MUX_ERRORS                 0x00080000
#define INV_MUX_TYPE                   INV_MUX_ERRORS + 0
#define INV_MUX_PULLDOWN               INV_MUX_ERRORS + 1
#define INV_MUX_PULLDOWN_MPEG1         INV_MUX_ERRORS + 2
#define INV_MUX_COMPUTED_BITRATE       INV_MUX_ERRORS + 3 
#define INV_MUX_VCD_PAD                INV_MUX_ERRORS + 4
#define INV_MUX_VCD_PAD_NOVCD          INV_MUX_ERRORS + 5
#define INV_MUX_ALIGN_SH               INV_MUX_ERRORS + 6
#define INV_MUX_RATE                   INV_MUX_ERRORS + 7
#define INV_MUX_SECTOR_SIZE            INV_MUX_ERRORS + 8
#define INV_MUX_PACKETS_PER_PACK       INV_MUX_ERRORS + 9
#define INV_MUX_AVP_DELAY              INV_MUX_ERRORS + 10
#define INV_MUX_AUDIO1_BUFFER_SIZE     INV_MUX_ERRORS + 11
#define INV_MUX_AUDIO2_BUFFER_SIZE     INV_MUX_ERRORS + 12
#define INV_MUX_VIDEO_BUFFER_SIZE      INV_MUX_ERRORS + 13
#define INV_MUX_VBR                    INV_MUX_ERRORS + 14
#define INV_MUX_PEC                    INV_MUX_ERRORS + 15
#define INV_MUX_SCAN_OFFSETS           INV_MUX_ERRORS + 16
#define INV_MUX_SCAN_ALIGN             INV_MUX_ERRORS + 17
#define INV_MUX_PRIVATE2               INV_MUX_ERRORS + 18
#define INV_MUX_PRIVATE2_NODVD         INV_MUX_ERRORS + 19
#define INV_MUX_FRAME_TS               INV_MUX_ERRORS + 20
#define INV_MUX_MAX_FILE_SIZE          INV_MUX_ERRORS + 21
#define INV_MUX_START_TIME             INV_MUX_ERRORS + 22
#define INV_MUX_RESET_CLOCKS           INV_MUX_ERRORS + 23
#define INV_MUX_END_CODES              INV_MUX_ERRORS + 24
#define INV_MUX_BROKEN_LINK            INV_MUX_ERRORS + 25
#define INV_MUX_MPEG1_VID_MPEG2        INV_MUX_ERRORS + 26
#define INV_MPEG_LOW_MUX_RATE          INV_MUX_ERRORS + 27
#define INV_MUX_VBR_TS                 INV_MUX_ERRORS + 28

// defines for PCM audio conformance check errors
#define INV_PCM_ERRORS                 0x00100000
#define INV_PCM_SAMPLE_RATE            INV_PCM_ERRORS + 0
#define INV_PCM_CHANNELS               INV_PCM_ERRORS + 1
#define INV_PCM_MUTE_FLAG              INV_PCM_ERRORS + 2
#define INV_PCM_QUANTIZATION           INV_PCM_ERRORS + 3
#define INV_PCM_EMPHASIS               INV_PCM_ERRORS + 4

// defines for AC3 audio conformance check errors
#define INV_AC3_ERRORS                 0x00120000
#define INV_AC3_SAMPLE_RATE            INV_AC3_ERRORS + 0
#define INV_AC3_AUDIO_TYPE             INV_AC3_ERRORS + 1
#define INV_AC3_AUDIO_CHANNELS         INV_AC3_ERRORS + 2
#define INV_AC3_AUDIO_BITRATE          INV_AC3_ERRORS + 3
#define INV_AC3_AUDIO_CMODE            INV_AC3_ERRORS + 4
#define INV_AC3_AUDIO_COMP             INV_AC3_ERRORS + 5
#define INV_AC3_AUDIO_SECCOMP          INV_AC3_ERRORS + 6
#define INV_AC3_NOT_INSTALLED          INV_AC3_ERRORS + 7
#define INV_AC3_AUDIO_LFEENA           INV_AC3_ERRORS + 8
#define INV_AC3_AUDIO_LFEFILT          INV_AC3_ERRORS + 9
#define INV_AC3_AUDIO_DELAYSUR         INV_AC3_ERRORS + 10
#define INV_AC3_AUDIO_INCHAN           INV_AC3_ERRORS + 11
#define INV_AC3_AUDIO_FL               INV_AC3_ERRORS + 12
#define INV_AC3_AUDIO_FR               INV_AC3_ERRORS + 13
#define INV_AC3_AUDIO_CENT             INV_AC3_ERRORS + 14
#define INV_AC3_AUDIO_LS               INV_AC3_ERRORS + 15
#define INV_AC3_AUDIO_RS               INV_AC3_ERRORS + 16
#define INV_AC3_AUDIO_LFE              INV_AC3_ERRORS + 17


//-- AAC audio defines ----------------------------------------------

// defines for AAC audio conformance check errors
#define INV_AAC_ERRORS					0x00200000
#define INV_AAC_CHANNELS				INV_AAC_ERRORS +  0
#define INV_AAC_OBJECT_TYPE				INV_AAC_ERRORS +  1
#define INV_AAC_BITRATE					INV_AAC_ERRORS +  2
#define INV_AAC_MPEG_VERSION			INV_AAC_ERRORS +  3
#define INV_AAC_SAMPLE_RATE 			INV_AAC_ERRORS +  4
#define INV_AAC_HEADER_TYPE 			INV_AAC_ERRORS +  5
#define INV_AAC_HF_CUTOFF 				INV_AAC_ERRORS +  6
#define INV_AAC_VBR 					INV_AAC_ERRORS +  7
#define INV_AAC_HE_SIGNALLING			INV_AAC_ERRORS +  8
#define INV_AAC_HE_VS_SAMPLERATE		INV_AAC_ERRORS +  9
#define INV_AAC_HE_VS_VBR				INV_AAC_ERRORS + 10
#define INV_AAC_HE_V2_CHANNELS			INV_AAC_ERRORS + 11
#define INV_AAC_HE_NO_PROFILE			INV_AAC_ERRORS + 12
#define INV_AAC_BITS_PER_SAMPLE			INV_AAC_ERRORS + 13


// defines for AAC bitrates:
#define AAC_AUDIOBITRATE_006  1
#define AAC_AUDIOBITRATE_007  2
#define AAC_AUDIOBITRATE_008  3
#define AAC_AUDIOBITRATE_010  4
#define AAC_AUDIOBITRATE_012  5
#define AAC_AUDIOBITRATE_014  6
#define AAC_AUDIOBITRATE_016  7
#define AAC_AUDIOBITRATE_020  8
#define AAC_AUDIOBITRATE_024  9
#define AAC_AUDIOBITRATE_028 10
#define AAC_AUDIOBITRATE_032 11
#define AAC_AUDIOBITRATE_040 12
#define AAC_AUDIOBITRATE_048 13
#define AAC_AUDIOBITRATE_056 14
#define AAC_AUDIOBITRATE_064 15
#define AAC_AUDIOBITRATE_080 16
#define AAC_AUDIOBITRATE_096 17
#define AAC_AUDIOBITRATE_112 18
#define AAC_AUDIOBITRATE_128 19
#define AAC_AUDIOBITRATE_160 20
#define AAC_AUDIOBITRATE_192 21
#define AAC_AUDIOBITRATE_224 22
#define AAC_AUDIOBITRATE_256 23
#define AAC_AUDIOBITRATE_320 24
#define AAC_AUDIOBITRATE_384 25
#define AAC_AUDIOBITRATE_448 26
#define AAC_AUDIOBITRATE_512 27
#define AAC_AUDIOBITRATE_640 28
#define AAC_AUDIOBITRATE_768 29
#define AAC_AUDIOBITRATE_896 30
#define AAC_AUDIOBITRATE_1024 31

// defines for number of channels and speaker possition: 
#define AAC_CHANNELS_1_MONO								1 // front center speaker
#define AAC_CHANNELS_2_STEREO							2 // front left-right
#define AAC_CHANNELS_3_FL_FR_FC						3 // front left-right-center
#define AAC_CHANNELS_4_FL_FR_FC_BC				4 // front left-right-center, back center
#define AAC_CHANNELS_5_FL_FR_FC_BL_BR			5 // front left-right-center, back left-right
#define AAC_CHANNELS_6_FL_FR_FC_BL_BR_LF	6 // front left-right-center, back left-right, subwoofer

// for backward compatibility reasons:
#define AAC_CHANNELS_MONO		AAC_CHANNELS_1_MONO
#define AAC_CHANNELS_STEREO AAC_CHANNELS_2_STEREO

// bits per PCM sample at input:
#define AAC_BITS_PER_SAMPLE_08  8
#define AAC_BITS_PER_SAMPLE_16 16
#define AAC_BITS_PER_SAMPLE_24 24


// defines for audio object type (mpeg-4 version)
#define AAC_LC 2
// #define AAC_HE 5

// defines for header type
#define AAC_HEADER_RAW  0
#define AAC_HEADER_ADTS 1
#define AAC_HEADER_LATMLOAS 2


// defines for cut-off frequency (can also be a number between 1000 and 44100, Hz)
#define AAC_CUTOFF_DEFAULT 0
#define AAC_CUTOFF_NOTUSED 1  

// defines for variable bitrate mode:
#define AAC_VBR_NOTUSED 0
#define AAC_VBR_LOW1		1 // low quality
#define AAC_VBR_LOW2		2
#define AAC_VBR_LOW3		3
#define AAC_VBR_MEDIUM1 4
#define AAC_VBR_MEDIUM2 5 
#define AAC_VBR_MEDIUM3 6
#define AAC_VBR_HIGH1		7
#define AAC_VBR_HIGH2		8
#define AAC_VBR_HIGH3		9 // high quality

// defines for HE signaling:
#define AAC_HE_NOTUSED											0 
#define AAC_HE_IMPLICIT											1 // HE v1! 
#define AAC_HE_IMPLICIT_WITH_PS							2 // HE v2! 
// #define AAC_HE_EXPLICIT_HIERARCHICAL
// #define AAC_HE_EXPLICIT_BACKWARD_COMPATIBLE

#define AAC_PROTECT_ADTS_STREAM_NO 0 
#define AAC_PROTECT_ADTS_STREAM_YES 1 

// defines for aac output return codes
#define aacOutErrNone	0 		// no error
#define aacOutCancel	1		// user canceled settings
#define aacOutError		2     // undefined error
#define aacOutDecline 3     // unable to use input settings, resolution, audio etc.


//-- WMA audio defines ----------------------------------------------

// defines for WMA audio conformance check errors
#define INV_WMA_ERRORS					0x00400000
#define INV_WMA_NO_ERROR				INV_WMA_ERRORS +  0
#define INV_WMA_CHANNELS				INV_WMA_ERRORS +  1
#define INV_WMA_SAMPLE_RATE				INV_WMA_ERRORS +  2
#define INV_WMA_BITRATE					INV_WMA_ERRORS +  3

// defines for WMA bitrates:

#define WMA_AUDIOBITRATE_006  1
#define WMA_AUDIOBITRATE_007  2
#define WMA_AUDIOBITRATE_008  3
#define WMA_AUDIOBITRATE_010  4
#define WMA_AUDIOBITRATE_012  5
#define WMA_AUDIOBITRATE_014  6
#define WMA_AUDIOBITRATE_016  7
#define WMA_AUDIOBITRATE_020  8 
#define WMA_AUDIOBITRATE_024  9
#define WMA_AUDIOBITRATE_028 10
#define WMA_AUDIOBITRATE_032 11
#define WMA_AUDIOBITRATE_040 12
#define WMA_AUDIOBITRATE_048 13
#define WMA_AUDIOBITRATE_056 14
#define WMA_AUDIOBITRATE_064 15
#define WMA_AUDIOBITRATE_080 16
#define WMA_AUDIOBITRATE_096 17
#define WMA_AUDIOBITRATE_112 18
#define WMA_AUDIOBITRATE_128 19
#define WMA_AUDIOBITRATE_160 20
#define WMA_AUDIOBITRATE_192 21
#define WMA_AUDIOBITRATE_224 22
#define WMA_AUDIOBITRATE_256 23
#define WMA_AUDIOBITRATE_320 24
#define WMA_AUDIOBITRATE_384 25
#define WMA_AUDIOBITRATE_448 26
#define WMA_AUDIOBITRATE_512 27
#define WMA_AUDIOBITRATE_640 28
#define WMA_AUDIOBITRATE_768 29
#define WMA_AUDIOBITRATE_896 30
#define WMA_AUDIOBITRATE_1024 31

// defines for number of channels and speaker possition: 

#define WMA_CHANNELS_1_MONO					1 // front center speaker
#define WMA_CHANNELS_2_STEREO				2 // front left-right
#define WMA_CHANNELS_3_FL_FR_FC				3 // front left-right-center
#define WMA_CHANNELS_4_FL_FR_FC_BC			4 // front left-right-center, back center
#define WMA_CHANNELS_5_FL_FR_FC_BL_BR		5 // front left-right-center, back left-right
#define WMA_CHANNELS_6_FL_FR_FC_BL_BR_LF	6 // front left-right-center, back left-right, subwoofer

// for backward compatibility reasons:

#define WMA_CHANNELS_MONO		WMA_CHANNELS_1_MONO
#define WMA_CHANNELS_STEREO		WMA_CHANNELS_2_STEREO

// defines for aac output return codes

#define wmaOutErrNone	0 	// no error
#define wmaOutCancel	1	// user canceled settings
#define wmaOutError		2   // undefined error
#define wmaOutDecline	3   // unable to use input settings, resolution, audio etc.


#endif /* _MPEGDEF_H_ */
