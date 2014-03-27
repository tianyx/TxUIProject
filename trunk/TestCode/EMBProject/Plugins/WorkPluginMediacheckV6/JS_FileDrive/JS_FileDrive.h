#ifndef _JS_FileDrive_H_
#define _JS_FileDrive_H_ 
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the JS_FILEDRIVE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// JS_FILEDRIVE_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef JS_FILEDRIVE_EXPORTS
#define _JS_Fix __declspec(dllexport)
#else
#define _JS_Fix __declspec(dllimport)
#endif
#pragma warning(disable : 4251)
#pragma warning(disable : 4005)
#pragma warning(disable : 4786)
#pragma warning(disable : 4183)

#endif
