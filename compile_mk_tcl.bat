@echo off

REM *********************************
set GCC=gcc -std=c99

set TCL=83
set TCLDIR=c:\DEV\TCL\TCL%TCL
REM set TCLLIB=-ltclstub%TCL -ltkStub%TCL
set TCLLIB=-ltclstub%TCL

set FMSYNTHDIR=.\fmsynth
set MKMIDIDIR=.\mkmidi
set WINLIB=-lwinmm

set FILE=mk
set EXT=dll
set FILEADD=%FMSYNTHDIR/fmsynth.c %FMSYNTHDIR/adlibemu.c %MKMIDIDIR%/mkmidi.c	
set PCK=c:\win-apps\tools\upx395.exe -9

%GCC -shared -o3 -o %FILE.%EXT %FILE.c %FILEADD -DUSE_TCL_STUBS -DUSE_TK_STUBS %WINLIB -I%TCLDIR\include -L%TCLDIR\lib %TCLLIB -I%MKMIDIDIR -I%FMSYNTHDIR

strip %FILE.%EXT
%PCK %FILE.%EXT
