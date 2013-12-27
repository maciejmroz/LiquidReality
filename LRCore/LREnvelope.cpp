/*****************************************************************
 Envelope/Motion/Color classes implementation for
 LiquidReality engine
 (c) 2001 Maciej Mróz
******************************************************************/

#include "StdAfx.h"
#include <math.h>

using namespace LR;

/**********************************
 Constructor, destructor, operators
***********************************/

void EnvelopeImpl::clear()
{
	m_lPostBehavior=BEH_RESET;
	m_lPreBehavior=BEH_RESET;
	m_nNumKeys=0;
	if(m_pfKeyBiases)
	{
		delete m_pfKeyBiases;
		m_pfKeyBiases=NULL;
	}
	if(m_pfKeyContinuities)
	{
		delete m_pfKeyContinuities;
		m_pfKeyContinuities=NULL;
	}
	if(m_pfKeyParameters1)
	{
		delete m_pfKeyParameters1;
		m_pfKeyParameters1=NULL;
	}
	if(m_pfKeyParameters2)
	{
		delete m_pfKeyParameters2;
		m_pfKeyParameters2=NULL;
	}
	if(m_pfKeyParameters3)
	{
		delete m_pfKeyParameters3;
		m_pfKeyParameters3=NULL;
	}
	if(m_pfKeyParameters4)
	{
		delete m_pfKeyParameters4;
		m_pfKeyParameters4=NULL;
	}
	if(m_pfKeyTensions)
	{
		delete m_pfKeyTensions;
		m_pfKeyTensions=NULL;
	}
	if(m_pfKeyValues)
	{
		delete m_pfKeyValues;
		m_pfKeyValues=NULL;
	}
	if(m_plIncomingCurveTypes)
	{
		delete m_plIncomingCurveTypes;
		m_plIncomingCurveTypes=NULL;
	}
	if(m_plKeyPositions)
	{
		delete m_plKeyPositions;
		m_plKeyPositions=NULL;
	}
	m_bUseExpression=false;
	m_pLRExpr->clear();
}

EnvelopeImpl::EnvelopeImpl()
{
	m_lPostBehavior=BEH_RESET;
	m_lPreBehavior=BEH_RESET;
	m_nNumKeys=0;
	m_pfKeyBiases=NULL;
	m_pfKeyContinuities=NULL;
	m_pfKeyParameters1=NULL;
	m_pfKeyParameters2=NULL;
	m_pfKeyParameters3=NULL;
	m_pfKeyParameters4=NULL;
	m_pfKeyTensions=NULL;
	m_pfKeyValues=NULL;
	m_plIncomingCurveTypes=NULL;
	m_plKeyPositions=NULL;
	m_bUseExpression=false;
	NMainDllGlobals::g_pEngine->createExpression(m_pLRExpr);
}

EnvelopeImpl::EnvelopeImpl(EnvelopeImpl &rfEnv)
{
	m_lPostBehavior=rfEnv.m_lPostBehavior;
	m_lPreBehavior=rfEnv.m_lPreBehavior;
	m_nNumKeys=rfEnv.m_nNumKeys;

	m_pfKeyBiases=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyBiases,rfEnv.m_pfKeyBiases,m_nNumKeys*sizeof(float));
	
	m_pfKeyContinuities=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyContinuities,rfEnv.m_pfKeyContinuities,m_nNumKeys*sizeof(float));

	m_pfKeyParameters1=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyParameters1,rfEnv.m_pfKeyParameters1,m_nNumKeys*sizeof(float));

	m_pfKeyParameters2=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyParameters2,rfEnv.m_pfKeyParameters2,m_nNumKeys*sizeof(float));

	m_pfKeyParameters3=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyParameters3,rfEnv.m_pfKeyParameters3,m_nNumKeys*sizeof(float));

	m_pfKeyParameters4=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyParameters4,rfEnv.m_pfKeyParameters4,m_nNumKeys*sizeof(float));

	m_pfKeyTensions=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyTensions,rfEnv.m_pfKeyTensions,m_nNumKeys*sizeof(float));

	m_pfKeyValues=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyValues,rfEnv.m_pfKeyValues,m_nNumKeys*sizeof(float));

	m_plIncomingCurveTypes=new long[m_nNumKeys*sizeof(long)];
	memcpy(m_plIncomingCurveTypes,rfEnv.m_plIncomingCurveTypes,m_nNumKeys*sizeof(long));

	m_plKeyPositions=new long[m_nNumKeys*sizeof(long)];
	memcpy(m_plKeyPositions,rfEnv.m_plKeyPositions,m_nNumKeys*sizeof(long));

	NMainDllGlobals::g_pEngine->createExpression(m_pLRExpr);
	m_pLRExpr->assign(rfEnv.m_pLRExpr);
	m_bUseExpression=rfEnv.m_bUseExpression;
}

EnvelopeImpl::~EnvelopeImpl()
{
	clear();
	m_pLRExpr=NULL;
}

void EnvelopeImpl::operator=(EnvelopeImpl &rfEnv)
{
	clear();

	m_lPostBehavior=rfEnv.m_lPostBehavior;
	m_lPreBehavior=rfEnv.m_lPreBehavior;
	m_nNumKeys=rfEnv.m_nNumKeys;

	m_pfKeyBiases=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyBiases,rfEnv.m_pfKeyBiases,m_nNumKeys*sizeof(float));
	
	m_pfKeyContinuities=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyContinuities,rfEnv.m_pfKeyContinuities,m_nNumKeys*sizeof(float));

	m_pfKeyParameters1=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyParameters1,rfEnv.m_pfKeyParameters1,m_nNumKeys*sizeof(float));

	m_pfKeyParameters2=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyParameters2,rfEnv.m_pfKeyParameters2,m_nNumKeys*sizeof(float));

	m_pfKeyParameters3=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyParameters3,rfEnv.m_pfKeyParameters3,m_nNumKeys*sizeof(float));

	m_pfKeyParameters4=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyParameters4,rfEnv.m_pfKeyParameters4,m_nNumKeys*sizeof(float));

	m_pfKeyTensions=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyTensions,rfEnv.m_pfKeyTensions,m_nNumKeys*sizeof(float));

	m_pfKeyValues=new float[m_nNumKeys*sizeof(float)];
	memcpy(m_pfKeyValues,rfEnv.m_pfKeyValues,m_nNumKeys*sizeof(float));

	m_plIncomingCurveTypes=new long[m_nNumKeys*sizeof(long)];
	memcpy(m_plIncomingCurveTypes,rfEnv.m_plIncomingCurveTypes,m_nNumKeys*sizeof(long));

	m_plKeyPositions=new long[m_nNumKeys*sizeof(long)];
	memcpy(m_plKeyPositions,rfEnv.m_plKeyPositions,m_nNumKeys*sizeof(long));

	m_pLRExpr->assign(rfEnv.m_pLRExpr);
	m_bUseExpression=rfEnv.m_bUseExpression;
}

bool EnvelopeImpl::operator==(EnvelopeImpl &rfEnv)
{

	if(m_lPostBehavior!=rfEnv.m_lPostBehavior)
	{
		return false;
	}
	if(m_lPreBehavior!=rfEnv.m_lPreBehavior)
	{
		return false;
	}
	if(m_nNumKeys!=rfEnv.m_nNumKeys)
	{
		return false;
	}

	if(memcmp(m_pfKeyBiases,rfEnv.m_pfKeyBiases,m_nNumKeys*sizeof(float))!=0)
	{
		return false;
	}
	if(memcmp(m_pfKeyContinuities,rfEnv.m_pfKeyContinuities,m_nNumKeys*sizeof(float))!=0)
	{
		return false;
	}
	if(memcmp(m_pfKeyParameters1,rfEnv.m_pfKeyParameters1,m_nNumKeys*sizeof(float))!=0)
	{
		return false;
	}
	if(memcmp(m_pfKeyParameters2,rfEnv.m_pfKeyParameters2,m_nNumKeys*sizeof(float))!=0)
	{
		return false;
	}
	if(memcmp(m_pfKeyParameters3,rfEnv.m_pfKeyParameters3,m_nNumKeys*sizeof(float))!=0)
	{
		return false;
	}
	if(memcmp(m_pfKeyParameters4,rfEnv.m_pfKeyParameters4,m_nNumKeys*sizeof(float))!=0)
	{
		return false;
	}
	if(memcmp(m_pfKeyTensions,rfEnv.m_pfKeyTensions,m_nNumKeys*sizeof(float))!=0)
	{
		return false;
	}
	if(memcmp(m_pfKeyValues,rfEnv.m_pfKeyValues,m_nNumKeys*sizeof(float))!=0)
	{
		return false;
	}
	if(memcmp(m_plIncomingCurveTypes,rfEnv.m_plIncomingCurveTypes,m_nNumKeys*sizeof(long))!=0)
	{
		return false;
	}
	if(memcmp(m_plKeyPositions,rfEnv.m_plKeyPositions,m_nNumKeys*sizeof(long))!=0)
	{
		return false;
	}
	if(m_bUseExpression!=rfEnv.m_bUseExpression)
	{
		return false;
	}
	return m_pLRExpr->equals(rfEnv.m_pLRExpr);
}

