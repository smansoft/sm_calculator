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
#include "safe_str_lib.h"
#include "safe_lib_errno.h"

#include "sm_log_types.h"
#include "sm_files_tools.h"
#include "sm_log.h"

#include "sm_calc_types.h"
#include "sm_calc.h"
#include "sm_calc_proc.h"

#define SM_LOG_DPATH		"."								//	default directory path, where log file will be created (current directory)
#define SM_LOG_FNAME		"sm_calc_test_log.log"			//	log file name

extern sm_log_config gsm_log_config;	//	global instance of main log support structure
#define SM_LOG_CONFIG &gsm_log_config	//	just synonym: SM_LOG_CONFIG == &gsm_log_config - for usage in log api calls

/*	initializing and starting the log output (SM_LOG_DPATH and SM_LOG_FNAME - init params)	*/
errno_t	sm_init_log_test();

/*	stopping and closing the log output	*/
errno_t	sm_close_log_test();

/*		Template functions		*/

/*
	Suite of template functions, which are called by unit test functions:
	void (*CMUnitTestFunction)(void **state)
*/

static void sm_make_path_abs_test_template(const char* const path, const char* const bin_path_marker);
static void sm_get_dpath_from_fpath_test_template(const char* const fpath, const char* const dpath_res, int res);
static void sm_get_fname_from_fpath_test_template(const char* const fpath, const char* const fname_res, int res);

/*		Test functions		*/

/*
	initializing of global instance of global instance of definition the current state of sm_calculator configuration;
	unit test code and all functions in sm_calc project/module use same instance of current state
	of sm_calculator configuration
*/
static void sm_calc_init_test(void** state);

/*
	Every function of follow unit tests provides testing the correspondent function from sm_log project
	(module sm_files_tools):

	checking, if argument path is an absulute:
		errno_t sm_path_is_abs(const char* const path, BOOL* const res);

	converting, a relative path to the absolute path:
		errno_t sm_make_path_abs(char* const path_abs, const size_t path_abs_len, const char* const path, const char* const bin_path_marker);

	return current directory path:
		errno_t sm_get_cur_dpath(char* const dpath, const size_t dpath_len);

	checking, if is some directory exists
		errno_t sm_is_dpath_found(const char* const dpath, BOOL* res);

	creates directory:
		errno_t sm_create_dpath(const char* const dpath);

	returns path of main module of executable file:
		errno_t sm_get_base_module_fpath(char* const exe_fpath, const size_t exe_fpath_len);

	returns directory path from full path:
		errno_t sm_get_dpath_from_fpath(char* const dpath, const size_t dpath_len, const char* const fpath);

	returns file name from full path:
		errno_t sm_get_fname_from_fpath(char* const fname, const size_t fname_len, const char* const fpath);

	returns temp directory path:
		errno_t sm_get_temp_dpath(char* const dpath, const size_t dpath_len);
*/

static void sm_get_cur_dpath_test(void** state);
static void sm_get_temp_dpath_test(void** state);
static void sm_create_dpath_is_dpath_found_test(void** state);
static void sm_get_base_module_fpath_test(void** state);
static void sm_get_dpath_from_fpath_test(void** state);
static void sm_get_fname_from_fpath_test(void** state);
static void sm_path_is_abs_test(void** state);
static void sm_make_path_abs_test(void** state);

/*		Template functions		*/

/*
	Suite of template functions, which are called by unit test functions:
	void (*CMUnitTestFunction)(void **state)
*/

static void sm_make_path_abs_test_template(const char* const path, const char* const bin_path_marker)
{
	errno_t err;
	errno_t err_safe;
	char path_abs[MAX_PATH];
	BOOL res;
	char* res_substr;
#if defined SM_OS_WINDOWS
	char curr_dir[] = "\\.\\";
	char par_dir[] = "\\..\\";
#elif defined SM_OS_LINUX
	char curr_dir[] = "/./";
	char par_dir[] = "/../";
#endif
	size_t curr_dir_len = safe_strnlen(curr_dir, MAX_PATH);
	size_t par_dir_len = safe_strnlen(par_dir, MAX_PATH);

	res = FALSE;
	err = sm_make_path_abs(path_abs, SM_ARRAY_SIZE(path_abs), path, bin_path_marker);
	assert_int_equal(err, SM_RES_OK);
	err = sm_path_is_abs(path_abs, &res);
	assert_int_equal(err, SM_RES_OK);
	assert_true(res);

	err_safe = safe_strstr(path_abs, SM_ARRAY_SIZE(path_abs), curr_dir, curr_dir_len, &res_substr);
	assert_true(err_safe);
	err_safe = safe_strstr(path_abs, SM_ARRAY_SIZE(path_abs), par_dir, par_dir_len, &res_substr);
	assert_true(err_safe);

	return;
}

