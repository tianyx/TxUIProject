//------------------------------------------------------------------------------
// File: mpg_dlg.hpp
//
// Desc: mpg_dlg's new API header
//       allow the user to change video, audio and multiplexing settings
//
// Copyright (c) 2000-2007, MainConcept AG.  All rights reserved.
//--------------------------------------------------------------------------

#ifndef _MPG_DLG_H_
#define _MPG_DLG_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include "mpgcheck.hpp"

// pages available
#define SETTINGS_BASIC            0x00000001  // display the basic settings page
#define SETTINGS_VIDEO            0x00000002  // display the video settings page
#define SETTINGS_ADVVIDEO         0x00000004  // display the advanced video settings page
#define SETTINGS_AUDIO            0x00000008  // display the audio settings page
#define SETTINGS_MULTIPLEX        0x00000010  // display the multiplexing page
#define SETTINGS_ALL              0x0000001F  // display all of the above pages

// dialog options
#define SETTINGS_LOADSAVE         0x00000020  // display the load/save buttons
#define SETTINGS_NOINITCHECK      0x00000040  // do not check the validity of the setting at function start
#define SETTINGS_DISABLENONE      0x00000080  // disable any 'none' selections in combo boxes

// extended dialog options
#define SETTINGS_REDWARNINGS      0x00000100  // display any summary warnings in red (default is yellow)
#define SETTINGS_FRAMESIZE        0x00000200  // display the frame size controls and load/save the
                                            // frame size settings from/to INI files
#define SETTINGS_FREQSTEREO       0x00000400  // display the audio rate and PCM stereo controls
                                            // and load/save the audio frequency and PCM stereo
                                            // settings from/to INI files
#define SETTINGS_FIELDS           0x00000800  // display the fields control
#define SETTINGS_DEINTERLACE      0x00001000  // display the deinterlace control
#define SETTINGS_HIFRAMERATES     0x00002000  // display frame rates > 30 fps
#define SETTINGS_PRESETS          0x00004000  // display the presets control
#define SETTINGS_MUXAUDIO2        0x00008000  // no longer used!!
#define SETTINGS_CLOSEDDVDGOPS    0x00010000  // set closed_GOPs to 1 if the settings are set to the DVD defaults
#define SETTINGS_DROPFRAME        0x00020000  // set drop frame frame rate if the settings are set to any NTSC defaults
#define SETTINGS_USEFONT          0x00040000  // use the font passed in the logicalFont for the dialogs
#define SETTINGS_NOBANNER         0x00080000  // do not display the banner
#define SETTINGS_TWOPASS          0x00100000  // display the 2-pass VBR checkbox
#define SETTINGS_SDTVLIMITS       0x00200000  // limit frame rates/resolutions to SDTV (720x576@30fps max)
#define SETTINGS_NOPCMAUDIO       0x00400000  // do not allow PCM audio to be selected
#define SETTINGS_PALDEFAULTS      0x00800000  // use PAL for any defaults
#define SETTINGS_NOAC3AUDIO       0x01000000  // no longer used!!
#define SETTINGS_NOMXFMUXTYPES    0x02000000  // do not show MXF multiplex types (XDCAM-HD, XDCAM-IMX)
#define SETTINGS_NOAVCHDMUXTYPE   0x04000000  // do not show the AVCHD multiplex type


#define settingsErrNone         0
#define settingsCancel          1
#define settingsError           2
#define settingsInvSettings     3

// string useds by the mpegOutDialogGetSettings and 
// mpegOutDialogPutSettings functions and when loading/saving
// INI files

// basic settings
#define STR_BASIC_TEXT    "basic"
#define STR_MPEGTYPE      "mpegType"
#define STR_MOTIONTYPE    "motionSearchType"
#define STR_ONLINE        "online"
#define STR_PERFLEVEL     "perfLevel"
#define STR_RELAXEDSTD    "relaxed_standards"
#define STR_DEINTMODE     "deinterlacing_mode"

