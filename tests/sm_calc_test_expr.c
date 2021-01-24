/*
 *    Copyright (c) 2020-2021 SManSoft <http://www.smansoft.com/>
 *    Sergey Manoylo <info@smansoft.com>
 */

#include "pch.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include "cmocka.h"

#include "safe_fopen.h"
#include "safe_str_lib.h"
#include "safe_lib_errno.h"

#include "sm_log.h"

#include "sm_calc_types.h"
#include "sm_calc.h"
#include "sm_calc_proc.h"

static const float gcsm_f_test_epsilon = 1e-10f;	//	inaccuracy (margin) constant, that is used during comparing of float values in test functions
													//	i.e. if (float a1 - float a2) < margin constant, float a1 == float a2

static const char* const gsm_test_out = "sm_calc_test.out.expr.txt";	//	temporary file name/path, that contains execution result of sm_calc_test_expr unit test

#define SM_LOG_FPATH		"./sm_calc_test_expr.log"	//	log file path

extern sm_log_config gsm_log_config;	//	global instance of main log support structure
#define SM_LOG_CONFIG &gsm_log_config	//	just synonym: SM_LOG_CONFIG == &gsm_log_config - for usage in log api calls

extern FILE* gsm_yyout;					//	stream file, that is used for temprorary saving the output stream (is used by unit tests)
extern FILE* gsm_yyin;					//	stream file, that is used for temprorary saving the input stream (is used by unit tests)

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
		- opens temporary file (using gsm_test_out name/path),
			that will contain execution result of sm_calc_test_expr unit test;
			yyout file stream devices is used;
		- initializing of command line parameters (int argc; char* argv[2];);
			--expression="tested equation" is used;
		- call original main() in module sm_calc/sm_calc.c with command line
			parametersm initialized prevoiusly; during this build, original main()
			in module sm_calc/sm_calc.c will be compiled with name
			int sm_calc_main(int argc, char* argv[]);
			sm_calc_main (during execution) provides write out to opened
			temporary file (using gsm_test_out name/path);
		- closes temporary file;
		- call the function: void sm_calc_equation_check_res(const long double check_res_value),
			that compares this result with checking/correct result value;
*/
static void sm_calc_equation_test(void** state);

/*
	the function receives checking/correct result value of processing
	of command line expression,
	the function opens temporary file gsm_test_out (file path),
	parses this file, searches solved result here and then
	compares this result with checked/correct result value;

	in args:
		check_res_value		- checking/correct result value of processing
								of command line expression
*/
static void sm_calc_equation_check_res(sm_parser_ctx* const parser_ctx, const long double check_res_value);

/*		Test functions		*/

/*
	the function receives checking/correct result value of processing
	of command line expression,
	the function opens temporary file gsm_test_out (file path),
	parses this file, searches solved result here and then
	compares this result with checking/correct result value;

	in args:
		check_res_value		- checking/correct result value of processing
								of command line expression
*/
static void sm_calc_equation_check_res(sm_parser_ctx* const parser_ctx, const long double check_res_value)
{
	errno_t err = SM_RES_OK;

	char buf[SM_L_BUFF_SIZE];
	FILE* res_file_h;
	char* data_p;

	err = safe_fopen(&res_file_h, gsm_test_out, "r", SM_SFOPEN_NSHARED_TYPE);
	assert_int_equal(err, SAFE_RES_OK);
	assert_true(res_file_h != NULL);

	char* res_str;
	const char* const out_str = "Out >> ";
	size_t len = safe_strnlen(out_str, SM_L_BUFF_SIZE);
	int count = 0;
	long double res_value_d = 0.0L;

	while (TRUE) {
		if (feof(res_file_h))
			break;
		data_p = fgets(buf, SM_ARRAY_SIZE(buf), res_file_h);
		if (!data_p)
			break;
		err = safe_strstr(data_p, SM_L_BUFF_SIZE, out_str, len, &res_str);
		if (err == EOK) {
			char* subs_str;
			char* ptr;
			len = safe_strnlen(res_str, SM_L_BUFF_SIZE);
			subs_str = safe_strtok(res_str, &len, " ;\n", &ptr);
			while (subs_str != NULL && err == SM_RES_OK) {
				count++;
				subs_str = safe_strtok(NULL, &len, " ;\n", &ptr);
				if (count == 2) {
					len = safe_strnlen(subs_str, SM_L_BUFF_SIZE);
					err = sm_conv_sz_2_f_dec(parser_ctx, subs_str, len, &res_value_d);
					assert_int_equal(err, SAFE_RES_OK);
					break;
				}
			}
		}
	}
	fclose(res_file_h);
	assert_float_equal(res_value_d, check_res_value, gcsm_f_test_epsilon);
	return;
}

