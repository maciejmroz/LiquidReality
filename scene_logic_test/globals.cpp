/*
 *	globals.cpp
 *  A few global utility functions used in LiquidReality
 *  editor application
 *  (c) 2002 Maciej Mróz
 */

#include "stdafx.h"
#include "globals.h"
#include <vector>
#include <string>
#include <direct.h>

BOOL checkFileInProject(const char *pszFileName) {
	CString cstrTmp=g_cstrProjectRoot;
	cstrTmp+="\\";
	cstrTmp+=pszFileName;
	FILE *f=NULL;
	f=fopen(cstrTmp,"r");
	if(f) {
		fclose(f);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

CString extractFileNameFromFullPath(const char *pszFullPathName) {
	CString cstrTmp="";
	if(!pszFullPathName) return cstrTmp;
	char *pszTmp=NULL;
	pszTmp=const_cast<char*>(pszFullPathName);
	pszTmp+=strlen(pszFullPathName);
	while((pszTmp>pszFullPathName) && (*pszTmp!='\\')) pszTmp--;
	if(pszTmp!=pszFullPathName) pszTmp++;
	cstrTmp=pszTmp;
	return cstrTmp;
}

std::string extractFileNameFromFullPathSTD(const char *pszFullPathName) {
	CString cstring=extractFileNameFromFullPath(pszFullPathName);
	std::string std_string=(LPCSTR)cstring;
	return std_string;
}

CString extractDirectoryNameFromFullPath(const char *pszFullPathName) {
	CString cstrTmp="";
	if(!pszFullPathName) return cstrTmp;
	char *pszTmp=NULL;
	pszTmp=const_cast<char*>(pszFullPathName);
	pszTmp+=strlen(pszFullPathName);
	while((pszTmp>pszFullPathName) && (*pszTmp!='\\')) pszTmp--;
	if(pszTmp!=pszFullPathName) pszTmp++;
	const char *pszTmp2=pszFullPathName;
	while(pszTmp2<pszTmp) {
		cstrTmp+=*pszTmp2;
		pszTmp2++;
	}
	return cstrTmp;
}

std::string extractDirectoryNameFromFullPathSTD(const char *pszFullPathName) {
	CString cstring=extractDirectoryNameFromFullPath(pszFullPathName);
	std::string std_string=(LPCSTR)cstring;
	return std_string;
}

BOOL importFileToProject(const char *pszFullPathName,BOOL bAutoReplace,
						 const char *pszProjectSubDirectory) {
	CString cstrCutName=extractFileNameFromFullPath(pszFullPathName);
	if(pszProjectSubDirectory) { //create directory tree if neccessary
		std::vector<std::string> DirectoryList;
		std::string strTmp="";
		char *pszTmp=const_cast<char*>(pszFullPathName);
		while(*pszTmp!='\0') {
			while((*pszTmp!='\\') && (*pszTmp!='\0')) strTmp+=*pszTmp++;
			DirectoryList.push_back(strTmp);
			strTmp="";
			if(*pszTmp!='\0') pszTmp++;
		}
		CString cstrTmpDirectory=g_cstrFilesystemRoot;
		cstrTmpDirectory+="\\";
		int i=0;
		int iVecSize=DirectoryList.size();
		for(i=0;i<iVecSize;i++) {
			cstrTmpDirectory+=DirectoryList[i].c_str();
			_mkdir(cstrTmpDirectory);
			cstrTmpDirectory+="\\";
		}
	}
	CString cstrDestFileName=g_cstrFilesystemRoot;
	if(g_cstrFilesystemRoot.GetLength()>0) {
		if(((LPCSTR)g_cstrFilesystemRoot)[g_cstrFilesystemRoot.GetLength()-1]!='\\') {
			cstrDestFileName+="\\";
		}
	}
	if(pszProjectSubDirectory) {
		cstrDestFileName+=pszProjectSubDirectory;
		cstrDestFileName+="\\";
	}
	cstrDestFileName+=cstrCutName;
	if(!bAutoReplace) {
		CString cstrRelativePath="";
		if(pszProjectSubDirectory) {
			cstrRelativePath=pszProjectSubDirectory;
			cstrRelativePath+="\\";
		}
		cstrRelativePath+=cstrCutName;
		if(checkFileInProject(cstrRelativePath)) {
			return TRUE;
		}
	}
	//check for self overwrite
	if(_stricmp(pszFullPathName,cstrDestFileName)==0) {
		return TRUE;
	}
	//copy file
	FILE *f_src=NULL;
	FILE *f_dst=NULL;
	char szCopyBuf[4096];
	f_src=fopen(pszFullPathName,"rb");
	if(f_src) {
		f_dst=fopen(cstrDestFileName,"wb");
		if(f_dst) {
			int nRead=0;
			do {
				nRead=fread(szCopyBuf,1,4096,f_src);
				if(nRead!=0) {
					fwrite(szCopyBuf,1,nRead,f_dst);
				}
			} while(nRead==4096);
			fclose(f_dst);
			f_dst=NULL;
		}
		fclose(f_src);
		f_src=NULL;
	}
	return TRUE;
}
