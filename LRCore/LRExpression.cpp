/*****************************************************************
 Expression parsing, compiling and evaluation for
 LiquidReality engine
 (c) 2001 Maciej Mróz
 *****************************************************************/

#include "StdAfx.h"
#include <string.h>
#include <math.h>

ParseExprNode	*pExprParserOutput=NULL;

using namespace LR;

void CreateExpression(LPVOID* ppv)
{
	*ppv = (LPVOID)new ExpressionImpl();
	((ExpressionImpl*)(*ppv))->AddRef();
}

/**********************************************
 Typical C++ stuff
 **********************************************/

ExpressionImpl::ExpressionImpl()
{
	m_pCompilerOutput=NULL;
	m_dwInstructionCount=0;
	m_pszExpression=NULL;
	m_pParserOutput=NULL;
	m_pExecutionStack=NULL;
	m_bParsingDone=FALSE;
	m_bCompilationDone=FALSE;
	m_bExpressionLoaded=FALSE;
	m_iStackSize=0;
}

ExpressionImpl::ExpressionImpl(const ExpressionImpl &rfExpr)
{
	//copy
	m_bCompilationDone=rfExpr.m_bCompilationDone;
	m_bExpressionLoaded=rfExpr.m_bExpressionLoaded;
	m_bParsingDone=rfExpr.m_bParsingDone;
	m_dwInstructionCount=rfExpr.m_dwInstructionCount;
	m_iStackSize=rfExpr.m_iStackSize;
	int i=0;
	for(;i<EXPR_INPUT_VAR_TBL_SIZE;i++)
	{
		m_InputVarTbl[i]=rfExpr.m_InputVarTbl[i];
	}
	if(rfExpr.m_pCompilerOutput)
	{
		m_pCompilerOutput=new ExprAsmInstruction[m_dwInstructionCount];
		memcpy(m_pCompilerOutput,rfExpr.m_pCompilerOutput,sizeof(ExprAsmInstruction)*m_dwInstructionCount);
	}
	else
	{
		m_pCompilerOutput=NULL;
	}
	if(rfExpr.m_pExecutionStack)
	{
		m_pExecutionStack=new float[m_iStackSize];
	}
	else
	{
		m_pExecutionStack=NULL;
	}
	if(rfExpr.m_pParserOutput)
	{
		ParseExprNode	*pNode1,*pNode2;
		pNode1=rfExpr.m_pParserOutput;
		pNode2=new ParseExprNode;
		m_pParserOutput=pNode2;
		pNode2->pNextNode=NULL;
		pNode2->opcode=pNode1->opcode;
		pNode2->token_val=new char[strlen(pNode1->token_val)+1];
		strcpy(pNode2->token_val,pNode1->token_val);
		while(pNode1->pNextNode)
		{
			pNode1=pNode1->pNextNode;
			pNode2->pNextNode=new ParseExprNode;
			pNode2=pNode2->pNextNode;
			pNode2->opcode=pNode1->opcode;
			pNode2->token_val=new char[strlen(pNode1->token_val)+1];
			strcpy(pNode2->token_val,pNode1->token_val);
			pNode2->pNextNode=NULL;
		}
	}
	else
	{
		m_pParserOutput=NULL;
	}
	if(rfExpr.m_pszExpression)
	{
		m_pszExpression=new char[strlen(rfExpr.m_pszExpression)+1];
		strcpy(m_pszExpression,rfExpr.m_pszExpression);
	}
	else
	{
		m_pszExpression=NULL;
	}
}

ExpressionImpl::~ExpressionImpl()
{
	clear();
}

