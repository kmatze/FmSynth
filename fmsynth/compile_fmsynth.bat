@echo off

REM *********************************
set GCC=tcc
set WINLIB=-lwinmm
set FILE=testfmsynth
set EXT=exe
set FILEADD=fmsynth.c adlibemu.c	
set PCK=c:\win-apps\tools\upx395.exe -9

%GCC -o %FILE.%EXT %FILE.c %FILEADD %WINLIB

strip %FILE.%EXT
%PCK %FILE.%EXT
