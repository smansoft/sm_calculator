/*
 *    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
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

#include "sm_calc_types.h"
#include "sm_calc.h"
#include "sm_calc_proc.h"

 /* If this is being built for a unit test. */
#ifdef UNIT_TESTING
int sm_calc_main(int argc, char* argv[]);
/* main is defined in the unit test so redefine name of the the main function
 * here. */
#define main sm_calc_main
#endif /* UNIT_TESTING */

#define NAME				"sm_calc"
#define VERSION				"0.0.1.1"
#define COPYRIGHT			"Copyright (c) 2020 SManSoft"
#define PRODUCT_NAME		"SM Calculator"

#define SM_LOG_DPATH		"."					//	default directory path, where log file will be created (current directory)
#define SM_LOG_FNAME		"sm_calc.log"		//	log file name

extern FILE* yyin;		//	stream file, that is used by lexical analyzer for reading of parsed content
extern FILE* yyout;		//	stream file, that is used by lexical analyzer for output of generated ontent

sm_log_config gsm_log_config = { 0 };	//	global instance of main log support structure
#define SM_LOG_CONFIG &gsm_log_config	//	just synonym: SM_LOG_CONFIG == &gsm_log_config - for usage in log api calls

const char gcsm_calc_short_options[] = "f:o:x:p:n:a:ih";	//	short options string array for getopt_lib library api

/*	options array for getopt_lib library api	*/
const struct option_a gcsm_calc_options[] = {
	{(char*)"in_file",1,0,'f'},
	{(char*)"out_file",1,0,'o'},
	{(char*)"expression",1,0,'x'},
	{(char*)"f_precision",1,0,'p'},
	{(char*)"i_format",1,0,'n'},
	{(char*)"a_unit",1,0,'a'},
	{(char*)"interactive",0,0,'i'},
	{(char*)"help",0,0,'h'},
	{0,0,0,0}
};

int				gsm_exit = 0;				//	flag (if gsm_exit != 0), that precessing cycle:
											//	taking expression(s) :
											//	In >>
											//	, printing result(s) :
											//	Out >>
											//	should be finished and application should exit

sm_calc_params	gsm_calc_params = { 0 };	//	global instance of definition the current state of sm_calculator configuration
sm_calc_res		gsm_calc_res = { 0 };		//	global instance of definition the current result of calculations

/*
	array of long double constants, defined in
	enum _sm_const_idx
	sm_const_idx
*/
const long double gcsm_consts[] =
{
		3.14159265358979323846L,		//	SM_CONST_PI
		2.71828182845904523536L,		//	SM_CONST_EXP
		0.57721566490153286060L,		//	SM_CONST_GAMMA
		1.61803398874989484820L,		//	SM_CONST_PHI

		0.015707963267948966192313216916L,		//	SM_CONST_GRAD_RAD: for converting Gradians -> Radians
		63.661977236758134307553505348999L,		//	SM_CONST_RAD_GRAD: for converting Radians -> Gradians

		57.295779513082320876798154814105L,		//	SM_CONST_RAD_DEG: for converting Radians -> Degree
		0.017453292519943295769236907685L,		//	SM_CONST_DEG_RAD: for converting Degree -> Radians

		0.9L,									//	SM_CONST_GRAD_DEG: for converting Gradians -> Degree
		1.111111111111111111111111111111L		//	SM_CONST_DEG_GRAD: for converting Degree -> Gradians
};

/* 
	here is suite of char arrays, which are used for data/results/modes/configuration reading(scanf) and output;
	some of them also are used for parsing values of command line options
*/
const char* const	gcsm_trig_units[] = { "rad", "deg", "grad" };			//	string description of unit for measuring of angles (radians, degrees, gradians) 
const char* const	gcsm_exp_precision = "exp";

const char* const	gcsm_f_scanf_formats = "%Lg";							//	scan format of float value (same for any output precision)
char				gsm_f_print_formats[][SM_S_BUFF_SIZE] = { "%Le", "" };	//	current precision of float value output formats
																			//	gsm_f_print_formats[0] is used, when current precision value == 'exp' (SM_PREC_EXP)
																			//	gsm_f_print_formats[1] is used, when current precision value == [0,20]
																			//	gsm_f_print_formats[1] contains current print format string %.[0,20]Lf"

