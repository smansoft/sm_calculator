
%{

/*
 *    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
 *    Sergey Manoylo <info@smansoft.com>
 */

#include "sm_calc_proc.h"
#include "sm_calc_y.h"

#if defined SM_OS_WINDOWS

#define fileno _fileno
#define isatty _isatty

#endif

YY_BUFFER_STATE gsm_string_buffer;

int gsm_string_buffer_active;

%}

%option noyywrap

%%

([1-9]{1}[0-9]*)|(0{1})  	{
			long long val;
			errno_t err = sm_conv_sz_2_i_dec(SM_I_10, yytext, SM_L_BUFF_SIZE, &val);
			if(err == SM_RES_OK)
 				yylval.ll_val = val;
			return T_LL_10; 
	}

0[0-7]+	{
			long long val;
			errno_t err = sm_conv_sz_2_i_dec(SM_I_8, yytext, SM_L_BUFF_SIZE, &val);
			if(err == SM_RES_OK)
 				yylval.ll_val = val;
			return T_LL_8; 
	}

0x[0-9a-fA-F]+	{
			long long val;
			errno_t err = sm_conv_sz_2_i_dec(SM_I_16, yytext, SM_L_BUFF_SIZE, &val);
			if(err == SM_RES_OK)
 				yylval.ll_val = val;
			return T_LL_16; 
	}

b[0-1]+ {
			long long val;
			errno_t err = sm_conv_sz_2_i_dec(SM_I_2, yytext, SM_S_BUFF_SIZE+2, &val);
			if(err == SM_RES_OK)
 				yylval.ll_val = val;
			return T_LL_2; 
	}

([0-9]+[.]{1}[0-9]*)|([0-9]*[.]{1}[0-9]+) {	
			long double val;
			errno_t err = sm_conv_sz_2_f_dec(yytext, SM_L_BUFF_SIZE, &val);
			if(err == SM_RES_OK)
 				yylval.ld_val = val;
			return T_LD_N; 
	}

(([0-9]+[.]{1}[0-9]*)|([0-9]*[.]{0,1}[0-9]+))([e|E]{1}[+|-]*[0-9]+) {
			long double val;
			errno_t err = sm_conv_sz_2_f_dec(yytext, SM_L_BUFF_SIZE, &val);
			if(err == SM_RES_OK)
 				yylval.ld_val = val;
			return T_LD_E; 
	}

(\".*\")|(\'.*\') {
			errno_t err = sm_conv_quot_sz_2_sz(yytext, yylval.sz_val, SM_L_BUFF_SIZE);
			if(err == SM_RES_ERROR)
				yylval.sz_val[0] = '\0';
			return T_SZ; 
	}

[ \t]+  		{ ; }

"+"    		{ return T_ADD; }
"-"    		{ return T_SUB; }
"*"    		{ return T_MUL; }
"/"    		{ return T_DIV; }

"="    		{ return T_ASSIGN; }

"f_precision" 		{ return T_F_PRECISION; }
"i_format" 			{ return T_I_FORMAT; }
"a_unit" 			{ return T_A_UNIT; }

"pi"		{	return T_PI;	}
"exp"		{ 	return T_EXP; }
"gamma"		{ 	return T_GAMMA; }
"phi"		{ 	return T_PHI; }

"rad"		{	return T_RAD;	}
"deg"   	{	return T_DEG;	}
"grad"		{	return T_GRAD;	}

"grad2rad"		{	return T_GRAD_RAD;	}
"rad2grad"		{	return T_RAD_GRAD;	}

"rad2deg"		{	return T_RAD_DEG;	}
"deg2rad"		{	return T_DEG_RAD;	}

"grad2deg"		{	return T_GRAD_DEG;	}
"deg2grad"		{	return T_DEG_GRAD;	}

"dec"		{	return T_DEC;	}
"oct"   	{	return T_OCT;	}
"hex"		{	return T_HEX;	}
"bin"		{	return T_BIN;	}

"dec2oct"		{	return T_DEC_OCT;	}
"dec2hex"		{	return T_DEC_HEX;	}
"dec2bin"		{	return T_DEC_BIN;	}

"oct2dec"		{	return T_OCT_DEC;	}
"hex2dec"		{	return T_HEX_DEC;	}
"bin2dec"		{	return T_BIN_DEC;	}

"sin"  			{ return T_SIN; }
"cos"  			{ return T_COS; }
"tan"  			{ return T_TAN; }

"asin"  		{ return T_ASIN; }
"acos"  		{ return T_ACOS; }
"atan"  		{ return T_ATAN; }

"!"				{ return T_FACTORIAL; }

"sqrt"			{ return T_SQRT; }
"ln"			{ return T_LN; }
"lg"			{ return T_LG; }
"log"			{ return T_LOG; }

"inv"			{ return T_RECIP; }

"pow"			{ return T_POWER; }

"pow_exp^"		{ return T_POWER_EXP_X; }
"pow_10^"		{ return T_POWER_10_X; }
"pow_2^"		{ return T_POWER_2_X; }
"pow_^2"		{ return T_POWER_X_2; }

"abs"			{ return T_ABS; }
"mod"			{ return T_MOD; }
"mod2"			{ return T_MOD_2; }

"rand"			{ return T_RAND; }

"<a<"			{ return T_L_ASHIFT; }
">a>"			{ return T_R_ASHIFT; }

"<l<"			{ return T_L_LSHIFT; }
">l>"			{ return T_R_LSHIFT; }

"<c<"			{ return T_L_CSHIFT; }
">c>"			{ return T_R_CSHIFT; }

"and"			{ return T_AND; }
"or"			{ return T_OR; }
"not"			{ return T_NOT; }
"nand"			{ return T_NAND; }
"nor"			{ return T_NOR; }
"xor"			{ return T_XOR; }

","    			{ return T_COMMA; }

"("    			{ return T_L_BRACK; }
")"    			{ return T_R_BRACK; }

"echo"  		{ return T_ECHO; }

"config"  		{ return T_CONFIG; }
"help"  		{ return T_HELP; }

";"     		{ return T_STOP; }
"\n"    		{ return T_NEWLINE; }
"quit"  		{ return T_QUIT; }
"exit"  		{ return T_EXIT; }

.       		{ return yytext[0]; }

%%

void sm_scan_string(const char* const in_str)
{
    YY_FLUSH_BUFFER;
    if (gsm_string_buffer_active)
        yy_delete_buffer(gsm_string_buffer);
    gsm_string_buffer = yy_scan_string(in_str);
    gsm_string_buffer_active = 1;
}

void sm_scan_file(FILE* const in_f)
{
    YY_FLUSH_BUFFER;
    if (gsm_string_buffer_active)
        yy_delete_buffer(gsm_string_buffer);
    yyrestart(in_f);
    gsm_string_buffer_active = 0;
}

int sm_get_buff_info(sm_l_buff_info* l_buff_info)
{
    int res;
    YY_BUFFER_STATE curr_buf = YY_CURRENT_BUFFER;
    if(curr_buf) {
        l_buff_info->yy_ch_buf = curr_buf->yy_ch_buf;
        l_buff_info->yy_buf_pos = curr_buf->yy_buf_pos;

        l_buff_info->yy_buf_size = curr_buf->yy_buf_size;
        l_buff_info->yy_n_chars = curr_buf->yy_n_chars;

        res = SM_RES_OK;
    }
    else
        res = SM_RES_ERROR;
    return res;
}
