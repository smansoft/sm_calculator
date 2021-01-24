
/*
 *    Copyright (c) 2020-2021 SManSoft <http://www.smansoft.com/>
 *    Sergey Manoylo <info@smansoft.com>
 */

%define api.pure full

%lex-param   { yyscan_t scanner }
%parse-param { yyscan_t scanner }
%parse-param { parser_callback_t parser_callback }

%code requires {

	#ifndef YY_EXTRA_TYPE
		#define YY_EXTRA_TYPE sm_parser_ctx*
	#endif	

	#ifndef YY_TYPEDEF_YY_SCANNER_T
		#define YY_TYPEDEF_YY_SCANNER_T
		typedef void *yyscan_t;
	#endif

	#include "sm_calc_y.h"
	#include "sm_calc_types.h"

	YY_EXTRA_TYPE yyget_extra (yyscan_t yyscanner);

	typedef int (*parser_callback_t)(sm_parser_ctx* const parser_ctx, void *param);
	extern int yyerror(yyscan_t scanner, parser_callback_t parser_callback, const char *err_message);

	#include "sm_calc_proc.h"
}

%code provides {

	extern int yylex (YYSTYPE * yylval_param , yyscan_t yyscanner);

}

%union {
	long long 		ll_val;
	long double 	ld_val;
	char			sz_val[SM_L_BUFF_SIZE];
}

%token<ll_val>  T_LL_10 T_LL_8 T_LL_16 T_LL_2 T_F_PRECISION
%token<ld_val>  T_LD_N T_LD_E T_PI T_EXP T_GAMMA T_PHI
%token<sz_val>  T_SZ

%token  T_NEWLINE T_STOP

%token  T_CONFIG T_QUIT T_EXIT T_HELP T_ECHO

%token	T_A_UNIT T_I_FORMAT

%token	T_AND T_OR T_NOT T_NAND T_NOR T_XOR

%token	T_RAND
%token  T_ABS T_MOD T_MOD_2

%token  T_SQRT T_LN T_LG T_LOG T_RECIP

%token	T_POWER

%token	T_POWER_X_2
%token	T_POWER_EXP_X T_POWER_10_X T_POWER_2_X

%token	T_L_ASHIFT T_R_ASHIFT T_L_LSHIFT T_R_LSHIFT T_L_CSHIFT T_R_CSHIFT

%token  T_SIN T_COS T_TAN T_ASIN T_ACOS	T_ATAN

%token	T_DEG T_GRAD T_RAD

%token  T_GRAD_RAD T_RAD_GRAD 
%token  T_RAD_DEG T_DEG_RAD 
%token  T_GRAD_DEG	T_DEG_GRAD

%token	T_DEC_OCT T_DEC_HEX T_DEC_BIN
%token	T_OCT_DEC T_HEX_DEC T_BIN_DEC

%token  T_DEC T_OCT T_HEX T_BIN

%token	T_COMMA

%token	T_FACTORIAL
%token  T_L_BRACK T_R_BRACK

%token	T_UMINUS

%left	T_ADD T_SUB
%left	T_MUL T_DIV
%left	T_POWER T_L_ASHIFT T_R_ASHIFT T_L_LSHIFT T_R_LSHIFT T_L_CSHIFT T_R_CSHIFT
%left	T_ASSIGN
%left	T_UMINUS T_FACTORIAL

%type<ll_val> 	ll_10_expr ll_8_expr ll_16_expr ll_2_expr 
%type<ld_val> 	ld_expr
%type<sz_val> 	sz_expr

%start calc_run

%%

calc_run	: 
		calc_res calc_run 
	| 	calc_res 
;

