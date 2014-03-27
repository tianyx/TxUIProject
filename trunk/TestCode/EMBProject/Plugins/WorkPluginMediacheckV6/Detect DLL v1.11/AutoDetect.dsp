# Microsoft Developer Studio Project File - Name="AutoDetect" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=AutoDetect - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AutoDetect.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AutoDetect.mak" CFG="AutoDetect - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AutoDetect - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AutoDetect - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Perforce Project"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AutoDetect - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Output\WorkPluginMediacheckV6\Release\AutoDetect"
# PROP Intermediate_Dir "..\..\..\Output\WorkPluginMediacheckV6\Release\AutoDetect"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AUTODETECT_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "..\..\..\commonDll" /I "..\..\..\ThirdPartySDK\DXSDK\Samples\C++\DirectShow\BaseClasses" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AUTODETECT_EXPORTS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /dll /pdb:"..\..\..\pdb\Release\MediaCheckV6\AutoDetect.pdb" /debug /machine:I386 /out:"..\..\..\bin\Release\exc\WorkPluginMediacheckV6\AutoDetect.dll" /implib:"..\..\..\lib\Release\MediaCheckV6\AutoDetect.lib" /libpath:"..\..\..\lib\Release" /libpath:"..\..\..\lib\release\MediaCheckV6"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "AutoDetect - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\Output\WorkPluginMediacheckV6\Debug\AutoDetect"
# PROP Intermediate_Dir "..\..\..\Output\WorkPluginMediacheckV6\Debug\AutoDetect"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AUTODETECT_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\..\..\commonDll" /I "..\..\..\ThirdPartySDK\DXSDK\Samples\C++\DirectShow\BaseClasses" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AUTODETECT_EXPORTS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib shlwapi.lib /nologo /dll /pdb:"..\..\..\pdb\Debug\MediaCheckV6\AutoDetect.pdb" /debug /machine:I386 /out:"..\..\..\bin\Debug\exc\WorkPluginMediacheckV6\AutoDetect.dll" /implib:"..\..\..\lib\Debug\MediaCheckV6\AutoDetect.lib" /pdbtype:sept /libpath:"..\..\..\lib\Debug" /libpath:"..\..\..\lib\Debug\MediaCheckV6"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "AutoDetect - Win32 Release"
# Name "AutoDetect - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AutoDetect.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoDetect.rc
# End Source File
# Begin Source File

SOURCE=.\AutoDetectApp.cpp
# End Source File
# Begin Source File

SOURCE=.\CTimer.cpp

!IF  "$(CFG)" == "AutoDetect - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "AutoDetect - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\loudclass.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkupMSXML.cpp
# End Source File
# Begin Source File

SOURCE=.\polyloud.cpp
# End Source File
# Begin Source File

SOURCE=.\volume.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AutoDetect.h
# End Source File
# Begin Source File

SOURCE=.\AutoDetectApp.h
# End Source File
# Begin Source File

SOURCE=.\CTimer.h
# End Source File
# Begin Source File

SOURCE=.\define.h
# End Source File
# Begin Source File

SOURCE=.\MarkupMSXML.h
# End Source File
# Begin Source File

SOURCE=.\OptionMap.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\volume.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "AutoDetect - Win32 Release"

!ELSEIF  "$(CFG)" == "AutoDetect - Win32 Debug"

# PROP Intermediate_Dir "F:\VideoAna v1.10\Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\memcpy.obj
# End Source File
# End Target
# End Project
