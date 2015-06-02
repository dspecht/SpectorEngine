@echo off

REM build.bat is to build the current projects things into a useable exe

REM PROGRAM FLAGS are       || Test -MTd seems to be giving error
REM SPECTOR_ENGINE_DEBUG_MODE --- Is the thing being built in a debug mode? Asserts on

set internalProgramFlags= SPECTOR_ENGINE_DEBUG_MODE

set commonWarningFlags= -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505
set commonFlags= -EHsc -nologo -fp:fast -Gm- -GR- -EHa- -Od -Oi %commonWarningFlags%

set commonLibsLinkedTo= user32.lib Gdi32.lib OpenGL32.lib ..\MiddleMan\glew\glew32s.lib
set commonLinkerFlags= -incremental:no -opt:ref -NODEFAULTLIB:library %commonLibsLinkedTo%

IF NOT EXIST build mkdir build
pushd build

REM MSVC build
cl %commonFlags% ..\win32platform.cpp /link %commonLinkerFlags%
popd
