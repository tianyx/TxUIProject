#pragma once

#ifdef TXDRAW_EXPORTS
#ifdef _cplusplus
#define TXDRAW_EXPORTS_API __declspec(dllexport)
#else
#define TXDRAW_EXPORTS_API EXTERN_C __declspec(dllexport)
#endif // _cplusplus
#else
#ifdef _cplusplus
#define TXDRAW_EXPORTS_API __declspec(dllimport)
#else
#define TXDRAW_EXPORTS_API EXTERN_C __declspec(dllimport)
#endif // _cplusplus
#endif


 TXDRAW_EXPORTS_API BOOL TxGlobalInit();
 TXDRAW_EXPORTS_API BOOL TxGlobalUnInit();