const char* const	gcsm_i_formats[] = { "dec", "oct", "hex", "bin" };		//	string description of output formats of integers (decimal, octal, hexadecimal, binary formats)

const char* const	gcsm_i_scanf_formats[] = { "%lld", "%llo", "%llx", "%s" };			//	scan formats (entering formats) of integers (decimal, octal, hexadecimal, binary formats)
const char* const	gcsm_i_printf_formats[] = { "%lld", "0%llo", "0x%llx", "b%s" };		//	print formats (output formats) of integers (decimal, octal, hexadecimal, binary formats)

const char* const	gcsm_s_format = "%s;";
const char* const	gcsm_nl = "\n";

/*	main entry point */
int main(int argc, char* argv[])
{
#ifndef UNIT_TESTING
	yyin = stdin;
	yyout = stdout;
#endif /* UNIT_TESTING */

	int ind = 0;
	int opt_val;
	errno_t	err_res = SM_RES_OK;

#ifndef UNIT_TESTING
	sm_init_log();

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s", __FUNCTION__,"---------------------------------------------------------------------- >>");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc is started");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");
#endif /* UNIT_TESTING */

	sm_calc_show_version();

	sm_init_random();
	sm_set_def_calc_params();

	//	cycle of processing of command line options, using getopt_lib
	while (TRUE)
	{
		opt_val = getopt_long_a(argc, argv, gcsm_calc_short_options, gcsm_calc_options, &ind);
		if (opt_val < 0)
			break;
		switch (opt_val) {
		case 'h': {
			sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "opt_val == 'h' ('help')");
			gsm_calc_params.m_help = TRUE;
			break;
		}
		case 'i': {
			sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "opt_val == 'i' ('interactive')");
			gsm_calc_params.m_interact_proc = TRUE;
			break;
		}
		case 'f': {
			sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "opt_val == 'f' ('in_file')");
			err_res = safe_strcpy(gsm_calc_params.m_in_file_path, SM_ARRAY_SIZE(gsm_calc_params.m_in_file_path), optarg);
			if (err_res == SM_RES_ERROR)
				sm_print_error("wrong value of option '-x --in_file'");
			break;
		}
		case 'o': {
			sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "opt_val == 'o' ('out_file')");
			err_res = safe_strcpy(gsm_calc_params.m_out_file_path, SM_ARRAY_SIZE(gsm_calc_params.m_out_file_path), optarg);
			if (err_res == SM_RES_ERROR)
				sm_print_error("wrong value of option '-o --out_file'");
			break;
		}
		case 'x': {
			sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "opt_val == 'x' ('expression')");
			size_t len;
			if (!(len = safe_strnlen(optarg, SM_L_BUFF_SIZE - 1))) {
				sm_print_error("wrong value of option '-x --expr'");
				err_res = SM_RES_ERROR;
				continue;
			}
			err_res = safe_strcpy(gsm_calc_params.m_expression, SM_ARRAY_SIZE(gsm_calc_params.m_expression) - 1, optarg);
			len = safe_strnlen(gsm_calc_params.m_expression, SM_ARRAY_SIZE(gsm_calc_params.m_expression) - 1);
			if (len) {
				if (gsm_calc_params.m_expression[len - 1] != ';')
					safe_strcat(gsm_calc_params.m_expression, SM_L_BUFF_SIZE, ";");
			}
			break;
		}
		case 'p': {
			sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "opt_val == 'p' ('f_precision')");
			int cmp_res = -1;
			size_t len = safe_strnlen(gcsm_exp_precision, SM_L_BUFF_SIZE);
			if (len > 0)
				err_res = safe_strcasecmp(gcsm_exp_precision, len, optarg, &cmp_res);
			if (err_res == SM_RES_OK && !cmp_res) {
				gsm_calc_params.m_f_precision = SM_PREC_EXP;
			}
			else {
				size_t len = safe_strnlen(optarg, 2);
				if (len > 0 && len <= 2) {
					for (unsigned int i = 0; i < len; i++)
						if (!isdigit(optarg[i])) {
							err_res = 1;
							break;
						}
					if (err_res == SM_RES_OK) {
						int precision = atoi(optarg);
						if (SM_PREC_MIN <= precision && precision <= SM_PREC_MAX)
							gsm_calc_params.m_f_precision = precision;
						else
							sm_print_error("wrong value of option '-p --precision'");
					}
					else
						sm_print_error("wrong value of option '-p --precision'");
				}
				else
					sm_print_error("wrong value of option '-p --precision'");
			}
			break;
		}
		case 'n': {
			sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "opt_val == 'n' ('i_format')");
			int cmp_res = -1;
			size_t len;
			for (int i = SM_I_FORMAT_MIN; i <= SM_I_FORMAT_MAX; i++) {
				cmp_res = -1;
				len = safe_strnlen(gcsm_i_formats[i], SM_L_BUFF_SIZE);
				if (len > 0)
					err_res = safe_strcasecmp(gcsm_i_formats[i], len, optarg, &cmp_res);
				if (err_res == SM_RES_OK && !cmp_res) {
					gsm_calc_params.m_i_format = i;
					break;
				}
			}
			if (cmp_res)
				sm_print_error("wrong value of option '-n --nformat'");
			break;
		}
		case 'a': {
			sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "opt_val == 'a' ('a_unit')");
			int cmp_res = -1;
			gsm_calc_params.m_trig_unit = SM_TRIG_RADIANS;
			for (int i = SM_TRIG_UNIT_MIN; i <= SM_TRIG_UNIT_MAX; i++) {
				size_t len = safe_strnlen(gcsm_trig_units[i], SM_L_BUFF_SIZE);
				cmp_res = -1;
				if (len > 0) {
					err_res = safe_strcasecmp(gcsm_trig_units[i], len, optarg, &cmp_res);
					if (err_res == SM_RES_OK && !cmp_res) {
						gsm_calc_params.m_trig_unit = (sm_trig_unit)i;
						break;
					}
				}
			}
			if (cmp_res) {
				sm_print_error("wrong value of option '-a --aunit'");
				err_res = SM_RES_ERROR;
			}
			break;
		}
		default: {
			sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "opt_val == default value");
			gsm_calc_params.m_help = TRUE;
			break;
		}
		};
	}
	while (TRUE) {
		if (err_res != SM_RES_OK)
			break;
		err_res = sm_init_f_prec_buf();
		if (err_res != SM_RES_OK)
			break;
		err_res = sm_proc_params();
		if (err_res != SM_RES_OK)
			break;
		err_res = sm_proc_calc();
		break;
	}

