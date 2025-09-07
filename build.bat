@echo off

set c_opts=-nologo -c -Zi -std:c++latest -MDd -Fo./obj/ -TP -I./include/ -I./include/dxc/ -I./include/SDL3/
set lnk_opts=/NOLOGO /DEBUG /LIBPATH:./libs/

cl %c_opts% main.cpp days/day%1.cpp -DDAY=%1

link.exe %lnk_opts% ./obj/main.obj ./obj/day%1.obj ^
    SDL3.lib dxcompiler.lib ^
    /OUT:aoc2025.exe
