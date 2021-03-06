/*
 *    Copyright (c) 2020-2021 SManSoft <http://www.smansoft.com/>
 *    Sergey Manoylo <info@smansoft.com>
 */

#pragma once

#if defined __cplusplus
extern "C" {
#endif

	/*
		if defined UNIT_TESTING, function main() is compiled with name sm_calc_main;
		it's necessary for
	*/
#ifdef UNIT_TESTING
	/*	redefinition of entry point */
	int sm_calc_main(int argc, char* argv[]);
#endif

	/*	printing the current version of sm_calculator (yyout device)	*/
	void sm_calc_show_version();

	/*	printing the current version of sm_calculator (log file)	*/
	void sm_calc_log_version();

	/*  printing info about launched log	*/
	void sm_calc_show_log_info();

	/*	printing the command line help (-h,--help)	*/
	void	sm_calc_usage();

	/*	initializing the parser context	*/
	void	sm_init_parser_ctx(sm_parser_ctx* const parser_ctx);

	/*	initializing the global sm_calc_params object	*/
	errno_t	sm_set_def_calc_params(sm_parser_ctx* const parser_ctx);

	/*	initializing the random seed (for 'rand' command)	*/
	void	sm_init_random();

	/*	initializing and starting the log output	*/
	errno_t	sm_init_log(sm_parser_ctx* const parser_ctx);
	/*	stopping and closing the log output	*/
	errno_t	sm_close_log(sm_parser_ctx* const parser_ctx);

	/*
		provides log out (in log file) the sm_calc_params gsm_calc_params,
		current state of sm_calculator configuration
	*/
	errno_t	sm_log_calc_params(sm_parser_ctx* const parser_ctx);
	/*
		provides log out (in log file) the sm_calc_res gsm_calc_res,
		current result of calculations
	*/
	errno_t	sm_log_calc_res(sm_parser_ctx* const parser_ctx);
	/*
		provides log out (in log file) sm_bin object
		(structure, that incapsulates integer value storaging in binary format):

		args:
			prfx	- message, that will be pronted before sm_bin object
			bin		- sm_bin object, that should be log out
	*/
	errno_t	sm_log_bin(const char* const prfx, const sm_bin* const bin);

	/*
		provides additional initializing the sm_calc_params gsm_calc_params,
		current state of sm_calculator configuration,
		based on current partialy-known fields of sm_calc_params gsm_calc_params
	*/
	errno_t	sm_proc_params(sm_parser_ctx* const parser_ctx);

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
	errno_t sm_init_f_prec_buf(sm_parser_ctx* const parser_ctx);

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
	errno_t sm_proc_calc(sm_parser_ctx* const parser_ctx);

	/*
		provides interactive (using stdin, stdout) processing of input of user:
		taking expression(s):
			In  >>
		, printing result(s):
			Out >>
		and cycle of input/output of
	*/
	errno_t sm_do_calc_interactive(sm_parser_ctx* const parser_ctx);
	/*
		provides processing m_expression (field of the sm_calc_params gsm_calc_params)
		and printing result
	*/
	errno_t sm_do_calc_expression(sm_parser_ctx* const parser_ctx);
	/*
		provides processing in_file (input) file and output result in out_file
	*/
	errno_t sm_do_calc_files(sm_parser_ctx* const parser_ctx, FILE* const in_file, FILE* const out_file);

	/*	these functions
			sm_scan_string
			sm_scan_file
		are implemented in module of lexical analyzer,
		as they use internals of lexical analyzer*/

	/*
		provides scanning of the str(buffer),
		calling yy_scan_string() in module of lexical analyzer
	*/
	void sm_scan_string(yyscan_t scanner, const char* const in_str);

	/*
		provides initializing the lexical analyzer for read the in_f file
	*/
	void sm_scan_file(yyscan_t scanner, FILE* const in_f);

	/*
		returns current output file stream, used by the parser
	*/
	FILE* sm_get_yyout(yyscan_t scanner);

	/*
		returns current input file stream, used by the parser
	*/
	FILE* sm_get_yyin(yyscan_t scanner);

	/*
		setup output file stream, that should be used by the parser
	*/
	FILE* sm_set_yyout(yyscan_t scanner, FILE * const yyout_v);

	/*
		setup input file stream, that should be used by the parser
	*/
	FILE* sm_set_yyin(yyscan_t scanner, FILE * const yyin_v);

	/*
		provides processing of gcsm_help_fpath, making it an absolute path, result will be copied to gsm_help_fpath;
		this gsm_help_fpath is used, during read of HELP file
	*/
	errno_t proc_help_fpath(sm_parser_ctx* const parser_ctx);

#if defined __cplusplus
}
#endif
