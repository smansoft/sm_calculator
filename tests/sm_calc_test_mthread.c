/*
 *    Copyright (c) 2020-2021 SManSoft <http://www.smansoft.com/>
 *    Sergey Manoylo <info@smansoft.com>
 */

#include "pch.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#ifdef SM_OS_LINUX
	#include <pthread.h>
#endif

#include "cmocka.h"

#include "safe_fopen.h"
#include "safe_str_lib.h"

#include "sm_log.h"

#include "sm_calc_types.h"
#include "sm_calc.h"
#include "sm_calc_proc.h"

#define SM_LOG_FPATH		"./sm_calc_test_mthread.log"	//	log file path

extern sm_log_config gsm_log_config;	//	global instance of main log support structure
#define SM_LOG_CONFIG &gsm_log_config	//	just synonym: SM_LOG_CONFIG == &gsm_log_config - for usage in log api calls

extern FILE* gsm_yyout;
extern FILE* gsm_yyin;

char* gsm_fpath_in;							//	global variable, that contains input file name/path for file processing mode of sm_calculator
char* gsm_fpath_out;						//	global variable, that contains output file name/path for file processing mode of sm_calculator

const char* gcsm_str_mask = "XXX";			//	global variable, that contains output file name/path for file processing mode of sm_calculator

#define SM_TEST_NUM_THREADS 10

typedef struct _sm_calc_test_params
{
	int m_id;
	char m_fpath_in[SM_L_BUFF_SIZE];
	char m_fpath_out[SM_L_BUFF_SIZE];
} sm_calc_test_params, * psm_calc_test_params;

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
static void sm_calc_mthread_processing_test(void** state);

errno_t gsm_tst_results[SM_TEST_NUM_THREADS] = { 0 };

#if defined SM_OS_WINDOWS

HANDLE gsm_tst_started_evnts_h[SM_TEST_NUM_THREADS] = { NULL };
HANDLE gsm_tst_finished_mtxs_h[SM_TEST_NUM_THREADS] = { NULL };

DWORD WINAPI sm_calc_test_thread(LPVOID param);
errno_t start_test_threads();

DWORD WINAPI sm_calc_test_thread(LPVOID param)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test_thread -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s", "sm_calc_test_thread", __FUNCTION__, "is started");

	errno_t res = 0;
	DWORD mtx_res = 0;

	sm_calc_test_params* test_params = (sm_calc_test_params*)param;

	while (test_params) {

		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "thread id: test_params->m_id = %d", test_params->m_id);

		mtx_res = WaitForSingleObject(gsm_tst_finished_mtxs_h[test_params->m_id], INFINITE);
		if (mtx_res != WAIT_OBJECT_0) {
			res = 1;
			break;
		}
		if (gsm_tst_started_evnts_h[test_params->m_id])
			SetEvent(gsm_tst_started_evnts_h[test_params->m_id]);

		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------------- >>");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "thread id: test_params->m_id = %d exec iteration is started...", test_params->m_id);

		// make testing

		sm_parser_ctx parser_ctx_o;
		sm_parser_ctx* parser_ctx = &parser_ctx_o;

		sm_init_parser_ctx(&parser_ctx_o);

		rsize_t len_fpath_out = safe_strnlen(test_params->m_fpath_out, SM_L_BUFF_SIZE);
		rsize_t len_mask = safe_strnlen(gcsm_str_mask, SM_L_BUFF_SIZE);

		char* mask_update = (char*)malloc(len_mask+1);
		char* res_str;

		errno_t err = safe_strstr(test_params->m_fpath_out, len_fpath_out, gcsm_str_mask, len_mask, &res_str);
		if (err || !res_str) {
			res = 1;
			break;
		}

		snprintf(mask_update, len_mask+1, "%03d",test_params->m_id);

		for (int i = 0; i < len_mask; i++)
			res_str[i] = mask_update[i];

		free(mask_update);

		snprintf(parser_ctx->m_calc_params.m_in_file_path, SM_ARRAY_SIZE(parser_ctx->m_calc_params.m_in_file_path), "%s", test_params->m_fpath_in);
		snprintf(parser_ctx->m_calc_params.m_out_file_path, SM_ARRAY_SIZE(parser_ctx->m_calc_params.m_out_file_path), "%s", test_params->m_fpath_out);

		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "calling the original entry point: sm_calc_main");

		if (err != SM_RES_OK)
			break;
		err = sm_init_f_prec_buf(parser_ctx);
		if (err != SM_RES_OK)
			break;
		err = sm_proc_params(parser_ctx);
		if (err != SM_RES_OK)
			break;
		err = sm_proc_calc(parser_ctx);

		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "thread id: test_params->m_id = %d exec iteration is finished...", test_params->m_id);
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "thread id: test_params->m_id = %d exec iteration is finished...", 0);
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------------- <<");

		ReleaseMutex(gsm_tst_finished_mtxs_h[test_params->m_id]);

		gsm_tst_results[test_params->m_id] = err;

		break;
	}

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s", "sm_calc_test_thread", __FUNCTION__, "is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test_thread -------------------------------------- <<");

	return (DWORD)0;
}

