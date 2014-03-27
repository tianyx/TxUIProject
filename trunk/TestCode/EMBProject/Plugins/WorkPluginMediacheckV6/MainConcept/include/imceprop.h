//------------------------------------------------------------------------------
// File: imceprop.h
//
// Desc: Encoder property interface
//
// Copyright (c) 2000-2007, MainConcept AG.  All rights reserved.
//--------------------------------------------------------------------------

#ifndef _IMCEPROP_H_
#define _IMCEPROP_H_

//////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////

#ifndef API_EXT_PARAM_LIST
  #define API_EXT_PARAM_LIST
#endif

#ifndef API_UD
  #define API_UD
#endif

//////////////////////////////////////////////////////////////////////////////

#include <mpegdef.h>
#include <mpgvout.hpp>
#include <mpgaout.hpp>
#include <mpgmux.hpp>

//////////////////////////////////////////////////////////////////////////////
//
// IMCMpegVideoEncoder
//
#ifndef __IMCMpegVideoEncoder__

DECLARE_INTERFACE_(IMCMpegVideoEncoder, IUnknown) 
{
    STDMETHOD(get_VideoMpegType)
        ( THIS_
          LPDWORD lpdwMpegType  // [out] mpeg type, defined in mpegdef.h
        ) PURE;

    STDMETHOD(put_VideoMpegType)
        ( THIS_
          DWORD   dwMpegType    // [in]  mpeg type, defined in mpegdef.h
        ) PURE;

    STDMETHOD(get_VideoMode)
        ( THIS_
          LPDWORD lpdwVideoMode // [out] video mode, defined in mpegdef.h
        ) PURE;

    STDMETHOD(put_VideoMode)
        ( THIS_
          DWORD   dwVideoMode   // [in]  video mode, defined in mpegdef.h
        ) PURE;

    STDMETHOD(get_VideoPerformance)
        ( THIS_
          LPBOOL  lpbOnline,    // [out] online/offline mode
          LPDWORD lpdwLevel,    // [out] performance level
          LPDWORD lpReserved    // [out] reserved, default 0
        ) PURE;

    STDMETHOD(put_VideoPerformance)
        ( THIS_
          BOOL    bOnline,      // [in]  online/offline mode
          DWORD   dwLevel,      // [in]  performance level
          DWORD   Reserved      // [in]  reserved, default 0
        ) PURE;

    STDMETHOD(get_VideoBitrate)
        ( THIS_
          LPDWORD lpdwBitrate   // [out] bits per second
        ) PURE;

    STDMETHOD(put_VideoBitrate)
        ( THIS_
          DWORD   dwBitrate     // [in]  bits per second
        ) PURE;

    STDMETHOD(GetVideoBitrateInfo)
        ( THIS_
          DWORD   dwMpegType,   // [in]  mpeg type to get range from   
          LPDWORD lpdwMinimum,  // [out] minimal bitrate
          LPDWORD lpdwMaximum,  // [out] maximal bitrate
          LPDWORD lpdwDefault   // [out] default bitrate
        ) PURE;

    STDMETHOD(get_VideoSettings)
        ( THIS_
          mpeg_v_settings* pVSettings   // [out] MPEG API video encoder settings
        ) PURE;

    STDMETHOD(put_VideoSettings)
        ( THIS_
          mpeg_v_settings* pVSettings   // [in] MPEG API video encoder settings
        ) PURE;

    STDMETHOD(GetDefaultVideoSettings)     
        ( THIS_                         
          DWORD            dwMpegType,  // [in] mpeg type (MPEG_MPEG1, ...
          DWORD            dwVideoMode, // [in] video mode (VM_PAL/VM_NTSC)
          mpeg_v_settings* pVSettings   // [out] MPEG API video encoder settings
        ) PURE;

    STDMETHOD(PutDefaultVideoSettings)     
        ( THIS_                         
          DWORD            dwMpegType,  // [in] mpeg type (MPEG_MPEG1, ...
          DWORD            dwVideoMode  // [in] video mode (VM_PAL/VM_NTSC)
        ) PURE;

};

#define __IMCMpegVideoEncoder__
#endif  //__IMCMpegVideoEncoder__