void ExpressionImpl::operator=(const ExpressionImpl &rfExpr)
{
	//free
	if(m_pCompilerOutput)
	{
		delete [] m_pCompilerOutput;
		m_pCompilerOutput=NULL;
	}
	m_dwInstructionCount=0;
	if(m_pszExpression)
	{
		delete m_pszExpression;
		m_pszExpression=NULL;
	}
	if(m_pParserOutput)
	{
		ParseExprNode *pTempNode1,*pTempNode2;
		pTempNode1=m_pParserOutput;
		while(pTempNode1)
		{
			pTempNode2=pTempNode1->pNextNode;
			delete pTempNode1->token_val;
			delete pTempNode1;
			pTempNode1=pTempNode2;
		}
		m_pParserOutput=NULL;
	}
	if(m_pExecutionStack)
	{
		delete [] m_pExecutionStack;
		m_pExecutionStack=NULL;
	}
	m_iStackSize=0;
	//copy
	m_bCompilationDone=rfExpr.m_bCompilationDone;
	m_bExpressionLoaded=rfExpr.m_bExpressionLoaded;
	m_bParsingDone=rfExpr.m_bParsingDone;
	m_dwInstructionCount=rfExpr.m_dwInstructionCount;
	m_iStackSize=rfExpr.m_iStackSize;
	int i=0;
	for(;i<EXPR_INPUT_VAR_TBL_SIZE;i++)
	{
		m_InputVarTbl[i]=rfExpr.m_InputVarTbl[i];
	}
	if(rfExpr.m_pCompilerOutput)
	{
		m_pCompilerOutput=new ExprAsmInstruction[m_dwInstructionCount];
		memcpy(m_pCompilerOutput,rfExpr.m_pCompilerOutput,sizeof(ExprAsmInstruction)*m_dwInstructionCount);
	}
	else
	{
		m_pCompilerOutput=NULL;
	}
	if(rfExpr.m_pExecutionStack)
	{
		m_pExecutionStack=new float[m_iStackSize];
	}
	else
	{
		m_pExecutionStack=NULL;
	}
	if(rfExpr.m_pParserOutput)
	{
		ParseExprNode	*pNode1,*pNode2;
		pNode1=rfExpr.m_pParserOutput;
		pNode2=new ParseExprNode;
		m_pParserOutput=pNode2;
		pNode2->pNextNode=NULL;
		pNode2->opcode=pNode1->opcode;
		pNode2->token_val=new char[strlen(pNode1->token_val)+1];
		strcpy(pNode2->token_val,pNode1->token_val);
		while(pNode1->pNextNode)
		{
			pNode1=pNode1->pNextNode;
			pNode2->pNextNode=new ParseExprNode;
			pNode2=pNode2->pNextNode;
			pNode2->opcode=pNode1->opcode;
			pNode2->token_val=new char[strlen(pNode1->token_val)+1];
			strcpy(pNode2->token_val,pNode1->token_val);
			pNode2->pNextNode=NULL;
		}
	}
	else
	{
		m_pParserOutput=NULL;
	}
	if(rfExpr.m_pszExpression)
	{
		m_pszExpression=new char[strlen(rfExpr.m_pszExpression)+1];
		strcpy(m_pszExpression,rfExpr.m_pszExpression);
	}
	else
	{
		m_pszExpression=NULL;
	}
}

bool ExpressionImpl::operator==(const ExpressionImpl &rfExpr)
{
	if(m_bCompilationDone!=rfExpr.m_bCompilationDone)
	{
		return false;
	}
	if(m_bExpressionLoaded!=rfExpr.m_bExpressionLoaded)
	{
		return false;
	}
	if(m_bParsingDone!=rfExpr.m_bParsingDone)
	{
		return false;
	}
	if(m_dwInstructionCount!=rfExpr.m_dwInstructionCount)
	{
		return false;
	}
	if(m_iStackSize!=rfExpr.m_iStackSize)
	{
		return false;
	}

	DWORD	dwI=0;
	if(m_pCompilerOutput)
	{
		for(;dwI<m_dwInstructionCount;dwI++)
		{
			if(memcmp(&m_pCompilerOutput[dwI],
				&rfExpr.m_pCompilerOutput[dwI],
				sizeof(ExprAsmInstruction))!=0)
			{
				return false;
			}
		}
	}
	if(m_pParserOutput)
	{
		ParseExprNode	*pNode1,*pNode2;
		pNode1=m_pParserOutput;
		pNode2=rfExpr.m_pParserOutput;
		while(pNode1 && pNode2)
		{
			if(pNode1->opcode!=pNode2->opcode)
			{
				return false;
			}
			if(strcmp(pNode1->token_val,pNode2->token_val)!=0)
			{
				return false;
			}
			pNode1=pNode1->pNextNode;
			pNode2=pNode2->pNextNode;
		}
		if(pNode1 || pNode2)
		{
			return false;
		}
	}
	if(m_pszExpression)
	{
		if(strcmp(m_pszExpression,rfExpr.m_pszExpression)!=0)
		{
			return false;
		}
	}
	return true;
}

