@echo off & setlocal
md generated 2>nul
set PS-UTF8-OUT=out-file -encoding utf8 generated\manifest.xml
powershell -c ^
    "(type template\application-manifest.xml) -replace 'APP_NAME', 'hello-world' | %PS-UTF8-OUT%"

md intermediary 2>nul
windres -i main.rc -o intermediary\resources.o

set GOODNESS=-std=c++17 -pedantic-errors -Wall -s
set GUI_SUBSYSTEM=-mwindows
g++ %GOODNESS% main.cpp c-mainfunc.standard.cpp intermediary\resources.o %GUI_SUBSYSTEM% -o "hello-world.exe"