//////////////////////////////////////////////////////////////////////////////
//
// IMCMpegVideoEncoderFilter
//

typedef BOOL (*VIDEOUSERDATACALLBACK) (user_data_tt**); // callback function to create user data struct
                                                        // returns TRUE if user data are valid, otherwise FALSE

#ifndef __IMCMpegVideoEncoderFilter__
DECLARE_INTERFACE_(IMCMpegVideoEncoderFilter, IUnknown) 
{
    STDMETHOD(get_VideoFilterSetting)
        ( THIS_
          DWORD   dwSettingID,  // [in]  video encoder filter setting ID
          LPDWORD lpdwSetting   // [out] video encoder filter setting
        ) PURE;

    STDMETHOD(put_VideoFilterSetting)
        ( THIS_
          DWORD   dwSettingID,  // [in] video encoder filter setting ID
          DWORD   dwSetting     // [in] video encoder filter setting
        ) PURE;
};

#define VFSETTING_INDEXOUTPUT             0x0010  // video index output buffer
    // setting is a pointer to bufstream_tt (see bufstrm.hpp)
    //   or zero to disable index output

#define VFSETTING_CREATEOFFSETTIMESTAMPS  0x0018  // output sample timestamps usage
	                                                // TRUE: the output sample time stamps are byte offsets 
	                                                // FALSE: the output sample time stamps are time stamps


#define VFSETTING_FRAMEENCODING_OPTIONS   0x0020 // frame encoding option
    //#define OPT_LINE_FILTER  (0x0200)     // defined in mpgvout.hpp

#define VFSETTING_AUTHENTIFICATION        0x0021 // authentification code, put only
    
#define VFSETTING_CROP_WITDH              0x0030 // crop width
#define VFSETTING_CROP_HEIGHT             0x0031 // crop height

#define VFSETTING_NOTIFYRECEIVE           0x0040 // send notification event on receive data at video input pin
                                                 // setting >= 0x8000 event code to send
                                                 // setting <  0x8000 disable event send

#define VFSETTING_PAGEDISPLAYMODE         0x0050 // send display mode for property page
    #define VFS_PAGEDISPLAYMODE_AUTO 0x0000      //   let the filter decide
    #define VFS_PAGEDISPLAYMODE_ON   0x0001      //   always enable all entries
    #define VFS_PAGEDISPLAYMODE_OFF  0x0002      //   always disable all entries

#define VFSETTING_HANDLETIMESTAMPS        0x0060 // take care of timestamps, bool
                                                 //   disabled if in online mode

#define VFSETTING_ONLINEMODE              0x0061 // online mode independend from put_VideoPerformance call

#define VFSETTING_REPEATFRAMETHRESHOLD    0x0062 // threshold when encoder starts inserting frames during capture, default is 3
#define VFSETTING_DISCARDFRAMETHRESHOLD   0x0063 // threshold when encoder starts discarding frames during capture, default is 1

#define VFSETTING_GET_RC                  0x0070 // get_rc func for low level encoder
                                                 //   setting is a pointer to the get_rc function
                                                 //   or zero to let the filter use it's own (default)

#define VFSETTING_BUFFERSIZE              0x0080 // output buffer size for delivered samples
                                                 //   0: use default
#define VFSETTING_SENDBUFFERONFRAME       0x0081 // send data downstream if frame is read, bool

#define VFSETTING_PICTUREUSERDATA         0x0090 // set fixed user data for each frame
                                                 //    lpdwSetting is pointer to user_data_tt struct  
                                                 //    or zero to reset the user data
#define VFSETTING_PICTUREUSERDATACALLBACK 0x0098 // set user data callback
                                                 //    lpdwSetting is pointer VIDEOUSERDATACALLBACK
                                                 //    or zero to reset the user data callback

#define VFSETTING_SEQUENCEUSERDATA_FIRST  0x00A0 // set user data for first sequence header
#define VFSETTING_SEQUENCEUSERDATA_ALL    0x00A1 // set user data for all sequence headers
                                                 //    lpdwSetting is pointer to user_data_tt struct  
                                                 //    or zero to reset the user data

#define VFSETTING_INPUTPIN_CONNECTED      0x8000 // video input pin connected, bool, read only


