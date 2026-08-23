
@echo off

REM OLD / UNUSED
REM set IncludeDirs=-I"..\..\vendor\GLFW\include" -I"..\..\vendor\glad\include" -I"..\..\vendor\cglm\include" -I"..\..\vendor\stb_image"
REM set LibPaths=-L"..\..\vendor\GLFW\lib-vc2022"

REM set CommonCompilerFlags=-std=c++17 -Wall -Wextra -Wpedantic -Wno-unknown-pragmas -O2 -g -DGLFW_STATIC
REM set CommonCompilerFlags=-std=c++17 -Wall -Wextra -Wpedantic -Werror -Wno-unknown-pragmas -Wno-writable-strings -Wno-unused-parameter -Wno-unused-variable -Wno-unused-value -Wno-unneeded-internal-declaration -O2 -g -DGLFW_STATIC
REM set CommonCompilerFlags=-std=c++17 -Wall -Wextra -Wpedantic -Werror -Wno-unknown-pragmas -Wno-writable-strings -Wno-unused-parameter -Wno-unused-variable -Wno-unused-value -Wno-unneeded-internal-declaration -O0 -g -DGLFW_STATIC


REM set CommonCompilerFlags=-Wall -Wextra -Wpedantic -Werror -Wno-unknown-pragmas -Wno-writable-strings -Wno-unused-parameter -Wno-unused-variable -Wno-unused-value -Wno-unneeded-internal-declaration -O0 -g -DGLFW_STATIC

REM set CommonLinkerFlags=-Wl,--gc-sections %LibPaths% %Libs% -luser32 -lgdi32 -lshell32

REM CLANG BUILD SCRIPT
REM set IncludeDirs=-I"..\vendor\GLFW\include" -I"..\vendor\glad\include" -I"..\vendor\cglm\include" -I"..\vendor\stb_image" -I"..\vendor\freetype-2.14.3\include"
REM set LibPaths=-L"..\vendor\GLFW\lib-vc2022"
REM set Libs=-lglfw3_mt
REM set CommonCompilerFlags=-Wno-deprecated-declarations -Wno-int-to-void-pointer-cast -g -DGLFW_STATIC
REM set CommonLinkerFlags=-Wl, %LibPaths% %Libs% -luser32 -lgdi32 -lshell32
REM
REM if not exist "build" mkdir "build"
REM
REM pushd build
REM
REM echo Building x64 with Clang...
REM clang %IncludeDirs% %CommonCompilerFlags% "..\src\windows_buoyantui.c" "..\vendor\glad\src\gl.c" %CommonLinkerFlags% -o windows_buoyantui.exe
REM echo Build complete
REM
REM popd

REM MSVC BUILD SCRIPT
REM set IncludeDirs=/I"..\vendor\GLFW\include" /I"..\vendor\glad\include" /I"..\vendor\cglm\include" /I"..\vendor\stb_image" /I"..\vendor\freetype-2.14.3\include"
REM
REM set LibPaths=/LIBPATH:"..\vendor\GLFW\lib-vc2022"
REM
REM set Libs=glfw3_mt.lib user32.lib gdi32.lib shell32.lib
REM
REM set CommonCompilerFlags=/DGLFW_STATIC /W3 /wd4996 /Zi /MT /nologo
REM
REM if not exist "build" mkdir "build"
REM
REM pushd build
REM
REM echo Building x64 with MSVC...
REM
REM cl %IncludeDirs% %CommonCompilerFlags% "..\src\windows_buoyantui.c" "..\vendor\glad\src\gl.c" /Fe:windows_buoyantui.exe /link %LibPaths% %Libs%
REM
REM if errorlevel 1 (
REM     echo Build failed
REM     popd
REM     exit /b 1
REM )
REM
REM echo Build complete
REM
REM popd
REM

if not exist "build" mkdir "build"

echo Building x64 with CBUILD

call build\cbuild.exe


