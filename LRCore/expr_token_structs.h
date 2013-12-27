/***************************************************
 Token stuctures for expression lexer and parser
 (c) 2001 Maciej Mróz
 ***************************************************/

#ifndef _EXPRESSION_TOKENS_INCLUDED_
#define _EXPRESSION_TOKENS_INCLUDED_

#define MAX_TOKEN_LENGTH 100    //just as with lex
#define MAX_TOKEN_COUNT  128    //should be enough

//use 'lex' prefix for everything related to lexical analysis
typedef struct {
  int    Type;
  char   szVal[MAX_TOKEN_LENGTH];
} lexExprToken;

#endif