calc_res	:   
		T_NEWLINE					{	sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}

	|	ll_10_expr 	T_STOP		{	sm_print_ll(yyget_extra(scanner), $1); SM_RES_OK;	}
	|	ld_expr 	T_STOP		{	sm_print_ld(yyget_extra(scanner), $1); SM_RES_OK; }
	|	sz_expr 	T_STOP		{	sm_print_sz(yyget_extra(scanner), $1); SM_RES_OK;	}

	|	T_HELP T_STOP			{	sm_print_help(yyget_extra(scanner)); SM_RES_OK;	}

	| 	T_QUIT T_STOP     		{	sm_do_exit(yyget_extra(scanner)); SM_RES_OK;	}
	| 	T_EXIT T_STOP			{	sm_do_exit(yyget_extra(scanner)); SM_RES_OK;	}

	|	ll_10_expr 	T_NEWLINE		{	sm_print_ll(yyget_extra(scanner), $1); sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
	|	ld_expr 	T_NEWLINE		{	sm_print_ld(yyget_extra(scanner), $1); sm_print_nl(yyget_extra(scanner)); SM_RES_OK; }
	|	sz_expr 	T_NEWLINE		{	sm_print_sz(yyget_extra(scanner), $1); sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}

	|	T_HELP T_NEWLINE			{	sm_print_help(yyget_extra(scanner)); sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}

	| 	T_QUIT T_NEWLINE     		{	sm_do_exit(yyget_extra(scanner)); sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
	| 	T_EXIT T_NEWLINE			{	sm_do_exit(yyget_extra(scanner)); sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}

	|	ll_10_expr  error T_STOP		{	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
	|	ld_expr 	error T_STOP		{	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK; }
	|	sz_expr 	error T_STOP		{	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}

	|	T_HELP error T_STOP				{	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}

	| 	T_QUIT error T_STOP     		{	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
	| 	T_EXIT error T_STOP				{	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}

	|	ll_10_expr 	error T_NEWLINE		{	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
	|	ld_expr 	error T_NEWLINE		{	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK; }
	|	sz_expr 	error T_NEWLINE		{	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}

	|	T_HELP error T_NEWLINE			{	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}

	| 	T_QUIT error T_NEWLINE     		{	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
	| 	T_EXIT error T_NEWLINE			{	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
;

sz_expr	:
		T_SZ					{ 	sm_copy_buf(yyget_extra(scanner), $1, $$, SM_ARRAY_SIZE($$)); }

	|	T_CONFIG				{	sm_get_config(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$));	}

	|	T_F_PRECISION 			{	sm_get_f_precision(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$));	}
	|	T_F_PRECISION	T_ASSIGN T_EXP		{	sm_set_f_exp_precision(yyget_extra(scanner)); sm_get_f_precision(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$)); }

	|	T_A_UNIT				{	sm_get_a_unit(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$));	}

	|	T_A_UNIT T_ASSIGN T_RAD		{	sm_set_a_unit(yyget_extra(scanner), SM_TRIG_RADIANS); sm_get_a_unit(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$));	}
	|	T_A_UNIT T_ASSIGN T_DEG		{	sm_set_a_unit(yyget_extra(scanner), SM_TRIG_DEGREES); sm_get_a_unit(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$)); 	}
	|	T_A_UNIT T_ASSIGN T_GRAD	{	sm_set_a_unit(yyget_extra(scanner), SM_TRIG_GRADIAN); sm_get_a_unit(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$)); 	}

	|	T_RAD 					{	sm_set_a_unit(yyget_extra(scanner), SM_TRIG_RADIANS); sm_get_a_unit(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$));	}
	|	T_DEG					{	sm_set_a_unit(yyget_extra(scanner), SM_TRIG_DEGREES); sm_get_a_unit(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$));	}
	|	T_GRAD					{	sm_set_a_unit(yyget_extra(scanner), SM_TRIG_GRADIAN); sm_get_a_unit(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$));	}

	|	T_I_FORMAT				{	sm_get_i_format(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$));	}

	|	T_I_FORMAT T_ASSIGN T_DEC	{	sm_set_i_format(yyget_extra(scanner), SM_I_10); sm_get_i_format(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$));	}
	|	T_I_FORMAT T_ASSIGN T_OCT	{	sm_set_i_format(yyget_extra(scanner), SM_I_8); sm_get_i_format(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$)); 	}
	|	T_I_FORMAT T_ASSIGN T_HEX	{	sm_set_i_format(yyget_extra(scanner), SM_I_16); sm_get_i_format(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$)); 	}
	|	T_I_FORMAT T_ASSIGN T_BIN	{	sm_set_i_format(yyget_extra(scanner), SM_I_2); sm_get_i_format(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$)); 	}

	|	T_DEC	 					{	sm_set_i_format(yyget_extra(scanner), SM_I_10); sm_get_i_format(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$));	}
	|	T_OCT						{	sm_set_i_format(yyget_extra(scanner), SM_I_8); sm_get_i_format(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$));	}
	|	T_HEX						{	sm_set_i_format(yyget_extra(scanner), SM_I_16); sm_get_i_format(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$));	}
	|	T_BIN						{	sm_set_i_format(yyget_extra(scanner), SM_I_2); sm_get_i_format(yyget_extra(scanner), $$, SM_ARRAY_SIZE($$));	}

	|	T_DEC_OCT T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_i_dec_2_sz(yyget_extra(scanner), SM_I_8, $3, $$, SM_ARRAY_SIZE($$));	}
	|	T_DEC_HEX T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_i_dec_2_sz(yyget_extra(scanner), SM_I_16, $3, $$, SM_ARRAY_SIZE($$));	}
	|	T_DEC_BIN T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_i_dec_2_sz(yyget_extra(scanner), SM_I_2, $3, $$, SM_S_BUFF_SIZE+2);	}

	|	T_OCT_DEC T_L_BRACK ll_8_expr T_R_BRACK		{	sm_conv_i_dec_2_sz(yyget_extra(scanner), SM_I_10, $3, $$, SM_ARRAY_SIZE($$));	}
	|	T_HEX_DEC T_L_BRACK ll_16_expr T_R_BRACK	{	sm_conv_i_dec_2_sz(yyget_extra(scanner), SM_I_10, $3, $$, SM_ARRAY_SIZE($$));	}
	|	T_BIN_DEC T_L_BRACK ll_2_expr T_R_BRACK		{	sm_conv_i_dec_2_sz(yyget_extra(scanner), SM_I_10, $3, $$, SM_ARRAY_SIZE($$));	}

	|	T_ECHO T_L_BRACK sz_expr T_R_BRACK			{	sm_copy_buf(yyget_extra(scanner), $3, $$, SM_ARRAY_SIZE($$));	}