/*
	the function provides initializing of global instance of global instance of definition the current state
	of sm_calculator configuration;
	unit test code and all functions in sm_calc project/module use same instance of current state
	of sm_calculator configuration
*/
static void sm_calc_init_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s", "test", __FUNCTION__, "is started");

	(void)state;

	sm_parser_ctx parser_ctx_o;
	sm_parser_ctx* parser_ctx = &parser_ctx_o;

	sm_init_parser_ctx(&parser_ctx_o);

	assert_int_equal(parser_ctx->m_calc_params.m_f_precision, SM_PREC_DEF);
	assert_int_equal(parser_ctx->m_calc_params.m_i_format, SM_I_FORMAT_DEF);
	assert_int_equal(parser_ctx->m_calc_params.m_trig_unit, SM_TRIG_UNIT_DEF);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s", "test", __FUNCTION__, "is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");

	return;
}

/*
	the function provides follow testing steps:
		- opens temporary file (using gsm_test_out name/path),
			that will contain execution result of sm_calc_test_expr unit test;
			yyout file stream devices is used;
		- initializing of command line parameters (int argc; char* argv[2];);
			--expression="tested equation" is used;
		- call original main() in module sm_calc/sm_calc.c with command line
			parametersm initialized prevoiusly; during this build, original main()
			in module sm_calc/sm_calc.c will be compiled with name
			int sm_calc_main(int argc, char* argv[]);
			sm_calc_main (during execution) provides write out to opened
			temporary file (using gsm_test_out name/path);
		- closes temporary file;
		- call the function: void sm_calc_equation_check_res(const long double check_res_value),
			that compares this result with checking/correct result value;
*/
static void sm_calc_equation_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s", "test", __FUNCTION__, "is started");

	errno_t err = SM_RES_OK;

	char buf[SM_L_BUFF_SIZE];
	int argc = 2;
	char* argv[2];

	(void)state;

	sm_parser_ctx parser_ctx_o;
	sm_parser_ctx* parser_ctx = &parser_ctx_o;

	sm_init_parser_ctx(&parser_ctx_o);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "opening the output file: %s", gsm_test_out);

	gsm_yyin = stdin;

	err = safe_fopen(&gsm_yyout, gsm_test_out, "w", SM_SFOPEN_NSHARED_TYPE);
	assert_int_equal(err, SAFE_RES_OK);
	assert_true(gsm_yyout != NULL);

	const char* const equation = "pow_^2(sin(pi/2))+pow_^2(cos(pi/2))+24.*2/12.0+exp-5.0;";
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "test equation: %s", equation);

	snprintf(buf, SM_ARRAY_SIZE(buf), "--expression=%s", equation);

	argv[0] = "sm_calc";
	argv[1] = buf;

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "calling the original entry point: sm_calc_main");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "arguments: argc = %d", argc);
	for (int i = 0; i < argc; i++)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "arguments: argv[%d] = %s", i, argv[0]);

	int res = sm_calc_main(argc, argv);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_main: res = %d", res);
	assert_int_equal(res, 0);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "closing the output file: %s", gsm_test_out);
	fclose(gsm_yyout);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "checking the result of processing the equation...");

	long double exp_value;

	err = sm_get_const(parser_ctx, SM_CONST_EXP, &exp_value);
	assert_int_equal(err, SAFE_RES_OK);

	sm_calc_equation_check_res(parser_ctx, exp_value);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s", "test", __FUNCTION__, "is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
}

/*	main entry point of the unit test module */
int main(int argc, char* argv[])
{
	/*
		array of mocka unit tests, pointers of test functions:
		void (*CMUnitTestFunction)(void **state)
	*/
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(sm_calc_init_test),
		cmocka_unit_test(sm_calc_equation_test),
	};

	sm_parser_ctx parser_ctx_o;
	sm_parser_ctx* parser_ctx = &parser_ctx_o;

	sm_init_parser_ctx(&parser_ctx_o);

	sm_init_log_test();			//	initializing and starting the log output (file path, defined by SM_LOG_DPATH and SM_LOG_FNAME is used)

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s", __FUNCTION__, "---------------------------------------------------------------------- >>");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test_expr is started");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");

	sm_init_random();					//	initializing the random seed (for 'rand[;]' command)
	sm_set_def_calc_params(parser_ctx);	//	initializing the global sm_calc_params object
	sm_log_calc_params(parser_ctx);		//	provides log out (in log file) the sm_calc_params gsm_calc_params, current state of sm_calculator configuration

	int res = cmocka_run_group_tests(tests, NULL, NULL);	//	execution of unit tests (array of void (*CMUnitTestFunction)(void **state) functions)
															//	the function cmocka_run_group_tests returns: total_failed + total_errors
															//	i.e. if result of execution of unit tests is OK, int res == 0

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "cmocka_run_group_tests: result = %d", res);
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test_expr is finished");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s", __FUNCTION__, "---------------------------------------------------------------------- <<");

	sm_close_log_test();		//	stopping and closing the log output

	return res;
}

/*	initializing and starting the log output (SM_LOG_DPATH and SM_LOG_FNAME - init params)	*/
errno_t	sm_init_log_test()
{
	errno_t err = SM_RES_OK;
	err = sm_log_init_fpath(SM_LOG_CONFIG, SM_LOG_FPATH);
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
