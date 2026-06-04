/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "compilador.y"

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
extern FILE *yyin;
extern int yylineno;
void yyerror(const char *s);

ASTNode* root_ast = NULL;

OpType str_to_op(const char* str) {
    if (strcmp(str, "=") == 0) return OP_EQ;
    if (strcmp(str, "<>") == 0) return OP_NEQ;
    if (strcmp(str, ">") == 0) return OP_GT;
    if (strcmp(str, ">=") == 0) return OP_GTE;
    if (strcmp(str, "<") == 0) return OP_LT;
    if (strcmp(str, "<=") == 0) return OP_LTE;
    if (strcmp(str, "*") == 0) return OP_MUL;
    if (strcmp(str, "/") == 0) return OP_DIV_REAL;
    if (strcasecmp(str, "div") == 0) return OP_DIV_INT;
    if (strcasecmp(str, "mod") == 0) return OP_MOD;
    if (strcasecmp(str, "and") == 0) return OP_AND;
    return OP_ADD; // Should not happen
}


#line 101 "compilador.tab.c"

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

#include "compilador.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_PROGRAM = 3,                    /* PROGRAM  */
  YYSYMBOL_VAR = 4,                        /* VAR  */
  YYSYMBOL_INTEIRO = 5,                    /* INTEIRO  */
  YYSYMBOL_REAL = 6,                       /* REAL  */
  YYSYMBOL_FUNCTION = 7,                   /* FUNCTION  */
  YYSYMBOL_PROCEDURE = 8,                  /* PROCEDURE  */
  YYSYMBOL_BEGIN_TOKEN = 9,                /* BEGIN_TOKEN  */
  YYSYMBOL_END = 10,                       /* END  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_THEN = 12,                      /* THEN  */
  YYSYMBOL_ELSE = 13,                      /* ELSE  */
  YYSYMBOL_WHILE = 14,                     /* WHILE  */
  YYSYMBOL_DO = 15,                        /* DO  */
  YYSYMBOL_NUM = 16,                       /* NUM  */
  YYSYMBOL_ID = 17,                        /* ID  */
  YYSYMBOL_OPERADOR_RELACIONAL = 18,       /* OPERADOR_RELACIONAL  */
  YYSYMBOL_OPERADOR_MULTIPLICATIVO = 19,   /* OPERADOR_MULTIPLICATIVO  */
  YYSYMBOL_OPERADOR_ATRIBUICAO = 20,       /* OPERADOR_ATRIBUICAO  */
  YYSYMBOL_MAIS = 21,                      /* MAIS  */
  YYSYMBOL_MENOS = 22,                     /* MENOS  */
  YYSYMBOL_OR = 23,                        /* OR  */
  YYSYMBOL_ABRE_PARENTESES = 24,           /* ABRE_PARENTESES  */
  YYSYMBOL_FECHA_PARENTESES = 25,          /* FECHA_PARENTESES  */
  YYSYMBOL_PONTO_FINAL = 26,               /* PONTO_FINAL  */
  YYSYMBOL_PONTO_VIRGULA = 27,             /* PONTO_VIRGULA  */
  YYSYMBOL_VIRGULA = 28,                   /* VIRGULA  */
  YYSYMBOL_DOIS_PONTOS = 29,               /* DOIS_PONTOS  */
  YYSYMBOL_YYACCEPT = 30,                  /* $accept  */
  YYSYMBOL_PROGRAMA = 31,                  /* PROGRAMA  */
  YYSYMBOL_LISTA_DE_IDENTIFICADORES = 32,  /* LISTA_DE_IDENTIFICADORES  */
  YYSYMBOL_DECLARACOES = 33,               /* DECLARACOES  */
  YYSYMBOL_TIPO = 34,                      /* TIPO  */
  YYSYMBOL_DECLARACOES_DE_SUBPROGRAMAS = 35, /* DECLARACOES_DE_SUBPROGRAMAS  */
  YYSYMBOL_DECLARACAO_DE_SUBPROGRAMA = 36, /* DECLARACAO_DE_SUBPROGRAMA  */
  YYSYMBOL_CABECALHO_DE_SUBPROGRAMA = 37,  /* CABECALHO_DE_SUBPROGRAMA  */
  YYSYMBOL_ARGUMENTOS = 38,                /* ARGUMENTOS  */
  YYSYMBOL_LISTA_DE_PARAMETROS = 39,       /* LISTA_DE_PARAMETROS  */
  YYSYMBOL_ENUNCIADO_COMPOSTO = 40,        /* ENUNCIADO_COMPOSTO  */
  YYSYMBOL_ENUNCIADOS_OPCIONAIS = 41,      /* ENUNCIADOS_OPCIONAIS  */
  YYSYMBOL_LISTA_DE_ENUNCIADOS = 42,       /* LISTA_DE_ENUNCIADOS  */
  YYSYMBOL_ENUNCIADO = 43,                 /* ENUNCIADO  */
  YYSYMBOL_VARIAVEL = 44,                  /* VARIAVEL  */
  YYSYMBOL_CHAMADA_DE_PROCEDIMENTO = 45,   /* CHAMADA_DE_PROCEDIMENTO  */
  YYSYMBOL_LISTA_DE_EXPRESSOES = 46,       /* LISTA_DE_EXPRESSOES  */
  YYSYMBOL_EXPRESSAO = 47,                 /* EXPRESSAO  */
  YYSYMBOL_EXPRESSAO_SIMPLES = 48,         /* EXPRESSAO_SIMPLES  */
  YYSYMBOL_TERMO = 49,                     /* TERMO  */
  YYSYMBOL_FATOR = 50,                     /* FATOR  */
  YYSYMBOL_SINAL = 51                      /* SINAL  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
typedef yytype_int8 yy_state_t;

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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if 1

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
#endif /* 1 */

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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   109

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  30
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  49
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  110

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   284


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

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
      25,    26,    27,    28,    29
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    63,    63,    75,    81,    89,    99,   104,   106,   110,
     120,   125,   133,   138,   145,   148,   151,   156,   161,   166,
     173,   179,   182,   185,   190,   197,   202,   204,   206,   210,
     216,   223,   228,   235,   240,   247,   249,   257,   259,   268,
     270,   272,   276,   278,   286,   291,   296,   305,   309,   311
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  static const char *const yy_sname[] =
  {
  "end of file", "error", "invalid token", "PROGRAM", "VAR", "INTEIRO",
  "REAL", "FUNCTION", "PROCEDURE", "BEGIN_TOKEN", "END", "IF", "THEN",
  "ELSE", "WHILE", "DO", "NUM", "ID", "OPERADOR_RELACIONAL",
  "OPERADOR_MULTIPLICATIVO", "OPERADOR_ATRIBUICAO", "MAIS", "MENOS", "OR",
  "ABRE_PARENTESES", "FECHA_PARENTESES", "PONTO_FINAL", "PONTO_VIRGULA",
  "VIRGULA", "DOIS_PONTOS", "$accept", "PROGRAMA",
  "LISTA_DE_IDENTIFICADORES", "DECLARACOES", "TIPO",
  "DECLARACOES_DE_SUBPROGRAMAS", "DECLARACAO_DE_SUBPROGRAMA",
  "CABECALHO_DE_SUBPROGRAMA", "ARGUMENTOS", "LISTA_DE_PARAMETROS",
  "ENUNCIADO_COMPOSTO", "ENUNCIADOS_OPCIONAIS", "LISTA_DE_ENUNCIADOS",
  "ENUNCIADO", "VARIAVEL", "CHAMADA_DE_PROCEDIMENTO",
  "LISTA_DE_EXPRESSOES", "EXPRESSAO", "EXPRESSAO_SIMPLES", "TERMO",
  "FATOR", "SINAL", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#define YYPACT_NINF (-57)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-31)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       6,     9,    36,    14,   -57,    15,   -57,     3,    55,    41,
     -57,   -57,    77,    15,    54,    39,    68,    69,    -1,    56,
     -57,    61,    66,    16,    16,    13,    13,    -9,   -57,    78,
      62,   -57,    70,   -57,   -57,     8,   -57,   -57,   -57,    64,
       1,    63,    67,   -57,    71,   -57,   -57,    13,    81,    23,
      79,   -57,    26,    82,    13,   -57,    -1,    13,   -57,   -57,
      15,    45,    -5,    66,   -57,    13,    74,    -1,    13,    13,
      13,    13,    26,    79,    -1,    31,   -57,   -57,   -57,    47,
      66,   -57,     2,    73,    32,   -57,    83,    43,   -57,   -57,
     -57,   -57,   -57,   -57,    13,    66,   -57,    15,    49,   -57,
     -57,    -1,   -57,   -57,    51,    66,   -57,    66,   -57,   -57
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     3,     0,     0,     0,
       6,     4,    10,     0,     0,     0,     0,     0,    22,     0,
       6,     0,     0,    15,    15,     0,     0,    31,    27,     0,
      21,    23,     0,    26,     9,     0,     2,     7,     8,     0,
       0,     0,     0,    46,    44,    48,    49,     0,     0,    35,
      37,    42,     0,     0,     0,    20,     0,     0,    11,     5,
       0,     0,     0,     0,    13,     0,     0,     0,     0,     0,
       0,     0,     0,    38,     0,     0,    33,    24,    25,     0,
       0,    14,     0,     0,     0,    47,     0,    36,    39,    40,
      41,    43,    29,    32,     0,     0,    16,     0,     0,    12,
      45,     0,    34,    17,     0,     0,    28,     0,    18,    19
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -57,   -57,   -13,    84,   -56,   -57,   -57,   -57,    85,   -57,
     -10,   -57,   -57,   -53,   -57,   -57,    37,   -24,   -16,    53,
      29,   -57
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     7,    12,    39,    14,    19,    20,    41,    62,
      28,    29,    30,    31,    32,    33,    75,    76,    49,    50,
      51,    52
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      15,    48,    53,    77,    21,    60,    97,    83,    18,     1,
      25,   -30,    13,    26,    86,    54,    27,    18,     6,     6,
      81,    92,    82,    66,    96,    58,     3,    61,     8,    43,
      44,     9,     6,    78,    45,    46,     4,    47,     5,   103,
      40,    68,    43,    44,    69,    70,    71,    79,   106,   108,
      47,   109,    87,    88,    89,    90,    93,   100,    11,    94,
      94,    16,    17,    18,    69,    70,    71,     9,    22,    98,
     102,    37,    38,     9,    80,     9,    95,     9,   105,     9,
     107,    13,    10,    34,   104,    23,    24,    36,    55,    56,
      57,    59,    63,    67,    64,    65,   101,    74,    72,    85,
      99,    91,    84,     0,    35,    73,     0,     0,     0,    42
};

