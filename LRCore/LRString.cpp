
#include "StdAfx.h"

LRString::LRString()
{
	m_pszString=NULL;
}

LRString::LRString(const char *pszString)
{
	m_pszString=new char[strlen(pszString)+1];
	strcpy(m_pszString,pszString);
}

LRString::LRString(const LRString &rfString)
{
	m_pszString=new char[strlen(rfString.m_pszString)+1];
	strcpy(m_pszString,rfString.m_pszString);
}

void LRString::operator=(const LRString &strRef)
{
	if(m_pszString)
	{
		delete [] m_pszString;
	}
	m_pszString=NULL;
	if(!strRef.m_pszString)
	{
		return;
	}
	m_pszString=new char[strlen(strRef.m_pszString)+1];
	strcpy(m_pszString,strRef.m_pszString);
}

void LRString::operator=(const char *pszString)
{
	if(m_pszString)
	{
		delete [] m_pszString;
	}
	m_pszString=NULL;
	if(!pszString)
	{
		return;
	}
	m_pszString=new char[strlen(pszString)+1];
	strcpy(m_pszString,pszString);
}

void LRString::operator+=(const char *pszString) {
	if(!pszString)
	{
		return;
	}
	if(!m_pszString)
	{
		operator=(pszString);
		return;
	}
	char *pszTempString=new char[strlen(m_pszString)+strlen(pszString)+1];
	pszTempString[0]='\0';
	strcat(pszTempString,m_pszString);
	strcat(pszTempString,pszString);
	delete [] m_pszString;
	m_pszString=pszTempString;
}

void LRString::operator+=(const LRString &rfString)
{
	operator+=(rfString.m_pszString);
}

int LRString::operator<(const LRString &rfString)
{
	if(!m_pszString)
	{
		return 0;
	}
	if(!rfString.m_pszString)
	{
		return 0;
	}
	if(strcmp(m_pszString,rfString.m_pszString)<0)
	{
		return 1;
	}
	return 0;
}

int LRString::operator==(const LRString &rfString)
{
	if((!m_pszString) && (!rfString.m_pszString))
	{
		return 1;
	}
	if(!m_pszString)
	{
		return 0;
	}
	if(!rfString.m_pszString)
	{
		return 0;
	}
	if(strcmp(m_pszString,rfString.m_pszString)==0)
	{
		return 1;
	}
	return 0;
}

void LRString::format(const char* pszFormatString, ...)
{
	char szBuffer[4096];
	va_list arg_list;
	va_start(arg_list,pszFormatString);
	vsprintf(szBuffer,pszFormatString,arg_list);
	va_end(arg_list);
	operator=(szBuffer);
}

LRString::~LRString()
{
	delete [] m_pszString;
	m_pszString=NULL;
}

int LRString::size() const
{
	if(!m_pszString)
	{
		return 0;
	}
	return strlen(m_pszString);
}
