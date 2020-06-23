@echo off

rem
rem    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
rem                       Sergey Manoylo <info@smansoft.com>
rem

win_flex.exe -L -i --nounistd -o sm_calc\sm_calc_l.c sm_calc\sm_calc.l
win_bison.exe -l -d -v -o sm_calc\sm_calc_y.c sm_calc\sm_calc.y
