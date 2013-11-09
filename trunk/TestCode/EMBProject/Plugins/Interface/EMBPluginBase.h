/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:07
	filename: 	EMBPluginBase.h
	author:		tianyx
	
	purpose:	for plugin export function
*********************************************************************/
#pragma once

#ifdef EMBPLUGIN_EXPORTS
#define EMBPLUGIN_API __declspec(dllexport)
#else
#define EMBPLUGIN_API __declspec(dllimport)
#endif

//must implement this func.
extern "C"
int EMBPLUGIN_API GetPluginInstance(LPVOID& pInterface);
