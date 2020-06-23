/*
 *    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
 *    Sergey Manoylo <info@smansoft.com>
 */

#include "pch.h"

#include "cmocka.h"

#include "safe_fopen.h"
#include "safe_str_lib.h"
#include "safe_mem_lib.h"

#include "sm_log.h"

#include "sm_calc_types.h"
#include "sm_calc.h"
#include "sm_calc_proc.h"

static const float gcsm_f_test_epsilon = 1e-10f;	//	inaccuracy (margin) constant, that is used during comparing of float values in test functions
													//	i.e. if (float a1 - float a2) < margin constant, float a1 == float a2

#define SM_LOG_DPATH		"."					//	default directory path, where log file will be created (current directory)
#define SM_LOG_FNAME		"sm_calc_test.log"	//	log file name

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

static int	 sm_compare_arrays(const unsigned char* const first_array, const size_t first_array_size,
	const unsigned char* const second_array, const size_t second_array_size);
static void sm_a_unit_test_template(const sm_trig_unit trig_unit, const char* const unit_name);
static void sm_i_format_test_template(const sm_i_format i_format, const char* const format);
static void sm_get_const_test_template(const sm_const_idx const_idx, long double const const_value);
static void sm_i_format_out_test_template(const long long arg, const char* const check_arrays_sz[]);
static void sm_conv_i_dec_2_sz_test_template(const long long arg, const char* const check_arrays_sz[]);
static void sm_conv_i_dec_2_bin_test_template(const long long arg, const unsigned char* const check_array, size_t check_array_size);
static void sm_conv_sz_2_f_dec_test_template(const char* const arrays_sz, long double check_arg);
static void sm_conv_i_test_template(long long arg);
static void sm_conv_f_test_template(long double arg);
static void sm_conv_bin_2_bin_sz_test_template(const long long arg, const unsigned char* const check_buf);
static void sm_f_precision_test_template(int precision, int expected);

/*		Test functions		*/

/*
	initializing of global instance of global instance of definition the current state of sm_calculator configuration;
	unit test code and all functions in sm_calc project/module use same instance of current state
	of sm_calculator configuration
*/
static void sm_calc_init_test(void** state);

/*
	Every function of follow unit tests suite provides:
		- necessary initializing of current state of sm_calculator configuration (optionally)
			or some partial configuration parameters;
		- initializing of source data/values/variables;
		- call of calculation processing functions (from sm_calc project/module, from sm_calc/sm_calc_proc.c);
		- checking result(s), returned by calculation processing functions;
		- comparing results with checking/correct data/values/variables;
*/

static void sm_get_const_test(void** state);
static void sm_calc_divide_test(void** state);
static void sm_a_unit_test(void** state);
static void sm_i_format_test(void** state);
static void sm_calc_trig_dir_test(void** state);
static void sm_calc_trig_rev_test(void** state);
static void sm_conv_trig_test(void** state);
static void sm_calc_factorial_test(void** state);
static void sm_calc_rand_test(void** state);
static void sm_conv_i_dec_2_sz_test(void** state);
static void sm_conv_i_test(void** state);
static void sm_conv_f_test(void** state);
static void sm_calc_l_ashift_test(void** state);
static void sm_calc_r_ashift_test(void** state);
static void sm_calc_l_lshift_test(void** state);
static void sm_calc_r_lshift_test(void** state);
static void sm_calc_l_cshift_test(void** state);
static void sm_calc_r_cshift_test(void** state);
static void sm_calc_and_test(void** state);
static void sm_calc_or_test(void** state);
static void sm_calc_not_test(void** state);
static void sm_calc_nand_test(void** state);
static void sm_calc_nor_test(void** state);
static void sm_calc_xor_test(void** state);
static void sm_conv_bin_2_bin_sz_test(void** state);
static void sm_f_precision_test(void** state);
static void sm_calc_mod_test(void** state);
static void sm_calc_abs_i_test(void** state);
static void sm_calc_abs_f_test(void** state);
static void sm_calc_sqrt_test(void** state);
static void sm_calc_ln_test(void** state);
static void sm_calc_lg_test(void** state);
static void sm_calc_log_test(void** state);
static void sm_calc_power_test(void** state);
static void sm_calc_power_test(void** state);
static void sm_calc_power_2_i_test(void** state);
static void sm_calc_power_10_i_test(void** state);
static void sm_calc_power_i_2_test(void** state);
static void sm_calc_power_2_d_test(void** state);
static void sm_calc_power_10_d_test(void** state);
static void sm_calc_power_exp_d_test(void** state);
static void sm_calc_power_d_2_test(void** state);
static void sm_echo_test(void** state);
static void sm_conv_quot_sz_2_sz_test(void** state);

/*		Template functions		*/

/*
	Suite of template functions, which are called by unit test functions:
	void (*CMUnitTestFunction)(void **state)
*/

static int sm_compare_arrays(const unsigned char* const first_array, const size_t first_array_size,
	const unsigned char* const second_array, const size_t second_array_size)
{
	if (first_array_size != second_array_size)
		return SM_RES_ERROR;

	for (int i = 0; i < first_array_size; i++)
		if (first_array[i] != second_array[i])
			return SM_RES_ERROR;

	return SM_RES_OK;
}

