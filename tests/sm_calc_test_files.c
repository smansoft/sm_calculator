/*
 *    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
 *    Sergey Manoylo <info@smansoft.com>
 */

#include "pch.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include "cmocka.h"

#include "safe_fopen.h"

#include "sm_log.h"

#include "sm_calc_types.h"
#include "sm_calc.h"
#include "sm_calc_proc.h"

#define SM_LOG_DPATH		"."							//	default directory path, where log file will be created (current directory)
#define SM_LOG_FNAME		"sm_calc_test_files.log"	//	log file name

extern sm_log_config gsm_log_config;	//	global instance of main log support structure
#define SM_LOG_CONFIG &gsm_log_config	//	just synonym: SM_LOG_CONFIG == &gsm_log_config - for usage in log api calls

const char* gsm_fpath_in;				//	global variable, that contains input file name/path for file processing mode of sm_calculator
const char* gsm_fpath_out;				//	global variable, that contains output file name/path for file processing mode of sm_calculator

/*	initializing and starting the log output (SM_LOG_DPATH and SM_LOG_FNAME - init params)	*/
errno_t	sm_init_log_test();

/*	stopping and closing the log output	*/
errno_t	sm_close_log_test();

/*		Test functions		*/

/*
	initializing of global instance of global instance of definition the current state of sm_calculator configuration;
	unit test code and all functions in sm_calc project/module use same instance of current state
	of sm_calculator configuration
*/
static void sm_calc_init_test(void** state);

/*
	the function provides follow testing steps:
		- initializing of command line parameters (int argc; char* argv[3];);
			--in_file=gsm_fpath_in
			--out_file=gsm_fpath_out
			are used;
		- call original main() in module sm_calc/sm_calc.c with command line
			parametersm initialized prevoiusly; during this build, original main()
			in module sm_calc/sm_calc.c will be compiled with name
			int sm_calc_main(int argc, char* argv[]);
			as options --in_file and --out_file are used, sm_calc_main
			opens and processing file gsm_fpath_in
			and opens and output result to file gsm_fpath_out;

		developer should use scripts
			sm_calc_test_files.check.bat
			sm_calc_test_files.check.sh
			sm_calc_test_files.check.py
		for follow analysis of results of unit test execution

		please, read README and INSTALL files for more info...
*/
static void sm_calc_files_processing_test(void** state);

/*		Test functions		*/

/*
	initializing of global instance of global instance of definition the current state of sm_calculator configuration;
	unit test code and all functions in sm_calc project/module use same instance of current state
	of sm_calculator configuration
*/
static void sm_calc_init_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	(void)state;

	sm_set_def_calc_params();

	assert_int_equal(gsm_calc_params.m_f_precision, SM_PREC_DEF);
	assert_int_equal(gsm_calc_params.m_i_format, SM_I_FORMAT_DEF);
	assert_int_equal(gsm_calc_params.m_trig_unit, SM_TRIG_UNIT_DEF);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");

	return;
}

/*
	the function provides follow testing steps:
		- initializing of command line parameters (int argc; char* argv[3];);
			--in_file=gsm_fpath_in
			--out_file=gsm_fpath_out
			are used;
		- call original main() in module sm_calc/sm_calc.c with command line
			parametersm initialized prevoiusly; during this build, original main()
			in module sm_calc/sm_calc.c will be compiled with name
			int sm_calc_main(int argc, char* argv[]);
			as options --in_file and --out_file are used, sm_calc_main
			opens and processing file gsm_fpath_in
			and opens and output result to file gsm_fpath_out;

		developer should use scripts 
			sm_calc_test_files.check.bat
			sm_calc_test_files.check.sh
			sm_calc_test_files.check.py
		for follow analysis of results of unit test execution

		please, read README and INSTALL files for more info...
*/
static void sm_calc_files_processing_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	(void)state;

	char	buf_in[SM_L_BUFF_SIZE];
	char	buf_out[SM_L_BUFF_SIZE];

	int argc = 3;
	char* argv[3];

	snprintf(buf_in, SM_ARRAY_SIZE(buf_in), "--in_file=%s", gsm_fpath_in);
	snprintf(buf_out, SM_ARRAY_SIZE(buf_out), "--out_file=%s", gsm_fpath_out);

	argv[0] = "sm_calc";
	argv[1] = buf_in;
	argv[2] = buf_out;

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "calling the original entry point: sm_calc_main");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "arguments: argc = %d", argc);
	for (int i = 0; i < argc; i++)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "arguments: argv[%d] = %s", i, argv[0]);

	int res = sm_calc_main(argc, argv);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_main: res = %d", res);
	assert_int_equal(res, 0);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");

	return;
}

