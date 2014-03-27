//------------------------------------------------------------------------------
// File: mxfdmux.h
//
// Desc: mxf demuxer header
//
// Copyright (c) 2000-2006, MainConcept AG.  All rights reserved.
//--------------------------------------------------------------------------

#ifndef _MXFDMUX_H_
#define _MXFDMUX_H_

#include "mctypes.h"
#include "mpegdef.h"
#include "bufstrm.hpp"


// sdk_stream_type defines, general stream types reported
#define MXFDMUX_MPEG_VIDEO				0x01	// MPEG video
#define MXFDMUX_AES_331M_AUDIO			0x02	// AES 331M PCM audio
#define MXFDMUX_AES_382M_AUDIO			0x03	// AES 382M PCM audio


// called to seek in the source file in splitter mode
// position - file position to seek to
// return 0 if OK, 1 if fault
typedef int (*mxf_splitter_seek_callback)(void* split_app_ptr, int64_t position);

// called to read data in splitter file mode
// read bufferSize bytes from file into buffer
// return number of bytes read, 0 if fault
typedef int (*mxf_splitter_read_callback)(void* split_app_ptr, uint8_t *buffer, int bufferSize);


// external i/o function prototypes
// note: if these are used, the mpgdmux_settings.file_length field MUST be filled in
//       with the correct file size by the app
// called to open a file
// and the return value will be passed back to the seek, read and close functions
// ext_app_ptr - the ext_app_ptr field in the mpgdmux_settings structure is passed back here
// fileName - if a filename was passed to mpgdmux, it is returned here
// return the file handle as a long or -1 if an error occurs
typedef long (*mxf_external_open_callback)(void *ext_app_ptr, char *fileName);

// called to seek in a file
// ext_app_ptr - the ext_app_ptr field in the mpgdmux_settings structure is passed back here
// fileHandle - a file handle returned by the fn_external_open_callback function
// position - the position to seek to
// return 0 if Ok, 1 if an error occurs
typedef int (*mxf_external_seek_callback)(void *ext_app_ptr, long fileHandle, int64_t position);

// called to read from a file
// appData - the ext_app_ptr field in the mpgdmux_settings structure is passed back here
// fileHandle - a file handle returned by the fn_external_open_callback function
// buffer - a buffer for the data
// bufferSize - the number of bytes to read
// return the number of bytes actually read or 0 if an error occurs
typedef int (*mxf_external_read_callback)(void *ext_app_ptr, long fileHandle, uint8_t *buffer, int bufferSize);

// called to close a file
// appData - the ext_app_ptr field in the mpgdmux_settings structure is passed back here
// fileHandle - a file handle returned by the fn_external_open_callback function
typedef void (*mxf_external_close_callback)(void *ext_app_ptr, long fileHandle);


// demuxer instance
typedef struct mxf_demuxer mxfdmux_tt;


#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

struct mxfdmux_settings
{
	int find_stream_limit;

	// splitter usage
	int splitter_mode;
	mxf_splitter_seek_callback splitter_seek_callback;
	mxf_splitter_read_callback splitter_read_callback;
	void *split_app_ptr;

	// external IO stuff
	int use_external_io;
	mxf_external_open_callback external_open_callback;
	mxf_external_seek_callback external_seek_callback;
	mxf_external_read_callback external_read_callback;
	mxf_external_close_callback external_close_callback;
	void *ext_app_ptr;

	int64_t file_length;

	unsigned char reserved[64];
};


struct mxfdmux_parser_settings
{
	int requested_buffer_size;

	unsigned char reserved[64];
};


struct mxfdmux_stream_info
{
	int sdk_stream_type;				// stream type using SDK defines
	unsigned int ID;
	int aes3_sample_size;
	int aes3_sample_rate;
	int aes3_channels;
	int ES_info_length;					// length of the descriptors below
	unsigned char *descriptors;

	unsigned char reserved[64];
};


struct mxfdmux_file_info
{
	int stream_count;					// number of elementary streams in this file
	int edit_unit_size;
	int64_t duration;
	int file_type;
	int64_t file_size;

	unsigned int index_num_entries;
	unsigned int index_entry_len;
	unsigned char *index_table;

	int file_info_length;				// length of the descriptors below
	unsigned char *descriptors;

	unsigned char reserved[64];
};


struct mxfdmux_stream_settings
{
	int stream_num;
	bufstream_tt *bs;

	unsigned char reserved[64];
};

#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif


