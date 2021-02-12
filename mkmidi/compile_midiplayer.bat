@echo off

REM *********************************
rem set GCC=gcc -std=c99
set GCC=tcc

set FMSYNTHDIR=..\fmsynth
set WINLIB=-lwinmm

set FILE=midiplayer
set EXT=exe
set FILEADD=mkmidi.c %FMSYNTHDIR/fmsynth.c %FMSYNTHDIR/adlibemu.c	
set PCK=c:\win-apps\tools\upx395.exe -9

%GCC -o %FILE.%EXT %FILE.c %FILEADD %WINLIB -I%FMSYNTHDIR

rem strip %FILE.%EXT
%PCK %FILE.%EXT

