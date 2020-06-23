/*
 *    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
 *    Sergey Manoylo <info@smansoft.com>
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#if defined SM_OS_LINUX
#include <unistd.h>
#elif defined SM_OS_WINDOWS
#include <io.h> 
#endif

#include "sm_calc_types.h"

#if defined __cplusplus
extern "C" {
#endif

	/*	launching the lexical analyzer;	provides scanning the buffer or scanning the stream file (stdin or some open input file)	*/
	int	yylex(void);

	/*	launching the syntax parser	*/
	int	yyparse(void);

	/*	error processor; this function is called by the syntax parser	*/
	int	yyerror(char* error);

	/*
		the function gets long long argument, converts it into string, according to 
		current output format of integers (decimal, octal, hexadecimal,
		binary formats) (from the current state of sm_calculator configuration)
		and prints out it in yyout stream device
	*/
	errno_t sm_print_ll(const long long arg);

	/*
		the function gets long double argument and prints out it in yyout stream device,
		according to m_f_prec_buf (current precision buffer for printing of float values)
		(from the current state of sm_calculator configuration);
		m_f_prec_buf is updated, (using function sm_init_f_prec_buf()) after every updating
		the current precision of float values
	*/
	errno_t sm_print_ld(const long double arg);

	/*	the function, that prints string (ascii) message (arg) into yyout stream device	*/
	errno_t sm_print_sz(const char* const arg);

	/*	the function, that prints '\n' (end of line) symbol into yyout stream device	*/
	errno_t sm_print_nl();

	/*	the  function, that prints error into yyout stream device	*/
	errno_t sm_print_error(const char* const err_message);

	/*	
		this function reads file from gcsm_gelp_fpath path and print out 
		content of this file into stdout device;

		gcsm_gelp_fpath == "../share/smansoft/doc/HELP"		- Linux platform
		gcsm_gelp_fpath == "..\share\smansoft\doc\HELP"		- Windows platform
	*/
	errno_t sm_print_help();

	/*
		exit from the application;
		setup
			gsm_exit = 1;
		or
			calls exit(0);

		this function is called, when user enters commands 'exit[;]' or 'quit[;]'
	*/
	void sm_do_exit();

	/*
		just copying buffer;

		in args:
			in_buf		- source buffer
			out_buf_len	- destination buffer length
		out args:
			out_buf		- destination buffer

		this function is called, when user enters sting in command prompt:  "string_value" | 'string_value'
	*/
	errno_t sm_copy_buf(const char* const in_buf, char* const out_buf, const size_t out_buf_len);

	/*
		copying current configuration into buffer in format:

			Current Configuration :
			---------------------------------------------
			Current double precision          : 10;
			Current output format of integers : dec;
			Unit of measurement of an angle   : rad;
		;

		in args:
			buf_len	- destination buffer length
		out args:
			buf		- destination buffer

		this function is called, when user enters command 'config[;]'
	*/
	errno_t	sm_get_config(char* const buf, const size_t buf_len);

	/*
		copying current entering and output formats of integers (decimal, octal, hexadecimal, binary formats)
		in buffer; current entering and output formats of integers is read from the current state
		of sm_calculator configuration;

		in args:
			buf_len	- destination buffer length
		out args:
			buf		- destination buffer

		this function is called, when user enters commands:
			i_format = <dec|hex|oct|bin>[;]
			i_format[;]
			dec[;]
			oct[;]
			hex[;]
			bin[;]
	*/
	errno_t	sm_get_i_format(char* const buf, const size_t buf_len);

	/*
		setup current entering and output formats of integers (decimal, octal, hexadecimal, binary formats)
		in global instance of definition the current state of sm_calculator configuration
		(sm_calc_params gsm_calc_param);

		in args:
			i_format	- entering and output formats of integers (decimal, octal, hexadecimal, binary formats)

		this function is called, when user enters commands:
			i_format = <dec|hex|oct|bin>[;]
			dec[;]
			oct[;]
			hex[;]
			bin[;]
	*/
	errno_t sm_set_i_format(const sm_i_format i_format);

	/*
		copying current unit for measuring of angles (radians, degrees, gradians), which are used
        by trigonometric functions in buffer;
		current unit for measuring of angles is read from the current state of sm_calculator configuration;

		in args:
			buf_len	- destination buffer length
		out args:
			buf		- destination buffer

		this function is called, when user enters commands:
			a_unit = <deg|rad|grad>[;]
			a_unit[;]
			deg[;]
			rad[;]
			grad[;]
	*/
	errno_t	sm_get_a_unit(char* const buf, const size_t buf_len);

	/*
		setup current unit for measuring of angles (radians, degrees, gradians), which are used
        by trigonometric functions in buffer in global instance of definition the current state
		of sm_calculator configuration (sm_calc_params gsm_calc_param);

		in args:
			trig_unit	- unit for measuring of angles (radians, degrees, gradians)

		this function is called, when user enters commands:
			a_unit = <deg|rad|grad>[;]
			deg[;]
			rad[;]
			grad[;]
	*/
	errno_t sm_set_a_unit(const sm_trig_unit trig_unit);

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
			const_idx	- constant index

		out args:
			const_value	- result (constant value)

		this function is called, when user uses constants:
			pi,
			exp,
			gamms,
			phi
		in expression(s);
		also this function is called in functions, which provide converting of
		unit for measuring of angles
	*/
	errno_t	sm_get_const(const sm_const_idx const_idx, long double* const const_value);

	/*
		function retuns integer (long long) result of sum of integer (long long) arguments;

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		the function is called, when user uses operator '+' in expression(s)
	*/
	errno_t	sm_calc_add_i(const long long arg1, const long long arg2, long long* const res);

	/*
		function retuns integer (long long) result of subtraction of integer (long long) arguments;

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		the function is called, when user uses operator '-' in expression(s)
	*/
	errno_t	sm_calc_sub_i(const long long arg1, const long long arg2, long long* const res);

	/*
		function retuns integer (long long) result of multiplication of integer (long long) arguments;

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		the function is called, when user uses operator '*' in expression(s)
	*/
	errno_t	sm_calc_mul_i(const long long arg1, const long long arg2, long long* const res);

	/*
		function retuns float (long double) result of sum of float (long double) arguments;

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		the function is called, when user uses operator '+' in expression(s)
	*/
	errno_t	sm_calc_add_f(const long double arg1, const long double arg2, long double* const res);

	/*
		function retuns float (long double) result of subtraction of float (long double) arguments;

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		the function is called, when user uses operator '-' in expression(s)
	*/
	errno_t	sm_calc_sub_f(const long double arg1, const long double arg2, long double* const res);

	/*
		function retuns float (long double) result of multiplication of float (long double) arguments;

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		the function is called, when user uses operator '*' in expression(s)
	*/
	errno_t	sm_calc_mul_f(const long double arg1, const long double arg2, long double* const res);

	/*
		function retuns float (long double) result of division of float (long double) arguments;

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		the function is called, when user uses operator '/' in expression(s)
	*/
	errno_t	sm_calc_div(const long double arg1, const long double arg2, long double* const res);

	/*
		trigonometric function retuns float (long double) result of sine of float (long double) argument;

		in args:
			arg		- argument
		out args:
			res		- result

		argument should be defined as value of current unit for measuring of angles(deg|rad|grad);
		the function is called, when user uses function 'sin' in expression(s)
	*/
	errno_t	sm_calc_sin(const long double arg, long double* const res);

	/*
		trigonometric function retuns float (long double) result of cosine of float (long double) argument;

		in args:
			arg		- argument
		out args:
			res		- result

		argument should be defined as value of current unit for measuring of angles(deg|rad|grad);
		the function is called, when user uses function 'cos' in expression(s)
	*/
	errno_t	sm_calc_cos(const long double arg, long double* const res);

	/*
		trigonometric function retuns float (long double) result of tangent of float (long double) argument;

		in args:
			arg		- argument
		out args:
			res		- result

		argument should be defined as value of current unit for measuring of angles(deg|rad|grad);
		the function is called, when user uses function 'tan' in expression(s)
	*/
	errno_t	sm_calc_tan(const long double arg, long double* const res);

	/*
		trigonometric function retuns float (long double) result of arcsine of float (long double) argument;

		in args:
			arg		- argument
		out args:
			res		- result

		result of this function is defined as value of current unit for measuring of angles(deg|rad|grad);
		the function is called, when user uses function 'asin' in expression(s)
	*/
	errno_t	sm_calc_asin(const long double arg, long double* const res);

	/*
		trigonometric function retuns float (long double) result of arccosine of float (long double) argument;

		in args:
			arg		- argument
		out args:
			res		- result

		result of this function is defined as value of current unit for measuring of angles(deg|rad|grad);
		the function is called, when user uses function 'acos' in expression(s)
	*/
	errno_t	sm_calc_acos(const long double arg, long double* const res);

	/*
		trigonometric function retuns float (long double) result of arctangent of float (long double) argument;

		in args:
			arg		- argument
		out args:
			res		- result

		result of this function is defined as value of current unit for measuring of angles(deg|rad|grad);
		the function is called, when user uses function 'atan' in expression(s)
	*/
	errno_t	sm_calc_atan(const long double arg, long double* const res);

	/*
		function retuns integer (long long) result of factorial of integer (long long) argument;

		in args:
			arg		- argument
		out args:
			res		- result

		the function is called, when user uses operator '!' in expression(s);

		For example:
			In  >> 3!
			Out >> 6;

			In  >> 5!
			Out >> 120;
	*/
	errno_t sm_calc_factorial(const long long arg, long long* const res);

	/*
		the function returns float (long double) result: arg1 (long double) raised to the power
		of arg2 (long double);

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		where arg1 - float value (long double), arg2 - float value (long double);
		result - float value (long double);

		i.e. the function calculates:
			*res = arg1 ** arg2 
		or 
			pow(arg1, arg2 )

		the function is called, when user uses function 'pow' in expression(s)
	*/
	errno_t	sm_calc_power(const long double arg1, const long double arg2, long double* const res);

	/*
		the function, that calculates and returns '2' raised to the power of arg;

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - integer value (long long); arg should be >= 0;
		result - integer value (long long);

		i.e. the function calculates:
			*res = 2 ** arg
		or
			(int)pow(2, arg )

		the function is called, when user uses function 'pow_2^' in expression(s)
	*/
	errno_t sm_calc_power_2_i(const long long arg, long long* const res);			// 2^i

	/*
		the function, that calculates and returns '10' raised to the power of arg;

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - integer value (long long); arg should be >= 0;
		result - integer value (long long);

		i.e. the function calculates:
			*res = 10 ** arg
		or
			(int)pow(10, arg )

		the function is called, when user uses function 'pow_10^' in expression(s)
	*/
	errno_t sm_calc_power_10_i(const long long arg, long long* const res);		// 10^i

	/*
		the function, that calculates and returns argument raised to the power of '2';

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - integer value (long long);
		result - integer value (long long);

		i.e. the function calculates:
			*res = arg ** 2
		or
			*res = arg * arg

		the function is called, when user uses function 'pow_^2' in expression(s)
	*/
	errno_t sm_calc_power_i_2(const long long arg, long long* const res);			// i^2

	/*
		the function, that calculates and returns '2' raised to the power of arg;

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - float value (long double);
		result - float value (long double);

		i.e. the function calculates:
			*res = 2 ** arg
		or
			pow(2, arg )

		the function is called, when user uses function 'pow_2^' in expression(s)
	*/
	errno_t	sm_calc_power_2_d(const long double arg, long double* const res);		// 2^d

	/*
		the function, that calculates and returns '10' raised to the power of arg;

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - float value (long double);
		result - float value (long double);

		i.e. the function calculates:
			*res = 10 ** arg
		or
			pow(10, arg )

		the function is called, when user uses function 'pow_10^' in expression(s)
	*/
	errno_t	sm_calc_power_10_d(const long double arg, long double* const res);	// 10^d

	/*
		the function, that calculates and returns 'exp' (2.71828182845904523536L)
		raised to the power of arg;

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - float value (long double);
		result - float value (long double);

		i.e. the function calculates:
			*res = exp ** arg
		or
			pow(exp, arg )

		the function is called, when user uses function 'pow_exp^' in expression(s)
	*/
	errno_t	sm_calc_power_exp_d(const long double arg, long double* const res);	// exp^d

	/*
		the function, that calculates and returns arg raised to the power of '2';

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - float value (long double);
		result - float value (long double);

		i.e. the function calculates:
			*res = arg ** 2
		or
			*res = arg * arg

		the function is called, when user uses function 'pow_^2' in expression(s)
	*/
	errno_t sm_calc_power_d_2(const long double, long double* const res);			// d^2

	/*
		the function, that calculates and returns left arithmetic shift of argument;

		in args:
			arg1	- argument
			arg2	- number bits to shift
		out args:
			res		- result

		where arg1 - integer value (long long), arg2 - integer value (long long);
		arg2 should be >= 0;
		result - integer value (long long);

		the function saves sign of negative value during shift;

		the function is called, when user uses operator '<a<' in expression(s)
	*/
	errno_t	sm_calc_l_ashift(const long long arg1, const long long arg2, long long* const res);

	/*
		the function, that calculates and returns right arithmetic shift of argument;

		in args:
			arg1	- argument
			arg2	- number bits to shift
		out args:
			res		- result

		where arg1 - integer value (long long), arg2 - integer value (long long);
		arg2 should be >= 0;
		result - integer value (long long);

		the function is called, when user uses operator '>a>' in expression(s)
	*/
	errno_t	sm_calc_r_ashift(const long long arg1, const long long arg2, long long* const res);

	/*
		the function, that calculates and returns left logical shift of argument;

		in args:
			arg1	- argument
			arg2	- number bits to shift
		out args:
			res		- result

		where arg1 - integer value (long long), arg2 - integer value (long long);
		arg2 should be >= 0;
		result - integer value (long long);

		the function is called, when user uses operator '>l>' in expression(s)
	*/
	errno_t	sm_calc_l_lshift(const long long arg1, const long long arg2, long long* const res);

	/*
		the function, that calculates and returns right logical shift of argument;

		in args:
			arg1	- argument
			arg2	- number bits to shift
		out args:
			res		- result

		where arg1 - integer value (long long), arg2 - integer value (long long);
		arg2 should be >= 0;
		result - integer value (long long);

		the function doesn't save sign of negative value during shift;

		the function is called, when user uses operator '>l>' in expression(s)
	*/
	errno_t	sm_calc_r_lshift(const long long arg1, const long long arg2, long long* const res);

	/*
		the function, that calculates and returns left circular shift of argument;

		in args:
			arg1	- argument
			arg2	- number bits to shift
		out args:
			res		- result

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
	errno_t	sm_calc_l_cshift(const long long arg1, const long long arg2, long long* const res);

	/*
		the function, that calculates and returns right circular shift of argument;

		in args:
			arg1	- argument
			arg2	- number bits to shift
		out args:
			res		- result

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
	errno_t	sm_calc_r_cshift(const long long arg1, const long long arg2, long long* const res);

	/*
		the function, that calculates and returns inverse square root of argument;

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - float value (long double);
		result - float value (long double);

		the function is called, when user uses function 'sqrt' in expression(s)
	*/
	errno_t sm_calc_sqrt(const long double arg, long double* const res);

	/*
		the function, that calculates and returns the natural logarithm (base of the exp) of argument (float long double);

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - float value (long double); arg should be > 0;
		result - float value (long double);

		the function is called, when user uses function 'ln' in expression(s)
	*/
	errno_t sm_calc_ln(const long double arg, long double* const res);

	/*
		the function, that calculates and returns the common logarithm (base of the 10) of argument (float long double);

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - float value (long double); arg should be > 0;
		result - float value (long double);

		the function is called, when user uses function 'ln' in expression(s)
	*/
	errno_t sm_calc_lg(const long double arg, long double* const res);

	/*
		the function, that calculates and returns the logarithm (base, defied as parameter) of argument (float long double);

		in args:
			base		- base of the logarithm
			arg			- argument
		out args:
			res		- result

		where base - float value (long double); base should be > 0;
		arg - float value (long double); arg should be > 0;
		result - float value (long double);

		the function is called, when user uses function 'log' in expression(s)
	*/
	errno_t sm_calc_log(const long double base, const long double arg, long double* const res);

	/*
		the function, that calculates and returns absolute value of argument;

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - integer value (long long);
		result - integer value (long long);

		the function is called, when user uses function 'abs' in expression(s), but argument
		of 'abs' function is a integer value, defined by lexical analyzer
	*/
	errno_t sm_calc_abs_i(const long long arg, long long* const res);

	/*
		the function, that calculates and returns absolute value of argument;

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - float value (long double);
		result - float value (long double);

		the function is called, when user uses function 'abs' in expression(s), but argument
		of 'abs' function is a float value, defined by lexical analyzer
	*/
	errno_t sm_calc_abs_f(const long double arg, long double* const res);

	/*
		the function, that calculates and returns remainder value of division arg1 / arg2
		(often marked as arg1 % arg2);

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		where arg1 - integer value (long long), arg2 - integer value (long long);
		result - integer value (long long);

		i.e. the function calculates:
			*res = arg1 % arg2

		the function is called, when user uses functions 'mod' and 'mod2' in expression(s)
	*/
	errno_t sm_calc_mod(const long long arg1, const long long arg2, long long* const res);

	/*
		the function, that calculates and returns inverse value (1/x) of argument;

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - float value (long double);
		result - float value (long double);

		the function is called, when user uses function 'inv' in expression(s)
	*/
	errno_t	sm_calc_recip(const long double arg, long double* const res);

	/*
		the function, that returns pseudo-random value [0,1];

		out args:
			res		- result

		the function is called, when user uses command 'rand[;]' in expression(s)
	*/
	errno_t	sm_get_rand(long double* const res);

	/*
		the function (bitwise function), that calculates bitwise multiplication of arguments ( arg1 && arg2 );

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		where arg1 - integer value (long long), arg2 - integer value (long long);
		result - integer value (long long);
		
		the function is called, when user uses function 'and' in expression(s)
	*/
	errno_t	sm_calc_and(const long long arg1, const long long arg2, long long* const res);

	/*
		the function (bitwise function), that calculates bitwise addition of arguments ( arg1 && arg2 );

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		where arg1 - integer value (long long), arg2 - integer value (long long);
		result - integer value (long long);

		the function is called, when user uses function 'or' in expression(s)
	*/
	errno_t	sm_calc_or(const long long arg1, const long long arg2, long long* const res);

	/*
		the function (bitwise function), that calculates bitwise negation of argument ( !arg );

		in args:
			arg		- argument
		out args:
			res		- result

		where arg - integer value (long long);
		result - integer value (long long);

		the function is called, when user uses function 'not' in expression(s)
	*/
	errno_t	sm_calc_not(const long long arg, long long* const res);

	/*
		the function (bitwise function), that calculates bitwise negation of bitwise multiplication of arguments ( !(arg1 && arg2) );

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		where arg1 - integer value (long long), arg2 - integer value (long long);
		result - integer value (long long);

		the function is called, when user uses function 'nand' in expression(s)
	*/
	errno_t	sm_calc_nand(const long long arg1, const long long arg2, long long* const res);

	/*
		the function (bitwise function), that calculates bitwise negation of bitwise addition
		of arguments ( !(arg1 || arg2) );

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		where arg1 - integer value (long long), arg2 - integer value (long long);
		result - integer value (long long);

		the function is called, when user uses function 'nor' in expression(s)
	*/
	errno_t	sm_calc_nor(const long long arg1, const long long arg2, long long* const res);

	/*
		the function (bitwise function), that calculates bitwise 'exclusive or' or 'exclusive disjunction'
		of arguments ( arg1 ^ arg2) );

		in args:
			arg1	- first argument
			arg2	- second argument
		out args:
			res		- result

		where arg1 - integer value (long long), arg2 - integer value (long long);
		result - integer value (long long);

		the function is called, when user uses function 'xor' in expression(s)
	*/
	errno_t	sm_calc_xor(const long long arg1, const long long arg2, long long* const res);

	/*
		the function that setup and returns current precision of float values;

		in args:
			precision	- argument
		out args:
			res			- result

		where precision - integer value (long long); precision should be [0,20];
		result - integer value (long long);

		the function is called, when user calls command 'f_precision = N[;]'
	*/
	errno_t sm_set_f_precision(const long long precision, long long* const res);

	/*
		the function that setup current precision of float values as 'exp' (-1);

		the function is called, when user calls command 'f_precision = exp[;]'
	*/
	errno_t sm_set_f_exp_precision();

	/*
		the function that returns current precision of float values and copying precision 
		current precision of float values in buffer;

		in args:
			buf_len		- length of the buffer
		out args:
			buf			- result

		buf == [0,20]|exp

		the function is called, when user calls commands 'f_precision = exp[;]' or 'f_precision[;]'
	*/
	errno_t	sm_get_f_precision(char* buf, const size_t buf_len);

	/*
		the function removes symbols ' and " (quotation marks) from input buffer and copies result
		into result buffer;

		in args:
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
	errno_t	sm_conv_quot_sz_2_sz(char* const in_buf, char* const out_buf, const size_t out_buf_len);

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
	errno_t	sm_conv_i_dec_2_sz(const sm_i_format i_format, const long long arg, char* const buf, const size_t buf_len);

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
	errno_t	sm_conv_sz_2_i_dec(const sm_i_format i_format, const unsigned char* const buf, const size_t buf_len, long long* const res);

	/*
		converting of sm_bin (native unsigned char format) to char sz array:

		for example:
			in:
				10101
			out:
				"00010101"
	*/
	errno_t sm_conv_bin_2_bin_sz(const sm_bin* const in_bin, char* const out_buf, const size_t out_buf_len);


	/*
		converting of char sz array to sm_bin (native unsigned char format):

		for example:
			in:
				"00010101"
			out:
				10101
	*/
	errno_t sm_conv_bin_sz_2_bin(const char* const in_buf, const size_t in_buf_len, sm_bin* const out_bin);

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
	errno_t sm_conv_i_dec_2_bin(const long long arg, sm_bin* const out_bin);

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
	errno_t sm_conv_bin_2_i_dec(const sm_bin* const in_bin, long long* const res);

	/*	
		converting of char sz array (ascii char format) to long double value:

		in:
			buf		- input char sz array (for example: "3.14159265358979323846", "1.2e-5")
			buf_len - size of input char sz array
		out:
			long double	- result variable
	*/
	errno_t	sm_conv_sz_2_f_dec(char* const buf, const size_t buf_len, long double* const res);

	/*	
		converting of long double value to char sz array (ascii char format):

		in:
			long double variable
		out:
			buf		- output char sz array (for example: "3.14159265358979323846", "1.2e-5")
			buf_len - size of input char sz array
	*/
	errno_t	sm_conv_f_dec_2_sz(long double arg, char* const buf, const size_t buf_len);

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
	errno_t	sm_conv_curr_2_rad(const long double arg, long double* const res);

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
	errno_t	sm_conv_rad_2_curr(const long double rad, long double* const res);

	/*
		the function, that calculates and returns converting of value of unit for measuring of
		angles == radians ('rad'); argument is value of unit for measuring of angles == gradians ('grad');

		in args:
			grad			- argument
		out args:
			res			- result

		where grad - float value (long double); grad should be value of unit for measuring of
		angles == gradians ('grad');
		res - float value (long double); result is a value of unit for measuring of
		angles == radians ('rad');
	*/
	errno_t	sm_conv_grad_2_rad(const long double grad, long double* const res);

	/*
		the function, that calculates and returns converting of value of unit for measuring of
		angles == gradians ('grad'); argument is value of unit for measuring of angles == radians ('rad');

		in args:
			rad			- argument
		out args:
			res			- result

		where rad - float value (long double); rad should be value of unit for measuring of
		angles == radians ('rad');
		res - float value (long double); result is a value of unit for measuring of
		angles == gradians ('grad');
	*/
	errno_t	sm_conv_rad_2_grad(const long double rad, long double* const res);

	/*
		the function, that calculates and returns converting of value of unit for measuring of
		angles == degrees ('deg'); argument is value of unit for measuring of angles == radians ('rad');

		in args:
			rad			- argument
		out args:
			res			- result

		where rad - float value (long double); rad should be value of unit for measuring of
		angles == radians ('rad');
		res - float value (long double); result is a value of unit for measuring of
		angles == degrees ('deg');
	*/
	errno_t	sm_conv_rad_2_deg(const long double rad, long double* const res);

	/*
		the function, that calculates and returns converting of value of unit for measuring of
		angles == radians ('rad'); argument is value of unit for measuring of angles == degrees ('deg');

		in args:
			deg			- argument
		out args:
			res			- result

		where deg - float value (long double); deg should be value of unit for measuring of
		angles == degrees ('deg');
		res - float value (long double); result is a value of unit for measuring of
		angles == radians ('rad');
	*/
	errno_t	sm_conv_deg_2_rad(const long double deg, long double* const res);

	/*
		the function, that calculates and returns converting of value of unit for measuring of
		angles == degrees ('deg'); argument is value of unit for measuring of angles == gradians ('grad');

		in args:
			grad		- argument
		out args:
			res			- result

		where grad - float value (long double); grad should be value of unit for measuring of
		angles == gradians ('grad');
		res - float value (long double); result is a value of unit for measuring of
		angles == degrees ('deg');
	*/
	errno_t	sm_conv_grad_2_deg(const long double grad, long double* const res);

	/*
		the function, that calculates and returns converting of value of unit for measuring of
		angles == gradians ('grad'); argument is value of unit for measuring of angles == degrees ('deg');

		in args:
			deg			- argument
		out args:
			res			- result

		where deg - float value (long double); deg should be value of unit for measuring of
		angles == degrees ('deg');
		res - float value (long double); result is a value of unit for measuring of
		angles == gradians ('grad');
	*/
	errno_t	sm_conv_deg_2_grad(const long double deg, long double* const res);