;

ll_10_expr	:
		T_LL_10						{ 	$$ = $1; }

	|	ll_8_expr
	|	ll_16_expr
	|	ll_2_expr

    |	T_L_BRACK ll_10_expr T_R_BRACK	{	$$=$2;	}

	|	ll_10_expr T_ADD ll_10_expr		{	sm_calc_add_i(yyget_extra(scanner), $1, $3, &$$);	}
	|	ll_10_expr T_SUB ll_10_expr		{	sm_calc_sub_i(yyget_extra(scanner), $1, $3, &$$);	}
	|	ll_10_expr T_MUL ll_10_expr		{	sm_calc_mul_i(yyget_extra(scanner), $1, $3, &$$);	}

	|	T_SUB ll_10_expr %prec T_UMINUS	{	$$ = (-1) * $2;	}

	|	T_F_PRECISION T_ASSIGN T_LL_10	{	sm_set_f_precision(yyget_extra(scanner), (int)$3, &$$);	}

	|	ll_10_expr T_FACTORIAL		{	sm_calc_factorial(yyget_extra(scanner), $1, &$$);		}
	/* "^2" - T_POWER_X_2 */
	|	T_POWER_X_2 T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_power_i_2(yyget_extra(scanner), $3, &$$);	}
	/* "10^" - T_POWER_10_X */
	|	T_POWER_10_X T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_power_10_i(yyget_extra(scanner), $3, &$$);	}
	/* "2^" - T_POWER_2_X */
	|	T_POWER_2_X T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_power_2_i(yyget_extra(scanner), $3, &$$);	}

	|	ll_10_expr T_L_ASHIFT ll_10_expr	{	sm_calc_l_ashift(yyget_extra(scanner), $1, $3, &$$);	}
	|	ll_10_expr T_R_ASHIFT ll_10_expr	{	sm_calc_r_ashift(yyget_extra(scanner), $1, $3, &$$);	}

	|	ll_10_expr T_L_LSHIFT ll_10_expr	{	sm_calc_l_lshift(yyget_extra(scanner), $1, $3, &$$);	}
	|	ll_10_expr T_R_LSHIFT ll_10_expr	{	sm_calc_r_lshift(yyget_extra(scanner), $1, $3, &$$);	}

	|	ll_10_expr T_L_CSHIFT ll_10_expr	{	sm_calc_l_cshift(yyget_extra(scanner), $1, $3, &$$);	}
	|	ll_10_expr T_R_CSHIFT ll_10_expr	{	sm_calc_r_cshift(yyget_extra(scanner), $1, $3, &$$);	}

	|	T_ABS T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_abs_i(yyget_extra(scanner), $3, &$$);	}

	|	T_MOD T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_mod(yyget_extra(scanner), $3, $5, &$$);	}
	|	T_MOD_2 T_L_BRACK ll_10_expr T_R_BRACK						{	sm_calc_mod(yyget_extra(scanner), $3, 2ll, &$$);	}

	|	T_AND 	T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_and(yyget_extra(scanner), $3, $5, &$$);	}
	|	T_OR 	T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_or(yyget_extra(scanner), $3, $5, &$$);	}
	|	T_NOT 	T_L_BRACK ll_10_expr T_R_BRACK							{	sm_calc_not(yyget_extra(scanner), $3, &$$);		}
	|	T_NAND 	T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_nand(yyget_extra(scanner), $3, $5, &$$);	}
	|	T_NOR 	T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_nor(yyget_extra(scanner), $3, $5, &$$);	}
	|	T_XOR 	T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_xor(yyget_extra(scanner), $3, $5, &$$);	}
