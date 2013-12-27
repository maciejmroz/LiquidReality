/*******************************************************************************
 LiquidReality engine dll globals
 (c) 2002 Maciej Mróz
 *******************************************************************************/

#ifndef _LR_DLL_GLOBALS_INCLUDED_
#define _LR_DLL_GLOBALS_INCLUDED_

namespace LR {

	//LRCore contains two logical parts - "main" module (engine)
	//and "plugins" module. They are logically and physically separate
	//(minor modifications are needed for them to be separate dll-s)
	//and this separation is also kept here
	namespace NMainDllGlobals {
		extern AutoPtr<Engine>		g_pEngine;
		extern AutoPtr<D3D>			g_pD3D;
		extern HINSTANCE			g_hInstance;
	};

	class LocalPluginDesc;

	namespace NPluginDllGlobals {
		extern AutoPtr<Engine>					plg_pEngine;
		extern LRTPtrTable<LocalPluginDesc*>	*plg_pLocalPlugins;
	};

}

#endif