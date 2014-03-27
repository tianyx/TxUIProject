# Microsoft Developer Studio Project File - Name="JS_FileDrive" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=JS_FileDrive - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "JS_FileDrive.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "JS_FileDrive.mak" CFG="JS_FileDrive - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "JS_FileDrive - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "JS_FileDrive - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "JS_FileDrive"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "JS_FileDrive - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Output\WorkPluginMediacheckV6\Release\JS_FileDrive"
# PROP Intermediate_Dir "..\..\..\Output\WorkPluginMediacheckV6\Release\JS_FileDrive"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JS_FILEDRIVE_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./mxflib-1.0.0" /I "..\..\..\ThirdPartySDK\DXSDK\include" /I "..\..\..\ThirdPartySDK\DXSDK\Samples\C++\DirectShow\BaseClasses" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JS_FILEDRIVE_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /Zm200 /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 mxflib.lib Dmoguids.lib VFW32.lib  Strmiids.lib strmbase.lib /nologo /dll /pdb:"..\..\..\pdb\Release\MediaCheckV6\JS_FileDrive.pdb" /debug /machine:I386 /nodefaultlib:"libcmt.lib" /def:".\JS_FileDrive.Def" /out:"..\..\..\bin\Release\exc\WorkPluginMediacheckV6\JS_FileDrive.dll" /implib:"..\..\..\lib\Release\MediaCheckV6\JS_FileDrive.lib" /libpath:"..\..\..\lib\Release" /libpath:"..\..\..\lib\Release\MediaCheckV6" /libpath:"..\..\..\ThirdPartySDK\DXSDK\Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "JS_FileDrive - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\Output\WorkPluginMediacheckV6\Debug\JS_FileDrive"
# PROP Intermediate_Dir "..\..\..\Output\WorkPluginMediacheckV6\Debug\JS_FileDrive"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "JS_FILEDRIVE_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "./mxflib-1.0.0" /I "..\..\..\ThirdPartySDK\DXSDK\include" /I "..\..\..\ThirdPartySDK\DXSDK\Samples\C++\DirectShow\BaseClasses" /D "_WIN32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXEXT" /D "JS_FILEDRIVE_EXPORTS" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 mxflib_D.lib dmoguids.lib VFW32.lib Strmiids.lib strmbase.lib /nologo /dll /pdb:"..\..\..\pdb\Debug\MediaCheckV6\JS_FileDrive.pdb" /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /def:".\JS_FileDrive.Def" /out:"..\..\..\bin\Debug\exc\WorkPluginMediacheckV6\JS_FileDrive.dll" /implib:"..\..\..\lib\Debug\MediaCheckV6\JS_FileDrive.lib" /pdbtype:sept /libpath:"..\..\..\lib\Debug" /libpath:"..\..\..\lib\Debug\MediaCheckV6" /libpath:"..\..\..\ThirdPartySDK\DXSDK\Lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "JS_FileDrive - Win32 Release"
# Name "JS_FileDrive - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AFDWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioGrabberCB.cpp
# End Source File
# Begin Source File

SOURCE=.\AVIInfoExtract.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_3GP.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_AVI.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_Hik.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_LXF.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_M2V.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_MOV.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_MP4.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_MPG.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_MSS.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_MXF.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_TS.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_WMV.cpp
# End Source File
# Begin Source File

SOURCE=.\CallBack.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_3GP.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_AVI.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_Hik.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_LXF.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_M2V.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_MOV.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_MP4.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_MPG.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_MSS.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_MXF.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_TS.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_WMV.cpp
# End Source File
# Begin Source File

SOURCE=.\File64.cpp
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract.cpp
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_3GP.cpp
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_Hik.cpp
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_MP4.cpp
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_MPG.cpp
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_MSS.cpp
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_TS.cpp
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_WMV.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoExtract_ASF.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoFromDS.cpp
# End Source File
# Begin Source File

SOURCE=.\JS_FileDrive.cpp
# End Source File
# Begin Source File

SOURCE=.\JS_FileDrive.Def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\LxfInfoExtract.cpp
# End Source File
# Begin Source File

SOURCE=.\M2vInfoExtract.cpp
# End Source File
# Begin Source File

SOURCE=.\MovInfoExtract.cpp
# End Source File
# Begin Source File

SOURCE=.\MxfInfoExtract.cpp
# End Source File
# Begin Source File

SOURCE=.\mxfliberror.cpp
# End Source File
# Begin Source File

SOURCE=.\SampleGrabberCB.cpp
# End Source File
# Begin Source File

SOURCE=.\SequenceHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UUID.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AFDWriter.h
# End Source File
# Begin Source File

SOURCE=.\AudioGrabberCB.h
# End Source File
# Begin Source File

SOURCE=.\AVIInfoExtract.h
# End Source File
# Begin Source File

SOURCE=.\BaseGraph.h
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_3GP.h
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_AVI.h
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_Hik.h
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_LXF.h
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_M2V.h
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_MOV.h
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_MP4.h
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_MPG.h
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_MSS.h
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_MXF.h
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_TS.h
# End Source File
# Begin Source File

SOURCE=.\BaseGraph_WMV.h
# End Source File
# Begin Source File

SOURCE=.\CallBack.h
# End Source File
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\DriveMgr.h
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_3GP.h
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_AVI.h
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_Hik.h
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_LXF.h
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_M2V.h
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_MOV.h
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_MP4.h
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_MPG.h
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_MSS.h
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_MXF.h
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_TS.h
# End Source File
# Begin Source File

SOURCE=.\DriveMgr_WMV.h
# End Source File
# Begin Source File

SOURCE=.\File64.h
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract.h
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_3GP.h
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_Hik.h
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_MOV.h
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_MP4.h
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_MPG.h
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_MSS.h
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_TS.h
# End Source File
# Begin Source File

SOURCE=.\FileInfoExtract_WMV.h
# End Source File
# Begin Source File

SOURCE=.\GuidRef.h
# End Source File
# Begin Source File

SOURCE=.\InfoExtract_ASF.h
# End Source File
# Begin Source File

SOURCE=.\InfoFromDS.h
# End Source File
# Begin Source File

SOURCE=.\JS_FileDrive.h
# End Source File
# Begin Source File

SOURCE=.\LxfInfoExtract.h
# End Source File
# Begin Source File

SOURCE=.\M2vInfoExtract.h
# End Source File
# Begin Source File

SOURCE=.\MediaSample.h
# End Source File
# Begin Source File

SOURCE=.\MovInfoExtract.h
# End Source File
# Begin Source File

SOURCE=.\MxfInfoExtract.h
# End Source File
# Begin Source File

SOURCE=.\SampleGrabberCB.h
# End Source File
# Begin Source File

SOURCE=.\SequenceHeader.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UUID.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
