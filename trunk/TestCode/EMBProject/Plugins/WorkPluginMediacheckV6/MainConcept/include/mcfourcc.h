/* ----------------------------------------------------------------------------
 * File: mcfourcc.h
 *
 * Desc: fourcc defines
 *
 * Copyright (c) 2009, MainConcept GmbH. All rights reserved.
 *
 * This software is the confidential and proprietary information of
 * MainConcept GmbH and may be used only in accordance with the terms of
 * your license from MainConcept GmbH.
 * ----------------------------------------------------------------------------
 */

#if !defined (__MC_FOURCC_INCLUDED__)
#define __MC_FOURCC_INCLUDED__

#include "mctypes.h"

#ifndef MAKEFOURCC
  #define MAKEFOURCC(ch0, ch1, ch2, ch3)  \
    ((uint32_t)(int8_t)(ch0) | ((uint32_t)(int8_t)(ch1) << 8) | \
    ((uint32_t)(int8_t)(ch2) << 16) | ((uint32_t)(int8_t)(ch3) << 24 ))
#endif // MAKEFOURCC


//#ifndef FOURCC_RGB3
//#define FOURCC_RGB3 MAKEFOURCC('R', 'G', 'B', '3')
//#endif
#ifndef FOURCC_BGR3
#define FOURCC_BGR3 MAKEFOURCC('B', 'G', 'R', '3')
#endif
#ifndef FOURCC_BGRA
#define FOURCC_BGRA MAKEFOURCC('B', 'G', 'R', 'A')
#endif
#ifndef FOURCC_BGRA32F
#define FOURCC_BGRA32F MAKEFOURCC('B', '3', '2', 'F')
#endif
#ifndef FOURCC_ARGB
#define FOURCC_ARGB MAKEFOURCC('A', 'R', 'G', 'B') // ARGB Colorspace as common used on Macintosh. May not be available on other platforms
#endif
#ifndef FOURCC_BGR4
#define FOURCC_BGR4 MAKEFOURCC('B', 'G', 'R', '4')
#endif
#ifndef FOURCC_R555
#define FOURCC_R555 MAKEFOURCC('R', '5', '5', '5')
#endif
#ifndef FOURCC_R565
#define FOURCC_R565 MAKEFOURCC('R', '5', '6', '5')
#endif
#ifndef FOURCC_AYUV
#define FOURCC_AYUV MAKEFOURCC('A', 'Y', 'U', 'V')
#endif
#ifndef FOURCC_VUYA
#define FOURCC_VUYA MAKEFOURCC('V', 'U', 'Y', 'A')
#endif
#ifndef FOURCC_VUYA32F
#define FOURCC_VUYA32F MAKEFOURCC('Y', '3', '2', 'F')
#endif
#ifndef FOURCC_UYVY
#define FOURCC_UYVY MAKEFOURCC('U', 'Y', 'V', 'Y')
#endif
#ifndef FOURCC_YUY2
#define FOURCC_YUY2 MAKEFOURCC('Y', 'U', 'Y', '2')
#endif
#ifndef FOURCC_YUYV
#define FOURCC_YUYV MAKEFOURCC('Y', 'U', 'Y', 'V')
#endif
#ifndef FOURCC_EXTP
#define FOURCC_EXTP MAKEFOURCC('E', 'X', 'T', 'P') // external, 3-plane colorspace-converter (requires a valid ext_cc_struct)
#endif
#ifndef FOURCC_I420
#define FOURCC_I420 MAKEFOURCC('I', '4', '2', '0')
#endif
#ifndef FOURCC_I422
#define FOURCC_I422 MAKEFOURCC('I', '4', '2', '2')
#endif
#ifndef FOURCC_I444
#define FOURCC_I444 MAKEFOURCC('I', '4', '4', '4')
#endif
#ifndef FOURCC_YV12
#define FOURCC_YV12 MAKEFOURCC('Y', 'V', '1', '2')
#endif
#ifndef FOURCC_YV16
#define FOURCC_YV16 MAKEFOURCC('Y', 'V', '1', '6')
#endif
#ifndef FOURCC_Y41P
#define FOURCC_Y41P MAKEFOURCC('Y', '4', '1', 'P')
#endif
#ifndef FOURCC_RAW
#define FOURCC_RAW  MAKEFOURCC('R', 'A', 'W', ' ')
#endif