#define __IMCMpegVideoEncoderFilter__
#endif  //__IMCMpegVideoEncoderFilter__

//////////////////////////////////////////////////////////////////////////////
//
// IMCMpegAudioEncoder
//

#ifndef __IMCMpegAudioEncoder__
DECLARE_INTERFACE_(IMCMpegAudioEncoder, IUnknown) 
{
    STDMETHOD(get_AudioMode)
        ( THIS_
          LPDWORD lpdwMode      // [out] audio mode, defined in mpegdef.h (MPG_MD_STEREO, MPG_MD_JOINT_STEREO, ...)
        ) PURE;

    STDMETHOD(put_AudioMode)
        ( THIS_
          DWORD   dwMode        // [in]  mpeg type, defined in mpegdef.h
        ) PURE;

    STDMETHOD(get_AudioLayer)
        ( THIS_
          LPDWORD lpdwLayer     // [out] mpeg audio layer (1,2)
        ) PURE;

    STDMETHOD(put_AudioLayer)
        ( THIS_
          DWORD   dwLayer       // [in]  mpeg audio layer (1,2)
        ) PURE;

    STDMETHOD(get_AudioBitrateIndex)
        ( THIS_
          LPDWORD lpdwIndex     // [out] mpeg audio bitrate index, defined in mpegdef.h
        ) PURE;

    STDMETHOD(put_AudioBitrateIndex)
        ( THIS_
          DWORD   dwIndex       // [in]  mpeg audio bitrate index, defined in mpegdef.h
        ) PURE;


    STDMETHOD(get_AudioCRCCheck)
        ( THIS_
          LPBOOL  lpbCRCOn      // [out] mpeg audio CRC check on
        ) PURE;

    STDMETHOD(put_AudioCRCCheck)
        ( THIS_
          BOOL    bCRCOn        // [in]  mpeg audio CRC check on
        ) PURE;

    STDMETHOD(get_AudioSettings)
        ( THIS_
          mpeg_a_settings* pASettings   // [out] MPEG API audio encoder settings
        ) PURE;

    STDMETHOD(put_AudioSettings)
        ( THIS_
          mpeg_a_settings* pASettings   // [in] MPEG API audio encoder settings
        ) PURE;

    STDMETHOD(GetDefaultAudioSettings)     
        ( THIS_                         
          DWORD            dwMpegType,  // [in] mpeg type (MPEG_MPEG1, ...
          mpeg_a_settings* pASettings   // [out] MPEG API audio encoder settings
        ) PURE;

    STDMETHOD(PutDefaultAudioSettings)     
        ( THIS_                         
          DWORD            dwMpegType   // [in] mpeg type (MPEG_MPEG1, ...
        ) PURE;


};

#define __IMCMpegAudioEncoder__
#endif  //__IMCMpegAudioEncoder__

//////////////////////////////////////////////////////////////////////////////
//
// IMCMpegAudioEncoderFilter
//
#ifndef __IMCMpegAudioEncoderFilter__

DECLARE_INTERFACE_(IMCMpegAudioEncoderFilter, IUnknown) 
{
    STDMETHOD(get_AudioFilterSetting)
        ( THIS_
          DWORD   dwSettingID,  // [in]  audio encoder filter setting ID
          LPDWORD lpdwSetting   // [out] audio encoder filter setting
        ) PURE;

    STDMETHOD(put_AudioFilterSetting)
        ( THIS_
          DWORD   dwSettingID,  // [in] audio encoder filter setting ID
          DWORD   dwSetting     // [in] audio encoder filter setting
        ) PURE;
};

#define AFSETTING_FREQUENCYSUPPORT   0x0001  // supported audio frequencies, bitfield
    #define AFS_FREQUENCY_32000 0x0001
    #define AFS_FREQUENCY_44100 0x0002
    #define AFS_FREQUENCY_48000 0x0004
    #define AFS_FREQUENCY_ALL   (AFS_FREQUENCY_32000 | AFS_FREQUENCY_44100 | AFS_FREQUENCY_48000)

#define AFSETTING_INDEXOUTPUT        0x0010  // audio index output buffer
    // setting is a pointer to bufstream_tt (see bufstrm.hpp)
    //   or zero to disable index output