// video settings
#define STR_VIDEO_TEXT    "video"
#define STR_RATECONTROL   "rateControlMode"
#define STR_VPULLDOWN     "videoPulldownFlag"
#define STR_ASPECTRATIO   "aspectRatio"
#define STR_FRAMECODE     "frameRateCode"
#define STR_IFRAMEINGOP   "iFramesInGOP"
#define STR_IPFRAME       "ipFrameDistance"
#define STR_BITRATEFLAG   "bitrateFlag"
#define STR_MQUANTVAL     "mquantValue"
#define STR_CONSTBITRATE  "constBitrate"
#define STR_MAXBITRATE    "maxBitrate"
#define STR_AVGBITRATE    "avgBitrate"
#define STR_MINBITRATE    "minBitrate"
#define STR_TIMECODE0     "timeCode0"
#define STR_DROPFRAMETC   "dropFrameTC"
#define STR_CONSTRPARMS   "constrParms"
#define STR_CLOSEDGOPS    "closedGOPs"

// audio settings
#define STR_AUDIO_TEXT    "audio"
#define STR_AUDIOLAYER    "audioLayer"
#define STR_AUDIOMODE     "audioMode"
#define STR_DEEMPHASIS    "deEmphasis"
#define STR_AUDIOBITRATE  "audioBitrate"
#define STR_PRIVATEBIT    "privateBit"
#define STR_COPYRIGHTBIT  "copyrightBit"
#define STR_ORIGINALBIT   "originalBit"
#define STR_ERRORPROT     "errorProtection"
#define STR_PSYCHMODEL    "psychModel"

// muxer settings
#define STR_MUX_TEXT       "multiplex"
#define STR_MUXTYPE        "programStreamType"
#define STR_VBRMUX         "vbrMultiplex"
#define STR_PADVCDAUDIO    "padVCDAudio"
#define STR_WRITEENDCODE   "writeProgramEndCode"
#define STR_MUXSVCDOFF     "muxSVCDScanOffsets"
#define STR_ALIGNSEQ       "alignSequenceHeaders"
#define STR_USEPRIVATE2    "usePrivateStream2"
#define STR_USECOMPBRATE   "useComputedBitrate"  // do not use anymore!!
#define STR_USERMUXRATEBPS "userMuxRateBPS"      // use this one instead
#define STR_FRAMETSTAMPS   "frameTimestamps"
#define STR_MUXPULLDOWN    "muxPulldownFlag"
#define STR_USERMUXRATE    "userMuxRate"
#define STR_SECTORSIZE     "sectorSize"
#define STR_PACKETSPACK    "packetsPerPack"
#define STR_VIDEOBSIZE     "videoBufferSize"
#define STR_AUDIOBSIZE     "audioBufferSize"
#define STR_AUDIO1BSIZE    "audio1BufferSize"
#define STR_SECTORSDELAY   "sectorDelay"
#define STR_VIDEODELAY     "videoDelay"
#define STR_AUDIODELAY     "audioDelay"
#define STR_AUDIO1DELAY    "audio1Delay"
#define STR_ALWAYSSYS      "alwaysWriteSysHeader"  // do not use anymore!!
#define STR_EXTRAPADDING   "extraPadding"
#define STR_MAXFILESIZE    "maxFileSize"
#define STR_MUXSTARTTIME   "muxStartTime"          // do not use anymore!!
#define STR_MUXSTOPTIME    "muxStopTime"           // do not use anymore!!
#define STR_RESETCLOCKS    "resetClocks"
#define STR_SETBROKEN      "setBrokenLink"
#define STR_WRITEEND       "writeEndCodes"

