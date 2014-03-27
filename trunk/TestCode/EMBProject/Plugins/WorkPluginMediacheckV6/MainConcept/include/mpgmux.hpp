//------------------------------------------------------------------------------
// File: mpgmux.hpp
//
// Desc: MPEG muxer API declaration
//
// Copyright (c) 2000-2007, MainConcept AG.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef _MPGMUX_HPP_
#define _MPGMUX_HPP_

#include "bufstrm.hpp"
#include "mpegdef.h"
#include "mcprofile.h"

typedef struct mpeg_muxer mpegmuxer_tt;		// muxer object


// maximum stream of each type that can be added
#define MAX_VIDEO_STREAMS	8
#define MAX_AUDIO_STREAMS	8
#define MAX_SUBPIC_STREAMS	32


// flags for the mpeg_m_settings.feature_flags field
#define MUX_FEATURE_TS_ATS				0x00000001	// add 4-byte arrival time stamp prefix to all transport stream packets
													// only the lower 30 bits of the 4 bytes are used
													// the upper 2 bits are set to 0
#define MUX_FEATURE_TS_NO_PSI			0x00000002	// no transport stream Program Specific Information (PAT, PMT, etc.)
													// is placed in the transport stream
#define MUX_FEATURE_TS_AU_INFO			0x00000004	// AU_information private data as defined by ETSI TS 101 154 V1.7.1
													// is placed in the transport stream
#define MUX_FEATURE_TS_PCR_IFRAMES		0x00000008	// PCR is placed in transport stream packets that contain the start
													// of an I or IDR frame
#define MUX_FEATURE_TS_PCR_ALL_FRAMES	0x00000010	// PCR is placed in transport stream packets that contain the start
													// of any frame

// flags for the stream_settings.flags field
#define STREAM_FLAG_STILL_PICTURES		0x00000001	// video stream contains still images, I frames only!
#define STREAM_FLAG_VCD_VBV_UPDATE		0x00000002	// only use with a single I frame, STREAM_FLAG_STILL_PICTURES
													// must also be present. The muxer will update the VBV in the
													// seq hdr to the length of the pic rounded up to a multiple
													// of 2048 bytes (as well as the P-STD buffer).
#define STREAM_FLAG_SVCD_STILL_OFFSET	0x00000004	// put a SVCD scan offset in the still picture
													// STREAM_FLAG_STILL_PICTURES must also be present
#define STREAM_FLAG_SUBPIC_WITH_PTS		0x00000008	// each subpic is prefixed by an 8 byte (int64_t) PTS value, file mux only!
#define STREAM_FLAG_H264_AU_DELIMITER	0x00000010	// put an AU delimiter before each H.264 frame
#define STREAM_FLAG_STOP_AT_EOF			0x00000020	// stop muxing when this file ends, file muxing only, video or audio only

#define STREAM_FLAG_VIDEO_STREAM		0x00010000	// stream is a video stream
#define STREAM_FLAG_AUDIO_STREAM		0x00020000	// stream is an audio stream
#define STREAM_FLAG_SUBPIC_STREAM		0x00040000	// stream is a subpicture stream


// flags for the extended_m_settings.flags field
#define EXT_FLAG_EMPTY_SECTORS			0x00000001	// make empty sectors instead of padding sectors, VCD only
#define EXT_OUTPUT_DATA_FOR_XML			0x00000002	// output video_seq_info_xml and video_au_info_xml data
#define EXT_USE_PROGRAM_NUMBER			0x00000010	// the program_number field is valid
#define EXT_USE_NETWORK_ID				0x00000020	// the network_id field is valid
#define EXT_USE_TRANSPORT_ID			0x00000040	// the transport_id field is valid
#define EXT_PAD_PES_EXTENSION			0x00000080	// add 3 PES padding bytes to reserve space for a PES
													// extension in the first packet of each stream in a VOBU
													// mplex_type must be one of the DVD types!
