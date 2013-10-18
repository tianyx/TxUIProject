/********************************************************************
	created:	2013/05/22
	created:	2013:5
	filename: 	ZQLicenseProtect.h
	file base:	ZQLicenseProtect
	file ext:	h
	author:		tian_yx
	purpose:	export interface
*********************************************************************/
// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� ZQLICENSEPROTECT_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// ZQLICENSEPROTECT_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef ZQLICENSEPROTECT_EXPORTS
#define ZQLICENSEPROTECT_API __declspec(dllexport)
#else
#define ZQLICENSEPROTECT_API __declspec(dllimport)
#endif

#ifdef __cplusplus   
extern "C" {
#endif 
ZQLICENSEPROTECT_API int __cdecl InitZQApp(const char* szData);
ZQLICENSEPROTECT_API int __cdecl InitZQApp2(const char* szData, int* nRand);
ZQLICENSEPROTECT_API int __cdecl GetZQApp(char* szData, const int nBuff);
ZQLICENSEPROTECT_API int __cdecl GenZQReqCode();
#ifdef __cplusplus  
}  
#endif  