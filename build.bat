@echo off

set c_opts=-nologo -c -Zi -std:c++latest -MDd -Fo./obj/ -TP
set lnk_opts=/NOLOGO /DEBUG /LIBPATH:./libs/

cl %c_opts% main.cpp days/day%1.cpp -DDAY=%1

link.exe %lnk_opts% ./obj/main.obj ./obj/day%1.obj /OUT:aoc2025.exe