#define EXT_AUDIO_NO_SPAN_VOBU			0x00000100	// do not let an audio frame span a VOBU
													// boundary. mplex_type must be one of the DVD types
#define EXT_PTS_START_OFFSETS			0x00000200	// streaming input PTS's start with offset, 
													// adjust SCR start accordingly, leave audio PTS untouched
#define EXT_WARN_ON_LOW_MUXRATE			0x00000400	// call wrn_printf (instead of calling inf_printf,
													// if the user mux rate is computed to be too low
#define EXT_ERR_ON_LOW_MUXRATE			0x00000800	// call err_printf and abort (instead of calling inf_printf and continuing),
													// if the user mux rate is computed to be too low
#define EXT_PRESERVE_TIMESTAMPS			0x00001000	// use the streaming input timestamps without modification


enum MuxStatus
{
	MUX_OK			= 0,
	MUX_NOVID		= 1,
	MUX_NOAUD1		= 2,
	MUX_NOAUD2		= 4,
	MUX_NOTHINGTODO	= 8,
	MUX_ERR			= 16,
	MUX_NOSP		= 32
};


#ifdef __GNUC__
	#pragma pack(push,1)
#else
	#pragma pack(push)
	#pragma pack(1)
#endif


struct dash_vr_settings
{
	unsigned char dci_cci_ss;
	unsigned char dci;
	unsigned char cci;
	unsigned char mnf_id[32];
	unsigned char mnfi_dt[1944];
};


struct stream_settings
{
	unsigned char id;			// stream ID, 0 = automatically computed
	unsigned char sub_id;		// substream ID, 0 = automatically computed
	int PID;					// PID, 0 = automatically computed
	int buffer_size;			// P-STD buffer size, 0 = automatically computed
	int delay_ms;				// stream delay, 0 = automatically computed
	int flags;					// flags, one or more of the STREAM_FLAGS_ defines above
								// flags MUST contain one and only one of:
								// STREAM_FLAG_VIDEO_STREAM, STREAM_FLAG_AUDIO_STREAM or STREAM_FLAG_SUBPIC_STREAM
	double h264_frame_rate;		// H264 frame rate, only used by mpegOutMuxAddFile for H264 video streams

	int stream_fifo_size;			// the size of the fifo in bytes being used for this stream, only used
								// by mpegOutMuxAddStream. If 0, the muxer will use a fixed size
								// for its internal fifo, else the muxer will compute an appropriate
								// internal fifo size based on this value

	int still_picture_duration;	// duration of still pictures if the STREAM_FLAG_STILL_PICTURES flags
								// is present. The value is the number of frames to display each picture
	unsigned char lang_code[3];	// ISO 639-2 language code of the audio stream
	
	unsigned char reserved[45];
};


struct ts_program_settings
{
	uint32_t reserved1; 
	uint16_t reserved2;
	uint16_t reserved3;
	uint32_t reserved4;
	
	// currently put in both the PMT and any EIT tables
	uint32_t descriptor_len;
	uint8_t *descriptor;

	uint8_t reserved5[64];
};


struct extended_m_settings
{
	unsigned int flags;						// flags, one or more of the EXT_FLAG_ defines above

	// transport stream items
	int NIT_PID;							// network information section PID, 0 = automatically computed
	int SDT_PID;							// service description section PID, 0 = automatically computed
	int EIT_PID;							// event information section PID, 0 = automatically computed
	int TDT_PID;							// time and date section PID, 0 = automatically computed
	int PMT_PID;							// program map section PID, 0 = automatically computed
	int PCR_PID;							// PID the PCR is placed on, 0 = automatically computed
	int PAT_LIMIT;							// time (in ms) before the PAT and other tables are repeated, 0 = automatically computed
	int PCR_LIMIT;							// upper limit (in ms) of the PCR repeat rate, 0 = automatically computed

	int program_number;						// transport stream program number
	int network_id;							// transport stream network id
	int transport_id;						// transport stream id
	
