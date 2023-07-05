#pragma once

#include "global.h"
#include "LuaDebugPlugin.h"

#include "RageLog.h"
#include "LuaManager.h"

REGISTER_PLUGIN(LuaDebugPlugin, "0.0.1", "Electromuis")

LuaDebugPlugin::LuaDebugPlugin()
	:debug_server(listen_port)
{
	
}

LuaDebugPlugin::~LuaDebugPlugin()
{
	
}

void LuaDebugPlugin::Update(float fDeltaTime)
{

}
