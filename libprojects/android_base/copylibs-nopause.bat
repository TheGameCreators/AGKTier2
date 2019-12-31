@echo off

copy /y "obj\local\armeabi-v7a\libAGKAndroid.a" "..\..\platform\android\jni\armeabi-v7a\libAGKAndroid.a"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
copy /y "obj\local\arm64-v8a\libAGKAndroid.a" "..\..\platform\android\jni\arm64-v8a\libAGKAndroid.a"
if not %ERRORLEVEL% equ 0 ( GOTO failed )

copy /y "..\android_bullet\obj\local\armeabi-v7a\libAGKBullet.a" "..\..\platform\android\jni\armeabi-v7a\libAGKBullet.a"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
copy /y "..\android_bullet\obj\local\arm64-v8a\libAGKBullet.a" "..\..\platform\android\jni\arm64-v8a\libAGKBullet.a"
if not %ERRORLEVEL% equ 0 ( GOTO failed )

copy /y "..\android_assimp\obj\local\armeabi-v7a\libAGKAssimp.a" "..\..\platform\android\jni\armeabi-v7a\libAGKAssimp.a"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
copy /y "..\android_assimp\obj\local\arm64-v8a\libAGKAssimp.a" "..\..\platform\android\jni\arm64-v8a\libAGKAssimp.a" 
if not %ERRORLEVEL% equ 0 ( GOTO failed )

copy /y "..\android_curl\obj\local\armeabi-v7a\libAGKCurl.a" "..\..\platform\android\jni\armeabi-v7a\libAGKCurl.a"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
copy /y "..\android_curl\obj\local\arm64-v8a\libAGKCurl.a" "..\..\platform\android\jni\arm64-v8a\libAGKCurl.a" 
if not %ERRORLEVEL% equ 0 ( GOTO failed )

rem copy ARCore libs to projects that might use it
IF NOT EXIST ..\..\IDE GOTO NOIDE
copy /y "..\..\platform\android\ARCore\libs\armeabi-v7a\libarcore_sdk.so" "..\..\IDE\Geany-1.24.1\data\android\lib\armeabi-v7a\libarcore_sdk.so"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
copy /y "..\..\platform\android\ARCore\libs\arm64-v8a\libarcore_sdk.so" "..\..\IDE\Geany-1.24.1\data\android\lib\arm64-v8a\libarcore_sdk.so"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
:NOIDE

IF NOT EXIST ..\..\apps\interpreter_android_google GOTO NOAPP1
copy /y "..\..\platform\android\ARCore\libs\armeabi-v7a\libarcore_sdk.so" "..\..\apps\interpreter_android_google\AGKPlayer2\src\main\jniLibs\armeabi-v7a\libarcore_sdk.so"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
copy /y "..\..\platform\android\ARCore\libs\arm64-v8a\libarcore_sdk.so" "..\..\apps\interpreter_android_google\AGKPlayer2\src\main\jniLibs\arm64-v8a\libarcore_sdk.so"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
:NOAPP1

IF NOT EXIST ..\..\apps\interpreter_android_google_compiler GOTO NOAPP2
copy /y "..\..\platform\android\ARCore\libs\armeabi-v7a\libarcore_sdk.so" "..\..\apps\interpreter_android_google_compiler\AGKPlayer2\src\main\jniLibs\armeabi-v7a\libarcore_sdk.so"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
copy /y "..\..\platform\android\ARCore\libs\arm64-v8a\libarcore_sdk.so" "..\..\apps\interpreter_android_google_compiler\AGKPlayer2\src\main\jniLibs\arm64-v8a\libarcore_sdk.so"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
:NOAPP2

IF NOT EXIST ..\..\apps\template_android_google GOTO NOAPP3
copy /y "..\..\platform\android\ARCore\libs\armeabi-v7a\libarcore_sdk.so" "..\..\apps\template_android_google\AGK2Template\src\main\jniLibs\armeabi-v7a\libarcore_sdk.so"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
copy /y "..\..\platform\android\ARCore\libs\arm64-v8a\libarcore_sdk.so" "..\..\apps\template_android_google\AGK2Template\src\main\jniLibs\arm64-v8a\libarcore_sdk.so"
if not %ERRORLEVEL% equ 0 ( GOTO failed )
:NOAPP3

goto :eof

:failed
EXIT /B 1