#!/usr/bin/python3

#
#    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
#                       Sergey Manoylo <info@smansoft.com>
#

import sys;

num_diffs = 0;

def compare_files(in_fpath_fst, in_fpath_sec):
	global num_diffs;
	num_diffs = 0;
	print ("\ncomparing files:");
	print ("first in fpath  = %s" % in_fpath_fst);
	print ("second in fpath = %s\n" % in_fpath_sec);
	in_f_fst = open(in_fpath_fst, "r");
	in_f_sec = open(in_fpath_sec, "r");
	try:
		count = 0;
		while True:
			line_fst = in_f_fst.readline();
			line_sec = in_f_sec.readline();
			if line_fst == "" and line_sec == "":
				break;
			count += 1;
			line_fst = line_fst.replace("\n","");
			line_sec = line_sec.replace("\n","");
			lines_equals = True;
			if line_fst == "" and line_sec != "" or line_fst != "" and line_sec == "" or line_fst != line_sec:
				lines_equals = False;
				num_diffs += 1;
			if lines_equals is False:
				print ("--- lines ---");
				print ("%03d > %s" % (count, line_fst));
				print ("%03d < %s" % (count, line_sec));
	except Exception as ex:
		print(ex)
	in_f_fst.close()
	in_f_sec.close();
	return;

def main(argv):
	for i in (1,2,3,4,5,6,7):
		in_fpath_fst="./sm_calc_test.check.0%d.txt" % i;
		in_fpath_sec="./sm_calc_test.out.0%d.txt" % i;		
		compare_files(in_fpath_fst, in_fpath_sec);

		if num_diffs == 0:
			print("------------------");
			print("files %s and %s are identical" % (in_fpath_fst, in_fpath_sec));
		else:
			print("\n------------------");
			print("files %s and %s are different" % (in_fpath_fst, in_fpath_sec));

if __name__ == "__main__":
	main(sys.argv[1:]);