/*******************************
 Serialization
********************************/

void EnvelopeImpl::unserializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs)
{
	clear();
	char	pTempOut[256];
	pfs->scanString(uVFileHandle,pTempOut);
	m_nNumKeys=pfs->scanUInt(uVFileHandle);
	//allocate memory
	m_pfKeyValues=new float[m_nNumKeys];
	m_plKeyPositions=new long[m_nNumKeys];
	m_plIncomingCurveTypes=new long[m_nNumKeys];
	m_pfKeyTensions=new float[m_nNumKeys];
	m_pfKeyContinuities=new float[m_nNumKeys];
	m_pfKeyBiases=new float[m_nNumKeys];
	m_pfKeyParameters1=new float[m_nNumKeys];
	m_pfKeyParameters2=new float[m_nNumKeys];
	m_pfKeyParameters3=new float[m_nNumKeys];
	m_pfKeyParameters4=new float[m_nNumKeys];
	unsigned int uiTmp;
	for(uiTmp=0;uiTmp<m_nNumKeys;uiTmp++)
	{
		pfs->scanString(uVFileHandle,pTempOut);
		m_pfKeyValues[uiTmp]=pfs->scanFloat(uVFileHandle);
		m_plKeyPositions[uiTmp]=pfs->scanLong(uVFileHandle);
		m_plIncomingCurveTypes[uiTmp]=pfs->scanLong(uVFileHandle);
		m_pfKeyTensions[uiTmp]=pfs->scanFloat(uVFileHandle);
		m_pfKeyContinuities[uiTmp]=pfs->scanFloat(uVFileHandle);
		m_pfKeyBiases[uiTmp]=pfs->scanFloat(uVFileHandle);
		m_pfKeyParameters1[uiTmp]=pfs->scanFloat(uVFileHandle);
		m_pfKeyParameters2[uiTmp]=pfs->scanFloat(uVFileHandle);
		m_pfKeyParameters3[uiTmp]=pfs->scanFloat(uVFileHandle);
		m_pfKeyParameters4[uiTmp]=pfs->scanFloat(uVFileHandle);
	}
	pfs->scanString(uVFileHandle,pTempOut);
	m_lPreBehavior=pfs->scanLong(uVFileHandle);
	m_lPostBehavior=pfs->scanLong(uVFileHandle);
	pfs->scanString(uVFileHandle,pTempOut);
	pfs->scanString(uVFileHandle,pTempOut);
	if(strlen(pTempOut)==2)
	{
		m_bUseExpression=false;
	}
	else
	{
		m_bUseExpression=true;
		pTempOut[strlen(pTempOut)-1]='\0';
		m_pLRExpr->loadExpression(pTempOut+1);
		//this _may_ sometimes fail but it is desireable not to clear
		//expression data (important for editing purposes)
		m_pLRExpr->parseExpression();
		m_pLRExpr->compileExpression();
	}
	pfs->scanString(uVFileHandle,pTempOut);
}

void EnvelopeImpl::unserializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs)
{
	clear();
	DWORD	dwTmp;
	char	szExprBuf[256];
	pfs->fread(uVFileHandle,4,&dwTmp);	//chunk ID
	if(dwTmp!=NLRChunkIDs::dwEnvelope)
	{
		throw FileFormatExceptionImpl("Expected envelope chunk");
	}
	pfs->fread(uVFileHandle,4,&dwTmp);	//chunk length
	pfs->fread(uVFileHandle,4,&dwTmp);	//number of keys
	m_nNumKeys=dwTmp;
	//allocate memory
	m_pfKeyValues=new float[m_nNumKeys];
	m_plKeyPositions=new long[m_nNumKeys];
	m_plIncomingCurveTypes=new long[m_nNumKeys];
	m_pfKeyTensions=new float[m_nNumKeys];
	m_pfKeyContinuities=new float[m_nNumKeys];
	m_pfKeyBiases=new float[m_nNumKeys];
	m_pfKeyParameters1=new float[m_nNumKeys];
	m_pfKeyParameters2=new float[m_nNumKeys];
	m_pfKeyParameters3=new float[m_nNumKeys];
	m_pfKeyParameters4=new float[m_nNumKeys];
	//read key data
	pfs->fread(uVFileHandle,m_nNumKeys*sizeof(float),m_pfKeyValues);
	pfs->fread(uVFileHandle,m_nNumKeys*sizeof(long),m_plKeyPositions);
	pfs->fread(uVFileHandle,m_nNumKeys*sizeof(long),m_plIncomingCurveTypes);
	pfs->fread(uVFileHandle,m_nNumKeys*sizeof(float),m_pfKeyTensions);
	pfs->fread(uVFileHandle,m_nNumKeys*sizeof(float),m_pfKeyContinuities);
	pfs->fread(uVFileHandle,m_nNumKeys*sizeof(float),m_pfKeyBiases);
	pfs->fread(uVFileHandle,m_nNumKeys*sizeof(float),m_pfKeyParameters1);
	pfs->fread(uVFileHandle,m_nNumKeys*sizeof(float),m_pfKeyParameters2);
	pfs->fread(uVFileHandle,m_nNumKeys*sizeof(float),m_pfKeyParameters3);
	pfs->fread(uVFileHandle,m_nNumKeys*sizeof(float),m_pfKeyParameters4);
	//expression
	pfs->fread(uVFileHandle,4,&dwTmp);	//expr length
	if(dwTmp>0)
	{
		if(dwTmp>256)
		{
			char*	pszTempExprBuf=new char[dwTmp];
			pfs->fread(uVFileHandle,dwTmp,pszTempExprBuf);
			m_pLRExpr->loadExpression(pszTempExprBuf);
			m_pLRExpr->parseExpression();
			delete pszTempExprBuf;
		}
		else
		{
			pfs->fread(uVFileHandle,dwTmp,szExprBuf);
			m_pLRExpr->loadExpression(szExprBuf);
			m_pLRExpr->parseExpression();
		}
		m_bUseExpression=true;
	}
	else
	{
		m_bUseExpression=false;
	}
}


DWORD EnvelopeImpl::getChunkLength()
{
	//data to be stored:
	//pre/post behaviors - 2*long
	//number of keys - long
	//key data: (2*long+8*float)*num_keys
	//expression long(expr_str_len)+expr_str_len
	DWORD dwBaseLen=3*sizeof(long)+(2*sizeof(long)+8*sizeof(float))*m_nNumKeys;
	const char *pszExprStr;
	pszExprStr=m_pLRExpr->getExpressionString();
	if(pszExprStr)
	{
		return dwBaseLen+sizeof(long)+strlen(pszExprStr);
	}
	else
	{
		return dwBaseLen+sizeof(long);
	}
}

void EnvelopeImpl::serializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs,DWORD dwColumn)
{
	DWORD dwTmp,dwTmp2;
	pfs->writeString(uVFileHandle," {\n");
	for(dwTmp=0;dwTmp<(dwColumn+1);dwTmp++)
	{
		pfs->writeString(uVFileHandle," ");
	}
	pfs->writeUInt(uVFileHandle,m_nNumKeys);
	pfs->writeString(uVFileHandle,"\n");

	for(dwTmp2=0;dwTmp2<m_nNumKeys;dwTmp2++)
	{
		for(dwTmp=0;dwTmp<(dwColumn+1);dwTmp++)
		{
			pfs->writeString(uVFileHandle," ");
		}
		pfs->writeString(uVFileHandle,"Key ");
		pfs->writeFloat(uVFileHandle,m_pfKeyValues[dwTmp2]);
		pfs->writeString(uVFileHandle," ");
		pfs->writeLong(uVFileHandle,m_plKeyPositions[dwTmp2]);
		pfs->writeString(uVFileHandle," ");
		pfs->writeLong(uVFileHandle,m_plIncomingCurveTypes[dwTmp2]);
		pfs->writeString(uVFileHandle," ");
		pfs->writeFloat(uVFileHandle,m_pfKeyTensions[dwTmp2]);
		pfs->writeString(uVFileHandle," ");
		pfs->writeFloat(uVFileHandle,m_pfKeyContinuities[dwTmp2]);
		pfs->writeString(uVFileHandle," ");
		pfs->writeFloat(uVFileHandle,m_pfKeyBiases[dwTmp2]);
		pfs->writeString(uVFileHandle," ");
		pfs->writeFloat(uVFileHandle,m_pfKeyParameters1[dwTmp2]);
		pfs->writeString(uVFileHandle," ");
		pfs->writeFloat(uVFileHandle,m_pfKeyParameters2[dwTmp2]);
		pfs->writeString(uVFileHandle," ");
		pfs->writeFloat(uVFileHandle,m_pfKeyParameters3[dwTmp2]);
		pfs->writeString(uVFileHandle," ");
		pfs->writeFloat(uVFileHandle,m_pfKeyParameters4[dwTmp2]);
		pfs->writeString(uVFileHandle,"\n");
	}
	for(dwTmp=0;dwTmp<(dwColumn+1);dwTmp++)
	{
		pfs->writeString(uVFileHandle," ");
	}
	pfs->writeString(uVFileHandle,"Behaviors ");
	pfs->writeLong(uVFileHandle,m_lPreBehavior);
	pfs->writeString(uVFileHandle," ");
	pfs->writeLong(uVFileHandle,m_lPostBehavior);
	pfs->writeString(uVFileHandle,"\n");
	//serialize expression here
	for(dwTmp=0;dwTmp<(dwColumn+1);dwTmp++) pfs->writeString(uVFileHandle," ");
	const char	*pszString=m_pLRExpr->getExpressionString();
	if(pszString)
	{
		pfs->writeString(uVFileHandle,"Expression \"");
		pfs->writeString(uVFileHandle,pszString);
		pfs->writeString(uVFileHandle,"\"\n");
	}
	else
	{
		pfs->writeString(uVFileHandle,"Expression \"\"\n");
	}
	//end
	for(dwTmp=0;dwTmp<(dwColumn);dwTmp++) pfs->writeString(uVFileHandle," ");
	pfs->writeString(uVFileHandle,"}\n");
}