#define AFSETTING_NOTIFYRECEIVE      0x0040 // send notification event on receive data at audio input pin
                                            // setting >= 0x8000 event code to send
                                            // setting <  0x8000 disable event send

#define AFSETTING_PAGEDISPLAYMODE         0x0050 // send display mode for property page
    #define AFS_PAGEDISPLAYMODE_AUTO 0x0000      //   let the filter decide
    #define AFS_PAGEDISPLAYMODE_ON   0x0001      //   always enable all entries
    #define AFS_PAGEDISPLAYMODE_OFF  0x0002      //   always disable all entries

#define AFSETTING_HANDLETIMESTAMPS        0x0060 // take care of timestamps, bool
                                                 //   disabled if in online mode

#define AFSETTING_GET_RC                  0x0070 // get_rc func for low level encoder
    // setting is a pointer to the get_rc function
    //   or zero to let the filter use it's own (default)

#define AFSETTING_BUFFERSIZE              0x0080 // output buffer size for delivered samples
                                                 //   0: use default

#define AFSETTING_INPUTPIN_CONNECTED 0x8000 // audio input pin connected, bool, read only

#define __IMCMpegAudioEncoderFilter__
#endif  //__IMCMpegAudioEncoderFilter__



//////////////////////////////////////////////////////////////////////////////
//
// IMCMpegMuxer
//
#ifndef __IMCMpegMuxer__

DECLARE_INTERFACE_(IMCMpegMuxer, IUnknown) 
{
    STDMETHOD(get_MuxMpegType)
        ( THIS_
          LPDWORD lpdwMpegType  // [out] mpeg type, defined in mpegdef.h
        ) PURE;

    STDMETHOD(put_MuxMpegType)
        ( THIS_
          DWORD   dwMpegType    // [in]  mpeg type, defined in mpegdef.h
        ) PURE;

    STDMETHOD(get_MuxerSettings)
        ( THIS_
          mpeg_m_settings* pMSettings   // [out] MPEG API muxer settings
        ) PURE;

    STDMETHOD(put_MuxerSettings)
        ( THIS_
          mpeg_m_settings* pMSettings   // [in] MPEG API muxer settings
        ) PURE;

    STDMETHOD(GetDefaultMuxerSettings)     
        ( THIS_                         
          DWORD            dwMpegType,  // [in] mpeg type (MPEG_MPEG1, ...
          mpeg_m_settings* pMSettings   // [out] MPEG API muxer settings
        ) PURE;

    STDMETHOD(PutDefaultMuxerSettings)     
        ( THIS_                         
          DWORD            dwMpegType   // [in] mpeg type (MPEG_MPEG1, ...)
        ) PURE;

		STDMETHOD(get_ExtMuxerSettings)
        ( THIS_
          extended_m_settings* pExtMSettings   // [out] MPEG API extended muxer settings
        ) PURE;

    STDMETHOD(put_ExtMuxerSettings)
        ( THIS_
          extended_m_settings* pExtMSettings   // [in] MPEG API extended muxer settings - NULL to reset extended settings
        ) PURE;																 // note: EXT_OUTPUT_DATA_FOR_XML flag is not suported.
	
	// note: put_ExtMuxerSettings overwrites previously set filter settings MFSETTING_PAD_PES_EXTENSION and
	// MFSETTING_AUDIO_NO_SPAN_VOBU!
	// Deprecated filter settings MFSETTING_PAD_PES_EXTENSION and MFSETTING_AUDIO_NO_SPAN_VOBU 
	// overwrite corresponding flags in extended_m_settings.flags when extended_m_settings where 
	// set by put_ExtMuxerSettings before!
};

#define __IMCMpegMuxer__
#endif //__IMCMpegMuxer__

//////////////////////////////////////////////////////////////////////////////
//
// IMCMpegMuxerFilter
//
#ifndef __IMCMpegMuxerFilter__

DECLARE_INTERFACE_(IMCMpegMuxerFilter, IUnknown) 
{
    STDMETHOD(get_MuxerFilterSetting)
        ( THIS_
          DWORD   dwSettingID,  // [in]  muxer filter setting ID
          LPDWORD lpdwSetting   // [out] muxer filter setting
        ) PURE;

    STDMETHOD(put_MuxerFilterSetting)
        ( THIS_
          DWORD   dwSettingID,  // [in] muxer filter setting ID
          DWORD   dwSetting     // [in] muxer filter setting
        ) PURE;


};