// advanced video settings
#define STR_ADV_TEXT      "advVideo"
#define STR_PROFILE       "profile"
#define STR_LEVEL         "level"
#define STR_CHROMAFORMAT  "chromaFormat"
#define STR_CPRIMARIES    "colorPrimaries"
#define STR_TRANSFERCHAR  "transferCharacteristics"
#define STR_MATRIXCOEFF   "matrixCoefficients"
#define STR_VIDEOFORMAT   "videoFormat"
#define STR_INTRADC       "intraDCPrec"
#define STR_VBVSIZE       "vbvBufferSize"
#define STR_FORCEVBV      "forceVBVDelay"
#define STR_DHORZSIZE     "displayHorizontalSize"
#define STR_DVERTSIZE     "displayVerticalSize"
#define STR_PROGSEQ       "progressiveSequence"
#define STR_PROGFRAME     "progressiveFrame"
#define STR_FIELDPICS     "fieldPictures"
#define STR_TOPFIELD      "topFieldFirst"
#define STR_REPEATFIELD   "repeatFirstField"
#define STR_INTRAVLCI     "intraVLCFormatI"
#define STR_INTRAVLCP     "intraVLCFormatP"
#define STR_INTRAVLCB     "intraVLCFormatB"
#define STR_FRAMEPREDI    "framePredDCTI"
#define STR_FRAMEPREDP    "framePredDCTP"
#define STR_FRAMEPREDB    "framePredDCTB"
#define STR_QUANTSCALEI   "quantizationScaleI"
#define STR_QUANTSCALEP   "quantizationScaleP"
#define STR_QUANTSCALEB   "quantizationScaleB"
#define STR_ALTSCANI      "alternateScanI"
#define STR_ALTSCANP      "alternateScanP"
#define STR_ALTSCANB      "alternateScanB"
#define STR_WRITESEQDISP  "writeSequenceDisplayExt"
#define STR_WRITESEQGOP   "writeSequenceEveryGOP"
#define STR_WRITESEQEND   "writeSequenceEndCode"
#define STR_EMBEDSVCD     "embedSVCDUserBlocks"
#define STR_WRITEPICDISP  "writePictureDisplayExt"
#define STR_FRAMEHORZ     "frameCentreHorizontalOffset"
#define STR_FRAMEVERT     "frameCentreVerticalOffset"
#define STR_QTNOISE       "qtNoiseSensitivity"
#define STR_FEATUREFLAGS  "featureFlags"
#define STR_REACTIONPARM  "reactionParameter"
#define STR_AVGACTIVITY   "initialAverageActivity"
#define STR_ICOMPLEXITY   "initialIComplexity"
#define STR_PCOMPLEXITY   "initialPComplexity"
#define STR_BCOMPLEXITY   "initialBComplexity"
#define STR_IFULLNESS     "initialIFullness"
#define STR_PFULLNESS     "initialPFullness"
#define STR_BFULLNESS     "initialBFullness"
#define STR_MINPERCENT    "minFramePercentage"
#define STR_PADPERCENT    "padFramePercentage"
#define STR_AUTOVECTOR    "autoVectorLengths"
#define STR_HORZPEL       "horzPelMovement"
#define STR_VERTPEL       "vertPelMovement"
#define STR_FORWHFCODEP   "forwHorzFCodeP"
#define STR_FORWVFCODEP   "forwVertFCodeP"
#define STR_FORWHSEARCHP  "forwHorzSearchP"
#define STR_FORWVSEARCHP  "forwVertSearchP"

#define STR_FORWHFCODEB1  "forwHorzFCodeB1"
#define STR_FORWVFCODEB1  "forwVertFCodeB1"
#define STR_FORWHSEARCHB1 "forwHorzSearchB1"
#define STR_FORWVSEARCHB1 "forwVertSearchB1"
#define STR_BACKHFCODEB1  "backHorzFCodeB1"
#define STR_BACKVFCODEB1  "backVertFCodeB1"
#define STR_BACKHSEARCHB1 "backHorzSearchB1"
#define STR_BACKVSEARCHB1 "backVertSearchB1"

#define STR_FORWHFCODEB2  "forwHorzFCodeB2"
#define STR_FORWVFCODEB2  "forwVertFCodeB2"
#define STR_FORWHSEARCHB2 "forwHorzSearchB2"
#define STR_FORWVSEARCHB2 "forwVertSearchB2"
#define STR_BACKHFCODEB2  "backHorzFCodeB2"
#define STR_BACKVFCODEB2  "backVertFCodeB2"
#define STR_BACKHSEARCHB2 "backHorzSearchB2"
#define STR_BACKVSEARCHB2 "backVertSearchB2"

#define STR_FORWHFCODEB3  "forwHorzFCodeB3"
#define STR_FORWVFCODEB3  "forwVertFCodeB3"
#define STR_FORWHSEARCHB3 "forwHorzSearchB3"
#define STR_FORWVSEARCHB3 "forwVertSearchB3"
#define STR_BACKHFCODEB3  "backHorzFCodeB3"
#define STR_BACKVFCODEB3  "backVertFCodeB3"
#define STR_BACKHSEARCHB3 "backHorzSearchB3"
#define STR_BACKVSEARCHB3 "backVertSearchB3"