	dash_vr_settings *dvr_set;				// DVD-VR settings

	int num_programs;						// number of ts_program_settings structures in the program_settings ptr
											// currently only 1 is supported
	ts_program_settings *program_settings;	// an array of one or more ts_program_settings structures

	unsigned char reserved[40];
};


struct mpeg_m_settings
{
	int sectors_delay;				// time stamp of first mpeg pack
	int video_delay_ms;				// default time stamp of the first video packet for all video streams, 0 = automatically computed
	int audio1_delay_ms;			// default time stamp of the first audio packet for all audio streams, 0 = automatically computed
	int audio2_delay_ms;			// not used
	int sector_size;				// size of the mpeg packs
	int packets_per_pack;			// number of PES packets per pack
	int audio1_buffer_size;			// default P-STD audio buffer size for all audio streams, 0 = automatically computed
	int audio2_buffer_size;			// not used
	int video_buffer_size;			// default P-STD video buffer size for all video streams, 0 = automatically computed
	int extra_padding_packs;		// extra padding packets per second for transport streams only
	int user_mux_rate_bps;			// force a particular mux rate (in bps)
	int mplex_type;					// type of program stream, one of the MPEG_ defines in mpegdef.h 
	int mplex_pulldown_flag;		// video stream is 23.976fps converted to 29.97fps or 24fps converted to 30fps
	int vcd_audio_pad;				// pad each vcd audio sector with 20 zeros 
	int align_sequence_headers;		// align sequence headers to the start of a PES packet 
	int user_mux_rate;				// force a particular mux rate (in 50 byte/sec units)
	int put_private2;				// put DVD private stream 2's instead of padding packets
	int frame_timestamps;			// which frames to timestamp 
	int VBR_multiplex;				// do VBR style multiplexing
	int write_pec;					// write a program end code 
	int mux_SVCD_scan_offsets;		// compute SVCD scan offsets 
	int max_file_size;				// split file every so many MB 
	unsigned int feature_flags;		// one or more of the MUX_FEATURE_* defines
	int reserved3;					// 
	int reset_clocks;				// reset the clocks in split files 
	int write_end_codes;			// write program end codes in split files 
	int set_broken_link;			// set the broken link flag in the first GOP of split files 
};

#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif

// call to fill an mpeg_m_settings structure with defaults values
// based on one of the SDK profile ID's
// 
//	inputs:
//		set - pointer to an mpeg_m_settings structure
//		video_type - one of the MPEG_* constants
//	output:
//		modified mpeg_m_settings structure
//	return:
//		none

char *mpegOutMuxDefaults(struct mpeg_m_settings *set, int profileID);


// call to create an mpeg muxer object
//
//	inputs:
//		get_rc - pointer to a get resource function 
//		set - pointer to a filled in mpeg_m_settings structure
//	outputs:
//		none
//	return:
//		pointer to a mpegmuxer_tt object if succesful
//		NULL if unsuccesful

mpegmuxer_tt * mpegOutMuxNew(void *(*get_rc)(char* name), const struct mpeg_m_settings *set);


// call to initialize the muxer for a streaming mode muxing session,
// the audio/video data comes from audio and video bufstream
// objects passed to mpegOutMuxAddStream
//
//	inputs:
//		muxer - pointer to an mpeg muxer object
//		output - pointer to a bufstream_tt object for the output muxed data
//		ext_set - pointer to an extended_m_settings structure, can be NULL
//	outputs:
//		none
//	return:
//		MUX_OK if successful
//		MUX_ERR if unsuccesful

int mpegOutMuxInitStream(mpegmuxer_tt *muxer, extended_m_settings *ext_set, bufstream_tt *output);


// call to add a video, audio or subpic stream to the muxer in streaming mode,
//
//	inputs:
//		muxer - pointer to an mpeg muxer object
//		set - pointer to a stream_settings structure
//		input - pointer to a bufstream_tt object for the input data
//	outputs:
//		none
//	return:
//		MUX_OK if successful
//		MUX_ERR if unsuccesful

