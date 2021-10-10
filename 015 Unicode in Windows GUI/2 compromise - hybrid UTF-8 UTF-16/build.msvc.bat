@echo off & setlocal
set CL=^
/nologo /utf-8 /EHsc /GR /permissive- /std:c++17 /Zc:__cplusplus /Zc:externC- /W4 /wd4459 ^
/D _CRT_SECURE_NO_WARNINGS=1 /D _STL_SECURE_NO_WARNINGS=1

cl main.cpp c-mainfunc.msvc-gui.cpp user32.lib /Fe"hello-world.exe"