#define STR_FORWHFCODEB4  "forwHorzFCodeB4"
#define STR_FORWVFCODEB4  "forwVertFCodeB4"
#define STR_FORWHSEARCHB4 "forwHorzSearchB4"
#define STR_FORWVSEARCHB4 "forwVertSearchB4"
#define STR_BACKHFCODEB4  "backHorzFCodeB4"
#define STR_BACKVFCODEB4  "backVertFCodeB4"
#define STR_BACKHSEARCHB4 "backHorzSearchB4"
#define STR_BACKVSEARCHB4 "backVertSearchB4"

#define STR_FORWHFCODEB5  "forwHorzFCodeB5"
#define STR_FORWVFCODEB5  "forwVertFCodeB5"
#define STR_FORWHSEARCHB5 "forwHorzSearchB5"
#define STR_FORWVSEARCHB5 "forwVertSearchB5"
#define STR_BACKHFCODEB5  "backHorzFCodeB5"
#define STR_BACKVFCODEB5  "backVertFCodeB5"
#define STR_BACKHSEARCHB5 "backHorzSearchB5"
#define STR_BACKVSEARCHB5 "backVertSearchB5"

#define STR_FORWHFCODEB6  "forwHorzFCodeB6"
#define STR_FORWVFCODEB6  "forwVertFCodeB6"
#define STR_FORWHSEARCHB6 "forwHorzSearchB6"
#define STR_FORWVSEARCHB6 "forwVertSearchB6"
#define STR_BACKHFCODEB6  "backHorzFCodeB6"
#define STR_BACKVFCODEB6  "backVertFCodeB6"
#define STR_BACKHSEARCHB6 "backHorzSearchB6"
#define STR_BACKVSEARCHB6 "backVertSearchB6"

#define STR_FORWHFCODEB7  "forwHorzFCodeB7"
#define STR_FORWVFCODEB7  "forwVertFCodeB7"
#define STR_FORWHSEARCHB7 "forwHorzSearchB7"
#define STR_FORWVSEARCHB7 "forwVertSearchB7"
#define STR_BACKHFCODEB7  "backHorzFCodeB7"
#define STR_BACKVFCODEB7  "backVertFCodeB7"
#define STR_BACKHSEARCHB7 "backHorzSearchB7"
#define STR_BACKVSEARCHB7 "backVertSearchB7"

#define STR_IGNOREINT     "ignore_frame_interval"
#define STR_USERIQUANT    "user_iquant"

#define STR_INTRAQ0       "intra_q[0]"
#define STR_INTRAQ1       "intra_q[1]"
#define STR_INTRAQ2       "intra_q[2]"
#define STR_INTRAQ3       "intra_q[3]"
#define STR_INTRAQ4       "intra_q[4]"
#define STR_INTRAQ5       "intra_q[5]"
#define STR_INTRAQ6       "intra_q[6]"
#define STR_INTRAQ7       "intra_q[7]"
#define STR_INTRAQ8       "intra_q[8]"
#define STR_INTRAQ9       "intra_q[9]"

#define STR_INTRAQ10      "intra_q[10]"
#define STR_INTRAQ11      "intra_q[11]"
#define STR_INTRAQ12      "intra_q[12]"
#define STR_INTRAQ13      "intra_q[13]"
#define STR_INTRAQ14      "intra_q[14]"
#define STR_INTRAQ15      "intra_q[15]"
#define STR_INTRAQ16      "intra_q[16]"
#define STR_INTRAQ17      "intra_q[17]"
#define STR_INTRAQ18      "intra_q[18]"
#define STR_INTRAQ19      "intra_q[19]"

#define STR_INTRAQ20      "intra_q[20]"
#define STR_INTRAQ21      "intra_q[21]"
#define STR_INTRAQ22      "intra_q[22]"
#define STR_INTRAQ23      "intra_q[23]"
#define STR_INTRAQ24      "intra_q[24]"
#define STR_INTRAQ25      "intra_q[25]"
#define STR_INTRAQ26      "intra_q[26]"
#define STR_INTRAQ27      "intra_q[27]"
#define STR_INTRAQ28      "intra_q[28]"
#define STR_INTRAQ29      "intra_q[29]"

#define STR_INTRAQ30      "intra_q[30]"
#define STR_INTRAQ31      "intra_q[31]"
#define STR_INTRAQ32      "intra_q[32]"
#define STR_INTRAQ33      "intra_q[33]"
#define STR_INTRAQ34      "intra_q[34]"
#define STR_INTRAQ35      "intra_q[35]"
#define STR_INTRAQ36      "intra_q[36]"
#define STR_INTRAQ37      "intra_q[37]"
#define STR_INTRAQ38      "intra_q[38]"
#define STR_INTRAQ39      "intra_q[39]"