/*	main entry point of the unit test module */
int main(int argc, char* argv[])
{
	sm_init_log_test();			//	initializing and starting the log output (file path, defined by SM_LOG_DPATH and SM_LOG_FNAME is used)

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__,  "%s %s", __FUNCTION__, "---------------------------------------------------------------------- >>");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test_files is started");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");

	/*
		this module should receive 3 params, as it should be called:
		sm_calc_test_files.exe <input_file_path> <output_file_path>
	*/
	if (argc < 3) {
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "wrong number of parameters: argc = %d", argc);

		printf("wrong number of parameters:\n");
		printf("sm_calc_test_files.exe <input_file_path> <output_file_path>\n");
		return 1;
	}

	/*
		array of mocka unit tests, pointers of test functions:
		void (*CMUnitTestFunction)(void **state)
	*/
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(sm_calc_init_test),
		cmocka_unit_test(sm_calc_files_processing_test),
	};

	sm_init_random();			//	initializing the random seed (for 'rand[;]' command)
	sm_set_def_calc_params();	//	initializing the global sm_calc_params object
	sm_log_calc_params();		//	provides log out (in log file) the sm_calc_params gsm_calc_params, current state of sm_calculator configuration

	/*
		initializing of 
			const char* gsm_fpath_in;		-	global variable, that contains input file name/path for file processing mode of sm_calculator
			const char* gsm_fpath_out;		-	global variable, that contains output file name/path for file processing mode of sm_calculator

		by arguments of call:
			sm_calc_test_files.exe <input_file_path> <output_file_path>
		
		i.e.
			gsm_fpath_in = input_file_path
			gsm_fpath_out = output_file_path
	*/
	gsm_fpath_in = argv[1];
	gsm_fpath_out = argv[2];
	printf("sm_calc_test_files:\n");
	printf("input    file: %s\n", gsm_fpath_in);
	printf("output   file: %s\n", gsm_fpath_out);

	int res = cmocka_run_group_tests(tests, NULL, NULL);		//	execution of unit tests (array of void (*CMUnitTestFunction)(void **state) functions)
																//	the function cmocka_run_group_tests returns: total_failed + total_errors
																//	i.e. if result of execution of unit tests is OK, int res == 0

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "cmocka_run_group_tests: res = %d", res);
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test_files is finished");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s",__FUNCTION__, "---------------------------------------------------------------------- <<");

	sm_close_log_test();		//	stopping and closing the log output

	return res;
}

/*	initializing and starting the log output (SM_LOG_DPATH and SM_LOG_FNAME - init params)	*/
errno_t	sm_init_log_test()
{
	errno_t err = SM_RES_OK;
	err = sm_log_init(SM_LOG_CONFIG, SM_LOG_DPATH, SM_LOG_FNAME);
	if (err == SM_RES_OK)
		err = sm_log_start(SM_LOG_CONFIG);
	return err;
}

/*	stopping and closing the log output	*/
errno_t	sm_close_log_test()
{
	sm_log_stop(SM_LOG_CONFIG);
	return sm_log_close(SM_LOG_CONFIG);
}
