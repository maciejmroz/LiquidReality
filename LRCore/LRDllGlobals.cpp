/*******************************************************************************
 LiquidReality engine dll globals - this file contains DllMain function
 that does all initialization and cleanup
 (c) 2002 Maciej Mróz
 *******************************************************************************/

#include "StdAfx.h"
#include "LRLocalPluginDesc.h"

using namespace LR;
using namespace NMainDllGlobals;
using namespace NPluginDllGlobals;

AutoPtr<Engine> NMainDllGlobals::g_pEngine;
HINSTANCE NMainDllGlobals::g_hInstance;
AutoPtr<Engine> NPluginDllGlobals::plg_pEngine;
AutoPtr<D3D> NMainDllGlobals::g_pD3D;
LRTPtrTable<LocalPluginDesc*>	*NPluginDllGlobals::plg_pLocalPlugins=NULL;

int WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved) {
	switch(fdwReason) {
		case DLL_PROCESS_ATTACH:
			//initialization
			g_hInstance=hInstance;
			plg_pLocalPlugins=new LRTPtrTable<LocalPluginDesc*>();
			return TRUE;
		case DLL_PROCESS_DETACH:
			//cleanup
			g_pD3D=NULL;
			plg_pEngine=NULL;
			g_pEngine=NULL;
			delete plg_pLocalPlugins;
			return TRUE;
		default:
			return TRUE;
	}
}
