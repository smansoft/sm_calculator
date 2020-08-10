@echo off

rem
rem    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
rem                       Sergey Manoylo <info@smansoft.com>
rem

echo ""
echo "Comparing unit test (results) files and check (original) files:"
echo ""

for %%i in ( 1 2 3 4 5 6 7 8 ) do (

	echo "----------------------------------------------"

	echo "comparing files:"
	echo "check file  = sm_calc_test.check.0%%i.txt"
	echo "out file    = sm_calc_test.out.0%%i.txt"

	echo ""

	diff -s "sm_calc_test.check.0%%i.txt" "sm_calc_test.out.0%%i.txt"
)

echo "----------------------------------------------"
