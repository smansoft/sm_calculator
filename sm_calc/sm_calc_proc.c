/*
 *    Copyright (c) 2020-2021 SManSoft <http://www.smansoft.com/>
 *    Sergey Manoylo <info@smansoft.com>
 */

#include "pch.h"

#include "safe_lib_errno.h"
#include "safe_str_lib.h"
#include "safe_mem_lib.h"
#include "safe_fopen.h"
#include "safe_gets.h"

#include "getopt.h"
#include "sm_log.h"

#include "sm_calc_proc.h"

#include "sm_calc_y.h"
#include "sm_calc_l.h"

#include "sm_calc.h"

extern sm_log_config gsm_log_config;	//	global instance of main log support structure
#define SM_LOG_CONFIG &gsm_log_config	//	just synonym: SM_LOG_CONFIG == &gsm_log_config - for usage in log api calls

#if defined SM_PARSER_CTX
#undefine SM_PARSER_CTX
#endif

#define SM_PARSER_CTX parser_ctx

/*	error processor; this function is called by the syntax parser	*/
int yyerror(yyscan_t scanner, parser_callback_t parser_callback, const char* err_message)
{
	sm_parser_ctx* parser_ctx = yyget_extra(scanner);
	sm_print_error_ext(parser_ctx, parser_callback, err_message);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "yyerror: %s", err_message);
	return SM_RES_OK;
}

/*	the function, that prints error into yyout stream device	*/
errno_t sm_print_error(sm_parser_ctx* const parser_ctx, const char* const err_message)
{
	errno_t err = SM_RES_ERROR;
	int err_prn = fprintf(sm_get_yyout(parser_ctx->m_scanner), "Error: %s", err_message);
	err = (err_prn > 0) ? SM_RES_OK : SM_RES_ERROR;
	if (err == SM_RES_OK)
		SM_SET_CALC_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Error: %s", err_message);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that prints error into yyout stream device;
	additional info is printed:
		- current lexem;
		- length oof the current lexem;
		- internal data from YY_CURRENT_BUFFER;
*/
errno_t sm_print_error_ext(sm_parser_ctx* const parser_ctx, parser_callback_t parser_callback, const char* const err_message)
{
	errno_t err = SM_RES_ERROR;
	int err_prn;
	sm_l_buff_info l_buff_info;
	safe_memset(&l_buff_info, sizeof(sm_l_buff_info), 0);
	errno_t err_buf = sm_get_buff_info(parser_ctx->m_scanner, &l_buff_info);
	if(err_buf == SM_RES_OK)
		err_prn = fprintf(sm_get_yyout(parser_ctx->m_scanner), "Error: %s:\n\tcurrent buffer : %s\n\tcurrent error lexem: %s"
			"\n\tlength of the current error lexem: %d\n;",
			err_message, l_buff_info.yy_buf_pos,
			yyget_text(parser_ctx->m_scanner), yyget_leng(parser_ctx->m_scanner));
	else
		err_prn = fprintf(sm_get_yyout(parser_ctx->m_scanner), "Error: %s:\n\tcurrent lexem: %s\n\tlength of the current lexem: %d\n;",
			err_message, yyget_text(parser_ctx->m_scanner), yyget_leng(parser_ctx->m_scanner));
	err = (err_prn > 0) ? SM_RES_OK : SM_RES_ERROR;
	if (err == SM_RES_OK)
		SM_SET_CALC_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Error: %s", err_message);
	if (err_buf == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "current buffer: %s", l_buff_info.yy_buf_pos);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "current error lexem: %s", yyget_text(parser_ctx->m_scanner));
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "length of the current error lexem: %d", yyget_leng(parser_ctx->m_scanner));
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	exit from the application;
	setup
		gsm_exit = 1;
	or
		calls exit(0);

	this function is called, when user enters commands 'exit[;]' or 'quit[;]'
*/
void sm_do_exit(sm_parser_ctx* const parser_ctx)
{
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Exit. Good Bye!");
	fprintf(sm_get_yyout(parser_ctx->m_scanner), "Exit. Good Bye!\n");
	if (sm_get_yyin(parser_ctx->m_scanner) != NULL && sm_get_yyin(parser_ctx->m_scanner) != stdin && sm_get_yyin(parser_ctx->m_scanner) == parser_ctx->m_calc_params.m_in_file) {
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "exit(0)");
		exit(0);
	}
	else {
		parser_ctx->m_calc_params.m_exit = 1;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "setup parser_ctx->m_calc_params.m_exit flag: %d", parser_ctx->m_calc_params.m_exit);
	}
}

/*
	the function removes symbols ' and " (quotation marks) from input buffer and copies result
	into result buffer;

	in args:
		parser_ctx		- current parser context
		in_buf			- length of the buffer
		out_buf_len		- length of the out buffer (out_buf)
	out args:
		out_buf			- result

	for example:
		in_buf == "111""222"
		out_buf == 111222

		in_buf == "111"'222'"333"
		out_buf == 111222333

		in_buf == "111" '222' "333"
		out_buf == 111 222 333
*/
errno_t	sm_conv_quot_sz_2_sz(sm_parser_ctx* const parser_ctx, char* const in_buf, char* const out_buf, const size_t out_buf_len)
{
	errno_t err = SM_RES_ERROR;

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: in_buf = %s out_buf_len = %d", in_buf, out_buf_len);

	rsize_t in_len = safe_strnlen(in_buf, out_buf_len);
	if (in_len <= 0) {
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "in_len <= 0");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return err;
	}

	out_buf[0] = 0;

	char* ptr;
	char* res;

	res = safe_strtok(in_buf, &in_len, "\"\'", &ptr);
	if (res)
		err = safe_strcat(out_buf, out_buf_len, res);
	while (res != NULL && err == SM_RES_OK) {
		res = safe_strtok(NULL, &in_len, "\"\'", &ptr);
		if (res)
			err = safe_strcat(out_buf, out_buf_len, res);
	}
	rsize_t out_len = safe_strnlen(out_buf, out_buf_len);
	if (out_len > 0)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: out_buf = %s", out_buf);
	err = (out_len > 0) ? SM_RES_OK : SM_RES_ERROR;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	converting of sm_bin (native unsigned char format) to char sz array:

	for example:
		in:
			10101
		out:
			"00010101"
*/
errno_t sm_conv_bin_2_bin_sz(sm_parser_ctx* const parser_ctx, const sm_bin* const in_bin, char* const out_buf, const size_t out_buf_len)
{
	errno_t err = SM_RES_ERROR;
	if (out_buf_len < in_bin->m_len) {
		SM_SET_CALC_RES_ERROR("size of otput buffer should be >= then input buffer");
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "out_buf_len < in_bin->m_len");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return err;
	}
	sm_log_bin("sm_conv_bin_2_bin_sz: in_bin = ", in_bin);
	safe_memset(out_buf, out_buf_len, 0);
	unsigned int first_1_i = 0;
	for (int i = (int)(in_bin->m_len - 1); i >= 0; i--) {
		if (in_bin->m_data[i]) {
			first_1_i = i;
			break;
		}
	}
	unsigned int start_i = (first_1_i / 4 + 1) * 4;
	if (start_i > in_bin->m_len)
		start_i = (unsigned int)in_bin->m_len;
	for (int i = (start_i - 1), j = 0; i >= 0; i--, j++)
		out_buf[j] = '0' + in_bin->m_data[i];
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: out_buf = %s", out_buf);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	err = SM_RES_OK;
	return err;
}

/*
	converting of char sz array to sm_bin (native unsigned char format):

	for example:
		in:
			"00010101"
		out:
			10101
*/
errno_t sm_conv_bin_sz_2_bin(sm_parser_ctx* const parser_ctx, const char* const in_buf, const size_t in_buf_len, sm_bin* const out_bin)
{
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: in_buf = ", in_buf, in_buf_len);
	errno_t err = SM_RES_ERROR;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: in_buf = %s in_buf_len = %d", in_buf, in_buf_len);
	if (SM_ARRAY_SIZE(out_bin->m_data) < (in_buf_len - 1)) {
		SM_SET_CALC_RES_ERROR("size of otput buffer should be >= then output buffer");
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "out_buf_len < in_bin->m_len");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return err;
	}
	safe_memset(out_bin->m_data, SM_ARRAY_SIZE(out_bin->m_data), 0);
	size_t len = safe_strnlen(in_buf, in_buf_len);
	if (!len) {
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "len == 0");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return err;
	}

	for (unsigned int i = 0; i < len; i++)
		out_bin->m_data[i] = (in_buf[len - 1 - i] == '1') ? 1 : 0;

	out_bin->m_len = SM_ARRAY_SIZE(out_bin->m_data);
	err = SM_RES_OK;
	sm_log_bin("sm_conv_bin_sz_2_bin: out_bin = ", out_bin);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	converting of long long value to char sz array:

	for example:
		in:
			arg == 91167
		out:
			i_format==SM_I_10
				buf contains "91167";
			i_format==SM_I_8
				buf contains "0262037";
			i_format==SM_I_16
				buf contains "0x1641f";
			i_format==SM_I_2
				buf contains "b00010110010000011111";
