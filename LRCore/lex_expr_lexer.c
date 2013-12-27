#include <mlex.h>

#define YYLEXFAST
#line 1 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"

/******************************************
 lex_test.l
 My very first approach to lexical analyzer
 ******************************************/
 
 /***********
 declarations
 ************/
 
#include<string.h>
#include<stdlib.h>

#include "expr_token_structs.h"

lexExprToken lexExpressionTbl[MAX_TOKEN_COUNT];

static int lexTokenCount=0;

void lexZeroTokenCount() {
 lexTokenCount=0;
}

void lexAddToken(int Type,yymlex_t* yy) {
 if(lexTokenCount==(MAX_TOKEN_COUNT-1)) {
  printf("Expression lexer critical error - MAX_TOKEN_COUNT reached!\n");
  return;
 }
 if(strlen(yy->yymtext)>MAX_TOKEN_LENGTH) {
  printf("Expression lexer critical error - MAX_TOKEN_LENGTH reached!\n");
  return;
 }
 strcpy(lexExpressionTbl[lexTokenCount].szVal,yy->yymtext);
 lexExpressionTbl[lexTokenCount++].Type=Type;
}

/* multiple instance lex conversion file */
#include "milconv.h"

#line 45 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
/* repeated because of possible precompiled header */
#include <mlex.h>

#define YYLEXFAST
#include "lex_expr_lexer.h"

#ifndef YYTEXT_SIZE
#define YYTEXT_SIZE 100
#endif
#ifndef YYUNPUT_SIZE
#define YYUNPUT_SIZE YYTEXT_SIZE
#endif

/* backwards compatability with lex */
#ifdef input
#ifdef YYPROTOTYPE
static int YYCDECL yyminput(yymlex_t YYFAR *yy)
#else
static int YYCDECL yyminput(yy)
yymlex_t YYFAR *yy;
#endif
{
	yyassert(yy != NULL);
	return input();
}
#else
#define input() (*yy->yyminput)(yy)
#endif

#ifdef output
#ifdef YYPROTOTYPE
static void YYCDECL yymoutput(yymlex_t YYFAR *yy, int ch)
#else
static void YYCDECL yymoutput(yy, ch)
yymlex_t YYFAR *yy;
int ch;
#endif
{
	yyassert(yy != NULL);
	output(ch);
}
#else
#define output(ch) (*yy->yymoutput)(yy, (ch))
#endif

#ifdef unput
#ifdef YYPROTOTYPE
static void YYCDECL yymunput(yymlex_t YYFAR *yy, int ch)
#else
static void YYCDECL yymunput(yy, ch)
yymlex_t YYFAR *yy;
int ch;
#endif
{
	yyassert(yy != NULL);
	unput(ch);
}
#else
#define unput(ch) (*yy->yymunput)(yy, (ch))
#endif

/* variables */
#ifndef YYMIN
#ifndef YYNINITSTREAM
#define YYMIN stdin
#else
#define YYMIN NULL
#endif
#endif

#ifndef YYMOUT
#ifndef YYNINITSTREAM
#define YYMOUT stdout
#else
#define YYMOUT NULL
#endif
#endif

#ifndef YYMLEXERR
#ifndef YYNINITSTREAM
#define YYMLEXERR stderr
#else
#define YYMLEXERR NULL
#endif
#endif

#ifndef YYMLEXDEBUGOUT
#ifndef YYNINITSTREAM
#define YYMLEXDEBUGOUT stdout
#else
#define YYMLEXDEBUGOUT NULL
#endif
#endif

/* pointers to functions */
#ifndef YYMINPUT
#define YYMINPUT yyminput
#endif
#ifndef YYMOUTPUT
#define YYMOUTPUT yymoutput
#endif
#ifndef YYMUNPUT
#define YYMUNPUT yymunput
#endif
#ifndef YYMGETCHAR
#define YYMGETCHAR yymgetchar
#endif
#ifndef YYMWRAP
#define YYMWRAP yymwrap
#endif
#ifndef YYMTEXTOVERFLOW
#define YYMTEXTOVERFLOW yymtextoverflow
#endif
#ifndef YYMUNPUTOVERFLOW
#define YYMUNPUTOVERFLOW yymunputoverflow
#endif
#ifndef YYMLEXDATA
#define YYMLEXDATA NULL
#endif

#ifndef YYNBORLANDWARN
#ifdef __BORLANDC__
#pragma warn -rch		/* <warning: unreachable code> off */
#endif
#endif

