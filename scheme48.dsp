# Microsoft Developer Studio Project File - Name="scheme48" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=scheme48 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "scheme48.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "scheme48.mak" CFG="scheme48 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "scheme48 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "scheme48 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "scheme48 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "c" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__COMPILING_SCHEME48_ITSELF__" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /fo"scheme48.res" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mswsock.lib /nologo /subsystem:console /machine:I386 /out:"scheme48vm.exe"

!ELSEIF  "$(CFG)" == "scheme48 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "c" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__COMPILING_SCHEME48_ITSELF__" /FR /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /fo"scheme48.res" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib mswsock.lib /nologo /subsystem:console /debug /machine:I386 /out:"scheme48vm.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "scheme48 - Win32 Release"
# Name "scheme48 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\c\bignum.c
# End Source File
# Begin Source File

SOURCE=.\c\double_to_string.c
# End Source File
# Begin Source File

SOURCE=.\c\win32\dynlink.c
# End Source File
# Begin Source File

SOURCE=.\c\win32\event.c
# End Source File
# Begin Source File

SOURCE=.\c\extension.c
# End Source File
# Begin Source File

SOURCE=.\c\external.c
# End Source File
# Begin Source File

SOURCE=".\c\win32\fd-io.c"
# End Source File
# Begin Source File

SOURCE=.\c\free.c
# End Source File
# Begin Source File

SOURCE=.\c\fake\glue.c
# End Source File
# Begin Source File

SOURCE=".\c\win32\init-external.c"
# End Source File
# Begin Source File

SOURCE=.\c\init.c
# End Source File
# Begin Source File

SOURCE=.\c\win32\io.c
# End Source File
# Begin Source File

SOURCE=.\c\main.c
# End Source File
# Begin Source File

SOURCE=.\c\win32\misc.c
# End Source File
# Begin Source File

SOURCE=.\build\scheme48.rc
# End Source File
# Begin Source File

SOURCE=.\c\scheme48heap.c
# End Source File
# Begin Source File

SOURCE=".\c\scheme48read-image.c"
# End Source File
# Begin Source File

SOURCE=.\c\scheme48vm.c
# End Source File
# Begin Source File

SOURCE=".\c\scheme48write-image.c"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\c\bignum.h
# End Source File
# Begin Source File

SOURCE=.\c\bignumint.h
# End Source File
# Begin Source File

SOURCE=".\c\c-mods.h"
# End Source File
# Begin Source File

SOURCE=.\c\event.h
# End Source File
# Begin Source File

SOURCE=".\c\fd-io.h"
# End Source File
# Begin Source File

SOURCE=.\c\io.h
# End Source File
# Begin Source File

SOURCE=.\c\prescheme.h
# End Source File
# Begin Source File

SOURCE=.\c\scheme48.h
# End Source File
# Begin Source File

SOURCE=.\c\scheme48heap.h
# End Source File
# Begin Source File

SOURCE=.\c\scheme48image.h
# End Source File
# Begin Source File

SOURCE=".\c\scheme48vm-prelude.h"
# End Source File
# Begin Source File

SOURCE=.\c\scheme48vm.h
# End Source File
# Begin Source File

SOURCE=".\c\scheme48write-barrier.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\build\scheme48.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
