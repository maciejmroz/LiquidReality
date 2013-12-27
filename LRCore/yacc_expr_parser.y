%{
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
%}

%name expr_parser		// give the parser a name

%token EXPR_FPNUMBER
%left  EXPR_MINUS
%left  EXPR_PLUS
%left  EXPR_MULTIPLY
%left  EXPR_DIVIDE
%token EXPR_LBRACKET
%token EXPR_RBRACKET
%right EXPR_POWER
%token EXPR_TIME
%token EXPR_ENVELOPE
%token EXPR_VOLUME
%token EXPR_SIN
%token EXPR_COS
%token EXPR_TG
%token EXPR_CTG
%token EXPR_ASIN
%token EXPR_ACOS
%token EXPR_ATAN
%token EXPR_SQRT
%token EXPR_EXP
%token EXPR_LN
%token EXPR_FLOOR
%token EXPR_CEIL
%token EXPR_ABS
%token EXPR_FFT
%token EXPR_CHANNEL
%token EXPR_MATERIAL_CHANNEL
%token EXPR_MESH_NODE_CHANNEL
%token EXPR_TRANSFORM_NODE_CHANNEL
%token EXPR_INPUT_VARIABLE
%token EXPR_COND_START
%token EXPR_COND_END
%left EXPR_EQUAL
%left EXPR_GREATER
%left EXPR_GEQUAL
%left EXPR_LESS
%left EXPR_LEQUAL
%left EXPR_NOTEQUAL
%token EXPR_PARSE_ERROR

%%
wyrazenie     :  wyrazenie EXPR_COND_START wyrazenie EXPR_COND_END wyrazenie { yaccEmitExprOp(EASM_COND,0); }
              |  wyrazenie_zwykle
              ;
wyrazenie_zwykle     :  wyrazenie_zwykle op_warunku warunek { yaccEmitExprOp($2,0); }
                     |  warunek
                     ;
op_warunku    :  EXPR_EQUAL { $$=EASM_EQU; }
              |  EXPR_GREATER { $$=EASM_GT; }
              |  EXPR_GEQUAL  { $$=EASM_GTE; }
              |  EXPR_LESS { $$=EASM_LT; }
              |  EXPR_LEQUAL { $$=EASM_LTE; }
              |  EXPR_NOTEQUAL { $$=EASM_NEQU; }
              ;

warunek       :  warunek   EXPR_PLUS   skladnik { yaccEmitExprOp(EASM_ADD,0);}
              |  warunek   EXPR_MINUS  skladnik { yaccEmitExprOp(EASM_SUB,0);}
              |  skladnik {}
              ;
skladnik      :  skladnik EXPR_MULTIPLY czynnik { yaccEmitExprOp(EASM_MUL,0);}
              |  skladnik EXPR_DIVIDE   czynnik { yaccEmitExprOp(EASM_DIV,0);}
              |  czynnik
              ;
czynnik       :  czynnik EXPR_POWER wykladnik { yaccEmitExprOp(EASM_POW,0);}
              |  wykladnik {}
              ;
wykladnik     :  funkcja argument { yaccEmitExprOp($1,0); }
              |  argument {}
              ;
funkcja       :  EXPR_SIN { $$=EASM_SIN; }
              |  EXPR_COS { $$=EASM_COS; }
              |  EXPR_TG { $$=EASM_TAN; }
              |  EXPR_CTG { $$=EASM_CTG; }
              |  EXPR_ASIN { $$=EASM_ASIN; }
              |  EXPR_ACOS { $$=EASM_ACOS; }
              |  EXPR_ATAN { $$=EASM_ATAN; }
              |  EXPR_SQRT { $$=EASM_SQRT; }
              |  EXPR_EXP { $$=EASM_EXP; }
              |  EXPR_LN { $$=EASM_LN; }
              |  EXPR_FLOOR { $$=EASM_FLR; }
              |  EXPR_CEIL { $$=EASM_CEIL; }
              |  EXPR_ABS { $$=EASM_ABS; }
              |  EXPR_FFT { $$=EASM_FFT; }
              |  EXPR_CHANNEL { $$=EASM_EVAL; }
              |  EXPR_MATERIAL_CHANNEL { $$=EASM_EVAL; }
              |  EXPR_MESH_NODE_CHANNEL { $$=EASM_EVAL; }
              |  EXPR_TRANSFORM_NODE_CHANNEL { $$=EASM_EVAL; }
              |  EXPR_MINUS { $$=EASM_NEG; }
              ;
              
argument      :  EXPR_LBRACKET wyrazenie EXPR_RBRACKET {}
              |  EXPR_FPNUMBER { yaccEmitExprOp(EASM_PUSH,$1); }
              |  EXPR_TIME { yaccEmitExprOp(EASM_TIME,0); }
              |  EXPR_ENVELOPE { yaccEmitExprOp(EASM_ENVL,0); }
              |  EXPR_VOLUME { yaccEmitExprOp(EASM_VOL,0); }
              |  EXPR_INPUT_VARIABLE { yaccEmitExprOp(EASM_VARIABLE,$1); }
              ;
%%

#include "lex_expr_lexer.c"