static void sm_get_dpath_from_fpath_test_template(const char* const fpath, const char* const dpath_res, int res)
{
	errno_t err;

	char dpath[MAX_PATH];

	dpath[0] = '\0';
	err = sm_get_dpath_from_fpath(dpath, SM_ARRAY_SIZE(dpath), fpath);
	assert_int_equal(err, res);
	assert_string_equal(dpath, dpath_res);

	return;
}

static void sm_get_fname_from_fpath_test_template(const char* const fpath, const char* const fname_res, int res)
{
	errno_t err;

	char fname[MAX_PATH];

	fname[0] = '\0';
	err = sm_get_fname_from_fpath(fname, SM_ARRAY_SIZE(fname), fpath);
	assert_int_equal(err, res);
	assert_string_equal(fname, fname_res);

	return;
}

/*		Test functions		*/

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

	sm_set_def_calc_params();

	assert_int_equal(gsm_calc_params.m_f_precision, SM_PREC_DEF);
	assert_int_equal(gsm_calc_params.m_i_format, SM_I_FORMAT_DEF);
	assert_int_equal(gsm_calc_params.m_trig_unit, SM_TRIG_UNIT_DEF);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s", "test", __FUNCTION__, "is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");

	return;
}

/*
	Every function of follow unit tests provides testing the correspondent function from sm_log project
	(module sm_files_tools):

	checking, if argument path is an absulute:
		errno_t sm_path_is_abs(const char* const path, BOOL* const res);	

	converting, a relative path to the absolute path:
		errno_t sm_make_path_abs(char* const path_abs, const size_t path_abs_len, const char* const path, const char* const bin_path_marker);

	return current directory path:
		errno_t sm_get_cur_dpath(char* const dpath, const size_t dpath_len);

	checking, if is some directory exists
		errno_t sm_is_dpath_found(const char* const dpath, BOOL* res);

	creates directory:
		errno_t sm_create_dpath(const char* const dpath);

	returns path of main module of executable file:
		errno_t sm_get_base_module_fpath(char* const exe_fpath, const size_t exe_fpath_len);

	returns directory path from full path:
		errno_t sm_get_dpath_from_fpath(char* const dpath, const size_t dpath_len, const char* const fpath);

	returns file name from full path:
		errno_t sm_get_fname_from_fpath(char* const fname, const size_t fname_len, const char* const fpath);

	returns temp directory path:
		errno_t sm_get_temp_dpath(char* const dpath, const size_t dpath_len);
*/

static void sm_get_cur_dpath_test(void** state)
{
	errno_t err;
	char cur_dpath[MAX_PATH];

	err = sm_get_cur_dpath(cur_dpath, SM_ARRAY_SIZE(cur_dpath));
	assert_int_equal(err, SM_RES_OK);

	size_t path_len = safe_strnlen(cur_dpath, MAX_PATH);
	assert_true(path_len > 0);

	return;
}

static void sm_get_temp_dpath_test(void** state)
{
	errno_t err;
	char tmp_dpath[MAX_PATH];

	err = sm_get_temp_dpath(tmp_dpath, SM_ARRAY_SIZE(tmp_dpath));
	assert_int_equal(err, SM_RES_OK);

	size_t dpath_len = safe_strnlen(tmp_dpath, MAX_PATH);
	assert_true(dpath_len > 0);

	return;
}

