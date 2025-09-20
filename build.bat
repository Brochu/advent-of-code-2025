@echo off

set c_opts=-nologo -c -Zi -std:c++latest -EHsc -Fo./obj/ -TP -I./include/ -I./include/ispc/
set lnk_opts=/NOLOGO /DEBUG /LIBPATH:./libs/

cl %c_opts% ./src/main.cpp ./src/day%1.cpp -DDAY=%1

link.exe %lnk_opts% ./obj/main.obj ./obj/day%1.obj ^
    ispc.lib ^
    /OUT:aoc2025.exe
