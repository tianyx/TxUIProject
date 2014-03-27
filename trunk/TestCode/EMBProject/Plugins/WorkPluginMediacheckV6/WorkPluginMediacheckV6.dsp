# Microsoft Developer Studio Project File - Name="WorkPluginMediacheckV6" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=WorkPluginMediacheckV6 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WorkPluginMediacheckV6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WorkPluginMediacheckV6.mak" CFG="WorkPluginMediacheckV6 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WorkPluginMediacheckV6 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WorkPluginMediacheckV6 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "WorkPluginMediacheckV6"
# PROP Scc_LocalPath ".."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WorkPluginMediacheckV6 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Output\WorkPluginMediacheckV6\Release"
# PROP Intermediate_Dir "..\..\Output\WorkPluginMediacheckV6\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\interface" /I "..\..\common" /I "..\..\commonDll" /I "..\..\ThirdPartySDK\DXSDK\include" /I "..\..\ThirdPartySDK\DXSDK\Samples\C++\DirectShow\BaseClasses" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "EMBPLUGIN_EXPORTS" /D "VC6DEFINE" /Yu"stdafx.h" /FD /Zm500 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 AutoDetect.lib JS_FileDrive.lib EMBCommonLibV6.lib WSOCK32.lib /nologo /subsystem:windows /dll /pdb:"..\..\pdb\Release\MediaCheckV6\WorkPluginMediaCheckV6.pdb" /debug /machine:I386 /nodefaultlib:"libcmt" /def:".\WorkPluginMediacheckV6.def" /out:"..\..\bin\Release\exc\WorkPluginMediacheckV6\WorkPluginMediacheckV6.epl" /implib:"../../lib/Release\MediaCheckV6/WorkPluginMediacheckV6.lib" /libpath:"..\..\lib\Release\MediaCheckV6" /libpath:"..\..\lib\Release" /libpath:"..\..\..\ThirdPartySDK\DXSDK\Lib"
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "WorkPluginMediacheckV6 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Output\WorkPluginMediacheckV6\Debug"
# PROP Intermediate_Dir "..\..\Output\WorkPluginMediacheckV6\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\interface" /I "..\..\common" /I "..\..\commonDll" /I "..\..\ThirdPartySDK\DXSDK\include" /I "..\..\ThirdPartySDK\DXSDK\Samples\C++\DirectShow\BaseClasses" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "EMBPLUGIN_EXPORTS" /D "VC6DEFINE" /FR /Yu"stdafx.h" /FD /GZ /Zm500 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 AutoDetect.lib JS_FileDrive.lib EMBCommonLibV6.lib WSOCK32.lib /nologo /subsystem:windows /dll /pdb:"..\..\pdb\Debug\MediaCheckV6\WorkPluginMediaCheckV6.pdb" /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /nodefaultlib:"libcmt.lib" /def:".\WorkPluginMediacheckV6.def" /out:"..\..\bin\Debug\exc\WorkPluginMediacheckV6\WorkPluginMediacheckV6.epl" /implib:"../../lib/Debug\MediaCheckV6/WorkPluginMediacheckV6.lib" /pdbtype:sept /libpath:"..\..\lib\Debug\MediaCheckV6" /libpath:"..\..\lib\Debug" /libpath:"..\..\..\ThirdPartySDK\DXSDK\Lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "WorkPluginMediacheckV6 - Win32 Release"
# Name "WorkPluginMediacheckV6 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Interface\EMBGuid.cpp
# End Source File
# Begin Source File

SOURCE=.\FCVSClass.cpp
# End Source File
# Begin Source File

SOURCE=.\FCVSServerConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDrive_3GP.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDrive_3GP.h
# End Source File
# Begin Source File

SOURCE=.\FileDrive_MP4.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDrive_MP4.h
# End Source File
# Begin Source File

SOURCE=.\FileDriver.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDriver.h
# End Source File
# Begin Source File

SOURCE=.\FileDriver_AVI.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDriver_AVI.h
# End Source File
# Begin Source File

SOURCE=.\FileDriver_Hik.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDriver_Hik.h
# End Source File
# Begin Source File

SOURCE=.\FileDriver_Leitch.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDriver_Leitch.h
# End Source File
# Begin Source File

SOURCE=.\FileDriver_M2V.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDriver_M2V.h
# End Source File
# Begin Source File

SOURCE=.\FileDriver_mov.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDriver_mov.h
# End Source File
# Begin Source File

SOURCE=.\FileDriver_MPG.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDriver_MPG.h
# End Source File
# Begin Source File

SOURCE=.\FileDriver_MSS.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDriver_MSS.h
# End Source File
# Begin Source File

SOURCE=.\FileDriver_Omn.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDriver_Omn.h
# End Source File
# Begin Source File

SOURCE=.\FileDriver_TS.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDriver_TS.h
# End Source File
# Begin Source File

SOURCE=.\FileDriver_WMV.cpp
# End Source File
# Begin Source File

SOURCE=.\FileDriver_WMV.h
# End Source File
# Begin Source File

SOURCE=.\GeneralDef.h
# End Source File
# Begin Source File

SOURCE=.\MediaCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\mxfliberror.cpp
# End Source File
# Begin Source File

SOURCE=.\RuntimeClassFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\RuntimeClassFactory.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\WorkPluginMediacheckV6.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkPluginMediacheckV6.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\WorkPluginMediacheckV6.rc
# End Source File
# Begin Source File

SOURCE=.\XMLOp.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLOp.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FCVSClass.h
# End Source File
# Begin Source File

SOURCE=.\FCVSServerConfig.h
# End Source File
# Begin Source File

SOURCE=.\MediaCheck.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\WorkPluginMediacheckV6.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