errno_t start_test_threads()
{
	errno_t err = SM_RES_OK;

	HANDLE th_h[SM_TEST_NUM_THREADS] = { NULL };
	DWORD th_id[SM_TEST_NUM_THREADS] = { 0 };

	sm_calc_test_params* test_params = malloc(sizeof(sm_calc_test_params)*SM_TEST_NUM_THREADS);

	DWORD mtx_res = 0;

	while (true) {
		for (int i = 0; i < SM_TEST_NUM_THREADS; i++) {
			test_params[i].m_id = i;
			gsm_tst_results[i] = SM_RES_ERROR;
			safe_strcpy(test_params[i].m_fpath_in, SM_L_BUFF_SIZE, gsm_fpath_in);
			safe_strcpy(test_params[i].m_fpath_out, SM_L_BUFF_SIZE, gsm_fpath_out);
			gsm_tst_started_evnts_h[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
			if (!gsm_tst_started_evnts_h[i]) {
				err = SM_RES_ERROR;
				break;
			}
			gsm_tst_finished_mtxs_h[i] = CreateMutex(NULL, FALSE, NULL);
			if (!gsm_tst_finished_mtxs_h[i]) {
				err = SM_RES_ERROR;
				break;
			}
			sm_calc_test_params* test_params_p = &test_params[i];
			th_h[i] = CreateThread(NULL, 0, sm_calc_test_thread, (void*)test_params_p, 0, &th_id[i]);
			if(!th_h[i]) {
				err = SM_RES_ERROR;
				break;
			}
		}
		break;
	}

	WaitForMultipleObjects(SM_TEST_NUM_THREADS, gsm_tst_started_evnts_h, TRUE, INFINITE);
	WaitForMultipleObjects(SM_TEST_NUM_THREADS, gsm_tst_finished_mtxs_h, TRUE, INFINITE);
	WaitForMultipleObjects(SM_TEST_NUM_THREADS, th_h, TRUE, INFINITE);

	if (err == SM_RES_OK) {
		for (int i = 0; i < SM_TEST_NUM_THREADS; i++) {
			if (gsm_tst_results[i] != SM_RES_OK) {
				err = SM_RES_ERROR;
				break;
			}
		}
	}

	for (int i = 0; i < SM_TEST_NUM_THREADS; i++) {
		if (gsm_tst_started_evnts_h[i]) {
			CloseHandle(gsm_tst_started_evnts_h[i]);
			gsm_tst_started_evnts_h[i] = NULL;
		}
		if (gsm_tst_finished_mtxs_h[i]) {
			CloseHandle(gsm_tst_finished_mtxs_h[i]);
			gsm_tst_finished_mtxs_h[i] = NULL;
		}
	}

	free(test_params);

	return err;
}

#elif defined SM_OS_LINUX

pthread_mutex_t gsm_tst_started_mtxs[SM_TEST_NUM_THREADS];
pthread_cond_t  gsm_tst_started_cndts[SM_TEST_NUM_THREADS];

pthread_mutex_t gsm_tst_finished_mtxs[SM_TEST_NUM_THREADS];

void* sm_calc_test_thread(void* param);
errno_t start_test_threads();

void* sm_calc_test_thread(void* param)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test_thread -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s", "sm_calc_test_thread", __FUNCTION__, "is started");

	errno_t res = 0;

	sm_calc_test_params* test_params = (sm_calc_test_params*)param;

	while (test_params) {

		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "thread id: test_params->m_id = %d", test_params->m_id);

		pthread_mutex_lock(&gsm_tst_finished_mtxs[test_params->m_id]);

		pthread_mutex_lock(&gsm_tst_started_mtxs[test_params->m_id]);

		pthread_cond_signal(&gsm_tst_started_cndts[test_params->m_id]);

		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "thread id: test_params->m_id = %d thread started...", test_params->m_id);

		pthread_mutex_unlock(&gsm_tst_started_mtxs[test_params->m_id]);

		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------------- >>");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "thread id: test_params->m_id = %d exec iteration is started...", test_params->m_id);

		// make testing

		sm_parser_ctx parser_ctx_o;
		sm_parser_ctx* parser_ctx = &parser_ctx_o;

		sm_init_parser_ctx(&parser_ctx_o);

		rsize_t len_fpath_out = safe_strnlen(test_params->m_fpath_out, SM_L_BUFF_SIZE);
		rsize_t len_mask = safe_strnlen(gcsm_str_mask, SM_L_BUFF_SIZE);

		char* mask_update = (char*)malloc(len_mask + 1);
		char* res_str;

		errno_t err = safe_strstr(test_params->m_fpath_out, len_fpath_out, gcsm_str_mask, len_mask, &res_str);
		if (err || !res_str) {
			res = 1;
			break;
		}

		snprintf(mask_update, len_mask + 1, "%03d", test_params->m_id);

		for (int i = 0; i < len_mask; i++)
			res_str[i] = mask_update[i];

		free(mask_update);

		snprintf(parser_ctx->m_calc_params.m_in_file_path, SM_ARRAY_SIZE(parser_ctx->m_calc_params.m_in_file_path), "%s", test_params->m_fpath_in);
		snprintf(parser_ctx->m_calc_params.m_out_file_path, SM_ARRAY_SIZE(parser_ctx->m_calc_params.m_out_file_path), "%s", test_params->m_fpath_out);

		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "calling the original entry point: sm_calc_main");

		if (err != SM_RES_OK)
			break;
		err = sm_init_f_prec_buf(parser_ctx);
		if (err != SM_RES_OK)
			break;
		err = sm_proc_params(parser_ctx);
		if (err != SM_RES_OK)
			break;
		err = sm_proc_calc(parser_ctx);

		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "thread id: test_params->m_id = %d exec iteration is finished...", test_params->m_id);
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "thread id: test_params->m_id = %d exec iteration is finished...", 0);
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------------- <<");

		pthread_mutex_unlock(&gsm_tst_finished_mtxs[test_params->m_id]);

		gsm_tst_results[test_params->m_id] = err;

		break;
	}

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s", "sm_calc_test_thread", __FUNCTION__, "is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test_thread -------------------------------------- <<");

	return NULL;
}

