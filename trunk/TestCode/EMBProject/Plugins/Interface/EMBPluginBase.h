#pragma once

#ifdef EMBPLUGIN_EXPORTS
#define EMBPLUGIN_API __declspec(dllexport)
#else
#define EMBPLUGIN_API __declspec(dllimport)
#endif

//must implement this func.
extern "C"
int EMBPLUGIN_API GetPluginInstance(LPVOID& pInterface);
