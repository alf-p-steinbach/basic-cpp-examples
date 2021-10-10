@echo off & setlocal
md generated 2>nul
set PS-UTF8-OUT=out-file -encoding utf8 generated\manifest.xml
powershell -c ^
    "(type template\application-manifest.xml) -replace 'APP_NAME', 'hello-world' | %PS-UTF8-OUT%"

md intermediary 2>nul
rc /nologo /fo intermediary\resources.res main.rc


set CL=^
/nologo /utf-8 /EHsc /GR /permissive- /std:c++17 /Zc:__cplusplus /Zc:externC- /W4 /wd4459 ^
/D _CRT_SECURE_NO_WARNINGS=1 /D _STL_SECURE_NO_WARNINGS=1

cl main.cpp c-mainfunc.msvc-gui.cpp intermediary\resources.res user32.lib /Fe"hello-world.exe"
