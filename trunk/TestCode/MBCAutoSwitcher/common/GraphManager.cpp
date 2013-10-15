// Encoder.cpp: implementation of the CEncoder class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GraphManager.h"
#include <InitGuid.h>
#include <ModuleConfig/ModuleConfig.h>
#include <ModuleConfig/common_mc.h>
#include <ModuleConfig/mpeg2enc_mc.h>
#include <ModuleConfig/mpeg2dec_mc.h>
#include <ModuleConfig/mpeg2mux_mc.h>
#include <ModuleConfig/eaacenc.h>
#include <ModuleConfig/sf_mc.h>
#include <clsid/sf_guid.h>
#include <clsid/mpeg2enc_guid.h>
#include <clsid/mpeg2dec_guid.h>
#include <clsid/mpeg2demux_guid.h>
#include <clsid/mpeg2mux_guid.h>
#include <clsid/l2aenc_guid.h>
#include <streams.h>



// Filter nicknames for graph viewer
#if		(defined USE_H264_ENCODER)
		#define	VIDEO_ENCODER_FILTER	_T("H.264 Video Encoder")
#elif	(defined USE_MPEG4_ENCODER)
		#define	VIDEO_ENCODER_FILTER	_T("MPEG-4 Video Encoder")
#else
		#define	VIDEO_ENCODER_FILTER	_T("MPEG-2 Video Encoder")
#endif

#define	VIDEO_SOURCE_FILTER		_T("Video Source")
#define	AUDIO_SOURCE_FILTER		_T("Audio Source")
#define	AUDIO_ENCODER_FILTER	_T("AAC Encoder")
#define	MUXER_FILTER			_T("Multiplexer")
#define	WAVDEST_FILTER			_T("WavDest Filter")
#define	DUMP_FILTER				_T("Sink Filter")
#define	IMAGE_SCALER			_T("Image Scaler")
#define	MPEG2_VIDEO_DECODER		_T("MPEG2 Video Decoder")
#define	MPEG4_VIDEO_DECODER		_T("MPEG4 Video Decoder")
#define DEMUXER_FILTER			_T("Mpeg Demultiplexer")
#define AUDIO_DECODER_FILTER	_T("Audio Decoder")
#define SMART_TEE_FILTER        _T("Smart Tee Filter")
#define SMART_TEE_FILTER_AUDIO  _T("Smart Tee Filter Audio")
#define FILEWRITE_AUDIO         _T("File Write Audio")

//#define DECKLINK_MONITER_CAPTURE       
//#define DECKLINK_MONITER_PLAYBACK
#define DECKLINK_MONITER_CAPTURE_AUDIO       
//#define DECKLINK_MONITER_PLAYBACK_AUDIO


DEFINE_GUID(CLSID_DEMULTIPLEXER,
			0x136DCBF5, 0x3874, 0x4B70, 0xAE, 0x3E, 0x15, 0x99, 0x7D, 0x63, 0x34, 0xF7);
#define MPEG_DEMULTIPLEXER        _T("Mpeg Demultiplexerer")


DEFINE_GUID(CLSID_DECKLINKVIDEORENDER, 
			0xCEB13CC8, 0x3591, 0x45A5, 0xBA, 0x0F, 0x20, 0xE9, 0xA1, 0xD7, 0x2F, 0x76);
#define DECKLINK_VIDEO_RENDER        _T("Decklink Video Render")


DEFINE_GUID(CLSID_DECKLINKAUDIORENDER, 
			0x19FA8CC3, 0x56CE, 0x46AB, 0x82, 0x5D, 0x5C, 0xE1, 0xA3, 0x9B, 0x13, 0x7A);
#define DECKLINK_AUDIO_RENDER        _T("Decklink Audio Render")


#define DECKLINK_AUDIO_DEVICE       _T("DeckLink Audio Device")

DEFINE_GUID(CLSID_DECKLINKCAPTURE, 
			0x44A8B5C7, 0x13B6, 0x4211, 0xBD, 0x40, 0x35, 0xB6, 0x29, 0xD9, 0xE6, 0xDF);
#define DECKLINK_CAPTURE        _T("Decklink Video Capture")

DEFINE_GUID(CLSID_DECKLINKCAPTURE_AUDIO, 
			0xAAA22F7E, 0x5AA0, 0x49D9, 0x8C, 0x8D, 0xB5, 0x2B, 0x1A, 0xA9, 0x2E, 0xB7);
#define DECKLINK_CAPTURE_AUDIO  _T("Decklink Audio Capture")

DEFINE_GUID(CLSID_SMARTTEE, 
			0xCC58E280, 0x8AA1, 0x11D1, 0xB3, 0xF1, 0x00, 0xAA, 0x00, 0x37, 0x61, 0xC5);

DEFINE_GUID(CLSID_FounderTSPCRAdjust, 
			0x32feab30, 0xc6d7, 0x48fe, 0x8a, 0xf3, 0xad, 0xb0, 0x75, 0x0b, 0x9d, 0x84);

DEFINE_GUID(CLSID_WavDest,
			0x3c78b8e2, 0x6c4d, 0x11d1, 0xad, 0xe2, 0x00, 0x00, 0xf8, 0x75, 0x4b, 0x99);

DEFINE_GUID(CLSID_H264VideoEncoder,
			0xF9D05B58, 0xA2D6, 0x431A, 0x92, 0x52, 0x1F, 0x05, 0xEA, 0x2F, 0x33, 0x64);

DEFINE_GUID(CLSID_MCM2Enc,
			0x15BEBB32, 0x5BB5, 0x42B6, 0xB4, 0x5A, 0xBA, 0x49, 0xF7, 0x8B, 0xA1, 0x9F);


DEFINE_GUID(IID_IDecklinkRawDeviceControl,
			0x72D62DE6, 0x010F, 0x48e6, 0xA2, 0x51, 0x78, 0xCA, 0x28, 0x5B, 0xDF, 0xE0);
// {e9441935-d6b4-48de-98f8-4659e478f090} 
DEFINE_GUID(IID_IDecklinkIOControl, 
			0x60F58A81, 0xA387, 0x4922, 0xAA, 0xAC, 0x99, 0x8B, 0xD9, 0xFB, 0xE1, 0xAA);//IOControl


#define VIDEO_ENCODER_CLSID CLSID_EM2VE

DEFINE_GUID(VIDEO_DECODER_CLSID,
			0xBC4EB321, 0x771F, 0x4E9F, 0xAF, 0x67, 0x37, 0xC6, 0x31, 0xEC, 0xA1, 0x06);

