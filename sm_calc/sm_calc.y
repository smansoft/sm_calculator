
%{

/*
 *    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
 *    Sergey Manoylo <info@smansoft.com>
 */

#include "sm_calc_proc.h"

%}

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
		T_NEWLINE					{	sm_print_nl(); SM_RES_OK;	}

	|	ll_10_expr 	T_STOP		{	sm_print_ll($1); SM_RES_OK;	}
	|	ld_expr 	T_STOP		{	sm_print_ld($1); SM_RES_OK; }
	|	sz_expr 	T_STOP		{	sm_print_sz($1); SM_RES_OK;	}

	|	T_HELP T_STOP			{	sm_print_help(); SM_RES_OK;	}

	| 	T_QUIT T_STOP     		{	sm_do_exit(); SM_RES_OK;	}
	| 	T_EXIT T_STOP			{	sm_do_exit(); SM_RES_OK;	}

	|	ll_10_expr 	T_NEWLINE		{	sm_print_ll($1); sm_print_nl(); SM_RES_OK;	}
	|	ld_expr 	T_NEWLINE		{	sm_print_ld($1); sm_print_nl(); SM_RES_OK; }
	|	sz_expr 	T_NEWLINE		{	sm_print_sz($1); sm_print_nl(); SM_RES_OK;	}

	|	T_HELP T_NEWLINE			{	sm_print_help(); sm_print_nl(); SM_RES_OK;	}

	| 	T_QUIT T_NEWLINE     		{	sm_do_exit(); sm_print_nl(); SM_RES_OK;	}
	| 	T_EXIT T_NEWLINE			{	sm_do_exit(); sm_print_nl(); SM_RES_OK;	}

	|	ll_10_expr  error T_STOP		{	yyerrok; sm_print_nl(); SM_RES_OK;	}
	|	ld_expr 	error T_STOP		{	yyerrok; sm_print_nl(); SM_RES_OK; }
	|	sz_expr 	error T_STOP		{	yyerrok; sm_print_nl(); SM_RES_OK;	}

	|	T_HELP error T_STOP			{	yyerrok; sm_print_nl(); SM_RES_OK;	}

	| 	T_QUIT error T_STOP     		{	yyerrok; sm_print_nl(); SM_RES_OK;	}
	| 	T_EXIT error T_STOP			{	yyerrok; sm_print_nl(); SM_RES_OK;	}

	|	ll_10_expr 	error T_NEWLINE		{	yyerrok; sm_print_nl(); SM_RES_OK;	}
	|	ld_expr 	error T_NEWLINE		{	yyerrok; sm_print_nl(); SM_RES_OK; }
	|	sz_expr 	error T_NEWLINE		{	yyerrok; sm_print_nl(); SM_RES_OK;	}

	|	T_HELP error T_NEWLINE			{	yyerrok; sm_print_nl(); SM_RES_OK;	}

	| 	T_QUIT error T_NEWLINE     		{	yyerrok; sm_print_nl(); SM_RES_OK;	}
	| 	T_EXIT error T_NEWLINE			{	yyerrok; sm_print_nl(); SM_RES_OK;	}
;