static const yytype_int8 yycheck[] =
{
      13,    25,    26,    56,    14,     4,     4,    63,     9,     3,
      11,    20,     4,    14,    67,    24,    17,     9,    17,    17,
      25,    74,    27,    47,    80,    35,    17,    40,    25,    16,
      17,    28,    17,    57,    21,    22,     0,    24,    24,    95,
      24,    18,    16,    17,    21,    22,    23,    60,   101,   105,
      24,   107,    68,    69,    70,    71,    25,    25,    17,    28,
      28,     7,     8,     9,    21,    22,    23,    28,    29,    82,
      94,     5,     6,    28,    29,    28,    29,    28,    29,    28,
      29,     4,    27,    27,    97,    17,    17,    26,    10,    27,
      20,    27,    29,    12,    27,    24,    13,    15,    19,    25,
      27,    72,    65,    -1,    20,    52,    -1,    -1,    -1,    24
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    31,    17,     0,    24,    17,    32,    25,    28,
      27,    17,    33,     4,    35,    32,     7,     8,     9,    36,
      37,    40,    29,    17,    17,    11,    14,    17,    40,    41,
      42,    43,    44,    45,    27,    33,    26,     5,     6,    34,
      24,    38,    38,    16,    17,    21,    22,    24,    47,    48,
      49,    50,    51,    47,    24,    10,    27,    20,    40,    27,
       4,    32,    39,    29,    27,    24,    47,    12,    18,    21,
      22,    23,    19,    49,    15,    46,    47,    43,    47,    32,
      29,    25,    27,    34,    46,    25,    43,    48,    48,    48,
      48,    50,    43,    25,    28,    29,    34,     4,    32,    27,
      25,    13,    47,    34,    32,    29,    43,    29,    34,    34
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    30,    31,    32,    32,    33,    33,    34,    34,    35,
      35,    36,    37,    37,    38,    38,    39,    39,    39,    39,
      40,    41,    41,    42,    42,    43,    43,    43,    43,    43,
      44,    45,    45,    46,    46,    47,    47,    48,    48,    48,
      48,    48,    49,    49,    50,    50,    50,    50,    51,    51
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,    10,     1,     3,     6,     0,     1,     1,     3,
       0,     3,     6,     4,     3,     0,     3,     4,     5,     6,
       3,     1,     0,     1,     3,     3,     1,     1,     6,     4,
       1,     1,     4,     1,     3,     1,     3,     1,     2,     3,
       3,     3,     1,     3,     1,     4,     1,     3,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
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
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
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
# endif
#endif



static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
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
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
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
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
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
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
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


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 2: /* PROGRAMA: PROGRAM ID ABRE_PARENTESES LISTA_DE_IDENTIFICADORES FECHA_PARENTESES PONTO_VIRGULA DECLARACOES DECLARACOES_DE_SUBPROGRAMAS ENUNCIADO_COMPOSTO PONTO_FINAL  */
#line 68 "compilador.y"
        {
            (yyval.ast_node) = ast_new_program((yyvsp[-8].str_val), (yyvsp[-6].ast_node), (yyvsp[-3].ast_node), (yyvsp[-2].ast_node), (yyvsp[-1].ast_node), yylineno);
            root_ast = (yyval.ast_node);
            free((yyvsp[-8].str_val));
        }
#line 1427 "compilador.tab.c"
    break;

  case 3: /* LISTA_DE_IDENTIFICADORES: ID  */
#line 76 "compilador.y"
                        {
                            (yyval.ast_node) = ast_new_list(AST_ID_LIST, yylineno);
                            ast_list_add((yyval.ast_node), ast_new_id_list_item((yyvsp[0].str_val), yylineno));
                            free((yyvsp[0].str_val));
                        }
#line 1437 "compilador.tab.c"
    break;

  case 4: /* LISTA_DE_IDENTIFICADORES: LISTA_DE_IDENTIFICADORES VIRGULA ID  */
#line 82 "compilador.y"
                        {
                            (yyval.ast_node) = (yyvsp[-2].ast_node);
                            ast_list_add((yyval.ast_node), ast_new_id_list_item((yyvsp[0].str_val), yylineno));
                            free((yyvsp[0].str_val));
                        }
#line 1447 "compilador.tab.c"
    break;

  case 5: /* DECLARACOES: DECLARACOES VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO PONTO_VIRGULA  */
#line 90 "compilador.y"
           {
               if ((yyvsp[-5].ast_node) == NULL) {
                   (yyval.ast_node) = ast_new_list(AST_DECL_LIST, yylineno);
               } else {
                   (yyval.ast_node) = (yyvsp[-5].ast_node);
               }
               ast_list_add((yyval.ast_node), ast_new_decl((yyvsp[-3].ast_node), (ExprType)(yyvsp[-1].int_val), yylineno));
           }
#line 1460 "compilador.tab.c"
    break;

  case 6: /* DECLARACOES: %empty  */
#line 99 "compilador.y"
           {
               (yyval.ast_node) = NULL;
           }
#line 1468 "compilador.tab.c"
    break;

  case 7: /* TIPO: INTEIRO  */
#line 105 "compilador.y"
    { (yyval.int_val) = TYPE_INTEGER; }
#line 1474 "compilador.tab.c"
    break;

  case 8: /* TIPO: REAL  */
#line 107 "compilador.y"
    { (yyval.int_val) = TYPE_REAL; }
#line 1480 "compilador.tab.c"
    break;

  case 9: /* DECLARACOES_DE_SUBPROGRAMAS: DECLARACOES_DE_SUBPROGRAMAS DECLARACAO_DE_SUBPROGRAMA PONTO_VIRGULA  */
#line 111 "compilador.y"
                           {
                               if ((yyvsp[-2].ast_node) == NULL) {
                                   (yyval.ast_node) = ast_new_list(AST_DECL_LIST, yylineno);
                               } else {
                                   (yyval.ast_node) = (yyvsp[-2].ast_node);
                               }
                               ast_list_add((yyval.ast_node), (yyvsp[-1].ast_node));
                           }
#line 1493 "compilador.tab.c"
    break;

  case 10: /* DECLARACOES_DE_SUBPROGRAMAS: %empty  */
#line 120 "compilador.y"
                           {
                               (yyval.ast_node) = NULL;
                           }
#line 1501 "compilador.tab.c"
    break;

  case 11: /* DECLARACAO_DE_SUBPROGRAMA: CABECALHO_DE_SUBPROGRAMA DECLARACOES ENUNCIADO_COMPOSTO  */
#line 126 "compilador.y"
                         {
                             (yyval.ast_node) = (yyvsp[-2].ast_node); // CABECALHO creates the func_decl node
                             (yyval.ast_node)->func_decl.decls = (yyvsp[-1].ast_node);
                             (yyval.ast_node)->func_decl.compound = (yyvsp[0].ast_node);
                         }
#line 1511 "compilador.tab.c"
    break;

  case 12: /* CABECALHO_DE_SUBPROGRAMA: FUNCTION ID ARGUMENTOS DOIS_PONTOS TIPO PONTO_VIRGULA  */
#line 134 "compilador.y"
                        {
                            (yyval.ast_node) = ast_new_func_decl((yyvsp[-4].str_val), 1, (yyvsp[-3].ast_node), (ExprType)(yyvsp[-1].int_val), NULL, NULL, yylineno);
                            free((yyvsp[-4].str_val));
                        }
#line 1520 "compilador.tab.c"
    break;

  case 13: /* CABECALHO_DE_SUBPROGRAMA: PROCEDURE ID ARGUMENTOS PONTO_VIRGULA  */
#line 139 "compilador.y"
                        {
                            (yyval.ast_node) = ast_new_func_decl((yyvsp[-2].str_val), 0, (yyvsp[-1].ast_node), TYPE_VOID, NULL, NULL, yylineno);
                            free((yyvsp[-2].str_val));
                        }
#line 1529 "compilador.tab.c"
    break;

  case 14: /* ARGUMENTOS: ABRE_PARENTESES LISTA_DE_PARAMETROS FECHA_PARENTESES  */
#line 146 "compilador.y"
          { (yyval.ast_node) = (yyvsp[-1].ast_node); }
#line 1535 "compilador.tab.c"
    break;

  case 15: /* ARGUMENTOS: %empty  */
#line 148 "compilador.y"
          { (yyval.ast_node) = NULL; }
#line 1541 "compilador.tab.c"
    break;

  case 16: /* LISTA_DE_PARAMETROS: LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO  */
#line 152 "compilador.y"
                   {
                       (yyval.ast_node) = ast_new_list(AST_PARAM_LIST, yylineno);
                       ast_list_add((yyval.ast_node), ast_new_param((yyvsp[-2].ast_node), (ExprType)(yyvsp[0].int_val), 0, yylineno));
                   }
#line 1550 "compilador.tab.c"
    break;

  case 17: /* LISTA_DE_PARAMETROS: VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO  */
#line 157 "compilador.y"
                   {
                       (yyval.ast_node) = ast_new_list(AST_PARAM_LIST, yylineno);
                       ast_list_add((yyval.ast_node), ast_new_param((yyvsp[-2].ast_node), (ExprType)(yyvsp[0].int_val), 1, yylineno));
                   }
#line 1559 "compilador.tab.c"
    break;

  case 18: /* LISTA_DE_PARAMETROS: LISTA_DE_PARAMETROS PONTO_VIRGULA LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO  */
#line 162 "compilador.y"
                   {
                       (yyval.ast_node) = (yyvsp[-4].ast_node);
                       ast_list_add((yyval.ast_node), ast_new_param((yyvsp[-2].ast_node), (ExprType)(yyvsp[0].int_val), 0, yylineno));
                   }
#line 1568 "compilador.tab.c"
    break;

  case 19: /* LISTA_DE_PARAMETROS: LISTA_DE_PARAMETROS PONTO_VIRGULA VAR LISTA_DE_IDENTIFICADORES DOIS_PONTOS TIPO  */
#line 167 "compilador.y"
                   {
                       (yyval.ast_node) = (yyvsp[-5].ast_node);
                       ast_list_add((yyval.ast_node), ast_new_param((yyvsp[-2].ast_node), (ExprType)(yyvsp[0].int_val), 1, yylineno));
                   }
#line 1577 "compilador.tab.c"
    break;

  case 20: /* ENUNCIADO_COMPOSTO: BEGIN_TOKEN ENUNCIADOS_OPCIONAIS END  */
#line 174 "compilador.y"
                  {
                      (yyval.ast_node) = ast_new_compound((yyvsp[-1].ast_node), yylineno);
                  }
#line 1585 "compilador.tab.c"
    break;

  case 21: /* ENUNCIADOS_OPCIONAIS: LISTA_DE_ENUNCIADOS  */
#line 180 "compilador.y"
                    { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 1591 "compilador.tab.c"
    break;

  case 22: /* ENUNCIADOS_OPCIONAIS: %empty  */
#line 182 "compilador.y"
                    { (yyval.ast_node) = NULL; }
#line 1597 "compilador.tab.c"
    break;

  case 23: /* LISTA_DE_ENUNCIADOS: ENUNCIADO  */
#line 186 "compilador.y"
                   {
                       (yyval.ast_node) = ast_new_list(AST_STMT_LIST, yylineno);
                       ast_list_add((yyval.ast_node), (yyvsp[0].ast_node));
                   }
#line 1606 "compilador.tab.c"
    break;

  case 24: /* LISTA_DE_ENUNCIADOS: LISTA_DE_ENUNCIADOS PONTO_VIRGULA ENUNCIADO  */
#line 191 "compilador.y"
                   {
                       (yyval.ast_node) = (yyvsp[-2].ast_node);
                       ast_list_add((yyval.ast_node), (yyvsp[0].ast_node));
                   }
#line 1615 "compilador.tab.c"
    break;

  case 25: /* ENUNCIADO: VARIAVEL OPERADOR_ATRIBUICAO EXPRESSAO  */
#line 198 "compilador.y"
         {
             (yyval.ast_node) = ast_new_assign((yyvsp[-2].ast_node)->var.id, (yyvsp[0].ast_node), yylineno);
             ast_free((yyvsp[-2].ast_node));
         }
#line 1624 "compilador.tab.c"
    break;

  case 26: /* ENUNCIADO: CHAMADA_DE_PROCEDIMENTO  */
#line 203 "compilador.y"
         { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 1630 "compilador.tab.c"
    break;

  case 27: /* ENUNCIADO: ENUNCIADO_COMPOSTO  */
#line 205 "compilador.y"
         { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 1636 "compilador.tab.c"
    break;

  case 28: /* ENUNCIADO: IF EXPRESSAO THEN ENUNCIADO ELSE ENUNCIADO  */
#line 207 "compilador.y"
         {
             (yyval.ast_node) = ast_new_if((yyvsp[-4].ast_node), (yyvsp[-2].ast_node), (yyvsp[0].ast_node), yylineno);
         }
#line 1644 "compilador.tab.c"
    break;

  case 29: /* ENUNCIADO: WHILE EXPRESSAO DO ENUNCIADO  */
#line 211 "compilador.y"
         {
             (yyval.ast_node) = ast_new_while((yyvsp[-2].ast_node), (yyvsp[0].ast_node), yylineno);
         }
#line 1652 "compilador.tab.c"
    break;

  case 30: /* VARIAVEL: ID  */
#line 217 "compilador.y"
        {
            (yyval.ast_node) = ast_new_var((yyvsp[0].str_val), yylineno);
            free((yyvsp[0].str_val));
        }
#line 1661 "compilador.tab.c"
    break;

  case 31: /* CHAMADA_DE_PROCEDIMENTO: ID  */
#line 224 "compilador.y"
                    {
                        (yyval.ast_node) = ast_new_call((yyvsp[0].str_val), NULL, yylineno);
                        free((yyvsp[0].str_val));
                    }
#line 1670 "compilador.tab.c"
    break;

  case 32: /* CHAMADA_DE_PROCEDIMENTO: ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES  */
#line 229 "compilador.y"
                    {
                        (yyval.ast_node) = ast_new_call((yyvsp[-3].str_val), (yyvsp[-1].ast_node), yylineno);
                        free((yyvsp[-3].str_val));
                    }
#line 1679 "compilador.tab.c"
    break;

  case 33: /* LISTA_DE_EXPRESSOES: EXPRESSAO  */
#line 236 "compilador.y"
                   {
                       (yyval.ast_node) = ast_new_list(AST_EXPR_LIST, yylineno);
                       ast_list_add((yyval.ast_node), (yyvsp[0].ast_node));
                   }
#line 1688 "compilador.tab.c"
    break;

  case 34: /* LISTA_DE_EXPRESSOES: LISTA_DE_EXPRESSOES VIRGULA EXPRESSAO  */
#line 241 "compilador.y"
                   {
                       (yyval.ast_node) = (yyvsp[-2].ast_node);
                       ast_list_add((yyval.ast_node), (yyvsp[0].ast_node));
                   }
#line 1697 "compilador.tab.c"
    break;

  case 35: /* EXPRESSAO: EXPRESSAO_SIMPLES  */
#line 248 "compilador.y"
         { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 1703 "compilador.tab.c"
    break;

  case 36: /* EXPRESSAO: EXPRESSAO_SIMPLES OPERADOR_RELACIONAL EXPRESSAO_SIMPLES  */
#line 250 "compilador.y"
         {
             OpType op = str_to_op((yyvsp[-1].str_val));
             (yyval.ast_node) = ast_new_binop(op, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), yylineno);
             free((yyvsp[-1].str_val));
         }
#line 1713 "compilador.tab.c"
    break;

  case 37: /* EXPRESSAO_SIMPLES: TERMO  */
#line 258 "compilador.y"
                 { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 1719 "compilador.tab.c"
    break;

  case 38: /* EXPRESSAO_SIMPLES: SINAL TERMO  */
#line 260 "compilador.y"
                 {
                     if ((yyvsp[-1].int_val) == OP_SUB) {
                         ASTNode* zero = ast_new_num_int(0, yylineno);
                         (yyval.ast_node) = ast_new_binop(OP_SUB, zero, (yyvsp[0].ast_node), yylineno);
                     } else {
                         (yyval.ast_node) = (yyvsp[0].ast_node);
                     }
                 }
#line 1732 "compilador.tab.c"
    break;

  case 39: /* EXPRESSAO_SIMPLES: EXPRESSAO_SIMPLES MAIS EXPRESSAO_SIMPLES  */
#line 269 "compilador.y"
                 { (yyval.ast_node) = ast_new_binop(OP_ADD, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), yylineno); }
#line 1738 "compilador.tab.c"
    break;

  case 40: /* EXPRESSAO_SIMPLES: EXPRESSAO_SIMPLES MENOS EXPRESSAO_SIMPLES  */
#line 271 "compilador.y"
                 { (yyval.ast_node) = ast_new_binop(OP_SUB, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), yylineno); }
#line 1744 "compilador.tab.c"
    break;

  case 41: /* EXPRESSAO_SIMPLES: EXPRESSAO_SIMPLES OR EXPRESSAO_SIMPLES  */
#line 273 "compilador.y"
                 { (yyval.ast_node) = ast_new_binop(OP_OR, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), yylineno); }
#line 1750 "compilador.tab.c"
    break;

  case 42: /* TERMO: FATOR  */
#line 277 "compilador.y"
     { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 1756 "compilador.tab.c"
    break;

  case 43: /* TERMO: TERMO OPERADOR_MULTIPLICATIVO FATOR  */
#line 279 "compilador.y"
     {
         OpType op = str_to_op((yyvsp[-1].str_val));
         (yyval.ast_node) = ast_new_binop(op, (yyvsp[-2].ast_node), (yyvsp[0].ast_node), yylineno);
         free((yyvsp[-1].str_val));
     }
#line 1766 "compilador.tab.c"
    break;

  case 44: /* FATOR: ID  */
#line 287 "compilador.y"
     {
         (yyval.ast_node) = ast_new_var((yyvsp[0].str_val), yylineno);
         free((yyvsp[0].str_val));
     }
#line 1775 "compilador.tab.c"
    break;

  case 45: /* FATOR: ID ABRE_PARENTESES LISTA_DE_EXPRESSOES FECHA_PARENTESES  */
#line 292 "compilador.y"
     {
         (yyval.ast_node) = ast_new_call((yyvsp[-3].str_val), (yyvsp[-1].ast_node), yylineno);
         free((yyvsp[-3].str_val));
     }
#line 1784 "compilador.tab.c"
    break;

  case 46: /* FATOR: NUM  */
#line 297 "compilador.y"
     {
         if (strchr((yyvsp[0].str_val), '.') || strchr((yyvsp[0].str_val), 'e') || strchr((yyvsp[0].str_val), 'E')) {
             (yyval.ast_node) = ast_new_num_real(atof((yyvsp[0].str_val)), yylineno);
         } else {
             (yyval.ast_node) = ast_new_num_int(atoi((yyvsp[0].str_val)), yylineno);
         }
         free((yyvsp[0].str_val));
     }
#line 1797 "compilador.tab.c"
    break;

  case 47: /* FATOR: ABRE_PARENTESES EXPRESSAO FECHA_PARENTESES  */
#line 306 "compilador.y"
     { (yyval.ast_node) = (yyvsp[-1].ast_node); }
#line 1803 "compilador.tab.c"
    break;

  case 48: /* SINAL: MAIS  */
#line 310 "compilador.y"
     { (yyval.int_val) = OP_ADD; }
#line 1809 "compilador.tab.c"
    break;

  case 49: /* SINAL: MENOS  */
#line 312 "compilador.y"
     { (yyval.int_val) = OP_SUB; }
#line 1815 "compilador.tab.c"
    break;


#line 1819 "compilador.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
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
                      yytoken, &yylval);
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 315 "compilador.y"


extern void codegen_generate(ASTNode* root);

int main(int argc, char ** argv) {
    if (argc == 2) {
        yyin = fopen(argv[1], "r");
        yylineno = 1;
        if (!yyparse()) {
            codegen_generate(root_ast);
        }
        ast_free(root_ast);
        fclose(yyin);
    } else {
        fprintf(stderr, "Arquivo de entrada nao foi fornecido\n");
    }    
    return 0;
}

void yyerror(const char *s) {
  fprintf(stderr, "Erro na linha %d: %s\n", yylineno,s);
  exit(1);
}