/*
	Macros, that setup SM_RES_OK in the global
	sm_calc_res gsm_calc_res	- global instance of definition the current result of calculations
*/
#define SM_SET_CALC_RES_OK	{	gsm_calc_res.m_res = SM_RES_OK;	\
								gsm_calc_res.m_sz_message[0] = '\0'; }

/*
	Macros, that setup SM_RES_ERROR and saves error message 
	in the global sm_calc_res gsm_calc_res	- global instance of definition the current result of calculations

	gsm_calc_res.m_sz_message is used during printing the result of last command/function/operator execution
*/
#define SM_SET_CALC_RES_ERROR(message)	{	gsm_calc_res.m_res = SM_RES_ERROR;	\
											safe_strcpy(gsm_calc_res.m_sz_message, SM_ARRAY_SIZE(gsm_calc_res.m_sz_message), message);	}

	extern	int				gsm_exit;			//	flag (if gsm_exit != 0), that precessing cycle:
												//	taking expression(s) :
												//	In >>
												//	, printing result(s) :
												//	Out >>
												//	should be finished and application should exit

	extern	sm_calc_params	gsm_calc_params;	//	global instance of definition the current state of sm_calculator configuration
	extern	sm_calc_res		gsm_calc_res;		//	global instance of definition the current result of calculations

	extern const long double	gcsm_consts[];	//	array of long double constants, defined in
												//	enum _sm_const_idx
												//	sm_const_idx

	extern const char* const	gcsm_trig_units[];		//	string description of unit for measuring of angles (radians, degrees, gradians) 
	extern const char* const	gcsm_exp_precision;		//	contsins exponential constant string ('exp')

	extern const char* const	gcsm_f_scanf_formats;					//	scan format of float value (same for any output precision)
	extern char					gsm_f_print_formats[][SM_S_BUFF_SIZE];	//	current precision of float value output formats
																		//	gsm_f_print_formats[0] is used, when current precision value == 'exp' (SM_PREC_EXP)
																		//	gsm_f_print_formats[1] is used, when current precision value == [0,20]
																		//	gsm_f_print_formats[1] contains current print format string %.[0,20]Lf"

	extern const char* const	gcsm_i_formats[];						//	string description of output formats of integers (decimal, octal, hexadecimal, binary formats)

	extern const char* const	gcsm_i_scanf_formats[];					//	scan formats (entering formats) of integers (decimal, octal, hexadecimal, binary formats)
	extern const char* const	gcsm_i_printf_formats[];				//	print formats (output formats) of integers (decimal, octal, hexadecimal, binary formats)

	extern const char* const	gcsm_s_format;							// contains format for string output/print
	extern const char* const	gcsm_nl;								// contains format for new line output/print

#if defined __cplusplus
}
#endif
