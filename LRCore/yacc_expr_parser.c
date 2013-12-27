#include <myacc.h>

#define YYPARSEFAST
#line 1 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"

/************************************************************
 My very first parser
************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "expr_token_structs.h"

extern lexExprToken lexExpressionTbl[MAX_TOKEN_COUNT];

#include "LRExprAssembly.h"

static struct ParseExprNode	*pExprLastAddedNode;

void yaccEmitExprOp(int iType, int iTokenNum) {
 if(pExprParserOutput==NULL) {
  pExprParserOutput=(struct ParseExprNode*)malloc(sizeof(struct ParseExprNode));
  pExprLastAddedNode=pExprParserOutput;
  pExprLastAddedNode->pNextNode=NULL;
  pExprLastAddedNode->opcode=iType;
  pExprLastAddedNode->token_val=
    (char*)malloc(strlen(lexExpressionTbl[iTokenNum].szVal)+1);
  strcpy(pExprLastAddedNode->token_val,lexExpressionTbl[iTokenNum].szVal);
 }
 else {
  ((*pExprLastAddedNode).pNextNode)=((struct ParseExprNode*)malloc(sizeof(struct ParseExprNode)));
  pExprLastAddedNode=pExprLastAddedNode->pNextNode;
  pExprLastAddedNode->pNextNode=NULL;
  pExprLastAddedNode->opcode=iType;
  pExprLastAddedNode->token_val=
    (char*)malloc(strlen(lexExpressionTbl[iTokenNum].szVal)+1);
  strcpy(pExprLastAddedNode->token_val,lexExpressionTbl[iTokenNum].szVal);
 }
 //printf("Expression token emitted, id %d, value(string) %s\n",
 //        pExprLastAddedNode->opcode,
 //        pExprLastAddedNode->token_val);
}

#define YYSTYPE int

/* multiple instance yacc conversion file */
#include <miyconv.h>
#include <string.h>

#line 54 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
/* repeated because of possible precompiled header */
#include <myacc.h>

#define YYPARSEFAST
#include "yacc_expr_parser.h"

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#ifndef YYSTACK_SIZE
#define YYSTACK_SIZE 100
#endif

/* yyattribute */
#ifdef YYDEBUG
#ifdef YYPROTOTYPE
static YYSTYPE YYFAR *yyattribute1(yymparse_t YYFAR *yy, int index)
#else
static YYSTYPE YYFAR *yyattribute1(yy, index)
yymparse_t YYFAR *yy;
int index;
#endif
{
	YYSTYPE YYFAR *p;
	yyassert(yy != NULL);

	p = &((YYSTYPE YYFAR *) yy->yymattributestackptr)[yy->yymtop + index];
	return p;
}
#define yyattribute(yy, index) (*yyattribute1(yy, index))
#else
#define yyattribute(yy, index) (((YYSTYPE YYFAR *) yy->yymattributestackptr)[yy->yymtop + (index)])
#endif

#ifdef YYDEBUG
#ifdef YYPROTOTYPE
static void yyinitdebug(yymparse_t YYFAR *yy, YYSTYPE YYFAR **p, int count)
#else
static void yyinitdebug(yy, p, count)
yymparse_t YYFAR *yy;
YYSTYPE YYFAR **p;
int count;
#endif
{
	int i;
	yyassert(yy != NULL);
	yyassert(p != NULL);
	yyassert(count >= 1);

	for (i = 0; i < count; i++) {
		p[i] = &((YYSTYPE YYFAR *) yy->yymattributestackptr)[yy->yymtop + i - (count - 1)];
	}
}
#endif

/* variables */
#ifndef YYMPARSEERR
#ifndef YYNINITSTREAM
#define YYMPARSEERR stderr
#else
#define YYMPARSEERR NULL
#endif
#endif

#ifndef YYMPARSEDEBUGOUT
#ifndef YYNINITSTREAM
#define YYMPARSEDEBUGOUT stdout
#else
#define YYMPARSEDEBUGOUT NULL
#endif
#endif

/* pointers to functions */
#ifndef YYMSTACKOVERFLOW
#define YYMSTACKOVERFLOW yymstackoverflow
#endif
#ifndef YYMERROR
#define YYMERROR yymerror
#endif
#ifndef YYMSYNTAXERROR
#define YYMSYNTAXERROR yymsyntaxerror
#endif
#ifndef YYMDISCARD
#define YYMDISCARD yymdiscard
#endif
#ifndef YYMGETTOKEN
#define YYMGETTOKEN yymgettoken
#endif
#ifndef YYMPARSEDATA
#define YYMPARSEDATA NULL
#endif

