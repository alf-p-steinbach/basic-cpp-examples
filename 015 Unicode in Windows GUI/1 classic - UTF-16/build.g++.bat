@echo off & setlocal
set GOODNESS=-std=c++17 -pedantic-errors -Wall -s
set GUI_SUBSYSTEM=-mwindows
g++ %GOODNESS% -D UNICODE main.cpp c-mainfunc.standard.cpp %GUI_SUBSYSTEM% -o "hello-world.exe"
