@echo off

copy /y "AGKPlayer2\src\main\jniLibs\arm64-v8a\libandroid_player.so" "..\interpreter_android_amazon\AGKPlayer2\src\main\jniLibs\arm64-v8a\libandroid_player.so"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
copy /y "AGKPlayer2\src\main\jniLibs\armeabi-v7a\libandroid_player.so" "..\interpreter_android_amazon\AGKPlayer2\src\main\jniLibs\armeabi-v7a\libandroid_player.so"
if not %ERRORLEVEL% equ 0 ( GOTO failed )

copy /y "AGKPlayer2\src\main\jniLibs\arm64-v8a\libandroid_player.so" "..\interpreter_android_ouya\AGKPlayer2\src\main\jniLibs\arm64-v8a\libandroid_player.so"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
copy /y "AGKPlayer2\src\main\jniLibs\armeabi-v7a\libandroid_player.so" "..\interpreter_android_ouya\AGKPlayer2\src\main\jniLibs\armeabi-v7a\libandroid_player.so"
if not %ERRORLEVEL% equ 0 ( GOTO failed )

goto :eof

:failed
EXIT /B 1