static void sm_create_dpath_is_dpath_found_test(void** state)
{
	errno_t err;
	errno_t err_safe;
	BOOL res;

	char dpath[] = "./tmp_subdir_1/tmp_subdir_2/tmp_subdir_3/";
	char dpath_tmp_base[MAX_PATH];
	char dpath_tmp[MAX_PATH];

	err = sm_create_dpath(dpath);
	assert_int_equal(err, SM_RES_OK);

	res = FALSE;
	err = sm_is_dpath_found(dpath, &res);
	assert_true(err == SM_RES_OK && res == TRUE);

#if defined SM_OS_LINUX
	int res_rem;

	res_rem = rmdir(dpath);
	assert_int_equal(res_rem, 0);

	res_rem = rmdir("./tmp_subdir_1/tmp_subdir_2/");
	assert_int_equal(res_rem, 0);

	res_rem = rmdir("./tmp_subdir_1/");
	assert_int_equal(res_rem, 0);
#elif defined SM_OS_WINDOWS
	BOOL res_rem;

	res_rem = RemoveDirectory(dpath);
	assert_int_equal(res_rem, TRUE);

	res_rem = RemoveDirectory("./tmp_subdir_1/tmp_subdir_2/");
	assert_int_equal(res_rem, TRUE);

	res_rem = RemoveDirectory("./tmp_subdir_1/");
	assert_int_equal(res_rem, TRUE);
#endif

	err = sm_get_temp_dpath(dpath_tmp_base, SM_ARRAY_SIZE(dpath_tmp_base));
	assert_int_equal(err, SM_RES_OK);

	res = FALSE;
	err = sm_is_dpath_found(dpath_tmp_base, &res);
	assert_true(err == SM_RES_OK && res == TRUE);

	size_t dpath_len = safe_strnlen(dpath_tmp_base, MAX_PATH);
	assert_true(dpath_len > 0);

	if (dpath_len > 0 && dpath_tmp_base[dpath_len - 1] != '/' && dpath_tmp_base[dpath_len - 1] != '\\') {
		err_safe = safe_strncat(dpath_tmp_base, SM_ARRAY_SIZE(dpath_tmp_base), SM_FILES_DIR_SLASH, SM_ARRAY_SIZE(SM_FILES_DIR_SLASH));
		assert_int_equal(err_safe, 0);
	}

	err_safe = safe_strcpy(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), dpath_tmp_base);
	assert_int_equal(err_safe, 0);

	err_safe = safe_strcat(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), dpath);
	assert_int_equal(err_safe, 0);

	err = sm_create_dpath(dpath_tmp);
	assert_int_equal(err, SM_RES_OK);

	res = FALSE;
	err = sm_is_dpath_found(dpath_tmp, &res);
	assert_true(err == SM_RES_OK && res == TRUE);

#if defined SM_OS_LINUX
	err_safe = safe_strcpy(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), dpath_tmp_base);
	assert_int_equal(err_safe, 0);

	err_safe = safe_strcat(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), dpath);
	assert_int_equal(err_safe, 0);

	res_rem = rmdir(dpath_tmp);
	assert_int_equal(res_rem, 0);

	err_safe = safe_strcpy(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), dpath_tmp_base);
	assert_int_equal(err_safe, 0);

	err_safe = safe_strcat(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), "./tmp_subdir_1/tmp_subdir_2/");
	assert_int_equal(err_safe, 0);

	res_rem = rmdir(dpath_tmp);
	assert_int_equal(res_rem, 0);

	err_safe = safe_strcpy(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), dpath_tmp_base);
	assert_int_equal(err_safe, 0);

	err_safe = safe_strcat(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), "./tmp_subdir_1/");
	assert_int_equal(err_safe, 0);

	res_rem = rmdir(dpath_tmp);
	assert_int_equal(res_rem, 0);
#elif defined SM_OS_WINDOWS
	err_safe = safe_strcpy(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), dpath_tmp_base);
	assert_int_equal(err_safe, 0);

	err_safe = safe_strcat(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), dpath);
	assert_int_equal(err_safe, 0);

	res_rem = RemoveDirectory(dpath_tmp);
	assert_int_equal(res_rem, TRUE);

	err_safe = safe_strcpy(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), dpath_tmp_base);
	assert_int_equal(err_safe, 0);

	err_safe = safe_strcat(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), "./tmp_subdir_1/tmp_subdir_2/");
	assert_int_equal(err_safe, 0);

	res_rem = RemoveDirectory(dpath_tmp);
	assert_int_equal(res_rem, TRUE);

	err_safe = safe_strcpy(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), dpath_tmp_base);
	assert_int_equal(err_safe, 0);

	err_safe = safe_strcat(dpath_tmp, SM_ARRAY_SIZE(dpath_tmp), "./tmp_subdir_1/");
	assert_int_equal(err_safe, 0);

	res_rem = RemoveDirectory(dpath_tmp);
	assert_int_equal(res_rem, TRUE);
