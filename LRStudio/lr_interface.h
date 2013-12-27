#pragma once

#define WINVER			0x0500
#define _WIN32_WINNT	0x0500
#define _WIN32_IE		0x0500
#include "LRCore/LRCore.h"
#undef GetObject
#undef MessageBox
#include <vector>
#include <string>
#include "LWOImport.h"
#include "LWSImport.h"

enum EnvelopeChannel
{
	ecStandalone,
	ecColor,
	ecMotion
};

struct GlobalsDesc
{
	LR::AutoPtr<LR::Engine>		engine;
	LR::AutoPtr<LR::D3D>		renderer;
	LR::ScenePtr				scene;				//currently rendered scene, if any
	int							active_camera;		//camera index or -1 (no camera in scene)
	LR::AutoPtr<LR::fs>			filesystem;			//current filesystem, none if no project is loaded
													//otherwise (primary) root directory is set to
													//project root and filesystem is set as default on
													//engine object
	std::string					current_obj_path_prefix;	//SC/MN/TN/TX/SF/FX etc
	LR::AutoPtr<LR::BaseObject>	current_obj;		//currently edited object
	std::string					project;			//absolute directory path, including trailing '\'

	EnvelopeChannel				channel_id;
	LR::MotionPtr				current_motion;
	LR::ColorPtr				current_color;
	LR::EnvelopePtr				current_env;

	GlobalsDesc() :
	active_camera(-1),
	channel_id(ecStandalone)
	{
		clean();
	}

	void clean()
	{
		current_env=NULL;
		current_motion=NULL;
		current_color=NULL;
		current_obj=NULL;
		scene=NULL;
	}

	void unloadAll()
	{
		if(engine.isNull())
		{
			return;
		}
		engine->unloadAll();
	}
};

extern GlobalsDesc globals;

