@echo off

REM build.bat is to build the current projects things into a useable exe

REM PROGRAM FLAGS are || Test -MTd seems to be giving error
REM SPECTOR_ENGINE_DEBUG_MODE || Is the thing being built in a debug mode? Asserts on

set internalProgramFlags= SPECTOR_ENGINE_DEBUG_MODE

set WarningFlags= -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505
set Flags= -Zi -EHsc -nologo -fp:fast -Gm- -GR- -EHa- -Od -Oi %WarningFlags%

set LibsLinkedTo= user32.lib Gdi32.lib OpenGL32.lib ..\MiddleMan\glew\glew32s.lib
set LinkerFlags= -incremental:no -opt:ref -NODEFAULTLIB:library %LibsLinkedTo%

IF NOT EXIST build mkdir build
pushd build

REM Clearing debug info so new information can be correctly stored

REM MSVC build
cl %Flags% ..\win32platform.cpp /link %LinkerFlags%
popd