*/
errno_t	sm_conv_i_dec_2_sz(sm_parser_ctx* const parser_ctx, const sm_i_format i_format, const long long arg, char* const buf, const size_t buf_len)
{
	errno_t err = SM_RES_ERROR;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: i_format = %d arg = %lld", i_format, arg);
	switch (i_format) {
	case SM_I_NO_FORMAT:
		SM_SET_CALC_RES_ERROR("wrong i_format value");
		err = SM_RES_ERROR;
		break;
	case SM_I_10:
	case SM_I_8:
	case SM_I_16: {
		int res = snprintf(buf, buf_len, gcsm_i_printf_formats[i_format], arg);
		err = (res > 0) ? SM_RES_OK : SM_RES_ERROR;
		break;
	}
	case SM_I_2: {
		sm_bin out_bin;
		err = sm_conv_i_dec_2_bin(parser_ctx, arg, &out_bin);
		if (err == SM_RES_OK) {
			char out_buf[SM_S_BUFF_SIZE + 1];
			err = sm_conv_bin_2_bin_sz(parser_ctx, &out_bin, out_buf, SM_ARRAY_SIZE(out_buf));
			if (err == SM_RES_OK) {
				int res = snprintf(buf, buf_len, gcsm_i_printf_formats[i_format], out_buf);
				err = (res > 0) ? SM_RES_OK : SM_RES_ERROR;
			}
		}
		break;
	}
	default:
		SM_SET_CALC_RES_ERROR("wrong i_format value");
		err = SM_RES_ERROR;
		break;
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: buf = %s", buf);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	converting of char sz array to long long value:

	for example:
		in:
			i_format==SM_I_10
				buf contains "91167";
			i_format==SM_I_8
				buf contains "0262037";
			i_format==SM_I_16
				buf contains "0x1641f";
			i_format==SM_I_2
				buf contains "b00010110010000011111";
		out:
			arg == 91167
*/
errno_t	sm_conv_sz_2_i_dec(sm_parser_ctx* const parser_ctx, const sm_i_format i_format, const unsigned char* const buf, const size_t buf_len, long long* const res)
{
	errno_t err = SM_RES_ERROR;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: i_format = %d buf = %s buf_len = %d", i_format, buf, buf_len);
	switch (i_format) {
	case SM_I_NO_FORMAT:
		SM_SET_CALC_RES_ERROR("wrong i_format value");
		err = SM_RES_ERROR;
		break;
	case SM_I_10:
	case SM_I_8:
	case SM_I_16: {
#if defined SM_OS_WINDOWS
		int res_s = sscanf_s(buf, gcsm_i_scanf_formats[i_format], res);
#elif defined SM_OS_LINUX
		int res_s = sscanf(buf, gcsm_i_scanf_formats[i_format], res);
#endif
		err = (res_s > 0) ? SM_RES_OK : SM_RES_ERROR;
		break;
	}
	case SM_I_2: {
		if (buf_len > 0) {
			const unsigned char* proc_buf;
			size_t proc_buf_len;
			if (buf[0] == 'b' || buf[0] == 'B') {
				proc_buf = buf + 1;
				proc_buf_len = buf_len - 1;
			}
			else {
				proc_buf = buf;
				proc_buf_len = buf_len;
			}
			sm_bin out_bin;
			err = sm_conv_bin_sz_2_bin(parser_ctx, proc_buf, proc_buf_len, &out_bin);
			if (err == SM_RES_OK)
				err = sm_conv_bin_2_i_dec(parser_ctx, &out_bin, res);
		}
		break;
	}
	default:
		SM_SET_CALC_RES_ERROR("wrong i_format value");
		err = SM_RES_ERROR;
		break;
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	converting of char sz array (ascii char format) to long double value:

	in:
		buf		- input char sz array (for example: "3.14159265358979323846", "1.2e-5")
		buf_len - size of input char sz array
	out:
		long double	- result variable
*/
errno_t	sm_conv_sz_2_f_dec(sm_parser_ctx* const parser_ctx, char* const buf, const size_t buf_len, long double* const res)
{
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: buf = %s buf_len = %d", buf, buf_len);
#if defined SM_OS_WINDOWS
	int res_s = sscanf_s(buf, gcsm_f_scanf_formats, res);
#elif defined SM_OS_LINUX
	int res_s = sscanf(buf, gcsm_f_scanf_formats, res);
#endif
	errno_t err = (res_s == 1) ? SM_RES_OK : SM_RES_ERROR;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	converting of long double value to char sz array (ascii char format):

	in:
		long double variable
	out:
		buf		- output char sz array (for example: "3.14159265358979323846", "1.2e-5")
		buf_len - size of input char sz array
*/
errno_t	sm_conv_f_dec_2_sz(sm_parser_ctx* const parser_ctx, const long double arg, char* const buf, const size_t buf_len)
{
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %lld", arg);
	int res = snprintf(buf, buf_len, parser_ctx->m_calc_params.m_f_print_formats[1], arg);
	errno_t err = (res > 0) ? SM_RES_OK : SM_RES_ERROR;
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: buf = %s", buf);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	converting of value (long long arg) to sm_bin (native unsigned char format):

	native unsigned char char format -
		array[i] == 0
			or
		array[i] == 1
	i.e. every element of sm_bin array contains 0 or 1

	in:
		long long variable
	out:
		sm_bin (native unsigned char format)
*/
errno_t sm_conv_i_dec_2_bin(sm_parser_ctx* const parser_ctx, const long long arg, sm_bin* const out_bin)
{
	errno_t err = SM_RES_ERROR;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %lld", arg);
	int	work_len = sizeof(out_bin->m_data);
	safe_memset(out_bin->m_data, SM_ARRAY_SIZE(out_bin->m_data), 0);
	BOOL			sign = (arg < 0);
	long long		res_abs = llabs(arg);
	for (int i = 0; i < work_len; i++) {
		out_bin->m_data[i] = (res_abs % 2);
		res_abs >>= 1;
		if (!res_abs)
			break;
	}
	if (res_abs) {
		SM_SET_CALC_RES_ERROR("wrong size of buffer; buffer is small");
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "res_abs != null: wrong size of buffer; buffer is small");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return err;
	}
	if (sign) {
		for (int i = 0; i < work_len; i++)
			out_bin->m_data[i] = out_bin->m_data[i] ? 0 : 1;
		BOOL hyphen = TRUE;
		for (int i = 0; i < work_len && hyphen; i++) {
			if (out_bin->m_data[i]) {
				out_bin->m_data[i] = 0;
				hyphen = TRUE;
			}
			else {
				out_bin->m_data[i] = 1;
				hyphen = FALSE;
			}
		}
	}
	out_bin->m_len = work_len;
	sm_log_bin("sm_conv_i_dec_2_bin: out_bin = ", out_bin);
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	converting of sm_bin (native unsigned char format) to long long value:

	native unsigned char char format -
		array[i] == 0
			or
		array[i] == 1
	i.e. every element of sm_bin array contains 0 or 1

	in:
		sm_bin (native unsigned char format)
	out:
		long long variable
*/
errno_t sm_conv_bin_2_i_dec(sm_parser_ctx* const parser_ctx, const sm_bin* const in_bin, long long* const res)
{
	errno_t err = SM_RES_ERROR;
	sm_log_bin("sm_conv_bin_2_i_dec: in_bin = ", in_bin);
	*res = 0ll;
	if (!in_bin->m_len) {
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "in_bin->m_len == 0");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return err;
	}
	BOOL minus = in_bin->m_data[in_bin->m_len - 1] > 0;
	if (minus && in_bin->m_len <= 1) {
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "minus == 0 || in_bin->m_len > 1");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return SM_RES_ERROR;
	}
	unsigned char* buf_uc = (unsigned char*)malloc(in_bin->m_len);
	safe_memset(buf_uc, in_bin->m_len, 0);
	if (minus) {
		unsigned char hyphen = 0;
		for (unsigned int i = 0; i < in_bin->m_len; i++) {
			unsigned char bit = in_bin->m_data[i] + 1 + hyphen;
			switch (bit) {
			case 1:
				hyphen = 0;
				buf_uc[i] = 1;
				break;
			case 2:
				hyphen = 1;
				buf_uc[i] = 0;
				break;
			case 3:
				hyphen = 1;
				buf_uc[i] = 1;
				break;
			}
		}
		for (unsigned int i = 0; i < in_bin->m_len; i++)
			buf_uc[i] = buf_uc[i] ? 0 : 1;
	}
	else
		safe_memcpy(buf_uc, in_bin->m_len, in_bin->m_data, in_bin->m_len);
	long long base = 1ll;
	for (unsigned int i = 0; i < (minus ? in_bin->m_len - 1 : in_bin->m_len); i++) {
		if (buf_uc[i])
			*res += base;
		base <<= 1;
	}
	*res = minus ? (-1) * *res : *res;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function gets long long argument, converts it into string, according to
	current output format of integers (decimal, octal, hexadecimal,
	binary formats) (from the current state of sm_calculator configuration)
	and prints out it in yyout stream device
*/
errno_t sm_print_ll(sm_parser_ctx* const parser_ctx, const long long arg)
{
	errno_t err = SM_RES_ERROR;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %lld", arg);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "print in format: parser_ctx->m_calc_params.m_i_format = %d", parser_ctx->m_calc_params.m_i_format);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "parser_ctx->m_calc_res.m_res = %d", parser_ctx->m_calc_res.m_res);
	switch (parser_ctx->m_calc_res.m_res) {
	case SM_RES_OK: {
		if (parser_ctx->m_calc_params.m_i_format == SM_I_2) {
			sm_bin out_bin;
			err = sm_conv_i_dec_2_bin(parser_ctx, arg, &out_bin);
			if (err == SM_RES_OK) {
				char out_buf[SM_S_BUFF_SIZE + 1];
				char out_prn_buf[SM_M_BUFF_SIZE];
				out_buf[0] = '\0';
				err = sm_conv_bin_2_bin_sz(parser_ctx , &out_bin, out_buf, SM_ARRAY_SIZE(out_buf));
				if (err != SM_RES_OK) {
					sm_print_error(parser_ctx, "error of converting long long from 10th to 2th format");
					sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
					return err;
				}
				int res = snprintf(out_prn_buf, SM_ARRAY_SIZE(out_prn_buf), gcsm_i_printf_formats[parser_ctx->m_calc_params.m_i_format], out_buf);
				if (res > 0)
					err = sm_print_sz(parser_ctx, out_prn_buf);
			}
			else
				sm_print_error(parser_ctx, "error of converting long long from 10th to 2th format");
		}
		else {
			char out_buf[SM_S_BUFF_SIZE + 1];
			int res = snprintf(out_buf, SM_ARRAY_SIZE(out_buf), gcsm_i_printf_formats[parser_ctx->m_calc_params.m_i_format], arg);
			if (res > 0) {
				sm_print_sz(parser_ctx, out_buf);
				err = SM_RES_OK;
			}
		}
		break;
	}
	case SM_RES_ERROR:
		sm_print_error(parser_ctx, parser_ctx->m_calc_res.m_sz_message);
		break;
	default:
		break;
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function gets long double argument and prints out it in yyout stream device,
	according to m_f_prec_buf (current precision buffer for printing of float values)
	(from the current state of sm_calculator configuration);
	m_f_prec_buf is updated, (using function sm_init_f_prec_buf()) after every updating
	the current precision of float values
*/
errno_t sm_print_ld(sm_parser_ctx* const parser_ctx, const long double arg)
{
	errno_t err = SM_RES_ERROR;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "print in format: parser_ctx->m_calc_params_proc.m_f_prec_buf = %s", parser_ctx->m_calc_params.m_f_prec_buf);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "parser_ctx->m_calc_res.m_res = %d", parser_ctx->m_calc_res.m_res);
	switch (parser_ctx->m_calc_res.m_res) {
	case SM_RES_OK: {
		char out_buf_prn[SM_L_BUFF_SIZE];
		int res = snprintf(out_buf_prn, SM_ARRAY_SIZE(out_buf_prn), parser_ctx->m_calc_params.m_f_prec_buf, arg);
		if (res > 0)
			err = sm_print_sz(parser_ctx, out_buf_prn);
		break;
	}
	case SM_RES_ERROR:
		sm_print_error(parser_ctx, parser_ctx->m_calc_res.m_sz_message);
		break;
	default:
		sm_print_error(parser_ctx, "wrong current state of exec result storage");
		break;
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*	the function, that prints string (ascii) message (arg) into yyout stream device	*/
errno_t sm_print_sz(sm_parser_ctx* const parser_ctx, const char* const arg)
{
	errno_t err = SM_RES_ERROR;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %s", arg);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "parser_ctx->m_calc_res.m_res = %d", parser_ctx->m_calc_res.m_res);
	switch (parser_ctx->m_calc_res.m_res) {
	case SM_RES_OK: {
		int err_prn = fprintf(sm_get_yyout(parser_ctx->m_scanner), gcsm_s_format, arg);
		err = (err_prn > 0) ? SM_RES_OK : SM_RES_ERROR;
		break;
	}
	case SM_RES_ERROR:
		sm_print_error(parser_ctx, parser_ctx->m_calc_res.m_sz_message);
		break;
	default:
		sm_print_error(parser_ctx, "wrong current state of exec result storage");
		break;
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*	the function, that prints '\n' (end of line) symbol into yyout stream device	*/
errno_t sm_print_nl(sm_parser_ctx* const parser_ctx)
{
	errno_t err = SM_RES_ERROR;
	int err_prn = fprintf(sm_get_yyout(parser_ctx->m_scanner), gcsm_nl);
	err = (err_prn > 0) ? SM_RES_OK : SM_RES_ERROR;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	just copying buffer;

	in args:
		parser_ctx		- current parser context
		in_buf			- source buffer
		out_buf_len		- destination buffer length
	out args:
		out_buf			- destination buffer

	this function is called, when user enters sting in command prompt:  "string_value" | 'string_value'
*/
errno_t sm_copy_buf(sm_parser_ctx* const parser_ctx, const char* const in_buf, char* const out_buf, const size_t out_buf_len)
{
	errno_t err;
	errno_t err_cpy;
	err_cpy = safe_strcpy(out_buf, out_buf_len, in_buf);
	err = (err_cpy == SM_RES_OK) ? SM_RES_OK : SM_RES_ERROR;
	return err;
}

/*
	copying current configuration into buffer in format:

		Current Configuration :
		---------------------------------------------
		Current double precision          : 10;
		Current output format of integers : dec;
		Unit of measurement of an angle   : rad;
	;

	in args:
		parser_ctx		- current parser context
		buf_len			- destination buffer length
	out args:
		buf				- destination buffer

	this function is called, when user enters command 'config[;]'
*/
errno_t	sm_get_config(sm_parser_ctx* const parser_ctx, char* const buf, const size_t buf_len)
{
	errno_t err = SM_RES_ERROR;
	int err_prn;
	char prec_buf[SM_S_BUFF_SIZE];
	if (parser_ctx->m_calc_params.m_f_precision == SM_PREC_EXP) {
		err = safe_strcpy(prec_buf, SM_ARRAY_SIZE(prec_buf), "EXPONENTIAL");
		if (err) {
			SM_SET_CALC_RES_ERROR("error copying of precision description into buffer");
			err = SM_RES_ERROR;
			sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
			return err;
		}
	}
	else {
		err_prn = snprintf(prec_buf, SM_ARRAY_SIZE(prec_buf), "%d", parser_ctx->m_calc_params.m_f_precision);
		if (err_prn <= 0) {
			SM_SET_CALC_RES_ERROR("error copying of precision description into buffer");
			err = SM_RES_ERROR;
			sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
			return err;
		}
	}

	err_prn = snprintf(buf, buf_len, "Current Configuration :\n"
		"---------------------------------------------\n"
		"Current double precision          : %s;\n"
		"Current output format of integers : %s;\n"
		"Unit of measurement of an angle   : %s",
		prec_buf,
		gcsm_i_formats[parser_ctx->m_calc_params.m_i_format],
		gcsm_trig_units[parser_ctx->m_calc_params.m_trig_unit]);

	if (err_prn > 0) {
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: buf = %s", buf);
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("size of buffer is too small");
		err = SM_RES_ERROR;
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	this function reads file from gcsm_gelp_fpath path and print out
	content of this file into stdout device;
	gsm_help_fpath is read
*/
errno_t sm_print_help(sm_parser_ctx* const parser_ctx)
{
	errno_t err = SM_RES_OK;
	char buf[SM_L_BUFF_SIZE];
	char* buf_in;
	FILE* help_file_h;

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "opening of the help file: %s", parser_ctx->m_calc_params.m_help_fpath);
	err = safe_fopen(&help_file_h, parser_ctx->m_calc_params.m_help_fpath, "r", SM_SFOPEN_NSHARED_TYPE);
	if (err == SAFE_RES_ERROR || help_file_h == NULL) {
		err = SM_RES_ERROR;
		char err_buf[SM_L_BUFF_SIZE];
		snprintf(err_buf, SM_ARRAY_SIZE(err_buf), "help file %s hasn't been opened for reading", parser_ctx->m_calc_params.m_help_fpath);
		SM_SET_CALC_RES_ERROR(err_buf);
		sm_print_error(parser_ctx, err_buf);
		err = SM_RES_ERROR;
		return err;
	}
	unsigned int count = 0;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "reading help (from the file: %s)", parser_ctx->m_calc_params.m_help_fpath);
	printf("Help (from the file: %s):\n", parser_ctx->m_calc_params.m_help_fpath);
	while (TRUE) {
		if (count == 30) {
			printf("\nPress '<Enter>' button to continue.\n");
			printf("':q<Enter>' or ':x<Enter>' for exit from Help...\n");
			safe_memset(buf, SM_ARRAY_SIZE(buf), 0);
			buf_in = safe_gets(stdin, buf, SM_ARRAY_SIZE(buf));
			if (!buf_in || buf[0] == ':' && (buf[1] == tolower('q') || buf[1] == tolower('x')))
				break;
			count = 0;
		}
		if (feof(help_file_h))
			break;
		safe_memset(buf, SM_ARRAY_SIZE(buf), 0);
		buf_in = fgets(buf, SM_ARRAY_SIZE(buf), help_file_h);
		if (!buf_in)
			break;
		printf("%s", buf);
		count++;
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "closing of the help file: %s", parser_ctx->m_calc_params.m_help_fpath);
	fclose(help_file_h);
	return err;
}

/*
	the function, that calculates and returns converting value of
	unit for measuring of angles == radians ('rad');

	in args:
		arg			- argument
	out args:
		res			- result

	where arg - float value (long double); arg should be value of current unit for measuring of angles;
	current unit for measuring of angles is read from the current state of sm_calculator configuration;
	res - float value (long double); result is a value of unit for measuring of angles == radians ('rad');
*/
errno_t	sm_conv_curr_2_rad(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "parser_ctx->m_calc_params.m_trig_unit = %d", parser_ctx->m_calc_params.m_trig_unit);
	switch (parser_ctx->m_calc_params.m_trig_unit)
	{
	case SM_TRIG_RADIANS:
		*res = arg;
		err = SM_RES_OK;
		break;
	case SM_TRIG_DEGREES:
		err = sm_conv_deg_2_rad(parser_ctx, arg, res);
		break;
	case SM_TRIG_GRADIAN:
		err = sm_conv_grad_2_rad(parser_ctx, arg, res);
		break;
	default:
		err = SM_RES_ERROR;
		break;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns converting of value of current unit for measuring of angles;
	argument is value of unit for measuring of angles == radians ('rad');

	in args:
		rad			- argument
	out args:
		res			- result

	where rad - float value (long double); arg should be value of unit for measuring of
	angles == radians ('rad');
	res - float value (long double); result is a value of current unit for measuring of angles;
	current unit for measuring of angles is read from the current state of sm_calculator configuration;
*/
errno_t	sm_conv_rad_2_curr(sm_parser_ctx* const parser_ctx, const long double rad, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: rad = %.10Lf", rad);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "parser_ctx->m_calc_params.m_trig_unit = %d", parser_ctx->m_calc_params.m_trig_unit);
	switch (parser_ctx->m_calc_params.m_trig_unit)
	{
	case SM_TRIG_RADIANS:
		*res = rad;
		err = SM_RES_OK;
		break;
	case SM_TRIG_DEGREES:
		err = sm_conv_rad_2_deg(parser_ctx, rad, res);
		break;
	case SM_TRIG_GRADIAN:
		err = sm_conv_rad_2_grad(parser_ctx, rad, res);
		break;
	default:
		err = SM_RES_ERROR;
		break;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	trigonometric function retuns float (long double) result of sine of float (long double) argument;

	in args:
		arg		- argument
	out args:
		res		- result

	argument should be defined as value of current unit for measuring of angles(deg|rad|grad);
	the function is called, when user uses function 'sin' in expression(s)
*/
errno_t	sm_calc_sin(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)
{
	errno_t err;
	long double arg_rad;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	err = sm_conv_curr_2_rad(parser_ctx, arg, &arg_rad);
	if (err == SM_RES_ERROR) {
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return err;
	}
	*res = sinl(arg_rad);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	trigonometric function retuns float (long double) result of cosine of float (long double) argument;

	in args:
		arg		- argument
	out args:
		res		- result

	argument should be defined as value of current unit for measuring of angles(deg|rad|grad);
	the function is called, when user uses function 'cos' in expression(s)
*/
errno_t	sm_calc_cos(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)
{
	errno_t err;
	long double arg_rad;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	err = sm_conv_curr_2_rad(parser_ctx, arg, &arg_rad);
	if (err == SM_RES_ERROR) {
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return err;
	}
	*res = cosl(arg_rad);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	trigonometric function retuns float (long double) result of tangent of float (long double) argument;

	in args:
		arg		- argument
	out args:
		res		- result

	argument should be defined as value of current unit for measuring of angles(deg|rad|grad);
	the function is called, when user uses function 'tan' in expression(s)
*/
errno_t	sm_calc_tan(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)
{
	errno_t err;
	long double arg_rad;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	err = sm_conv_curr_2_rad(parser_ctx, arg, &arg_rad);
	if (err == SM_RES_ERROR) {
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return err;
	}
	*res = tanl(arg_rad);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	trigonometric function retuns float (long double) result of arcsine of float (long double) argument;

	in args:
		arg		- argument
	out args:
		res		- result

	result of this function is defined as value of current unit for measuring of angles(deg|rad|grad);
	the function is called, when user uses function 'asin' in expression(s)
*/
errno_t	sm_calc_asin(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)
{
	errno_t err;
	long double res_rad;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	res_rad = asinl(arg);
	err = sm_conv_rad_2_curr(parser_ctx, res_rad, res);
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	trigonometric function retuns float (long double) result of arccosine of float (long double) argument;

	in args:
		arg		- argument
	out args:
		res		- result

	result of this function is defined as value of current unit for measuring of angles(deg|rad|grad);
	the function is called, when user uses function 'acos' in expression(s)
*/
errno_t	sm_calc_acos(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)
{
	errno_t err;
	long double res_rad;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	res_rad = acosl(arg);
	err = sm_conv_rad_2_curr(parser_ctx, res_rad, res);
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	trigonometric function retuns float (long double) result of arctangent of float (long double) argument;

	in args:
		arg		- argument
	out args:
		res		- result

	result of this function is defined as value of current unit for measuring of angles(deg|rad|grad);
	the function is called, when user uses function 'atan' in expression(s)
*/
errno_t	sm_calc_atan(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)
{
	errno_t err;
	long double res_rad;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	res_rad = atanl(arg);
	err = sm_conv_rad_2_curr(parser_ctx, res_rad, res);
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	function returns long double constant value by constant index:
		SM_CONST_PI = 0,
		SM_CONST_EXP,
		SM_CONST_GAMMA,
		SM_CONST_PHI,

		SM_CONST_GRAD_RAD,		//	for converting Gradians -> Radians
		SM_CONST_RAD_GRAD,		//	for converting Radians -> Gradians

		SM_CONST_RAD_DEG,		//	for converting Radians -> Degree
		SM_CONST_DEG_RAD,		//	for converting Degree -> Radians

		SM_CONST_GRAD_DEG,		//	for converting Gradians -> Degree
		SM_CONST_DEG_GRAD		//	for converting Degree -> Gradians
	;

	in args:
		parser_ctx		- current parser context
		const_idx		- constant index

	out args:
		const_value		- result (constant value)

	this function is called, when user uses constants:
		pi,
		exp,
		gamms,
		phi
	in expression(s);
	also this function is called in functions, which provide converting of
	unit for measuring of angles
*/
errno_t  sm_get_const(sm_parser_ctx* const parser_ctx, const sm_const_idx const_idx, long double* const const_value)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: const_idx = %d", const_idx);
	if (SM_CONST_DATA_MIN <= const_idx && const_idx <= SM_CONST_DATA_MAX) {
		*const_value = gcsm_consts[const_idx];
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: const_value = %.10Lf", *const_value);
		err = SM_RES_OK;
	}
	else
		err = SM_RES_ERROR;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns converting of value of unit for measuring of
	angles == radians ('rad'); argument is value of unit for measuring of angles == gradians ('grad');

	in args:
		parser_ctx		- current parser context
		grad			- argument
	out args:
		res				- result

	where grad - float value (long double); grad should be value of unit for measuring of
	angles == gradians ('grad');
	res - float value (long double); result is a value of unit for measuring of
	angles == radians ('rad');
*/
errno_t sm_conv_grad_2_rad(sm_parser_ctx* const parser_ctx, const long double grad, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: grad = %.10Lf", grad);
	long double grad_rad = 0;
	err = sm_get_const(parser_ctx, SM_CONST_GRAD_RAD, &grad_rad);
	if (err == SM_RES_OK) {
		*res = grad * grad_rad;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns converting of value of unit for measuring of
	angles == gradians ('grad'); argument is value of unit for measuring of angles == radians ('rad');

	in args:
		parser_ctx		- current parser context
		rad				- argument
	out args:
		res				- result

	where rad - float value (long double); rad should be value of unit for measuring of
	angles == radians ('rad');
	res - float value (long double); result is a value of unit for measuring of
	angles == gradians ('grad');
*/
errno_t sm_conv_rad_2_grad(sm_parser_ctx* const parser_ctx, const long double rad, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: rad = %.10Lf", rad);
	long double rad_grad = 0;
	err = sm_get_const(parser_ctx, SM_CONST_RAD_GRAD, &rad_grad);
	if (err == SM_RES_OK) {
		*res = rad * rad_grad;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns converting of value of unit for measuring of
	angles == degrees ('deg'); argument is value of unit for measuring of angles == radians ('rad');

	in args:
		parser_ctx		- current parser context
		rad				- argument
	out args:
		res				- result

	where rad - float value (long double); rad should be value of unit for measuring of
	angles == radians ('rad');
	res - float value (long double); result is a value of unit for measuring of
	angles == degrees ('deg');
*/
errno_t sm_conv_rad_2_deg(sm_parser_ctx* const parser_ctx, const long double rad, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: rad = %.10Lf", rad);
	long double rad_deg = 0;
	err = sm_get_const(parser_ctx, SM_CONST_RAD_DEG, &rad_deg);
	if (err == SM_RES_OK) {
		*res = rad * rad_deg;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns converting of value of unit for measuring of
	angles == radians ('rad'); argument is value of unit for measuring of angles == degrees ('deg');

	in args:
		parser_ctx		- current parser context
		deg				- argument
	out args:
		res				- result

	where deg - float value (long double); deg should be value of unit for measuring of
	angles == degrees ('deg');
	res - float value (long double); result is a value of unit for measuring of
	angles == radians ('rad');
*/
errno_t sm_conv_deg_2_rad(sm_parser_ctx* const parser_ctx, const long double deg, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: deg = %.10Lf", deg);
	long double deg_rad = 0;
	err = sm_get_const(parser_ctx, SM_CONST_DEG_RAD, &deg_rad);
	if (err == SM_RES_OK) {
		*res = deg * deg_rad;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns converting of value of unit for measuring of
	angles == degrees ('deg'); argument is value of unit for measuring of angles == gradians ('grad');

	in args:
		parser_ctx		- current parser context
		grad			- argument
	out args:
		res				- result

	where grad - float value (long double); grad should be value of unit for measuring of
	angles == gradians ('grad');
	res - float value (long double); result is a value of unit for measuring of
	angles == degrees ('deg');
*/
errno_t sm_conv_grad_2_deg(sm_parser_ctx* const parser_ctx, const long double grad, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: grad = %.10Lf", grad);
	long double grad_deg = 0;
	err = sm_get_const(parser_ctx, SM_CONST_GRAD_DEG, &grad_deg);
	if (err == SM_RES_OK) {
		*res = grad * grad_deg;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns converting of value of unit for measuring of
	angles == gradians ('grad'); argument is value of unit for measuring of angles == degrees ('deg');

	in args:
		parser_ctx		- current parser context
		deg				- argument
	out args:
		res				- result

	where deg - float value (long double); deg should be value of unit for measuring of
	angles == degrees ('deg');
	res - float value (long double); result is a value of unit for measuring of
	angles == gradians ('grad');
*/
errno_t sm_conv_deg_2_grad(sm_parser_ctx* const parser_ctx, const long double deg, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: grad = %.10Lf", deg);
	long double deg_grad = 0;
	err = sm_get_const(parser_ctx, SM_CONST_DEG_GRAD, &deg_grad);
	if (err == SM_RES_OK) {
		*res = deg * deg_grad;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function that setup and returns current precision of float values;

	in args:
		parser_ctx		- current parser context
		precision		- argument
	out args:
		res				- result

	where precision - integer value (long long); precision should be [0,20];
	result - integer value (long long);

	the function is called, when user calls command 'f_precision = N[;]'
*/
errno_t sm_set_f_precision(sm_parser_ctx* const parser_ctx, const long long precision, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: precision = %d", precision);
	if (SM_PREC_MIN <= precision && precision <= SM_PREC_MAX) {
		parser_ctx->m_calc_params.m_f_precision = (int)precision;
		*res = precision;
		err = sm_init_f_prec_buf(parser_ctx);
		if (err == SM_RES_OK) {
			sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
			SM_SET_CALC_RES_OK
		}
	}
	else {
		char buf[SM_L_BUFF_SIZE] = { 0 };
		int res = snprintf(buf, SM_L_BUFF_SIZE, "wrong parameter of command 'f_precision': arg should be >= %d and <= %d", SM_PREC_MIN, SM_PREC_MAX);
		if (res > 0)
			SM_SET_CALC_RES_ERROR(buf)
		else
			SM_SET_CALC_RES_ERROR("wrong parameter of command 'f_precision'")
			err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK) {
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function that setup current precision of float values as 'exp' (-1);

	the function is called, when user calls command 'f_precision = exp[;]'
*/
errno_t sm_set_f_exp_precision(sm_parser_ctx* const parser_ctx)
{
	errno_t err;
	parser_ctx->m_calc_params.m_f_precision = SM_PREC_EXP;
	err = sm_init_f_prec_buf(parser_ctx);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function that returns current precision of float values and copying precision
	current precision of float values in buffer;

	in args:
		parser_ctx		- current parser context
		buf_len			- length of the buffer
	out args:
		buf				- result

	buf == [0,20]|exp

	the function is called, when user calls commands 'f_precision = exp[;]' or 'f_precision[;]'
*/
errno_t	sm_get_f_precision(sm_parser_ctx* const parser_ctx, char* buf, const size_t buf_len)
{
	errno_t err = SM_RES_ERROR;
	if (SM_PREC_MIN <= parser_ctx->m_calc_params.m_f_precision && parser_ctx->m_calc_params.m_f_precision <= SM_PREC_MAX)
		err = sm_conv_i_dec_2_sz(parser_ctx,
			parser_ctx->m_calc_params.m_i_format, (const long long)parser_ctx->m_calc_params.m_f_precision, buf, buf_len);
	else if (parser_ctx->m_calc_params.m_f_precision == SM_PREC_EXP) {
		err = safe_strcpy(buf, buf_len, gcsm_exp_precision);
		if (!err) {
			SM_SET_CALC_RES_OK;
		}
		else
			err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK) {
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: buf = %s", buf);
		SM_SET_CALC_RES_OK;
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	copying current unit for measuring of angles (radians, degrees, gradians), which are used
	by trigonometric functions in buffer;
	current unit for measuring of angles is read from the current state of sm_calculator configuration;

	in args:
		parser_ctx		- current parser context
		buf_len			- destination buffer length
	out args:
		buf				- destination buffer

	this function is called, when user enters commands:
		a_unit = <deg|rad|grad>[;]
		a_unit[;]
		deg[;]
		rad[;]
		grad[;]
*/
errno_t	sm_get_a_unit(sm_parser_ctx* const parser_ctx, char* const buf, const size_t buf_len)
{
	errno_t err;
	if (SM_TRIG_UNIT_MIN <= parser_ctx->m_calc_params.m_trig_unit && parser_ctx->m_calc_params.m_trig_unit <= SM_TRIG_UNIT_MAX)
		err = safe_strcpy(buf, buf_len, gcsm_trig_units[parser_ctx->m_calc_params.m_trig_unit]);
	else
		err = SM_RES_ERROR;
	if (err == SM_RES_OK) {
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: buf = %s", buf);
		SM_SET_CALC_RES_OK;
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	setup current unit for measuring of angles (radians, degrees, gradians), which are used
	by trigonometric functions in buffer in global instance of definition the current state
	of sm_calculator configuration (sm_calc_params gsm_calc_param);

	in args:
		parser_ctx		- current parser context
		trig_unit		- unit for measuring of angles (radians, degrees, gradians)

	this function is called, when user enters commands:
		a_unit = <deg|rad|grad>[;]
		deg[;]
		rad[;]
		grad[;]
*/
errno_t sm_set_a_unit(sm_parser_ctx* const parser_ctx, const sm_trig_unit trig_unit)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: trig_unit = %d", trig_unit);
	if (SM_TRIG_UNIT_MIN <= trig_unit && trig_unit <= SM_TRIG_UNIT_MAX) {
		parser_ctx->m_calc_params.m_trig_unit = trig_unit;
		SM_SET_CALC_RES_OK;
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("setuping a_unit: wrong format value: arg should be: [SM_TRIG_RADIANS('rad'), SM_TRIG_DEGREES('deg'), SM_TRIG_GRADIAN('grad')]");
		err = SM_RES_ERROR;
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	copying current entering and output formats of integers (decimal, octal, hexadecimal, binary formats)
	in buffer; current entering and output formats of integers is read from the current state
	of sm_calculator configuration;

	in args:
		parser_ctx		- current parser context
		buf_len			- destination buffer length
	out args:
		buf				- destination buffer

	this function is called, when user enters commands:
		i_format = <dec|hex|oct|bin>[;]
		i_format[;]
		dec[;]
		oct[;]
		hex[;]
		bin[;]
*/
errno_t	sm_get_i_format(sm_parser_ctx* const parser_ctx, char* const buf, const size_t buf_len)
{
	errno_t err;
	if (SM_I_FORMAT_MIN <= parser_ctx->m_calc_params.m_i_format && parser_ctx->m_calc_params.m_i_format <= SM_I_FORMAT_MAX) {
		err = safe_strcpy(buf, buf_len, gcsm_i_formats[parser_ctx->m_calc_params.m_i_format]);
		SM_SET_CALC_RES_OK;
		err = SM_RES_OK;
	}
	else
		err = SM_RES_ERROR;
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: buf = %s", buf);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	setup current entering and output formats of integers (decimal, octal, hexadecimal, binary formats)
	in global instance of definition the current state of sm_calculator configuration
	(sm_calc_params gsm_calc_param);

	in args:
		parser_ctx		- current parser context
		i_format		- entering and output formats of integers (decimal, octal, hexadecimal, binary formats)

	this function is called, when user enters commands:
		i_format = <dec|hex|oct|bin>[;]
		dec[;]
		oct[;]
		hex[;]
		bin[;]
*/
errno_t sm_set_i_format(sm_parser_ctx* const parser_ctx, const sm_i_format i_format)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: i_format = %d", i_format);
	if (SM_I_FORMAT_MIN <= i_format && i_format <= SM_I_FORMAT_MAX) {
		parser_ctx->m_calc_params.m_i_format = i_format;
		SM_SET_CALC_RES_OK;
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("setuping i_format: wrong format value: arg should be: [SM_I_10('dec'), SM_I_8('oct'), SM_I_16('hex'), SM_I_2('bin')]");
		err = SM_RES_ERROR;
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	function retuns integer (long long) result of factorial of integer (long long) argument;

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	the function is called, when user uses operator '!' in expression(s);

	For example:
		In  >> 3!
		Out >> 6;

		In  >> 5!
		Out >> 120;
*/
errno_t sm_calc_factorial(sm_parser_ctx* const parser_ctx, const long long arg, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %lld", arg);
	if (arg >= 0) {
		*res = 1;
		for (long long i = 1; i <= arg; i++)
			*res *= i;
		SM_SET_CALC_RES_OK;
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of operator '!': arg should be >= 0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function returns float (long double) result: arg1 (long double) raised to the power
	of arg2 (long double);

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	where arg1 - float value (long double), arg2 - float value (long double);
	result - float value (long double);

	i.e. the function calculates:
		*res = arg1 ** arg2
	or
		pow(arg1, arg2 )

	the function is called, when user uses function 'pow' in expression(s)
*/
errno_t	sm_calc_power(sm_parser_ctx* const parser_ctx, const long double arg1, const long double arg2, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %.10Lf arg2 = %.10Lf", arg1, arg2);
	*res = powl(arg1, arg2);
	SM_SET_CALC_RES_OK;
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns 'exp' (2.71828182845904523536L)
	raised to the power of arg;

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - float value (long double);
	result - float value (long double);

	i.e. the function calculates:
		*res = exp ** arg
	or
		pow(exp, arg )

	the function is called, when user uses function 'pow_exp^' in expression(s)
*/
errno_t	sm_calc_power_exp_d(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	long double exp;
	err = sm_get_const(parser_ctx, SM_CONST_EXP, &exp);
	if (err == SM_RES_OK) {
		*res = powl(gcsm_consts[SM_CONST_EXP], arg);
		SM_SET_CALC_RES_OK;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns '2' raised to the power of arg;

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - integer value (long long); arg should be >= 0;
	result - integer value (long long);

	i.e. the function calculates:
		*res = 2 ** arg
	or
		(int)pow(2, arg )

	the function is called, when user uses function 'pow_2^' in expression(s)
*/
errno_t sm_calc_power_2_i(sm_parser_ctx* const parser_ctx, const long long arg, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %lld", arg);
	if (arg >= 0) {
		if (!arg)
			*res = 1;
		else if (arg > 0)
			*res = (1LL << arg);
		SM_SET_CALC_RES_OK;
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of function '2^': arg should be >= 0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns '10' raised to the power of arg;

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - integer value (long long); arg should be >= 0;
	result - integer value (long long);

	i.e. the function calculates:
		*res = 10 ** arg
	or
		(int)pow(10, arg )

	the function is called, when user uses function 'pow_10^' in expression(s)
*/
errno_t sm_calc_power_10_i(sm_parser_ctx* const parser_ctx, const long long arg, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %lld", arg);
	if (arg >= 0) {
		*res = 1;
		for (long long i = 0; i < arg; i++)
			*res *= 10ll;
		SM_SET_CALC_RES_OK;
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of operator '2^': arg should be >= 0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns argument raised to the power of '2';

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - integer value (long long);
	result - integer value (long long);

	i.e. the function calculates:
		*res = arg ** 2
	or
		*res = arg * arg

	the function is called, when user uses function 'pow_^2' in expression(s)
*/
errno_t sm_calc_power_i_2(sm_parser_ctx* const parser_ctx, const long long arg, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %lld", arg);
	*res = arg * arg;
	SM_SET_CALC_RES_OK;
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns '2' raised to the power of arg;

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - float value (long double);
	result - float value (long double);

	i.e. the function calculates:
		*res = 2 ** arg
	or
		pow(2, arg )

	the function is called, when user uses function 'pow_2^' in expression(s)
*/
errno_t	sm_calc_power_2_d(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)		// 2^d
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	*res = powl(2.0L, arg);
	SM_SET_CALC_RES_OK;
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns '10' raised to the power of arg;

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - float value (long double);
	result - float value (long double);

	i.e. the function calculates:
		*res = 10 ** arg
	or
		pow(10, arg )

	the function is called, when user uses function 'pow_10^' in expression(s)
*/
errno_t	sm_calc_power_10_d(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)		// 10^d
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	*res = powl(10.0L, arg);
	SM_SET_CALC_RES_OK;
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns arg raised to the power of '2';

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - float value (long double);
	result - float value (long double);

	i.e. the function calculates:
		*res = arg ** 2
	or
		*res = arg * arg

	the function is called, when user uses function 'pow_^2' in expression(s)
*/
errno_t sm_calc_power_d_2(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)		// d^2
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	*res = powl(arg, 2.0L);
	SM_SET_CALC_RES_OK;
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns inverse square root of argument;

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - float value (long double);
	result - float value (long double);

	the function is called, when user uses function 'sqrt' in expression(s)
*/
errno_t sm_calc_sqrt(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	if (arg >= 0) {
		*res = sqrtl(arg);
		SM_SET_CALC_RES_OK;
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of function 'sqrt': arg should be >= 0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns the natural logarithm (base of the exp) of argument (float long double);

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - float value (long double); arg should be > 0;
	result - float value (long double);

	the function is called, when user uses function 'ln' in expression(s)
*/
errno_t sm_calc_ln(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	if (arg > 0) {
		*res = logl(arg);
		SM_SET_CALC_RES_OK;
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of function 'ln': arg should be > 0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns the common logarithm (base of the 10) of argument (float long double);

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - float value (long double); arg should be > 0;
	result - float value (long double);

	the function is called, when user uses function 'ln' in expression(s)
*/
errno_t sm_calc_lg(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	if (arg > 0) {
		*res = log10l(arg);
		SM_SET_CALC_RES_OK;
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of function 'lg': arg should be > 0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns the logarithm (base, defied as parameter) of argument (float long double);

	in args:
		parser_ctx		- current parser context
		base			- base of the logarithm
		arg				- argument
	out args:
		res				- result

	where base - float value (long double); base should be > 0;
	arg - float value (long double); arg should be > 0;
	result - float value (long double);

	the function is called, when user uses function 'log' in expression(s)
*/
errno_t sm_calc_log(sm_parser_ctx* const parser_ctx, const long double base, const long double arg, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: base = %.10Lf arg = %.10Lf", base, arg);
	if (base <= 0) {
		SM_SET_CALC_RES_ERROR("wrong parameter of function 'log': base should be > 0");
		err = SM_RES_ERROR;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return err;
	}
	if (arg == 1) {
		*res = 0.0L;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
		err = SM_RES_OK;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return err;
	}
	if (arg > 0 && arg != 1) {
		*res = logl(arg) / logl(base);
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
		err = SM_RES_OK;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return err;
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of function 'log': arg should be > 0 && arg should be != 1 && arg should be > 0");
		err = SM_RES_ERROR;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
		return err;
	}
}

/*
	function retuns integer (long long) result of sum of integer (long long) arguments;

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	the function is called, when user uses operator '+' in expression(s)
*/
errno_t	sm_calc_add_i(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	*res = arg1 + arg2;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	function retuns integer (long long) result of subtraction of integer (long long) arguments;

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	the function is called, when user uses operator '-' in expression(s)
*/
errno_t	sm_calc_sub_i(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	*res = arg1 - arg2;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	function retuns integer (long long) result of multiplication of integer (long long) arguments;

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	the function is called, when user uses operator '*' in expression(s)
*/
errno_t	sm_calc_mul_i(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	*res = arg1 * arg2;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	function retuns float (long double) result of sum of float (long double) arguments;

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	the function is called, when user uses operator '+' in expression(s)
*/
errno_t	sm_calc_add_f(sm_parser_ctx* const parser_ctx, const long double arg1, const long double arg2, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %.10Lf arg2 = %.10Lf", arg1, arg2);
	*res = arg1 + arg2;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	function retuns float (long double) result of subtraction of float (long double) arguments;

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	the function is called, when user uses operator '-' in expression(s)
*/
errno_t	sm_calc_sub_f(sm_parser_ctx* const parser_ctx, const long double arg1, const long double arg2, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %.10Lf arg2 = %.10Lf", arg1, arg2);
	*res = arg1 - arg2;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	function retuns float (long double) result of multiplication of float (long double) arguments;

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	the function is called, when user uses operator '*' in expression(s)
*/
errno_t	sm_calc_mul_f(sm_parser_ctx* const parser_ctx, const long double arg1, const long double arg2, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %.10Lf arg2 = %.10Lf", arg1, arg2);
	*res = arg1 * arg2;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	function retuns float (long double) result of division of float (long double) arguments;

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	the function is called, when user uses operator '/' in expression(s)
*/
errno_t	sm_calc_div(sm_parser_ctx* const parser_ctx, const long double arg1, const long double arg2, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %.10Lf arg2 = %.10Lf", arg1, arg2);
	if (arg2 != 0.0) {
		*res = arg1 / arg2;
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of operator '/': arg2 should be != 0.0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns inverse value (1/x) of argument;

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - float value (long double);
	result - float value (long double);

	the function is called, when user uses function 'inv' in expression(s)
*/
errno_t	sm_calc_recip(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	if (arg != 0.0) {
		*res = 1.0L / arg;
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of function 'inv': arg should be != 0.0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns left arithmetic shift of argument;

	in args:
		parser_ctx		- current parser context
		arg1			- argument
		arg2			- number bits to shift
	out args:
		res				- result

	where arg1 - integer value (long long), arg2 - integer value (long long);
	arg2 should be >= 0;
	result - integer value (long long);

	the function saves sign of negative value during shift;

	the function is called, when user uses operator '<a<' in expression(s)
*/
errno_t	sm_calc_l_ashift(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	if (arg2 >= 0) {
		*res = arg1 << arg2;
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of operator '<a<': arg2 should be >= 0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns right arithmetic shift of argument;

	in args:
		parser_ctx		- current parser context
		arg1			- argument
		arg2			- number bits to shift
	out args:
		res				- result

	where arg1 - integer value (long long), arg2 - integer value (long long);
	arg2 should be >= 0;
	result - integer value (long long);

	the function is called, when user uses operator '>a>' in expression(s)
*/
errno_t	sm_calc_r_ashift(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	if (arg2 >= 0) {
		*res = arg1 >> arg2;
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of operator '>a>': arg2 should be >= 0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns left logical shift of argument;

	in args:
		parser_ctx		- current parser context
		arg1			- argument
		arg2			- number bits to shift
	out args:
		res				- result

	where arg1 - integer value (long long), arg2 - integer value (long long);
	arg2 should be >= 0;
	result - integer value (long long);

	the function is called, when user uses operator '>l>' in expression(s)
*/
errno_t	sm_calc_l_lshift(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	if (arg2 >= 0) {
		sm_bin bin;
		err = sm_conv_i_dec_2_bin(parser_ctx, arg1, &bin);
		if (err == SM_RES_OK) {
			for (long long j = 0; j < arg2; j++) {
				for (long long i = (long long)(bin.m_len - 2); i >= 0; i--)
					bin.m_data[i + 1] = bin.m_data[i];
				bin.m_data[0] = 0;
			}
			err = sm_conv_bin_2_i_dec(parser_ctx, &bin, res);
		}
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of operator '<l<': arg2 should be >= 0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns right logical shift of argument;

	in args:
		parser_ctx		- current parser context
		arg1			- argument
		arg2			- number bits to shift
	out args:
		res				- result

	where arg1 - integer value (long long), arg2 - integer value (long long);
	arg2 should be >= 0;
	result - integer value (long long);

	the function doesn't save sign of negative value during shift;

	the function is called, when user uses operator '>l>' in expression(s)
*/
errno_t	sm_calc_r_lshift(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	if (arg2 >= 0) {
		sm_bin bin;
		err = sm_conv_i_dec_2_bin(parser_ctx, arg1, &bin);
		if (err == SM_RES_OK) {
			for (long long j = 0; j < arg2; j++) {
				for (long long i = 0; i < (long long)(bin.m_len - 1); i++)
					bin.m_data[i] = bin.m_data[i + 1];
				bin.m_data[bin.m_len - 1] = 0;
			}
			err = sm_conv_bin_2_i_dec(parser_ctx, &bin, res);
		}
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of operator '<l<': arg2 should be >= 0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns left circular shift of argument;

	in args:
		parser_ctx		- current parser context
		arg1			- argument
		arg2			- number bits to shift
	out args:
		res				- result

	where arg1 - integer value (long long), arg2 - integer value (long long);
	arg2 should be >= 0;
	result - integer value (long long);

	for example:
		b10010010 == sm_calc_l_cshift(b1001001,1);
		b000100100100 == sm_calc_l_cshift(b1001001,2);
		b010010010000 == sm_calc_l_cshift(b1001001,4);
		b1111111111111110000000000000000000000000000000000000000000000001 == sm_calc_l_cshift(0xffff000000000000,1);
		b1111111111111100000000000000000000000000000000000000000000000011 == sm_calc_l_cshift(0xffff000000000000,2);
		b1111111111111000000000000000000000000000000000000000000000000111 == sm_calc_l_cshift(0xffff000000000000,3);
		b1111111111110000000000000000000000000000000000000000000000001111 == sm_calc_l_cshift(0xffff000000000000,4);

	the function is called, when user uses operator '>c>' in expression(s)
*/
errno_t	sm_calc_l_cshift(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	if (arg2 >= 0) {
		sm_bin bin;
		err = sm_conv_i_dec_2_bin(parser_ctx, arg1, &bin);
		if (err == SM_RES_OK) {
			for (long long j = 0; j < arg2; j++) {
				unsigned char last_bit = bin.m_data[bin.m_len - 1];
				for (long long i = (long long)(bin.m_len - 2); i >= 0; i--)
					bin.m_data[i + 1] = bin.m_data[i];
				bin.m_data[0] = last_bit;
			}
			err = sm_conv_bin_2_i_dec(parser_ctx, &bin, res);
		}
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of operator '<l<': arg2 should be >= 0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns right circular shift of argument;

	in args:
		parser_ctx		- current parser context
		arg1			- argument
		arg2			- number bits to shift
	out args:
		res				- result

	where arg1 - integer value (long long), arg2 - integer value (long long);
	arg2 should be >= 0;
	result - integer value (long long);

	for example:
		b1000000000000000000000000000000000000000000000000000000000100100 == sm_calc_r_cshift(b1001001,1);
		b0100000000000000000000000000000000000000000000000000000000010010 == sm_calc_r_cshift(b1001001,2);
		b1001000000000000000000000000000000000000000000000000000000000100 == sm_calc_r_cshift(b1001001,4);
		b1000011111111111100000000000000000000000000000000111111111111111 == sm_calc_r_cshift(0x0fff00000000ffff,1);
		b1100001111111111110000000000000000000000000000000011111111111111 == sm_calc_r_cshift(0x0fff00000000ffff,2);
		b1111000011111111111100000000000000000000000000000000111111111111 == sm_calc_r_cshift(0x0fff00000000ffff,4);

	the function is called, when user uses operator '<c<' in expression(s)
*/
errno_t	sm_calc_r_cshift(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	if (arg2 >= 0) {
		sm_bin bin;
		err = sm_conv_i_dec_2_bin(parser_ctx, arg1, &bin);
		if (err == SM_RES_OK) {
			for (long long j = 0; j < arg2; j++) {
				unsigned char first_bit = bin.m_data[0];
				for (long long i = 0; i < (long long)(bin.m_len - 1); i++)
					bin.m_data[i] = bin.m_data[i + 1];
				bin.m_data[bin.m_len - 1] = first_bit;
			}
			err = sm_conv_bin_2_i_dec(parser_ctx, &bin, res);
		}
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of operator '<l<': arg2 should be >= 0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return SM_RES_OK;
}

/*
	the function, that calculates and returns absolute value of argument;

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - integer value (long long);
	result - integer value (long long);

	the function is called, when user uses function 'abs' in expression(s), but argument
	of 'abs' function is a integer value, defined by lexical analyzer
*/
errno_t sm_calc_abs_i(sm_parser_ctx* const parser_ctx, const long long arg, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %lld", arg);
	*res = llabs(arg);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns absolute value of argument;

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - float value (long double);
	result - float value (long double);

	the function is called, when user uses function 'abs' in expression(s), but argument
	of 'abs' function is a float value, defined by lexical analyzer
*/
errno_t sm_calc_abs_f(sm_parser_ctx* const parser_ctx, const long double arg, long double* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %.10Lf", arg);
	*res = fabsl(arg);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that calculates and returns remainder value of division arg1 / arg2
	(often marked as arg1 % arg2);

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	where arg1 - integer value (long long), arg2 - integer value (long long);
	result - integer value (long long);

	i.e. the function calculates:
		*res = arg1 % arg2

	the function is called, when user uses functions 'mod' and 'mod2' in expression(s)
*/
errno_t sm_calc_mod(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	if (arg2) {
		*res = llabs(arg1 % arg2);
		err = SM_RES_OK;
	}
	else {
		SM_SET_CALC_RES_ERROR("wrong parameter of function 'mod': arg2 should be != 0");
		err = SM_RES_ERROR;
	}
	if (err == SM_RES_OK)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function, that returns pseudo-random value [0,1];

	in args:
		parser_ctx		- current parser context
	out args:
		res				- result

	the function is called, when user uses command 'rand[;]' in expression(s)
*/
errno_t	sm_get_rand(sm_parser_ctx* const parser_ctx, long double* const res)
{
	errno_t err;
	*res = (long double)rand() / (long double)RAND_MAX;
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %.10Lf", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function (bitwise function), that calculates bitwise multiplication of arguments ( arg1 && arg2 );

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	where arg1 - integer value (long long), arg2 - integer value (long long);
	result - integer value (long long);

	the function is called, when user uses function 'and' in expression(s)
*/
errno_t	sm_calc_and(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	*res = arg1 & arg2;
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function (bitwise function), that calculates bitwise addition of arguments ( arg1 && arg2 );

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	where arg1 - integer value (long long), arg2 - integer value (long long);
	result - integer value (long long);

	the function is called, when user uses function 'or' in expression(s)
*/
errno_t	sm_calc_or(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	*res = arg1 | arg2;
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function (bitwise function), that calculates bitwise negation of argument ( !arg );

	in args:
		parser_ctx		- current parser context
		arg				- argument
	out args:
		res				- result

	where arg - integer value (long long);
	result - integer value (long long);

	the function is called, when user uses function 'not' in expression(s)
*/
errno_t	sm_calc_not(sm_parser_ctx* const parser_ctx, const long long arg, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg = %lld", arg);
	*res = ~arg;
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function (bitwise function), that calculates bitwise negation of bitwise multiplication of arguments ( !(arg1 && arg2) );

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	where arg1 - integer value (long long), arg2 - integer value (long long);
	result - integer value (long long);

	the function is called, when user uses function 'nand' in expression(s)
*/
errno_t	sm_calc_nand(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	*res = ~(arg1 & arg2);
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function (bitwise function), that calculates bitwise negation of bitwise addition
	of arguments ( !(arg1 || arg2) );

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	where arg1 - integer value (long long), arg2 - integer value (long long);
	result - integer value (long long);

	the function is called, when user uses function 'nor' in expression(s)
*/
errno_t	sm_calc_nor(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	*res = ~(arg1 | arg2);
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	the function (bitwise function), that calculates bitwise 'exclusive or' or 'exclusive disjunction'
	of arguments ( arg1 ^ arg2) );

	in args:
		parser_ctx		- current parser context
		arg1			- first argument
		arg2			- second argument
	out args:
		res				- result

	where arg1 - integer value (long long), arg2 - integer value (long long);
	result - integer value (long long);

	the function is called, when user uses function 'xor' in expression(s)
*/
errno_t	sm_calc_xor(sm_parser_ctx* const parser_ctx, const long long arg1, const long long arg2, long long* const res)
{
	errno_t err;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in: arg1 = %lld arg2 = %lld", arg1, arg2);
	*res = arg1 ^ arg2;
	err = SM_RES_OK;
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out: res = %lld", *res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "result = %d", err);
	return err;
}

/*
	callback function (for the parser);

	in args:
		parser_ctx		- current parser context
		param			- argument
	out args:
		res				- result

	the function can be used for returning the result from the parser;
	this function just defined, but is not called in this implementing;
*/
int parser_callback(sm_parser_ctx* const parser_ctx, void* arg)
{
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "parser_callback...");
	return 0;
}