sz_expr	:
		T_SZ					{ 	sm_copy_buf($1, $$, SM_ARRAY_SIZE($$)); }

	|	T_CONFIG				{	sm_get_config($$, SM_ARRAY_SIZE($$));	}

	|	T_F_PRECISION 			{	sm_get_f_precision($$, SM_ARRAY_SIZE($$));	}
	|	T_F_PRECISION	T_ASSIGN T_EXP		{	sm_set_f_exp_precision(); sm_get_f_precision($$, SM_ARRAY_SIZE($$)); }

	|	T_A_UNIT				{	sm_get_a_unit($$, SM_ARRAY_SIZE($$));	}

	|	T_A_UNIT T_ASSIGN T_RAD		{	sm_set_a_unit(SM_TRIG_RADIANS); sm_get_a_unit($$, SM_ARRAY_SIZE($$));	}
	|	T_A_UNIT T_ASSIGN T_DEG		{	sm_set_a_unit(SM_TRIG_DEGREES); sm_get_a_unit($$, SM_ARRAY_SIZE($$)); 	}
	|	T_A_UNIT T_ASSIGN T_GRAD	{	sm_set_a_unit(SM_TRIG_GRADIAN); sm_get_a_unit($$, SM_ARRAY_SIZE($$)); 	}

	|	T_RAD 					{	sm_set_a_unit(SM_TRIG_RADIANS); sm_get_a_unit($$, SM_ARRAY_SIZE($$));	}
	|	T_DEG					{	sm_set_a_unit(SM_TRIG_DEGREES); sm_get_a_unit($$, SM_ARRAY_SIZE($$));	}
	|	T_GRAD					{	sm_set_a_unit(SM_TRIG_GRADIAN); sm_get_a_unit($$, SM_ARRAY_SIZE($$));	}

	|	T_I_FORMAT				{	sm_get_i_format($$, SM_ARRAY_SIZE($$));	}

	|	T_I_FORMAT T_ASSIGN T_DEC	{	sm_set_i_format(SM_I_10); sm_get_i_format($$, SM_ARRAY_SIZE($$));	}
	|	T_I_FORMAT T_ASSIGN T_OCT	{	sm_set_i_format(SM_I_8); sm_get_i_format($$, SM_ARRAY_SIZE($$)); 	}
	|	T_I_FORMAT T_ASSIGN T_HEX	{	sm_set_i_format(SM_I_16); sm_get_i_format($$, SM_ARRAY_SIZE($$)); 	}
	|	T_I_FORMAT T_ASSIGN T_BIN	{	sm_set_i_format(SM_I_2); sm_get_i_format($$, SM_ARRAY_SIZE($$)); 	}

	|	T_DEC	 					{	sm_set_i_format(SM_I_10); sm_get_i_format($$, SM_ARRAY_SIZE($$));	}
	|	T_OCT						{	sm_set_i_format(SM_I_8); sm_get_i_format($$, SM_ARRAY_SIZE($$));	}
	|	T_HEX						{	sm_set_i_format(SM_I_16); sm_get_i_format($$, SM_ARRAY_SIZE($$));	}
	|	T_BIN						{	sm_set_i_format(SM_I_2); sm_get_i_format($$, SM_ARRAY_SIZE($$));	}

	|	T_DEC_OCT T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_i_dec_2_sz(SM_I_8, $3, $$, SM_ARRAY_SIZE($$));	}
	|	T_DEC_HEX T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_i_dec_2_sz(SM_I_16, $3, $$, SM_ARRAY_SIZE($$));	}
	|	T_DEC_BIN T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_i_dec_2_sz(SM_I_2, $3, $$, SM_S_BUFF_SIZE+2);	}

	|	T_OCT_DEC T_L_BRACK ll_8_expr T_R_BRACK		{	sm_conv_i_dec_2_sz(SM_I_10, $3, $$, SM_ARRAY_SIZE($$));	}
	|	T_HEX_DEC T_L_BRACK ll_16_expr T_R_BRACK	{	sm_conv_i_dec_2_sz(SM_I_10, $3, $$, SM_ARRAY_SIZE($$));	}
	|	T_BIN_DEC T_L_BRACK ll_2_expr T_R_BRACK		{	sm_conv_i_dec_2_sz(SM_I_10, $3, $$, SM_ARRAY_SIZE($$));	}

	|	T_ECHO T_L_BRACK sz_expr T_R_BRACK			{	sm_copy_buf($3, $$, SM_ARRAY_SIZE($$));	}
;