void EnvelopeImpl::serializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs)
{
	DWORD dwTmp=NLRChunkIDs::dwEnvelope;
	pfs->fwrite(uVFileHandle,&dwTmp,4);
	dwTmp=getChunkLength();
	pfs->fwrite(uVFileHandle,&dwTmp,4);
	dwTmp=m_nNumKeys;
	pfs->fwrite(uVFileHandle,&dwTmp,4);
	pfs->fwrite(uVFileHandle,m_pfKeyValues,m_nNumKeys*sizeof(float));
	pfs->fwrite(uVFileHandle,m_plKeyPositions,m_nNumKeys*sizeof(long));
	pfs->fwrite(uVFileHandle,m_plIncomingCurveTypes,m_nNumKeys*sizeof(long));

	pfs->fwrite(uVFileHandle,m_pfKeyTensions,m_nNumKeys*sizeof(float));
	pfs->fwrite(uVFileHandle,m_pfKeyContinuities,m_nNumKeys*sizeof(float));
	pfs->fwrite(uVFileHandle,m_pfKeyBiases,m_nNumKeys*sizeof(float));
	pfs->fwrite(uVFileHandle,m_pfKeyParameters1,m_nNumKeys*sizeof(float));
	pfs->fwrite(uVFileHandle,m_pfKeyParameters2,m_nNumKeys*sizeof(float));
	pfs->fwrite(uVFileHandle,m_pfKeyParameters3,m_nNumKeys*sizeof(float));
	pfs->fwrite(uVFileHandle,m_pfKeyParameters4,m_nNumKeys*sizeof(float));

	pfs->fwrite(uVFileHandle,&m_lPreBehavior,4);
	pfs->fwrite(uVFileHandle,&m_lPostBehavior,4);

	const char *pszString=m_pLRExpr->getExpressionString();
	if(pszString)
	{
		dwTmp=strlen(pszString);
		pfs->fwrite(uVFileHandle,&dwTmp,4);
		pfs->fwrite(uVFileHandle,pszString,dwTmp);
	}
	else
	{
		dwTmp=0;
		pfs->fwrite(uVFileHandle,&dwTmp,4);
	}
}

/*********************************
 Complex (editor only) operations
**********************************/

long EnvelopeImpl::getNumKeys()
{
	return m_nNumKeys;
}

void EnvelopeImpl::addKey(long *lOutIndex,long lPosition,float fValue,long lIncomingCurveType,
									float fTension,float fContinuity,float fBias,
									float fKeyParameter1,float fKeyParameter2,
									float fKeyParameter3,float fKeyParameter4)
{
	if(m_nNumKeys==0)
	{
		clear();
		m_plKeyPositions=new long[1];
		m_pfKeyValues=new float[1];
		m_plIncomingCurveTypes=new long[1];
		m_pfKeyTensions=new float[1];
		m_pfKeyContinuities=new float[1];
		m_pfKeyBiases=new float[1];
		m_pfKeyParameters1=new float[1];
		m_pfKeyParameters2=new float[1];
		m_pfKeyParameters3=new float[1];
		m_pfKeyParameters4=new float[1];
		m_plKeyPositions[m_nNumKeys]=lPosition;
		m_pfKeyValues[m_nNumKeys]=fValue;
		m_plIncomingCurveTypes[m_nNumKeys]=lIncomingCurveType;
		m_pfKeyTensions[m_nNumKeys]=fTension;
		m_pfKeyContinuities[m_nNumKeys]=fContinuity;
		m_pfKeyBiases[m_nNumKeys]=fBias;
		m_pfKeyParameters1[m_nNumKeys]=fKeyParameter1;
		m_pfKeyParameters2[m_nNumKeys]=fKeyParameter2;
		m_pfKeyParameters3[m_nNumKeys]=fKeyParameter3;
		m_pfKeyParameters4[m_nNumKeys++]=fKeyParameter4;
		*lOutIndex=0;
	}
	else
	{
		//find key index
		unsigned int uiIndex=0;
		while(uiIndex<m_nNumKeys)
		{
			if(lPosition<m_plKeyPositions[uiIndex])
			{
				break;
			}
			else if(lPosition==m_plKeyPositions[uiIndex])
			{
				*lOutIndex=uiIndex;
				return;
			}
			uiIndex++;
		}
		long *plTempKeyPositions=new long[m_nNumKeys+1];
		float *pfTempKeyValues=new float[m_nNumKeys+1];
		long *plTempIncomingCurveTypes=new long[m_nNumKeys+1];
		float *pfTempKeyTensions=new float[m_nNumKeys+1];
		float *pfTempKeyContinuities=new float[m_nNumKeys+1];
		float *pfTempKeyBiases=new float[m_nNumKeys+1];
		float *pfTempKeyParameters1=new float[m_nNumKeys+1];
		float *pfTempKeyParameters2=new float[m_nNumKeys+1];
		float *pfTempKeyParameters3=new float[m_nNumKeys+1];
		float *pfTempKeyParameters4=new float[m_nNumKeys+1];
		if(uiIndex==m_nNumKeys)
		{
			//new key at the end
			memcpy(plTempKeyPositions,m_plKeyPositions,m_nNumKeys*sizeof(long));
			memcpy(pfTempKeyValues,m_pfKeyValues,m_nNumKeys*sizeof(float));
			memcpy(plTempIncomingCurveTypes,m_plIncomingCurveTypes,m_nNumKeys*sizeof(long));
			memcpy(pfTempKeyTensions,m_pfKeyTensions,m_nNumKeys*sizeof(float));
			memcpy(pfTempKeyContinuities,m_pfKeyContinuities,m_nNumKeys*sizeof(float));
			memcpy(pfTempKeyBiases,m_pfKeyBiases,m_nNumKeys*sizeof(float));
			memcpy(pfTempKeyParameters1,m_pfKeyParameters1,m_nNumKeys*sizeof(float));
			memcpy(pfTempKeyParameters2,m_pfKeyParameters2,m_nNumKeys*sizeof(float));
			memcpy(pfTempKeyParameters3,m_pfKeyParameters3,m_nNumKeys*sizeof(float));
			memcpy(pfTempKeyParameters4,m_pfKeyParameters4,m_nNumKeys*sizeof(float));
		}
		else
		{
			if(uiIndex>0)
			{
				memcpy(plTempKeyPositions,m_plKeyPositions,uiIndex*sizeof(long));
				memcpy(pfTempKeyValues,m_pfKeyValues,uiIndex*sizeof(float));
				memcpy(plTempIncomingCurveTypes,m_plIncomingCurveTypes,uiIndex*sizeof(long));
				memcpy(pfTempKeyTensions,m_pfKeyTensions,uiIndex*sizeof(float));
				memcpy(pfTempKeyContinuities,m_pfKeyContinuities,uiIndex*sizeof(float));
				memcpy(pfTempKeyBiases,m_pfKeyBiases,uiIndex*sizeof(float));
				memcpy(pfTempKeyParameters1,m_pfKeyParameters1,uiIndex*sizeof(float));
				memcpy(pfTempKeyParameters2,m_pfKeyParameters2,uiIndex*sizeof(float));
				memcpy(pfTempKeyParameters3,m_pfKeyParameters3,uiIndex*sizeof(float));
				memcpy(pfTempKeyParameters4,m_pfKeyParameters4,uiIndex*sizeof(float));
			}
			memcpy(&plTempKeyPositions[uiIndex+1],&m_plKeyPositions[uiIndex],(m_nNumKeys-uiIndex)*sizeof(long));
			memcpy(&pfTempKeyValues[uiIndex+1],&m_pfKeyValues[uiIndex],(m_nNumKeys-uiIndex)*sizeof(float));
			memcpy(&plTempIncomingCurveTypes[uiIndex+1],&m_plIncomingCurveTypes[uiIndex],(m_nNumKeys-uiIndex)*sizeof(long));
			memcpy(&pfTempKeyTensions[uiIndex+1],&m_pfKeyTensions[uiIndex],(m_nNumKeys-uiIndex)*sizeof(float));
			memcpy(&pfTempKeyContinuities[uiIndex+1],&m_pfKeyContinuities[uiIndex],(m_nNumKeys-uiIndex)*sizeof(float));
			memcpy(&pfTempKeyBiases[uiIndex+1],&m_pfKeyBiases[uiIndex],(m_nNumKeys-uiIndex)*sizeof(float));
			memcpy(&pfTempKeyParameters1[uiIndex+1],&m_pfKeyParameters1[uiIndex],(m_nNumKeys-uiIndex)*sizeof(float));
			memcpy(&pfTempKeyParameters2[uiIndex+1],&m_pfKeyParameters2[uiIndex],(m_nNumKeys-uiIndex)*sizeof(float));
			memcpy(&pfTempKeyParameters3[uiIndex+1],&m_pfKeyParameters3[uiIndex],(m_nNumKeys-uiIndex)*sizeof(float));
			memcpy(&pfTempKeyParameters4[uiIndex+1],&m_pfKeyParameters4[uiIndex],(m_nNumKeys-uiIndex)*sizeof(float));
		}
		delete m_plKeyPositions;
		m_plKeyPositions=plTempKeyPositions;
		delete m_pfKeyValues;
		m_pfKeyValues=pfTempKeyValues;
		delete m_plIncomingCurveTypes;
		m_plIncomingCurveTypes=plTempIncomingCurveTypes;
		delete m_pfKeyTensions;
		m_pfKeyTensions=pfTempKeyTensions;
		delete m_pfKeyContinuities;
		m_pfKeyContinuities=pfTempKeyContinuities;
		delete m_pfKeyBiases;
		m_pfKeyBiases=pfTempKeyBiases;
		delete m_pfKeyParameters1;
		m_pfKeyParameters1=pfTempKeyParameters1;
		delete m_pfKeyParameters2;
		m_pfKeyParameters2=pfTempKeyParameters2;
		delete m_pfKeyParameters3;
		m_pfKeyParameters3=pfTempKeyParameters3;
		delete m_pfKeyParameters4;
		m_pfKeyParameters4=pfTempKeyParameters4;

		m_plKeyPositions[uiIndex]=lPosition;
		m_pfKeyValues[uiIndex]=fValue;
		m_plIncomingCurveTypes[uiIndex]=lIncomingCurveType;
		m_pfKeyTensions[uiIndex]=fTension;
		m_pfKeyContinuities[uiIndex]=fContinuity;
		m_pfKeyBiases[uiIndex]=fBias;
		m_pfKeyParameters1[uiIndex]=fKeyParameter1;
		m_pfKeyParameters2[uiIndex]=fKeyParameter2;
		m_pfKeyParameters3[uiIndex]=fKeyParameter3;
		m_pfKeyParameters4[uiIndex]=fKeyParameter4;
		m_nNumKeys++;
		*lOutIndex=uiIndex;
	}
}

