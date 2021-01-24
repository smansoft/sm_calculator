/* A Bison parser, made by GNU Bison 3.5.0.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.0"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1





# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_G_SMAN_GIT_CUR_W_SM_CALCULATOR_SRC_MAIN_SM_CALCULATOR_SM_CALC_SM_CALC_Y_H_INCLUDED
# define YY_YY_G_SMAN_GIT_CUR_W_SM_CALCULATOR_SRC_MAIN_SM_CALCULATOR_SM_CALC_SM_CALC_Y_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */


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



int yyparse (yyscan_t scanner, parser_callback_t parser_callback);
/* "%code provides" blocks.  */


	extern int yylex (YYSTYPE * yylval_param , yyscan_t yyscanner);



#endif /* !YY_YY_G_SMAN_GIT_CUR_W_SM_CALCULATOR_SRC_MAIN_SM_CALCULATOR_SM_CALC_SM_CALC_Y_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  131
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1342

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  85
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  9
/* YYNRULES -- Number of rules.  */
#define YYNRULES  155
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  377

#define YYUNDEFTOK  2
#define YYMAXUTOK   339


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   106,   106,   107,   111,   113,   114,   115,   117,   119,
     120,   122,   123,   124,   126,   128,   129,   131,   132,   133,
     135,   137,   138,   140,   141,   142,   144,   146,   147,   151,
     153,   155,   156,   158,   160,   161,   162,   164,   165,   166,
     168,   170,   171,   172,   173,   175,   176,   177,   178,   180,
     181,   182,   184,   185,   186,   188,   192,   194,   195,   196,
     198,   200,   201,   202,   204,   206,   208,   210,   212,   214,
     216,   217,   219,   220,   222,   223,   225,   227,   228,   230,
     231,   232,   233,   234,   235,   239,   243,   247,   251,   252,
     254,   256,   257,   258,   259,   261,   262,   263,   265,   266,
     267,   269,   270,   271,   273,   274,   275,   276,   278,   280,
     281,   282,   284,   285,   286,   288,   289,   290,   292,   293,
     294,   296,   297,   299,   300,   302,   303,   305,   306,   308,
     309,   311,   312,   314,   315,   316,   317,   319,   321,   322,
     324,   326,   328,   329,   331,   332,   334,   335,   337,   338,
     339,   340,   342,   343,   345,   347
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_LL_10", "T_LL_8", "T_LL_16", "T_LL_2",
  "T_F_PRECISION", "T_LD_N", "T_LD_E", "T_PI", "T_EXP", "T_GAMMA", "T_PHI",
  "T_SZ", "T_NEWLINE", "T_STOP", "T_CONFIG", "T_QUIT", "T_EXIT", "T_HELP",
  "T_ECHO", "T_A_UNIT", "T_I_FORMAT", "T_AND", "T_OR", "T_NOT", "T_NAND",
  "T_NOR", "T_XOR", "T_RAND", "T_ABS", "T_MOD", "T_MOD_2", "T_SQRT",
  "T_LN", "T_LG", "T_LOG", "T_RECIP", "T_POWER", "T_POWER_X_2",
  "T_POWER_EXP_X", "T_POWER_10_X", "T_POWER_2_X", "T_L_ASHIFT",
  "T_R_ASHIFT", "T_L_LSHIFT", "T_R_LSHIFT", "T_L_CSHIFT", "T_R_CSHIFT",
  "T_SIN", "T_COS", "T_TAN", "T_ASIN", "T_ACOS", "T_ATAN", "T_DEG",
  "T_GRAD", "T_RAD", "T_GRAD_RAD", "T_RAD_GRAD", "T_RAD_DEG", "T_DEG_RAD",
  "T_GRAD_DEG", "T_DEG_GRAD", "T_DEC_OCT", "T_DEC_HEX", "T_DEC_BIN",
  "T_OCT_DEC", "T_HEX_DEC", "T_BIN_DEC", "T_DEC", "T_OCT", "T_HEX",
  "T_BIN", "T_COMMA", "T_FACTORIAL", "T_L_BRACK", "T_R_BRACK", "T_UMINUS",
  "T_ADD", "T_SUB", "T_MUL", "T_DIV", "T_ASSIGN", "$accept", "calc_run",
  "calc_res", "sz_expr", "ll_10_expr", "ll_8_expr", "ll_16_expr",
  "ll_2_expr", "ld_expr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339
};
# endif