#ifndef FOURCC_R32C
#define FOURCC_R32C MAKEFOURCC('R', '3', '2', 'C') //16..235 (16:black, 235-white) RGBA
#endif
#ifndef FOURCC_argb
#define FOURCC_argb MAKEFOURCC('a', 'r', 'g', 'b') //16..235 (16:black, 235-white) argb
#endif
#ifndef FOURCC_R24C
#define FOURCC_R24C MAKEFOURCC('R', '2', '4', 'C') //16..235 (16:black, 235-white) RGB3
#endif
#ifndef FOURCC_YuYv
#define FOURCC_YuYv MAKEFOURCC('Y', 'u', 'Y', 'v') //No clamping to ITU601 range
#endif
#ifndef FOURCC_UyVy
#define FOURCC_UyVy MAKEFOURCC('U', 'y', 'V', 'y') //No clamping to ITU601 range
#endif

#ifndef FOURCC_XYZ12
#define FOURCC_XYZ12 MAKEFOURCC('X', 'Y', 'Z', 0x0C) //D-cinema xyz colorspace, 12bit
#endif
#ifndef FOURCC_DPX10
#define FOURCC_DPX10 MAKEFOURCC('D', 'P', 'X', 0x0A) //10bit DPX format, SMPTE 268M
#endif
#ifndef FOURCC_V210
#define FOURCC_V210  MAKEFOURCC('v', '2', '1', '0') //Quicktime v210 10bit format
#endif

#ifndef FOURCC_W009
#define FOURCC_W009 MAKEFOURCC('W', '0', '0', '9')	// like yv12 16-bit component, 9 - significant
#endif
#ifndef FOURCC_W010
#define FOURCC_W010 MAKEFOURCC('W', '0', '1', '0')
#endif
#ifndef FOURCC_W011
#define FOURCC_W011 MAKEFOURCC('W', '0', '1', '1')
#endif
#ifndef FOURCC_W012
#define FOURCC_W012 MAKEFOURCC('W', '0', '1', '2')
#endif
#ifndef FOURCC_W209
#define FOURCC_W209 MAKEFOURCC('W', '2', '0', '9') //  like yv16 16-bit component, 9 - significant
#endif
#ifndef FOURCC_W210
#define FOURCC_W210 MAKEFOURCC('W', '2', '1', '0')
#endif
#ifndef FOURCC_W211
#define FOURCC_W211 MAKEFOURCC('W', '2', '1', '1')
#endif
#ifndef FOURCC_W212
#define FOURCC_W212 MAKEFOURCC('W', '2', '1', '2')
#endif

#ifndef FOURCC_P010 
#define FOURCC_P010 MAKEFOURCC('P', '0', '1', '0')
#endif
#ifndef FOURCC_P016 
#define FOURCC_P016 MAKEFOURCC('P', '0', '1', '6')
#endif

#ifndef FOURCC_P210 
#define FOURCC_P210 MAKEFOURCC('P', '2', '1', '0')
#endif
#ifndef FOURCC_P216 
#define FOURCC_P216 MAKEFOURCC('P', '2', '1', '6')
#endif
#ifndef FOURCC_Y210 
#define FOURCC_Y210 MAKEFOURCC('Y', '2', '1', '0')
#endif
#ifndef FOURCC_Y216
#define FOURCC_Y216 MAKEFOURCC('Y', '2', '1', '6')
#endif

#ifndef FOURCC_H264 
#define FOURCC_H264 MAKEFOURCC('H', '2', '6', '4')
#endif

#endif // #if !defined (__MC_FOURCC_INCLUDED__)