#ifndef UNIT_TESTING
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "------------------");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_calc is finished");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "main ---------------------------------------------------------------------- <<");

	sm_close_log();
#endif /* UNIT_TESTING */

	return err_res;
}

/*	initializing and starting the log output	*/
errno_t	sm_init_log()
{
	errno_t err = SM_RES_OK;
	err = sm_log_init(SM_LOG_CONFIG, SM_LOG_DPATH, SM_LOG_FNAME);
	if (err == SM_RES_OK)
		err = sm_log_start(SM_LOG_CONFIG);
	return err;
}

/*	stopping and closing the log output	*/
errno_t	sm_close_log()
{
	sm_log_stop(SM_LOG_CONFIG);
	return sm_log_close(SM_LOG_CONFIG);
}

/*
	provides log out (in log file) the sm_calc_params gsm_calc_params,
	current state of sm_calculator configuration
*/
errno_t	sm_log_calc_params()
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "calc_params --------------- >>");

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_interact_proc = %d", gsm_calc_params.m_interact_proc);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_cmd_line_proc = %d", gsm_calc_params.m_cmd_line_proc);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_file_proc     = %d", gsm_calc_params.m_file_proc);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_help          = %d", gsm_calc_params.m_help);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_in_file_path = %s",
		gsm_calc_params.m_in_file_path ? gsm_calc_params.m_in_file_path : "NULL");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_out_file_path = %s",
		gsm_calc_params.m_out_file_path ? gsm_calc_params.m_out_file_path : "NULL");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_expression = %s",
		gsm_calc_params.m_expression ? gsm_calc_params.m_expression : "NULL");

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_in_file  = 0x%x", gsm_calc_params.m_in_file);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_out_file = 0x%x", gsm_calc_params.m_out_file);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_trig_unit   = %d", gsm_calc_params.m_trig_unit);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_f_precision = %d", gsm_calc_params.m_f_precision);

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_i_format = %d", gsm_calc_params.m_i_format);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "calc_params --------------- <<"); \

		return SM_RES_OK;
}

