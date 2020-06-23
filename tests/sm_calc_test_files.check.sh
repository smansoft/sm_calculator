#!/bin/bash

#
#    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
#                       Sergey Manoylo <info@smansoft.com>
#

echo;
echo "Comparing unit test (results) files and check (original) files:";
echo;

for i in 1 2 3 4 5 6 7 ; do 
	check_fname="./sm_calc_test.check.0${i}.txt";
	out_fname="./sm_calc_test.out.0${i}.txt";

	echo "----------------------------------------------";
	echo "comparing files:";
	echo "check file  = ${check_fname}";
	echo "out file    = ${out_fname}";
	echo;

	diff -s "${check_fname}" "${out_fname}";

done;

echo "----------------------------------------------";
