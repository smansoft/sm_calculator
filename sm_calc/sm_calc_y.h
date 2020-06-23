/* A Bison parser, made by GNU Bison 3.5.0.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_E_SMAN_GIT_CUR_W_COMPILERS_SRC_INV_SM_CALCULATOR_SM_CALC_SM_CALC_Y_H_INCLUDED
# define YY_YY_E_SMAN_GIT_CUR_W_COMPILERS_SRC_INV_SM_CALCULATOR_SM_CALC_SM_CALC_Y_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_LL_10 = 258,
    T_LL_8 = 259,
    T_LL_16 = 260,
    T_LL_2 = 261,
    T_F_PRECISION = 262,
    T_LD_N = 263,
    T_LD_E = 264,
    T_PI = 265,
    T_EXP = 266,
    T_GAMMA = 267,
    T_PHI = 268,
    T_SZ = 269,
    T_NEWLINE = 270,
    T_STOP = 271,
    T_CONFIG = 272,
    T_QUIT = 273,
    T_EXIT = 274,
    T_HELP = 275,
    T_ECHO = 276,
    T_A_UNIT = 277,
    T_I_FORMAT = 278,
    T_AND = 279,
    T_OR = 280,
    T_NOT = 281,
    T_NAND = 282,
    T_NOR = 283,
    T_XOR = 284,
    T_RAND = 285,
    T_ABS = 286,
    T_MOD = 287,
    T_MOD_2 = 288,
    T_SQRT = 289,
    T_LN = 290,
    T_LG = 291,
    T_LOG = 292,
    T_RECIP = 293,
    T_POWER = 294,
    T_POWER_X_2 = 295,
    T_POWER_EXP_X = 296,
    T_POWER_10_X = 297,
    T_POWER_2_X = 298,
    T_L_ASHIFT = 299,
    T_R_ASHIFT = 300,
    T_L_LSHIFT = 301,
    T_R_LSHIFT = 302,
    T_L_CSHIFT = 303,
    T_R_CSHIFT = 304,
    T_SIN = 305,
    T_COS = 306,
    T_TAN = 307,
    T_ASIN = 308,
    T_ACOS = 309,
    T_ATAN = 310,
    T_DEG = 311,
    T_GRAD = 312,
    T_RAD = 313,
    T_GRAD_RAD = 314,
    T_RAD_GRAD = 315,
    T_RAD_DEG = 316,
    T_DEG_RAD = 317,
    T_GRAD_DEG = 318,
    T_DEG_GRAD = 319,
    T_DEC_OCT = 320,
    T_DEC_HEX = 321,
    T_DEC_BIN = 322,
    T_OCT_DEC = 323,
    T_HEX_DEC = 324,
    T_BIN_DEC = 325,
    T_DEC = 326,
    T_OCT = 327,
    T_HEX = 328,
    T_BIN = 329,
    T_COMMA = 330,
    T_FACTORIAL = 331,
    T_L_BRACK = 332,
    T_R_BRACK = 333,
    T_UMINUS = 334,
    T_ADD = 335,
    T_SUB = 336,
    T_MUL = 337,
    T_DIV = 338,
    T_ASSIGN = 339
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{

	long long 		ll_val;
	long double 	ld_val;
	char			sz_val[SM_L_BUFF_SIZE];


};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_E_SMAN_GIT_CUR_W_COMPILERS_SRC_INV_SM_CALCULATOR_SM_CALC_SM_CALC_Y_H_INCLUDED  */