/*
	provides log out (in log file) the sm_calc_res gsm_calc_res,
	current result of calculations
*/
errno_t	sm_log_calc_res()
{
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "calc_res --------------- >>");

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_res.m_res        = %d", gsm_calc_res.m_res);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_res.m_sz_message = %s", gsm_calc_res.m_sz_message);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "calc_res --------------- <<");

	return SM_RES_OK;
}

/*
	provides log out (in log file) sm_bin object
	(structure, that incapsulates integer value storaging in binary format):

	args:
		prfx	- message, that will be pronted before sm_bin object
		bin		- sm_bin object, that should be log out
*/
errno_t	sm_log_bin(const char* const prfx, const sm_bin* const bin)
{
	if (!bin)
		return SM_RES_ERROR;
	char buf[SM_S_BUFF_SIZE + 1];
	safe_memset(buf, SM_ARRAY_SIZE(buf), 0);
	for (unsigned int i = 0; i < bin->m_len; i++) {
		if (bin->m_data[i] == 0)
			buf[i] = '0';
		else if (bin->m_data[i] == 1)
			buf[i] = '1';
		else
			buf[i] = '\0';
	}
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_bin --------------- >>");
	if (prfx)
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "sm_bin prfx: %s", prfx);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "sm_bin: m_len  = %d", bin->m_len);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "sm_bin: m_data = %s", buf);
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "sm_bin --------------- <<");
	return SM_RES_OK;
}

/*	initializing the global sm_calc_params object	*/
void sm_set_def_calc_params()
{
	safe_memset(&gsm_calc_params, sizeof(gsm_calc_params), 0);

	gsm_calc_params.m_f_precision = SM_PREC_DEF;
	gsm_calc_params.m_i_format = SM_I_FORMAT_DEF;
	gsm_calc_params.m_trig_unit = SM_TRIG_UNIT_DEF;

	gsm_calc_params.m_interact_proc = FALSE;
	gsm_calc_params.m_cmd_line_proc = FALSE;
	gsm_calc_params.m_file_proc = FALSE;

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "default parameters have been initialized");

	return;
}

/*	initializing the random seed (for 'rand' command)	*/
void sm_init_random()
{
	srand((unsigned int)time(NULL));
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "random starting point has been initialized");
	return;
}