//int my_exprmgetchar(yymlex_t* pLex);
//void my_exprmerror(yymparse_t* pYacc,const char *text);

void ExpressionImpl::initParser()
{
/*	yymcreatelex(&ms_ExprLexer,&expr_lexer);
	//		yymlexinit(&ms_ExprLexer);
	ms_ExprLexer.yymgetchar=my_exprmgetchar;
	yymcreateparse(&ms_ExprParser,&expr_parser);
	//		yymparseinit(&ms_ExprParser);
	ms_ExprParser.yymdata=&ms_ExprLexer;
	ms_ExprLexer.yymdata=ms_ExprParser.yymlvalptr;
	ms_ExprParser.yymerror=my_exprmerror;*/
}

void ExpressionImpl::freeParser()
{
/*	yymdestroylex(&ms_ExprLexer);
	yymdestroyparse(&ms_ExprParser);*/
}

void ExpressionImpl::cleanupParser()
{
/*	yymlexcleanup(&ms_ExprLexer);
	yymparsecleanup(&ms_ExprParser);*/
}

//TODO: what is desired behavior when called with NULL pszExpr??
void ExpressionImpl::loadExpression(const char *pszExpr)
{
/*	m_bExpressionLoaded=FALSE;
	m_bParsingDone=FALSE;
	m_bCompilationDone=FALSE;
	if(m_pszExpression)
	{
		delete m_pszExpression;
		m_pszExpression=NULL;
	}
	if(pExprParserOutput)
	{
		ParseExprNode *pTempNode1,*pTempNode2;
		pTempNode1=pExprParserOutput;
		while(pTempNode1)
		{
			pTempNode2=pTempNode1->pNextNode;
			if(pTempNode1->token_val)
			{
				free(pTempNode1->token_val);
			}
			free(pTempNode1);
			pTempNode1=pTempNode2;
		}
		pExprParserOutput=NULL;
	}
	if(m_pCompilerOutput)
	{		//Release operates on compiler output but requires old parsing output
		ParseExprNode *pTempNode;
		pTempNode=m_pParserOutput;
		DWORD dwI;
		for(dwI=0;dwI<m_dwInstructionCount;dwI++)
		{
			if(pTempNode->opcode==EASM_EVAL)
			{
				if((Envelope*)m_pCompilerOutput[dwI].pSomething)
				{
					((Envelope*)m_pCompilerOutput[dwI].pSomething)->Release();
					m_pCompilerOutput[dwI].pSomething=NULL;
				}
			}
			pTempNode=pTempNode->pNextNode;
		}
	}
	if(m_pCompilerOutput)
	{
		delete [] m_pCompilerOutput;
		m_pCompilerOutput=FALSE;
	}
	if(m_pParserOutput)
	{
		ParseExprNode *pTempNode1,*pTempNode2;
		pTempNode1=m_pParserOutput;
		while(pTempNode1)
		{
			pTempNode2=pTempNode1->pNextNode;
			if(pTempNode1->token_val)
			{
				delete pTempNode1->token_val;
			}
			delete pTempNode1;
			pTempNode1=pTempNode2;
		}
		m_pParserOutput=NULL;
	}
	if(!pszExpr)
	{
		return;
	}
	m_pszExpression=new char[strlen(pszExpr)+1];
	strcpy(m_pszExpression,pszExpr);
	m_lstrExpression=m_pszExpression;
	m_bExpressionLoaded=TRUE;*/
}

const char* ExpressionImpl::getExpressionString()
{
	return m_pszExpression;
}

