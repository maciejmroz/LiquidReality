#include "stdafx.h"
#include "direct.h"

std::string extractFileNameFromFullPath(const char *pszFullPathName)
{
	std::string strTmp="";
	if(!pszFullPathName)
	{
		return strTmp;
	}
	const char *pszTmp=pszFullPathName;
	pszTmp+=strlen(pszFullPathName);
	while((pszTmp>pszFullPathName) && (*pszTmp!='\\'))
	{
		pszTmp--;
	}
	if(pszTmp!=pszFullPathName)
	{
		pszTmp++;
	}
	strTmp=pszTmp;
	return strTmp;
}

std::string extractDirectoryNameFromFullPath(const char *pszFullPathName)
{
	std::string strTmp="";
	if(!pszFullPathName)
	{
		return strTmp;
	}
	const char *pszTmp=pszFullPathName;
	pszTmp+=strlen(pszFullPathName);
	while((pszTmp>pszFullPathName) && (*pszTmp!='\\'))
	{
		pszTmp--;
	}
	if(pszTmp!=pszFullPathName)
	{
		pszTmp++;
	}
	const char *pszTmp2=pszFullPathName;
	while(pszTmp2<pszTmp)
	{
		strTmp+=*pszTmp2;
		pszTmp2++;
	}
	return strTmp;
}

static bool checkFileInProject(const char *pszFileName)
{
	std::string cstrTmp=globals.project;
	cstrTmp+=pszFileName;
	FILE *f=NULL;
	f=fopen(cstrTmp.c_str(),"r");
	if(f)
	{
		fclose(f);
		return true;
	}
	return false;
}

bool importFileToProject(const char *pszFullPathName,bool bAutoReplace,
						 const char *pszProjectSubDirectory)
{
	std::string cstrCutName=extractFileNameFromFullPath(pszFullPathName);
	if(pszProjectSubDirectory)
	{ //create directory tree if neccessary
		std::vector<std::string> DirectoryList;
		std::string strTmp="";
		const char *pszTmp=pszFullPathName;
		while(*pszTmp!='\0')
		{
			while((*pszTmp!='\\') && (*pszTmp!='\0'))
			{
				strTmp+=*pszTmp++;
			}
			DirectoryList.push_back(strTmp);
			strTmp="";
			if(*pszTmp!='\0')
			{
				pszTmp++;
			}
		}
		std::string cstrTmpDirectory=globals.project;
		cstrTmpDirectory+="\\";
		int i=0;
		int iVecSize=DirectoryList.size();
		for(i=0;i<iVecSize;i++)
		{
			cstrTmpDirectory+=DirectoryList[i].c_str();
			_mkdir(cstrTmpDirectory.c_str());
			cstrTmpDirectory+="\\";
		}
	}
	std::string cstrDestFileName=globals.project;
	if(globals.project.size()>0)
	{
		if(globals.project[globals.project.size()-1]!='\\')
		{
			cstrDestFileName+="\\";
		}
	}
	if(pszProjectSubDirectory)
	{
		cstrDestFileName+=pszProjectSubDirectory;
		cstrDestFileName+="\\";
	}
	cstrDestFileName+=cstrCutName;
	if(!bAutoReplace)
	{
		std::string cstrRelativePath="";
		if(pszProjectSubDirectory)
		{
			cstrRelativePath=pszProjectSubDirectory;
			cstrRelativePath+="\\";
		}
		cstrRelativePath+=cstrCutName;
		if(checkFileInProject(cstrRelativePath.c_str()))
		{
			return true;
		}
	}
	//check for self overwrite
	if(_stricmp(pszFullPathName,cstrDestFileName.c_str())==0)
	{
		return true;
	}
	//copy file
	FILE *f_src=NULL;
	FILE *f_dst=NULL;
	char szCopyBuf[4096];
	f_src=fopen(pszFullPathName,"rb");
	if(f_src)
	{
		f_dst=fopen(cstrDestFileName.c_str(),"wb");
		if(f_dst)
		{
			int nRead=0;
			do 
			{
				nRead=fread(szCopyBuf,1,4096,f_src);
				if(nRead!=0)
				{
					fwrite(szCopyBuf,1,nRead,f_dst);
				}
			} while(nRead==4096);
			fclose(f_dst);
			f_dst=NULL;
		}
		fclose(f_src);
		f_src=NULL;
	}
	return true;
}