#endif

	return;
}

static void sm_get_base_module_fpath_test(void** state)
{
	errno_t err;
	char fpath_base_module[MAX_PATH];

	err =  sm_get_base_module_fpath(fpath_base_module, SM_ARRAY_SIZE(fpath_base_module));
	assert_int_equal(err, SM_RES_OK);

	size_t fpath_len = safe_strnlen(fpath_base_module, MAX_PATH);
	assert_true(fpath_len > 0);

	return;
}

static void sm_get_dpath_from_fpath_test(void** state)
{
	errno_t err;

	char dpath[MAX_PATH];

#if defined SM_OS_WINDOWS
	sm_get_dpath_from_fpath_test_template("d:\\some_subdir1\\some_subdir2\\some_subdir3\\file.dat",		"d:\\some_subdir1\\some_subdir2\\some_subdir3\\",	SM_RES_OK);
	sm_get_dpath_from_fpath_test_template("d:/some_subdir1/some_subdir2/some_subdir3/file.dat",			"d:/some_subdir1/some_subdir2/some_subdir3/",		SM_RES_OK);
	sm_get_dpath_from_fpath_test_template("d:\\some_subdir1\\some_subdir2\\some_subdir3\\",				"d:\\some_subdir1\\some_subdir2\\some_subdir3\\",	SM_RES_OK);
	sm_get_dpath_from_fpath_test_template("d:/some_subdir1/some_subdir2/some_subdir3/",					"d:/some_subdir1/some_subdir2/some_subdir3/",		SM_RES_OK);
	sm_get_dpath_from_fpath_test_template("d:",															"d:\\",		SM_RES_OK);
	sm_get_dpath_from_fpath_test_template("d:\\",														"d:\\",		SM_RES_OK);
	sm_get_dpath_from_fpath_test_template("d:/",														"d:/",		SM_RES_OK);
#elif defined SM_OS_LINUX
	sm_get_dpath_from_fpath_test_template("/some_subdir1/some_subdir2/some_subdir3/file.dat",			"/some_subdir1/some_subdir2/some_subdir3/",		SM_RES_OK);
	sm_get_dpath_from_fpath_test_template("/some_subdir1/some_subdir2/some_subdir3/",					"/some_subdir1/some_subdir2/some_subdir3/",		SM_RES_OK);
	sm_get_dpath_from_fpath_test_template("/",															"/",											SM_RES_OK);
#endif

	dpath[0] = '\0';
	err = sm_get_dpath_from_fpath(dpath, SM_ARRAY_SIZE(dpath), "");
	assert_int_equal(err, SM_RES_ERROR);

	return;
}

static void sm_get_fname_from_fpath_test(void** state)
{
	errno_t err;

	char fname[MAX_PATH];

#if defined SM_OS_WINDOWS
	sm_get_fname_from_fpath_test_template("d:\\some_subdir1\\some_subdir2\\some_subdir3\\file.dat",		"file.dat", SM_RES_OK);
	sm_get_fname_from_fpath_test_template("d:/some_subdir1/some_subdir2/some_subdir3/file.dat",			"file.dat", SM_RES_OK);
	sm_get_fname_from_fpath_test_template("d:\\file.dat",												"file.dat", SM_RES_OK);
	sm_get_fname_from_fpath_test_template("d:/file.dat",												"file.dat", SM_RES_OK);
	sm_get_fname_from_fpath_test_template("d:\\some_subdir1\\some_subdir2\\some_subdir3\\",				"",			SM_RES_ERROR);
	sm_get_fname_from_fpath_test_template("d:",															"",			SM_RES_ERROR);
	sm_get_fname_from_fpath_test_template("d:\\",														"",			SM_RES_ERROR);
#elif defined SM_OS_LINUX
	sm_get_fname_from_fpath_test_template("/some_subdir1/some_subdir2/some_subdir3/file.dat",			"file.dat", SM_RES_OK);
	sm_get_fname_from_fpath_test_template("file.dat",													"file.dat", SM_RES_OK);
	sm_get_fname_from_fpath_test_template("/file.dat",													"file.dat", SM_RES_OK);
	sm_get_fname_from_fpath_test_template("/some_subdir1/some_subdir2/some_subdir3/",					"",			SM_RES_ERROR);
	sm_get_fname_from_fpath_test_template("/",															"",			SM_RES_ERROR);
#endif

	fname[0] = '\0';
	err = sm_get_dpath_from_fpath(fname, SM_ARRAY_SIZE(fname), "");
	assert_int_equal(err, SM_RES_ERROR);

	return;
}

