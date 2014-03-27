#ifndef _COMMON_H_
#define _COMMON_H_

#pragma once
#include <iostream>
#include "JS_FileDrive.h"

//#include "VLD.h"
//发给上层的解码消息
#define WM_DS_EC_COMPLETE                  WM_USER+1
#define WM_DS_EC_PAUSED                    WM_USER+2
#define WM_DS_EC_SHUTTING_DOWN             WM_USER+3
#define WM_DS_EC_STOP                      WM_USER+4
#define WM_DS_EC_REPAINT                   WM_USER+5
#define WM_DS_EC_EC_SNDDEV_IN_ERROR        WM_USER+6
#define WM_DS_EC_EC_SNDDEV_OUT_ERROR       WM_USER+7
#define WM_DS_EC_STREAM_ERROR_STILLPLAYING WM_USER+8
#define WM_DS_EC_STREAM_ERROR_STOPPED      WM_USER+9
#define WM_DS_EC_USERABORT                 WM_USER+10

#define WM_DS_EC_USE_CLEANUP               WM_USER+50
#define WM_DS_EC_USE_UNRENDER              WM_USER+51
//发给上层的分析文件消息
#define WM_IC_UNRECOG                      WM_USER+100 //文件不能分析

//解码自定义消息
#define EC_USE_CLEANUP                     EC_USER+1

struct  _JS_Fix JS_VideoInfo
{
	std::string strVideoCodecFormat;
	std::string strColorSpace;
	std::string strAspectRatio;
	int         iVideoHeight;
	int         iVideowidth;
	
	int         iFrameRate;
	int         iMaxGOP;
	long        lVideoBitRate;
	long        lvbvBufferSiae;
	std::string strAFDInfo;
} ;
struct _JS_Fix JS_AudioInfo
{
	std::string strAudioCodecFormat;
	int         iAudioSamplingRate;
	long        lAudioQuantizationBits;
	long        lChannels;
	
} ;
struct _JS_Fix JS_StreamInfo
{
	LONGLONG       llFileDuration;
	std::string    strOP;
	JS_VideoInfo   VideoInfo;
	JS_AudioInfo   AudioInfo;

};

#endif