;

ll_8_expr:
		T_LL_8							{ 	$$ = $1; }
;

ll_16_expr:
		T_LL_16							{ 	$$ = $1; }
;

ll_2_expr:
		T_LL_2							{ 	$$ = $1; }
;

ld_expr	:
		T_LD_N 							{ 	$$ = $1; 	}
	|	T_LD_E 							{ 	$$ = $1; 	}

	|	T_L_BRACK ld_expr T_R_BRACK		{	$$ = $2;	}

	|	T_PI 							{	sm_get_const(yyget_extra(scanner), SM_CONST_PI, &$$);		}
	|	T_EXP							{	sm_get_const(yyget_extra(scanner), SM_CONST_EXP, &$$);	}
	|	T_GAMMA							{ 	sm_get_const(yyget_extra(scanner), SM_CONST_GAMMA, &$$);	}
	|	T_PHI							{ 	sm_get_const(yyget_extra(scanner), SM_CONST_PHI, &$$);	}

	|	ld_expr T_ADD ll_10_expr		{	sm_calc_add_f(yyget_extra(scanner), $1, (long double)$3, &$$);	}
	|	ld_expr T_SUB ll_10_expr		{	sm_calc_sub_f(yyget_extra(scanner), $1, (long double)$3, &$$);	}
	|	ld_expr T_MUL ll_10_expr		{	sm_calc_mul_f(yyget_extra(scanner), $1, (long double)$3, &$$);	}

	|	ll_10_expr T_ADD ld_expr		{	sm_calc_add_f(yyget_extra(scanner), (long double)$1, $3, &$$);	}
	|	ll_10_expr T_SUB ld_expr		{	sm_calc_sub_f(yyget_extra(scanner), (long double)$1, $3, &$$);	}
	|	ll_10_expr T_MUL ld_expr		{	sm_calc_mul_f(yyget_extra(scanner), (long double)$1, $3, &$$);	}

	|	ld_expr T_ADD ld_expr			{	sm_calc_add_f(yyget_extra(scanner), $1, $3, &$$);	}
	|	ld_expr T_SUB ld_expr			{	sm_calc_sub_f(yyget_extra(scanner), $1, $3, &$$);	}
	|	ld_expr T_MUL ld_expr			{	sm_calc_mul_f(yyget_extra(scanner), $1, $3, &$$);	}

	|	ll_10_expr T_DIV ll_10_expr		{	sm_calc_div(yyget_extra(scanner), (long double)$1, (long double)$3, &$$);	}
	|	ld_expr T_DIV ll_10_expr		{	sm_calc_div(yyget_extra(scanner), $1, (long double)$3, &$$);			}
	|	ll_10_expr T_DIV ld_expr		{	sm_calc_div(yyget_extra(scanner), (long double)$1, $3, &$$);			}
	|	ld_expr T_DIV ld_expr			{	sm_calc_div(yyget_extra(scanner), $1, $3, &$$);	}

	|	T_SUB ld_expr %prec T_UMINUS {	$$ = (-1.0) * $2;  }

	|	T_SIN T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_sin(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_COS T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_cos(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_TAN T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_tan(yyget_extra(scanner), (long double)$3, &$$);	}

	|	T_ASIN T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_asin(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_ACOS T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_acos(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_ATAN T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_atan(yyget_extra(scanner), (long double)$3, &$$);	}

	|	T_SIN T_L_BRACK ld_expr	T_R_BRACK	{	sm_calc_sin(yyget_extra(scanner), $3, &$$); }
	|	T_COS T_L_BRACK ld_expr	T_R_BRACK	{	sm_calc_cos(yyget_extra(scanner), $3, &$$); }
	|	T_TAN T_L_BRACK ld_expr	T_R_BRACK	{	sm_calc_tan(yyget_extra(scanner), $3, &$$); }

	|	T_ASIN T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_asin(yyget_extra(scanner), $3, &$$);	}
	|	T_ACOS T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_acos(yyget_extra(scanner), $3, &$$);	}
	|	T_ATAN T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_atan(yyget_extra(scanner), $3, &$$); 	}

	|	T_GRAD_RAD T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_grad_2_rad(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_RAD_GRAD T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_rad_2_grad(yyget_extra(scanner), (long double)$3, &$$);	}

	|	T_RAD_DEG T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_rad_2_deg(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_DEG_RAD T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_deg_2_rad(yyget_extra(scanner), (long double)$3, &$$);	}

	|	T_GRAD_DEG T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_grad_2_deg(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_DEG_GRAD T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_deg_2_grad(yyget_extra(scanner), (long double)$3, &$$);	}

	|	T_GRAD_RAD T_L_BRACK ld_expr T_R_BRACK	{	sm_conv_grad_2_rad(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_RAD_GRAD T_L_BRACK ld_expr T_R_BRACK	{	sm_conv_rad_2_grad(yyget_extra(scanner), (long double)$3, &$$);	}

	|	T_RAD_DEG T_L_BRACK ld_expr T_R_BRACK	{	sm_conv_rad_2_deg(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_DEG_RAD T_L_BRACK ld_expr T_R_BRACK	{	sm_conv_deg_2_rad(yyget_extra(scanner), (long double)$3, &$$);	}

	|	T_GRAD_DEG T_L_BRACK ld_expr T_R_BRACK	{	sm_conv_grad_2_deg(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_DEG_GRAD T_L_BRACK ld_expr T_R_BRACK	{	sm_conv_deg_2_grad(yyget_extra(scanner), (long double)$3, &$$);	}
	/* "pow" - T_POWER */
	|	T_POWER T_L_BRACK ld_expr T_COMMA ld_expr T_R_BRACK 		{ sm_calc_power(yyget_extra(scanner), $3, $5, &$$); }
	|	T_POWER T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK 	{ sm_calc_power(yyget_extra(scanner), (long double)$3, (long double)$5, &$$); }
	|	T_POWER T_L_BRACK ld_expr T_COMMA ll_10_expr T_R_BRACK 		{ sm_calc_power(yyget_extra(scanner), $3, (long double)$5, &$$); }
	|	T_POWER T_L_BRACK ll_10_expr T_COMMA ld_expr T_R_BRACK 		{ sm_calc_power(yyget_extra(scanner), (long double)$3, $5, &$$); }
	/* "^2" - T_POWER_2_X */
	|	T_POWER_X_2 	T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_power_d_2(yyget_extra(scanner), $3, &$$);	}
	/* "exp^" - T_POWER_EXP_X */
	|	T_POWER_EXP_X 	T_L_BRACK ld_expr T_R_BRACK		{	sm_calc_power_exp_d(yyget_extra(scanner), $3, &$$);	}
	|	T_POWER_EXP_X 	T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_power_exp_d(yyget_extra(scanner), (long double)$3, &$$);	}
	/* 	"10^" - T_POWER_10_X */
	|	T_POWER_10_X 	T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_power_10_d(yyget_extra(scanner), $3, &$$);	}
	/* 	"2^"  - T_POWER_2_X */
	|	T_POWER_2_X 	T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_power_2_d(yyget_extra(scanner), $3, &$$);	}

	|	T_SQRT T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_sqrt(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_SQRT T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_sqrt(yyget_extra(scanner), $3, &$$);			}

	|	T_LN T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_ln(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_LN T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_ln(yyget_extra(scanner), $3, &$$);			}

	|	T_LG T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_lg(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_LG T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_lg(yyget_extra(scanner), $3, &$$);			}

	|	T_LOG T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK	{	sm_calc_log(yyget_extra(scanner), (long double)$3, (long double)$5, &$$);	}
	|	T_LOG T_L_BRACK ld_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_log(yyget_extra(scanner), $3, (long double)$5, &$$);		}
	|	T_LOG T_L_BRACK ll_10_expr T_COMMA ld_expr T_R_BRACK		{	sm_calc_log(yyget_extra(scanner), (long double)$3, $5, &$$);		}
	|	T_LOG T_L_BRACK ld_expr T_COMMA ld_expr T_R_BRACK			{	sm_calc_log(yyget_extra(scanner), $3, $5, &$$);					}

	|	T_RECIP T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_recip(yyget_extra(scanner), (long double)$3, &$$);	}
	|	T_RECIP T_L_BRACK ld_expr T_R_BRACK		{	sm_calc_recip(yyget_extra(scanner), $3, &$$);					}

	|	T_ABS T_L_BRACK ld_expr T_R_BRACK		{	sm_calc_abs_f(yyget_extra(scanner), $3, &$$);	}

	|	T_RAND	{ sm_get_rand(yyget_extra(scanner), &$$);	}
;

%%
