@echo off

set c_opts=-nologo -Zi -MDd -Fo./obj/ -TP

cl %c_opts% main.cpp days/day%1.cpp -DDAY=%1 -link -out:aoc2025.exe
