#pragma once

#ifdef EMBPLUGIN_EXPORTS
#define EMBNETMSGTRANSDLL_API __declspec(dllexport)
#else
#define EMBNETMSGTRANSDLL_API __declspec(dllimport)
#endif

//must implement this func.
extern "C"
int EMBNETMSGTRANSDLL_API ExpFunPackEMBTcpMsg( const int nTcpMsgTypeIn, const char* szGuidIn, const char* szXmlIn, char* szBuffOut, const int nbuffLenIn, int& nUsedOut );

extern "C"
int EMBNETMSGTRANSDLL_API ExpFunUnPackEMBTcpMsg( const char* szMsgBuffIn, const int nMsgbuffLenIn, int& nTcpMsgTypeOut, char* szGuidOut, char* szXmlBuffOut, const int nXmlBuffLen );
