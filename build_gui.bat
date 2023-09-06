
@ECHO OFF

SET LIB_DIRS=/LIBPATH:..\thirdparty\glfw\lib-vc2010-64

SET LIBS= glfw3.lib kernel32.lib opengl32.lib gdi32.lib shell32.lib

SET INCLUDE_DIRS=/I..\thirdparty\glfw\include /I..\thirdparty\imgui\backends /I..\thirdparty\imgui

SET SRC_FILES=..\src\assembler.cpp ..\src\assembler_util.cpp ..\src\lib8085.cpp ..\src\gui\app.cpp ..\thirdparty\imgui\backends\imgui_impl_glfw.cpp ..\thirdparty\imgui\backends\imgui_impl_opengl3.cpp ..\thirdparty\imgui\imgui*.cpp 
SET MAIN_FILE=..\src\gui\main.cpp

SET CFLAGS=/EHsc /MD /Zi /nologo /Fe"retro85" 

pushd .
mkdir build
cd build

cl %SRC_FILES% %MAIN_FILE% %INCLUDE_DIRS% %CFLAGS% /link %LIB_DIRS% %LIBS%

if ERRORLEVEL 1 GOTO EXIT

call retro85.exe

:EXIT
popd