static void sm_conv_i_test_template(long long arg)
{
	errno_t err;
	long long res;
	char buf[SM_S_BUFF_SIZE + 2];

	err = sm_conv_i_dec_2_sz(SM_I_8, arg, buf, SM_ARRAY_SIZE(buf));
	assert_true(err == SM_RES_OK);
	err = sm_conv_sz_2_i_dec(SM_I_8, buf, SM_ARRAY_SIZE(buf), &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(arg, res);

	err = sm_conv_i_dec_2_sz(SM_I_16, arg, buf, SM_ARRAY_SIZE(buf));
	assert_true(err == SM_RES_OK);
	err = sm_conv_sz_2_i_dec(SM_I_16, buf, SM_ARRAY_SIZE(buf), &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(arg, res);

	err = sm_conv_i_dec_2_sz(SM_I_2, arg, buf, SM_ARRAY_SIZE(buf));
	assert_true(err == SM_RES_OK);
	err = sm_conv_sz_2_i_dec(SM_I_2, buf, SM_ARRAY_SIZE(buf), &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(arg, res);

	return;
}

static void sm_conv_f_test_template(long double arg)
{
	errno_t err;
	long double res;
	char buf[SM_L_BUFF_SIZE];

	err = sm_conv_f_dec_2_sz(arg, buf, SM_ARRAY_SIZE(buf));
	assert_true(err == SM_RES_OK);

	err = sm_conv_sz_2_f_dec(buf, SM_ARRAY_SIZE(buf), &res);
	assert_true(err == SM_RES_OK);

	assert_float_equal(arg, res, gcsm_f_test_epsilon);

	return;
}

static void sm_a_unit_test_template(const sm_trig_unit trig_unit, const char* const unit_name)
{
	errno_t err;
	char buf[SM_S_BUFF_SIZE];

	sm_set_def_calc_params();

	err = sm_set_a_unit(trig_unit);
	assert_true(err == SM_RES_OK);
	err = sm_get_a_unit(buf, SM_ARRAY_SIZE(buf));
	assert_true(err == SM_RES_OK);
	assert_string_equal(buf, gcsm_trig_units[trig_unit]);
	assert_string_equal(buf, unit_name);

	return;
}

static void sm_i_format_test_template(const sm_i_format i_format, const char* const format)
{
	errno_t err;
	char buf[SM_S_BUFF_SIZE];

	sm_set_def_calc_params();

	err = sm_set_i_format(i_format);
	assert_true(err == SM_RES_OK);
	err = sm_get_i_format(buf, SM_ARRAY_SIZE(buf));
	assert_true(err == SM_RES_OK);
	assert_string_equal(buf, gcsm_i_formats[i_format]);

	return;
}

static void sm_get_const_test_template(const sm_const_idx const_idx, const long double const_value)
{
	errno_t err;
	long double res;

	err = sm_get_const(const_idx, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, const_value, gcsm_f_test_epsilon);

	return;
}

static void sm_i_format_out_test_template(long long arg, const char* const check_arrays_sz[])
{
	errno_t err;
	sm_bin out_bin;
	char buf[SM_S_BUFF_SIZE + 1];

	int res;
	size_t len;

	for (int i = SM_I_FORMAT_MIN; i <= SM_I_FORMAT_MAX; i++) {
		if (i == SM_I_2) {
			char out_buf[SM_S_BUFF_SIZE + 2];

			err = sm_set_i_format(SM_I_2);
			assert_true(err == SM_RES_OK);

			err = sm_conv_i_dec_2_bin(arg, &out_bin);
			assert_true(err == SM_RES_OK);

			err = sm_conv_bin_2_bin_sz(&out_bin, buf, SM_ARRAY_SIZE(buf));
			assert_true(err == SM_RES_OK);

			size_t len = safe_strnlen(buf, SM_S_BUFF_SIZE);
			assert_true(len > 0);

			res = snprintf(out_buf, SM_ARRAY_SIZE(out_buf), gcsm_i_printf_formats[gsm_calc_params.m_i_format], buf);
			assert_true(res > 0);

			assert_string_equal(out_buf, check_arrays_sz[i]);
		}
		else {
			err = sm_set_i_format(i);
			assert_true(err == SM_RES_OK);
			res = snprintf(buf, SM_ARRAY_SIZE(buf), gcsm_i_printf_formats[gsm_calc_params.m_i_format], arg);
			assert_true(res > 0);
			len = safe_strnlen(buf, SM_S_BUFF_SIZE);
			assert_true(len > 0);
			assert_string_equal(buf, check_arrays_sz[i]);
		}
	}

	return;
}

static void sm_conv_i_dec_2_sz_test_template(const long long arg, const char* const check_arrays_sz[])
{
	errno_t err;
	char buf[SM_S_BUFF_SIZE + 2];
	long long res;

	for (int i = SM_I_FORMAT_MIN; i <= SM_I_FORMAT_MAX; i++) {
		err = sm_set_i_format(i);
		assert_true(err == SM_RES_OK);

		err = sm_conv_i_dec_2_sz(i, arg, buf, SM_ARRAY_SIZE(buf));
		assert_true(err == SM_RES_OK);

		size_t len = safe_strnlen(buf, SM_S_BUFF_SIZE);
		assert_true(len > 0);

		assert_string_equal(buf, check_arrays_sz[i]);

		err = sm_conv_sz_2_i_dec(i, buf, SM_ARRAY_SIZE(buf), &res);
		assert_true(err == SM_RES_OK);

		assert_int_equal(arg, res);
	}

	return;
}

static void sm_conv_i_dec_2_bin_test_template(const long long arg, const unsigned char* const check_array, size_t check_array_size)
{
	errno_t err;
	sm_bin out_bin;
	long long res;

	err = sm_conv_i_dec_2_bin(arg, &out_bin);
	assert_true(err == SM_RES_OK);

	assert_true(out_bin.m_len == check_array_size);

	err = sm_compare_arrays(out_bin.m_data, SM_ARRAY_SIZE(out_bin.m_data), check_array, check_array_size);
	assert_true(err == SM_RES_OK);

	err = sm_conv_bin_2_i_dec(&out_bin, &res);
	assert_true(err == SM_RES_OK);

	assert_int_equal(arg, res);

	return;
}

static void sm_conv_sz_2_f_dec_test_template(const char* const arrays_sz, long double check_arg)
{
	errno_t err;
	long double res;
	char buf[SM_L_BUFF_SIZE];

	err = safe_strcpy(buf, SM_ARRAY_SIZE(buf), arrays_sz);
	assert_true(err == SM_RES_OK);

	err = sm_conv_sz_2_f_dec(buf, SM_ARRAY_SIZE(buf), &res);
	assert_true(err == SM_RES_OK);

	assert_float_equal(res, check_arg, gcsm_f_test_epsilon);

	err = snprintf(buf, SM_ARRAY_SIZE(buf), gsm_calc_params.m_f_prec_buf, res);
	assert_true(err > 0);

	assert_string_equal(arrays_sz, buf);

	return;
}

static void sm_conv_bin_2_bin_sz_test_template(const long long arg, const unsigned char* const check_buf)
{
	errno_t err;
	sm_bin  out_bin_1;
	sm_bin  out_bin_2;
	char buf[SM_S_BUFF_SIZE + 1];
	long long res;

	err = sm_conv_i_dec_2_bin(arg, &out_bin_1);
	assert_true(err == SM_RES_OK);

	err = sm_conv_bin_2_bin_sz(&out_bin_1, buf, SM_ARRAY_SIZE(buf));
	assert_true(err == SM_RES_OK);

	assert_string_equal(buf, check_buf);

	err = sm_conv_bin_sz_2_bin(buf, SM_ARRAY_SIZE(buf), &out_bin_2);
	assert_true(err == SM_RES_OK);

	err = sm_compare_arrays(out_bin_1.m_data, out_bin_1.m_len, out_bin_2.m_data, out_bin_2.m_len);
	assert_true(err == SM_RES_OK);

	err = sm_conv_bin_2_i_dec(&out_bin_1, &res);
	assert_true(err == SM_RES_OK);

	assert_int_equal(arg, res);

	return;
}

static void sm_f_precision_test_template(int precision, int expected)
{
	errno_t err;
	long long res_precision;

	err = sm_set_f_precision((long long)precision, &res_precision);

	if (expected == 1) {
		assert_true(err == SM_RES_OK);
		assert_int_equal(precision, res_precision);
		assert_int_equal(precision, gsm_calc_params.m_f_precision);
	}
	else
		assert_true(err == SM_RES_ERROR);
	return;
}

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
	Every function of follow unit tests suite provides:
		- necessary initializing of current state of sm_calculator configuration (optionally)
			or some partial configuration parameters;
		- initializing of source data/values/variables;
		- call of calculation processing functions (from sm_calc project/module, from sm_calc/sm_calc_proc.c);
		- checking result(s), returned by calculation processing functions;
		- comparing results with checking/correct data/values/variables;
*/

static void sm_get_const_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	(void)state;

	sm_get_const_test_template(SM_CONST_PI, 3.14159265358979323846L);
	sm_get_const_test_template(SM_CONST_EXP, 2.71828182845904523536L);
	sm_get_const_test_template(SM_CONST_GAMMA, 0.57721566490153286060L);
	sm_get_const_test_template(SM_CONST_PHI, 1.61803398874989484820L);
	sm_get_const_test_template(SM_CONST_GRAD_RAD, 0.015707963267948966192313216916L);
	sm_get_const_test_template(SM_CONST_RAD_GRAD, 63.661977236758134307553505348999L);
	sm_get_const_test_template(SM_CONST_RAD_DEG, 57.295779513082320876798154814105L);
	sm_get_const_test_template(SM_CONST_DEG_RAD, 0.017453292519943295769236907685L);
	sm_get_const_test_template(SM_CONST_GRAD_DEG, 0.9L);
	sm_get_const_test_template(SM_CONST_DEG_GRAD, 1.111111111111111111111111111111L);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");

	return;
}

static void sm_calc_divide_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;
	long double res;

	(void)state;

	sm_set_def_calc_params();

	err = sm_calc_div(10.0L, 2.0L, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 5.0, gcsm_f_test_epsilon);

	err = sm_calc_div(47.0L, 33.0L, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.424242424242, gcsm_f_test_epsilon);

	err = sm_calc_div(-199.0L, 2.0L, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -99.5, gcsm_f_test_epsilon);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");

	return;
}

static void sm_a_unit_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	(void)state;

	sm_a_unit_test_template(SM_TRIG_GRADIAN, "grad");
	sm_a_unit_test_template(SM_TRIG_DEGREES, "deg");
	sm_a_unit_test_template(SM_TRIG_RADIANS, "rad");

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");

	return;
}

static void sm_i_format_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	(void)state;

	sm_i_format_test_template(SM_I_10, "dec");
	sm_i_format_test_template(SM_I_8, "oct");
	sm_i_format_test_template(SM_I_16, "hex");
	sm_i_format_test_template(SM_I_2, "bin");

	sm_set_def_calc_params();

	const char* const ress_1_sz[] = { "268083481", "01776520431", "0xffaa119", "b1111111110101010000100011001" };
	sm_i_format_out_test_template(268083481ll, ress_1_sz);

	const char* const ress_2_sz[] = { "777", "01411", "0x309", "b001100001001" };
	sm_i_format_out_test_template(777ll, ress_2_sz);

	const char* const ress_3_sz[] = { "-777", "01777777777777777776367", "0xfffffffffffffcf7", "b1111111111111111111111111111111111111111111111111111110011110111" };
	sm_i_format_out_test_template(-777ll, ress_3_sz);

	const char* const ress_4_sz[] = { "-7856", "01777777777777777760520", "0xffffffffffffe150", "b1111111111111111111111111111111111111111111111111110000101010000" };
	sm_i_format_out_test_template(-7856ll, ress_4_sz);

	const char* const ress_5_sz[] = { "0", "00", "0x0", "b0000" };
	sm_i_format_out_test_template(0ll, ress_5_sz);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");

	return;
}