#define YYPACT_NINF (-80)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     235,   -80,   -80,   -80,   -80,   -79,   -80,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,   -80,     1,     3,     5,   -55,   -26,
     -12,     7,    87,    94,   102,   117,   118,   -80,   125,   126,
     128,   129,   134,   141,   145,   146,   148,   149,   150,   153,
     174,   203,   233,   234,   236,   237,   238,   -80,   -80,   -80,
     242,   243,   258,   263,   264,   265,   266,   287,   299,   300,
     301,   308,   -80,   -80,   -80,   -80,   320,   320,    99,   235,
      91,   100,   -80,   -80,   -80,    78,    54,    80,   -80,   -80,
     127,   -80,   -80,   135,   -80,   -80,   389,    83,    37,   362,
     362,   362,   362,   362,   362,   320,   362,   362,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   320,   320,
     362,   362,   362,   181,   182,   198,   314,    22,   201,   -80,
     -80,   -80,   -80,   147,   -80,   -80,   154,   -80,   -80,   362,
     362,   362,   362,   362,   362,   -80,   320,   320,   320,   320,
     162,   -80,   -80,   320,   320,   320,   320,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,   -80,   315,   151,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,   323,   330,   331,   332,   362,   362,
     812,   828,   835,   851,   874,   890,   108,   256,   898,   913,
     388,   335,   404,   341,   428,   347,   444,   -72,   452,  1127,
     468,   -68,   492,  1145,   508,  1151,   516,  1157,   532,  1163,
     556,  1169,   572,  1175,   580,  1181,   596,  1187,   620,  1193,
     636,  1199,   644,  1205,   660,  1211,   684,  1217,   700,  1223,
     708,  1229,   724,  1235,   937,   952,   959,   336,   342,   348,
     209,   -80,   -80,   -80,   -80,   -80,   -80,   155,   155,   155,
     155,   155,   155,   152,   110,   152,   110,  1146,   -80,  1146,
     -80,   -80,   -80,   152,   110,   152,   110,  1146,   -80,  1146,
     -80,   381,   -80,   362,   362,   362,   362,   976,   -80,   362,
     362,   362,   362,   362,   -80,   362,   362,   362,   -80,   -80,
     362,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   320,   320,
     -80,   -80,   320,   320,   -80,   -80,   -80,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
     -80,   -80,   998,  1015,  1022,  1037,  1061,    15,    15,  1146,
    1076,  1083,  1100,  1122,  1139,   748,  1241,   764,  1247,   772,
    1253,   788,  1259,   -80,   -80,   -80,   -80,   -80,   -80,   -80,
     -80,   -80,   -80,   -80,   -80,   -80,   -80
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    56,    85,    86,    87,    31,    88,    89,    91,    92,
      93,    94,    29,     4,    30,     0,     0,     0,     0,    33,
      40,     0,     0,     0,     0,     0,     0,   155,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    38,    39,    37,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    45,    46,    47,    48,     0,     0,     0,     3,
       0,     0,    57,    58,    59,     0,     0,     0,    15,     9,
       0,    16,    10,     0,    14,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
     108,     1,     2,     0,    13,     7,     0,    11,     5,     0,
       0,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,    12,     6,     0,     0,     0,     0,    65,    32,    27,
      21,    28,    22,    26,    20,    31,     0,    35,    36,    34,
      41,    42,    43,    44,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    90,    25,    19,    23,    17,    70,    71,    72,
      73,    74,    75,    61,    98,    62,    99,    63,   100,   104,
     106,    24,    18,    95,   101,    96,   102,    97,   103,   105,
     107,     0,    55,     0,     0,     0,     0,     0,    64,     0,
       0,     0,     0,     0,    81,     0,     0,     0,    76,   154,
       0,    78,   142,   143,   144,   145,   146,   147,     0,     0,
     152,   153,     0,     0,    67,   137,   139,   138,    68,   140,
      69,   141,   109,   115,   110,   116,   111,   117,   112,   118,
     113,   119,   114,   120,   121,   127,   122,   128,   123,   129,
     124,   130,   125,   131,   126,   132,    49,    50,    51,    52,
      53,    54,     0,     0,     0,     0,     0,    61,    62,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    79,    80,    82,    83,    84,    77,   148,
     150,   149,   151,   134,   136,   135,   133
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -80,   369,   -80,   345,   -66,   317,   318,   316,    19
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    68,    69,    70,    71,    72,    73,    74,    75
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     127,   129,    77,   299,    80,    76,    83,   303,   153,   154,
     155,   156,   153,   154,   155,   156,    78,    79,    81,    82,
      84,    85,    86,   180,   181,   182,   183,   184,   185,   186,
     188,   189,   190,   192,   194,   196,   198,   200,   202,   204,
     206,   208,   210,   212,   214,   216,   218,   220,   222,   224,
     226,   228,   230,   232,   234,   235,   236,   157,    87,   139,
     140,   141,   142,   143,   144,   158,   139,   140,   141,   142,
     143,   144,    88,   247,   248,   249,   250,   251,   252,   150,
     253,   255,   257,   259,    89,   128,   130,   263,   265,   267,
     269,   145,   133,   151,   152,   159,   160,   282,   145,   131,
     241,   136,   146,   147,   148,   149,   134,   135,   170,   171,
     172,   173,   277,   278,   187,   137,   138,   191,   193,   195,
     197,   199,   201,   203,   205,   207,   209,   211,   213,   215,
     217,   219,   221,   223,   225,   227,   229,   231,   233,   167,
     168,   169,   161,   162,   139,   140,   141,   142,   143,   144,
     163,   164,   139,   140,   141,   142,   143,   144,   153,   154,
     155,   156,   243,   244,    90,   254,   256,   258,   260,   245,
     246,    91,   264,   266,   268,   270,   145,   261,   262,    92,
     146,   147,   148,   149,   145,     2,   288,     3,   146,   147,
     148,   149,   155,   156,    93,    94,   139,   140,   141,   142,
     143,   144,    95,    96,     4,    97,    98,   342,   343,   344,
     345,    99,   157,   346,   347,   348,   349,   350,   100,   351,
     352,   353,   101,   102,   354,   103,   104,   105,   145,   272,
     106,   145,   355,   357,   148,   149,   359,   361,     1,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,   107,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,   242,
     108,   153,   154,   155,   156,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
     109,   110,    66,   111,   112,   113,    67,   356,   358,   114,
     115,   360,   362,     1,     2,     3,     4,   126,     6,     7,
       8,     9,    10,    11,   289,   116,   153,   154,   155,   156,
     117,   118,   119,   120,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,   121,     1,     2,     3,     4,   126,
      41,    42,    43,    44,    45,    46,   122,   123,   124,    50,
      51,    52,    53,    54,    55,   125,    21,    22,    23,    24,
      25,    26,   158,   174,    29,    30,   165,    66,   240,   271,
     273,    67,   175,    12,   176,   177,    14,   274,   275,   276,
      18,    19,    20,   293,   339,   153,   154,   155,   156,   295,
     340,   153,   154,   155,   156,   297,   341,   153,   154,   155,
     156,   166,   139,   140,   141,   142,   143,   144,   132,   178,
     237,   239,   238,   179,     0,    47,    48,    49,   139,   140,
     141,   142,   143,   144,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,   145,     0,   292,     0,   146,   147,
     148,   149,   139,   140,   141,   142,   143,   144,     0,     0,
     145,     0,   294,     0,   146,   147,   148,   149,   139,   140,
     141,   142,   143,   144,     0,     0,   139,   140,   141,   142,
     143,   144,     0,     0,   145,     0,   296,     0,   146,   147,
     148,   149,   139,   140,   141,   142,   143,   144,     0,   298,
     145,     0,     0,     0,   146,   147,   148,   149,   145,     0,
     300,     0,   146,   147,   148,   149,   139,   140,   141,   142,
     143,   144,     0,   302,   145,     0,     0,     0,   146,   147,
     148,   149,   139,   140,   141,   142,   143,   144,     0,     0,
     139,   140,   141,   142,   143,   144,     0,     0,   145,     0,
     304,     0,   146,   147,   148,   149,   139,   140,   141,   142,
     143,   144,     0,     0,   145,     0,   306,     0,   146,   147,
     148,   149,   145,     0,   308,     0,   146,   147,   148,   149,
     139,   140,   141,   142,   143,   144,     0,     0,   145,     0,
     310,     0,   146,   147,   148,   149,   139,   140,   141,   142,
     143,   144,     0,     0,   139,   140,   141,   142,   143,   144,
       0,     0,   145,     0,   312,     0,   146,   147,   148,   149,
     139,   140,   141,   142,   143,   144,     0,     0,   145,     0,
     314,     0,   146,   147,   148,   149,   145,     0,   316,     0,
     146,   147,   148,   149,   139,   140,   141,   142,   143,   144,
       0,     0,   145,     0,   318,     0,   146,   147,   148,   149,
     139,   140,   141,   142,   143,   144,     0,     0,   139,   140,
     141,   142,   143,   144,     0,     0,   145,     0,   320,     0,
     146,   147,   148,   149,   139,   140,   141,   142,   143,   144,
       0,     0,   145,     0,   322,     0,   146,   147,   148,   149,
     145,     0,   324,     0,   146,   147,   148,   149,   139,   140,
     141,   142,   143,   144,     0,     0,   145,     0,   326,     0,
     146,   147,   148,   149,   139,   140,   141,   142,   143,   144,
       0,     0,   139,   140,   141,   142,   143,   144,     0,     0,
     145,     0,   328,     0,   146,   147,   148,   149,   139,   140,
     141,   142,   143,   144,     0,     0,   145,     0,   330,     0,
     146,   147,   148,   149,   145,     0,   332,     0,   146,   147,
     148,   149,   139,   140,   141,   142,   143,   144,     0,     0,
     145,     0,   334,     0,   146,   147,   148,   149,   139,   140,
     141,   142,   143,   144,     0,     0,   139,   140,   141,   142,
     143,   144,     0,     0,   145,     0,   369,     0,   146,   147,
     148,   149,   139,   140,   141,   142,   143,   144,     0,     0,
     145,     0,   371,     0,   146,   147,   148,   149,   145,     0,
     373,     0,   146,   147,   148,   149,   139,   140,   141,   142,
     143,   144,     0,     0,   145,     0,   375,     0,   146,   147,
     148,   149,   139,   140,   141,   142,   143,   144,     0,   139,
     140,   141,   142,   143,   144,     0,     0,   279,   145,     0,
       0,     0,   280,   281,   282,   139,   140,   141,   142,   143,
     144,     0,     0,   283,   145,     0,     0,     0,   280,   281,
     282,   145,     0,   284,     0,   280,   281,   282,   139,   140,
     141,   142,   143,   144,     0,     0,   285,   145,     0,     0,
       0,   280,   281,   282,   139,   140,   141,   142,   143,   144,
       0,     0,   139,   140,   141,   142,   143,   144,     0,   286,
     145,     0,     0,     0,   280,   281,   282,   139,   140,   141,
     142,   143,   144,     0,     0,   287,   145,     0,     0,     0,
     280,   281,   282,   290,   145,     0,     0,     0,   280,   281,
     282,   139,   140,   141,   142,   143,   144,     0,     0,   145,
       0,   291,     0,   280,   281,   282,   139,   140,   141,   142,
     143,   144,     0,   139,   140,   141,   142,   143,   144,     0,
       0,     0,     0,   145,     0,   336,     0,   280,   281,   282,
     139,   140,   141,   142,   143,   144,     0,     0,   145,     0,
     337,     0,   280,   281,   282,   145,     0,   338,     0,   280,
     281,   282,   139,   140,   141,   142,   143,   144,     0,     0,
       0,     0,   145,     0,   241,     0,   280,   281,   282,   139,
     140,   141,   142,   143,   144,     0,   139,   140,   141,   142,
     143,   144,     0,     0,   145,     0,   288,     0,   280,   281,
     282,   139,   140,   141,   142,   143,   144,     0,     0,     0,
       0,   145,     0,   304,     0,   280,   281,   282,   145,     0,
     308,     0,   280,   281,   282,   139,   140,   141,   142,   143,
     144,     0,     0,   145,     0,   310,     0,   280,   281,   282,
     139,   140,   141,   142,   143,   144,     0,   139,   140,   141,
     142,   143,   144,     0,     0,     0,     0,   145,     0,   363,
       0,   280,   281,   282,   139,   140,   141,   142,   143,   144,
       0,     0,   145,     0,   364,     0,   280,   281,   282,   145,
       0,   365,     0,   280,   281,   282,   139,   140,   141,   142,
     143,   144,     0,     0,     0,     0,   145,     0,   366,     0,
     280,   281,   282,   139,   140,   141,   142,   143,   144,     0,
     139,   140,   141,   142,   143,   144,     0,     0,   145,     0,
     367,     0,   280,   281,   282,   301,     0,   153,   154,   155,
     156,     0,     0,     0,     0,   145,     0,   368,     0,   280,
     281,   282,   145,   305,     0,   153,   154,   155,   156,   307,
       0,   153,   154,   155,   156,   309,     0,   153,   154,   155,
     156,   311,     0,   153,   154,   155,   156,   313,     0,   153,
     154,   155,   156,   315,     0,   153,   154,   155,   156,   317,
       0,   153,   154,   155,   156,   319,     0,   153,   154,   155,
     156,   321,     0,   153,   154,   155,   156,   323,     0,   153,
     154,   155,   156,   325,     0,   153,   154,   155,   156,   327,
       0,   153,   154,   155,   156,   329,     0,   153,   154,   155,
     156,   331,     0,   153,   154,   155,   156,   333,     0,   153,
     154,   155,   156,   335,     0,   153,   154,   155,   156,   370,
       0,   153,   154,   155,   156,   372,     0,   153,   154,   155,
     156,   374,     0,   153,   154,   155,   156,   376,     0,   153,
     154,   155,   156
};