int mpegOutMuxAddStream(mpegmuxer_tt *muxer, stream_settings *set, bufstream_tt *input);


// call to initialize the muxer for a file mode muxing session,
// the audio/video data comes from files added via mpegOutMuxAddFile
//
//	inputs:
//		muxer - pointer to an mpeg muxer object
//		output - pointer to an output filename or bufstream for the output muxed data
//		ext_set - pointer to an extended_m_settings structure, can be NULL
//	outputs:
//		none
//	return:
//		MUX_OK if successful
//		MUX_ERR if unsuccesful

int mpegOutMuxInitFile(mpegmuxer_tt *muxer, extended_m_settings *ext_set, char *output);
int mpegOutMuxInitFileBS(mpegmuxer_tt *muxer, extended_m_settings *ext_set, bufstream_tt *output);


// call to add a video, audio or subpic stream to the muxer in file mode,
//
//	inputs:
//		muxer - pointer to an mpeg muxer object
//		set - pointer to a stream_settings structure
//		input - pointer to a filename for the input data
//		input1, input2 - input bufstreams, same file opened twice
//		video_bitrate - bitrate (in bits/sec) of the video stream, only needed for MPEG-1 VBR or H264 video
//						also used for AAC audio streams!
//	outputs:
//		none
//	return:
//		MUX_OK if successful
//		MUX_ERR if unsuccesful

int mpegOutMuxAddFile(mpegmuxer_tt *muxer, stream_settings *set, char *input, int video_bitrate);
int mpegOutMuxAddFileBS(mpegmuxer_tt *muxer, stream_settings *set, bufstream_tt *input1, bufstream_tt *input2, int video_bitrate);


// call to do the multiplexing in file mode
// 
//	inputs:
//		muxer -  pointer to an mpeg muxer object
//	outputs:
//		muxed data to output file
//	return:
//		MUX_OK if succesful
//		MUX_ERR if unsuccesful

unsigned long mpegOutMux(mpegmuxer_tt *muxer);


// call to finish a muxing session, set abort non-zero if muxing is being
// aborted.
//
//	inputs:
//		muxer - pointer to an mpeg muxer object
//		abort - set to 0 to finish any leftover muxing and clean up,
//				else just clean up
//	outputs:
//		muxed data to output file if needed
//	return:
//		MUX_OK if successful
//		MUX_ERR if unsuccesful

int mpegOutMuxDone(mpegmuxer_tt *muxer, int abort);


// call to free an mpeg muxer object
//
//	inputs:
//		muxer - muxer object to free
//	outputs:
//		none
//	return:
//		none

void mpegOutMuxFree(mpegmuxer_tt *muxer);


// call to get the setting errors/warnings in an mpeg_m_settings structure
// use with the get_rc callback to provide an err_printf callback to get
// error messages that can be localized
//
//	inputs:
//		get_rc - pointer to a get_rc function
//		set - pointer to an mpeg_m_settings structure
//		options - check options, one or more of the CHECK_* defines in mpegdef.h
//		app - reserved
//	outputs:
//		none
//	return:
//		MUX_OK if no errors found
//		one of the INV_* error codes if an error is found

int mpegOutMuxChkSettings(void *(*get_rc)(char *name), const struct mpeg_m_settings *set, unsigned int options, void *app);


// call to send messages to the muxer
//
//	inputs:
//		muxer -  pointer to an mpeg muxer object
//		offs - value is dependent on the message
//		info_ID - the message to process
//		info_ptr - pointer, dependent on the message
//		info_size - size of the structure pointed to by info_ptr
//	outputs:
//		none
//	return:
//		none

void mpegOutMuxAuxinfo(mpegmuxer_tt *muxer, unsigned long offs, unsigned long info_ID, void *info_ptr, unsigned long info_size);



#ifdef __cplusplus
}
#endif

#endif // _MPGMUX_HPP_
