@echo off

REM build.bat is to build the current projects things into a useable exe

REM PROGRAM FLAGS are || Test -MTd seems to be giving error
REM SPECTOR_ENGINE_DEBUG_MODE || Is the thing being built in a debug mode? Asserts on

set buildDIR=\build\
set internalProgramFlags= SPECTOR_ENGINE_DEBUG_MODE=1
set outputDirectories=/Fd.%buildDIR% /Fe.%buildDir% /Fo.%buildDIR%

set WarningFlags= -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505
set Flags= -Zi -EHsc -nologo -fp:fast -Gm- -GR- -EHa- -Od %WarningFlags% %outputDirectories%

set LibsLinkedTo= user32.lib Gdi32.lib C:\VulkanSDK\1.0.39.1\Source\lib32\vulkan-1.lib
set LinkerFlags= -incremental:no -opt:ref -NODEFAULTLIB:library %LibsLinkedTo%

IF NOT EXIST build mkdir build

REM Clearing debug info so new information can be correctly stored
del .%buildDIR%*.pdb

REM MSVC build
cl %Flags% win32platform.cpp /link %LinkerFlags%
popd