// call to create and initialize an demuxer instance
//
//  inputs:
//    get_rc_ex - a pointer to a get_rc_ex resource function
//    rc_app_ptr - an application supplied pointer that will be passed back with any get_rc calls
//    set - a pointer to an mxfdmux_settings structure
//
// outputs:
//    none
//
//  return:
//    a pointer to a demuxer instance if successful, else NULL

mxfdmux_tt *mxfDemuxNew(void *(*get_rc_ex)(void *rc_app_ptr, char* name), void *rc_app_ptr, mxfdmux_settings *set);


// call to free a demuxer instance
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//
// outputs:
//    none
//
//  return:
//    none

void mxfDemuxFree(mxfdmux_tt *demuxer);


// call to open a file for demuxing
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    reserved - reserved
//    filename - a pointer to a filename to open
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mxfDemuxOpen(mxfdmux_tt *demuxer, void *reserved, char *filename);

// unicode version
#if !defined(__APPLE__) && !defined(__linux__) && !defined(__vxworks)

int mxfDemuxOpenW(mxfdmux_tt *demuxer, void *reserved, wchar_t *filename);

#else

// this version will currently return an error!
int mxfDemuxOpenW(mxfdmux_tt *demuxer, void *reserved, unsigned short *filename);

#endif


// call to close file
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//
// outputs:
//    none
//
//  return:
//    none

void mxfDemuxClose(mxfdmux_tt *demuxer);


// call to get information about the input file
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    file_info - a pointer to a file_info_struct
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mxfDemuxGetFileInfo(mxfdmux_tt *demuxer, mxfdmux_file_info *file_info);


// call to get information about the specified stream in the file
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    stream_info - a pointer to a stream_info_struct
//    program_num - a zero-based program number
//    stream_num - a zero-based stream number
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mxfDemuxGetStreamInfo(mxfdmux_tt *demuxer, mxfdmux_stream_info *stream_info, int stream_num);


// call to create and initialize a parser instance
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    set - a pointer to a mxfdmux_parser_settings structure
//
// outputs:
//    none
//
//  return:
//    a non-zero parser number if successful, else 0

int mxfDemuxNewParser(mxfdmux_tt *demuxer, mxfdmux_parser_settings *set);


// call to free a parser instance
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//
// outputs:
//    none
//
//  return:
//    none

void mxfDemuxFreeParser(mxfdmux_tt *demuxer, int parser_num);


// call to add an output bufstream for the specified stream
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//    set - a pointer to an mxfdmux_stream_settings structure
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mxfDemuxAddStream(mxfdmux_tt *demuxer, int parser_num, mxfdmux_stream_settings *set);


// call to remove an output bufstream
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//    set - a pointer to an mxfdmux_stream_settings structure
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mxfDemuxRemoveStream(mxfdmux_tt *demuxer, int parser_num, mxfdmux_stream_settings *set);


// call to seek to the specified file position
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//    file_pos - the position to seek to
//
// outputs:
//    none
//
//  return:
//    0 if successful, 2 if EOF, 1 if an error occurs

int mxfDemuxSeek(mxfdmux_tt *demuxer, int parser_num, int64_t file_pos);


// call to push a packet through the parser
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//
// outputs:
//    none
//
//  return:
//    0 if successful, 2 if EOF, 1 if an error occurs

int mxfDemuxPush(mxfdmux_tt *demuxer, int parser_num);


// call to re-open a parser
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mxfDemuxWakeParser(mxfdmux_tt *demuxer, int parser_num);


// call to close the actual file of a parser, the parser
// is left intact and can be re-opened with the wake function
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mxfDemuxSleepParser(mxfdmux_tt *demuxer, int parser_num);


// call to have the file size updated if it is getting bigger
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//    file_size - the new file size if splitter or external i/o mode, else unused
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mxfDemuxUpdateFilesize(mxfdmux_tt *demuxer, int parser_num, int64_t file_size);


// call to have the parser perform a seek before the next read
// and optionally update the internal app_data ptr
//
//  inputs:
//    demuxer - a pointer to a demuxer instance
//    parser_num - a parser number
//    split_app_ptr - can be used to updated the split_app_ptr from the mxfdmux_settings structure
//
// outputs:
//    none
//
//  return:
//    0 if successful, else non-zero

int mxfDemuxSetFirstSplitRead(mxfdmux_tt *demuxer, int parser_num, void *split_app_ptr);


#ifdef __cplusplus
}
#endif

#endif // _mxfdmux_h_
