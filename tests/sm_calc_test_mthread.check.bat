@echo off

rem
rem    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
rem                       Sergey Manoylo <info@smansoft.com>
rem

echo ""
echo "Comparing unit test (results) files and check (original) files:"
echo ""

set num_threads=9

set /a j_loop_max=%num_threads%-1

for %%i in ( 1 2 3 4 5 6 7 8 ) do (
    for /l %%j in (0,1,%j_loop_max%) do (call :compFiles %%i %%j)
)

goto :eof

:compFiles
        set check_fname_1="sm_calc_test.check.0%1.txt"
	set check_fname_2="sm_calc_test.out.0%1.00%2.txt"

        if %2 lss 10 (
             set check_fname_2="sm_calc_test.out.0%1.00%2.txt"
        ) else (
             set check_fname_2="sm_calc_test.out.0%1.0%2.txt"
        )

     	echo "----------------------------------------------"

        echo "comparing files:"
        echo "check_fname_1  = %check_fname_1%"
        echo "check_fname_2  = %check_fname_2%"

        echo ""

        diff -s "%check_fname_1%" "%check_fname_2%"

	goto :eof

echo "----------------------------------------------"

exit /b /0