void ExpressionImpl::assign(const ExpressionPtr pILRExpr)
{
	ExpressionImpl *pExpr=dynamic_cast<ExpressionImpl*>(pILRExpr);
	if(pExpr)
	{
		if(pExpr!=this)
		{
			this->operator=(*pExpr);
		}
	}
	else
	{
		clear();
		//TODO: should probably throw exception here
	}
}

bool ExpressionImpl::equals(const ExpressionPtr pILRExpr)
{
	ExpressionImpl *pExpr=dynamic_cast<ExpressionImpl*>(pILRExpr);
	if(pExpr)
	{
		if(this->operator==(*pExpr))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void ExpressionImpl::clear()
{
/*	if(m_pCompilerOutput)
	{		//Release operates on compiler output but requires old parsing output
		ParseExprNode *pTempNode;
		pTempNode=m_pParserOutput;
		DWORD dwI;
		for(dwI=0;dwI<m_dwInstructionCount;dwI++)
		{
			if(pTempNode->opcode==EASM_EVAL)
			{
				if((Envelope*)m_pCompilerOutput[dwI].pSomething)
				{
					((Envelope*)m_pCompilerOutput[dwI].pSomething)->Release();
					m_pCompilerOutput[dwI].pSomething=NULL;
				}
			}
			pTempNode=pTempNode->pNextNode;
		}
	}
	if(m_pCompilerOutput)
	{
		delete [] m_pCompilerOutput;
		m_pCompilerOutput=NULL;
	}
	m_dwInstructionCount=0;
	if(m_pszExpression)
	{
		delete [] m_pszExpression;
		m_pszExpression=NULL;
	}
	if(m_pParserOutput)
	{
		ParseExprNode *pTempNode1,*pTempNode2;
		pTempNode1=m_pParserOutput;
		while(pTempNode1)
		{
			pTempNode2=pTempNode1->pNextNode;
			delete pTempNode1->token_val;
			delete pTempNode1;
			pTempNode1=pTempNode2;
		}
		m_pParserOutput=NULL;
	}
	if(m_pExecutionStack)
	{
		delete [] m_pExecutionStack;
		m_pExecutionStack=NULL;
	}
	m_bCompilationDone=FALSE;
	m_bExpressionLoaded=FALSE;
	m_bParsingDone=FALSE;*/
}

/****************************
 Parser wrapper
 ****************************/

/*static char		*pszParsePos=NULL; //just to easy detect errors
static bool		bParseError=false;

namespace LR
{
	yymlex_t		ExpressionImpl::ms_ExprLexer;
	yymparse_t		ExpressionImpl::ms_ExprParser;
}

//the parameter is ignored but supplied for compatibility reasons
int my_exprmgetchar(yymlex_t* pLex)
{
	if(*pszParsePos=='\0') {
		return -1;
	}
	return (unsigned char)*pszParsePos++;
}

void my_exprmerror(yymparse_t *pYacc,const char *text)
{
	printf("Moje parse error\n");
	bParseError=TRUE;
}*/

void ExpressionImpl::parseExpression()
{
/*	m_bParsingDone=FALSE;
	m_bCompilationDone=FALSE;
	pszParsePos=m_pszExpression;
	lexZeroTokenCount();
	if(pExprParserOutput)
	{
		ParseExprNode *pTempNode1,*pTempNode2;
		pTempNode1=pExprParserOutput;
		while(pTempNode1)
		{
			pTempNode2=pTempNode1->pNextNode;
			if(pTempNode1->token_val)
			{
				free(pTempNode1->token_val);
			}
			free(pTempNode1);
			pTempNode1=pTempNode2;
		}
		pExprParserOutput=NULL;
	}
	if(m_pCompilerOutput)
	{
		DWORD dwI=0;
		for(;dwI<m_dwInstructionCount;dwI++)
		{
			if(m_pCompilerOutput[dwI].pSomething)
			{
				delete [] m_pCompilerOutput[dwI].pSomething;
				m_pCompilerOutput[dwI].pSomething=NULL;
			}
		}
	}
	if(m_pParserOutput)
	{
		ParseExprNode *pTempNode1,*pTempNode2;
		pTempNode1=m_pParserOutput;
		while(pTempNode1)
		{
			pTempNode2=pTempNode1->pNextNode;
			if(pTempNode1->token_val)
			{
				delete pTempNode1->token_val;
			}
			delete pTempNode1;
			pTempNode1=pTempNode2;
		}
		m_pParserOutput=NULL;
	}
	bParseError=FALSE;
	ExpressionImpl::cleanupParser();
	yymparse(&ms_ExprParser);
	if(bParseError || (!pExprParserOutput))
	{
		throw ExpressionParsingExceptionImpl();
	}
	ParseExprNode	*pTempNode1=NULL;
	ParseExprNode	*pTempNode2=NULL;
	pTempNode1=pExprParserOutput;
	m_dwInstructionCount=1;
	m_pParserOutput=new ParseExprNode;
	pTempNode2=m_pParserOutput;
	*pTempNode2=*pTempNode1;
	pTempNode2->token_val=new char[strlen(pTempNode1->token_val)+1];
	strcpy(pTempNode2->token_val,pTempNode1->token_val);
	pTempNode2->pNextNode=NULL;
	while(pTempNode1->pNextNode)
	{
		pTempNode1=pTempNode1->pNextNode;
		pTempNode2->pNextNode=new ParseExprNode;
		pTempNode2=pTempNode2->pNextNode;
		*pTempNode2=*pTempNode1;
		pTempNode2->token_val=new char[strlen(pTempNode1->token_val)+1];
		strcpy(pTempNode2->token_val,pTempNode1->token_val);
		pTempNode2->pNextNode=NULL;
		m_dwInstructionCount++;
	}
	m_bParsingDone=TRUE;*/
}

/*****************
 VM compiler
 *****************/

static float fEnvelopeValue=0.0f;
static float fTimeValue=0.0f;
static float *pfVariableArrary=NULL;

static float* __fastcall easm_ret(float* pStackTop,void* pAdditionalData)
{
	return pStackTop;
}

static float* __fastcall easm_envl(float* pStackTop,void* pAdditionalData)
{
	*pStackTop=fEnvelopeValue;
	return (pStackTop+1);
}

static float* __fastcall easm_time(float* pStackTop,void* pAdditionalData)
{
	*pStackTop=fTimeValue;
	return (pStackTop+1);
}

static float* __fastcall easm_vol(float* pStackTop,void* pAdditionalData)
{
	*pStackTop=0.0f;
	return (pStackTop+1);
}

static float* __fastcall easm_push(float* pStackTop,void* pAdditionalData)
{
	*pStackTop=*((float*)pAdditionalData);
	return (pStackTop+1);
}

static float* __fastcall easm_add(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-2)=*(pStackTop-2)+*(pStackTop-1);
	return (pStackTop-1);
}

static float* __fastcall easm_sub(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-2)=*(pStackTop-2)-*(pStackTop-1);
	return (pStackTop-1);
}