void EnvelopeImpl::removeKey(long lPosition)
{
	unsigned int uiIndex=0;
	while(uiIndex<m_nNumKeys)
	{
		if(lPosition==m_plKeyPositions[uiIndex])
		{
			break;
		}
		uiIndex++;
	}
	if(uiIndex==m_nNumKeys)
	{
		throw InvalidParameterExceptionImpl("No key at requested position");
	}
	removeKeyAtIndex(uiIndex);
}

void EnvelopeImpl::removeKeyAtIndex(unsigned int uiIndex)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	long *plTempKeyPositions=new long[m_nNumKeys-1];
	float *pfTempKeyValues=new float[m_nNumKeys-1];
	long *plTempIncomingCurveTypes=new long[m_nNumKeys-1];
	float *pfTempKeyTensions=new float[m_nNumKeys-1];
	float *pfTempKeyContinuities=new float[m_nNumKeys-1];
	float *pfTempKeyBiases=new float[m_nNumKeys-1];
	float *pfTempKeyParameters1=new float[m_nNumKeys-1];
	float *pfTempKeyParameters2=new float[m_nNumKeys-1];
	float *pfTempKeyParameters3=new float[m_nNumKeys-1];
	float *pfTempKeyParameters4=new float[m_nNumKeys-1];
	if(uiIndex>0)
	{
		memcpy(plTempKeyPositions,m_plKeyPositions,uiIndex*sizeof(long));
		memcpy(pfTempKeyValues,m_pfKeyValues,uiIndex*sizeof(float));
		memcpy(plTempIncomingCurveTypes,m_plIncomingCurveTypes,uiIndex*sizeof(long));
		memcpy(pfTempKeyTensions,m_pfKeyTensions,uiIndex*sizeof(float));
		memcpy(pfTempKeyContinuities,m_pfKeyContinuities,uiIndex*sizeof(float));
		memcpy(pfTempKeyBiases,m_pfKeyBiases,uiIndex*sizeof(float));
		memcpy(pfTempKeyParameters1,m_pfKeyParameters1,uiIndex*sizeof(float));
		memcpy(pfTempKeyParameters2,m_pfKeyParameters2,uiIndex*sizeof(float));
		memcpy(pfTempKeyParameters3,m_pfKeyParameters3,uiIndex*sizeof(float));
		memcpy(pfTempKeyParameters4,m_pfKeyParameters4,uiIndex*sizeof(float));
	}
	memcpy(&plTempKeyPositions[uiIndex],&m_plKeyPositions[uiIndex+1],(m_nNumKeys-uiIndex-1)*sizeof(long));
	memcpy(&pfTempKeyValues[uiIndex],&m_pfKeyValues[uiIndex+1],(m_nNumKeys-uiIndex-1)*sizeof(float));
	memcpy(&plTempIncomingCurveTypes[uiIndex],&m_plIncomingCurveTypes[uiIndex+1],(m_nNumKeys-uiIndex-1)*sizeof(long));
	memcpy(&pfTempKeyTensions[uiIndex],&m_pfKeyTensions[uiIndex+1],(m_nNumKeys-uiIndex-1)*sizeof(float));
	memcpy(&pfTempKeyContinuities[uiIndex],&m_pfKeyContinuities[uiIndex+1],(m_nNumKeys-uiIndex-1)*sizeof(float));
	memcpy(&pfTempKeyBiases[uiIndex],&m_pfKeyBiases[uiIndex+1],(m_nNumKeys-uiIndex-1)*sizeof(float));
	memcpy(&pfTempKeyParameters1[uiIndex],&m_pfKeyParameters1[uiIndex+1],(m_nNumKeys-uiIndex-1)*sizeof(float));
	memcpy(&pfTempKeyParameters2[uiIndex],&m_pfKeyParameters2[uiIndex+1],(m_nNumKeys-uiIndex-1)*sizeof(float));
	memcpy(&pfTempKeyParameters3[uiIndex],&m_pfKeyParameters3[uiIndex+1],(m_nNumKeys-uiIndex-1)*sizeof(float));
	memcpy(&pfTempKeyParameters4[uiIndex],&m_pfKeyParameters4[uiIndex+1],(m_nNumKeys-uiIndex-1)*sizeof(float));

	delete m_plKeyPositions;
	m_plKeyPositions=plTempKeyPositions;
	delete m_pfKeyValues;
	m_pfKeyValues=pfTempKeyValues;
	delete m_plIncomingCurveTypes;
	m_plIncomingCurveTypes=plTempIncomingCurveTypes;
	delete m_pfKeyTensions;
	m_pfKeyTensions=pfTempKeyTensions;
	delete m_pfKeyContinuities;
	m_pfKeyContinuities=pfTempKeyContinuities;
	delete m_pfKeyBiases;
	m_pfKeyBiases=pfTempKeyBiases;
	delete m_pfKeyParameters1;
	m_pfKeyParameters1=pfTempKeyParameters1;
	delete m_pfKeyParameters2;
	m_pfKeyParameters2=pfTempKeyParameters2;
	delete m_pfKeyParameters3;
	m_pfKeyParameters3=pfTempKeyParameters3;
	delete m_pfKeyParameters4;
	m_pfKeyParameters4=pfTempKeyParameters4;
	m_nNumKeys--;
}

void EnvelopeImpl::setPreBehavior(long lBehavior)
{
	if((lBehavior>BEH_LINEAR) ||
		(lBehavior<BEH_RESET))
	{
		throw InvalidParameterExceptionImpl("Unknown envelope behavior requested");
	}
	m_lPreBehavior=lBehavior;
}

void EnvelopeImpl::setPostBehavior(long lBehavior)
{
	if((lBehavior>BEH_LINEAR) ||
		(lBehavior<BEH_RESET))
	{
		throw InvalidParameterExceptionImpl("Unknown envelope behavior requested");
	}
	m_lPostBehavior=lBehavior;
}

long EnvelopeImpl::getPreBehavior()
{
	return m_lPreBehavior;
}

long EnvelopeImpl::getPostBehavior()
{
	return m_lPostBehavior;
}

void EnvelopeImpl::getExpression(ExpressionPtr &ppIExpr)
{
	ppIExpr=m_pLRExpr;
}

void EnvelopeImpl::setExpressionEnable(bool bEnable)
{
	m_bUseExpression=bEnable;
}

bool EnvelopeImpl::getExpressionEnable()
{
	return m_bUseExpression;
}

long EnvelopeImpl::getKeyPositionAtIndex(unsigned int uiIndex)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	return m_plKeyPositions[uiIndex];
}