static void sm_conv_i_dec_2_sz_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	(void)state;

	sm_set_def_calc_params();

	const char* const ress_1_sz[] = { "268083481", "01776520431", "0xffaa119", "b1111111110101010000100011001" };
	sm_conv_i_dec_2_sz_test_template(268083481ll, ress_1_sz);

	const char* const ress_2_sz[] = { "777", "01411", "0x309", "b001100001001" };
	sm_conv_i_dec_2_sz_test_template(777ll, ress_2_sz);

	const char* const ress_3_sz[] = { "-777", "01777777777777777776367", "0xfffffffffffffcf7", "b1111111111111111111111111111111111111111111111111111110011110111" };
	sm_conv_i_dec_2_sz_test_template(-777ll, ress_3_sz);

	const char* const ress_4_sz[] = { "-7856", "01777777777777777760520", "0xffffffffffffe150", "b1111111111111111111111111111111111111111111111111110000101010000" };
	sm_conv_i_dec_2_sz_test_template(-7856ll, ress_4_sz);

	const char* const ress_5_sz[] = { "0", "00", "0x0", "b0000" };
	sm_conv_i_dec_2_sz_test_template(0ll, ress_5_sz);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");

	return;
}

static void sm_calc_trig_dir_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;
	long double arg;
	long double res;

	(void)state;

	err = sm_get_const(SM_CONST_PI, &arg);
	assert_true(err == SM_RES_OK);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "setup default a_unit == 'rad'");

	sm_set_def_calc_params();
	sm_a_unit_test_template(SM_TRIG_RADIANS, "rad");

	err = sm_calc_sin(0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_sin(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_sin(arg / 2.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0, gcsm_f_test_epsilon);

	err = sm_calc_sin((3 * arg) / 11.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.7557495744, gcsm_f_test_epsilon);

	err = sm_calc_cos(0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0, gcsm_f_test_epsilon);

	err = sm_calc_cos(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -1.0, gcsm_f_test_epsilon);

	err = sm_calc_cos(arg / 2.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_cos((3 * arg) / 11.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.6548607339, gcsm_f_test_epsilon);

	err = sm_calc_tan(0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_tan(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_tan(arg / 4.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0, gcsm_f_test_epsilon);

	err = sm_calc_tan((3 * arg) / 11.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.1540615205, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "setup default a_unit == 'deg'");

	sm_set_def_calc_params();
	sm_a_unit_test_template(SM_TRIG_DEGREES, "deg");

	err = sm_calc_sin(0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_sin(180, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_sin(180 / 2.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0, gcsm_f_test_epsilon);

	err = sm_calc_sin((3 * 180) / 11.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.7557495744, gcsm_f_test_epsilon);

	err = sm_calc_cos(0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0, gcsm_f_test_epsilon);

	err = sm_calc_cos(180, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -1.0, gcsm_f_test_epsilon);

	err = sm_calc_cos(180 / 2.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_cos((3 * 180) / 11.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.6548607339, gcsm_f_test_epsilon);

	err = sm_calc_tan(0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_tan(180, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_tan(180 / 4.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0, gcsm_f_test_epsilon);

	err = sm_calc_tan((3 * 180) / 11.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.1540615205, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "setup default a_unit == 'grad'");

	sm_set_def_calc_params();
	sm_a_unit_test_template(SM_TRIG_GRADIAN, "grad");

	err = sm_calc_sin(0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_sin(200, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_sin(200 / 2.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0, gcsm_f_test_epsilon);

	err = sm_calc_sin((3 * 200) / 11.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.7557495744, gcsm_f_test_epsilon);

	err = sm_calc_cos(0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0, gcsm_f_test_epsilon);

	err = sm_calc_cos(200, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -1.0, gcsm_f_test_epsilon);

	err = sm_calc_cos(200 / 2.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_cos((3 * 200) / 11.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.6548607339, gcsm_f_test_epsilon);

	err = sm_calc_tan(0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_tan(200, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	err = sm_calc_tan(200 / 4.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0, gcsm_f_test_epsilon);

	err = sm_calc_tan((3 * 200) / 11.0, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.1540615205, gcsm_f_test_epsilon);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");

	return;
}

static void sm_calc_trig_rev_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;
	long double pi;
	long double arg;
	long double res;

	(void)state;

	err = sm_get_const(SM_CONST_PI, &pi);
	assert_true(err == SM_RES_OK);

	assert_float_equal(pi, 3.1415926536, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "setup default a_unit == 'rad'");

	sm_set_def_calc_params();
	sm_a_unit_test_template(SM_TRIG_RADIANS, "rad");

	arg = 0.0;
	err = sm_calc_asin(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 1.0;
	err = sm_calc_asin(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (pi / 2.0), gcsm_f_test_epsilon);

	arg = 0.7071067812;
	err = sm_calc_asin(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (pi / 4.0), gcsm_f_test_epsilon);

	arg = 0.0;
	err = sm_calc_acos(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (pi / 2.0), gcsm_f_test_epsilon);

	arg = 1.0;
	err = sm_calc_acos(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 0.7071067812;
	err = sm_calc_acos(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (pi / 4.0), gcsm_f_test_epsilon);

	arg = 0.0;
	err = sm_calc_atan(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 1.0;
	err = sm_calc_atan(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (pi / 4.0), gcsm_f_test_epsilon);

	arg = 1.1540615205;
	err = sm_calc_atan(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, ((3 * pi) / 11.0), gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "setup default a_unit == 'deg'");

	sm_set_def_calc_params();
	sm_a_unit_test_template(SM_TRIG_DEGREES, "deg");

	arg = 0.0;
	err = sm_calc_asin(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 1.0;
	err = sm_calc_asin(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (180.0 / 2.0), gcsm_f_test_epsilon);

	arg = 0.7071067812;
	err = sm_calc_asin(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (180.0 / 4.0), gcsm_f_test_epsilon);

	arg = 0.0;
	err = sm_calc_acos(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (180.0 / 2.0), gcsm_f_test_epsilon);

	arg = 1.0;
	err = sm_calc_acos(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 0.7071067812;
	err = sm_calc_acos(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (180.0 / 4.0), gcsm_f_test_epsilon);

	arg = 0.0;
	err = sm_calc_atan(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 1.0;
	err = sm_calc_atan(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (180.0 / 4.0), gcsm_f_test_epsilon);

	arg = 1.1540615205;
	err = sm_calc_atan(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, ((3 * 180.0) / 11.0), gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "setup default a_unit == 'grad'");

	sm_set_def_calc_params();
	sm_a_unit_test_template(SM_TRIG_GRADIAN, "grad");

	arg = 0.0;
	err = sm_calc_asin(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 1.0;
	err = sm_calc_asin(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (200.0 / 2.0), gcsm_f_test_epsilon);

	arg = 0.7071067812;
	err = sm_calc_asin(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (200.0 / 4.0), gcsm_f_test_epsilon);

	arg = 0.0;
	err = sm_calc_acos(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (200.0 / 2.0), gcsm_f_test_epsilon);

	arg = 1.0;
	err = sm_calc_acos(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 0.7071067812;
	err = sm_calc_acos(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (200.0 / 4.0), gcsm_f_test_epsilon);

	arg = 0.0;
	err = sm_calc_atan(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 1.0;
	err = sm_calc_atan(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, (200.0 / 4.0), gcsm_f_test_epsilon);

	arg = 1.1540615205;
	err = sm_calc_atan(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, ((3 * 200.0) / 11.0), gcsm_f_test_epsilon);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");

	return;
}

static void sm_conv_trig_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;
	long double pi;
	long double arg;
	long double res;

	(void)state;

	sm_set_def_calc_params();

	assert_int_equal(gsm_calc_params.m_trig_unit, SM_TRIG_UNIT_DEF);

	err = sm_get_const(SM_CONST_PI, &pi);
	assert_true(err == SM_RES_OK);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "setup default a_unit == 'deg'");

	err = sm_set_a_unit(SM_TRIG_DEGREES);
	assert_true(err == SM_RES_OK);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "testing sm_conv_curr_2_rad");

	arg = 0;
	err = sm_conv_curr_2_rad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 45;
	err = sm_conv_curr_2_rad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.78539816339744830962L, gcsm_f_test_epsilon);

	arg = 180.0 / 5.0;
	err = sm_conv_curr_2_rad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.62831853071795864769L, gcsm_f_test_epsilon);

	arg = 180.0;
	err = sm_conv_curr_2_rad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 3.14159265358979323846L, gcsm_f_test_epsilon);

	arg = (-1) * 180.0 / 5.0;
	err = sm_conv_curr_2_rad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -0.6283185307179586L, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "setup default a_unit == 'rad'");

	err = sm_set_a_unit(SM_TRIG_RADIANS);
	assert_true(err == SM_RES_OK);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "testing sm_conv_deg_2_grad , sm_conv_grad_2_deg");

	arg = 0.0;
	err = sm_conv_deg_2_grad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 0.0;
	err = sm_conv_grad_2_deg(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 25.7142857143;
	err = sm_conv_deg_2_grad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 28.571428571, gcsm_f_test_epsilon);

	arg = 28.571428571;
	err = sm_conv_grad_2_deg(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 25.7142857143, gcsm_f_test_epsilon);

	arg = -25.7142857143;
	err = sm_conv_deg_2_grad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -28.571428571, gcsm_f_test_epsilon);

	arg = -28.571428571;
	err = sm_conv_grad_2_deg(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -25.7142857143, gcsm_f_test_epsilon);

	arg = 180.0;
	err = sm_conv_deg_2_grad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 200.0L, gcsm_f_test_epsilon);

	arg = 200.0;
	err = sm_conv_grad_2_deg(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 180.0L, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "testing sm_conv_deg_2_rad, sm_conv_rad_2_deg");

	arg = 0.0;
	err = sm_conv_deg_2_rad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 0.0;
	err = sm_conv_rad_2_deg(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 180.0;
	err = sm_conv_deg_2_rad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 3.14159265358979323846, gcsm_f_test_epsilon);

	arg = 3.14159265358979323846L;
	err = sm_conv_rad_2_deg(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 180.0, gcsm_f_test_epsilon);

	arg = -180.0 / 5.0;
	err = sm_conv_deg_2_rad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -0.62831853072, gcsm_f_test_epsilon);

	arg = 0.62831853072;
	err = sm_conv_rad_2_deg(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 36.0, gcsm_f_test_epsilon);

	arg = -180.0 / 7.0;
	err = sm_conv_deg_2_rad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -0.44879895051, gcsm_f_test_epsilon);

	arg = -0.44879895051;
	err = sm_conv_rad_2_deg(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -25.71428571428571, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "testing sm_conv_grad_2_rad, sm_conv_rad_2_grad");

	arg = 0.0;
	err = sm_conv_grad_2_rad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 0.0;
	err = sm_conv_rad_2_grad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0, gcsm_f_test_epsilon);

	arg = 200.0;
	err = sm_conv_grad_2_rad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 3.14159265358979323846, gcsm_f_test_epsilon);

	arg = 3.14159265358979323846;
	err = sm_conv_rad_2_grad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 200.0, gcsm_f_test_epsilon);

	arg = -200.0;
	err = sm_conv_grad_2_rad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -3.14159265358979323846, gcsm_f_test_epsilon);

	arg = -3.14159265358979323846;
	err = sm_conv_rad_2_grad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -200.0, gcsm_f_test_epsilon);

	arg = -200.0 / 7.0; // 28.57142857142857
	err = sm_conv_grad_2_rad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -0.44879895051, gcsm_f_test_epsilon);

	arg = -0.44879895051;
	err = sm_conv_rad_2_grad(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -28.57142857142857, gcsm_f_test_epsilon);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");

	return;
}

static void sm_calc_factorial_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;
	long long arg;
	long long res;

	(void)state;

	arg = 0;
	err = sm_calc_factorial(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 1);

	arg = 1;
	err = sm_calc_factorial(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 1);

	arg = 5;
	err = sm_calc_factorial(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 120);

	arg = 10;
	err = sm_calc_factorial(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 3628800);

	arg = -1;
	err = sm_calc_factorial(arg, &res);
	assert_true(err == SM_RES_ERROR);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}


static void sm_calc_rand_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;
	long double res1;
	long double res2;

	(void)state;

	sm_set_def_calc_params();

	err = sm_get_rand(&res1);
	assert_true(err == SM_RES_OK);
	assert_true(0.0L <= res1 && res1 <= 1.0L);

	for (int i = 0; i < 10; i++) {
		err = sm_get_rand(&res2);
		assert_true(err == SM_RES_OK);
		assert_true(0.0L <= res2 && res2 <= 1.0L);

		assert_true(res1 != res2);
	}

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_conv_i_dec_2_bin_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	(void)state;

	long long arg = 268083481ll;
	unsigned char true_array_1[] = {
			1,0,0,1,1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};
	sm_conv_i_dec_2_bin_test_template(arg, true_array_1, SM_ARRAY_SIZE(true_array_1));

	arg = 777ll;
	unsigned char true_array_2[] = {
			1,0,0,1,0,0,0,0,1,1,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};
	sm_conv_i_dec_2_bin_test_template(arg, true_array_2, SM_ARRAY_SIZE(true_array_2));

	arg = -777ll;
	unsigned char true_array_3[] = {
			1,1,1,0,1,1,1,1,0,0,
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	};
	sm_conv_i_dec_2_bin_test_template(arg, true_array_3, SM_ARRAY_SIZE(true_array_3));

	arg = -7856ll;
	unsigned char true_array_4[] = {
			0,0,0,0,1,0,1,0,1,0,
			0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	};
	sm_conv_i_dec_2_bin_test_template(arg, true_array_4, SM_ARRAY_SIZE(true_array_4));

	arg = 0ll;
	unsigned char true_array_5[64] = { 0 };
	sm_conv_i_dec_2_bin_test_template(arg, true_array_5, SM_ARRAY_SIZE(true_array_5));

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_conv_sz_2_f_dec_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	(void)state;

	errno_t err;
	long long precision;
	long long res_precision;

	sm_set_def_calc_params();

	precision = 2;
	err = sm_set_f_precision(precision, &res_precision);
	assert_true(err == SM_RES_OK);
	assert_int_equal(precision, res_precision);

	sm_conv_sz_2_f_dec_test_template("0.00", 0.0L);
	sm_conv_sz_2_f_dec_test_template("-1.00", -1.0L);
	sm_conv_sz_2_f_dec_test_template("3.14", 3.14L);
	sm_conv_sz_2_f_dec_test_template("-2.72", -2.72L);
	sm_conv_sz_2_f_dec_test_template("-7777.99", -7777.99L);

	precision = 5;
	err = sm_set_f_precision(precision, &res_precision);
	assert_true(err == SM_RES_OK);
	assert_int_equal(precision, res_precision);

	sm_conv_sz_2_f_dec_test_template("0.00000", 0.0L);
	sm_conv_sz_2_f_dec_test_template("-1.00000", -1.0L);
	sm_conv_sz_2_f_dec_test_template("3.14159", 3.14159L);
	sm_conv_sz_2_f_dec_test_template("-2.71828", -2.71828L);
	sm_conv_sz_2_f_dec_test_template("-7777.99999", -7777.99999L);

	precision = 10;
	err = sm_set_f_precision(precision, &res_precision);
	assert_true(err == SM_RES_OK);
	assert_int_equal(precision, res_precision);

	sm_conv_sz_2_f_dec_test_template("0.0000000000", 0.0L);
	sm_conv_sz_2_f_dec_test_template("-1.0000000000", -1.0L);
	sm_conv_sz_2_f_dec_test_template("3.1415926536", 3.1415926536L);
	sm_conv_sz_2_f_dec_test_template("-2.7182818285", -2.7182818285L);
	sm_conv_sz_2_f_dec_test_template("-7777.9999999999", -7777.9999999999L);

	err = sm_set_f_exp_precision();
	assert_true(err == SM_RES_OK);
	assert_int_equal(gsm_calc_params.m_f_precision, -1);

	sm_conv_sz_2_f_dec_test_template("0.000000e+00", 0.0L);
	sm_conv_sz_2_f_dec_test_template("-1.000000e+00", -1.0L);
	sm_conv_sz_2_f_dec_test_template("3.141593e+00", 3.1415926536L);
	sm_conv_sz_2_f_dec_test_template("-2.718282e+00", -2.7182818285L);
	sm_conv_sz_2_f_dec_test_template("1.237110e-03", 123.711e-5);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_conv_i_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	(void)state;

	sm_set_def_calc_params();

	sm_conv_i_test_template(0ll);
	sm_conv_i_test_template(1ll);
	sm_conv_i_test_template(-1ll);
	sm_conv_i_test_template(798156ll);
	sm_conv_i_test_template(-798156ll);
	sm_conv_i_test_template(0x7fffffffffffffffll);
	sm_conv_i_test_template(0xffffffffffffffffll);

	for (int i = 0; i < 10; i++)
		sm_conv_i_test_template((long long)rand());

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_conv_f_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;
	long long res;

	(void)state;

	sm_set_def_calc_params();

	err = sm_set_f_precision(10ll, &res);
	assert_true(err == SM_RES_OK);

	err = sm_init_f_prec_buf();
	assert_true(err == SM_RES_OK);

	sm_conv_f_test_template(0.0L);
	sm_conv_f_test_template(-1.0L);
	sm_conv_f_test_template(3.1415926536L);
	sm_conv_f_test_template(-2.7182818285L);
	sm_conv_f_test_template(-7777.9999999999L);

	for (int i = 0; i < 10; i++)
		sm_conv_f_test_template((long double)rand() / (long double)RAND_MAX);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_l_ashift_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg;
	long long res;

	(void)state;

	arg = 0ll;
	err = sm_calc_l_ashift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	err = sm_calc_l_ashift(arg, 100, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg = 19ll;
	err = sm_calc_l_ashift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * 2);

	err = sm_calc_l_ashift(arg, 2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * 4);

	err = sm_calc_l_ashift(arg, 10, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 10));

	arg = 755ll;
	err = sm_calc_l_ashift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 16));

	arg = -19ll;
	err = sm_calc_l_ashift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 16));

	arg = 0x7ff12a8976341122;
	err = sm_calc_l_ashift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 16));

	arg = 0xfff12a8976341122;
	err = sm_calc_l_ashift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 16));

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_r_ashift_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg;
	long long res;

	(void)state;

	arg = 0ll;

	err = sm_calc_r_ashift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	err = sm_calc_r_ashift(arg, 100, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg = 19ll;

	err = sm_calc_r_ashift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 9);

	err = sm_calc_r_ashift(arg, 2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 4);

	err = sm_calc_r_ashift(arg, 10, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg = 0x7ff12a8976341122;

	err = sm_calc_r_ashift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x3ff89544bb1a0891);

	err = sm_calc_r_ashift(arg, 4, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ff12A897634112);

	err = sm_calc_r_ashift(arg, 32, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ff12a89);

	err = sm_calc_r_ashift(arg, 60, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 7);

	err = sm_calc_r_ashift(arg, 63, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg = 0xfff12a8976341122;

	err = sm_calc_r_ashift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfff89544bb1a0891);

	err = sm_calc_r_ashift(arg, 2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfffc4aa25d8d0448);

	err = sm_calc_r_ashift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfffffff12a897634);

	err = sm_calc_r_ashift(arg, 32, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfffffffffff12a89);

	err = sm_calc_r_ashift(arg, 34, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfffffffffffc4aa2);

	err = sm_calc_r_ashift(arg, 62, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffffffff);

	err = sm_calc_r_ashift(arg, 63, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffffffff);

	arg = 0x7ff1763411222a89;

	err = sm_calc_r_ashift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x3ff8bb1a08911544);

	err = sm_calc_r_ashift(arg, 4, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ff1763411222a8);

	err = sm_calc_r_ashift(arg, 32, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ff17634);

	err = sm_calc_r_ashift(arg, 60, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 7);

	err = sm_calc_r_ashift(arg, 63, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg = 0xfff1763411222a89;

	err = sm_calc_r_ashift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfff8bb1a08911544);

	err = sm_calc_r_ashift(arg, 2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfffc5d8d04488aa2);

	err = sm_calc_r_ashift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfffffff176341122);

	err = sm_calc_r_ashift(arg, 32, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfffffffffff17634);

	err = sm_calc_r_ashift(arg, 34, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfffffffffffc5d8d);

	err = sm_calc_r_ashift(arg, 62, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffffffff);

	err = sm_calc_r_ashift(arg, 63, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffffffff);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_l_lshift_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg;
	long long res;

	(void)state;

	arg = 0ll;
	err = sm_calc_l_lshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	err = sm_calc_l_lshift(arg, 100, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg = 19ll;
	err = sm_calc_l_lshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * 2);

	err = sm_calc_l_lshift(arg, 2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * 4);

	err = sm_calc_l_lshift(arg, 10, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 10));

	arg = 755ll;
	err = sm_calc_l_lshift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 16));

	arg = -19ll;
	err = sm_calc_l_lshift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 16));

	arg = 0x7ff12a8976341122;
	err = sm_calc_l_lshift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 16));

	arg = 0xfff12a8976341122;
	err = sm_calc_l_lshift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 16));

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_r_lshift_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg;
	long long res;

	(void)state;

	arg = 0ll;

	err = sm_calc_r_lshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	err = sm_calc_r_lshift(arg, 100, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg = 19ll;

	err = sm_calc_r_lshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 9);

	err = sm_calc_r_lshift(arg, 2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 4);

	err = sm_calc_r_lshift(arg, 10, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg = 0x7ff12a8976341122;

	err = sm_calc_r_lshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x3ff89544bb1a0891);

	err = sm_calc_r_lshift(arg, 4, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ff12A897634112);

	err = sm_calc_r_lshift(arg, 32, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ff12a89);

	err = sm_calc_r_lshift(arg, 60, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 7);

	err = sm_calc_r_lshift(arg, 63, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg = 0xfff12a8976341122;

	err = sm_calc_r_lshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ff89544bb1a0891);

	err = sm_calc_r_lshift(arg, 2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x3ffc4aa25d8d0448);

	err = sm_calc_r_lshift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfff12a897634);

	err = sm_calc_r_lshift(arg, 32, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfff12a89);

	err = sm_calc_r_lshift(arg, 34, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x3ffc4aa2);

	err = sm_calc_r_lshift(arg, 62, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x3);

	err = sm_calc_r_lshift(arg, 63, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x1);

	arg = 0x7ff1763411222a89;

	err = sm_calc_r_lshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x3ff8bb1a08911544);

	err = sm_calc_r_lshift(arg, 4, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ff1763411222a8);

	err = sm_calc_r_lshift(arg, 32, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ff17634);

	err = sm_calc_r_lshift(arg, 60, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 7);

	err = sm_calc_r_lshift(arg, 63, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg = 0xfff1763411222a89;

	err = sm_calc_r_lshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ff8bb1a08911544);

	err = sm_calc_r_lshift(arg, 2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x3ffc5d8d04488aa2);

	err = sm_calc_r_lshift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfff176341122);

	err = sm_calc_r_lshift(arg, 32, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfff17634);

	err = sm_calc_r_lshift(arg, 34, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x3ffc5d8d);

	err = sm_calc_r_lshift(arg, 62, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x3);

	err = sm_calc_r_lshift(arg, 63, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x1);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_l_cshift_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg;
	long long res;

	(void)state;

	arg = 0ll;
	err = sm_calc_l_cshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	err = sm_calc_l_cshift(arg, 100, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg = 19ll;
	err = sm_calc_l_cshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * 2);

	err = sm_calc_l_cshift(arg, 2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * 4);

	err = sm_calc_l_cshift(arg, 10, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 10));

	arg = 0xffffffffffffffff;
	err = sm_calc_l_cshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * 2 | 0x1);

	err = sm_calc_l_cshift(arg, 2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * 4 | 0xffff);

	err = sm_calc_l_cshift(arg, 10, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * 1024 | 0x3ff);

	arg = 755ll;
	err = sm_calc_l_cshift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 16));

	arg = -19ll;
	err = sm_calc_l_cshift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 16) | 0xffff);

	arg = 0x7ff12a8976341122;
	err = sm_calc_l_cshift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 16) | 0x7ff1);

	arg = 0xfff12a8976341122;
	err = sm_calc_l_cshift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, arg * (1 << 16) | 0xfff1);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_r_cshift_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg;
	long long res;

	arg = 0ll;

	(void)state;

	err = sm_calc_r_cshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	err = sm_calc_r_cshift(arg, 100, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg = 19ll;

	err = sm_calc_r_cshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x8000000000000009);

	err = sm_calc_r_cshift(arg, 2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xc000000000000004);

	err = sm_calc_r_cshift(arg, 10, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x04c0000000000000);

	arg = 0x7ff12a8976341122;

	err = sm_calc_r_cshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x3ff89544bb1a0891);

	err = sm_calc_r_cshift(arg, 4, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x27ff12a897634112);

	err = sm_calc_r_cshift(arg, 32, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x763411227ff12a89);

	err = sm_calc_r_cshift(arg, 60, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xff12a89763411227);

	err = sm_calc_r_cshift(arg, 63, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffe25512ec682244);

	arg = 0xfff12a8976341122;

	err = sm_calc_r_cshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ff89544bb1a0891);

	err = sm_calc_r_cshift(arg, 2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xbffc4aa25d8d0448);

	err = sm_calc_r_cshift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x1122fff12a897634);

	err = sm_calc_r_cshift(arg, 32, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x76341122fff12a89);

	err = sm_calc_r_cshift(arg, 34, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x5d8d0448bffc4aa2);

	err = sm_calc_r_cshift(arg, 62, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffc4aa25d8d0448b);

	err = sm_calc_r_cshift(arg, 63, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffe25512ec682245);

	arg = 0x7ff1763411222a89;

	err = sm_calc_r_cshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xbff8bb1a08911544);

	err = sm_calc_r_cshift(arg, 4, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x97ff1763411222a8);

	err = sm_calc_r_cshift(arg, 32, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x11222a897ff17634);

	err = sm_calc_r_cshift(arg, 60, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xff1763411222a897);

	err = sm_calc_r_cshift(arg, 63, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffe2ec6822445512);

	arg = 0xfff1763411222a89;

	err = sm_calc_r_cshift(arg, 1, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfff8bb1a08911544);

	err = sm_calc_r_cshift(arg, 2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ffc5d8d04488aa2);

	err = sm_calc_r_cshift(arg, 16, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x2a89fff176341122);

	err = sm_calc_r_cshift(arg, 32, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x11222a89fff17634);

	err = sm_calc_r_cshift(arg, 34, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x04488aa27ffc5d8d);

	err = sm_calc_r_cshift(arg, 62, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffc5d8d04488aa27);

	err = sm_calc_r_cshift(arg, 63, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffe2ec6822445513);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_and_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg1;
	long long arg2;
	long long res;

	(void)state;

	arg1 = 0x0;
	arg2 = 0x0;

	err = sm_calc_and(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x0);

	arg1 = 0x01;
	arg2 = 0x10;

	err = sm_calc_and(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x0);

	arg1 = 0x0;
	arg2 = 0xffff;

	err = sm_calc_and(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x0);

	arg1 = 0x11;
	arg2 = 0x01;

	err = sm_calc_and(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x01);

	arg1 = 0x7ff1763411222a89;
	arg2 = 0x1122fff12a897634;

	err = sm_calc_and(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x1120763000002200);

	arg1 = 0xfff1763411222a89;
	arg2 = 0x9122fff12a897634;

	err = sm_calc_and(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x9120763000002200);

	arg1 = 0xfff1763411222a89;
	arg2 = 0x7122fff12a897634;

	err = sm_calc_and(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7120763000002200);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_or_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg1;
	long long arg2;
	long long res;

	(void)state;

	arg1 = 0x0;
	arg2 = 0x0;

	err = sm_calc_or(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x0);

	arg1 = 0x01;
	arg2 = 0x10;

	err = sm_calc_or(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x11);

	arg1 = 0x0;
	arg2 = 0xffff;

	err = sm_calc_or(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffff);

	arg1 = 0x11;
	arg2 = 0x01;

	err = sm_calc_or(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x11);

	arg1 = 0x7ff1763411222a89;
	arg2 = 0x1122fff12a897634;

	err = sm_calc_or(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ff3fff53bab7ebd);

	arg1 = 0xfff1763411222a89;
	arg2 = 0x9122fff12a897634;

	err = sm_calc_or(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfff3fff53bab7ebd);

	arg1 = 0xfff1763411222a89;
	arg2 = 0x7122fff12a897634;

	err = sm_calc_or(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfff3fff53bab7ebd);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_not_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg;
	long long res;

	(void)state;

	arg = 0x0;

	err = sm_calc_not(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffffffff);

	arg = 0xff00;

	err = sm_calc_not(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffff00ff);

	arg = 0x01;

	err = sm_calc_not(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfffffffffffffffe);

	arg = 0x11;

	err = sm_calc_not(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffffffee);

	arg = 0x7ff1763411222a89;

	err = sm_calc_not(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x800e89cbeeddd576);

	arg = 0xfff1763411222a89;

	err = sm_calc_not(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x000e89cbeeddd576);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_nand_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg1;
	long long arg2;
	long long res;

	(void)state;

	arg1 = 0x0;
	arg2 = 0x0;

	err = sm_calc_nand(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffffffff);

	arg1 = 0x01;
	arg2 = 0x10;

	err = sm_calc_nand(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffffffff);

	arg1 = 0x0;
	arg2 = 0xffff;

	err = sm_calc_nand(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffffffff);

	arg1 = 0x11;
	arg2 = 0x01;

	err = sm_calc_nand(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xfffffffffffffffe);

	arg1 = 0x7ff1763411222a89;
	arg2 = 0x1122fff12a897634;

	err = sm_calc_nand(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xeedf89cfffffddff);

	arg1 = 0xfff1763411222a89;
	arg2 = 0x9122fff12a897634;

	err = sm_calc_nand(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x6edf89cfffffddff);

	arg1 = 0xfff1763411222a89;
	arg2 = 0x7122fff12a897634;

	err = sm_calc_nand(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x8edf89cfffffddff);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_nor_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg1;
	long long arg2;
	long long res;

	(void)state;

	arg1 = 0x0;
	arg2 = 0x0;

	err = sm_calc_nor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffffffff);

	arg1 = 0x01;
	arg2 = 0x10;

	err = sm_calc_nor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffffffee);

	arg1 = 0x0;
	arg2 = 0xffff;

	err = sm_calc_nor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffff0000);

	arg1 = 0x11;
	arg2 = 0x01;

	err = sm_calc_nor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffffffffffffffee);

	arg1 = 0x7ff1763411222a89;
	arg2 = 0x1122fff12a897634;

	err = sm_calc_nor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x800c000ac4548142);

	arg1 = 0xfff1763411222a89;
	arg2 = 0x9122fff12a897634;

	err = sm_calc_nor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x000c000ac4548142);

	arg1 = 0xfff1763411222a89;
	arg2 = 0x7122fff12a897634;

	err = sm_calc_nor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x000c000ac4548142);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_xor_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg1;
	long long arg2;
	long long res;

	(void)state;

	arg1 = 0x0;
	arg2 = 0x0;

	err = sm_calc_xor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x0);

	arg1 = 0x01;
	arg2 = 0x10;

	err = sm_calc_xor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x11);

	arg1 = 0x11;
	arg2 = 0x10;

	err = sm_calc_xor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x01);

	arg1 = 0x0;
	arg2 = 0xffff;

	err = sm_calc_xor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xffff);

	arg1 = 0x11;
	arg2 = 0x01;

	err = sm_calc_xor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x10);

	arg1 = 0x7ff1763411222a89;
	arg2 = 0x1122fff12a897634;

	err = sm_calc_xor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x6ed389c53bab5cbd);

	arg1 = 0xfff1763411222a89;
	arg2 = 0x9122fff12a897634;

	err = sm_calc_xor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x6ed389c53bab5cbd);

	arg1 = 0xfff1763411222a89;
	arg2 = 0x7122fff12a897634;

	err = sm_calc_xor(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x8ed389c53bab5cbd);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_conv_bin_2_bin_sz_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	(void)state;

	sm_set_def_calc_params();

	sm_conv_bin_2_bin_sz_test_template(0ll, "0000");
	sm_conv_bin_2_bin_sz_test_template(1ll, "0001");
	sm_conv_bin_2_bin_sz_test_template(-1ll, "1111111111111111111111111111111111111111111111111111111111111111");
	sm_conv_bin_2_bin_sz_test_template(10ll, "1010");
	sm_conv_bin_2_bin_sz_test_template(-10ll, "1111111111111111111111111111111111111111111111111111111111110110");
	sm_conv_bin_2_bin_sz_test_template(100ll, "01100100");
	sm_conv_bin_2_bin_sz_test_template(-100ll, "1111111111111111111111111111111111111111111111111111111110011100");
	sm_conv_bin_2_bin_sz_test_template(0xfff1763411222a89, "1111111111110001011101100011010000010001001000100010101010001001");
	sm_conv_bin_2_bin_sz_test_template(0x1122fff12a897634, "0001000100100010111111111111000100101010100010010111011000110100");
	sm_conv_bin_2_bin_sz_test_template(0x9122fff12a897634, "1001000100100010111111111111000100101010100010010111011000110100");
	sm_conv_bin_2_bin_sz_test_template(0x1ed389c53bab5cbd, "0001111011010011100010011100010100111011101010110101110010111101");

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_f_precision_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	(void)state;

	sm_set_def_calc_params();

	sm_f_precision_test_template(SM_PREC_MAX + 1, 0);
	sm_f_precision_test_template(SM_PREC_MIN - 1, 0);
	sm_f_precision_test_template(5, 1);
	sm_f_precision_test_template(10, 1);
	sm_f_precision_test_template(15, 1);

	err = sm_set_f_exp_precision();
	assert_true(err == SM_RES_OK);
	assert_int_equal(gsm_calc_params.m_f_precision, -1);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_recip_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;
	long double res;

	(void)state;

	sm_set_def_calc_params();

	err = sm_calc_recip(0.0L, &res);
	assert_true(err == SM_RES_ERROR);

	err = sm_calc_recip(12.0L, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0833333333333333, gcsm_f_test_epsilon);

	err = sm_calc_recip(-12.0L, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -0.0833333333333333, gcsm_f_test_epsilon);

	err = sm_calc_recip(18963.1478599L, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 5.2733860822e-5, gcsm_f_test_epsilon);

	err = sm_calc_recip(100.0L, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.01, gcsm_f_test_epsilon);

	err = sm_calc_recip(-100.0L, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -0.01, gcsm_f_test_epsilon);

	err = sm_calc_recip(0.001747811221L, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 572.144169796470256, gcsm_f_test_epsilon);

	err = sm_calc_recip(0.000015912867L, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 62842.2269852444079524, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_mod_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg1;
	long long arg2;
	long long res;

	(void)state;

	arg1 = 0;
	arg2 = 2;
	err = sm_calc_mod(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg1 = 10;
	arg2 = 2;
	err = sm_calc_mod(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg1 = -10;
	arg2 = 2;
	err = sm_calc_mod(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg1 = 0x7ff1763411222a89;
	arg2 = 2;
	err = sm_calc_mod(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 1);

	arg1 = 0x1122fff12a897634;
	arg2 = 2;
	err = sm_calc_mod(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg1 = 0xfff1763411222a89;
	arg2 = 2;
	err = sm_calc_mod(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 1);

	arg1 = 0x9122fff12a897634;
	arg2 = 2;
	err = sm_calc_mod(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg1 = 0x7ff1763411222a89;
	arg2 = arg1 / 2;
	err = sm_calc_mod(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 1);

	arg1 = 0x9122fff12a897634;
	arg2 = arg1 / 2;
	err = sm_calc_mod(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg1 = 0x7ff1763411222a89;
	arg2 = arg1 / 2;
	err = sm_calc_mod(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 1);

	arg1 = 0x1122fff12a897634;
	arg2 = arg1 / 2;
	err = sm_calc_mod(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_abs_i_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg;
	long long res;

	(void)state;

	arg = 10;
	err = sm_calc_abs_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 10);

	arg = -10;
	err = sm_calc_abs_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 10);

	arg = LLONG_MIN;
	err = sm_calc_abs_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, LLONG_MIN);

	arg = LLONG_MAX;
	err = sm_calc_abs_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, LLONG_MAX);

	arg = 0x7ff1763411222a89;
	err = sm_calc_abs_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x7ff1763411222a89);

	arg = 0xfff1763411222a89;
	err = sm_calc_abs_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0xe89cbeeddd577);

	arg = 0xffffffffffffffff;
	err = sm_calc_abs_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 1);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_abs_f_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long double arg;
	long double res;

	(void)state;

	arg = 10.12344321;
	err = sm_calc_abs_f(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 10.12344321);

	arg = -10.12344321;
	err = sm_calc_abs_f(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 10.12344321);

	arg = -1.123e-5;
	err = sm_calc_abs_f(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 1.123e-5);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

void sm_calc_sqrt_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long double arg;
	long double res;

	(void)state;

	arg = 144.0L;
	err = sm_calc_sqrt(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 12.0L, gcsm_f_test_epsilon);

	arg = 169.0L;
	err = sm_calc_sqrt(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 13.0L, gcsm_f_test_epsilon);

	arg = -36.0L;
	err = sm_calc_sqrt(arg, &res);
	assert_true(err == SM_RES_ERROR);

	arg = 9999.9999L;
	err = sm_calc_sqrt(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 99.9999995L, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_ln_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long double arg;
	long double res;

	(void)state;

	arg = 100.0L;
	err = sm_calc_ln(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 4.605170185988091L, gcsm_f_test_epsilon);

	arg = 50.0L;
	err = sm_calc_ln(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 3.9120230054281460L, gcsm_f_test_epsilon);

	arg = 1.0L;
	err = sm_calc_ln(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0L, gcsm_f_test_epsilon);

	arg = 0.1L;
	err = sm_calc_ln(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -2.30258509299404L, gcsm_f_test_epsilon);

	arg = 0.01L;
	err = sm_calc_ln(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -4.6051701859880913L, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_lg_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long double arg;
	long double res;

	(void)state;

	arg = 100.0L;
	err = sm_calc_lg(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 2.0L, gcsm_f_test_epsilon);

	arg = 50.0L;
	err = sm_calc_lg(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.6989700043360187L, gcsm_f_test_epsilon);

	arg = 1.0L;
	err = sm_calc_lg(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0L, gcsm_f_test_epsilon);

	arg = 0.1L;
	err = sm_calc_lg(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -1.00L, gcsm_f_test_epsilon);

	arg = 0.01L;
	err = sm_calc_lg(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -2.00L, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_log_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long double arg;
	long double base;
	long double res;

	(void)state;

	arg = 2.0L;
	base = 2.0L;
	err = sm_calc_log(base, arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0L, gcsm_f_test_epsilon);

	arg = 100.0L;
	base = 2.0L;
	err = sm_calc_log(base, arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 6.6438561897747253L, gcsm_f_test_epsilon);

	arg = 10.0L;
	base = 2.0L;
	err = sm_calc_log(base, arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 3.3219280948873626L, gcsm_f_test_epsilon);

	arg = 0.1L;
	base = 2.0L;
	err = sm_calc_log(base, arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -3.3219280948873622L, gcsm_f_test_epsilon);

	arg = 0.01L;
	base = 2.0L;
	err = sm_calc_log(base, arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, -6.6438561897747244L, gcsm_f_test_epsilon);

	arg = -2.0L;
	base = 2.0L;
	err = sm_calc_log(base, arg, &res);
	assert_true(err == SM_RES_ERROR);

	arg = 2.0L;
	base = -2.0L;
	err = sm_calc_log(base, arg, &res);
	assert_true(err == SM_RES_ERROR);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_power_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long double arg1;
	long double arg2;
	long double res;

	(void)state;

	arg1 = 10.0L;
	arg2 = 2.0L;
	err = sm_calc_power(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 100.00L, gcsm_f_test_epsilon);

	arg1 = 12.0L;
	arg2 = 4.0L;
	err = sm_calc_power(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 20736.00L, gcsm_f_test_epsilon);

	arg1 = 144.0L;
	arg2 = 0.5L;
	err = sm_calc_power(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 12.00L, gcsm_f_test_epsilon);

	arg1 = -2.0L;
	arg2 = 2.0L;
	err = sm_calc_power(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 4.00L, gcsm_f_test_epsilon);

	arg1 = -4.0L;
	arg2 = 0.5L;
	err = sm_calc_power(arg1, arg2, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 2.00L, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_power_2_i_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg;
	long long res;

	(void)state;

	arg = 0;
	err = sm_calc_power_2_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 1ll);

	arg = 2ll;
	err = sm_calc_power_2_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 4ll);

	arg = 20ll;
	err = sm_calc_power_2_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x100000);

	arg = 32ll;
	err = sm_calc_power_2_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0x100000000);

	arg = -2ll;
	err = sm_calc_power_2_i(arg, &res);
	assert_true(err == SM_RES_ERROR);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_power_10_i_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg;
	long long res;

	(void)state;

	arg = 0;
	err = sm_calc_power_10_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 1ll);

	arg = 2ll;
	err = sm_calc_power_10_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 100ll);

	arg = 10ll;
	err = sm_calc_power_10_i(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 10000000000);

	arg = -2ll;
	err = sm_calc_power_10_i(arg, &res);
	assert_true(err == SM_RES_ERROR);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_power_i_2_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long long arg;
	long long res;

	(void)state;

	arg = 0;
	err = sm_calc_power_i_2(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 0);

	arg = 1ll;
	err = sm_calc_power_i_2(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 1ll);

	arg = 2ll;
	err = sm_calc_power_i_2(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 4ll);

	arg = -2ll;
	err = sm_calc_power_i_2(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 4ll);

	arg = -1000ll;
	err = sm_calc_power_i_2(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_int_equal(res, 1000 * 1000);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_power_2_d_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long double arg;
	long double res;

	(void)state;

	arg = 0.0L;
	err = sm_calc_power_2_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0L, gcsm_f_test_epsilon);

	arg = 1.0L;
	err = sm_calc_power_2_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 2.0L, gcsm_f_test_epsilon);

	arg = 2.1L;
	err = sm_calc_power_2_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 4.287093850145172L, gcsm_f_test_epsilon);

	arg = -2.1L;
	err = sm_calc_power_2_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.233258247884202L, gcsm_f_test_epsilon);

	arg = 10.45L;
	err = sm_calc_power_2_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1398.825222916501L, gcsm_f_test_epsilon);

	arg = -10.45L;
	err = sm_calc_power_2_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 7.148855937234499e-4L, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_power_10_d_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long double arg;
	long double res;

	(void)state;

	arg = 0.0L;
	err = sm_calc_power_10_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0L, gcsm_f_test_epsilon);

	arg = 1.0L;
	err = sm_calc_power_10_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 10.0L, gcsm_f_test_epsilon);

	arg = 2.1L;
	err = sm_calc_power_10_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 125.8925411794167210L, gcsm_f_test_epsilon);

	arg = -2.1L;
	err = sm_calc_power_10_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.007943282347242815L, gcsm_f_test_epsilon);

	arg = 10.45L;
	err = sm_calc_power_10_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 28183829312.644538191019234L, gcsm_f_test_epsilon);

	arg = -10.45L;
	err = sm_calc_power_10_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 3.5481338923357545843321870e-11L, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_power_exp_d_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long double arg;
	long double exp;
	long double res;

	(void)state;

	arg = 0.0L;
	err = sm_calc_power_exp_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0L, gcsm_f_test_epsilon);

	err = sm_get_const(SM_CONST_EXP, &exp);
	assert_true(err == SM_RES_OK);

	arg = 1.0L;
	err = sm_calc_power_exp_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, exp, gcsm_f_test_epsilon);

	arg = 2.1L;
	err = sm_calc_power_exp_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 8.16616991256765L, gcsm_f_test_epsilon);

	arg = -2.1L;
	err = sm_calc_power_exp_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.122456428252982L, gcsm_f_test_epsilon);

	arg = 5.45L;
	err = sm_calc_power_exp_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 232.7581659076620538L, gcsm_f_test_epsilon);

	arg = -5.45L;
	err = sm_calc_power_exp_d(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.00429630469075234057L, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_calc_power_d_2_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	long double arg;
	long double res;

	(void)state;

	arg = 0.0L;
	err = sm_calc_power_d_2(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 0.0L, gcsm_f_test_epsilon);

	arg = 1.0L;
	err = sm_calc_power_d_2(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 1.0L, gcsm_f_test_epsilon);

	arg = 2.1L;
	err = sm_calc_power_d_2(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 4.41L, gcsm_f_test_epsilon);

	arg = -2.1L;
	err = sm_calc_power_d_2(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 4.41L, gcsm_f_test_epsilon);

	arg = 10.45L;
	err = sm_calc_power_d_2(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 109.2025L, gcsm_f_test_epsilon);

	arg = -10.45L;
	err = sm_calc_power_d_2(arg, &res);
	assert_true(err == SM_RES_OK);
	assert_float_equal(res, 109.2025L, gcsm_f_test_epsilon);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_echo_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	char in_buf[SM_S_BUFF_SIZE];
	char out_buf[SM_S_BUFF_SIZE];

	char out_buf_s[10];

	(void)state;

	err = safe_strcpy(in_buf, SM_ARRAY_SIZE(in_buf), "test buffer message");
	assert_true(err == SM_RES_OK);

	err = sm_copy_buf(in_buf, out_buf, SM_ARRAY_SIZE(out_buf));
	assert_true(err == SM_RES_OK);
	assert_string_equal(in_buf, out_buf);

	err = sm_copy_buf(in_buf, out_buf_s, SM_ARRAY_SIZE(out_buf_s));
	assert_true(err == SM_RES_ERROR);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

	return;
}