#ifdef YYPROTOTYPE
static int YYCDECL yylexaction(yymlex_t YYFAR *yy, int action)
#else
static int YYCDECL yylexaction(yy, action)
yymlex_t YYFAR *yy;
int action;
#endif
{
#line 56 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"

YYSTYPE *yymlvalptr = (YYSTYPE *) yy->yymdata;
yyassert(yymlvalptr != NULL);
#define yylval (*yymlvalptr)

#line 186 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
	yy->yymreturnflg = 1;
	switch (action) {
	case 1:
	case 2:
	case 3:
		{
#line 64 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_FPNUMBER,yy);
                                                  yylval=lexTokenCount-1;
                                                  return EXPR_FPNUMBER; 
#line 197 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 4:
		{
#line 67 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_MINUS,yy);
                                                  return EXPR_MINUS; 
#line 205 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 5:
		{
#line 69 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_PLUS,yy); 
                                                  return EXPR_PLUS; 
#line 213 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 6:
		{
#line 71 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_MULTIPLY,yy);
                                                  return EXPR_MULTIPLY; 
#line 221 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 7:
		{
#line 73 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_DIVIDE,yy);
                                                  return EXPR_DIVIDE; 
#line 229 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 8:
		{
#line 75 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_LBRACKET,yy);
                                                  return EXPR_LBRACKET; 
#line 237 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 9:
		{
#line 77 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_RBRACKET,yy);
                                                  return EXPR_RBRACKET; 
#line 245 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 10:
		{
#line 79 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_POWER,yy);
                                                  return EXPR_POWER; 
#line 253 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 11:
		{
#line 81 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_TIME,yy);
                                                  return EXPR_TIME; 
#line 261 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 12:
		{
#line 83 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_ENVELOPE,yy);
                                                  return EXPR_ENVELOPE; 
#line 269 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 13:
		{
#line 85 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_VOLUME,yy);
                                                  return EXPR_VOLUME; 
#line 277 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 14:
		{
#line 87 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_SIN,yy);
                                                  return EXPR_SIN; 
#line 285 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 15:
		{
#line 89 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_COS,yy);
                                                  return EXPR_COS; 
#line 293 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 16:
		{
#line 91 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_TG,yy);
                                                  return EXPR_TG; 
#line 301 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 17:
		{
#line 93 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_CTG,yy);
                                                  return EXPR_CTG; 
#line 309 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 18:
		{
#line 95 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_ASIN,yy);
                                                  return EXPR_ASIN; 
#line 317 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 19:
		{
#line 97 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_ACOS,yy);
                                                  return EXPR_ACOS; 
#line 325 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 20:
		{
#line 99 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_ATAN,yy);
                                                  return EXPR_ATAN; 
#line 333 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 21:
		{
#line 101 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_SQRT,yy);
                                                  return EXPR_SQRT; 
#line 341 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 22:
		{
#line 103 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_EXP,yy);
                                                  return EXPR_EXP; 
#line 349 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 23:
		{
#line 105 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_LN,yy);
                                                  return EXPR_LN; 
#line 357 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 24:
		{
#line 107 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_FLOOR,yy);
                                                  return EXPR_FLOOR; 
#line 365 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 25:
		{
#line 109 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_CEIL,yy);
                                                  return EXPR_CEIL; 
#line 373 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 26:
		{
#line 111 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_ABS,yy);
                                                  return EXPR_ABS; 
#line 381 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 27:
		{
#line 113 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_FFT,yy);
                                                  return EXPR_FFT; 
#line 389 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 28:
		{
#line 115 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_COND_START,yy);
                                                  return EXPR_COND_START; 
#line 397 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 29:
		{
#line 117 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_COND_END,yy);
                                                  return EXPR_COND_END; 
#line 405 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 30:
		{
#line 119 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_EQUAL,yy);
                                                  return EXPR_EQUAL;
#line 413 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 31:
		{
#line 121 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_GREATER,yy);
                                                  return EXPR_GREATER;
#line 421 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 32:
		{
#line 123 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_LESS,yy);
                                                  return EXPR_LESS;
#line 429 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 33:
		{
#line 125 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_GEQUAL,yy);
                                                  return EXPR_GEQUAL;
#line 437 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 34:
		{
#line 127 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_LEQUAL,yy);
                                                  return EXPR_LEQUAL;
#line 445 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 35:
		{
#line 129 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_NOTEQUAL,yy);
                                                  return EXPR_NOTEQUAL;
#line 453 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 36:
		{
#line 131 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_MATERIAL_CHANNEL,yy);
                                                  return EXPR_MATERIAL_CHANNEL;
#line 461 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 37:
		{
#line 133 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_MESH_NODE_CHANNEL,yy);
                                                  return EXPR_MESH_NODE_CHANNEL;
#line 469 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 38:
		{
#line 135 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_TRANSFORM_NODE_CHANNEL,yy);
                                                  return EXPR_TRANSFORM_NODE_CHANNEL;
#line 477 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 39:
		{
#line 137 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 lexAddToken(EXPR_INPUT_VARIABLE,yy);
                                                  yylval=lexTokenCount-1;
											      return EXPR_INPUT_VARIABLE;
#line 486 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 40:
		{
#line 140 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 return EXPR_PARSE_ERROR; 
#line 493 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
	case 41:
		{
#line 141 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"

#line 500 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
		}
		break;
#line 143 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 /***************
#line 144 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 regexp's+actions
#line 145 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 ****************/
#line 146 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"
 
#line 511 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
	default:
		yyassert(0);
		break;
	}
	yy->yymreturnflg = 0;
	return 0;
}

#ifndef YYNBORLANDWARN
#ifdef __BORLANDC__
#pragma warn .rch		/* <warning: unreachable code> to the old state */
#endif
#endif


#line 147 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.l"

#line 529 "D:\\MyProjects\\LiquidReality\\LRCore\\lex_expr_lexer.c"
static YYCONST yymatch_t YYNEARFAR YYBASED_CODE yymatch[] = {
	0
};

#define YYTRANSITIONMAX 817
static YYCONST yytransition_t YYNEARFAR YYBASED_CODE yytransition[] = {
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 78, 57 },
	{ 80, 58 },
	{ 76, 55 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 77, 56 },
	{ 60, 34 },
	{ 0, 0 },
	{ 53, 26 },
	{ 54, 26 },
	{ 51, 24 },
	{ 52, 25 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 83, 83 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 47, 19 },
	{ 61, 35 },
	{ 85, 62 },
	{ 86, 64 },
	{ 75, 50 },
	{ 89, 70 },
	{ 65, 39 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 92, 92 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 72, 46 },
	{ 64, 38 },
	{ 97, 91 },
	{ 73, 47 },
	{ 105, 101 },
	{ 70, 44 },
	{ 74, 49 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 93, 93 },
	{ 94, 100 },
	{ 36, 14 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 101, 95 },
	{ 91, 74 },
	{ 88, 69 },
	{ 35, 13 },
	{ 90, 71 },
	{ 68, 42 },
	{ 63, 37 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 100, 100 },
	{ 98, 103 },
	{ 48, 20 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 50, 21 },
	{ 96, 85 },
	{ 59, 33 },
	{ 87, 67 },
	{ 62, 36 },
	{ 95, 84 },
	{ 49, 21 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 103, 103 },
	{ 99, 104 },
	{ 0, 4 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 69, 43 },
	{ 67, 41 },
	{ 66, 40 },
	{ 84, 61 },
	{ 82, 59 },
	{ 102, 96 },
	{ 106, 105 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 104, 104 },
	{ 30, 1 },
	{ 34, 12 },
	{ 31, 31 },
	{ 31, 31 },
	{ 31, 31 },
	{ 31, 31 },
	{ 31, 31 },
	{ 31, 31 },
	{ 31, 31 },
	{ 31, 31 },
	{ 31, 31 },
	{ 31, 31 },
	{ 46, 18 },
	{ 44, 18 },
	{ 37, 15 },
	{ 42, 17 },
	{ 71, 45 },
	{ 0, 0 },
	{ 56, 27 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 38, 15 },
	{ 30, 1 },
	{ 55, 27 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 33, 12 },
	{ 43, 18 },
	{ 45, 18 },
	{ 9, 1 },
	{ 10, 1 },
	{ 7, 1 },
	{ 6, 1 },
	{ 0, 0 },
	{ 5, 1 },
	{ 4, 1 },
	{ 8, 1 },
	{ 3, 1 },
	{ 3, 1 },
	{ 3, 1 },
	{ 3, 1 },
	{ 3, 1 },
	{ 3, 1 },
	{ 3, 1 },
	{ 3, 1 },
	{ 3, 1 },
	{ 3, 1 },
	{ 23, 1 },
	{ 0, 0 },
	{ 26, 1 },
	{ 24, 1 },
	{ 25, 1 },
	{ 22, 1 },
	{ 57, 31 },
	{ 28, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 13, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 27, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 12, 1 },
	{ 28, 1 },
	{ 14, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 28, 1 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 11, 1 },
	{ 41, 16 },
	{ 0, 0 },
	{ 18, 1 },
	{ 0, 0 },
	{ 16, 1 },
	{ 0, 0 },
	{ 19, 1 },
	{ 21, 1 },
	{ 0, 0 },
	{ 0, 0 },
	{ 39, 16 },
	{ 0, 0 },
	{ 0, 0 },
	{ 20, 1 },
	{ 0, 0 },
	{ 40, 16 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 15, 1 },
	{ 17, 1 },
	{ 0, 94 },
	{ 0, 0 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 98 },
	{ 0, 0 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 99 },
	{ 0, 0 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 94 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 98 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 99 },
	{ 0, 60 },
	{ 0, 0 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 76 },
	{ 0, 0 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 77 },
	{ 0, 0 },
	{ 0, 77 },
	{ 0, 77 },
	{ 0, 77 },
	{ 0, 77 },
	{ 0, 77 },
	{ 0, 77 },
	{ 0, 77 },
	{ 0, 77 },
	{ 0, 77 },
	{ 0, 77 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 60 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 76 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 77 },
	{ 0, 77 },
	{ 0, 77 },
	{ 0, 77 },
	{ 0, 77 },
	{ 0, 77 },
	{ 32, 32 },
	{ 32, 32 },
	{ 32, 32 },
	{ 32, 32 },
	{ 32, 32 },
	{ 32, 32 },
	{ 32, 32 },
	{ 32, 32 },
	{ 32, 32 },
	{ 32, 32 },
	{ 31, 3 },
	{ 0, 0 },
	{ 3, 3 },
	{ 3, 3 },
	{ 3, 3 },
	{ 3, 3 },
	{ 3, 3 },
	{ 3, 3 },
	{ 3, 3 },
	{ 3, 3 },
	{ 3, 3 },
	{ 3, 3 },
	{ 79, 79 },
	{ 79, 79 },
	{ 79, 79 },
	{ 79, 79 },
	{ 79, 79 },
	{ 79, 79 },
	{ 79, 79 },
	{ 79, 79 },
	{ 79, 79 },
	{ 79, 79 },
	{ 81, 81 },
	{ 81, 81 },
	{ 81, 81 },
	{ 81, 81 },
	{ 81, 81 },
	{ 81, 81 },
	{ 81, 81 },
	{ 81, 81 },
	{ 81, 81 },
	{ 81, 81 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 58, 32 }
};

static YYCONST yystate_t YYNEARFAR YYBASED_CODE yystate[] = {
	{ 0, 0, 0 },
	{ -29, 496, 0 },
	{ 1, 0, 0 },
	{ 81, 727, 1 },
	{ 32, 328, 40 },
	{ 0, 0, 4 },
	{ 0, 0, 5 },
	{ 0, 0, 6 },
	{ 0, 0, 7 },
	{ 0, 0, 8 },
	{ 0, 0, 9 },
	{ 0, 0, 10 },
	{ 43, 428, 39 },
	{ 71, 179, 39 },
	{ 95, 164, 39 },
	{ 43, 414, 40 },
	{ 105, 490, 40 },
	{ 45, 423, 40 },
	{ 70, 419, 40 },
	{ 0, 14, 40 },
	{ 71, 242, 40 },
	{ 84, 261, 40 },
	{ 0, 0, 28 },
	{ 0, 0, 29 },
	{ 25, 3, 40 },
	{ 0, 4, 31 },
	{ 25, 1, 32 },
	{ 0, 445, 39 },
	{ 0, 0, 39 },
	{ 0, 0, 40 },
	{ 0, 0, 41 },
	{ 105, 459, 2 },
	{ 105, 715, 3 },
	{ 85, 256, 0 },
	{ 56, 14, 0 },
	{ 0, 17, 0 },
	{ 84, 259, 0 },
	{ 71, 182, 0 },
	{ 91, 96, 0 },
	{ 70, 25, 0 },
	{ 0, 339, 0 },
	{ 43, 336, 0 },
	{ 71, 181, 0 },
	{ 0, 335, 0 },
	{ 95, 103, 0 },
	{ 0, 424, 0 },
	{ 70, 94, 0 },
	{ 101, 100, 0 },
	{ 0, 0, 23 },
	{ 95, 104, 0 },
	{ 64, 22, 0 },
	{ 0, 0, 30 },
	{ 0, 0, 33 },
	{ 0, 0, 34 },
	{ 0, 0, 35 },
	{ 56, 2, 0 },
	{ 0, 13, 0 },
	{ 79, 1, 0 },
	{ 81, 2, 0 },
	{ 105, 343, 0 },
	{ 83, 642, 0 },
	{ 105, 342, 0 },
	{ 0, 19, 0 },
	{ 0, 0, 14 },
	{ 0, 21, 0 },
	{ 0, 0, 15 },
	{ 0, 0, 17 },
	{ 84, 258, 0 },
	{ 0, 0, 16 },
	{ 71, 178, 0 },
	{ 0, 24, 0 },
	{ 0, 180, 0 },
	{ 0, 0, 26 },
	{ 0, 0, 22 },
	{ 95, 176, 0 },
	{ 0, 0, 27 },
	{ 92, 654, 0 },
	{ 93, 666, 0 },
	{ 79, 0, 0 },
	{ 81, 737, 2 },
	{ 81, 0, 0 },
	{ 4, 747, 3 },
	{ 0, 0, 11 },
	{ 100, 1, 0 },
	{ 0, 260, 0 },
	{ 0, 255, 0 },
	{ 0, 0, 21 },
	{ 0, 0, 25 },
	{ 0, 0, 18 },
	{ 0, 0, 19 },
	{ 0, 0, 20 },
	{ 0, 97, 0 },
	{ 103, 76, 0 },
	{ 104, 151, 0 },
	{ 100, 567, 0 },
	{ 0, 175, 0 },
	{ 105, 344, 0 },
	{ 0, 0, 24 },
	{ 103, 579, 0 },
	{ 104, 591, 0 },
	{ 105, 228, 38 },
	{ 0, 101, 0 },
	{ 0, 0, 13 },
	{ 105, 305, 36 },
	{ 105, 382, 37 },
	{ 0, 345, 0 },
	{ 0, 0, 12 }
};

static YYCONST yybackup_t YYNEARFAR YYBASED_CODE yybackup[] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

YYCONST yymlex_t YYNEAR YYLEXNAME = {
	0,					/* yymmoreflg */
	0,					/* yymrejectflg */
	0,					/* yymreturnflg */
	1,					/* yymtextgrow */
	1,					/* yymunputgrow */
	1,					/* yymeol */
	1,					/* yymoldeol */
	0,					/* yymstart */
	YYMIN,				/* yymin */
	YYMOUT,				/* yymout */
	YYMLEXERR,			/* yymerr */
	0,					/* yymleng */
	1,					/* yymlineno */
	0,					/* yymunputindex */
	NULL,				/* yymtext */
	NULL,				/* yymstext */
	YYTEXT_SIZE,		/* yymtext_size */
	YYTEXT_SIZE,		/* yymstext_size */
	NULL,				/* yymstatebuf */
	NULL,				/* yymsstatebuf */
	NULL,				/* yymunputbufptr */
	NULL,				/* yymsunputbufptr */
	YYUNPUT_SIZE,		/* yymunput_size */
	YYUNPUT_SIZE,		/* yymsunput_size */
	YYMINPUT,			/* yyminput */
	YYMOUTPUT,			/* yymoutput */
	YYMUNPUT,			/* yymunput */
	YYMWRAP,			/* yymwrap */
	YYMGETCHAR,			/* yymgetchar */
	YYMTEXTOVERFLOW,	/* yymtextoverflow */
	YYMUNPUTOVERFLOW,	/* yymunputoverflow */
	yylexaction,		/* yymlexaction */
#ifndef YYLEXCOMPACT
	yytransition,		/* yymtransition */
	YYTRANSITIONMAX,	/* yymtransitionmax */
	NULL,				/* yymctransition */
#else
	NULL,				/* yymtransition */
	0,					/* yymtransitionmax */
	yyctransition,		/* yymctransition */
#endif
	yymatch,			/* yymmatch */
	yystate,			/* yymstate */
	yybackup,			/* yymbackup */
	YYMLEXDATA			/* yymdata */

	/* debugging */
#ifdef YYDEBUG
	,
	0,					/* yymdebug */
	0,					/* yymdebugflush */
	YYMLEXDEBUGOUT		/* yymdebugout */
#endif
};