#define MFSETTING_STATISTIC               0x0010 // get statistic values (read only)
                                                 // lpdwSetting is pointer to MUXERSTATISTIK
  typedef struct tagMUXERSTATISTIK
  {
    DWORD    dwSize;                 // size of this structure
    LONGLONG llTotalBytes;           // total output bytes 
    DWORD    dwStartTime;            // encoding start time (msec)
    DWORD    dwCurrTime;             // current processing time
    DWORD    dwTotalTime;            // total processing time
    DWORD    dwVideoFrames;          // number of processed video frames
    DWORD    dwRepeatedVideoFrames;  // number of repeated video frames
    DWORD    dwDiscardedVideoFrames; // number of discarded video frames
    DWORD    dwAudioSamples;         // number of processed audio samples
  } MUXERSTATISTIK, *PMUXERSTATISTIK;


#define MFSETTING_WRITERMODE		     0x0020  // set one of the following writer modes:
    #define		MFS_WRITER_SINGLE        0x00  //   single file mode - default, filename: myfile.mpg 
	                                           //   no limit if splitsize is 0 or stops at filesize limit. On Fat32 volumes stops at 4GB  
	  #define		MFS_WRITER_MULTIPLE      0x01  //   multiple file mode, split by size - filenames: myfile0001.mpg, myfile0002.mpg ....

#define MFSETTING_FILESPLITSIZE      0x0021  // split file size in MByte, 0: disable

#define MFSETTING_CREATELISTFILE	   0x0022  // create a file with a list of captured files .mzl
																		         // default 0: disabled, 1 enabled
                                             // multiple file mode only

#define MFSETTING_SHOW_SPLIT_OPTIONS 0x0023  // show split options on property page 
																					   // default 1: enabled, 0: disabled,

#define MFSETTING_SPLIT_OUPUT				 0x0024  // put only, multiple file mode only:
																						 // split file on next GOP boundary 

#define MFSETTING_SYNC_ON_TIMESTAMPS 0x0025  // synchonize audio inputs on first video input's first I-Frames timestamp
																						 // to be used for live input, eg. demuxed DVB TS. Inputs must have valid continous timestamps
																						 // 0: disabled (default), 1 enabled

#define MFSETTING_TIMELIMIT          0x0030  // time limit in msec, default 0: disabled

#define MFSETTING_WRITE_IDX_FILES    0x0040	 // write index output to files - takes effect if a file has been set

// deprecated - use IMCMpegMuxer::put_ExtMuxerSettings to set this flags 
#define MFSETTING_PAD_PES_EXTENSION  0x0041	// pad 3 bytes PES extension field for all video streams
																						// default 0: disabled, 1 enabled
																						// takes effect on DVD format only

// deprecated - use IMCMpegMuxer::put_ExtMuxerSettings to set this flags
#define MFSETTING_AUDIO_NO_SPAN_VOBU 0x0044 // don't split audio frames over a VOBU

// note: put_ExtMuxerSettings overwrites previously set filter settings MFSETTING_PAD_PES_EXTENSION and
// MFSETTING_AUDIO_NO_SPAN_VOBU!
// MFSETTING_PAD_PES_EXTENSION and MFSETTING_AUDIO_NO_SPAN_VOBU overwrite corresponding flags
// in extended_m_settings.flags when extended_m_settings where set by put_ExtMuxerSettings before!



#define MFSETTING_VIDEO_INDEXOUTPUT		0x0042 // video index output bufstream - use for standalone muxer only
#define MFSETTING_AUDIO_INDEXOUTPUT		0x0043 // audio index output bufstream - use for standalone muxer only
// setting is a pointer to bufstream_tt (see bufstrm.hpp)
// ignored if MFSETTING_WRITE_IDX_FILES is set
// For all in one encoder use AFSETTING_INDEXOUTPUT and VFSETTING_INDEXOUTPUT instead

#define MFSETTING_BUFFERSIZE          0x0080 // output buffer size for delivered samples
                                             // 0: use default