static void sm_conv_quot_sz_2_sz_test(void** state)
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- >>");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is started");

	errno_t err;

	char in_buf[SM_S_BUFF_SIZE];
	char out_buf[SM_S_BUFF_SIZE];

	(void)state;

	err = safe_strcpy(in_buf, SM_ARRAY_SIZE(in_buf), "\"test buffer message\"");
	assert_true(err == SM_RES_OK);

	err = sm_conv_quot_sz_2_sz(in_buf, out_buf, SM_ARRAY_SIZE(out_buf));
	assert_true(err == SM_RES_OK);

	assert_string_equal(out_buf, "test buffer message");

	err = safe_strcpy(in_buf, SM_ARRAY_SIZE(in_buf), "test buffer message");
	assert_true(err == SM_RES_OK);

	err = sm_conv_quot_sz_2_sz(in_buf, out_buf, SM_ARRAY_SIZE(out_buf));
	assert_true(err == SM_RES_OK);

	assert_string_equal(in_buf, "test buffer message");

	err = safe_strcpy(in_buf, SM_ARRAY_SIZE(in_buf), "test buffer message\"");
	assert_true(err == SM_RES_OK);

	err = sm_conv_quot_sz_2_sz(in_buf, out_buf, SM_ARRAY_SIZE(out_buf));
	assert_true(err == SM_RES_OK);

	assert_string_equal(out_buf, "test buffer message");

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "test -------------------------------------- <<");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s","test",__FUNCTION__,"is finished: Ok");

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
		cmocka_unit_test(sm_get_const_test),
		cmocka_unit_test(sm_calc_divide_test),
		cmocka_unit_test(sm_a_unit_test),
		cmocka_unit_test(sm_i_format_test),
		cmocka_unit_test(sm_calc_trig_dir_test),
		cmocka_unit_test(sm_calc_trig_rev_test),
		cmocka_unit_test(sm_conv_trig_test),
		cmocka_unit_test(sm_calc_factorial_test),
		cmocka_unit_test(sm_calc_rand_test),
		cmocka_unit_test(sm_conv_i_dec_2_sz_test),
		cmocka_unit_test(sm_conv_i_dec_2_bin_test),
		cmocka_unit_test(sm_conv_sz_2_f_dec_test),
		cmocka_unit_test(sm_conv_sz_2_f_dec_test),
		cmocka_unit_test(sm_conv_i_test),
		cmocka_unit_test(sm_conv_f_test),
		cmocka_unit_test(sm_calc_l_ashift_test),
		cmocka_unit_test(sm_calc_r_ashift_test),
		cmocka_unit_test(sm_calc_l_lshift_test),
		cmocka_unit_test(sm_calc_r_lshift_test),
		cmocka_unit_test(sm_calc_l_cshift_test),
		cmocka_unit_test(sm_calc_r_cshift_test),
		cmocka_unit_test(sm_calc_and_test),
		cmocka_unit_test(sm_calc_or_test),
		cmocka_unit_test(sm_calc_not_test),
		cmocka_unit_test(sm_calc_nand_test),
		cmocka_unit_test(sm_calc_nor_test),
		cmocka_unit_test(sm_calc_xor_test),
		cmocka_unit_test(sm_conv_bin_2_bin_sz_test),
		cmocka_unit_test(sm_f_precision_test),
		cmocka_unit_test(sm_calc_recip_test),
		cmocka_unit_test(sm_calc_mod_test),
		cmocka_unit_test(sm_calc_abs_i_test),
		cmocka_unit_test(sm_calc_abs_f_test),
		cmocka_unit_test(sm_calc_sqrt_test),
		cmocka_unit_test(sm_calc_ln_test),
		cmocka_unit_test(sm_calc_lg_test),
		cmocka_unit_test(sm_calc_log_test),
		cmocka_unit_test(sm_calc_power_test),
		cmocka_unit_test(sm_calc_power_2_i_test),
		cmocka_unit_test(sm_calc_power_10_i_test),
		cmocka_unit_test(sm_calc_power_i_2_test),
		cmocka_unit_test(sm_calc_power_2_d_test),
		cmocka_unit_test(sm_calc_power_10_d_test),
		cmocka_unit_test(sm_calc_power_exp_d_test),
		cmocka_unit_test(sm_calc_power_d_2_test),
		cmocka_unit_test(sm_echo_test),
		cmocka_unit_test(sm_conv_quot_sz_2_sz_test),
	};

	sm_init_log_test();			//	initializing and starting the log output (file path, defined by SM_LOG_DPATH and SM_LOG_FNAME is used)

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s", __FUNCTION__, "---------------------------------------------------------------------- >>");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test is started");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");

	sm_init_random();			//	initializing the random seed (for 'rand[;]' command)
	sm_set_def_calc_params();	//	initializing the global sm_calc_params object
	sm_log_calc_params();		//	provides log out (in log file) the sm_calc_params gsm_calc_params, current state of sm_calculator configuration

	int res = cmocka_run_group_tests(tests, NULL, NULL);	//	execution of unit tests (array of void (*CMUnitTestFunction)(void **state) functions)
															//	the function cmocka_run_group_tests returns: total_failed + total_errors
															//	i.e. if result of execution of unit tests is OK, int res == 0

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "cmocka_run_group_tests: res = %d", res);
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc_test is finished");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s", __FUNCTION__, "---------------------------------------------------------------------- <<");

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