#define STR_INTRAQ40      "intra_q[40]"
#define STR_INTRAQ41      "intra_q[41]"
#define STR_INTRAQ42      "intra_q[42]"
#define STR_INTRAQ43      "intra_q[43]"
#define STR_INTRAQ44      "intra_q[44]"
#define STR_INTRAQ45      "intra_q[45]"
#define STR_INTRAQ46      "intra_q[46]"
#define STR_INTRAQ47      "intra_q[47]"
#define STR_INTRAQ48      "intra_q[48]"
#define STR_INTRAQ49      "intra_q[49]"

#define STR_INTRAQ50      "intra_q[50]"
#define STR_INTRAQ51      "intra_q[51]"
#define STR_INTRAQ52      "intra_q[52]"
#define STR_INTRAQ53      "intra_q[53]"
#define STR_INTRAQ54      "intra_q[54]"
#define STR_INTRAQ55      "intra_q[55]"
#define STR_INTRAQ56      "intra_q[56]"
#define STR_INTRAQ57      "intra_q[57]"
#define STR_INTRAQ58      "intra_q[58]"
#define STR_INTRAQ59      "intra_q[59]"

#define STR_INTRAQ60      "intra_q[60]"
#define STR_INTRAQ61      "intra_q[61]"
#define STR_INTRAQ62      "intra_q[62]"
#define STR_INTRAQ63      "intra_q[63]"

#define STR_USERNIQUANT   "user_niquant"

#define STR_INTERQ0       "inter_q[0]"
#define STR_INTERQ1       "inter_q[1]"
#define STR_INTERQ2       "inter_q[2]"
#define STR_INTERQ3       "inter_q[3]"
#define STR_INTERQ4       "inter_q[4]"
#define STR_INTERQ5       "inter_q[5]"
#define STR_INTERQ6       "inter_q[6]"
#define STR_INTERQ7       "inter_q[7]"
#define STR_INTERQ8       "inter_q[8]"
#define STR_INTERQ9       "inter_q[9]"

#define STR_INTERQ10      "inter_q[10]"
#define STR_INTERQ11      "inter_q[11]"
#define STR_INTERQ12      "inter_q[12]"
#define STR_INTERQ13      "inter_q[13]"
#define STR_INTERQ14      "inter_q[14]"
#define STR_INTERQ15      "inter_q[15]"
#define STR_INTERQ16      "inter_q[16]"
#define STR_INTERQ17      "inter_q[17]"
#define STR_INTERQ18      "inter_q[18]"
#define STR_INTERQ19      "inter_q[19]"

#define STR_INTERQ20      "inter_q[20]"
#define STR_INTERQ21      "inter_q[21]"
#define STR_INTERQ22      "inter_q[22]"
#define STR_INTERQ23      "inter_q[23]"
#define STR_INTERQ24      "inter_q[24]"
#define STR_INTERQ25      "inter_q[25]"
#define STR_INTERQ26      "inter_q[26]"
#define STR_INTERQ27      "inter_q[27]"
#define STR_INTERQ28      "inter_q[28]"
#define STR_INTERQ29      "inter_q[29]"

#define STR_INTERQ30      "inter_q[30]"
#define STR_INTERQ31      "inter_q[31]"
#define STR_INTERQ32      "inter_q[32]"
#define STR_INTERQ33      "inter_q[33]"
#define STR_INTERQ34      "inter_q[34]"
#define STR_INTERQ35      "inter_q[35]"
#define STR_INTERQ36      "inter_q[36]"
#define STR_INTERQ37      "inter_q[37]"
#define STR_INTERQ38      "inter_q[38]"
#define STR_INTERQ39      "inter_q[39]"

#define STR_INTERQ40      "inter_q[40]"
#define STR_INTERQ41      "inter_q[41]"
#define STR_INTERQ42      "inter_q[42]"
#define STR_INTERQ43      "inter_q[43]"
#define STR_INTERQ44      "inter_q[44]"
#define STR_INTERQ45      "inter_q[45]"
#define STR_INTERQ46      "inter_q[46]"
#define STR_INTERQ47      "inter_q[47]"
#define STR_INTERQ48      "inter_q[48]"
#define STR_INTERQ49      "inter_q[49]"