ll_10_expr	:
		T_LL_10						{ 	$$ = $1; }

	|	ll_8_expr
	|	ll_16_expr
	|	ll_2_expr

    |	T_L_BRACK ll_10_expr T_R_BRACK	{	$$=$2;	}

	|	ll_10_expr T_ADD ll_10_expr		{	sm_calc_add_i($1, $3, &$$);	}
	|	ll_10_expr T_SUB ll_10_expr		{	sm_calc_sub_i($1, $3, &$$);	}
	|	ll_10_expr T_MUL ll_10_expr		{	sm_calc_mul_i($1, $3, &$$);	}

	|	T_SUB ll_10_expr %prec T_UMINUS	{	$$ = (-1) * $2;	}

	|	T_F_PRECISION T_ASSIGN T_LL_10	{	sm_set_f_precision((int)$3, &$$);	}

	|	ll_10_expr T_FACTORIAL		{	sm_calc_factorial($1, &$$);		}
	/* "^2" - T_POWER_X_2 */
	|	T_POWER_X_2 T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_power_i_2($3, &$$);	}
	/* "10^" - T_POWER_10_X */
	|	T_POWER_10_X T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_power_10_i($3, &$$);	}
	/* "2^" - T_POWER_2_X */
	|	T_POWER_2_X T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_power_2_i($3, &$$);	}

	|	ll_10_expr T_L_ASHIFT ll_10_expr	{	sm_calc_l_ashift($1, $3, &$$);	}
	|	ll_10_expr T_R_ASHIFT ll_10_expr	{	sm_calc_r_ashift($1, $3, &$$);	}

	|	ll_10_expr T_L_LSHIFT ll_10_expr	{	sm_calc_l_lshift($1, $3, &$$);	}
	|	ll_10_expr T_R_LSHIFT ll_10_expr	{	sm_calc_r_lshift($1, $3, &$$);	}

	|	ll_10_expr T_L_CSHIFT ll_10_expr	{	sm_calc_l_cshift($1, $3, &$$);	}
	|	ll_10_expr T_R_CSHIFT ll_10_expr	{	sm_calc_r_cshift($1, $3, &$$);	}

	|	T_ABS T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_abs_i($3, &$$);	}

	|	T_MOD T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_mod($3, $5, &$$);	}
	|	T_MOD_2 T_L_BRACK ll_10_expr T_R_BRACK						{	sm_calc_mod($3, 2ll, &$$);	}

	|	T_AND 	T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_and($3, $5, &$$);	}
	|	T_OR 	T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_or($3, $5, &$$);	}
	|	T_NOT 	T_L_BRACK ll_10_expr T_R_BRACK							{	sm_calc_not($3, &$$);		}
	|	T_NAND 	T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_nand($3, $5, &$$);	}
	|	T_NOR 	T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_nor($3, $5, &$$);	}
	|	T_XOR 	T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_xor($3, $5, &$$);	}
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

	|	T_PI 							{	sm_get_const(SM_CONST_PI, &$$);		}
	|	T_EXP							{	sm_get_const(SM_CONST_EXP, &$$);	}
	|	T_GAMMA							{ 	sm_get_const(SM_CONST_GAMMA, &$$);	}
	|	T_PHI							{ 	sm_get_const(SM_CONST_PHI, &$$);	}

	|	ld_expr T_ADD ll_10_expr		{	sm_calc_add_f($1, (long double)$3, &$$);	}
	|	ld_expr T_SUB ll_10_expr		{	sm_calc_sub_f($1, (long double)$3, &$$);	}
	|	ld_expr T_MUL ll_10_expr		{	sm_calc_mul_f($1, (long double)$3, &$$);	}

	|	ll_10_expr T_ADD ld_expr		{	sm_calc_add_f((long double)$1, $3, &$$);	}
	|	ll_10_expr T_SUB ld_expr		{	sm_calc_sub_f((long double)$1, $3, &$$);	}
	|	ll_10_expr T_MUL ld_expr		{	sm_calc_mul_f((long double)$1, $3, &$$);	}

	|	ld_expr T_ADD ld_expr			{	sm_calc_add_f($1, $3, &$$);	}
	|	ld_expr T_SUB ld_expr			{	sm_calc_sub_f($1, $3, &$$);	}
	|	ld_expr T_MUL ld_expr			{	sm_calc_mul_f($1, $3, &$$);	}

	|	ll_10_expr T_DIV ll_10_expr		{	sm_calc_div((long double)$1, (long double)$3, &$$);	}
	|	ld_expr T_DIV ll_10_expr		{	sm_calc_div($1, (long double)$3, &$$);			}
	|	ll_10_expr T_DIV ld_expr		{	sm_calc_div((long double)$1, $3, &$$);			}
	|	ld_expr T_DIV ld_expr			{	sm_calc_div($1, $3, &$$);	}

	|	T_SUB ld_expr %prec T_UMINUS {	$$ = (-1.0) * $2;  }

	|	T_SIN T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_sin((long double)$3, &$$);	}
	|	T_COS T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_cos((long double)$3, &$$);	}
	|	T_TAN T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_tan((long double)$3, &$$);	}

	|	T_ASIN T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_asin((long double)$3, &$$);	}
	|	T_ACOS T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_acos((long double)$3, &$$);	}
	|	T_ATAN T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_atan((long double)$3, &$$);	}

	|	T_SIN T_L_BRACK ld_expr	T_R_BRACK	{	sm_calc_sin($3, &$$); }
	|	T_COS T_L_BRACK ld_expr	T_R_BRACK	{	sm_calc_cos($3, &$$); }
	|	T_TAN T_L_BRACK ld_expr	T_R_BRACK	{	sm_calc_tan($3, &$$); }

	|	T_ASIN T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_asin($3, &$$);	}
	|	T_ACOS T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_acos($3, &$$);	}
	|	T_ATAN T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_atan($3, &$$); 	}

	|	T_GRAD_RAD T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_grad_2_rad((long double)$3, &$$);	}
	|	T_RAD_GRAD T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_rad_2_grad((long double)$3, &$$);	}

	|	T_RAD_DEG T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_rad_2_deg((long double)$3, &$$);	}
	|	T_DEG_RAD T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_deg_2_rad((long double)$3, &$$);	}

	|	T_GRAD_DEG T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_grad_2_deg((long double)$3, &$$);	}
	|	T_DEG_GRAD T_L_BRACK ll_10_expr T_R_BRACK	{	sm_conv_deg_2_grad((long double)$3, &$$);	}

	|	T_GRAD_RAD T_L_BRACK ld_expr T_R_BRACK	{	sm_conv_grad_2_rad((long double)$3, &$$);	}
	|	T_RAD_GRAD T_L_BRACK ld_expr T_R_BRACK	{	sm_conv_rad_2_grad((long double)$3, &$$);	}

	|	T_RAD_DEG T_L_BRACK ld_expr T_R_BRACK	{	sm_conv_rad_2_deg((long double)$3, &$$);	}
	|	T_DEG_RAD T_L_BRACK ld_expr T_R_BRACK	{	sm_conv_deg_2_rad((long double)$3, &$$);	}

	|	T_GRAD_DEG T_L_BRACK ld_expr T_R_BRACK	{	sm_conv_grad_2_deg((long double)$3, &$$);	}
	|	T_DEG_GRAD T_L_BRACK ld_expr T_R_BRACK	{	sm_conv_deg_2_grad((long double)$3, &$$);	}
	/* "pow" - T_POWER */
	|	T_POWER T_L_BRACK ld_expr T_COMMA ld_expr T_R_BRACK 		{ sm_calc_power($3, $5, &$$); }
	|	T_POWER T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK 	{ sm_calc_power((long double)$3, (long double)$5, &$$); }
	|	T_POWER T_L_BRACK ld_expr T_COMMA ll_10_expr T_R_BRACK 		{ sm_calc_power($3, (long double)$5, &$$); }
	|	T_POWER T_L_BRACK ll_10_expr T_COMMA ld_expr T_R_BRACK 		{ sm_calc_power((long double)$3, $5, &$$); }
	/* "^2" - T_POWER_2_X */
	|	T_POWER_X_2 	T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_power_d_2($3, &$$);	}
	/* "exp^" - T_POWER_EXP_X */
	|	T_POWER_EXP_X 	T_L_BRACK ld_expr T_R_BRACK		{	sm_calc_power_exp_d($3, &$$);	}
	|	T_POWER_EXP_X 	T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_power_exp_d((long double)$3, &$$);	}
	/* 	"10^" - T_POWER_10_X */
	|	T_POWER_10_X 	T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_power_10_d($3, &$$);	}
	/* 	"2^"  - T_POWER_2_X */
	|	T_POWER_2_X 	T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_power_2_d($3, &$$);	}

	|	T_SQRT T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_sqrt((long double)$3, &$$);	}
	|	T_SQRT T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_sqrt($3, &$$);			}

	|	T_LN T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_ln((long double)$3, &$$);	}
	|	T_LN T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_ln($3, &$$);			}

	|	T_LG T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_lg((long double)$3, &$$);	}
	|	T_LG T_L_BRACK ld_expr T_R_BRACK	{	sm_calc_lg($3, &$$);			}

	|	T_LOG T_L_BRACK ll_10_expr T_COMMA ll_10_expr T_R_BRACK	{	sm_calc_log((long double)$3, (long double)$5, &$$);	}
	|	T_LOG T_L_BRACK ld_expr T_COMMA ll_10_expr T_R_BRACK		{	sm_calc_log($3, (long double)$5, &$$);		}
	|	T_LOG T_L_BRACK ll_10_expr T_COMMA ld_expr T_R_BRACK		{	sm_calc_log((long double)$3, $5, &$$);		}
	|	T_LOG T_L_BRACK ld_expr T_COMMA ld_expr T_R_BRACK			{	sm_calc_log($3, $5, &$$);					}

	|	T_RECIP T_L_BRACK ll_10_expr T_R_BRACK	{	sm_calc_recip((long double)$3, &$$);	}
	|	T_RECIP T_L_BRACK ld_expr T_R_BRACK		{	sm_calc_recip($3, &$$);					}

	|	T_ABS T_L_BRACK ld_expr T_R_BRACK		{	sm_calc_abs_f($3, &$$);	}

	|	T_RAND	{ sm_get_rand(&$$);	}
;

%%