float EnvelopeImpl::getKeyValueAtIndex(unsigned int uiIndex)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	return m_pfKeyValues[uiIndex];
}

float EnvelopeImpl::getKeyTensionAtIndex(unsigned int uiIndex)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	return m_pfKeyTensions[uiIndex];
}

float EnvelopeImpl::getKeyContinuityAtIndex(unsigned int uiIndex)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	return m_pfKeyContinuities[uiIndex];
}

float EnvelopeImpl::getKeyBiasAtIndex(unsigned int uiIndex)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	return m_pfKeyBiases[uiIndex];
}

float EnvelopeImpl::getKeyParameter1AtIndex(unsigned int uiIndex)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	return m_pfKeyParameters1[uiIndex];
}

float EnvelopeImpl::getKeyParameter2AtIndex(unsigned int uiIndex)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	return m_pfKeyParameters2[uiIndex];
}

float EnvelopeImpl::getKeyParameter3AtIndex(unsigned int uiIndex)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	return m_pfKeyParameters3[uiIndex];
}

float EnvelopeImpl::getKeyParameter4AtIndex(unsigned int uiIndex)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	return m_pfKeyParameters4[uiIndex];
}


long EnvelopeImpl::getKeyTypeAtIndex(unsigned int uiIndex)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	return m_plIncomingCurveTypes[uiIndex];
}

void EnvelopeImpl::setKeyPositionAtIndex(unsigned int uiIndex,long lVal)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	m_plKeyPositions[uiIndex]=lVal;
}

void EnvelopeImpl::setKeyValueAtIndex(unsigned int uiIndex,float fVal)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	m_pfKeyValues[uiIndex]=fVal;
}

void EnvelopeImpl::setKeyTensionAtIndex(unsigned int uiIndex,float fVal)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	m_pfKeyTensions[uiIndex]=fVal;
}

void EnvelopeImpl::setKeyContinuityAtIndex(unsigned int uiIndex,float fVal)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	m_pfKeyContinuities[uiIndex]=fVal;
}

void EnvelopeImpl::setKeyBiasAtIndex(unsigned int uiIndex,float fVal)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	m_pfKeyBiases[uiIndex]=fVal;
}

void EnvelopeImpl::setKeyParameter1AtIndex(unsigned int uiIndex,float fVal)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	m_pfKeyParameters1[uiIndex]=fVal;
}

void EnvelopeImpl::setKeyParameter2AtIndex(unsigned int uiIndex,float fVal)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	m_pfKeyParameters2[uiIndex]=fVal;
}

void EnvelopeImpl::setKeyParameter3AtIndex(unsigned int uiIndex,float fVal)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	m_pfKeyParameters3[uiIndex]=fVal;
}

void EnvelopeImpl::setKeyParameter4AtIndex(unsigned int uiIndex,float fVal)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	m_pfKeyParameters4[uiIndex]=fVal;
}

void EnvelopeImpl::setKeyTypeAtIndex(unsigned int uiIndex,long lVal)
{
	if(uiIndex>=m_nNumKeys)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_nNumKeys,uiIndex);
	}
	m_plIncomingCurveTypes[uiIndex]=lVal;
}

