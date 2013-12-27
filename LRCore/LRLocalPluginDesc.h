/*******************************************************************************
 LiquidReality engine local (inside-plugin) plugin descriptor class
 Implemented in LRCorePlugins.cpp
 (c) 2002 Maciej Mróz
 *******************************************************************************/

#ifndef _LR_LOCAL_PLUGIN_DESC_INCLUDED_
#define _LR_LOCAL_PLUGIN_DESC_INCLUDED_

namespace LR
{

	class LocalPluginDesc
	{
	public:
		PluginDesc		Desc;
		void			*m_pRefObj;
		LocalPluginDesc();
		LocalPluginDesc(DWORD dwType,const char *pszSuperClassName,
					const char *pszClassName,void *pRefObject);
		LocalPluginDesc(const LocalPluginDesc& rfDesc);
		~LocalPluginDesc();
		void operator=(const LocalPluginDesc &rfDesc);
		bool operator==(const LocalPluginDesc &rfDesc);
		bool operator<(const LocalPluginDesc &rfDesc);
	};

}

#endif