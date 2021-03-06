#
#    Copyright (c) 2020-2021 SManSoft <http://www.smansoft.com/>
#                       Sergey Manoylo <info@smansoft.com>
#

add_test(sm_calc_test "sm_calc_test")

add_test(sm_calc_test_files_01 "sm_calc_test_files" "./sm_calc_test.in.01.txt" "./sm_calc_test.out.01.txt")
add_test(sm_calc_test_files_02 "sm_calc_test_files" "./sm_calc_test.in.02.txt" "./sm_calc_test.out.02.txt")
add_test(sm_calc_test_files_03 "sm_calc_test_files" "./sm_calc_test.in.03.txt" "./sm_calc_test.out.03.txt")
add_test(sm_calc_test_files_04 "sm_calc_test_files" "./sm_calc_test.in.04.txt" "./sm_calc_test.out.04.txt")
add_test(sm_calc_test_files_05 "sm_calc_test_files" "./sm_calc_test.in.05.txt" "./sm_calc_test.out.05.txt")
add_test(sm_calc_test_files_06 "sm_calc_test_files" "./sm_calc_test.in.06.txt" "./sm_calc_test.out.06.txt")
add_test(sm_calc_test_files_07 "sm_calc_test_files" "./sm_calc_test.in.07.txt" "./sm_calc_test.out.07.txt")
add_test(sm_calc_test_files_08 "sm_calc_test_files" "./sm_calc_test.in.08.txt" "./sm_calc_test.out.08.txt")
                                                                                                           
add_test(sm_calc_test_expr "sm_calc_test_expr")

add_test(sm_calc_test_log "sm_calc_test_log")

add_test(sm_calc_test_mthread_01 "sm_calc_test_mthread" "./sm_calc_test.in.01.txt" "./sm_calc_test.out.01.XXX.txt")
add_test(sm_calc_test_mthread_02 "sm_calc_test_mthread" "./sm_calc_test.in.02.txt" "./sm_calc_test.out.02.XXX.txt")
add_test(sm_calc_test_mthread_03 "sm_calc_test_mthread" "./sm_calc_test.in.03.txt" "./sm_calc_test.out.03.XXX.txt")
add_test(sm_calc_test_mthread_04 "sm_calc_test_mthread" "./sm_calc_test.in.04.txt" "./sm_calc_test.out.04.XXX.txt")
add_test(sm_calc_test_mthread_05 "sm_calc_test_mthread" "./sm_calc_test.in.05.txt" "./sm_calc_test.out.05.XXX.txt")
add_test(sm_calc_test_mthread_06 "sm_calc_test_mthread" "./sm_calc_test.in.06.txt" "./sm_calc_test.out.06.XXX.txt")
add_test(sm_calc_test_mthread_07 "sm_calc_test_mthread" "./sm_calc_test.in.07.txt" "./sm_calc_test.out.07.XXX.txt")
add_test(sm_calc_test_mthread_08 "sm_calc_test_mthread" "./sm_calc_test.in.08.txt" "./sm_calc_test.out.08.XXX.txt")

set_tests_properties(sm_calc_test sm_calc_test_expr sm_calc_test_log
						sm_calc_test_files_01 sm_calc_test_files_02 
						sm_calc_test_files_03 sm_calc_test_files_04
						sm_calc_test_files_05 sm_calc_test_files_06
						sm_calc_test_files_08
						sm_calc_test_mthread_01 sm_calc_test_mthread_02 
						sm_calc_test_mthread_03 sm_calc_test_mthread_04
						sm_calc_test_mthread_05 sm_calc_test_mthread_06
						sm_calc_test_mthread_08
						PROPERTIES WILL_FAIL "0")
set_tests_properties(sm_calc_test_files_07 sm_calc_test_mthread_07
						PROPERTIES WILL_FAIL "1") 