#define STR_INTERQ50      "inter_q[50]"
#define STR_INTERQ51      "inter_q[51]"
#define STR_INTERQ52      "inter_q[52]"
#define STR_INTERQ53      "inter_q[53]"
#define STR_INTERQ54      "inter_q[54]"
#define STR_INTERQ55      "inter_q[55]"
#define STR_INTERQ56      "inter_q[56]"
#define STR_INTERQ57      "inter_q[57]"
#define STR_INTERQ58      "inter_q[58]"
#define STR_INTERQ59      "inter_q[59]"

#define STR_INTERQ60      "inter_q[60]"
#define STR_INTERQ61      "inter_q[61]"
#define STR_INTERQ62      "inter_q[62]"
#define STR_INTERQ63      "inter_q[63]"

// callback to localize the encoder error strings
// the dialog will use get_rc to try to get this function
typedef char*(*LPFN_MPG_DLG_localizeConvertStr)(char *fmt);

// callbacks for summary list box functions
#define STR_NORMAL 0
#define STR_WARN   1
#define STR_ERR    2
typedef void(*LPFNresetLBContent)();
typedef void(*LPFNaddLBString)(LPCTSTR str, int strType);

#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif

// use this structure with the Ex versions of the dialog functions
// to control the look and feel of the dialogs

struct mpeg_extended_dialog_settings
{
	// these items must be filled in and will not be modified
	// these should be a combination of one or more of the SETTINGS_* defines
	int options;                     // dialog pages to display and other options
	int initPage;                    // initial page to display

	// this item must be filled in and may be updated by the dialog
	mpeg_all_settings *mpegSettings; // the mpeg settings to display/return

	// this item is optional and may be filled in before calling
	// the dialog, it may be updated by the dialog
	char presetName[MAX_PATH];       // current settings filename
	char presetPath[MAX_PATH];       // current settings path

	// these items are optional and may be filled in before calling
	// the dialog, they will not be modified by the dialog
	char *fixedPresetPath;           // a fixed path to settings that will be listed in the presets control
	char *helpFilename;              // help filename with path
	char *appName;                   // application name
	HWND parent;                     // parent window for the dialog
	unsigned int totalTime;          // total time in seconds for file size estimation
	LOGFONT logicalFont;             // font to use for the dialogs. The font must already be loaded.
									 // WARNING: the dialogs and controls are not resized so use with care!
	void *(*get_rc)(char* name);     // callback for error, warning and localization functions

	unsigned char two_pass_flag;     // 1 = do 2 pass VBR

	unsigned char reserved[511];
};

#pragma pack(pop)


