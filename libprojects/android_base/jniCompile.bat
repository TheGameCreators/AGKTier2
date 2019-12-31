@echo off

set NDKBUILDCMD="E:\Data\NDK\android-ndk-r20\ndk-build"

cd ..\android_bullet
call %NDKBUILDCMD% -j16 2> log.txt
if not %ERRORLEVEL% equ 0 ( GOTO failed )

cd ..\android_assimp
call %NDKBUILDCMD% -j16 2> log.txt
if not %ERRORLEVEL% equ 0 ( GOTO failed )

cd ..\android_curl
call %NDKBUILDCMD% -j16 2> log.txt
if not %ERRORLEVEL% equ 0 ( GOTO failed )

cd ..\android_base
call %NDKBUILDCMD% -j16 2> log.txt
if not %ERRORLEVEL% equ 0 ( GOTO failed )

:failed

if "%1"=="nopause" goto end
pause
:end

if not %ERRORLEVEL% equ 0 ( EXIT /B 1 )