/*
	provides additional initializing the sm_calc_params gsm_calc_params,
	current state of sm_calculator configuration,
	based on current partialy-known fields of sm_calc_params gsm_calc_params
*/
errno_t	sm_proc_params()
{
	errno_t err = SM_RES_OK;
	size_t len;
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "processing of parameters has been started");
	// opening of files for processing
	while (1) {
		len = safe_strnlen(gsm_calc_params.m_in_file_path, MAX_PATH);
		if (!len) {
			sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "length of gsm_calc_params.m_in_file_path == 0");
			break;
		}
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "length of gsm_calc_params.m_in_file_path > 0");
		err = safe_fopen(&gsm_calc_params.m_in_file, gsm_calc_params.m_in_file_path, "r", SM_SFOPEN_NSHARED_TYPE);
		if (err == SAFE_RES_ERROR) {
			err = SM_RES_ERROR;
			char err_buf[SM_L_BUFF_SIZE];
			snprintf(err_buf, SM_ARRAY_SIZE(err_buf), "input file: %s hasn't been opened for reading", gsm_calc_params.m_in_file_path);
			sm_print_error(err_buf);
			break;
		}
		else
			sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "input file: %s has been opened for reading", gsm_calc_params.m_in_file_path);
		len = safe_strnlen(gsm_calc_params.m_out_file_path, MAX_PATH);
		if (!len) {
			sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "length of gsm_calc_params.m_out_file_path == 0");
			break;
		}
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "length of gsm_calc_params.m_out_file_path > 0");
		err = safe_fopen(&gsm_calc_params.m_out_file, gsm_calc_params.m_out_file_path, "w", SM_SFOPEN_NSHARED_TYPE);
		if (err == SAFE_RES_ERROR) {
			err = SM_RES_ERROR;
			char err_buf[SM_L_BUFF_SIZE];
			snprintf(err_buf, SM_ARRAY_SIZE(err_buf), "output file: %s hasn't been opened for writing", gsm_calc_params.m_out_file_path);
			sm_print_error(err_buf);
			break;
		}
		else
			sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "input file: %s has been opened for writing", gsm_calc_params.m_in_file_path);
		break;
	}
	if (err == SM_RES_OK && gsm_calc_params.m_in_file)
		gsm_calc_params.m_file_proc = (gsm_calc_params.m_in_file != NULL);
	else
		gsm_calc_params.m_file_proc = FALSE;

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_file_proc = %d", gsm_calc_params.m_file_proc);

	err = SM_RES_OK;

	len = safe_strnlen(gsm_calc_params.m_expression, SM_L_BUFF_SIZE);
	gsm_calc_params.m_cmd_line_proc = len > 0;

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "length of gsm_calc_params.m_expression > 0");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_cmd_line_proc = %d", gsm_calc_params.m_cmd_line_proc);

	if (!gsm_calc_params.m_interact_proc)
		gsm_calc_params.m_interact_proc = !gsm_calc_params.m_file_proc && !gsm_calc_params.m_cmd_line_proc;

	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_interact_proc = %d", gsm_calc_params.m_interact_proc);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Result = %d", err);

	sm_log_calc_params();

	return err;
}

/*
	initializing the m_f_prec_buf (field of the sm_calc_params gsm_calc_params)
	after changing of m_f_precision;
	i.e. if m_f_precision has been updated, current format string
	(depends on current precision of float value output) should be updated;
	i.e.
		if m_f_precision == 10 -> gsm_f_print_formats[1]="%.10Lf" and m_f_prec_buf == gsm_f_print_formats[1]
		if m_f_precision == 20 -> gsm_f_print_formats[1]="%.20Lf" and m_f_prec_buf == gsm_f_print_formats[1]
		if m_f_precision == -1 ('exp') -> gsm_f_print_formats[0]="%Le" and m_f_prec_buf == gsm_f_print_formats[0]
*/
errno_t sm_init_f_prec_buf()
{
	errno_t err = SM_RES_OK;
	if (gsm_calc_params.m_f_precision == SM_PREC_EXP) {
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_f_precision == SM_PREC_EXP");
		gsm_calc_params.m_f_prec_buf = gsm_f_print_formats[0];
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_f_prec_buf = %s", gsm_calc_params.m_f_prec_buf);
	}
	else {
		int prn_res = snprintf(gsm_f_print_formats[1], SM_ARRAY_SIZE(gsm_f_print_formats[1]),
			"%c.%dLf", '%', gsm_calc_params.m_f_precision);
		if (prn_res > 0) {
			gsm_calc_params.m_f_prec_buf = gsm_f_print_formats[1];
			sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "gsm_calc_params.m_f_prec_buf = %s", gsm_calc_params.m_f_prec_buf);
			err = SM_RES_OK;
		}
		else
			err = SM_RES_ERROR;
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Result = %d", err);
	return err;
}

