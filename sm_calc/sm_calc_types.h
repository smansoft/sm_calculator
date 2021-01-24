/*
 *    Copyright (c) 2020-2021 SManSoft <http://www.smansoft.com/>
 *    Sergey Manoylo <info@smansoft.com>
 */

#pragma once

#include <stdio.h>

#if defined __cplusplus
extern "C" {
#endif

typedef int				BOOL;			//	type BOOL definition
typedef int				errno_t;		//	type errno_t (int) definitition
										//	errno_t - is a basic type, returned by most calc proc functions

#ifndef NULL
#define NULL			0
#endif

#ifndef VOID
#define VOID			void
#endif

#ifndef FALSE
#define FALSE			0
#endif

#ifndef TRUE
#define TRUE			1
#endif

#if !defined MAX_PATH
#define MAX_PATH    260
#endif

#if !defined SM_ARRAY_SIZE
#define SM_ARRAY_SIZE(in_array) (sizeof(in_array)/sizeof(in_array[0]))	//	calc size of the array
#endif

/*	large buffer definition	*/
#if !defined SM_L_BUFF_SIZE
#define SM_L_BUFF_SIZE	1024
#endif

/*	small buffer definition	*/
#if !defined SM_S_BUFF_SIZE
#define SM_S_BUFF_SIZE	64
#endif

/*	medium buffer definition	*/
#if !defined SM_M_BUFF_SIZE
#define SM_M_BUFF_SIZE	128
#endif

#ifndef SM_RES_OK
#define SM_RES_OK			0
#endif

#ifndef SM_RES_ERROR
#define SM_RES_ERROR		1
#endif

/*	definition of index of constants (for access to const long double gcsm_consts[] array)	*/
typedef enum _sm_const_idx
{
	SM_CONST_NO_TYPE = -1,

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
} sm_const_idx;

/*	...MIN and ...MAX - they are used in cycles	*/
#define SM_CONST_DATA_MIN	SM_CONST_PI
#define SM_CONST_DATA_MAX	SM_CONST_DEG_GRAD

/*	definition the unit for measuring of angles (radians, degrees, gradians)	*/
typedef enum _sm_trig_unit
{
	SM_TRIG_NO_UNIT = -1,

	SM_TRIG_RADIANS = 0,
	SM_TRIG_DEGREES,
	SM_TRIG_GRADIAN
} sm_trig_unit;

/*	...MIN and ...MAX - they are used in cycles	*/
#define SM_TRIG_UNIT_MIN	SM_TRIG_RADIANS
#define SM_TRIG_UNIT_MAX	SM_TRIG_GRADIAN

#define SM_TRIG_UNIT_DEF	SM_TRIG_UNIT_MIN	//	default value of definition the unit for measuring of angles
												//	(radians, degrees, gradians) - SM_TRIG_RADIANS

/* definition the entering and output formats of integers (decimal, octal, hexadecimal, binary formats)	*/
typedef enum _sm_i_format
{
	SM_I_NO_FORMAT = -1,

	SM_I_10 = 0,
	SM_I_8,
	SM_I_16,
	SM_I_2,
} sm_i_format;

/*	...MIN and ...MAX - they are used in cycles	*/
#define SM_I_FORMAT_MIN		SM_I_10				
#define SM_I_FORMAT_MAX		SM_I_2

#define SM_I_FORMAT_DEF		SM_I_FORMAT_MIN		//	default value of entering and output formats of integers
												//	(decimal, octal, hexadecimal, binary formats) - SM_I_FORMAT_MIN == SM_I_10 (decimal format)

/*
	definition the structure, that incapsulates integer value storaging in
	binary format;
	for example:
		original (decimal format) data == 10
		binary format data == 1010
		then
			m_data[0] = 0,
			m_data[1] = 1,
			m_data[2] = 0,
			m_data[3] = 1,
			m_data[4] = 0,
			...
			m_data[SM_S_BUFF_SIZE-2] = 0,
			m_data[SM_S_BUFF_SIZE-1] = 0;

	i.e. m_data[0...SM_S_BUFF_SIZE-1] array saves decimal 0 or decimal 1.
	Such format of data is useful for providing of some bitwise
	operations with original data;

	SM_S_BUFF_SIZE == 64, i.e. maximum size of integer value (long long)
*/
typedef struct _sm_bin
{
	unsigned char m_data[SM_S_BUFF_SIZE];	//	array, that saves decimal 0 or decimal 1 of binary format
	size_t m_len;							//	number cells of m_data array, which are filled (often m_len == SM_S_BUFF_SIZE)
} sm_bin, * psm_bin;

/*
	definition the current result of calculations
*/
typedef struct _sm_calc_res
{
	errno_t	m_res;							//	SM_RES_OK or SM_RES_ERROR
	char	m_sz_message[SM_L_BUFF_SIZE];	//	contains message if m_res == SM_RES_ERROR
											//	m_sz_message is printed if the result of last processing operation(m_res == SM_RES_ERROR)
} sm_calc_res, * psm_calc_res;

/*
	definition the current state of sm_calculator configuration
*/
typedef struct _sm_calc_params
{
	char m_log_file_path[MAX_PATH];			//	log file path (obtained via command line params: -o, --out_file)

	char m_in_file_path[MAX_PATH];			//	input file path (obtained via command line params: -f, --in_file )
	char m_out_file_path[MAX_PATH];			//	output file path (obtained via command line params: -o, --out_file)
	char m_expression[SM_L_BUFF_SIZE];		//	command line expression (-x, --expression)

	BOOL m_interact_proc;				//	TRUE if interactive mode is initialized
	BOOL m_cmd_line_proc;				//	TRUE if command line expression should be processed
	BOOL m_file_proc;					//	TRUE if file processing (input file path, [output file path]) is defined

	BOOL m_no_log;						//	TRUE if don't need to create/open and don't need to start log output

	BOOL m_help;						//	TRUE if sm_calc should show calc usage (sm_calc_usage()): -h, --help 

	int	m_f_precision;					//	current precision of float value output
	sm_i_format	m_i_format;				//	current output formats of integers (decimal, octal, hexadecimal, binary formats)

	sm_trig_unit m_trig_unit;			//	current unit for measuring of angles (radians, degrees, gradians)

	FILE* m_in_file;				//	stream of input file (if m_interact_proc == TRUE)
	FILE* m_out_file;				//	stream of output file (if m_interact_proc == TRUE)

	char* m_f_prec_buf;				//	current pointer to format string
									//	it depends on current precision value m_f_precision
									//	m_f_prec_buf contains address of gsm_f_print_formats[0] if current precision value == 'exp' (SM_PREC_EXP)
									//	m_f_prec_buf contains address of gsm_f_print_formats[1] if current precision value == [0,20]
									//	in this case format "%.[0,20]Lf" depends on m_f_precision value;

	char	m_f_print_formats[2][SM_S_BUFF_SIZE]; // = { "%Le", "" };		//	current precision of float value output formats
																			//	gsm_f_print_formats[0] is used, when current precision value == 'exp' (SM_PREC_EXP)
																			//	gsm_f_print_formats[1] is used, when current precision value == [0,20]
																			//	gsm_f_print_formats[1] contains current print format string %.[0,20]Lf"

	char	m_help_fpath[MAX_PATH];

	int		m_exit;					//	flag (if gsm_exit != 0), that precessing cycle:
									//	taking expression(s) :
									//	In >>
									//	, printing result(s) :
									//	Out >>
									//	should be finished and application should exit
} sm_calc_params, * psm_calc_params;

/*	contains internal data from YY_CURRENT_BUFFER	*/
typedef struct _sm_l_buff_info
{
	char* yy_ch_buf;		/* input buffer */
	char* yy_buf_pos;		/* current position in input buffer */

	/* Size of input buffer in bytes, not including room for EOB
	 * characters.
	 */
	int yy_buf_size;

	/* Number of characters read into yy_ch_buf, not including EOB
	 * characters.
	 */
	int yy_n_chars;
} sm_l_buff_info, * psm_l_buff_info;

typedef void* yyscan_t;

/*	parser context	*/
typedef struct _sm_parser_ctx
{
	yyscan_t			m_scanner;				//	current scanner
	sm_calc_params		m_calc_params;			//	parser/calculator params
	sm_calc_res			m_calc_res;				//	global instance of definition the current result of calculations
} sm_parser_ctx, *psm_parser_ctx;

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
typedef int (*parser_callback_t)(sm_parser_ctx* const parser_ctx, void* param);

#define SM_PREC_MIN			0		//	minimal value of m_f_precision (current precision of float value output)
#define SM_PREC_MAX			20		//	maximum value of m_f_precision (current precision of float value output)
#define SM_PREC_DEF			10		//	default value of m_f_precision (current precision of float value output)
#define SM_PREC_EXP			-1		//	value of m_f_precision (current precision of float value output) if current precision value == 'exp' 

#if defined __cplusplus
}
#endif
