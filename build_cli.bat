
@ECHO OFF

SET LIB_DIRS=

SET LIBS=

SET INCLUDE_DIRS=

SET SRC_FILES=..\src\assembler.cpp ..\src\assembler_util.cpp ..\src\lib8085.cpp
SET MAIN_FILE=..\src\cli\main.cpp

SET CFLAGS=/EHsc /MD /Zi /nologo /Fe"retro85a"

pushd .
mkdir build
cd build

cl %SRC_FILES% %MAIN_FILE% %INCLUDE_DIRS% %CFLAGS% /link %LIB_DIRS% %LIBS%

if ERRORLEVEL 1 GOTO EXIT
call retro85a.exe -a exampleasm/hello.asm
call retro85a.exe -d exampleasm/hello.asm.retro85

:EXIT
popd