/*
	provides processing of sm_calc_params gsm_calc_params and according to initialized fields:
		m_interact_proc
		m_cmd_line_proc
		m_file_proc
		m_help
	this function calls:
		sm_do_calc_interactive
		sm_do_calc_expression
		sm_do_calc_files
		sm_calc_usage
*/
errno_t sm_proc_calc()
{
	errno_t err = SM_RES_OK;
	if (gsm_calc_params.m_help) {
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "Help (sm_calc_usage):");
		sm_calc_usage();
		return err;
	}
	if (gsm_calc_params.m_file_proc) {
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "Files processing is started");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "in file  = %s", gsm_calc_params.m_in_file_path);
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "out file = %s", gsm_calc_params.m_out_file_path);
		printf("\tFiles processing:\n");
		printf("in file = %s\n", gsm_calc_params.m_in_file_path);
		printf("out file = %s\n", gsm_calc_params.m_out_file_path);
		err = sm_do_calc_files(gsm_calc_params.m_in_file, gsm_calc_params.m_out_file);
		printf("\n");
		if (gsm_calc_params.m_in_file)
			fclose(gsm_calc_params.m_in_file);
		if (gsm_calc_params.m_out_file)
			fclose(gsm_calc_params.m_out_file);
		gsm_calc_params.m_in_file = NULL;
		gsm_calc_params.m_out_file = NULL;
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Files processing: Result = %s", (err == SM_RES_OK) ? "Ok" : "Error");
	}
	if (gsm_calc_params.m_cmd_line_proc) {
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "Command line processing is started");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "expression  = %s", gsm_calc_params.m_expression);
		printf("\tCommand line processing:\n");
		err = sm_do_calc_expression();
		printf("\n");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Command line processing: Result = %s", (err == SM_RES_OK) ? "Ok" : "Error");
	}
	if (gsm_calc_params.m_interact_proc) {
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "Interactive processing is started");
		printf("\tInteractive processing:\n");
		printf("Type 'help' for more information.\n");
		sm_do_calc_interactive();
		printf("\n");
	}
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Result = %d", err);
	return err;
}

/*
	provides interactive (using stdin, stdout) processing of input of user:
	taking expression(s):
		In  >>
	, printing result(s):
		Out >>
	and cycle of input/output of
*/
errno_t sm_do_calc_interactive()
{
	errno_t err = SM_RES_OK;
#ifndef UNIT_TESTING	//	if defined UNIT_TESTING, yyin and yyout can be defined for input/output from/to files
	yyin = stdin;
	yyout = stdout;
#endif /* UNIT_TESTING */
	char in_buf[SM_L_BUFF_SIZE];
	char* in_res;
	size_t in_len;
	int cycle = 0;
	BOOL out = FALSE;
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "Interactive processing");
	do {
		SM_SET_CALC_RES_OK;
		if (cycle > 0 && out)
			fprintf(yyout, "\n");
		out = FALSE;
		fprintf(yyout, "In  >> ");
		in_res = safe_gets(yyin, in_buf, SM_ARRAY_SIZE(in_buf) - 1);
		if (!in_res || !(in_len = safe_strnlen(in_buf, SM_L_BUFF_SIZE)))
			continue;
		if (in_buf[in_len - 1] != ';')
			safe_strcat(in_buf, SM_ARRAY_SIZE(in_buf), ";");
		fprintf(yyout, "Out >> ");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Interactive processing: cycle = %d", cycle);
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Interactive processing: in_buf (user input) = %s", in_buf);
		sm_scan_string(in_buf);
		err = yyparse();
		fprintf(yyout, "\n");
		sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Interactive processing: yyparse res = %d", err);
		out = TRUE;
		cycle++;
	} while (!gsm_exit);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Interactive processing: gsm_exit = %d", gsm_exit);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Interactive processing: Result = %d", err);
	return err;
}

/*
	provides processing m_expression (field of the sm_calc_params gsm_calc_params)
	and printing result
*/
errno_t sm_do_calc_expression()
{
	errno_t err = SM_RES_OK;
#ifndef UNIT_TESTING	//	if defined UNIT_TESTING, yyin and yyout can be defined for input/output from/to files
	yyin = stdin;
	yyout = stdout;
#endif /* UNIT_TESTING */
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "Expression processing");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Expression processing: gsm_calc_params.m_expression = %s", gsm_calc_params.m_expression);
	fprintf(yyout, "In  >> %s\nOut >> ", gsm_calc_params.m_expression);
	sm_scan_string(gsm_calc_params.m_expression);
	err = yyparse();	//	lexical analyzer
	fprintf(yyout, "\n");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Expression processing: yyparse res = %d", err);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Expression processing: Result = %d", err);
	return err;
}

