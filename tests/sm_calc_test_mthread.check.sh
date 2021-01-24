#!/bin/bash

#
#    Copyright (c) 2020-2021 SManSoft <http://www.smansoft.com/>
#                       Sergey Manoylo <info@smansoft.com>
#

echo;
echo "Comparing unit test (results) files and check (original) files:";
echo;

num_threads=10;

for i in 1 2 3 4 5 6 7 8 ; do 
	th_i=0;
	while :
	do
		if [ $th_i -ge $num_threads ]; then
			break;
		fi;
		out_fname_1="./sm_calc_test.out.0${i}.txt";
		if [ $th_i -le 9 ]; then
			out_fname_2="./sm_calc_test.out.0${i}.00${th_i}.txt";
		else
			out_fname_2"./sm_calc_test.out.0${i}.0${th_i}.txt";
		fi;
		th_i=$((th_i+1));

		echo "----------------------------------------------";
		echo "comparing files:";
		echo "out_fname_1  = ${out_fname_1}";
		echo "out_fname_2  = ${out_fname_2}";
		echo;

		diff -s "${out_fname_1}" "${out_fname_2}";
	done
done;

echo "----------------------------------------------";