// standalone multiplexer only
#define MFSETTING_VIDEO_INPUT_BUFFERSIZE  0x0081  // size of video inputs fifo in kB. default (4MB)
#define MFSETTING_AUDIO_INPUT_BUFFERSIZE  0x0082  // size of audio inputs fifo in kB. default (2MB)
// use to increase the the audio and video inputs fifo buffer sizes to avoid input blocking
// in case upstream filters delay the elementary stream for some reason.
// Must be >= 4096kB for video inputs and >= 2048kB for audio inputs
// None of the inputs must be connected !



//
// filter events sent to the graph, when the all in one encoder or multiplexer is in filewriter mode
// an app should lounch for them and stop the graph
//
#define EC_MC_MPEGENC_BASE               EC_USER + 0x400
#define EC_MC_MPEGENC_SIZE_LIMIT         EC_MC_MPEGENC_BASE + 0x1  // file size limit reached
#define EC_MC_MPEGENC_4GB_LIMIT          EC_MC_MPEGENC_BASE + 0x2  // 4GB limit reached
#define EC_MC_MPEGENC_OUT_OF_DISKSPACE   EC_MC_MPEGENC_BASE + 0x3  // out of diskspace
#define EC_MC_MPEGENC_TIME_LIMIT         EC_MC_MPEGENC_BASE + 0x4  // capture time limit reached       
#define EC_MC_MPEGENC_WRITE_ERROR        EC_MC_MPEGENC_BASE + 0x6  // write or file creation error 
#define EC_MC_MPEGENC_FILE_SWAP					 EC_MC_MPEGENC_BASE + 0x7	 // actual file closed, going to open new file 
																																	 // sent when in multiple file mode


///////////////////////////////////////////////

#define MFSETTING_PAGEDISPLAYMODE         0x0050 // send display mode for property page
    #define MFS_PAGEDISPLAYMODE_AUTO 0x0000      //   let the filter decide
    #define MFS_PAGEDISPLAYMODE_ON   0x0001      //   always enable all entries
    #define MFS_PAGEDISPLAYMODE_OFF  0x0002      //   always disable all entries

#define MFSETTING_GET_RC                  0x0070 // get_rc func for low level muxer
    // setting is a pointer to the get_rc function
    //   or zero to let the filter use it's own (default)


#define MFSETTING_OUTPUTPIN_CONNECTED     0x8000 // muxer output pin connected, bool, read only



///////////////////////////////////////////////////

#define MFSETTING_TARGET_BUFSTREAM		0x0060	// External bufstream to deliver data to
																							// instead of output to file or output pin
                                              // setting is a pointer to an MF_EXT_BUFSTREAM_INFO struct.
																							

  // type of header to precede each packet with delivered to external bufstream
  typedef enum 
  {
	  MF_TYPE_MPEG_NET_HEADER = 1, // header is of type MC_MPEG_NET_STREAM_HEADER (see mpeg_net.h)

  }MF_EXT_BUFSTREAM_HEADERS;

  // setup info of an external output bufstream
  typedef struct tagMF_EXT_BUFSTREAM_INFO
  {
	  DWORD dwSize;								// size of this struct, too keep it extendable
	  DWORD lpdwBufStream;				// pointer to the external target bufstream
	  DWORD dwPacketSize;					// max. size of output packets including header data if any. A multiple of 188 for mpeg_net
	  DWORD dwHeaderType;					// one of MF_EXT_BUFSTREAM_HEADERS or 0
		DWORD dwHeaderSize;					// size of header data to preceede each packet with, or 0
	  DWORD lpdwHeaderData;				// header data to preceede each packet with or NULL
		DWORD dwPacketID;						// packet ID to set in header
  }MF_EXT_BUFSTREAM_INFO, *PMF_EXT_BUFSTREAM_INFO;

  // For all in one encoder/muxer in case of header type MF_TYPE_MPEG_NET_HEADER dwHeaderSize and
	// lpHeaderData may be NULL. 
	// The encoder then will generate the header data from its settings

/////////////////////////////////////////////////////
  

#define __IMCMpegMuxerFilter__
#endif //__IMCMpegMuxerFilter__



//////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////////
#endif // _IMCEPROP_H_
