/*********************************************************************
 LiquidReality engine expression assembly, core of expression
 system - describes result of yacc work
 (c) 2001 Maciej Mroz
 *********************************************************************/

#ifndef _LR_EXPR_ASSEMBLY_H_INCLUDED_
#define _LR_EXPR_ASSEMBLY_H_INCLUDED_

//opcodes
#define EASM_RET		0
#define EASM_ENVL		1
#define EASM_TIME		2
#define EASM_VOL		3
#define EASM_PUSH		4
#define EASM_ADD		5
#define EASM_SUB		6
#define EASM_MUL		7
#define EASM_DIV		8
#define EASM_POW		9
#define EASM_SIN		10
#define EASM_COS		11
#define EASM_TAN		12
#define EASM_CTG		13
#define EASM_ASIN		14
#define EASM_ACOS		15
#define EASM_ATAN		16
#define EASM_SQRT		17
#define EASM_EXP		18
#define EASM_LN			19
#define EASM_FLR		20
#define EASM_CEIL		21
#define EASM_ABS		22
#define EASM_FFT		23
#define EASM_NEG		24
#define EASM_EQU		25
#define EASM_NEQU		26
#define EASM_GT			27
#define EASM_GTE		28
#define EASM_LT			29
#define EASM_LTE		30
#define EASM_COND		31
#define EASM_EVAL		32
#define EASM_VARIABLE	33

struct ParseExprNode {
	int						opcode;
	char*					token_val;
	struct ParseExprNode*	pNextNode;
};

//these should be set manually before calling yacc
extern struct ParseExprNode* pExprParserOutput;

#endif