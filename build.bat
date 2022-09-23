
@ECHO OFF

REM SET LIB_DIRS=/LIBPATH:..\deps\glfw3\libvc2019

REM SET LIBS=kernel32 GDI32.lib shll32.lib user32.lib opengl32.lib glfw3_mt.lib glew32s.lib

REM SET INCLUDE DIRS=/I ..\deps\glfw3\include

SET SRC_FILES=..\src\*.cpp
SET EXECUTABLE=lib8085test.exe

SET CFLAGS=/EHsc /Zi /nologo /Fe"lib8085test"

pushd .
mkdir build
cd build

cl %SRC_FILES% %CFLAGS%

if ERRORLEVEL 1 GOTO EXIT
call lib8085test.exe

:EXIT
popd