static float* __fastcall easm_mul(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-2)=(*(pStackTop-2))*(*(pStackTop-1));
	return (pStackTop-1);
}

static float* __fastcall easm_div(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-2)=*(pStackTop-2)/(*(pStackTop-1));
	return (pStackTop-1);
}

static float* __fastcall easm_pow(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-2)=(float)pow(*(pStackTop-2),*(pStackTop-1));
	return (pStackTop-1);
}

static float* __fastcall easm_sin(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=(float)sin(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_cos(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=(float)cos(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_tan(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=(float)tan(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_ctg(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=(float)tan(*(pStackTop-1))==0 ? 0.0f : 1.0f/(float)tan(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_asin(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=(float)asin(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_acos(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=(float)acos(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_atan(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=(float)atan(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_sqrt(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=(float)sqrt(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_exp(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=(float)exp(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_ln(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=(*(pStackTop-1)<=0.0f) ? 0.0f : (float)log(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_flr(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=(float)floor(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_ceil(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=(float)ceil(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_abs(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=(float)fabs(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_fft(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=0.0f;
	return pStackTop;
}

static float* __fastcall easm_neg(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=-(*(pStackTop-1));
	return pStackTop;
}

static float* __fastcall easm_equ(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-2)=((*(pStackTop-2))==(*(pStackTop-1))) ? 1.0f : 0.0f ;
	return (pStackTop-1);
}

static float* __fastcall easm_nequ(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-2)=((*(pStackTop-2))!=(*(pStackTop-1))) ? 1.0f : 0.0f ;
	return (pStackTop-1);
}

static float* __fastcall easm_gt(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-2)=((*(pStackTop-2))>(*(pStackTop-1))) ? 1.0f : 0.0f ;
	return (pStackTop-1);
}

static float* __fastcall easm_gte(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-2)=((*(pStackTop-2))>=(*(pStackTop-1))) ? 1.0f : 0.0f ;
	return (pStackTop-1);
}

static float* __fastcall easm_lt(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-2)=((*(pStackTop-2))<(*(pStackTop-1))) ? 1.0f : 0.0f ;
	return (pStackTop-1);
}

static float* __fastcall easm_lte(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-2)=((*(pStackTop-2))<=(*(pStackTop-1))) ? 1.0f : 0.0f ;
	return (pStackTop-1);
}

static float* __fastcall easm_cond(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-3)= (*(pStackTop-3))>0.0f ? (*(pStackTop-2)) : (*(pStackTop-1));
	return (pStackTop-2);
}

static float* __fastcall easm_eval(float* pStackTop,void* pAdditionalData)
{
	*(pStackTop-1)=0.0f;
	return pStackTop;
}

static float* __fastcall easm_variable(float* pStackTop,void* pAdditionalData)
{
	*pStackTop=pfVariableArrary[(**((char**)pAdditionalData))-'A'];
	return (pStackTop+1);
}

static struct EasmInstructionDesc
{
	DWORD					dwOpcode;
	PFNEXPRASMOPCODEFUNC	pFunc;
	int						iStackSizeChange;
} EasmInstrDescriptions[]=
{
	{EASM_RET,easm_ret,0},
	{EASM_ENVL,easm_envl,1},
	{EASM_TIME,easm_time,1},
	{EASM_VOL,easm_vol,1},
	{EASM_PUSH,easm_push,1},
	{EASM_ADD,easm_add,-1},
	{EASM_SUB,easm_sub,-1},
	{EASM_MUL,easm_mul,-1},
	{EASM_DIV,easm_div,-1},
	{EASM_POW,easm_pow,-1},
	{EASM_SIN,easm_sin,0},
	{EASM_COS,easm_cos,0},
	{EASM_TAN,easm_tan,0},
	{EASM_CTG,easm_ctg,0},
	{EASM_ASIN,easm_asin,0},
	{EASM_ACOS,easm_acos,0},
	{EASM_ATAN,easm_atan,0},
	{EASM_SQRT,easm_sqrt,0},
	{EASM_EXP,easm_exp,0},
	{EASM_LN,easm_ln,0},
	{EASM_FLR,easm_flr,0},
	{EASM_CEIL,easm_ceil,0},
	{EASM_ABS,easm_abs,0},
	{EASM_FFT,easm_fft,0},
	{EASM_NEG,easm_neg,0},
	{EASM_EQU,easm_equ,-1},
	{EASM_NEQU,easm_nequ,-1},
	{EASM_GT,easm_gt,-1},
	{EASM_GTE,easm_gte,-1},
	{EASM_LT,easm_lt,-1},
	{EASM_LTE,easm_lte,-1},
	{EASM_COND,easm_cond,-2},
	{EASM_EVAL,easm_eval,0},
	{EASM_VARIABLE,easm_variable,1},
};

void ExpressionImpl::compileExpression()
{
	m_bCompilationDone=FALSE;
	if(!m_bParsingDone)
	{
		throw InvalidOperationExceptionImpl("trying to compile expression before successful parse");
	}
	if(m_pCompilerOutput)
	{
		DWORD dwI=0;
		for(;dwI<m_dwInstructionCount;dwI++)
		{
			if(m_pCompilerOutput[dwI].pSomething)
			{
				delete [] m_pCompilerOutput[dwI].pSomething;
				m_pCompilerOutput[dwI].pSomething=NULL;
			}
		}
	}
	if(m_pCompilerOutput)
	{
		delete [] m_pCompilerOutput;
		m_pCompilerOutput=FALSE;
	}
	m_pCompilerOutput=new ExprAsmInstruction[m_dwInstructionCount];
	ParseExprNode	*pTempNode=m_pParserOutput;
	DWORD			dwI;
	int				iSimulatedSize=0;
	int				iMaxSimulatedSize=0;
	for(dwI=0;dwI<m_dwInstructionCount;dwI++)
	{
		m_pCompilerOutput[dwI].pFunc=EasmInstrDescriptions[pTempNode->opcode].pFunc;
		m_pCompilerOutput[dwI].pSomething=NULL;
		iSimulatedSize+=EasmInstrDescriptions[pTempNode->opcode].iStackSizeChange;
		if(iSimulatedSize>iMaxSimulatedSize)
		{
			iMaxSimulatedSize=iSimulatedSize;
		}
		switch(pTempNode->opcode)
		{
			case EASM_PUSH:
				m_pCompilerOutput[dwI].fVal=(float)atof(pTempNode->token_val);
				break;
			case EASM_EVAL:
			case EASM_VARIABLE:
				m_pCompilerOutput[dwI].pSomething=new char[strlen(pTempNode->token_val)+1];
				strcpy((char*)(m_pCompilerOutput[dwI].pSomething),pTempNode->token_val);
				break;
			default:
				m_pCompilerOutput[dwI].fVal=0.0f;
		}
		pTempNode=pTempNode->pNextNode;
	}
	if(m_pExecutionStack)
	{
		delete [] m_pExecutionStack;
		m_pExecutionStack=NULL;
	}
	m_iStackSize=iMaxSimulatedSize;
	m_pExecutionStack=new float[m_iStackSize];
	m_bCompilationDone=TRUE;
}

float ExpressionImpl::evalExpression(float fEnvelope,float fTime)
{
	return fEnvelope;
/*	if(!m_bCompilationDone)
	{
		throw InvalidOperationExceptionImpl("trying to evaluate expression before successful compilation");
	}
	//preserve time/envelope values
	float fTmpEnvelope=fEnvelopeValue;
	float fTmpTime=fTimeValue;
	fEnvelopeValue=fEnvelope;
	fTimeValue=fTime;
	pfVariableArrary=m_InputVarTbl;
	DWORD dwI;
	float* pfStackTop=m_pExecutionStack;
	for(dwI=0;dwI<m_dwInstructionCount;dwI++)
	{
		pfStackTop=m_pCompilerOutput[dwI].pFunc(pfStackTop,&(m_pCompilerOutput[dwI].pSomething));
	}
	fEnvelopeValue=fTmpEnvelope;
	fTimeValue=fTmpTime;
	pfVariableArrary=NULL;
	return *m_pExecutionStack;*/
}

void ExpressionImpl::setInputVariable(DWORD dwID,float fValue)
{
	assert(dwID<EXPR_INPUT_VAR_TBL_SIZE);
	m_InputVarTbl[dwID]=fValue;
}

void ExpressionImpl::setInputVariableVec(DWORD dwStartID,DWORD dwCount,float *pfValueVec)
{
	assert(dwStartID<EXPR_INPUT_VAR_TBL_SIZE);
	assert(dwStartID+dwCount<=EXPR_INPUT_VAR_TBL_SIZE);
	assert(dwCount!=0);
	assert(pfValueVec!=NULL);
	memcpy(&m_InputVarTbl[dwStartID],pfValueVec,dwCount*sizeof(float));
}

bool ExpressionImpl::isStatic()
{
	return ((m_lstrExpression.getPtr()==NULL) ||
		(strlen(m_lstrExpression.getPtr())==0)) ? true : false;
}