bool EnvelopeImpl::equals(EnvelopePtr pILREnv)
{
	EnvelopeImpl *pEnv=dynamic_cast<EnvelopeImpl*>(pILREnv);
	if(pEnv)
	{
		if(this->operator==(*pEnv))
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

void EnvelopeImpl::assign(EnvelopePtr pILREnv)
{
	EnvelopeImpl *pEnv=dynamic_cast<EnvelopeImpl*>(pILREnv);
	if(pEnv)
	{
		if(pEnv!=this)
		{
			this->operator=(*pEnv);
		}
	}
	else
	{
		clear();
		//TODO: should probably throw exception here
	}
}

/***********************************************************
 Envelope interpolation functions
 original code by Ernie Wright
 slightly modified only to match my data structures
 and code formatting style
 ***********************************************************/

//we don't store keys separately and there are no 
//next/prev pointers. These helper functions return
//index of next/prev key or -1

long EnvelopeImpl::prevKey(unsigned long lKey)
{
	if((lKey<m_nNumKeys) &&
		(lKey>0))
	{
		return (lKey-1);
	}
	return -1;
}

long EnvelopeImpl::nextKey(unsigned long lKey)
{
	if(lKey<(m_nNumKeys-1))
	{
		return (lKey+1);
	}
	return -1;
}

//we store frame numbers so they have to
//(not really, but we need it floating point anyway)
//be converted to seconds

float EnvelopeImpl::frames2seconds(long frame,float frame_time)
{
	return frame*frame_time;
}

/*
======================================================================
range()

Given the value v of a periodic function, returns the equivalent value
v2 in the principal interval [lo, hi].  If i isn't NULL, it receives
the number of wavelengths between v and v2.

   v2 = v - i * (hi - lo)

For example, range( 3 pi, 0, 2 pi, i ) returns pi, with i = 1.
====================================================================== */

float EnvelopeImpl::range(float v, float lo, float hi, int *i)
{

	float v2,r=hi-lo;
	
	if(r==0.0)
	{
		if(i)
		{
			*i=0;
		}
		return lo;
	}
	v2=v-r*(float)floor((double)(v-lo)/r);
	if(i)*i=-(int)((v2-v)/r+(v2>v? 0.5 : -0.5));
	
	return v2;
}

/*
======================================================================
hermite()

Calculate the Hermite coefficients.
====================================================================== */

void EnvelopeImpl::hermite(float t, float *h1, float *h2, float *h3, float *h4) {

	float t2,t3;
	
	t2=t*t;
	t3=t*t2;
	
	*h2=3.0f*t2-t3-t3;
	*h1=1.0f-*h2;
	*h4=t3-t2;
	*h3=*h4-t2+t;
}

float EnvelopeImpl::outgoing(long lKey0, long lKey1)
{
	float a,b,d,t,out;
	
	switch (m_plIncomingCurveTypes[lKey0])  //unsafe!!!!
	{
		case SHAPE_TCB:
			a=(1.0f-m_pfKeyTensions[lKey0])
				*(1.0f+m_pfKeyContinuities[lKey0])
				*(1.0f+m_pfKeyBiases[lKey0]);
			b=(1.0f-m_pfKeyTensions[lKey0])
				*(1.0f-m_pfKeyContinuities[lKey0])
				*(1.0f-m_pfKeyBiases[lKey0]);
			d=m_pfKeyValues[lKey1]-m_pfKeyValues[lKey0];
			
			if(prevKey(lKey0)!=-1)
			{
				t=(frames2seconds(m_plKeyPositions[lKey1])
					-frames2seconds(m_plKeyPositions[lKey0]))/
					(frames2seconds(m_plKeyPositions[lKey1])
					-frames2seconds(m_plKeyPositions[prevKey(lKey0)]));
				out=t*(a*(m_pfKeyValues[lKey0]-
					m_pfKeyValues[prevKey(lKey0)]+b*d));
			}
			else
				out=0.5f*(a+b)*d;
			break;
			
		case SHAPE_BEZI:
		case SHAPE_HERM:
			out=m_pfKeyParameters2[lKey0];
			if(prevKey(lKey0)!=-1)
			{
				out*=(frames2seconds(m_plKeyPositions[lKey1])
					-frames2seconds(m_plKeyPositions[lKey0]))/
					(frames2seconds(m_plKeyPositions[lKey1])
					-frames2seconds(m_plKeyPositions[prevKey(lKey0)]));
			}
			break;
			
		case SHAPE_LINE:
		case SHAPE_STEP:
		case SHAPE_BEZ2:
		default:
			out=0.0f;
			break;
	}
	
	return out;
}

/*
======================================================================
incoming()

Return the incoming tangent to the curve at key1.  This works for
everything except BEZ2, which needs to be treated differently.
====================================================================== */

float EnvelopeImpl::incoming(long lKey0, long lKey1)
{
	float a,b,d,t,in;
	
	switch(m_plIncomingCurveTypes[lKey1])
	{
		case SHAPE_LINE:
			in=(m_pfKeyValues[lKey1]-m_pfKeyValues[lKey0])/
				(frames2seconds(m_plKeyPositions[lKey1])-
				frames2seconds(m_plKeyPositions[lKey0]));
			break;
			
		case SHAPE_TCB:
			a=(1.0f-m_pfKeyTensions[lKey1])
				*(1.0f-m_pfKeyContinuities[lKey1])
				*(1.0f+m_pfKeyBiases[lKey1]);
			b=(1.0f-m_pfKeyTensions[lKey1])
				*(1.0f+m_pfKeyContinuities[lKey1])
				*(1.0f-m_pfKeyBiases[lKey1]);
			d=m_pfKeyValues[lKey1]-m_pfKeyValues[lKey0];
			
			if(nextKey(lKey1)!=-1)
			{
				t=(frames2seconds(m_plKeyPositions[lKey1])-
					frames2seconds(m_plKeyPositions[lKey0]))/
					(frames2seconds(m_plKeyPositions[nextKey(lKey1)])-
					frames2seconds(m_plKeyPositions[lKey0]));
				in=t*(b*(m_pfKeyValues[nextKey(lKey1)]-m_pfKeyValues[lKey1])+a*d);
			}
			else
				in=0.5f*(a+b)*d;
			break;
			
		case SHAPE_BEZI:
		case SHAPE_HERM:
			in=m_pfKeyParameters1[lKey1];
			if(nextKey(lKey1)!=-1)
			{
				in*=(frames2seconds(m_plKeyPositions[lKey1])-
					frames2seconds(m_plKeyPositions[lKey0]))/
					(frames2seconds(m_plKeyPositions[nextKey(lKey1)])-
					frames2seconds(m_plKeyPositions[lKey0]));
			}
			break;
			return in;
			
		case SHAPE_STEP:
		default:
			in=0.0f;
			break;
	}
	
	return in;
}

/*
======================================================================
bezier()

Interpolate the value of a 1D Bezier curve.
====================================================================== */

float EnvelopeImpl::bezier(float x0, float x1, float x2, float x3, float t)
{
	float a,b,c,t2,t3;
	
	t2=t*t;
	t3=t2*t;
	
	c=3.0f*(x1-x0);
	b=3.0f*(x2-x1)-c;
	a=x3-x0-c-b;
	
	return a*t3+b*t2+c*t+x0;
}

/*
======================================================================
bez2_time()

Find the t for which bezier() returns the input time.  The handle
endpoints of a BEZ2 curve represent the control points, and these have
(time, value) coordinates, so time is used as both a coordinate and a
parameter for this curve type.
====================================================================== */


float EnvelopeImpl::bez2_time(float x0, float x1, float x2, float x3,
									float time, float *t0, float *t1)
{
	float v,t;
	
	t=*t0+(*t1-*t0)*0.5f;
	v=bezier( x0, x1, x2, x3, t );
	if(fabs(time-v)>.0001f)
	{
		if(v>time)
		{
			*t1=t;
		}
		else
		{
			*t0=t;
		}
		return bez2_time(x0,x1,x2,x3,time,t0,t1);
	}
	else return t;
}

/*
======================================================================
bez2()

Interpolate the value of a BEZ2 curve.
====================================================================== */

float EnvelopeImpl::bez2(long lKey0, long lKey1, float time)
{
	float x,y,t,t0=0.0f,t1=1.0f;
	
	if(m_plIncomingCurveTypes[lKey0]==SHAPE_BEZ2)
	{
		x=frames2seconds(m_plKeyPositions[lKey0])+m_pfKeyParameters3[lKey0];
	}
	else
	{
		x=frames2seconds(m_plKeyPositions[lKey0])+
			(frames2seconds(m_plKeyPositions[lKey1])
			-frames2seconds(m_plKeyPositions[lKey0]))/3.0f;
	}
	
	t=bez2_time(frames2seconds(m_plKeyPositions[lKey0]),x,
		frames2seconds(m_plKeyPositions[lKey1])
		+m_pfKeyParameters1[lKey1],
		frames2seconds(m_plKeyPositions[lKey1]),
		time,&t0,&t1);
	
	if(m_plIncomingCurveTypes[lKey0]==SHAPE_BEZ2)
	{
		y=m_pfKeyValues[lKey0]+m_pfKeyParameters4[lKey0];
	}
	else
	{
		y=m_pfKeyValues[lKey0]+outgoing(lKey0,lKey1)/3.0f;
	}
	
	return bezier(m_pfKeyValues[lKey0],y,
		m_pfKeyParameters2[lKey1]+m_pfKeyValues[lKey1],
		m_pfKeyValues[lKey1],t);
}

/*
======================================================================
lwEvalEnvelope()

Given a list of keys and a time, returns the interpolated value of the
envelope at that time.
====================================================================== */

float EnvelopeImpl::evalEnvelope(float fTime)
{
	long	lKey0,lKey1,lSKey,lEKey;
	float	t,h1,h2,h3,h4,in,out,offset=0.0f;
	int		noff;
	
	
	// if there's no key, the value is 0
	if(m_nNumKeys==0)
	{
		if(m_bUseExpression)
		{
			return m_pLRExpr->evalExpression(0.0f,fTime);
		}
		else
		{
			return 0.0f;
		}
	}
		
	// if there's only one key, the value is constant 
	if(m_nNumKeys==1)
	{
		if(m_bUseExpression)
		{
			return m_pLRExpr->evalExpression(m_pfKeyValues[0],fTime);
		}
		else
		{
			return m_pfKeyValues[0];
		}
	}
	
	// find the first and last keys 
	lSKey=lEKey=0;
	while(nextKey(lEKey)!=-1)
	{
		lEKey++;
	}
	
	// use pre-behavior if time is before first key time 
	if(fTime<frames2seconds(m_plKeyPositions[lSKey]))
	{
		switch(m_lPreBehavior)
		{
			case BEH_RESET:
				if(m_bUseExpression)
				{
					return m_pLRExpr->evalExpression(0.0f,fTime);
				}
				else
				{
					return 0.0f;
				}
			case BEH_CONSTANT:
				if(m_bUseExpression)
				{
					return m_pLRExpr->evalExpression(m_pfKeyValues[lSKey],fTime);
				}
				else
				{
					return m_pfKeyValues[lSKey];
				}
			case BEH_REPEAT:
				fTime=range(fTime,frames2seconds(m_plKeyPositions[lSKey]),
					frames2seconds(m_plKeyPositions[lEKey]),NULL);
				break;			
			case BEH_OSCILLATE:
				fTime=range(fTime,frames2seconds(m_plKeyPositions[lSKey]),
					frames2seconds(m_plKeyPositions[lEKey]),&noff);
				if(noff%2) fTime=frames2seconds(m_plKeyPositions[lEKey])
					-frames2seconds(m_plKeyPositions[lSKey])-fTime;
				break;
			case BEH_OFFSET:
				fTime=range(fTime,frames2seconds(m_plKeyPositions[lSKey]),
					frames2seconds(m_plKeyPositions[lEKey]),&noff);
				offset=noff*(m_pfKeyValues[lEKey]
					-m_pfKeyValues[lSKey]);
				break;
			case BEH_LINEAR:
				switch(m_plIncomingCurveTypes[lSKey])
				{
					case SHAPE_STEP:
						if(m_bUseExpression)
						{
							return m_pLRExpr->evalExpression(m_pfKeyValues[lSKey],fTime);
						}
						else
						{
							return m_pfKeyValues[lSKey];
						}
						break;
					case SHAPE_LINE:
						{
							float fTemp=(m_pfKeyValues[lSKey]- m_pfKeyValues[nextKey(lSKey)])
								/(frames2seconds(m_plKeyPositions[lSKey])
								-frames2seconds(m_plKeyPositions[nextKey(lSKey)]))
								*(fTime-frames2seconds(m_plKeyPositions[nextKey(lSKey)]))
								+m_pfKeyValues[nextKey(lSKey)];
							if(m_bUseExpression)
							{
								return m_pLRExpr->evalExpression(fTemp,fTime);
							}
							return fTemp;
						}
						break;
					case SHAPE_TCB:
					case SHAPE_HERM:
					case SHAPE_BEZI:
						{
							out=outgoing(lSKey,nextKey(lSKey))
								/(frames2seconds(m_plKeyPositions[nextKey(lSKey)])-
								frames2seconds(m_plKeyPositions[lSKey]));
							float fTemp=out*(fTime-frames2seconds(m_plKeyPositions[lSKey]))+
								m_pfKeyValues[lSKey];
							if(m_bUseExpression)
							{
								return m_pLRExpr->evalExpression(fTemp,fTime);
							}
							return fTemp;
						}
						break;
					case SHAPE_BEZ2:
						{
							float fTemp=(m_pfKeyParameters2[lSKey]-m_pfKeyParameters4[lSKey])
								/(m_pfKeyParameters1[lSKey]-m_pfKeyParameters3[lSKey])
								*(fTime-frames2seconds(m_plKeyPositions[lSKey]))
								+m_pfKeyValues[lSKey];
							if(m_bUseExpression)
							{
								return m_pLRExpr->evalExpression(fTemp,fTime);
							}
							return fTemp;
						}
						break;
				}
				break;
		}
	}
	
	/* use post-behavior if time is after last key time */
	else if(fTime>frames2seconds(m_plKeyPositions[lEKey]))
	{
		switch(m_lPostBehavior)
		{
			case BEH_RESET:
				if(m_bUseExpression)
				{
					return m_pLRExpr->evalExpression(0.0f,fTime);
				}
				return 0.0f;
			case BEH_CONSTANT:
				{
					float fTemp=m_pfKeyValues[lEKey];
					if(m_bUseExpression)
					{
						return m_pLRExpr->evalExpression(fTemp,fTime);
					}
					return fTemp;
				}
			case BEH_REPEAT:
				fTime=range(fTime,frames2seconds(m_plKeyPositions[lSKey]),
					frames2seconds(m_plKeyPositions[lEKey]),NULL);
				break;
			case BEH_OSCILLATE:
				fTime=range(fTime,frames2seconds(m_plKeyPositions[lSKey]),
					frames2seconds(m_plKeyPositions[lEKey]),&noff);
				if(noff%2) fTime=frames2seconds(m_plKeyPositions[lEKey])-
					frames2seconds(m_plKeyPositions[lSKey])-fTime;
				break;
			case BEH_OFFSET:
				fTime=range(fTime,frames2seconds(m_plKeyPositions[lSKey]),
					frames2seconds(m_plKeyPositions[lEKey]),&noff);
				offset=noff*(m_pfKeyValues[lEKey]-m_pfKeyValues[lSKey]);
				break;
			case BEH_LINEAR:
				switch(m_plIncomingCurveTypes[lEKey])
				{
					case SHAPE_STEP:
						{
							float fTemp=m_pfKeyValues[lEKey];
							if(m_bUseExpression)
							{
								return m_pLRExpr->evalExpression(fTemp,fTime);
							}
							return fTemp;
						}
						break;
					case SHAPE_LINE:
						{
							float fTemp=(m_pfKeyValues[lEKey]-m_pfKeyValues[prevKey(lEKey)])
								/(frames2seconds(m_plKeyPositions[lEKey])
								-frames2seconds(m_plKeyPositions[prevKey(lEKey)]))
								*(fTime-frames2seconds(m_plKeyPositions[prevKey(lEKey)]))
								+m_pfKeyValues[prevKey(lEKey)];
							if(m_bUseExpression)
							{
								return m_pLRExpr->evalExpression(fTemp,fTime);
							}
							return fTemp;
						}
						break;
					case SHAPE_TCB:
					case SHAPE_HERM:
					case SHAPE_BEZI:
						{
							in=incoming(prevKey(lEKey),lEKey)
								/(frames2seconds(m_plKeyPositions[lEKey])
								-frames2seconds(m_plKeyPositions[prevKey(lEKey)]));
							float fTemp=in*(fTime-frames2seconds(m_plKeyPositions[lEKey]))
								+m_pfKeyValues[lEKey];
							if(m_bUseExpression)
							{
								return m_pLRExpr->evalExpression(fTemp,fTime);
							}
							return fTemp;
						}
						break;
					case SHAPE_BEZ2:
						{
							float fTemp=(m_pfKeyParameters4[lEKey]-m_pfKeyParameters2[lEKey])
								/(m_pfKeyParameters3[lEKey]-m_pfKeyParameters1[lEKey])
								*(fTime-frames2seconds(m_plKeyPositions[lEKey]))
								+m_pfKeyValues[lEKey];
							if(m_bUseExpression)
							{
								return m_pLRExpr->evalExpression(fTemp,fTime);
							}
							return fTemp;
						}
						break;
				}
				break;
		}
	}
	
	/* get the endpoints of the interval being evaluated */
	lKey0=0;
	while(fTime>frames2seconds(m_plKeyPositions[nextKey(lKey0)]))
	{
		lKey0++;
	}
	lKey1=lKey0+1;
	
	/* check for singularities first */
	if(fTime==frames2seconds(m_plKeyPositions[lKey0]))
	{
		float fTemp=m_pfKeyValues[lKey0]+offset;
		if(m_bUseExpression)
		{
			return m_pLRExpr->evalExpression(fTemp,fTime);
		}
		return fTemp;
	}
	else if(fTime==frames2seconds(m_plKeyPositions[lKey1]))
	{
		float fTemp=m_pfKeyValues[lKey1]+offset;
		if(m_bUseExpression)
		{
			return m_pLRExpr->evalExpression(fTemp,fTime);
		}
		return fTemp;
	}
	
	/* get interval length, time in [0, 1] */
	t=(fTime-frames2seconds(m_plKeyPositions[lKey0]))
		/(frames2seconds(m_plKeyPositions[lKey1])
		  -frames2seconds(m_plKeyPositions[lKey0]));
	
	/* interpolate */
	switch(m_plIncomingCurveTypes[lKey1])
	{
		case SHAPE_TCB:
		case SHAPE_BEZI:
		case SHAPE_HERM:
			{
				out=outgoing(lKey0,lKey1);
				in=incoming(lKey0,lKey1);
				hermite(t,&h1,&h2,&h3,&h4);
				float fTemp=h1*m_pfKeyValues[lKey0]+
					h2*m_pfKeyValues[lKey1]+h3*out+h4*in+offset;
				if(m_bUseExpression)
				{
					return m_pLRExpr->evalExpression(fTemp,fTime);
				}
				return fTemp;
			}
			break;
		case SHAPE_BEZ2:
			{
				float fTemp=bez2(lKey0,lKey1,fTime)+offset;
				if(m_bUseExpression)
				{
					return m_pLRExpr->evalExpression(fTemp,fTime);
				}
				return fTemp;
			}
			break;
		case SHAPE_LINE:
			{
				float fTemp=m_pfKeyValues[lKey0]
					+t*(m_pfKeyValues[lKey1]-m_pfKeyValues[lKey0])+offset;
				if(m_bUseExpression)
				{
					return m_pLRExpr->evalExpression(fTemp,fTime);
				}
				return fTemp;
			}
			break;
		case SHAPE_STEP:
			{
				float fTemp=m_pfKeyValues[lKey0]+offset;
				if(m_bUseExpression)
				{
					return m_pLRExpr->evalExpression(fTemp,fTime);
				}
				return fTemp;
			}
			break;
		default:
			{
				float fTemp=offset;
				if(m_bUseExpression)
				{
					return m_pLRExpr->evalExpression(fTemp,fTime);
				}
				return fTemp;
			}
			break;
	}
}

bool EnvelopeImpl::isStatic()
{
	return ((m_nNumKeys<=1) && m_pLRExpr->isStatic());
}

void EnvelopeImpl::destroy()
{
	delete this;
}

/**********************************
 Constructor, destructor, operators
 Motion class
***********************************/

MotionImpl::MotionImpl()
{
	DWORD	dwI=0;
	for(;dwI<9;dwI++)
	{
		m_pChannels[dwI]=new EnvelopeImpl();
	}
	//set dafault scaling to (1,1,1)
	long lTmp=0;
	m_pChannels[MOT_CHANNEL_SCALE_X]->addKey(&lTmp,0,1.0f);
	m_pChannels[MOT_CHANNEL_SCALE_Y]->addKey(&lTmp,0,1.0f);
	m_pChannels[MOT_CHANNEL_SCALE_Z]->addKey(&lTmp,0,1.0f);
}

MotionImpl::MotionImpl(MotionImpl &rfEnv)
{
	DWORD	dwI=0;
	for(;dwI<9;dwI++)
	{
		//TODO: EXTREMELY UNSAFE CODE!!!!!
		m_pChannels[dwI]->assign(rfEnv.m_pChannels[dwI]);
	}
}

MotionImpl::~MotionImpl()
{
	DWORD	dwI=0;
	for(;dwI<9;dwI++)
	{
		delete m_pChannels[dwI];
	}
}

bool MotionImpl::operator==(MotionImpl &rfEnv)
{
	DWORD	dwI=0;
	for(;dwI<9;dwI++)
	{
		//TODO: EXTREMELY UNSAFE CODE!!!!!
		if(!m_pChannels[dwI]->equals(rfEnv.m_pChannels[dwI]))
		{
			return false;
		}
	}
	return true;
}

void MotionImpl::operator=(MotionImpl &rfEnv)
{
	DWORD	dwI=0;
	for(;dwI<9;dwI++)
	{
		//TODO: EXTREMELY UNSAFE CODE!!!!!
		m_pChannels[dwI]->assign(rfEnv.m_pChannels[dwI]);
	}
}

bool MotionImpl::equals(MotionPtr pILRMot)
{
	MotionImpl *pMot=dynamic_cast<MotionImpl*>(pILRMot);
	if(pMot)
	{
		if(this->operator==(*pMot))
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

void MotionImpl::assign(MotionPtr pILRMot)
{
	MotionImpl *pMot=dynamic_cast<MotionImpl*>(pILRMot);
	if(pMot)
	{
		if(pMot!=this)
		{
			this->operator=(*pMot);
		}
	}
	else
	{
		clear();
		//TODO: throw exception?
	}
}

//internal utilities
void MotionImpl::clear()
{
	DWORD	dwI=0;
	for(;dwI<9;dwI++)
	{
		m_pChannels[dwI]->clear();
	}
	long lTmp=0;
	m_pChannels[MOT_CHANNEL_SCALE_X]->addKey(&lTmp,0,1.0f);
	m_pChannels[MOT_CHANNEL_SCALE_Y]->addKey(&lTmp,0,1.0f);
	m_pChannels[MOT_CHANNEL_SCALE_Z]->addKey(&lTmp,0,1.0f);
}

/*******************************
 Serialization
********************************/

void MotionImpl::unserializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs)
{
	clear();
	char	pTempOut[256];
	pfs->scanString(uVFileHandle,pTempOut);
	DWORD dwI=0;
	for(;dwI<9;dwI++)
	{
		pfs->scanString(uVFileHandle,pTempOut);
		m_pChannels[dwI]->unserializeASCII(uVFileHandle,pfs);
	}
	pfs->scanString(uVFileHandle,pTempOut);
}

void MotionImpl::unserializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs)
{
	DWORD	dwTmp=0;
	pfs->fread(uVFileHandle,4,&dwTmp);	//chunk ID
	if(dwTmp!=NLRChunkIDs::dwMotion)
	{
		throw FileFormatExceptionImpl("Motion chunk expected");
	}
	pfs->fread(uVFileHandle,4,&dwTmp);	//chunk length
	for(dwTmp=0;dwTmp<9;dwTmp++)
	{
		m_pChannels[dwTmp]->unserializeBinary(uVFileHandle,pfs);
	}
}

const char szChannelNames[][10]={
	"PositionX",
	"PositionY",
	"PositionZ",
	"Heading",
	"Pitch",
	"Bank",
	"ScaleX",
	"ScaleY",
	"ScaleZ"
};

void MotionImpl::serializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs,DWORD dwColumn)
{
	DWORD	dwTmp,dwTmp2;
	pfs->writeString(uVFileHandle," {\n");
	for(dwTmp=0;dwTmp<9;dwTmp++)
	{
		for(dwTmp2=0;dwTmp2<(dwColumn+1);dwTmp2++) pfs->writeString(uVFileHandle," ");
		pfs->writeString(uVFileHandle,szChannelNames[dwTmp]);
		m_pChannels[dwTmp]->serializeASCII(uVFileHandle,pfs,dwColumn+1);
	}
	for(dwTmp=0;dwTmp<dwColumn;dwTmp++)
	{
		pfs->writeString(uVFileHandle," ");
	}
	pfs->writeString(uVFileHandle,"}\n");
}

void MotionImpl::serializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs)
{
	DWORD dwTmp=NLRChunkIDs::dwMotion;
	pfs->fwrite(uVFileHandle,&dwTmp,4);
	dwTmp=getChunkLength();
	pfs->fwrite(uVFileHandle,&dwTmp,4);
	for(dwTmp=0;dwTmp<9;dwTmp++)
	{
		m_pChannels[dwTmp]->serializeBinary(uVFileHandle,pfs);
	}
}

