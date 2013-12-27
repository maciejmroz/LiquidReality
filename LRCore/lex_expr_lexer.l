%{
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
%}

%name expr_lexer

%include {

//or however it is called
#include "expr_token_structs.h"

void lexZeroTokenCount();

extern lexExprToken lexExpressionTbl[MAX_TOKEN_COUNT];

}

%%

%{
YYSTYPE *yymlvalptr = (YYSTYPE *) yy->yymdata;
yyassert(yymlvalptr != NULL);
#define yylval (*yymlvalptr)
%}

[0-9]+                         |
[0-9]+\.[0-9]*(e-?[0-9]+)?     |
[0-9]*\.[0-9]+(e-?[0-9]+)?                      { lexAddToken(EXPR_FPNUMBER,yy);
                                                  yylval=lexTokenCount-1;
                                                  return EXPR_FPNUMBER; }
"-"                                             { lexAddToken(EXPR_MINUS,yy);
                                                  return EXPR_MINUS; }
"+"                                             { lexAddToken(EXPR_PLUS,yy); 
                                                  return EXPR_PLUS; }
"*"                                             { lexAddToken(EXPR_MULTIPLY,yy);
                                                  return EXPR_MULTIPLY; }
"/"                                             { lexAddToken(EXPR_DIVIDE,yy);
                                                  return EXPR_DIVIDE; }
"("                                             { lexAddToken(EXPR_LBRACKET,yy);
                                                  return EXPR_LBRACKET; }
")"                                             { lexAddToken(EXPR_RBRACKET,yy);
                                                  return EXPR_RBRACKET; }
"^"                                             { lexAddToken(EXPR_POWER,yy);
                                                  return EXPR_POWER; }
Time                                            { lexAddToken(EXPR_TIME,yy);
                                                  return EXPR_TIME; }
Envelope                                        { lexAddToken(EXPR_ENVELOPE,yy);
                                                  return EXPR_ENVELOPE; }
Volume                                          { lexAddToken(EXPR_VOLUME,yy);
                                                  return EXPR_VOLUME; }
sin                                             { lexAddToken(EXPR_SIN,yy);
                                                  return EXPR_SIN; }
cos                                             { lexAddToken(EXPR_COS,yy);
                                                  return EXPR_COS; }
tan                                              { lexAddToken(EXPR_TG,yy);
                                                  return EXPR_TG; }
ctg                                             { lexAddToken(EXPR_CTG,yy);
                                                  return EXPR_CTG; }
asin                                            { lexAddToken(EXPR_ASIN,yy);
                                                  return EXPR_ASIN; }
acos                                            { lexAddToken(EXPR_ACOS,yy);
                                                  return EXPR_ACOS; }
atan                                            { lexAddToken(EXPR_ATAN,yy);
                                                  return EXPR_ATAN; }
sqrt                                            { lexAddToken(EXPR_SQRT,yy);
                                                  return EXPR_SQRT; }
exp                                             { lexAddToken(EXPR_EXP,yy);
                                                  return EXPR_EXP; }
ln                                              { lexAddToken(EXPR_LN,yy);
                                                  return EXPR_LN; }
floor                                           { lexAddToken(EXPR_FLOOR,yy);
                                                  return EXPR_FLOOR; }
ceil                                            { lexAddToken(EXPR_CEIL,yy);
                                                  return EXPR_CEIL; }
abs                                             { lexAddToken(EXPR_ABS,yy);
                                                  return EXPR_ABS; }
fft                                             { lexAddToken(EXPR_FFT,yy);
                                                  return EXPR_FFT; }
"?"                                             { lexAddToken(EXPR_COND_START,yy);
                                                  return EXPR_COND_START; }
":"                                             { lexAddToken(EXPR_COND_END,yy);
                                                  return EXPR_COND_END; }
"=="                                            { lexAddToken(EXPR_EQUAL,yy);
                                                  return EXPR_EQUAL;}
">"                                             { lexAddToken(EXPR_GREATER,yy);
                                                  return EXPR_GREATER;}
"<"                                             { lexAddToken(EXPR_LESS,yy);
                                                  return EXPR_LESS;}
">="                                            { lexAddToken(EXPR_GEQUAL,yy);
                                                  return EXPR_GEQUAL;}
"<="                                            { lexAddToken(EXPR_LEQUAL,yy);
                                                  return EXPR_LEQUAL;}
"<>"                                            { lexAddToken(EXPR_NOTEQUAL,yy);
                                                  return EXPR_NOTEQUAL;}
MT(\.[a-zA-Z]+[a-zA-z0-9]*){2,}                 { lexAddToken(EXPR_MATERIAL_CHANNEL,yy);
                                                  return EXPR_MATERIAL_CHANNEL;}
MN(\.[a-zA-Z]+[a-zA-z0-9]*){2,}                 { lexAddToken(EXPR_MESH_NODE_CHANNEL,yy);
                                                  return EXPR_MESH_NODE_CHANNEL;}
TN(\.[a-zA-Z]+[a-zA-z0-9]*){2,}                 { lexAddToken(EXPR_TRANSFORM_NODE_CHANNEL,yy);
                                                  return EXPR_TRANSFORM_NODE_CHANNEL;}
[A-Z]											{ lexAddToken(EXPR_INPUT_VARIABLE,yy);
                                                  yylval=lexTokenCount-1;
											      return EXPR_INPUT_VARIABLE;}
[^ \t]                                          { return EXPR_PARSE_ERROR; }
[ \t]                                           {}

 /***************
 regexp's+actions
 ****************/
 
%%