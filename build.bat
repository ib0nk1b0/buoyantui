
@echo off

REM set IncludeDirs=-I"..\..\vendor\GLFW\include" -I"..\..\vendor\glad\include" -I"..\..\vendor\cglm\include" -I"..\..\vendor\stb_image"
REM set LibPaths=-L"..\..\vendor\GLFW\lib-vc2022"
set IncludeDirs=-I"..\vendor\GLFW\include" -I"..\vendor\glad\include" -I"..\vendor\cglm\include" -I"..\vendor\stb_image" -I"..\vendor\freetype-2.14.3\include"
set LibPaths=-L"..\vendor\GLFW\lib-vc2022"
set Libs=-lglfw3_mt

REM set CommonCompilerFlags=-std=c++17 -Wall -Wextra -Wpedantic -Wno-unknown-pragmas -O2 -g -DGLFW_STATIC
REM set CommonCompilerFlags=-std=c++17 -Wall -Wextra -Wpedantic -Werror -Wno-unknown-pragmas -Wno-writable-strings -Wno-unused-parameter -Wno-unused-variable -Wno-unused-value -Wno-unneeded-internal-declaration -O2 -g -DGLFW_STATIC
REM set CommonCompilerFlags=-std=c++17 -Wall -Wextra -Wpedantic -Werror -Wno-unknown-pragmas -Wno-writable-strings -Wno-unused-parameter -Wno-unused-variable -Wno-unused-value -Wno-unneeded-internal-declaration -O0 -g -DGLFW_STATIC


REM set CommonCompilerFlags=-Wall -Wextra -Wpedantic -Werror -Wno-unknown-pragmas -Wno-writable-strings -Wno-unused-parameter -Wno-unused-variable -Wno-unused-value -Wno-unneeded-internal-declaration -O0 -g -DGLFW_STATIC
set CommonCompilerFlags=-Wno-deprecated-declarations -Wno-int-to-void-pointer-cast -g -DGLFW_STATIC

REM set CommonLinkerFlags=-Wl,--gc-sections %LibPaths% %Libs% -luser32 -lgdi32 -lshell32
set CommonLinkerFlags=-Wl, %LibPaths% %Libs% -luser32 -lgdi32 -lshell32

if not exist "build" mkdir "build"

pushd build

echo Building x64 with Clang...
clang %IncludeDirs% %CommonCompilerFlags% "..\src\windows_buoyantui.c" "..\vendor\glad\src\gl.c" %CommonLinkerFlags% -o windows_buoyantui.exe
echo Build complete

popd