#ifdef YYPROTOTYPE
static void YYCDECL yyparseaction(yymparse_t YYFAR *yy, int action)
#else
static void YYCDECL yyparseaction(yy, action)
yymparse_t YYFAR *yy;
int action;
#endif
{
	switch (action) {
	case 0:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[6];
			yyinitdebug(yy, yya, 6);
#endif
			{
#line 93 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 yaccEmitExprOp(EASM_COND,0); 
#line 165 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 1:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yy, yya, 4);
#endif
			{
#line 96 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 yaccEmitExprOp(yyattribute(yy, 2 - 3),0); 
#line 178 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 2:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 99 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_EQU; 
#line 191 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 3:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 100 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_GT; 
#line 204 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 4:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 101 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_GTE; 
#line 217 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 5:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 102 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_LT; 
#line 230 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 6:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 103 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_LTE; 
#line 243 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 7:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 104 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_NEQU; 
#line 256 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 8:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yy, yya, 4);
#endif
			{
#line 107 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 yaccEmitExprOp(EASM_ADD,0);
#line 269 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 9:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yy, yya, 4);
#endif
			{
#line 108 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 yaccEmitExprOp(EASM_SUB,0);
#line 282 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 10:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 109 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"

#line 295 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 11:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yy, yya, 4);
#endif
			{
#line 111 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 yaccEmitExprOp(EASM_MUL,0);
#line 308 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 12:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yy, yya, 4);
#endif
			{
#line 112 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 yaccEmitExprOp(EASM_DIV,0);
#line 321 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 13:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yy, yya, 4);
#endif
			{
#line 115 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 yaccEmitExprOp(EASM_POW,0);
#line 334 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 14:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 116 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"

#line 347 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 15:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[3];
			yyinitdebug(yy, yya, 3);
#endif
			{
#line 118 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 yaccEmitExprOp(yyattribute(yy, 1 - 2),0); 
#line 360 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 16:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 119 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"

#line 373 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 17:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 121 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_SIN; 
#line 386 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 18:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 122 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_COS; 
#line 399 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 19:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 123 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_TAN; 
#line 412 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 20:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 124 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_CTG; 
#line 425 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 21:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 125 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_ASIN; 
#line 438 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 22:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 126 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_ACOS; 
#line 451 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 23:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 127 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_ATAN; 
#line 464 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 24:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 128 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_SQRT; 
#line 477 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 25:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 129 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_EXP; 
#line 490 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 26:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 130 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_LN; 
#line 503 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 27:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 131 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_FLR; 
#line 516 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 28:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 132 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_CEIL; 
#line 529 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 29:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 133 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_ABS; 
#line 542 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 30:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 134 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_FFT; 
#line 555 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 31:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 135 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_EVAL; 
#line 568 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 32:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 136 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_EVAL; 
#line 581 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 33:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 137 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_EVAL; 
#line 594 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 34:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 138 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_EVAL; 
#line 607 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 35:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 139 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 (*(YYSTYPE YYFAR *) yy->yymvalptr)=EASM_NEG; 
#line 620 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 36:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[4];
			yyinitdebug(yy, yya, 4);
#endif
			{
#line 142 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"

#line 633 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 37:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 143 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 yaccEmitExprOp(EASM_PUSH,yyattribute(yy, 1 - 1)); 
#line 646 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 38:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 144 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 yaccEmitExprOp(EASM_TIME,0); 
#line 659 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 39:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 145 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 yaccEmitExprOp(EASM_ENVL,0); 
#line 672 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 40:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 146 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 yaccEmitExprOp(EASM_VOL,0); 
#line 685 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	case 41:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR *yya[2];
			yyinitdebug(yy, yya, 2);
#endif
			{
#line 147 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"
 yaccEmitExprOp(EASM_VARIABLE,yyattribute(yy, 1 - 1)); 
#line 698 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
			}
		}
		break;
	default:
		yyassert(0);
		break;
	}
}

#line 149 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.y"


#include "lex_expr_lexer.c"

#line 713 "D:\\MyProjects\\LiquidReality\\LRCore\\yacc_expr_parser.c"
#ifdef YYDEBUG
static YYCONST yysymbol_t YYNEARFAR YYBASED_CODE yysymbol[] = {
	{ "$end", 0 },
	{ "error", 256 },
	{ "EXPR_FPNUMBER", 257 },
	{ "EXPR_MINUS", 258 },
	{ "EXPR_PLUS", 259 },
	{ "EXPR_MULTIPLY", 260 },
	{ "EXPR_DIVIDE", 261 },
	{ "EXPR_LBRACKET", 262 },
	{ "EXPR_RBRACKET", 263 },
	{ "EXPR_POWER", 264 },
	{ "EXPR_TIME", 265 },
	{ "EXPR_ENVELOPE", 266 },
	{ "EXPR_VOLUME", 267 },
	{ "EXPR_SIN", 268 },
	{ "EXPR_COS", 269 },
	{ "EXPR_TG", 270 },
	{ "EXPR_CTG", 271 },
	{ "EXPR_ASIN", 272 },
	{ "EXPR_ACOS", 273 },
	{ "EXPR_ATAN", 274 },
	{ "EXPR_SQRT", 275 },
	{ "EXPR_EXP", 276 },
	{ "EXPR_LN", 277 },
	{ "EXPR_FLOOR", 278 },
	{ "EXPR_CEIL", 279 },
	{ "EXPR_ABS", 280 },
	{ "EXPR_FFT", 281 },
	{ "EXPR_CHANNEL", 282 },
	{ "EXPR_MATERIAL_CHANNEL", 283 },
	{ "EXPR_MESH_NODE_CHANNEL", 284 },
	{ "EXPR_TRANSFORM_NODE_CHANNEL", 285 },
	{ "EXPR_INPUT_VARIABLE", 286 },
	{ "EXPR_COND_START", 287 },
	{ "EXPR_COND_END", 288 },
	{ "EXPR_EQUAL", 289 },
	{ "EXPR_GREATER", 290 },
	{ "EXPR_GEQUAL", 291 },
	{ "EXPR_LESS", 292 },
	{ "EXPR_LEQUAL", 293 },
	{ "EXPR_NOTEQUAL", 294 },
	{ NULL, 0 }
};

static YYCONST char *YYCONST YYNEARFAR YYBASED_CODE yyrule[] = {
	"$accept: wyrazenie",
	"wyrazenie: wyrazenie EXPR_COND_START wyrazenie EXPR_COND_END wyrazenie",
	"wyrazenie: wyrazenie_zwykle",
	"wyrazenie_zwykle: wyrazenie_zwykle op_warunku warunek",
	"wyrazenie_zwykle: warunek",
	"op_warunku: EXPR_EQUAL",
	"op_warunku: EXPR_GREATER",
	"op_warunku: EXPR_GEQUAL",
	"op_warunku: EXPR_LESS",
	"op_warunku: EXPR_LEQUAL",
	"op_warunku: EXPR_NOTEQUAL",
	"warunek: warunek EXPR_PLUS skladnik",
	"warunek: warunek EXPR_MINUS skladnik",
	"warunek: skladnik",
	"skladnik: skladnik EXPR_MULTIPLY czynnik",
	"skladnik: skladnik EXPR_DIVIDE czynnik",
	"skladnik: czynnik",
	"czynnik: czynnik EXPR_POWER wykladnik",
	"czynnik: wykladnik",
	"wykladnik: funkcja argument",
	"wykladnik: argument",
	"funkcja: EXPR_SIN",
	"funkcja: EXPR_COS",
	"funkcja: EXPR_TG",
	"funkcja: EXPR_CTG",
	"funkcja: EXPR_ASIN",
	"funkcja: EXPR_ACOS",
	"funkcja: EXPR_ATAN",
	"funkcja: EXPR_SQRT",
	"funkcja: EXPR_EXP",
	"funkcja: EXPR_LN",
	"funkcja: EXPR_FLOOR",
	"funkcja: EXPR_CEIL",
	"funkcja: EXPR_ABS",
	"funkcja: EXPR_FFT",
	"funkcja: EXPR_CHANNEL",
	"funkcja: EXPR_MATERIAL_CHANNEL",
	"funkcja: EXPR_MESH_NODE_CHANNEL",
	"funkcja: EXPR_TRANSFORM_NODE_CHANNEL",
	"funkcja: EXPR_MINUS",
	"argument: EXPR_LBRACKET wyrazenie EXPR_RBRACKET",
	"argument: EXPR_FPNUMBER",
	"argument: EXPR_TIME",
	"argument: EXPR_ENVELOPE",
	"argument: EXPR_VOLUME",
	"argument: EXPR_INPUT_VARIABLE"
};
#endif

static YYCONST yyreduction_t YYNEARFAR YYBASED_CODE yyreduction[] = {
	{ 0, 1, -1 },
	{ 1, 5, 0 },
	{ 1, 1, -1 },
	{ 2, 3, 1 },
	{ 2, 1, -1 },
	{ 3, 1, 2 },
	{ 3, 1, 3 },
	{ 3, 1, 4 },
	{ 3, 1, 5 },
	{ 3, 1, 6 },
	{ 3, 1, 7 },
	{ 4, 3, 8 },
	{ 4, 3, 9 },
	{ 4, 1, 10 },
	{ 5, 3, 11 },
	{ 5, 3, 12 },
	{ 5, 1, -1 },
	{ 6, 3, 13 },
	{ 6, 1, 14 },
	{ 7, 2, 15 },
	{ 7, 1, 16 },
	{ 8, 1, 17 },
	{ 8, 1, 18 },
	{ 8, 1, 19 },
	{ 8, 1, 20 },
	{ 8, 1, 21 },
	{ 8, 1, 22 },
	{ 8, 1, 23 },
	{ 8, 1, 24 },
	{ 8, 1, 25 },
	{ 8, 1, 26 },
	{ 8, 1, 27 },
	{ 8, 1, 28 },
	{ 8, 1, 29 },
	{ 8, 1, 30 },
	{ 8, 1, 31 },
	{ 8, 1, 32 },
	{ 8, 1, 33 },
	{ 8, 1, 34 },
	{ 8, 1, 35 },
	{ 9, 3, 36 },
	{ 9, 1, 37 },
	{ 9, 1, 38 },
	{ 9, 1, 39 },
	{ 9, 1, 40 },
	{ 9, 1, 41 }
};

static YYCONST yytokenaction_t YYNEARFAR YYBASED_CODE yytokenaction[] = {
	{ 26, YYAT_ACCEPT, 0 },
	{ 57, YYAT_SHIFT, 2 },
	{ 53, YYAT_SHIFT, 45 },
	{ 53, YYAT_SHIFT, 46 },
	{ 58, YYAT_SHIFT, 35 },
	{ 34, YYAT_SHIFT, 49 },
	{ 52, YYAT_SHIFT, 45 },
	{ 52, YYAT_SHIFT, 46 },
	{ 51, YYAT_SHIFT, 43 },
	{ 51, YYAT_SHIFT, 44 },
	{ 55, YYAT_SHIFT, 47 },
	{ 57, YYAT_SHIFT, 7 },
	{ 57, YYAT_SHIFT, 8 },
	{ 57, YYAT_SHIFT, 9 },
	{ 57, YYAT_SHIFT, 10 },
	{ 57, YYAT_SHIFT, 11 },
	{ 57, YYAT_SHIFT, 12 },
	{ 57, YYAT_SHIFT, 13 },
	{ 57, YYAT_SHIFT, 14 },
	{ 57, YYAT_SHIFT, 15 },
	{ 57, YYAT_SHIFT, 16 },
	{ 57, YYAT_SHIFT, 17 },
	{ 57, YYAT_SHIFT, 18 },
	{ 57, YYAT_SHIFT, 19 },
	{ 57, YYAT_SHIFT, 20 },
	{ 57, YYAT_SHIFT, 21 },
	{ 57, YYAT_SHIFT, 22 },
	{ 57, YYAT_SHIFT, 23 },
	{ 57, YYAT_SHIFT, 24 },
	{ 32, YYAT_SHIFT, 1 },
	{ 34, YYAT_ERROR, 0 },
	{ 50, YYAT_SHIFT, 35 },
	{ 50, YYAT_SHIFT, 57 },
	{ 54, YYAT_SHIFT, 47 },
	{ 32, YYAT_SHIFT, 3 },
	{ 29, YYAT_SHIFT, 45 },
	{ 29, YYAT_SHIFT, 46 },
	{ 32, YYAT_SHIFT, 4 },
	{ 32, YYAT_SHIFT, 5 },
	{ 32, YYAT_SHIFT, 6 },
	{ 27, YYAT_SHIFT, 36 },
	{ 27, YYAT_SHIFT, 37 },
	{ 27, YYAT_SHIFT, 38 },
	{ 27, YYAT_SHIFT, 39 },
	{ 27, YYAT_SHIFT, 40 },
	{ 27, YYAT_SHIFT, 41 },
	{ 28, YYAT_SHIFT, 43 },
	{ 28, YYAT_SHIFT, 44 },
	{ 30, YYAT_SHIFT, 47 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ 32, YYAT_SHIFT, 25 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ -1, YYAT_ERROR, 0 },
	{ 26, YYAT_ERROR, 0 }
};
#define YYTOKENACTION_SIZE 289

static YYCONST yystateaction_t YYNEARFAR YYBASED_CODE yystateaction[] = {
	{ 0, 0, YYAT_DEFAULT, 57 },
	{ 0, 0, YYAT_REDUCE, 41 },
	{ 0, 0, YYAT_REDUCE, 39 },
	{ 0, 0, YYAT_DEFAULT, 57 },
	{ 0, 0, YYAT_REDUCE, 42 },
	{ 0, 0, YYAT_REDUCE, 43 },
	{ 0, 0, YYAT_REDUCE, 44 },
	{ 0, 0, YYAT_REDUCE, 21 },
	{ 0, 0, YYAT_REDUCE, 22 },
	{ 0, 0, YYAT_REDUCE, 23 },
	{ 0, 0, YYAT_REDUCE, 24 },
	{ 0, 0, YYAT_REDUCE, 25 },
	{ 0, 0, YYAT_REDUCE, 26 },
	{ 0, 0, YYAT_REDUCE, 27 },
	{ 0, 0, YYAT_REDUCE, 28 },
	{ 0, 0, YYAT_REDUCE, 29 },
	{ 0, 0, YYAT_REDUCE, 30 },
	{ 0, 0, YYAT_REDUCE, 31 },
	{ 0, 0, YYAT_REDUCE, 32 },
	{ 0, 0, YYAT_REDUCE, 33 },
	{ 0, 0, YYAT_REDUCE, 34 },
	{ 0, 0, YYAT_REDUCE, 35 },
	{ 0, 0, YYAT_REDUCE, 36 },
	{ 0, 0, YYAT_REDUCE, 37 },
	{ 0, 0, YYAT_REDUCE, 38 },
	{ 0, 0, YYAT_REDUCE, 45 },
	{ 0, 1, YYAT_DEFAULT, 50 },
	{ -249, 1, YYAT_REDUCE, 2 },
	{ -212, 1, YYAT_REDUCE, 4 },
	{ -225, 1, YYAT_REDUCE, 13 },
	{ -216, 1, YYAT_REDUCE, 16 },
	{ 0, 0, YYAT_REDUCE, 18 },
	{ -228, 1, YYAT_ERROR, 0 },
	{ 0, 0, YYAT_REDUCE, 20 },
	{ -258, 1, YYAT_DEFAULT, 50 },
	{ 0, 0, YYAT_DEFAULT, 57 },
	{ 0, 0, YYAT_REDUCE, 5 },
	{ 0, 0, YYAT_REDUCE, 6 },
	{ 0, 0, YYAT_REDUCE, 7 },
	{ 0, 0, YYAT_REDUCE, 8 },
	{ 0, 0, YYAT_REDUCE, 9 },
	{ 0, 0, YYAT_REDUCE, 10 },
	{ 0, 0, YYAT_DEFAULT, 57 },
	{ 0, 0, YYAT_DEFAULT, 57 },
	{ 0, 0, YYAT_DEFAULT, 57 },
	{ 0, 0, YYAT_DEFAULT, 57 },
	{ 0, 0, YYAT_DEFAULT, 57 },
	{ 0, 0, YYAT_DEFAULT, 57 },
	{ 0, 0, YYAT_REDUCE, 19 },
	{ 0, 0, YYAT_REDUCE, 40 },
	{ -256, 1, YYAT_ERROR, 0 },
	{ -250, 1, YYAT_REDUCE, 3 },
	{ -254, 1, YYAT_REDUCE, 12 },
	{ -258, 1, YYAT_REDUCE, 11 },
	{ -231, 1, YYAT_REDUCE, 14 },
	{ -254, 1, YYAT_REDUCE, 15 },
	{ 0, 0, YYAT_REDUCE, 17 },
	{ -257, 1, YYAT_DEFAULT, 32 },
	{ -283, 1, YYAT_REDUCE, 1 }
};

static YYCONST yynontermgoto_t YYNEARFAR YYBASED_CODE yynontermgoto[] = {
	{ 57, 58 },
	{ 57, 27 },
	{ 45, 54 },
	{ 57, 28 },
	{ 47, 56 },
	{ 47, 32 },
	{ 47, 33 },
	{ 46, 55 },
	{ 46, 31 },
	{ 44, 53 },
	{ 44, 30 },
	{ 42, 51 },
	{ 42, 29 },
	{ 43, 52 },
	{ 35, 50 },
	{ 32, 48 },
	{ 27, 42 },
	{ 3, 34 },
	{ 0, 26 }
};
#define YYNONTERMGOTO_SIZE 19

static YYCONST yystategoto_t YYNEARFAR YYBASED_CODE yystategoto[] = {
	{ 17, 57 },
	{ 0, -1 },
	{ 0, -1 },
	{ 16, 57 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 13, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 6, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 13, 57 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 7, 44 },
	{ 8, 44 },
	{ 4, 46 },
	{ -4, 46 },
	{ 1, 47 },
	{ -3, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ 0, -1 },
	{ -1, 42 },
	{ 0, -1 }
};

#define YYDESTRUCTORPTR NULL

#define YYTOKENDESTPTR NULL
#define YYTOKENDEST_SIZE 0
#define YYTOKENDESTBASE 0

YYCONST yymparse_t YYNEAR YYPARSENAME = {
	0,							/* yymlookahead */
	1,							/* yymstackgrow */
	0,							/* yymexitflg */
	0,							/* yymretireflg */
	0,							/* yymerrorflg */
	0,							/* yympopflg */
	1,							/* yymwipe */
	-1,							/* yymtop */
	-1,							/* yymchar */
	0,							/* yymskip */
	0,							/* yymerrorcount */
	0,							/* yymexitcode */
	0,							/* yymretirecode */
	0,							/* yymerrorpop */
	YYMPARSEERR,				/* yymerr */
	NULL,						/* yymstackptr */
	NULL,						/* yymsstackptr */
	YYSTACK_SIZE,				/* yymstack_size */
	YYSTACK_SIZE,				/* yymsstack_size */

	sizeof(YYSTYPE),			/* yymattribute_size */

	NULL,						/* yymvalptr */
	NULL,						/* yymlvalptr  */
	NULL,						/* yymattributestackptr */
	NULL,						/* yymsattributestackptr */
	YYMSTACKOVERFLOW,			/* yymstackoverflow */
	YYMERROR,					/* yymerror */
	YYMSYNTAXERROR,				/* yymsyntaxerror */
	YYMDISCARD,					/* yymdiscard */
	YYMGETTOKEN,				/* yymgettoken */
	yyparseaction,				/* yymparseaction */

	/* tables */
#ifndef YYPARSECOMPACT
	yystateaction,				/* yymstateaction */
	yytokenaction,				/* yymtokenaction */
	YYTOKENACTION_SIZE,			/* yymtokenaction_size */
	yystategoto,				/* yymstategoto */
	yynontermgoto,				/* yymnontermgoto */
	YYNONTERMGOTO_SIZE,			/* yymnontermgoto_size */
	YYTOKENDESTPTR,				/* yymtokendestptr */
	YYTOKENDEST_SIZE,			/* yymtokendest_size */
	YYTOKENDESTBASE,			/* yymtokendestbase */
	NULL,						/* yymcstateaction */
	NULL,						/* yymctokenaction */
	NULL,						/* yymcstategoto */
	NULL,						/* yymcnontermgoto */
	NULL,						/* yymctokendestptr */
#else
	NULL,						/* yymstateaction */
	NULL,						/* yymtokenaction */
	0,							/* yymtokenaction_size */
	NULL,						/* yymstategoto */
	NULL,						/* yymnontermgoto */
	0,							/* yymnontermgoto_size */
	NULL,						/* yymtokendestptr */
	0,							/* yymtokendest_size */
	0,							/* yymtokendestbase */
	yycstateaction,				/* yymcstateaction */
	yyctokenaction,				/* yymctokenaction */
	yycstategoto,				/* yymcstategoto */
	yycnontermgoto,				/* yymcnontermgoto */
	YYCTOKENDESTPTR,			/* yymctokendestptr */
#endif
	yyreduction,				/* yymreduction */
	YYDESTRUCTORPTR,			/* yymdestructorptr */
	YYMPARSEDATA				/* yymdata */

	/* debugging */
#ifdef YYDEBUG
	,
	0,							/* yymdebug */
	0,							/* yymdebugstack */
	0,							/* yymdebugflush */
	YYMPARSEDEBUGOUT,			/* yymdebugout */
	yysymbol,					/* yymsymbol */
	yyrule						/* yymrule */
#endif
};