/* ************ Function-API Part ************* */
#ifdef __cplusplus
extern "C" {
#endif

// call to change the language of the dialog, normally the dialog will 
// check with the OS to see what language to use, but this can be used
// to override the default language
//
//  inputs: - the language ID to use
// outputs: none
//  return: settingsErrNone if the language was changed,
//          settingsError if not

int mpegOutDialogSetLang(LANGID userLang);


// the extended versions of the standard functions, use these to
// control the look and behaviour of the dialogs.

// call to display settings dialog, extended version
//
//  inputs: - a pointer to a filled in mpeg_dialog_settings structure
// outputs: none
//  return: settingsErrNone if user clicked ok button,
//          settingsCancel if user clicked cancel button,
//          settingsError if an error displaying dialog

int mpegOutDialogEx(mpeg_extended_dialog_settings *extendedSettings);

void mpegOutDialogInitSettingsEx(mpeg_extended_dialog_settings *extSet, int mpegType, int PAL);
int mpegOutDialogSaveSettingsEx(mpeg_extended_dialog_settings *extSet, char *filename);
int mpegOutDialogLoadSettingsEx(mpeg_extended_dialog_settings *extSet, char *filename, int updateName);
int mpegOutDialogFileSettingsEx(mpeg_extended_dialog_settings *extSet, char *filename, int updateName);
int mpegOutDialogIsDirtySettingsEx(mpeg_extended_dialog_settings *extSet, mpeg_all_settings *set);
void mpegOutDialogMPEGSummaryEx(void *(*lb_get_rc)(char* name), mpeg_extended_dialog_settings *extSet, mpeg_all_settings *set);




// the Alt versions of these functions behave identical to the standard
// versions except that the SETTINGS_FRAMESIZE and SETTINGS_FREQSTEREO
// defines are enabled meaning the frame size and audio frequency 
// control are visible.

// These functions will change the video encoders def_vertical_size, 
// def_horizontal_size and the mpeg_all_settings audio_frequency settings.
// 
// The Init/Load/Save functions will process the frame size and audio frequency
// settings

int mpegOutDialogAlt(mpeg_all_settings *settings, HWND parent, int showPages, int initPage);
void mpegOutDialogInitSettingsAlt(mpeg_all_settings *settings, int mpegType, int PAL);
int mpegOutDialogSaveSettingsAlt(mpeg_all_settings *settings, char *filename);
int mpegOutDialogLoadSettingsAlt(mpeg_all_settings *settings, char *filename);
int mpegOutDialogFileSettingsAlt(mpeg_all_settings *settings, char *filename);




// the standard (old) versions of the functions, these functions try not
// to change the video encoders def_vertical_size and def_horizontal_size 
// or the mpeg_all_settings audio_frequency settings. These items should be
// set before calling the dialog and the dialog will attempt to prevent the 
// user from selecting settings that would be incorrect for the frame size
// and audio frequency.
//
// the Init/Load/Save functions will ignore the frame size and audio frequency
// settings



// call to display settings dialog, allows user to specify mpeg settings
//
//  inputs: - a pointer to a filled in mpeg_all_settings structure
//          - a parent window for the dialog if desired, can be NULL
//          - dialog pages to display, combinations of the SETTINGS_* constants
//          - the initial dialog page to display, one of the SETTINGS_* constants
// outputs: none
//  return: settingsErrNone if user clicked ok button,
//          settingsCancel if user clicked cancel button,
//          settingsError if an error displaying dialog

int mpegOutDialog(mpeg_all_settings *settings, HWND parent, int showPages, int initPage);


// call to fill out an mpeg_all_settings structure with a specified MPEG type
//
//  inputs: - a pointer to a mpeg_all_settings structure
//          - mpegType, one of the MPEG_* constants
//          - PAL flag, one of the VM_* constants
// outputs: none
//  return: none

void mpegOutDialogInitSettings(mpeg_all_settings *settings, int mpegType, int PAL);

// call to save settings to an ini file
//
//  inputs: - a pointer to a mpeg_all_settings structure
//          - a filename to save the settings to
// outputs: none
//  return: settingsErrNone if succesful
//          settingsError if unsucessful

int mpegOutDialogSaveSettings(mpeg_all_settings *settings, char *filename);

// call to load settings from an ini file
//
//  inputs: - a pointer to a mpeg_all_settings structure
//          - a filename to load the settings from
//            NOTE! The filename should contain the complete path to the file
//            otherwise, windows will look for the ini file in the windows directory
// outputs: none
//  return: settingsErrNone if succesful
//          settingsError if unsucessful

int mpegOutDialogLoadSettings(mpeg_all_settings *settings, char *filename);

// call to load settings from an MPEG file
//
//  inputs: - a pointer to a mpeg_all_settings structure
//          - the MPEG filename to load the settings from
// outputs: none
//  return: settingsErrNone if succesful
//          settingsError if unsucessful

int mpegOutDialogFileSettings(mpeg_all_settings *settings, char *filename);


// provide interface to the video API from Dialog-structure and Dialog-dll
mpeg_v_settings* mpegOutDlgGetVideoSettingsPtr(mpeg_all_settings *settings);

// provide interface to the MPEG audio API from Dialog-structure and Dialog-dll
mpeg_a_settings* mpegOutDlgGetAudioSettingsPtr(mpeg_all_settings *settings);

// provide interface to the PCM audio API from Dialog-structure and Dialog-dll
pcm_a_settings* mpegOutDlgGetPCMSettingsPtr(mpeg_all_settings *settings);

// provide interface to the muxer API from Dialog-structure and Dialog-dll
mpeg_m_settings* mpegOutDlgGetMuxSettingsPtr(mpeg_all_settings *settings);

int mpegOutDialogGetSettings(mpeg_all_settings *settings, char *settingsList, void (*callback)(char* setting));
int mpegOutDialogPutSettings(mpeg_all_settings *settings, char *settingsList);


#ifdef __cplusplus
}
#endif

#endif // _mpg_dlg_h
