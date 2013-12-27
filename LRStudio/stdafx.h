// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

// TODO: reference additional headers your program requires here
#include "lr_interface.h"

extern std::string extractFileNameFromFullPath(const char *pszFullPathName);
extern std::string extractDirectoryNameFromFullPath(const char *pszFullPathName);
extern bool importFileToProject(const char *pszFullPathName,bool bAutoReplace,
						 const char *pszProjectSubDirectory=NULL);