static void sm_path_is_abs_test(void** state)
{
	errno_t err;
	BOOL res;

#if defined SM_OS_WINDOWS
	res = FALSE;
	err = sm_path_is_abs("c:/some_subdir1/some_subdir_2/file.txt", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_true(res);

	res = FALSE;
	err = sm_path_is_abs("d:\\some_subdir1\\some_subdir_2\\file.txt", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_true(res);

	res = FALSE;
	err = sm_path_is_abs("c:/some_subdir1/some_subdir_2/", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_true(res);

	res = FALSE;
	err = sm_path_is_abs("d:\\some_subdir1\\some_subdir_2\\", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_true(res);

	res = FALSE;
	err = sm_path_is_abs("d:\\some_subdir1\\some_subdir_2\\", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_true(res);

	res = FALSE;
	err = sm_path_is_abs("d:", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_true(res);

	res = FALSE;
	err = sm_path_is_abs("d:\\", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_true(res);

	res = FALSE;
	err = sm_path_is_abs("some_subdir1/some_subdir_2/file.txt", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_false(res);

	res = FALSE;
	err = sm_path_is_abs("./some_subdir1/some_subdir_2/file.txt", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_false(res);

	res = FALSE;
	err = sm_path_is_abs("../some_subdir1/some_subdir_2/file.txt", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_false(res);

	res = FALSE;
	err = sm_path_is_abs(".\\some_subdir1\\some_subdir_2\\file.txt", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_false(res);

	res = FALSE;
	err = sm_path_is_abs("..//some_subdir1//some_subdir_2//file.txt", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_false(res);

	res = FALSE;
	err = sm_path_is_abs("file.txt", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_false(res);

	res = FALSE;
	err = sm_path_is_abs("f", &res);
	assert_int_equal(err, SM_RES_ERROR);

	res = FALSE;
	err = sm_path_is_abs("", &res);
	assert_int_equal(err, SM_RES_ERROR);

#elif defined SM_OS_LINUX
	res = FALSE;
	err = sm_path_is_abs("/home/user_home/some_subdir1/some_subdir_2/file.txt", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_true(res);

	res = FALSE;
	err = sm_path_is_abs("/home/user_home/some_subdir1/some_subdir_2/", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_true(res);

	res = FALSE;
	err = sm_path_is_abs("/", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_true(res);

	res = FALSE;
	err = sm_path_is_abs("some_subdir1/some_subdir_2/file.txt", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_false(res);

	res = FALSE;
	err = sm_path_is_abs("./some_subdir1/some_subdir_2/file.txt", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_false(res);

	res = FALSE;
	err = sm_path_is_abs("../some_subdir1/some_subdir_2/file.txt", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_false(res);

	res = FALSE;
	err = sm_path_is_abs("file.txt", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_false(res);

	res = FALSE;
	err = sm_path_is_abs("f", &res);
	assert_int_equal(err, SM_RES_OK);
	assert_false(res);

	res = FALSE;
	err = sm_path_is_abs("", &res);
	assert_int_equal(err, SM_RES_ERROR);
#endif
	return;
}

static void sm_make_path_abs_test(void** state)
{
#if defined SM_OS_WINDOWS
	sm_make_path_abs_test_template("./some_subdir1/some_subdir_2/", "");
	sm_make_path_abs_test_template("../some_subdir1/some_subdir_2/", "");
	sm_make_path_abs_test_template("./some_subdir1/some_subdir_2/file.txt", "");
	sm_make_path_abs_test_template("../some_subdir1/some_subdir_2/file.txt", "");
	sm_make_path_abs_test_template("./some_subdir1/some_subdir_2/", "");
	sm_make_path_abs_test_template("./some_subdir1/some_subdir_2/", "");
	sm_make_path_abs_test_template("..\\some_subdir1\\some_subdir_2\\", "");
	sm_make_path_abs_test_template(".\\some_subdir1\\some_subdir_2\\file.txt", "");
	sm_make_path_abs_test_template("..\\some_subdir1\\some_subdir_2\\file.txt", "");
	sm_make_path_abs_test_template("file.txt", "");
	sm_make_path_abs_test_template("\\file.txt", "");
	sm_make_path_abs_test_template("*{SM_CALC_BIN}*\\file.txt", "*{SM_CALC_BIN}*");
	sm_make_path_abs_test_template("*{SM_CALC_BIN}*\\..\\..\\log\\file.txt", "*{SM_CALC_BIN}*");
	sm_make_path_abs_test_template("*{SM_CALC_BIN}*../../log/file.txt", "*{SM_CALC_BIN}*");
	sm_make_path_abs_test_template("*{SM_CALC_BIN}*/../../log/file.txt", "*{SM_CALC_BIN}*");
	sm_make_path_abs_test_template("\\file.txt", "*{SM_CALC_BIN}*");
#elif defined SM_OS_LINUX
	sm_make_path_abs_test_template("./some_subdir1/some_subdir_2/", "");
	sm_make_path_abs_test_template("../some_subdir1/some_subdir_2/", "");
	sm_make_path_abs_test_template("./some_subdir1/some_subdir_2/file.txt", "");
	sm_make_path_abs_test_template("../some_subdir1/some_subdir_2/file.txt", "");
	sm_make_path_abs_test_template("file.txt", "");
	sm_make_path_abs_test_template("*{SM_CALC_BIN}*../../log/file.txt", "*{SM_CALC_BIN}*");
	sm_make_path_abs_test_template("*{SM_CALC_BIN}*/../../log/file.txt", "*{SM_CALC_BIN}*");
	sm_make_path_abs_test_template("/file.txt", "*{SM_CALC_BIN}*");
#endif
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
		cmocka_unit_test(sm_get_cur_dpath_test),
		cmocka_unit_test(sm_get_temp_dpath_test),
		cmocka_unit_test(sm_create_dpath_is_dpath_found_test),
		cmocka_unit_test(sm_get_base_module_fpath_test),
		cmocka_unit_test(sm_get_dpath_from_fpath_test),
		cmocka_unit_test(sm_get_fname_from_fpath_test),
		cmocka_unit_test(sm_path_is_abs_test),
		cmocka_unit_test(sm_make_path_abs_test)
	};

	sm_init_log_test();			//	initializing and starting the log output (file path, defined by SM_LOG_DPATH and SM_LOG_FNAME is used)

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s", __FUNCTION__, "---------------------------------------------------------------------- >>");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test_log is started");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");

	sm_init_random();			//	initializing the random seed (for 'rand[;]' command)
	sm_set_def_calc_params();	//	initializing the global sm_calc_params object
	sm_log_calc_params();		//	provides log out (in log file) the sm_calc_params gsm_calc_params, current state of sm_calculator configuration

	int res = cmocka_run_group_tests(tests, NULL, NULL);	//	execution of unit tests (array of void (*CMUnitTestFunction)(void **state) functions)
															//	the function cmocka_run_group_tests returns: total_failed + total_errors
															//	i.e. if result of execution of unit tests is OK, int res == 0

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "cmocka_run_group_tests: result = %d", res);
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test_log is finished");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s", __FUNCTION__, "---------------------------------------------------------------------- <<");

	sm_close_log_test();		//	stopping and closing the log output

	return res;
}

/*	initializing and starting the log output (SM_LOG_DPATH and SM_LOG_FNAME - init params)	*/
errno_t	sm_init_log_test()
{
	errno_t err = SM_RES_OK;
	err = sm_log_init_dpath_fname(SM_LOG_CONFIG, SM_LOG_DPATH, SM_LOG_FNAME);
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