static const yytype_int16 yycheck[] =
{
      66,    67,     1,    75,     1,    84,     1,    75,    80,    81,
      82,    83,    80,    81,    82,    83,    15,    16,    15,    16,
      15,    16,    77,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,     3,    84,    44,
      45,    46,    47,    48,    49,    11,    44,    45,    46,    47,
      48,    49,    84,   139,   140,   141,   142,   143,   144,     1,
     146,   147,   148,   149,    77,    66,    67,   153,   154,   155,
     156,    76,     1,    15,    16,    15,    16,    82,    76,     0,
      78,     1,    80,    81,    82,    83,    15,    16,    71,    72,
      73,    74,   178,   179,    95,    15,    16,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    56,
      57,    58,    15,    16,    44,    45,    46,    47,    48,    49,
      15,    16,    44,    45,    46,    47,    48,    49,    80,    81,
      82,    83,    15,    16,    77,   146,   147,   148,   149,    15,
      16,    77,   153,   154,   155,   156,    76,    15,    16,    77,
      80,    81,    82,    83,    76,     4,    78,     5,    80,    81,
      82,    83,    82,    83,    77,    77,    44,    45,    46,    47,
      48,    49,    77,    77,     6,    77,    77,   273,   274,   275,
     276,    77,     3,   279,   280,   281,   282,   283,    77,   285,
     286,   287,    77,    77,   290,    77,    77,    77,    76,    78,
      77,    76,   298,   299,    82,    83,   302,   303,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    77,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    78,
      77,    80,    81,    82,    83,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      77,    77,    77,    77,    77,    77,    81,   298,   299,    77,
      77,   302,   303,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    78,    77,    80,    81,    82,    83,
      77,    77,    77,    77,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    77,     3,     4,     5,     6,     7,
      50,    51,    52,    53,    54,    55,    77,    77,    77,    59,
      60,    61,    62,    63,    64,    77,    24,    25,    26,    27,
      28,    29,    11,    31,    32,    33,     7,    77,    84,    84,
      77,    81,    40,    14,    42,    43,    17,    77,    77,    77,
      21,    22,    23,    78,    78,    80,    81,    82,    83,    78,
      78,    80,    81,    82,    83,    78,    78,    80,    81,    82,
      83,    86,    44,    45,    46,    47,    48,    49,    69,    77,
     123,   125,   124,    81,    -1,    56,    57,    58,    44,    45,
      46,    47,    48,    49,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    76,    -1,    78,    -1,    80,    81,
      82,    83,    44,    45,    46,    47,    48,    49,    -1,    -1,
      76,    -1,    78,    -1,    80,    81,    82,    83,    44,    45,
      46,    47,    48,    49,    -1,    -1,    44,    45,    46,    47,
      48,    49,    -1,    -1,    76,    -1,    78,    -1,    80,    81,
      82,    83,    44,    45,    46,    47,    48,    49,    -1,    75,
      76,    -1,    -1,    -1,    80,    81,    82,    83,    76,    -1,
      78,    -1,    80,    81,    82,    83,    44,    45,    46,    47,
      48,    49,    -1,    75,    76,    -1,    -1,    -1,    80,    81,
      82,    83,    44,    45,    46,    47,    48,    49,    -1,    -1,
      44,    45,    46,    47,    48,    49,    -1,    -1,    76,    -1,
      78,    -1,    80,    81,    82,    83,    44,    45,    46,    47,
      48,    49,    -1,    -1,    76,    -1,    78,    -1,    80,    81,
      82,    83,    76,    -1,    78,    -1,    80,    81,    82,    83,
      44,    45,    46,    47,    48,    49,    -1,    -1,    76,    -1,
      78,    -1,    80,    81,    82,    83,    44,    45,    46,    47,
      48,    49,    -1,    -1,    44,    45,    46,    47,    48,    49,
      -1,    -1,    76,    -1,    78,    -1,    80,    81,    82,    83,
      44,    45,    46,    47,    48,    49,    -1,    -1,    76,    -1,
      78,    -1,    80,    81,    82,    83,    76,    -1,    78,    -1,
      80,    81,    82,    83,    44,    45,    46,    47,    48,    49,
      -1,    -1,    76,    -1,    78,    -1,    80,    81,    82,    83,
      44,    45,    46,    47,    48,    49,    -1,    -1,    44,    45,
      46,    47,    48,    49,    -1,    -1,    76,    -1,    78,    -1,
      80,    81,    82,    83,    44,    45,    46,    47,    48,    49,
      -1,    -1,    76,    -1,    78,    -1,    80,    81,    82,    83,
      76,    -1,    78,    -1,    80,    81,    82,    83,    44,    45,
      46,    47,    48,    49,    -1,    -1,    76,    -1,    78,    -1,
      80,    81,    82,    83,    44,    45,    46,    47,    48,    49,
      -1,    -1,    44,    45,    46,    47,    48,    49,    -1,    -1,
      76,    -1,    78,    -1,    80,    81,    82,    83,    44,    45,
      46,    47,    48,    49,    -1,    -1,    76,    -1,    78,    -1,
      80,    81,    82,    83,    76,    -1,    78,    -1,    80,    81,
      82,    83,    44,    45,    46,    47,    48,    49,    -1,    -1,
      76,    -1,    78,    -1,    80,    81,    82,    83,    44,    45,
      46,    47,    48,    49,    -1,    -1,    44,    45,    46,    47,
      48,    49,    -1,    -1,    76,    -1,    78,    -1,    80,    81,
      82,    83,    44,    45,    46,    47,    48,    49,    -1,    -1,
      76,    -1,    78,    -1,    80,    81,    82,    83,    76,    -1,
      78,    -1,    80,    81,    82,    83,    44,    45,    46,    47,
      48,    49,    -1,    -1,    76,    -1,    78,    -1,    80,    81,
      82,    83,    44,    45,    46,    47,    48,    49,    -1,    44,
      45,    46,    47,    48,    49,    -1,    -1,    75,    76,    -1,
      -1,    -1,    80,    81,    82,    44,    45,    46,    47,    48,
      49,    -1,    -1,    75,    76,    -1,    -1,    -1,    80,    81,
      82,    76,    -1,    78,    -1,    80,    81,    82,    44,    45,
      46,    47,    48,    49,    -1,    -1,    75,    76,    -1,    -1,
      -1,    80,    81,    82,    44,    45,    46,    47,    48,    49,
      -1,    -1,    44,    45,    46,    47,    48,    49,    -1,    75,
      76,    -1,    -1,    -1,    80,    81,    82,    44,    45,    46,
      47,    48,    49,    -1,    -1,    75,    76,    -1,    -1,    -1,
      80,    81,    82,    75,    76,    -1,    -1,    -1,    80,    81,
      82,    44,    45,    46,    47,    48,    49,    -1,    -1,    76,
      -1,    78,    -1,    80,    81,    82,    44,    45,    46,    47,
      48,    49,    -1,    44,    45,    46,    47,    48,    49,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    80,    81,    82,
      44,    45,    46,    47,    48,    49,    -1,    -1,    76,    -1,
      78,    -1,    80,    81,    82,    76,    -1,    78,    -1,    80,
      81,    82,    44,    45,    46,    47,    48,    49,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    81,    82,    44,
      45,    46,    47,    48,    49,    -1,    44,    45,    46,    47,
      48,    49,    -1,    -1,    76,    -1,    78,    -1,    80,    81,
      82,    44,    45,    46,    47,    48,    49,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    81,    82,    76,    -1,
      78,    -1,    80,    81,    82,    44,    45,    46,    47,    48,
      49,    -1,    -1,    76,    -1,    78,    -1,    80,    81,    82,
      44,    45,    46,    47,    48,    49,    -1,    44,    45,    46,
      47,    48,    49,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    81,    82,    44,    45,    46,    47,    48,    49,
      -1,    -1,    76,    -1,    78,    -1,    80,    81,    82,    76,
      -1,    78,    -1,    80,    81,    82,    44,    45,    46,    47,
      48,    49,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    81,    82,    44,    45,    46,    47,    48,    49,    -1,
      44,    45,    46,    47,    48,    49,    -1,    -1,    76,    -1,
      78,    -1,    80,    81,    82,    78,    -1,    80,    81,    82,
      83,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      81,    82,    76,    78,    -1,    80,    81,    82,    83,    78,
      -1,    80,    81,    82,    83,    78,    -1,    80,    81,    82,
      83,    78,    -1,    80,    81,    82,    83,    78,    -1,    80,
      81,    82,    83,    78,    -1,    80,    81,    82,    83,    78,
      -1,    80,    81,    82,    83,    78,    -1,    80,    81,    82,
      83,    78,    -1,    80,    81,    82,    83,    78,    -1,    80,
      81,    82,    83,    78,    -1,    80,    81,    82,    83,    78,
      -1,    80,    81,    82,    83,    78,    -1,    80,    81,    82,
      83,    78,    -1,    80,    81,    82,    83,    78,    -1,    80,
      81,    82,    83,    78,    -1,    80,    81,    82,    83,    78,
      -1,    80,    81,    82,    83,    78,    -1,    80,    81,    82,
      83,    78,    -1,    80,    81,    82,    83,    78,    -1,    80,
      81,    82,    83
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    77,    81,    86,    87,
      88,    89,    90,    91,    92,    93,    84,     1,    15,    16,
       1,    15,    16,     1,    15,    16,    77,    84,    84,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,     7,    89,    93,    89,
      93,     0,    86,     1,    15,    16,     1,    15,    16,    44,
      45,    46,    47,    48,    49,    76,    80,    81,    82,    83,
       1,    15,    16,    80,    81,    82,    83,     3,    11,    15,
      16,    15,    16,    15,    16,     7,    88,    56,    57,    58,
      71,    72,    73,    74,    31,    40,    42,    43,    77,    81,
      89,    89,    89,    89,    89,    89,    89,    93,    89,    89,
      89,    93,    89,    93,    89,    93,    89,    93,    89,    93,
      89,    93,    89,    93,    89,    93,    89,    93,    89,    93,
      89,    93,    89,    93,    89,    93,    89,    93,    89,    93,
      89,    93,    89,    93,    89,    93,    89,    93,    89,    93,
      89,    93,    89,    93,    89,    89,    89,    90,    91,    92,
      84,    78,    78,    15,    16,    15,    16,    89,    89,    89,
      89,    89,    89,    89,    93,    89,    93,    89,    93,    89,
      93,    15,    16,    89,    93,    89,    93,    89,    93,    89,
      93,    84,    78,    77,    77,    77,    77,    89,    89,    75,
      80,    81,    82,    75,    78,    75,    75,    75,    78,    78,
      75,    78,    78,    78,    78,    78,    78,    78,    75,    75,
      78,    78,    75,    75,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    93,    89,    93,    89,
      93,    89,    93,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    85,    86,    86,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    90,    91,    92,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     1,     3,     1,     3,     3,     3,     1,     1,     1,
       1,     3,     3,     3,     3,     1,     1,     1,     1,     4,
       4,     4,     4,     4,     4,     4,     1,     1,     1,     1,
       3,     3,     3,     3,     2,     3,     2,     4,     4,     4,
       3,     3,     3,     3,     3,     3,     4,     6,     4,     6,
       6,     4,     6,     6,     6,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     6,     6,     6,     6,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     6,     6,
       6,     6,     4,     4,     4,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (scanner, parser_callback, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, scanner, parser_callback); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, yyscan_t scanner, parser_callback_t parser_callback)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (scanner);
  YYUSE (parser_callback);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, yyscan_t scanner, parser_callback_t parser_callback)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep, scanner, parser_callback);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule, yyscan_t scanner, parser_callback_t parser_callback)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              , scanner, parser_callback);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, scanner, parser_callback); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, yyscan_t scanner, parser_callback_t parser_callback)
{
  YYUSE (yyvaluep);
  YYUSE (scanner);
  YYUSE (parser_callback);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (yyscan_t scanner, parser_callback_t parser_callback)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4:
                                                                {	sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 5:
                                                {	sm_print_ll(yyget_extra(scanner), (yyvsp[-1].ll_val)); SM_RES_OK;	}
    break;

  case 6:
                                                {	sm_print_ld(yyget_extra(scanner), (yyvsp[-1].ld_val)); SM_RES_OK; }
    break;

  case 7:
                                                {	sm_print_sz(yyget_extra(scanner), (yyvsp[-1].sz_val)); SM_RES_OK;	}
    break;

  case 8:
                                                {	sm_print_help(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 9:
                                                {	sm_do_exit(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 10:
                                                {	sm_do_exit(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 11:
                                                        {	sm_print_ll(yyget_extra(scanner), (yyvsp[-1].ll_val)); sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 12:
                                                        {	sm_print_ld(yyget_extra(scanner), (yyvsp[-1].ld_val)); sm_print_nl(yyget_extra(scanner)); SM_RES_OK; }
    break;

  case 13:
                                                        {	sm_print_sz(yyget_extra(scanner), (yyvsp[-1].sz_val)); sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 14:
                                                        {	sm_print_help(yyget_extra(scanner)); sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 15:
                                                {	sm_do_exit(yyget_extra(scanner)); sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 16:
                                                        {	sm_do_exit(yyget_extra(scanner)); sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 17:
                                                        {	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 18:
                                                        {	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK; }
    break;

  case 19:
                                                        {	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 20:
                                                                {	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 21:
                                                        {	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 22:
                                                                {	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 23:
                                                        {	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 24:
                                                        {	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK; }
    break;

  case 25:
                                                        {	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 26:
                                                        {	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 27:
                                                        {	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 28:
                                                        {	yyerrok; sm_print_nl(yyget_extra(scanner)); SM_RES_OK;	}
    break;

  case 29:
                                                        { 	sm_copy_buf(yyget_extra(scanner), (yyvsp[0].sz_val), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val))); }
    break;

  case 30:
                                                        {	sm_get_config(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 31:
                                                {	sm_get_f_precision(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 32:
                                                        {	sm_set_f_exp_precision(yyget_extra(scanner)); sm_get_f_precision(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val))); }
    break;

  case 33:
                                                        {	sm_get_a_unit(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 34:
                                                {	sm_set_a_unit(yyget_extra(scanner), SM_TRIG_RADIANS); sm_get_a_unit(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 35:
                                                {	sm_set_a_unit(yyget_extra(scanner), SM_TRIG_DEGREES); sm_get_a_unit(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val))); 	}
    break;

  case 36:
                                                {	sm_set_a_unit(yyget_extra(scanner), SM_TRIG_GRADIAN); sm_get_a_unit(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val))); 	}
    break;

  case 37:
                                                        {	sm_set_a_unit(yyget_extra(scanner), SM_TRIG_RADIANS); sm_get_a_unit(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 38:
                                                        {	sm_set_a_unit(yyget_extra(scanner), SM_TRIG_DEGREES); sm_get_a_unit(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 39:
                                                        {	sm_set_a_unit(yyget_extra(scanner), SM_TRIG_GRADIAN); sm_get_a_unit(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 40:
                                                        {	sm_get_i_format(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 41:
                                                {	sm_set_i_format(yyget_extra(scanner), SM_I_10); sm_get_i_format(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 42:
                                                {	sm_set_i_format(yyget_extra(scanner), SM_I_8); sm_get_i_format(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val))); 	}
    break;

  case 43:
                                                {	sm_set_i_format(yyget_extra(scanner), SM_I_16); sm_get_i_format(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val))); 	}
    break;

  case 44:
                                                {	sm_set_i_format(yyget_extra(scanner), SM_I_2); sm_get_i_format(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val))); 	}
    break;

  case 45:
                                                                {	sm_set_i_format(yyget_extra(scanner), SM_I_10); sm_get_i_format(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 46:
                                                                {	sm_set_i_format(yyget_extra(scanner), SM_I_8); sm_get_i_format(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 47:
                                                                {	sm_set_i_format(yyget_extra(scanner), SM_I_16); sm_get_i_format(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 48:
                                                                {	sm_set_i_format(yyget_extra(scanner), SM_I_2); sm_get_i_format(yyget_extra(scanner), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 49:
                                                                {	sm_conv_i_dec_2_sz(yyget_extra(scanner), SM_I_8, (yyvsp[-1].ll_val), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 50:
                                                                {	sm_conv_i_dec_2_sz(yyget_extra(scanner), SM_I_16, (yyvsp[-1].ll_val), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 51:
                                                                {	sm_conv_i_dec_2_sz(yyget_extra(scanner), SM_I_2, (yyvsp[-1].ll_val), (yyval.sz_val), SM_S_BUFF_SIZE+2);	}
    break;

  case 52:
                                                                {	sm_conv_i_dec_2_sz(yyget_extra(scanner), SM_I_10, (yyvsp[-1].ll_val), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 53:
                                                                {	sm_conv_i_dec_2_sz(yyget_extra(scanner), SM_I_10, (yyvsp[-1].ll_val), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 54:
                                                                {	sm_conv_i_dec_2_sz(yyget_extra(scanner), SM_I_10, (yyvsp[-1].ll_val), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 55:
                                                                        {	sm_copy_buf(yyget_extra(scanner), (yyvsp[-1].sz_val), (yyval.sz_val), SM_ARRAY_SIZE((yyval.sz_val)));	}
    break;

  case 56:
                                                                { 	(yyval.ll_val) = (yyvsp[0].ll_val); }
    break;

  case 60:
                                        {	(yyval.ll_val)=(yyvsp[-1].ll_val);	}
    break;

  case 61:
                                                        {	sm_calc_add_i(yyget_extra(scanner), (yyvsp[-2].ll_val), (yyvsp[0].ll_val), &(yyval.ll_val));	}
    break;

  case 62:
                                                        {	sm_calc_sub_i(yyget_extra(scanner), (yyvsp[-2].ll_val), (yyvsp[0].ll_val), &(yyval.ll_val));	}
    break;

  case 63:
                                                        {	sm_calc_mul_i(yyget_extra(scanner), (yyvsp[-2].ll_val), (yyvsp[0].ll_val), &(yyval.ll_val));	}
    break;

  case 64:
                                                {	(yyval.ll_val) = (-1) * (yyvsp[0].ll_val);	}
    break;

  case 65:
                                                {	sm_set_f_precision(yyget_extra(scanner), (int)(yyvsp[0].ll_val), &(yyval.ll_val));	}
    break;

  case 66:
                                                {	sm_calc_factorial(yyget_extra(scanner), (yyvsp[-1].ll_val), &(yyval.ll_val));		}
    break;

  case 67:
                                                                {	sm_calc_power_i_2(yyget_extra(scanner), (yyvsp[-1].ll_val), &(yyval.ll_val));	}
    break;

  case 68:
                                                                {	sm_calc_power_10_i(yyget_extra(scanner), (yyvsp[-1].ll_val), &(yyval.ll_val));	}
    break;

  case 69:
                                                                {	sm_calc_power_2_i(yyget_extra(scanner), (yyvsp[-1].ll_val), &(yyval.ll_val));	}
    break;

  case 70:
                                                        {	sm_calc_l_ashift(yyget_extra(scanner), (yyvsp[-2].ll_val), (yyvsp[0].ll_val), &(yyval.ll_val));	}
    break;

  case 71:
                                                        {	sm_calc_r_ashift(yyget_extra(scanner), (yyvsp[-2].ll_val), (yyvsp[0].ll_val), &(yyval.ll_val));	}
    break;

  case 72:
                                                        {	sm_calc_l_lshift(yyget_extra(scanner), (yyvsp[-2].ll_val), (yyvsp[0].ll_val), &(yyval.ll_val));	}
    break;

  case 73:
                                                        {	sm_calc_r_lshift(yyget_extra(scanner), (yyvsp[-2].ll_val), (yyvsp[0].ll_val), &(yyval.ll_val));	}
    break;

  case 74:
                                                        {	sm_calc_l_cshift(yyget_extra(scanner), (yyvsp[-2].ll_val), (yyvsp[0].ll_val), &(yyval.ll_val));	}
    break;

  case 75:
                                                        {	sm_calc_r_cshift(yyget_extra(scanner), (yyvsp[-2].ll_val), (yyvsp[0].ll_val), &(yyval.ll_val));	}
    break;

  case 76:
                                                        {	sm_calc_abs_i(yyget_extra(scanner), (yyvsp[-1].ll_val), &(yyval.ll_val));	}
    break;

  case 77:
                                                                                {	sm_calc_mod(yyget_extra(scanner), (yyvsp[-3].ll_val), (yyvsp[-1].ll_val), &(yyval.ll_val));	}
    break;

  case 78:
                                                                                                {	sm_calc_mod(yyget_extra(scanner), (yyvsp[-1].ll_val), 2ll, &(yyval.ll_val));	}
    break;

  case 79:
                                                                                        {	sm_calc_and(yyget_extra(scanner), (yyvsp[-3].ll_val), (yyvsp[-1].ll_val), &(yyval.ll_val));	}
    break;

  case 80:
                                                                                        {	sm_calc_or(yyget_extra(scanner), (yyvsp[-3].ll_val), (yyvsp[-1].ll_val), &(yyval.ll_val));	}
    break;

  case 81:
                                                                                                        {	sm_calc_not(yyget_extra(scanner), (yyvsp[-1].ll_val), &(yyval.ll_val));		}
    break;

  case 82:
                                                                                        {	sm_calc_nand(yyget_extra(scanner), (yyvsp[-3].ll_val), (yyvsp[-1].ll_val), &(yyval.ll_val));	}
    break;

  case 83:
                                                                                        {	sm_calc_nor(yyget_extra(scanner), (yyvsp[-3].ll_val), (yyvsp[-1].ll_val), &(yyval.ll_val));	}
    break;

  case 84:
                                                                                        {	sm_calc_xor(yyget_extra(scanner), (yyvsp[-3].ll_val), (yyvsp[-1].ll_val), &(yyval.ll_val));	}
    break;

  case 85:
                                                                        { 	(yyval.ll_val) = (yyvsp[0].ll_val); }
    break;

  case 86:
                                                                        { 	(yyval.ll_val) = (yyvsp[0].ll_val); }
    break;

  case 87:
                                                                        { 	(yyval.ll_val) = (yyvsp[0].ll_val); }
    break;

  case 88:
                                                                        { 	(yyval.ld_val) = (yyvsp[0].ld_val); 	}
    break;

  case 89:
                                                                        { 	(yyval.ld_val) = (yyvsp[0].ld_val); 	}
    break;

  case 90:
                                                        {	(yyval.ld_val) = (yyvsp[-1].ld_val);	}
    break;

  case 91:
                                                                        {	sm_get_const(yyget_extra(scanner), SM_CONST_PI, &(yyval.ld_val));		}
    break;

  case 92:
                                                                        {	sm_get_const(yyget_extra(scanner), SM_CONST_EXP, &(yyval.ld_val));	}
    break;

  case 93:
                                                                        { 	sm_get_const(yyget_extra(scanner), SM_CONST_GAMMA, &(yyval.ld_val));	}
    break;

  case 94:
                                                                        { 	sm_get_const(yyget_extra(scanner), SM_CONST_PHI, &(yyval.ld_val));	}
    break;

  case 95:
                                                        {	sm_calc_add_f(yyget_extra(scanner), (yyvsp[-2].ld_val), (long double)(yyvsp[0].ll_val), &(yyval.ld_val));	}
    break;

  case 96:
                                                        {	sm_calc_sub_f(yyget_extra(scanner), (yyvsp[-2].ld_val), (long double)(yyvsp[0].ll_val), &(yyval.ld_val));	}
    break;

  case 97:
                                                        {	sm_calc_mul_f(yyget_extra(scanner), (yyvsp[-2].ld_val), (long double)(yyvsp[0].ll_val), &(yyval.ld_val));	}
    break;

  case 98:
                                                        {	sm_calc_add_f(yyget_extra(scanner), (long double)(yyvsp[-2].ll_val), (yyvsp[0].ld_val), &(yyval.ld_val));	}
    break;

  case 99:
                                                        {	sm_calc_sub_f(yyget_extra(scanner), (long double)(yyvsp[-2].ll_val), (yyvsp[0].ld_val), &(yyval.ld_val));	}
    break;

  case 100:
                                                        {	sm_calc_mul_f(yyget_extra(scanner), (long double)(yyvsp[-2].ll_val), (yyvsp[0].ld_val), &(yyval.ld_val));	}
    break;

  case 101:
                                                        {	sm_calc_add_f(yyget_extra(scanner), (yyvsp[-2].ld_val), (yyvsp[0].ld_val), &(yyval.ld_val));	}
    break;

  case 102:
                                                        {	sm_calc_sub_f(yyget_extra(scanner), (yyvsp[-2].ld_val), (yyvsp[0].ld_val), &(yyval.ld_val));	}
    break;

  case 103:
                                                        {	sm_calc_mul_f(yyget_extra(scanner), (yyvsp[-2].ld_val), (yyvsp[0].ld_val), &(yyval.ld_val));	}
    break;

  case 104:
                                                        {	sm_calc_div(yyget_extra(scanner), (long double)(yyvsp[-2].ll_val), (long double)(yyvsp[0].ll_val), &(yyval.ld_val));	}
    break;

  case 105:
                                                        {	sm_calc_div(yyget_extra(scanner), (yyvsp[-2].ld_val), (long double)(yyvsp[0].ll_val), &(yyval.ld_val));			}
    break;

  case 106:
                                                        {	sm_calc_div(yyget_extra(scanner), (long double)(yyvsp[-2].ll_val), (yyvsp[0].ld_val), &(yyval.ld_val));			}
    break;

  case 107:
                                                        {	sm_calc_div(yyget_extra(scanner), (yyvsp[-2].ld_val), (yyvsp[0].ld_val), &(yyval.ld_val));	}
    break;

  case 108:
                                             {	(yyval.ld_val) = (-1.0) * (yyvsp[0].ld_val);  }
    break;

  case 109:
                                                        {	sm_calc_sin(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 110:
                                                        {	sm_calc_cos(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 111:
                                                        {	sm_calc_tan(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 112:
                                                        {	sm_calc_asin(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 113:
                                                        {	sm_calc_acos(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 114:
                                                        {	sm_calc_atan(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 115:
                                                        {	sm_calc_sin(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val)); }
    break;

  case 116:
                                                        {	sm_calc_cos(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val)); }
    break;

  case 117:
                                                        {	sm_calc_tan(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val)); }
    break;

  case 118:
                                                        {	sm_calc_asin(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val));	}
    break;

  case 119:
                                                        {	sm_calc_acos(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val));	}
    break;

  case 120:
                                                        {	sm_calc_atan(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val)); 	}
    break;

  case 121:
                                                                {	sm_conv_grad_2_rad(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 122:
                                                                {	sm_conv_rad_2_grad(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 123:
                                                                {	sm_conv_rad_2_deg(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 124:
                                                                {	sm_conv_deg_2_rad(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 125:
                                                                {	sm_conv_grad_2_deg(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 126:
                                                                {	sm_conv_deg_2_grad(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 127:
                                                        {	sm_conv_grad_2_rad(yyget_extra(scanner), (long double)(yyvsp[-1].ld_val), &(yyval.ld_val));	}
    break;

  case 128:
                                                        {	sm_conv_rad_2_grad(yyget_extra(scanner), (long double)(yyvsp[-1].ld_val), &(yyval.ld_val));	}
    break;

  case 129:
                                                        {	sm_conv_rad_2_deg(yyget_extra(scanner), (long double)(yyvsp[-1].ld_val), &(yyval.ld_val));	}
    break;

  case 130:
                                                        {	sm_conv_deg_2_rad(yyget_extra(scanner), (long double)(yyvsp[-1].ld_val), &(yyval.ld_val));	}
    break;

  case 131:
                                                        {	sm_conv_grad_2_deg(yyget_extra(scanner), (long double)(yyvsp[-1].ld_val), &(yyval.ld_val));	}
    break;

  case 132:
                                                        {	sm_conv_deg_2_grad(yyget_extra(scanner), (long double)(yyvsp[-1].ld_val), &(yyval.ld_val));	}
    break;

  case 133:
                                                                                { sm_calc_power(yyget_extra(scanner), (yyvsp[-3].ld_val), (yyvsp[-1].ld_val), &(yyval.ld_val)); }
    break;

  case 134:
                                                                                { sm_calc_power(yyget_extra(scanner), (long double)(yyvsp[-3].ll_val), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val)); }
    break;

  case 135:
                                                                                { sm_calc_power(yyget_extra(scanner), (yyvsp[-3].ld_val), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val)); }
    break;

  case 136:
                                                                                { sm_calc_power(yyget_extra(scanner), (long double)(yyvsp[-3].ll_val), (yyvsp[-1].ld_val), &(yyval.ld_val)); }
    break;

  case 137:
                                                                {	sm_calc_power_d_2(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val));	}
    break;

  case 138:
                                                                        {	sm_calc_power_exp_d(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val));	}
    break;

  case 139:
                                                                {	sm_calc_power_exp_d(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 140:
                                                                {	sm_calc_power_10_d(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val));	}
    break;

  case 141:
                                                                {	sm_calc_power_2_d(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val));	}
    break;

  case 142:
                                                        {	sm_calc_sqrt(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 143:
                                                        {	sm_calc_sqrt(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val));			}
    break;

  case 144:
                                                        {	sm_calc_ln(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 145:
                                                        {	sm_calc_ln(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val));			}
    break;

  case 146:
                                                        {	sm_calc_lg(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 147:
                                                        {	sm_calc_lg(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val));			}
    break;

  case 148:
                                                                        {	sm_calc_log(yyget_extra(scanner), (long double)(yyvsp[-3].ll_val), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 149:
                                                                                {	sm_calc_log(yyget_extra(scanner), (yyvsp[-3].ld_val), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));		}
    break;

  case 150:
                                                                                {	sm_calc_log(yyget_extra(scanner), (long double)(yyvsp[-3].ll_val), (yyvsp[-1].ld_val), &(yyval.ld_val));		}
    break;

  case 151:
                                                                                        {	sm_calc_log(yyget_extra(scanner), (yyvsp[-3].ld_val), (yyvsp[-1].ld_val), &(yyval.ld_val));					}
    break;

  case 152:
                                                        {	sm_calc_recip(yyget_extra(scanner), (long double)(yyvsp[-1].ll_val), &(yyval.ld_val));	}
    break;

  case 153:
                                                                {	sm_calc_recip(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val));					}
    break;

  case 154:
                                                                {	sm_calc_abs_f(yyget_extra(scanner), (yyvsp[-1].ld_val), &(yyval.ld_val));	}
    break;

  case 155:
                        { sm_get_rand(yyget_extra(scanner), &(yyval.ld_val));	}
    break;



      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (scanner, parser_callback, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (scanner, parser_callback, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, scanner, parser_callback);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, scanner, parser_callback);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (scanner, parser_callback, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, scanner, parser_callback);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, scanner, parser_callback);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}