DEFINE_GUID(CLSID_MXFParser,				0x025BE2E4, 0x1787, 0x4DA4, 0xA5, 0x85, 0xC5, 0xB2, 0xB9, 0xEE, 0xB5, 0x7C);
DEFINE_GUID(CLSID_LXFParser,				0x6B4FABAA, 0x47CC, 0x41A6, 0xAA, 0x3B, 0xD8, 0xF1, 0xC3, 0xB4, 0x55, 0xD7);
DEFINE_GUID(CLSID_MCMpgDmx,					0x136DCBF5, 0x3874, 0x4B70, 0xAE, 0x3E, 0x15, 0x99, 0x7D, 0x63, 0x34, 0xF7);
//DEFINE_GUID(IID_IModuleConfig,				0x486F726E, 0x4D43, 0x49b9, 0x8A, 0x0C, 0xC2, 0x2A, 0x2B, 0x05, 0x24, 0xE8);
DEFINE_GUID(CLSID_MainConcept_MPEG_Decoder, 0xBC4EB321, 0x771F, 0x4E9F, 0xAF, 0x67, 0x37, 0xC6, 0x31, 0xEC, 0xA1, 0x06);
DEFINE_GUID(CLSID_MainConcept_MP4_Decoder,	0xFC86AD6C, 0x894A, 0x44E9, 0xA2, 0x83, 0x4B, 0x5A, 0x9D, 0xD6, 0xCA, 0x65);
DEFINE_GUID(CLSID_MainConcept_infTee,		0x08C0DE0E, 0xd048, 0x4d36, 0x84, 0x0e, 0x2b, 0x21, 0x35, 0xf7, 0x9a, 0x21);
DEFINE_GUID(CLSID_MainConcept_ColorSpace,   0x272D77A0, 0xA852, 0x4851, 0xAD, 0xA4, 0x90, 0x91, 0xFE, 0xAD, 0x4C, 0x86);
DEFINE_GUID(CLSID_Bluefish444_Video_Render, 0xD79F359D, 0x8eb2, 0x4d38, 0xa1, 0x2d, 0x07, 0x72, 0x20, 0xee, 0x36, 0x99);
DEFINE_GUID(CLSID_AviFileSource,			0xd3588ab0, 0x0781, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(CLSID_MatroxRiffReader, 		0x74A541E1, 0x34B0, 0x11CF, 0x82, 0xDA, 0x00, 0x20, 0xAF, 0x11, 0xC0, 0x10);
DEFINE_GUID(CLSID_FounderAVIDemultiplexer,  0x02b3384f, 0xfacf, 0x491e, 0x99, 0x19, 0xef, 0x22, 0x02, 0xcf, 0xbc, 0xcc);
DEFINE_GUID(CLSID_Bluefish444_Audio_Render,	0xF27A8BE3, 0x9177, 0x4F60, 0x95, 0x0A, 0x16, 0xC8, 0xCA, 0x42, 0x50, 0xE4);
DEFINE_GUID(CLSID_DecklinkVideoRender, 		0xCEB13CC8, 0x3591, 0x45A5, 0xBA, 0x0F, 0x20, 0xE9, 0xA1, 0xD7, 0x2F, 0x76);
DEFINE_GUID(CLSID_DecklinkAudioRender,		0x19FA8CC3, 0x56CE, 0x46AB, 0x82, 0x5D, 0x5C, 0xE1, 0xA3, 0x9B, 0x13, 0x7A);
DEFINE_GUID(CLSID_MCCSC,					0x272D77A0, 0xA852, 0x4851, 0xAD, 0xA4, 0x90, 0x91, 0xFE, 0xAD, 0x4C, 0x86);	
DEFINE_GUID(CLSID_WaveParser,				0xd51bd5a1, 0x7548, 0x11cf, 0xa5, 0x20, 0x00, 0x80, 0xc7, 0x7e, 0xf5, 0x8a);
DEFINE_GUID(CLSID_MCL2AD,					0x2F75E451, 0xA88C, 0x4939, 0xBF, 0xE5, 0xD9, 0x2D, 0x48, 0xC1, 0x02, 0xF2);
DEFINE_GUID(CLSID_LegacyAmFilterCategory,   0x083863F1, 0x70DE, 0x11d0, 0xBD, 0x40, 0x00, 0xA0, 0xC9, 0x11, 0xCE, 0x86);
DEFINE_GUID(CLSID_SystemDeviceEnum,			0x62BE5D10, 0x60EB, 0x11d0, 0xBD, 0x3B, 0x00, 0xA0, 0xC9, 0x11, 0xCE, 0x86);
DEFINE_GUID(CLSID_MXFMuxer,					0x24FF6532, 0x6BE7, 0x4151, 0xA5, 0x63, 0x4E, 0x57, 0xC1, 0x45, 0xF2, 0xB5);
	//0xF66305F6 ,0x0661, 0x4CB9, 0xBD, 0x22, 0x0D, 0x91, 0xBE, 0xC1, 0x4A, 0xBD);
DEFINE_GUID(CLSID_MP4Demux,					0xE7022EDA, 0xD953, 0x4018, 0xAE, 0x75, 0x89, 0x03, 0x6D, 0xEA, 0x0B, 0xB7);
DEFINE_GUID(CLSID_MP4Mux,					0xF5E52B44, 0xDFC9, 0x4c25, 0x8C, 0xB3, 0x5D, 0x5B, 0xB7, 0x51, 0x51, 0x72);
DEFINE_GUID(CLSID_MCMpgMux,                 0xCFD28198, 0x115C, 0x4606, 0xBD, 0x64, 0x27, 0xA7, 0xE0, 0x05, 0x1D, 0x2A);
DEFINE_GUID(CLSID_MCNetRender,              0x389BEC4F, 0x5829, 0x455E, 0x88, 0x21, 0xBE, 0x24, 0xD7, 0x97, 0x0B, 0xAD);
DEFINE_GUID(CLSID_TSSplitter,               0x8EAB4940, 0x09CA, 0x46AF, 0x9A, 0x9D, 0xD7, 0x3E, 0xE1, 0xA8, 0xD5, 0x69);
DEFINE_GUID(CLSID_MCNetRender1,             0x6E726F48, 0x7757, 0x412C, 0x8B, 0x34, 0x62, 0xA9, 0x8F, 0x1B, 0xD6, 0xA6);

//---------------------------------------------Founder Net Render----------------------------------------------------------
// {dc26c15a-ce7b-48df-8818-83e74e8135de} 
DEFINE_GUID(CLSID_FounderNetRendererFilter, 
			0xdc26c15a, 0xce7b, 0x48df, 0x88, 0x18, 0x83, 0xe7, 0x4e, 0x81, 0x35, 0xde);

// {64a178ff-3dc8-49d6-8111-952582fb31e4} 
DEFINE_GUID(CLSID_FounderNetRendererFilterProperty, 
			0x64a178ff, 0x3dc8, 0x49d6, 0x81, 0x11, 0x95, 0x25, 0x82, 0xfb, 0x31, 0xe4);

//---------------------------------------------New MCNetRender-------------------------------------------------------------

// {6E726F48-2C00-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_OriginNetworkType, 
			0x6E726F48, 0x2C00, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C01-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_OriginAddressType, 
			0x6E726F48, 0x2C01, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C02-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_ConnectionNetworkType, 
			0x6E726F48, 0x2C02, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C03-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_ConnectionAddressType, 
			0x6E726F48, 0x2C03, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C04-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_StreamingProtocol, 
			0x6E726F48, 0x2C04, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C05-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_StreamingType, 
			0x6E726F48, 0x2C05, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C06-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_StreamingPort, 
			0x6E726F48, 0x2C06, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C07-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_ConnectionTTL, 
			0x6E726F48, 0x2C07, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C08-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_BandWidthValue, 
			0x6E726F48, 0x2C08, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C09-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_OriginSessionID, 
			0x6E726F48, 0x2C09, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C0A-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_OutgoingNetworkInterface, 
			0x6E726F48, 0x2C0A, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C0B-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_DestinationAddress, 
			0x6E726F48, 0x2C0B, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C0C-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_EmailField, 
			0x6E726F48, 0x2C0C, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C0D-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_PhoneField, 
			0x6E726F48, 0x2C0D, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C0E-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_BandWidthModifier, 
			0x6E726F48, 0x2C0E, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C0F-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_SessionName, 
			0x6E726F48, 0x2C0F, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C10-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_OriginCreatorName, 
			0x6E726F48, 0x2C10, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C11-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_SessionInfo, 
			0x6E726F48, 0x2C11, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C12-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_SessionDescriptionURI, 
			0x6E726F48, 0x2C12, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C14-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_OriginSessionVersion,
			0x6E726F48, 0x2C14, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C15-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_MaxTransferUnit, 
			0x6E726F48, 0x2C15, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C16-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_SapMulticastGroup, 
			0x6E726F48, 0x2C16, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C18-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_RateControl, 
			0x6E726F48, 0x2C18, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C19-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_SdpDecoder, 
			0x6E726F48, 0x2C19, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C20-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_ServerWorkMode, 
			0x6E726F48, 0x2C20, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C21-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_NetworkPinProperties,
			0x6E726F48, 0x2C21, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C22-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_SdpFileName,
			0x6E726F48, 0x2C22, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C23-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_InterleavingConnectionSocket,
			0x6E726F48, 0x2C23, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C24-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_UseSAP,
			0x6E726F48, 0x2C24, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C25-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_UseRTCP,
			0x6E726F48, 0x2C25, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C26-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_StreamBitrate,
			0x6E726F48, 0x2C26, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C27-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_BitrateInfoStorage,
			0x6E726F48, 0x2C27, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C28-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_NetworkMediaSSRC,
			0x6E726F48, 0x2C28, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C29-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_PublicRtpInfo,
			0x6E726F48, 0x2C29, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C30-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_UseLossTollerantMP3,
			0x6E726F48, 0x2C30, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C31-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_SvcDependencyId,
			0x6E726F48, 0x2C31, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C33-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_SvcQualityId,
			0x6E726F48, 0x2C33, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {6E726F48-2C32-4493-AEE8-2615EBC11188}
DEFINE_GUID(MCNR_BurstnessValue,
			0x6E726F48, 0x2C32, 0x4493, 0xAE, 0xE8, 0x26, 0x15, 0xEB, 0xC1, 0x11, 0x88);

// {9451777A-18CB-49ce-BEA9-21CBC6C5159B}
DEFINE_GUID(MCNR_NetworkErrorHandling, 
			0x9451777a, 0x18cb, 0x49ce, 0xbe, 0xa9, 0x21, 0xcb, 0xc6, 0xc5, 0x15, 0x9b);

//---------------------------------------------NWRender Params-------------------------------------------------------------
// {97A5D863-EC94-4234-8C12-554DE775EAD0}
DEFINE_GUID(ENR_payload,                    0x97a5d863, 0xec94, 0x4234, 0x8c, 0x12, 0x55, 0x4d, 0xe7, 0x75, 0xea, 0xd0);

// {C49B0276-2227-4d72-88A2-B5D36969F099}
DEFINE_GUID(ENR_transport,                  0xc49b0276, 0x2227, 0x4d72, 0x88, 0xa2, 0xb5, 0xd3, 0x69, 0x69, 0xf0, 0x99);

// {31543CB0-C01C-4b58-B494-E32D0D52CBED}
DEFINE_GUID(ENR_port,                       0x31543cb0, 0xc01c, 0x4b58, 0xb4, 0x94, 0xe3, 0x2d, 0xd, 0x52, 0xcb, 0xed);
DEFINE_GUID(ENR_port_video,                 0x760fcb7a, 0x4f82, 0x4546, 0x93, 0x5c, 0xd9, 0xeb, 0x73, 0x80, 0x57, 0xcf);
DEFINE_GUID(ENR_port_audio,                 0x89453140, 0xa3c0, 0x47bd, 0x90, 0x38, 0x48, 0x1e, 0x5, 0xef, 0xfb, 0x39);
DEFINE_GUID(ENR_port_bifs,                  0x357fa049, 0xaff1, 0x4499, 0x9c, 0x6b, 0x49, 0x79, 0xf, 0x4d, 0xda, 0x94);

// {62E77578-3B29-4702-90AA-E2F6FBF29EDB}
DEFINE_GUID(ENR_destination,                0x62e77578, 0x3b29, 0x4702, 0x90, 0xaa, 0xe2, 0xf6, 0xfb, 0xf2, 0x9e, 0xdb);
// {5DCF9169-6B9C-40c7-BDA1-F4C9587E9F6F}
DEFINE_GUID(ENR_interface,                  0x5dcf9169, 0x6b9c, 0x40c7, 0xbd, 0xa1, 0xf4, 0xc9, 0x58, 0x7e, 0x9f, 0x6f);
// {AEB4E28D-D66F-4ec4-9ADD-52F27F44B23F}
DEFINE_GUID(ENR_connection_speed,           0xaeb4e28d, 0xd66f, 0x4ec4, 0x9a, 0xdd, 0x52, 0xf2, 0x7f, 0x44, 0xb2, 0x3f);
// {A060FFF0-0FC5-41e7-AE97-21484A91F6A8}
DEFINE_GUID(ENR_ttl,                        0xa060fff0, 0xfc5, 0x41e7, 0xae, 0x97, 0x21, 0x48, 0x4a, 0x91, 0xf6, 0xa8);
// {BB21FBC6-21B9-4b9b-AB5B-2435AE1B8C3B}
DEFINE_GUID(ENR_max_bit_rate,               0xbb21fbc6, 0x21b9, 0x4b9b, 0xab, 0x5b, 0x24, 0x35, 0xae, 0x1b, 0x8c, 0x3b);
// {560EB57B-F0CA-4a8e-90B4-FD6AD68B515C}
DEFINE_GUID(ENR_max_video_bit_rate,         0x560eb57b, 0xf0ca, 0x4a8e, 0x90, 0xb4, 0xfd, 0x6a, 0xd6, 0x8b, 0x51, 0x5c);
// {AE754EB2-4D02-48dd-BA3B-C0A3EFD2E76B}
DEFINE_GUID(ENR_max_audio_bit_rate,         0xae754eb2, 0x4d02, 0x48dd, 0xba, 0x3b, 0xc0, 0xa3, 0xef, 0xd2, 0xe7, 0x6b);
// {7696A0BA-C5B6-4786-91F5-8C2699C48EE8}
DEFINE_GUID(ENR_sync_point,                 0x7696a0ba, 0xc5b6, 0x4786, 0x91, 0xf5, 0x8c, 0x26, 0x99, 0xc4, 0x8e, 0xe8);
// {CE75DD63-F658-447e-ADC3-FA93D07D5043}
DEFINE_GUID(ENR_max_upd_size,               0xce75dd63, 0xf658, 0x447e, 0xad, 0xc3, 0xfa, 0x93, 0xd0, 0x7d, 0x50, 0x43);
// {239EEB26-4D56-4b3f-B892-2288D855A353}
DEFINE_GUID(ENR_not_control_rate,           0x239eeb26, 0x4d56, 0x4b3f, 0xb8, 0x92, 0x22, 0x88, 0xd8, 0x55, 0xa3, 0x53);

// {F406F4E5-CE8F-4a20-88B1-B7A9FC2A5FC6}
DEFINE_GUID(ENR_user_name,                  0xf406f4e5, 0xce8f, 0x4a20, 0x88, 0xb1, 0xb7, 0xa9, 0xfc, 0x2a, 0x5f, 0xc6);
// {E914C2B7-7A27-4bcd-AB66-1BC7B3D96C47}
DEFINE_GUID(ENR_session_name,               0xe914c2b7, 0x7a27, 0x4bcd, 0xab, 0x66, 0x1b, 0xc7, 0xb3, 0xd9, 0x6c, 0x47);
// {4271B640-E2EC-459e-8474-120AD5A227C6}
DEFINE_GUID(ENR_session_info,               0x4271b640, 0xe2ec, 0x459e, 0x84, 0x74, 0x12, 0xa, 0xd5, 0xa2, 0x27, 0xc6);
// {A1741E5B-FF49-4db4-91B1-2229712D92ED}
DEFINE_GUID(ENR_description,                0xa1741e5b, 0xff49, 0x4db4, 0x91, 0xb1, 0x22, 0x29, 0x71, 0x2d, 0x92, 0xed);
// {849F69DD-E669-4c78-80B2-31B66F0EF83D}
DEFINE_GUID(ENR_email,                      0x849f69dd, 0xe669, 0x4c78, 0x80, 0xb2, 0x31, 0xb6, 0x6f, 0xe, 0xf8, 0x3d);
// {4FEF6DA3-7869-40df-91BB-188438727B30}
DEFINE_GUID(ENR_phone,                      0x4fef6da3, 0x7869, 0x40df, 0x91, 0xbb, 0x18, 0x84, 0x38, 0x72, 0x7b, 0x30);

//{added by tianyx  17:7:2013   15:05
//{544B4544-0203-4000-8000-012144000193}
DEFINE_GUID(DTA_2144AsiRender1, 0x544B4544, 0x0203, 0x4000, 0x80,0x00,0x01,0x21,0x44,0x00,0x01,0x93);
//{544B4544-0203-4000-8000-022144000193}
DEFINE_GUID(DTA_2144AsiRender2, 0x544B4544, 0x0203, 0x4000, 0x80,0x00,0x02,0x21,0x44,0x00,0x01,0x93);
//{544B4544-0203-4000-8000-032144000193}
DEFINE_GUID(DTA_2144AsiRender3, 0x544B4544, 0x0203, 0x4000, 0x80,0x00,0x03,0x21,0x44,0x00,0x01,0x93);
//{544B4544-0203-4000-8000-042144000193}
DEFINE_GUID(DTA_2144AsiRender4, 0x544B4544, 0x0203, 0x4000, 0x80,0x00,0x04,0x21,0x44,0x00,0x01,0x93);

//{E436EBB5-524F-11CE-9F53-0020AF0BA770}
DEFINE_GUID(DTA_2144FileSource, 0xE436EBB5, 0x524F, 0x11CE, 0x9F,0x53,0x00,0x20,0xAF,0x0B,0xA7,0x70);
//}end added by tianyx

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

tagFilterName::tagFilterName()
{
	memset(m_szName, 0, sizeof(m_szName));
}

tagFilterName::tagFilterName(LPCSTR lpName)
{
	if (lpName)
	{
		USES_CONVERSION;
		_tcscpy(m_szName, A2T((LPSTR)lpName));
	}
}

tagFilterName::tagFilterName(LPCWSTR lpName)
{
	if (lpName)
	{
#if  (defined(_UNICODE) || defined(UNICODE))
		wcscpy(m_szName, lpName);
#else
		int result = ::WideCharToMultiByte(CP_ACP, 0, lpName, -1, m_szName, wcslen(lpName) + 1, NULL, NULL);
		if(result > 0)
			m_szName[result - 1] = 0;
#endif
	}
}



//////////////////////////////////////////////////////////////////////////
//
CAutoFilter::CAutoFilter()
:m_pFilter ( NULL )
{
}
CAutoFilter::CAutoFilter(REFCLSID FILTER_CLSID)
:m_pFilter ( NULL )
{
	Create(FILTER_CLSID);
}

CAutoFilter::CAutoFilter(LPCSTR FILTER_NAMEA)
:m_pFilter ( NULL )
{
	Create(FILTER_NAMEA);
}
CAutoFilter::CAutoFilter(LPCWSTR FILTER_NAMEW)
:m_pFilter ( NULL )
{
	Create(FILTER_NAMEW);
}

int CAutoFilter::Attach(IBaseFilter* pFilter)
{
	SAFE_RELEASE(m_pFilter);
	m_pFilter = pFilter;
	if (m_pFilter)
	{
		m_pFilter->AddRef();
	}
	return !!m_pFilter;
}
IBaseFilter* CAutoFilter::Detach()
{
	IBaseFilter* pFilter = m_pFilter;
	m_pFilter = NULL;

	if (pFilter)
	{
		unsigned long lRef = pFilter->Release();
		if (lRef)
		{
			return pFilter;
		}
	}
	return NULL;
}

int CAutoFilter::Create(REFCLSID FILTER_CLSID)
{
	SAFE_RELEASE(m_pFilter);
	HRESULT hr = S_OK;
	hr = ::CoCreateInstance(FILTER_CLSID, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, reinterpret_cast<void**>(&m_pFilter));
	return SUCCEEDED(hr);
}
int CAutoFilter::Create(LPCSTR FILTER_NAMEA, REFCLSID Category /*= CLSID_LegacyAmFilterCategory*/)
{	
	SAFE_RELEASE(m_pFilter);

	if (!FILTER_NAMEA || strlen(FILTER_NAMEA) <= 0)
	{
		return 0;
	}

	WCHAR FILTER_NAMEW[256];
	if( 0 !=::MultiByteToWideChar(CP_ACP, 0, FILTER_NAMEA, -1, FILTER_NAMEW, 256))
	{
		return Create(FILTER_NAMEW, Category);
	}
	else
	{
		return 0;
	}
}
int CAutoFilter::Create(LPCWSTR FILTER_NAMEW, REFCLSID Category /*= CLSID_LegacyAmFilterCategory*/)
{
	SAFE_RELEASE(m_pFilter);

	if (!FILTER_NAMEW || wcslen(FILTER_NAMEW) <= 0)
	{
		return 0;
	}

	HRESULT				hr				=	S_OK;   
	ICreateDevEnum		*pSysDevEnum	=   NULL;
	IEnumMoniker		*pEnum			=   NULL;
	IMoniker			*pMoniker		=   NULL;

	hr = ::CoCreateInstance(CLSID_SystemDeviceEnum, NULL,CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, reinterpret_cast<void**>(&pSysDevEnum));
	hr = pSysDevEnum->CreateClassEnumerator( Category, &pEnum, 0 );
	if (hr == S_OK)  //   S_FALSE   means   nothing   in   this   category. 
	{   
		while (S_OK == pEnum->Next(1, &pMoniker,NULL))
		{
			IPropertyBag   *pPropBag   =   NULL;
			pMoniker->BindToStorage(0, 0, IID_IPropertyBag,	 reinterpret_cast<void**>(&pPropBag));
			VARIANT   var;
			VariantInit(&var);
			WCHAR FriendlyName[256];
			hr   =   pPropBag->Read(L"FriendlyName",&var, 0); 
			wcscpy( FriendlyName, var.bstrVal);
			VariantClear(&var);
			if (SUCCEEDED(hr) && _wcsicmp(FriendlyName, FILTER_NAMEW) == 0)
			{ 
				hr =  pMoniker->BindToObject(NULL, NULL,IID_IBaseFilter, reinterpret_cast<void**>(m_pFilter));
				SAFE_RELEASE(pPropBag);
				SAFE_RELEASE(pMoniker);
			}
		}
		SAFE_RELEASE(pSysDevEnum);
		SAFE_RELEASE(pEnum);
	}
	return SUCCEEDED(hr);
}

FILTER_NAME CAutoFilter::Name()
{
	if(m_pFilter)
	{
		FILTER_INFO fi;
		m_pFilter->QueryFilterInfo(&fi);
		SAFE_RELEASE(fi.pGraph);
		return fi.achName;
	}
	else
	{
		return (LPCTSTR)0;
	}

}
BOOL CAutoFilter::IsInGraph()
{
	HRESULT hr = S_OK;

	FILTER_INFO	fi;
	if(IsValidate() && SUCCEEDED(hr = m_pFilter->QueryFilterInfo(&fi)) && fi.pGraph)
	{
		fi.pGraph->Release();
		fi.pGraph = NULL;

		return TRUE;
	}
	return FALSE;
}
BOOL CAutoFilter::IsConnected()
{
	IPin		*pPin		= NULL;
	IPin		*pPinCon	= NULL;
	IEnumPins	*pEnumPin	= NULL;
	BOOL		bConnected	= FALSE;

	if ( !IsValidate() )
	{
		return bConnected;
	}

	HRESULT hr = m_pFilter->EnumPins(&pEnumPin);
	pEnumPin->Reset();
	while( hr == S_OK )
	{
		hr = pEnumPin->Next(1, &pPin, NULL);
		if(hr == S_OK && pPin)
		{
			bConnected = IsPinConnected(pPin);
			SAFE_RELEASE(pPin);
		}
		if (bConnected)
		{
			break;
		}
	}
	SAFE_RELEASE(pEnumPin);
	return bConnected;
}

void CAutoFilter::Disconnect()
{
	IPin		*pPin		= NULL;
	IPin		*pPinCon	= NULL;
	IEnumPins	*pEnumPin	= NULL;

	if ( !IsValidate() )
	{
		return ;
	}
	FILTER_INFO fi;
	m_pFilter->QueryFilterInfo(&fi);
	
	if (!fi.pGraph)
	{
		return ;
	}

	HRESULT hr = m_pFilter->EnumPins(&pEnumPin);
	pEnumPin->Reset();
	while( hr == S_OK )
	{
		hr = pEnumPin->Next(1, &pPin, NULL);
		if(hr == S_OK && pPin)
		{
			pPin->ConnectedTo(&pPinCon);
			if (pPinCon)
			{
				//pPinCon->Disconnect();
				fi.pGraph->Disconnect(pPinCon);
				SAFE_RELEASE(pPinCon);
			}
			//pPin->Disconnect();
			fi.pGraph->Disconnect(pPin);
			SAFE_RELEASE(pPin);
		}
	}
	SAFE_RELEASE(pEnumPin);
	SAFE_RELEASE(fi.pGraph);
}

int CAutoFilter::Destroy()
{
	SAFE_RELEASE(m_pFilter);
	return 1;
}
bool CAutoFilter::IsValidate()
{
	return (m_pFilter != NULL);
}

template<typename TT>
HRESULT CAutoFilter::GetInterface(TT** ppIf)
{
	if (!IsValidate())
	{
		return E_FAIL;
	}
	else
	{
		return m_pFilter->QueryInterface(ppIf);
	}
}

IBaseFilter* CAutoFilter::operator->()
{
	return m_pFilter;
}
CAutoFilter& CAutoFilter::operator=(IBaseFilter* pFilter)
{
	Attach(pFilter);
	return *this;
}

CAutoFilter& CAutoFilter::operator=(CAutoFilter& refAf)
{
	Attach(refAf.operator ->());
	refAf.Detach();
	return *this;
}

CAutoFilter& CAutoFilter::operator=(LPUNKNOWN pUnknown)
{
	if (pUnknown)
	{
		IBaseFilter* pFilter = NULL;
		pUnknown->QueryInterface(IID_IBaseFilter, reinterpret_cast<void**>(&pFilter));
		Attach(pFilter);
	}
	return *this;
}
CAutoFilter& CAutoFilter::operator=(REFCLSID FILTER_CLSID)
{
	Create(FILTER_CLSID);
	return *this;
}
CAutoFilter& CAutoFilter::operator=(LPCSTR FILTER_NAMEA)
{
	Create(FILTER_NAMEA);
	return *this;
}
CAutoFilter& CAutoFilter::operator=(LPCWSTR FILTER_NAMEW)
{
	Create(FILTER_NAMEW);
	return *this;
}
CAutoFilter* CAutoFilter::FromPointer(IBaseFilter*& pFilter)
{
	return reinterpret_cast<CAutoFilter*>(&pFilter);
}
UINT CAutoFilter::PinsList(PINLIST& plist, CHECKPIN FunCheck /*= 0*/, LPCGUID lpMediaType /*= 0*/)
{	
	IEnumPins*	pEnumPin = NULL;
	IPin*		pPin	= NULL;
	try
	{
		plist.RemoveAll();

		HRESULT hr = m_pFilter->EnumPins(&pEnumPin);
		if (FAILED(hr))
		{
			return 0;
		}
		while (S_OK == (hr = pEnumPin->Next(1, &pPin, NULL)))
		{	
			BOOL bMediaCheck = TRUE;
			if( lpMediaType )
			{
				IEnumMediaTypes* pEnum = 0;
				pPin->EnumMediaTypes(&pEnum);
				if ( pEnum )
				{
					AM_MEDIA_TYPE* pMt = 0;
					while (S_OK == pEnum->Next(1, &pMt, 0) && pMt )
					{
						if (pMt->majortype == *lpMediaType)
						{
							bMediaCheck = TRUE;
							DeleteMediaType(pMt);
							break;
						}

						DeleteMediaType(pMt);
						bMediaCheck = FALSE;
					}
					SAFE_RELEASE(pEnum);
				}
			}
			BOOL bChecked = FunCheck ? FunCheck(pPin) : TRUE;

			if (bChecked && bMediaCheck)
			{
				plist.SetAtGrow(plist.GetSize(), pPin);
			}
			SAFE_RELEASE(pPin);
		}
		SAFE_RELEASE(pEnumPin);
		return plist.GetSize();
	}
	catch (...)
	{
		plist.RemoveAll();
		SAFE_RELEASE(pPin);
		SAFE_RELEASE(pEnumPin);
		return 0;
	}
}

BOOL CAutoFilter::IsOutAndFree(IPin* pin)
{
	return CheckPinDirect(pin, PINDIR_OUTPUT) && !IsPinConnected(pin);
}
BOOL CAutoFilter::IsInAndFree(IPin* pin)
{
	return CheckPinDirect(pin, PINDIR_INPUT) && !IsPinConnected(pin);
}

BOOL CAutoFilter::IsOutAndConnected(IPin* pin)
{
	return CheckPinDirect(pin, PINDIR_OUTPUT) && IsPinConnected(pin);
}

BOOL CAutoFilter::IsInAndConnected(IPin* pin)
{
	return CheckPinDirect(pin ,PINDIR_INPUT) && IsPinConnected(pin);
}

BOOL CAutoFilter::CheckPinDirect(IPin* pin, PIN_DIRECTION dir)
{
	if (!pin)
	{
		return FALSE;
	}

	PIN_INFO pi;
	pin->QueryPinInfo(&pi);
	SAFE_RELEASE(pi.pFilter);
	return pi.dir == dir;
}

BOOL CAutoFilter::IsPinConnected(IPin* pin)
{
	if (!pin)
	{
		return FALSE;
	}
	IPin* pCon = NULL;
	pin->ConnectedTo(&pCon);
	if (pCon)
	{
		SAFE_RELEASE(pCon);
		return TRUE;
	}
	return FALSE;
}

void CAutoFilter::print(CString& strInfo)
{
	strInfo = "";
	strInfo += Name().m_szName;
	strInfo +="\n\t";
	PINLIST lst;
	PinsList(lst);
	for (int i = 0; i < lst.GetSize(); i++)
	{
		IPin* pin = reinterpret_cast<IPin*>(lst.GetAt(i));
		PIN_INFO pi;
		pin->QueryPinInfo(&pi);
		SAFE_RELEASE(pi.pFilter);
		strInfo += pi.achName;
		strInfo +="<";
		strInfo += pi.dir == PINDIR_INPUT ? "PINDIR_INPUT" : "PINDIR_OUTPUT";
		strInfo +=">\n\t";

		IEnumMediaTypes* pEmt = NULL;
		pin->EnumMediaTypes(&pEmt);
		AM_MEDIA_TYPE* amt[1] = {0};
		if (pEmt)
		{
			while( S_OK == pEmt->Next(1, amt, NULL))
			{
				if( amt[0]->majortype == MEDIATYPE_Video)
				{
					strInfo += "MEDIATYPE_Video";
				}
				else if (amt[0]->majortype == MEDIATYPE_Audio)
				{
					strInfo += "MEDIATYPE_Audio";
				}
				else if (amt[0]->majortype == MEDIATYPE_NULL)
				{
					strInfo += "NULL";
				}
				else if (amt[0]->majortype == MEDIATYPE_Stream)
				{
					strInfo += "MEDIATYPE_Stream";
				}
				else
				{
					strInfo += "Other";
				}
				strInfo += "\n\t";
				DeleteMediaType(amt[0]);
			}
			SAFE_RELEASE(pEmt);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//


EXTERN_C void __stdcall gClearLocalDlls()
{
	CGraphBase::ClearLocalDlls();
}

CGraphBase::CMapClsidToDll* CGraphBase::m_pMapClsidToDll = NULL;
CGraphBase::CLocalDlls* CGraphBase::m_pLocalDlls = NULL;
BOOL CGraphBase::m_bAllLocalDllLoaded = FALSE;
CCritSec CGraphBase::m_LocalDllLock;


class CLocalDll
{
public:
	CLocalDll(LPCTSTR lpDllName)
		: m_hHandle(0)
		, fnDllGetClassObject(0)
	{
		m_hHandle = ::LoadLibraryEx(lpDllName, 0, LOAD_WITH_ALTERED_SEARCH_PATH);
		if( m_hHandle )
		{
			fnDllGetClassObject = (LPFNGETCLASSOBJECT)::GetProcAddress(m_hHandle,"DllGetClassObject");
			if( !fnDllGetClassObject )
			{
				::FreeLibrary(m_hHandle);
				m_hHandle = 0;
			}
		}
	}

	~CLocalDll()
	{
		if( m_hHandle )
		{
			fnDllGetClassObject = 0;
			::FreeLibrary(m_hHandle);
			m_hHandle = 0;
		}
	}
private:
	HMODULE m_hHandle;
	LPFNGETCLASSOBJECT fnDllGetClassObject;
public:
	operator bool() const {return m_hHandle && fnDllGetClassObject;}

	HRESULT __stdcall DllGetClassObject(__in REFCLSID rclsid, __in REFIID riid, LPVOID FAR* ppv)
	{
		if( *this )
		{
			return fnDllGetClassObject(rclsid, riid, ppv);
		}
		return E_FAIL;
	}
};

template<>
AFX_INLINE UINT AFXAPI HashKey<REFCLSID>(REFCLSID key)
{
	return key.Data1 + MAKELONG(key.Data2 ,key.Data3) +
		MAKELONG(MAKEWORD(key.Data4[0], key.Data4[1]),MAKEWORD(key.Data4[2],key.Data4[3])) +
		MAKELONG(MAKEWORD(key.Data4[4], key.Data4[5]),MAKEWORD(key.Data4[6],key.Data4[7]));
}
template<>
AFX_INLINE UINT AFXAPI HashKey<CString&>(CString& key)
{
	int nLength = key.GetLength();

	UINT uRet = 0;
	for (int n = 0; n < nLength; n ++)
	{
		uRet += ((UINT)(key.GetAt(n)) << ((n % 4) * 8));
	}	
	return uRet;
}

class CGraphBase::CMapClsidToDll : public CMap<CLSID, REFCLSID, CLocalDll*, CLocalDll*>
{
};

class CGraphBase::CLocalDlls: public CMap<CString, CString&, CLocalDll*, CLocalDll*>
{
	typedef CMap<CString, CString&, CLocalDll*, CLocalDll*> _BaseT;
public:
	~CLocalDlls()
	{
		Clear();
	}
public:
	void Clear()
	{
		CString strKey;
		CLocalDll* lpDll;
		POSITION pos = _BaseT::GetStartPosition();
		while(pos)
		{
			strKey.Empty();
			lpDll = 0;

			_BaseT::GetNextAssoc(pos, strKey,lpDll);
			if( lpDll )
			{
				delete lpDll;
			}
		}

		_BaseT::RemoveAll();
	}
};

void CGraphBase::ClearLocalDlls()
{
	CAutoLock lc(&m_LocalDllLock);

	if( m_pMapClsidToDll )
	{
		delete m_pMapClsidToDll;
		m_pMapClsidToDll = 0;
	}
	if( m_pLocalDlls )
	{
		delete m_pLocalDlls;
		m_pLocalDlls = 0;
	}
}

class CGraphBase::_ErrImpl
{
protected:
	CString m_strLastError;
	int		m_nLastError;
protected:
	CCritSec	m_csAccessLock;
public:
	_ErrImpl();
	void __cdecl ErrMsg(int nErrCode, LPCTSTR lpFormat, ...);
	void __cdecl ErrMsgV(int nErrCode, LPCTSTR lpFormat, va_list vp);
	void	GetLastError(CString& strLastError);
	int		GetLastErrorCode();
};

CGraphBase::_ErrImpl::_ErrImpl()
{
	m_strLastError = "";
	m_nLastError = 0;
}

void __cdecl CGraphBase::_ErrImpl::ErrMsg(int nErrCode, LPCTSTR lpFormat, ...)
{
	m_csAccessLock.Lock();
	m_nLastError = nErrCode;
	if (lpFormat)
	{
		va_list vp;
		va_start(vp, lpFormat);
		m_strLastError.FormatV(lpFormat, vp);
		va_end(vp);
		OutputDebugString(m_strLastError);
	}
	else
	{
		m_strLastError = "";
	}
	m_csAccessLock.Unlock();
}

void __cdecl CGraphBase::_ErrImpl::ErrMsgV(int nErrCode, LPCTSTR lpFormat, va_list vp)
{
	m_csAccessLock.Lock();
	m_nLastError = nErrCode;
	if (lpFormat)
	{
		m_strLastError.FormatV(lpFormat, vp);
		OutputDebugString(m_strLastError);
	}
	else
	{
		m_strLastError = "";
	}
	m_csAccessLock.Unlock();
}

void CGraphBase::_ErrImpl::GetLastError(CString& strLastError)
{
	m_csAccessLock.Lock();
	strLastError = m_strLastError;
	m_csAccessLock.Unlock();
}

int CGraphBase::_ErrImpl::GetLastErrorCode()
{
	return m_nLastError;
}

CGraphBase::CGraphBase()
: m_pFilterGraph(0)
, m_pError(0)
#ifdef _DEBUG
, m_RegdwID(0)
#endif // _DEBUG
{
	m_pError = new _ErrImpl;
	::CoInitialize(NULL);

	CAutoLock lc(&m_LocalDllLock);

	if( m_pMapClsidToDll == NULL )
	{
		m_pMapClsidToDll = new CGraphBase::CMapClsidToDll();
	}
	if( m_pLocalDlls == NULL )
	{
		m_pLocalDlls = new CGraphBase::CLocalDlls();
	}
}
CGraphBase::~CGraphBase()
{
	ReleaseGraph();
	if (m_pError)
	{
		delete m_pError;
		m_pError = NULL;
	}
	::CoUninitialize();
}

void __cdecl	CGraphBase::ErrMsg(LPCTSTR lpMsg, ...)
{
	try
	{
		if (m_pError && !IsBadWritePtr(m_pError, sizeof(CGraphBase::_ErrImpl)))
		{
			va_list vp;
			va_start(vp, lpMsg);
			m_pError->ErrMsgV(0, lpMsg, vp);
			va_end(vp);
		}
	}catch (...){};
}
void	CGraphBase::GetLastError(CString& strLastError)
{
	try
	{
		if (m_pError && !IsBadWritePtr(m_pError, sizeof(CGraphBase::_ErrImpl)))
		{
			m_pError->GetLastError(strLastError);
		}
	}catch (...){};
}

HRESULT CGraphBase::AddFilterSource(LPCSTR lpFileName, IBaseFilter** pFilter)
{
	USES_CONVERSION;
	return AddFilterSource(A2W(lpFileName), pFilter);
}


HRESULT CGraphBase::AddFilterSource(LPCWSTR lpFileName, IBaseFilter** pFilter)
{
	try
	{
		HRESULT hr = S_OK;
		IFileSourceFilter* pFileSourceFilter=NULL;
		*pFilter = NULL;

		do 
		{
			if(!CAutoFilter::FromPointer(*pFilter)->Create(CLSID_AsyncReader))
			{
				ErrMsg(_T("Error Occurred When Create File Source Filter") );
				break;
			}
			hr = (*pFilter)->QueryInterface(IID_IFileSourceFilter, reinterpret_cast<void**>(&pFileSourceFilter));	
			if(FAILED(hr))
			{
				ErrMsg(TEXT("Cannot find  IID_IFileSourceFilter  interface"));
				break;
			}
			hr=pFileSourceFilter->Load(lpFileName,NULL);
			if (FAILED(hr))
			{
				ErrMsg(TEXT("Cannot Load File "));
				break;
			}
			hr = m_pFilterGraph->AddFilter((*pFilter), L"File Source");
			if (FAILED(hr))
			{
				ErrMsg(TEXT("Cannot add File Source  Filter"));
				break;
			}
		} while(FALSE);

		SAFE_RELEASE(pFileSourceFilter);
		return hr;
	}catch (...)
	{
		return E_UNEXPECTED;
	}
}

HRESULT CGraphBase::AddFilterSink( REFCLSID clsid, LPCWSTR wcsFilterName, LPCWSTR wcsOutputFN, IBaseFilter** pFilter )
{
	try
	{
		HRESULT hr = S_OK;
		*pFilter = NULL;
		if(! CAutoFilter::FromPointer(*pFilter)->Create(clsid))
		{
			return E_OUTOFMEMORY;
		}

		hr = m_pFilterGraph->AddFilter((*pFilter), wcsFilterName);
		if( SUCCEEDED(hr) )
		{
			// Set output file name
			CComQIPtr<IFileSinkFilter> pFileSinkInterface = *pFilter;
			CComQIPtr<IFileSinkFilter2> pFileSinkInterface2 = *pFilter;
			
			if( pFileSinkInterface2 )
			{
				hr = pFileSinkInterface2->SetFileName(wcsOutputFN, NULL);
				pFileSinkInterface2->SetMode(AM_FILE_OVERWRITE);
			}
			else if(pFileSinkInterface)
			{
				hr = pFileSinkInterface->SetFileName(wcsOutputFN, NULL);
			}
		}
		return hr;
	} catch ( ... )
	{
		return E_UNEXPECTED;
	}
}

HRESULT	CGraphBase::AddFilterSink( REFCLSID clsid, LPCSTR csFilterName, LPCSTR csOutputFN, IBaseFilter** pFilter)
{
	USES_CONVERSION;
	return AddFilterSink(clsid, A2W(csFilterName), A2W(csOutputFN), pFilter);
}


HRESULT CGraphBase::AddFilter(REFCLSID clsid,IBaseFilter** pFilter,LPCWSTR lpName)
{
	try
	{
		HRESULT hr = S_OK;
		*pFilter = NULL;

		if(!CAutoFilter::FromPointer(*pFilter)->Create(clsid))
		{
			ErrMsg(TEXT("Cannot find  %s"),lpName);
			return E_FAIL;
		}
		USES_CONVERSION	;
		hr = m_pFilterGraph->AddFilter(*pFilter, lpName);
		if (FAILED(hr))
		{
			ErrMsg(TEXT("Cannot add  %s"),lpName);
			SAFE_RELEASE(*pFilter);
		}
		return hr;
	}catch (...)
	{
		return E_UNEXPECTED;
	}
}

HRESULT CGraphBase::AddFilter(REFCLSID clsid,IBaseFilter** pFilter,LPCSTR lpName)
{
		USES_CONVERSION	;
	return AddFilter(clsid, pFilter, A2W(LPSTR(lpName)));
}

HRESULT CGraphBase::AddFilter(LPCTSTR lpFriendlyName,IBaseFilter** pFilter,  REFCLSID Category/* = CLSID_LegacyAmFilterCategory*/)
{
	try
	{
		HRESULT   hr = S_OK; 
		if (!CAutoFilter::FromPointer(*pFilter)->Create(lpFriendlyName, Category))
		{
			ErrMsg(TEXT("Cannot find  %s"),lpFriendlyName);
			return hr;
		}
		USES_CONVERSION;
		hr = m_pFilterGraph->AddFilter(*pFilter, T2W(LPTSTR(lpFriendlyName)));
		if (FAILED(hr))
		{
			ErrMsg(TEXT("Cannot add  %s"),lpFriendlyName);
			(*pFilter)->Release();
		}
	
		return hr;
	}
	catch (...)
	{
		return E_UNEXPECTED;
	}
}

HRESULT CGraphBase::AddLocalFilter(REFCLSID clsid, IBaseFilter** pFilter, LPCSTR lpName)
{
	USES_CONVERSION	;
	return AddLocalFilter(clsid, pFilter, A2W(LPSTR(lpName)));
}

HRESULT CGraphBase::AddLocalFilter(REFCLSID clsid, IBaseFilter** pFilter, LPCWSTR lpName)
{
	*pFilter = NULL;
	CAutoLock lc(&m_LocalDllLock);

	if( m_pMapClsidToDll && m_pLocalDlls )
	{
		CLocalDll* pDll = NULL;
		if( m_pMapClsidToDll->Lookup(clsid, pDll) && pDll )
		{
			IClassFactory* pFactory = NULL;
			pDll->DllGetClassObject(clsid, IID_IClassFactory, (void**)&pFactory);
			if( pFactory )
			{
				pFactory->CreateInstance(NULL, IID_IBaseFilter, (void**)pFilter);
				pFactory->Release();
			}
		}
		else 
		{
			CString strKey;
			BOOL bFind = FALSE;
			POSITION pos = m_pLocalDlls->GetStartPosition();

			while (pos)
			{
				pDll = 0;
				strKey.Empty();
				m_pLocalDlls->GetNextAssoc(pos, strKey, pDll );

				IClassFactory* pFactory = NULL;
				pDll->DllGetClassObject(clsid, IID_IClassFactory, (void**)&pFactory);
				if( pFactory )
				{
					bFind = TRUE;
					pFactory->CreateInstance(NULL, IID_IBaseFilter, (void**)pFilter);
					pFactory->Release();

					m_pMapClsidToDll->SetAt(clsid, pDll);
					break;
				}
			}

			if( !bFind && !m_bAllLocalDllLoaded )
			{
				CString strPath;
				::GetModuleFileName( ::AfxGetInstanceHandle(), strPath.GetBuffer(1024), 1024 );
				strPath.ReleaseBuffer();
				int nIndex = max(strPath.ReverseFind('\\'), strPath.ReverseFind('/'));
				strPath.Delete(nIndex, strPath.GetLength());
				strPath += "\\Data\\*.*";

				CFileFind ffind;

				BOOL bFindFile = ffind.FindFile(strPath);
				BOOL bFindNext = FALSE;

				if( bFindFile )
					while( (bFindNext = ffind.FindNextFile()) || bFindFile )
					{
						bFindFile = bFindNext;

						if( ffind.IsDots() || ffind.IsDirectory() )
						{
							continue;
						}

						if( m_pLocalDlls->Lookup(ffind.GetFileTitle(), pDll))
						{
							continue;
						}

						CLocalDll* pTryDll = new CLocalDll(ffind.GetFilePath());
						if( *pTryDll )
						{
							m_pLocalDlls->SetAt(ffind.GetFileTitle(), pTryDll);

							IClassFactory* pFactory = NULL;
							pTryDll->DllGetClassObject(clsid, IID_IClassFactory, (void**)&pFactory);
							if( pFactory )
							{
								pFactory->CreateInstance(NULL, IID_IBaseFilter, (void**)pFilter);
								pFactory->Release();

								m_pMapClsidToDll->SetAt(clsid, pTryDll);
								break;
							}
						}
						else
						{
							delete pTryDll;
						}
					}

				if( !bFindFile )
					m_bAllLocalDllLoaded = TRUE;
			}
		}

		if( *pFilter )
		{
			HRESULT hr = m_pFilterGraph->AddFilter(*pFilter, lpName );
			if (FAILED(hr))
			{
				ErrMsg(_T("Cannot add  %s"), lpName);
				(*pFilter)->Release();
				(*pFilter) = 0;
			}
			return hr;
		}
		else
		{
			ErrMsg(_T("Cannot Add LocalFilter %s"), lpName);
		}
	}
	return E_FAIL;
}

HRESULT CGraphBase::AddLocalFilterSink( REFCLSID clsid, LPCSTR csFilterName ,LPCSTR csOutputFN, IBaseFilter** pFilter)
{
	USES_CONVERSION;
	return AddLocalFilterSink(clsid, A2W(csFilterName), A2W(csOutputFN), pFilter);
}

HRESULT CGraphBase::AddLocalFilterSink ( REFCLSID clsid, LPCWSTR wcsFilterName, LPCWSTR wcsOutputFN, IBaseFilter** pFilter)
{
	HRESULT hr = S_OK;
	if( FAILED( hr = AddLocalFilter( clsid, pFilter, wcsFilterName)))
	{
		return hr;
	}

	hr = E_NOINTERFACE;
	CComQIPtr<IFileSinkFilter2> pFileSinkInterface2 = *pFilter;
	CComQIPtr<IFileSinkFilter> pFileSinkInterface = *pFilter;

	if( pFileSinkInterface2 )
	{
		hr = pFileSinkInterface2->SetFileName(wcsOutputFN, NULL);
		pFileSinkInterface2->SetMode(AM_FILE_OVERWRITE);
	}
	else if( pFileSinkInterface )
	{
		hr = pFileSinkInterface->SetFileName(wcsOutputFN, NULL);
	}

	return hr;
}

HRESULT CGraphBase::RemoveNotConnectedFilters()
{
	HRESULT hr = S_OK;
	if (!m_pFilterGraph)
	{
		return hr;
	}

	// Enumerate the filters in the graph.
	IEnumFilters *pEnum = NULL;
	hr = m_pFilterGraph->EnumFilters(&pEnum);
	pEnum->Reset();
	if (SUCCEEDED(hr))
	{
		IBaseFilter *pFilter = NULL;
		while (S_OK == pEnum->Next(1, &pFilter, NULL))
		{
			if(!CAutoFilter::FromPointer(pFilter)->IsConnected())
			{	
				m_pFilterGraph->RemoveFilter(pFilter);
				pEnum->Reset();
			}
			SAFE_RELEASE(pFilter);
		}
		SAFE_RELEASE(pEnum);
	}
	return S_OK;
}
HRESULT CGraphBase::ConnectFilter(IBaseFilter*& pOutFilter, IBaseFilter*& pInFilter, BOOL bIntelligent/* = FALSE */)
{
	CheckPointer(pOutFilter, E_POINTER);
	CheckPointer(pInFilter, E_POINTER);

	CArray<void*, void*>		pins;
	UINT						nPins = CAutoFilter::FromPointer(pOutFilter)->PinsList(pins, &CAutoFilter::IsOutAndFree);

	HRESULT hr = E_FAIL;

	for(UINT i = 0; i < nPins; i++)
	{
		if(FAILED(hr = ConnectOutPinWithFilter(reinterpret_cast<IPin*&>(pins[i]), pInFilter, bIntelligent)))
			return hr;
	}
	CString strLog;
	strLog.Format("Can not Connect the Two Filters<%s>-<%s>", CAutoFilter::FromPointer(pOutFilter)->Name().m_szName, CAutoFilter::FromPointer(pInFilter)->Name().m_szName);
	OutputDebugString(strLog);
	CAutoFilter::FromPointer(pOutFilter)->print(strLog);
	OutputDebugString(strLog);
	CAutoFilter::FromPointer(pInFilter)->print(strLog);
	OutputDebugString(strLog);
	return hr;	
}

HRESULT CGraphBase::ConnectOutPinWithFilter(IPin*& OutPin, IBaseFilter*& Filter, BOOL bIntelligent /* = FALSE */)
{	
	// Look for unconnected input pins
	CArray<void*, void*>		pins;
	UINT						nPins	= CAutoFilter::FromPointer(Filter)->PinsList(pins, &CAutoFilter::IsInAndFree);

	HRESULT						hr		= E_FAIL;

	if(!OutPin)
		return hr;

	for(UINT i = 0; i < nPins; i++)
	{
		if(!pins[i])
			continue;

		// Try to connect
		hr = bIntelligent ?	m_pFilterGraph->Connect(OutPin, reinterpret_cast<IPin*>(pins[i])) :
			m_pFilterGraph->ConnectDirect(OutPin,  reinterpret_cast<IPin*>(pins[i]), NULL);
		if(SUCCEEDED(hr))
		{
			return hr;
		}
	}

	return hr;
}

BOOL CGraphBase::MakeGraph()
{
	// we have one already
	if(m_pFilterGraph)
	{
		return TRUE;
	}

	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, (LPVOID *)&m_pFilterGraph);

#ifdef _DEBUG
 	if (m_pFilterGraph)
 	{
 		AddToROT(&m_RegdwID);
 	}
#endif

	return ((hr == NOERROR) && (m_pFilterGraph != NULL)) ? TRUE : FALSE;
}
HRESULT	CGraphBase::AddToROT(DWORD* pROID)
{
#if !defined UNDER_CE
	IRunningObjectTable* pROT = NULL;
	if(FAILED(GetRunningObjectTable(0, &pROT)))
		return E_FAIL;

	WCHAR wsz[256], wsz1[24];
	ZeroMemory(wsz, sizeof(wsz));
// 	wcscpy(wsz, L"FilterGraph ");
// 	wcscat(wsz, _itow((DWORD_PTR)m_pFilterGraph, wsz1, 16));
// 	wcscat(wsz, L" pid ");
// 	wcscat(wsz, _itow(GetCurrentProcessId(), wsz1, 16));
	StringCchPrintfW(
		wsz, 256, 
		L"FilterGraph %08x pid %08x", 
		(DWORD_PTR)m_pFilterGraph, 
		GetCurrentProcessId()
		);

	IMoniker* pMoniker = NULL;
	HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);

	if(SUCCEEDED(hr)) 
	{
		hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, m_pFilterGraph, pMoniker, pROID);
		pMoniker->Release();
	}
	pROT->Release();
	return hr;

#else
	return S_OK;
#endif

}

HRESULT	CGraphBase::RemoveFromROT(DWORD* pROID)
{
#if !defined UNDER_CE
	IRunningObjectTable* pROT = NULL;
	if(pROID && *pROID && SUCCEEDED(GetRunningObjectTable(0, &pROT))) 
	{
		pROT->Revoke(*pROID);
		*pROID = 0;
		pROT->Release();
	}
#endif
	return S_OK;
}

HRESULT CGraphBase::NukeDownstream(IBaseFilter*& pf)
{
	IPin *pP=0, *pTo=0;
	ULONG u;
	IEnumPins *pins = NULL;
	PIN_INFO pininfo;

	if (!pf)
		return S_OK;

	HRESULT hr = pf->EnumPins(&pins);
	pins->Reset();
	while( hr == S_OK )
	{
		hr = pins->Next(1, &pP, &u);
		if(hr == S_OK && pP)
		{
			pP->ConnectedTo(&pTo);
			if(pTo)
			{
				hr = pTo->QueryPinInfo(&pininfo); //filter 引用加1.
				if( hr == S_OK )
				{ 
					if(pininfo.dir == PINDIR_INPUT)
					{
						NukeDownstream(pininfo.pFilter);
						m_pFilterGraph->Disconnect(pTo);
						m_pFilterGraph->Disconnect(pP);
					}
					SAFE_RELEASE(pininfo.pFilter); //释放引用。
				}
				SAFE_RELEASE(pTo);
			}
			SAFE_RELEASE(pP);
		}
	}
	SAFE_RELEASE(pins);
	return S_OK;
}

HRESULT CGraphBase::FindPin( IBaseFilter*& pFilter, 
								PIN_DIRECTION direction, 
								IPin* &pPin,
								const GUID* MediaTypes[],
								ULONG ctype
								)
{
	if( !pFilter )
	{
		return E_POINTER;
	}

	IEnumPins *pEnumPins = NULL;
	HRESULT hr = pFilter->EnumPins( &pEnumPins );
	if( FAILED( hr ) )
	{
		return hr;
	}

	IPin   *pTempPin = NULL, *pTo = NULL;
	ULONG           fetched;
	PIN_DIRECTION   dir;

	hr = pEnumPins->Reset( );

	while(S_OK == ( hr = pEnumPins->Next( 1, &pTempPin, &fetched ) ) )
	{
		ASSERT( pTempPin );

		hr = pTempPin->QueryDirection( &dir );

		if( FAILED( hr ) || dir != direction || !( VFW_E_NOT_CONNECTED == pTempPin->ConnectedTo( &pTo ) ) )
		{
			SAFE_RELEASE(pTo);
			SAFE_RELEASE(pTempPin);
			continue;
		}
		else if ( !MediaTypes )
		{
			pPin=pTempPin;
			SAFE_RELEASE(pTempPin);
			SAFE_RELEASE(pEnumPins);
			return S_OK;
		}
		else
		{
			IEnumMediaTypes * pEnumTypes = NULL;
			hr = pTempPin->EnumMediaTypes(&pEnumTypes);
			if (FAILED(hr)) 
			{
				pPin=pTempPin;
				SAFE_RELEASE(pTempPin);
				SAFE_RELEASE(pEnumPins);
				return S_FALSE;
			}

			AM_MEDIA_TYPE *mt = NULL;
			UINT i;
			UINT c=0;
			pEnumTypes->Reset( );
			hr = pEnumTypes->Next( 1, &mt, &fetched );
			while ( hr == S_OK )
			{
				c++;
				for ( i=0; i < ctype; i++ )
				{
					if ( mt->majortype == *MediaTypes[i] )
					{					
						pPin = pTempPin;
						SAFE_RELEASE(pEnumTypes);
						SAFE_RELEASE(pTempPin);					
						SAFE_RELEASE(pEnumPins);
						DeleteMediaType( mt );
						return S_OK;
					}
				}
				DeleteMediaType( mt );
				hr = pEnumTypes->Next( 1, &mt, &fetched );
			}
			SAFE_RELEASE(pEnumTypes);

			if ( c==0 )
			{
				pPin=pTempPin;
				SAFE_RELEASE(pTempPin);
				SAFE_RELEASE(pEnumPins);
				return S_OK;
			}		
		}
	}
	SAFE_RELEASE(pTempPin);
	SAFE_RELEASE(pEnumPins);

	return E_FAIL;
}

HRESULT CGraphBase::ReleaseGraph()
{
	Stop();
	RemoveAllFilters(m_pFilterGraph);
// #ifdef _DEBUG
// 	RemoveFromROT(&m_RegdwID);
// #endif // _DEBUG
	SAFE_RELEASE(m_pFilterGraph);
	return S_OK;
}

HRESULT CGraphBase::RemoveAllFilters(IGraphBuilder*& pFG)
{
	try
	{
		if (!m_pFilterGraph)
		{
			return S_OK;
		}
		HRESULT hr = S_OK;

		CArray<void*, void*>	FiltersList;
		// Enumerate the filters in the graph.
		IEnumFilters *pEnum = NULL;
		hr = pFG->EnumFilters(&pEnum);
		if (SUCCEEDED(hr) && pEnum)
		{
			pEnum->Reset();

			IBaseFilter *pFilter = NULL;
			while (S_OK == pEnum->Next(1, &pFilter, NULL))
			{
				FiltersList.Add( pFilter );
				SAFE_RELEASE(pFilter);
			}
			SAFE_RELEASE(pEnum);
		}

		for (int i = 0; i < FiltersList.GetSize(); i++)
		{
			CAutoFilter::FromPointer(reinterpret_cast<IBaseFilter*&>(FiltersList[i]))->Disconnect();
			pFG->RemoveFilter(reinterpret_cast<IBaseFilter*>(FiltersList.GetAt(i)));
		}
		return S_OK;
	}
	catch (...)
	{
		return E_UNEXPECTED;
	}
}

HRESULT CGraphBase::RenderOutputPin( IBaseFilter*& pFilter)
{
	if (!m_pFilterGraph )
	{
		return E_FAIL;
	}
	HRESULT         hr;
	bool            bConnect=false;    
	IPin            *pTempPin = NULL;
	ULONG           fetched;
	PIN_DIRECTION   dir;
	IEnumPins       *pEnumPins = NULL;

	if( !pFilter )
	{
		return E_POINTER;
	}

	hr = pFilter->EnumPins( &pEnumPins );
	if( FAILED( hr ) )
	{
		return hr;
	}

	hr = pEnumPins->Reset( );
	while( S_OK == pEnumPins->Next(1, &pTempPin, &fetched ) )
	{	
		ASSERT( pTempPin );
		IPin *pPin = NULL;
		bConnect = false;
		hr = pTempPin->QueryDirection( &dir );	
		if( pTempPin->ConnectedTo( &pPin ) == VFW_E_NOT_CONNECTED )
			bConnect = true;

		SAFE_RELEASE(pPin);

		//skip the pins begin with a tilde (~)
		PIN_INFO pininfo;
		WCHAR tilde = '~';
		pTempPin->QueryPinInfo(&pininfo);

		if( SUCCEEDED( hr ) && dir == PINDIR_OUTPUT && bConnect && pininfo.achName[0] != tilde )
		{
			/*hr = m_pFg->Render(pTempPin);*/
			IEnumMediaTypes *pEnumMts = NULL;
			hr = pTempPin->EnumMediaTypes(&pEnumMts);
			if( FAILED( hr ) )
			{
				continue;
			}
			pEnumMts->Reset();
			AM_MEDIA_TYPE* pmt;
			if(S_OK == pEnumMts->Next(1,&pmt,NULL))
			{
				IBaseFilter * pVideoFilter = NULL;
				IBaseFilter * pAudioFilter = NULL;

				hr = CoCreateInstance(CLSID_VideoRendererDefault,NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter, reinterpret_cast<void**>(&pVideoFilter));
				if (SUCCEEDED(hr))
				{
					m_pFilterGraph->AddFilter(pVideoFilter,L"Video Renderer");
				}
				hr = CoCreateInstance(CLSID_DSoundRender,NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,reinterpret_cast<void**>(&pAudioFilter));
				if (SUCCEEDED(hr))
				{
					m_pFilterGraph->AddFilter(pAudioFilter,L"DSounder Render");
				}

				if( pmt->majortype == MEDIATYPE_Stream )
				{
					hr = RenderAudioPin(pTempPin, pAudioFilter);
					if (FAILED(hr))
					{
						//render Audio Pin Failed .remove audio filter from m_pFg and then try  video render filter.
						m_pFilterGraph->RemoveFilter(pAudioFilter);
						hr = RenderVideoPin(pTempPin,pVideoFilter);
						if (FAILED(hr))
						{
							m_pFilterGraph->RemoveFilter(pVideoFilter);
						}
					}
					else //audio render successed.remove the video render from m_pFg.
					{
						m_pFilterGraph->RemoveFilter(pVideoFilter);
					}
				}
				else if (pmt->majortype == MEDIATYPE_Video)
				{
					m_pFilterGraph->RemoveFilter(pAudioFilter);
					hr = RenderVideoPin(pTempPin,pVideoFilter);
					if (FAILED(hr))
					{
						m_pFilterGraph->RemoveFilter(pVideoFilter);
					}
				}
				else if (pmt->majortype == MEDIATYPE_Audio)
				{
					m_pFilterGraph->RemoveFilter(pVideoFilter);

					hr = RenderAudioPin(pTempPin,pAudioFilter);

					if (FAILED(hr))
					{
						m_pFilterGraph->RemoveFilter(pAudioFilter);
					}
				}
				else
				{
					m_pFilterGraph->RemoveFilter(pVideoFilter);
					m_pFilterGraph->RemoveFilter(pAudioFilter);
				}

				SAFE_RELEASE(pVideoFilter);				
				SAFE_RELEASE(pAudioFilter);
				DeleteMediaType(pmt);
			}
			SAFE_RELEASE(pEnumMts);
		}
		SAFE_RELEASE(pininfo.pFilter);
		SAFE_RELEASE(pTempPin);
	}
	SAFE_RELEASE(pEnumPins);
	return hr;
}
HRESULT	CGraphBase::RenderVideoPin(IPin* pPin, IBaseFilter* pReceiveFilter)
{
	const GUID * vidoetype[] = {&MEDIATYPE_Video};
	IPin* pVideoPin = NULL;
	HRESULT hr = E_FAIL;
	if (!SUCCEEDED(hr = FindPin(pReceiveFilter, PINDIR_INPUT, pVideoPin, vidoetype,1)))
	{
		return hr;
	}

	if (!SUCCEEDED(hr = m_pFilterGraph->ConnectDirect(pPin, pVideoPin, NULL)))
	{
		return hr;
	}
	return S_OK;
}

HRESULT CGraphBase::RenderAudioPin(IPin* pPin, IBaseFilter* pReceiveFilter)
{
	const GUID * audiotype[] = {&MEDIATYPE_Audio};
	HRESULT hr = E_FAIL;

	IPin* pAudioPin = NULL;
	if (FAILED(hr = FindPin(pReceiveFilter, PINDIR_INPUT, pAudioPin, audiotype,1)))
	{
		return hr;
	}	

	if (FAILED(hr = m_pFilterGraph->ConnectDirect(pPin, pAudioPin, NULL)))
	{
		return hr;
	}

	return S_OK;
}

BOOL CGraphBase::SeekTo(REFERENCE_TIME rtPos)
{
	HRESULT			hr				= S_OK;
	REFERENCE_TIME	llVideoStopPos	= 0;
	IMediaSeeking* pMediaSeeking = NULL;
	m_pFilterGraph->QueryInterface(&pMediaSeeking);
	if( m_pFilterGraph && pMediaSeeking )
	{
		hr = pMediaSeeking->GetStopPosition(&llVideoStopPos);

		if(rtPos < 0 )
			rtPos = 0;
		if(rtPos >= llVideoStopPos)
			rtPos = llVideoStopPos;
		
		hr = pMediaSeeking->SetPositions(&rtPos,
			AM_SEEKING_AbsolutePositioning,
			NULL, AM_SEEKING_NoPositioning );

		SAFE_RELEASE(pMediaSeeking);
		if(SUCCEEDED(hr))
		{	
			return true;
		}
	}
	return false;
}

LONGLONG CGraphBase::GetDuration()
{
	HRESULT			hr				= S_OK;
	REFERENCE_TIME	llDur			= 0;
	IMediaSeeking* pMediaSeeking = NULL;
	if( m_pFilterGraph )
	{	
		m_pFilterGraph->QueryInterface(IID_IMediaSeeking, (void**)&pMediaSeeking);
		if ( pMediaSeeking )
		{
			hr = pMediaSeeking->GetDuration(&llDur);
			SAFE_RELEASE(pMediaSeeking);
			if(SUCCEEDED(hr))
			{
				return llDur;
			}
		}
	}
	return -1;
}

BOOL CGraphBase::Run()
{
	HRESULT			hr	= S_OK;
	IMediaControl*	pMC = NULL;
	// run the graph
	if(m_pFilterGraph)
	{
		hr = m_pFilterGraph->QueryInterface(IID_IMediaControl, (void **)&pMC);
		if(SUCCEEDED(hr))
		{
			hr = pMC->Run();
			if(FAILED(hr))
			{
				// stop parts that ran
				ErrMsg(TEXT("Error %x: Cannot run graph"), hr);
				pMC->Stop();
				SAFE_RELEASE(pMC);
				return false;
			}
			else
			{
				SAFE_RELEASE(pMC);
				return true;
			}
		}
	}
	return false;
}
BOOL CGraphBase::IsRuning()
{
	HRESULT hr = S_OK;
	IMediaControl* pMC = NULL;
	if ( m_pFilterGraph )
	{
		hr = m_pFilterGraph->QueryInterface(IID_IMediaControl, (void**)&pMC);
		if ( SUCCEEDED ( hr ))
		{
			OAFilterState s;
			hr = pMC->GetState(500, &s );
			SAFE_RELEASE(pMC);
			if ( hr == S_OK )
			{
				return s == State_Running;
			}
		}
	}
	return FALSE;
}
BOOL CGraphBase::Pause()
{
	IMediaControl *pMC = NULL;
	if(m_pFilterGraph)
	{
		HRESULT hr = m_pFilterGraph->QueryInterface(IID_IMediaControl, (void **)&pMC);
		if(SUCCEEDED(hr))
		{
			hr = pMC->Pause();
			SAFE_RELEASE(pMC);
			if( FAILED(hr) )
			{
				ErrMsg(TEXT("Error %x: Cannot Pause graph"), hr);
				return false;
			}
			else 
				return true;
		}
	}
	return false;
}

BOOL CGraphBase::Stop()
{
	IMediaControl *pMC = NULL;
	if(m_pFilterGraph)
	{
		HRESULT hr = m_pFilterGraph->QueryInterface(IID_IMediaControl, (void **)&pMC);
		if(SUCCEEDED(hr))
		{
			hr = pMC->Stop();
			SAFE_RELEASE(pMC);
			if( FAILED(hr) )
			{
				ErrMsg(TEXT("Error %x: Cannot stop graph"), hr);
				return false;
			}
			else 
			{
				return true;
			}
		}
	}
	return false;
}

BOOL CGraphBase::SetRate(double rate)
{
	HRESULT			hr				= S_OK;
	IMediaSeeking*	pMediaSeeking	= NULL;
	BOOL bret = false;
	m_pFilterGraph->QueryInterface(&pMediaSeeking);
	if(pMediaSeeking)
	{
		if( rate>0 )
		{
			hr=pMediaSeeking->SetRate(rate);

			if(SUCCEEDED(hr))
			{
				bret = true;
			}
		}
	}
	SAFE_RELEASE(pMediaSeeking);
	return bret;
}

double CGraphBase::GetRate()
{
	double			dRate			= 0;
	IMediaSeeking*	pMediaSeeking	= NULL;
	m_pFilterGraph->QueryInterface(&pMediaSeeking);
	if(pMediaSeeking)
	{
		pMediaSeeking->GetRate(&dRate);
	}
	SAFE_RELEASE(pMediaSeeking);
	return dRate;
}
int	CGraphBase::CurrentGraphState(IGraphBuilder*& pFG)
{
	if (!pFG)
	{
		return -1;
	}
	CComQIPtr<IMediaControl> pMediaControl = pFG;
	if (pMediaControl)
	{
		OAFilterState fs = -1;
		pMediaControl->GetState(500,&fs);
		return fs;
	}
	return -1;
}

HRESULT CGraphBase::CLSIDToString(const GUID* pGUID, CString& strGUID)
{
	CheckPointer(pGUID, E_POINTER);
	if (IsBadReadPtr(pGUID, sizeof(GUID)))
	{
		return E_INVALIDARG;
	}
	//////////////////////////////////////////////////////////////////////////
	// 形式如下：
	// {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX} 
	//
	strGUID.Format("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		pGUID->Data1,pGUID->Data2,pGUID->Data3,
		pGUID->Data4[0],pGUID->Data4[1],pGUID->Data4[2],pGUID->Data4[3],
		pGUID->Data4[4],pGUID->Data4[5],pGUID->Data4[6],pGUID->Data4[7]);
	return S_OK;
}

HRESULT CGraphBase::StringToCLSID(LPCTSTR lpszGUID, CLSID& rid)
{
	//////////////////////////////////////////////////////////////////////////
	// 形式如下：
	// {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX} 
	//
	rid = GUID_NULL;
	CheckPointer(lpszGUID, E_POINTER);
	DWORD dwVal[11];
	CString strGUID(lpszGUID);
	strGUID.Replace('-', ' ');
	strGUID.Trim("\t\n {[(<>)]}");
	sscanf(strGUID, "%x %x %x %2x%2x %2x%2x%2x%2x%2x%2x", 
		&dwVal[0], &dwVal[1], &dwVal[2],
		&dwVal[3], &dwVal[4], &dwVal[5], &dwVal[6],
		&dwVal[7], &dwVal[8], &dwVal[9], &dwVal[10]);

	rid.Data1		= dwVal[0];
	rid.Data2		= (unsigned short)dwVal[1];
	rid.Data3		= (unsigned short)dwVal[2];
	rid.Data4[0]	= (unsigned char)dwVal[3];
	rid.Data4[1]	= (unsigned char)dwVal[4];
	rid.Data4[2]	= (unsigned char)dwVal[5];
	rid.Data4[3]	= (unsigned char)dwVal[6];
	rid.Data4[4]	= (unsigned char)dwVal[7];
	rid.Data4[5]	= (unsigned char)dwVal[8];
	rid.Data4[6]	= (unsigned char)dwVal[9];
	rid.Data4[7]	= (unsigned char)dwVal[10];
	return S_OK;
}
HRESULT CGraphBase::ReleaseTimer()
{
	if (m_pFilterGraph)
	{
		IMediaFilter* pCon = NULL;
		m_pFilterGraph->QueryInterface(&pCon);
		if (pCon)
		{
			HRESULT hr =  pCon->SetSyncSource(NULL);
			SAFE_RELEASE(pCon);
			return hr;
		}
	}
	return E_UNEXPECTED;
}

#ifdef _DEBUG

HRESULT CGraphBase::savegrf()
{
	const WCHAR wszStreamName[] = L"ActiveMovieGraph"; 
	HRESULT hr;

	IStorage *pStorage = NULL;
	hr = StgCreateDocfile(
		L".\\output.grf",
		STGM_CREATE | STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
		0, &pStorage);
	if(FAILED(hr)) 
	{
		return hr;
	}

	IStream *pStream;
	hr = pStorage->CreateStream(
		wszStreamName,
		STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,
		0, 0, &pStream);
	if (FAILED(hr)) 
	{
		pStorage->Release();    
		return hr;
	}

	IPersistStream *pPersist = NULL;
	m_pFilterGraph->QueryInterface(IID_IPersistStream, (void**)&pPersist);
	hr = pPersist->Save(pStream, TRUE);
	pStream->Release();
	pPersist->Release();
	if (SUCCEEDED(hr)) 
	{
		hr = pStorage->Commit(STGC_DEFAULT);
	}
	pStorage->Release();
	return hr;

}
#endif

//////////////////////////////////////////////////////////////////////////
// CIndexedFilter
// 

IBaseFilter*&	CIndexedFilter::operator[](WORD wIndex)
{
	CAutoLock	lc(&m_Lock);
	void*		pKey = NULL;
	if( CMapWordToPtr::Lookup(wIndex, pKey))
	{
		return *reinterpret_cast<IBaseFilter**>(pKey);
	}
	else
	{
		IBaseFilter** pFilter = new IBaseFilter*();
		*pFilter = NULL;
		CMapWordToPtr::SetAt(wIndex, pFilter);
		return *pFilter;
	}
}
void CIndexedFilter::ReleaseAllFilter()
{
	CAutoLock	lc(&m_Lock);
	POSITION	pos = CMapWordToPtr::GetStartPosition();
	WORD		wIndex = 0;
	void*		pKey = 0;
	while (NULL != pos)
	{
		CMapWordToPtr::GetNextAssoc(pos, wIndex, pKey);
		SAFE_RELEASE(*reinterpret_cast<IBaseFilter**>(pKey));		
		delete reinterpret_cast<IBaseFilter**>(pKey);
	}
	CMapWordToPtr::RemoveAll();
}