/*
	provides processing in_file (input) file and output result in out_file
*/
errno_t sm_do_calc_files(FILE* const in_file, FILE* const out_file)
{
	errno_t err = SM_RES_OK;
	yyin = in_file;
	yyout = out_file;
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "Files processing");
	sm_scan_file(in_file);
	err = yyparse();	//	lexical analyzer
	fprintf(yyout, "\n");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Files processing: yyparse res = %d", err);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "Files processing: Result = %d", err);
	return err;
}

/*	printing the current version of sm_calculator	*/
void sm_calc_show_version()
{
	printf("%s %s %s\n%s\n\n", NAME, VERSION, COPYRIGHT, PRODUCT_NAME);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s %s %s", NAME, VERSION, COPYRIGHT);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "%s", PRODUCT_NAME);
	return;
}

/*	printing the command line help (-h,--help)	*/
void sm_calc_usage()
{
	char units_buf[SM_S_BUFF_SIZE];
	units_buf[0] = '\0';

	int res = snprintf(units_buf, SM_ARRAY_SIZE(units_buf), "%s|%s|%s",
		gcsm_trig_units[0],
		gcsm_trig_units[1],
		gcsm_trig_units[2]);
	if (res <= 0)
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "Error: Some problems with init of trig_units buffer");

	char i_formats_buf[SM_S_BUFF_SIZE];
	i_formats_buf[0] = '\0';

	res = snprintf(i_formats_buf, SM_ARRAY_SIZE(i_formats_buf), "%s|%s|%s|%s",
		gcsm_i_formats[0],
		gcsm_i_formats[1],
		gcsm_i_formats[2],
		gcsm_i_formats[3]);

	if (res <= 0)
		sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "Error: Some problems with init of trig_units buffer");

	printf("USAGE: sm_calc [OPTION]\n\n");
	printf("-h, --help                                             display this message\n");
	printf("-i, --interactive                                      force interactive mode\n");
	printf("-f<infile>, --in_file=<infile>                         name/path of input file\n");
	printf("-o<ofile>, --out_file=<ofile>                          name/path of output file\n");
	printf("-x<expression_value>, --expression=<expression_value>  expression, that should be solved\n");
	printf("-p<precision_value>, --f_precision=<precision_value>   floating-point precision (after '.' ): [%d-%d|%s]\n", SM_PREC_MIN, SM_PREC_MAX, gcsm_exp_precision);
	printf("                                                               by default, precision_value == '%d'\n", SM_PREC_DEF);
	printf("                                                               if 'precision_value' == '%s' - exponential output will be used\n", gcsm_exp_precision);
	printf("-n<i_format_value>, --i_format=<i_format_value>        output format of integers: [%s]\n", i_formats_buf);
	printf("                                                               by default, output format of integers == '%s'\n", gcsm_i_formats[SM_I_10]);
	printf("-a<unit_value>, --a_unit=<unit_value>                  default unit of measurement of an angle = '%s'\n\n", units_buf);

	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "USAGE: sm_calc [OPTION]");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "-h, --help                            display this message");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "-i, --interactive                     force interactive mode");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "-f<infile>, --in_file=<infile>        name/path of input file");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "-o<ofile>, --out_file=<ofile>         name/path of output file");
	sm_log_print(SM_LOG_CONFIG, __FUNCTION__, "-x<expression_value>, --expression=<expression_value>     expression, that should be solved");
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "-p<precision_value>, --f_precision=<precision_value>   floating-point precision (after '.' ): [%d-%d|%s]",
		SM_PREC_MIN, SM_PREC_MAX, gcsm_exp_precision);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "                                      by default, precision_value == '%d'", SM_PREC_DEF);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "                                      if 'precision_value' == '%s' - exponential output will be used", gcsm_exp_precision);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "-n<i_format_value>, --i_format=<i_format_value> output format of integers: [%s]", i_formats_buf);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "                                      by default, output format of integers == '%s'", gcsm_i_formats[SM_I_10]);
	sm_log_printf(SM_LOG_CONFIG, __FUNCTION__, "-a<unit_value>, --a_unit=<unit_value> default unit of measurement of an angle = '%s'", units_buf);

	return;
}