DWORD MotionImpl::getChunkLength()
{
	//data to store 9*channel+9*4
	DWORD	dwLen=0;
	DWORD	dwTmp;
	for(dwTmp=0;dwTmp<9;dwTmp++)
	{
		dwLen+=m_pChannels[dwTmp]->getChunkLength()+4;
	}
	return dwLen;
}

//complex operations
void MotionImpl::getChannel(unsigned int uiChannel,EnvelopePtr *ppILREnv /*OUT*/)
{
	if(uiChannel>9)
	{
		throw IndexOutOfBoundsExceptionImpl(0,9,uiChannel);
	}
	*ppILREnv=m_pChannels[uiChannel];
}

float MotionImpl::getChannelValue(unsigned int uiChannel,float fTime)
{
	if(uiChannel>9)
	{
		throw IndexOutOfBoundsExceptionImpl(0,9,uiChannel);
	}
	return m_pChannels[uiChannel]->evalEnvelope(fTime);
}

bool MotionImpl::isStatic()
{
	bool all_static=true;
	DWORD dwI=0;
	for(;dwI<9;dwI++)
	{
		if(!m_pChannels[dwI]->isStatic())
		{
			all_static=false;
			break;
		}
	}
	return all_static;
}

void MotionImpl::destroy()
{
	delete this;
}