errno_t start_test_threads()
{
	errno_t err = SM_RES_OK;
	int res;

	pthread_t pthreads[SM_TEST_NUM_THREADS];

	sm_calc_test_params* test_params = malloc(sizeof(sm_calc_test_params) * SM_TEST_NUM_THREADS);

	while (true) {
		for (int i = 0; i < SM_TEST_NUM_THREADS; i++) {
			res = pthread_mutex_init(&gsm_tst_started_mtxs[i], NULL);
			if (res) {
				err = SM_RES_ERROR;
				break;
			}
			res = pthread_cond_init(&gsm_tst_started_cndts[i], NULL);
			if (res) {
				err = SM_RES_ERROR;
				break;
			}
			pthread_mutex_lock(&gsm_tst_started_mtxs[i]);
			res = pthread_mutex_init(&gsm_tst_finished_mtxs[i], NULL);
			if (res) {
				err = SM_RES_ERROR;
				break;
			}
		}
		for (int i = 0; i < SM_TEST_NUM_THREADS; i++) {
			test_params[i].m_id = i;
			gsm_tst_results[i] = SM_RES_ERROR;
			safe_strcpy(test_params[i].m_fpath_in, SM_L_BUFF_SIZE, gsm_fpath_in);
			safe_strcpy(test_params[i].m_fpath_out, SM_L_BUFF_SIZE, gsm_fpath_out);
			res = pthread_create(&pthreads[i], NULL, sm_calc_test_thread, (void*)&test_params[i]);
			sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "thread %d has been created... res = %d", i, res);
			if (res) {
				err = SM_RES_ERROR;
				break;
			}
		}
		break;
	}

	for (int i = 0; i < SM_TEST_NUM_THREADS; i++) {
		pthread_cond_wait(&gsm_tst_started_cndts[i], &gsm_tst_started_mtxs[i]);
		pthread_mutex_unlock(&gsm_tst_started_mtxs[i]);
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "thread %d started... ", i);
	}

	for (int i = 0; i < SM_TEST_NUM_THREADS; i++) {
		pthread_mutex_lock(&gsm_tst_finished_mtxs[i]);
		pthread_mutex_unlock(&gsm_tst_started_mtxs[i]);
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "thread %d finished... ", i);
	}

	for (int i = 0; i < SM_TEST_NUM_THREADS; i++)
		pthread_join(pthreads[i], NULL);

	if (err == SM_RES_OK) {
		for (int i = 0; i < SM_TEST_NUM_THREADS; i++) {
			if (gsm_tst_results[i] != SM_RES_OK) {
				err = SM_RES_ERROR;
				break;
			}
		}
	}

	for (int i = 0; i < SM_TEST_NUM_THREADS; i++) {
		pthread_mutex_destroy(&gsm_tst_started_mtxs[i]);
		pthread_mutex_destroy(&gsm_tst_finished_mtxs[i]);
		pthread_cond_destroy(&gsm_tst_started_cndts[i]);
	}

	free(test_params);

	return err;
}

