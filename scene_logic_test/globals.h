
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define LRCORE_DYNAMIC_LOAD
#include "../LRCore/LRCore.h"
#include <string>

extern LR::AutoPtr<LR::Engine>		g_pEngine;
extern LR::AutoPtr<LR::Scene>		g_pCurrentScene;
extern LR::AutoPtr<LR::Envelope>	g_pEnvelopeClipboard;
extern LR::AutoPtr<LR::Motion>		g_pMotionClipboard;
extern LR::AutoPtr<LR::Color>		g_pColorClipboard;
extern LR::AutoPtr<LR::fs>		g_pLRfs;
extern LR::AutoPtr<LR::D3D>		g_pRenderer;
extern CString		g_cstrProjectRoot;
extern CString		g_cstrFilesystemRoot;
extern CString		g_cstrLiquidRealityRoot;
extern HMODULE		g_hLRCoreDll;
extern LR::PFNGETENGINE	GetEngine;

//checks if file exist in current project
//file is given with relative path ie
//project_subdir\filename.ext
//return TRUE is file already exists
extern BOOL checkFileInProject(const char *pszFileName);

//cut out file name from full path ie:
//"D:\Projects\subdir\filename.ext" -> "filename.ext"
extern CString extractFileNameFromFullPath(const char *pszFullPathName);
//... and version returning std::string :))
extern std::string extractFileNameFromFullPathSTD(const char *pszFullPathName);

//cur directory name from full path (including ending slash)
//"D:\Projects\subdir\filename.ext" -> "D:\Projects\subdir\"
extern CString extractDirectoryNameFromFullPath(const char *pszFullPathName);
//std:: version
extern std::string extractDirectoryNameFromFullPathSTD(const char *pszFullPathName);

//copy file to project directory, optionally to one of its
//subdirectories (third argument, if subdirectory tree does not exist
//then it is created)
//if bAutoReplace is FALSE then call fails for files that already
//exist in current project
BOOL importFileToProject(const char *pszFullPathName,BOOL bAutoReplace=TRUE,
						 const char *pszProjectSubDirectory=NULL);
#endif