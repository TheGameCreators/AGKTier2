@echo off

set NDKBUILDCMD="E:\Data\NDK\android-ndk-r20\ndk-build"

call %NDKBUILDCMD% NDK_OUT=../../build/jniObjs NDK_LIBS_OUT=./jniLibs 2> log.txt

pause
