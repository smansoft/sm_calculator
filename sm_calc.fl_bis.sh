#!/bin/bash

#
#    Copyright (c) 2020-2021 SManSoft <http://www.smansoft.com/>
#                       Sergey Manoylo <info@smansoft.com>
#

flex -L -i --nounistd -o sm_calc/sm_calc.l.c sm_calc/sm_calc.l;
bison -l -d -v -o sm_calc/sm_calc.y.c sm_calc/sm_calc.y;