/**********************************
 Color class
 Constructor, destructor, operators
***********************************/


ColorImpl::ColorImpl()
{
	DWORD	dwI;
	for(dwI=0;dwI<4;dwI++)
	{
		m_pChannels[dwI]=new EnvelopeImpl();
	}
}

ColorImpl::ColorImpl(ColorImpl &rfEnv)
{
	DWORD	dwI;
	for(dwI=0;dwI<4;dwI++)
	{
		m_pChannels[dwI]->assign(rfEnv.m_pChannels[dwI]);
	}
}

ColorImpl::~ColorImpl()
{
	DWORD	dwI;
	for(dwI=0;dwI<4;dwI++)
	{
		delete m_pChannels[dwI];
	}
}

bool ColorImpl::operator==(ColorImpl &rfEnv)
{
	DWORD	dwI;
	for(dwI=0;dwI<4;dwI++)
	{
		//TODO: EXTREMELY UNSAFE CODE!!!!!
		if(!m_pChannels[dwI]->equals(rfEnv.m_pChannels[dwI]))
		{
			return false;
		}
	}
	return true;
}

void ColorImpl::operator=(ColorImpl &rfEnv)
{
	DWORD dwI;
	for(dwI=0;dwI<4;dwI++)
	{
		//TODO: EXTREMELY UNSAFE CODE!!!!!
		m_pChannels[dwI]->assign(rfEnv.m_pChannels[dwI]);
	}
}

bool ColorImpl::equals(ColorPtr pILRCol)
{
	ColorImpl *pCol=dynamic_cast<ColorImpl*>(pILRCol);
	if(pCol)
	{
		if(this->operator==(*pCol))
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

void ColorImpl::assign(ColorPtr pILRCol)
{
	ColorImpl *pCol=dynamic_cast<ColorImpl*>(pILRCol);
	if(pCol)
	{
		if(pCol!=this)
		{
			this->operator=(*pCol);
		}
	}
	else
	{
		clear();
		//TODO: throw exception?
	}
}

//internal utilities
void ColorImpl::clear()
{
	DWORD	dwI;
	for(dwI=0;dwI<4;dwI++)
	{
		m_pChannels[dwI]->clear();
	}
}

/*******************************
 Serialization
********************************/

void ColorImpl::unserializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs)
{
	clear();
	char	pTempOut[256];
	pfs->scanString(uVFileHandle,pTempOut);
	DWORD dwI;
	for(dwI=0;dwI<4;dwI++)
	{
		pfs->scanString(uVFileHandle,pTempOut);
		m_pChannels[dwI]->unserializeASCII(uVFileHandle,pfs);
	}
	pfs->scanString(uVFileHandle,pTempOut);
}

void ColorImpl::unserializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs)
{
	DWORD	dwTmp;
	pfs->fread(uVFileHandle,4,&dwTmp);	//chunk ID
	if(dwTmp!=NLRChunkIDs::dwMotion)
	{
		throw FileFormatExceptionImpl("Color chunk expected");
	}
	pfs->fread(uVFileHandle,4,&dwTmp);	//chunk length
	for(dwTmp=0;dwTmp<4;dwTmp++)
	{
		m_pChannels[dwTmp]->unserializeBinary(uVFileHandle,pfs);
	}
}

static const char szColorChannelNames[][2]=
{
	"R",
	"G",
	"B",
	"A"
};

void ColorImpl::serializeASCII(ULONG uVFileHandle,AutoPtr<fs> &pfs,DWORD dwColumn)
{
	DWORD	dwTmp,dwTmp2;
	pfs->writeString(uVFileHandle," {\n");
	for(dwTmp=0;dwTmp<4;dwTmp++)
	{
		for(dwTmp2=0;dwTmp2<(dwColumn+1);dwTmp2++)
		{
			pfs->writeString(uVFileHandle," ");
		}
		pfs->writeString(uVFileHandle,szColorChannelNames[dwTmp]);
		m_pChannels[dwTmp]->serializeASCII(uVFileHandle,pfs,dwColumn+1);
	}
	for(dwTmp=0;dwTmp<dwColumn;dwTmp++) pfs->writeString(uVFileHandle," ");
	pfs->writeString(uVFileHandle,"}\n");
}

void ColorImpl::serializeBinary(ULONG uVFileHandle,AutoPtr<fs> &pfs)
{
	DWORD dwTmp=NLRChunkIDs::dwMotion;
	pfs->fwrite(uVFileHandle,&dwTmp,4);
	dwTmp=getChunkLength();
	pfs->fwrite(uVFileHandle,&dwTmp,4);
	for(dwTmp=0;dwTmp<4;dwTmp++)
	{
		m_pChannels[dwTmp]->serializeBinary(uVFileHandle,pfs);
	}
}

DWORD ColorImpl::getChunkLength()
{
	//data to store 9*channel+9*4
	DWORD	dwLen=0;
	DWORD	dwTmp;
	for(dwTmp=0;dwTmp<9;dwTmp++)
	{
		dwLen+=m_pChannels[dwTmp]->getChunkLength()+4;
	}
	return dwLen;
}

//complex operations
void ColorImpl::getChannel(unsigned int uiChannel,EnvelopePtr *ppILREnv)
{
	if(uiChannel>4)
	{
		throw IndexOutOfBoundsExceptionImpl(0,4,uiChannel);
	}
	*ppILREnv=m_pChannels[uiChannel];
}

float ColorImpl::getChannelValue(unsigned int uiChannel,float fTime)
{
	if(uiChannel>4)
	{
		throw IndexOutOfBoundsExceptionImpl(0,4,uiChannel);
	}
	return m_pChannels[uiChannel]->evalEnvelope(fTime);
}

bool ColorImpl::isStatic()
{
	bool all_static=true;
	DWORD dwI=0;
	for(;dwI<4;dwI++)
	{
		if(!m_pChannels[dwI]->isStatic())
		{
			all_static=false;
			break;
		}
	}
	return all_static;
}

void ColorImpl::destroy()
{
	delete this;
}