#endif

/*		Test functions		*/

/*
	initializing of global instance of global instance of definition the current state of sm_calculator configuration;
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
static void sm_calc_mthread_processing_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s", "test", __FUNCTION__, "is started");

	(void)state;

	errno_t res = start_test_threads();

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "start_test_thread: res = %d", res);
	assert_int_equal(res, 0);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s", "test", __FUNCTION__, "is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");

	return;
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
		cmocka_unit_test(sm_calc_mthread_processing_test),
	};

	sm_init_log_test();			//	initializing and starting the log output (file path, defined by SM_LOG_DPATH and SM_LOG_FNAME is used)

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s", __FUNCTION__, "---------------------------------------------------------------------- >>");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test_mthread is started");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");

	/*
		this module should receive 3 params, as it should be called:
		sm_calc_test_files.exe <input_file_path> <output_file_path>
	*/
	if (argc < 3) {
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "wrong number of parameters: argc = %d", argc);

		printf("wrong number of parameters:\n");
		printf("sm_calc_test_mthread.exe <input_file_path> <output_file_path_mask>\n");
		return 1;
	}

	sm_parser_ctx parser_ctx_o;
	sm_parser_ctx* parser_ctx = &parser_ctx_o;

	sm_init_parser_ctx(&parser_ctx_o);

	sm_init_random();					//	initializing the random seed (for 'rand[;]' command)
	sm_set_def_calc_params(parser_ctx);	//	initializing the global sm_calc_params object
	sm_log_calc_params(parser_ctx);		//	provides log out (in log file) the sm_calc_params gsm_calc_params, current state of sm_calculator configuration

	/*
		initializing of
			const char* gsm_fpath_in;		-	global variable, that contains input file name/path for file processing mode of sm_calculator
			const char* gsm_fpath_out;		-	global variable, that contains output file name/path for file processing mode of sm_calculator

		by arguments of call:
			sm_calc_test_mthread.exe <input_file_path> <output_file_path>

		i.e.
			gsm_fpath_in = input_file_path
			gsm_fpath_out = output_file_path
	*/
	gsm_fpath_in = argv[1];
	gsm_fpath_out = argv[2];
	printf("sm_calc_test_mthread:\n");
	printf("input    file: %s\n", gsm_fpath_in);
	printf("output   file: %s\n", gsm_fpath_out);

 	int res = cmocka_run_group_tests(tests, NULL, NULL);		//	execution of unit tests (array of void (*CMUnitTestFunction)(void **state) functions)
																//	the function cmocka_run_group_tests returns: total_failed + total_errors
																//	i.e. if result of execution of unit tests is OK, int res == 0

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "cmocka_run_group_tests: result = %d", res);
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test_mthread is finished");
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
