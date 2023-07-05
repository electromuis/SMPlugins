#pragma once

#include "config.hpp"
#include "global.h"
#include "PluginManager.h"
#include "MessageManager.h"
#include "lrdb/server.hpp"

class LuaDebugPlugin : public PluginBase {
public:
	LuaDebugPlugin();
	~LuaDebugPlugin();

	void Update(float fDeltaTime);
protected:
	int listen_port = 21110;
	lrdb::server debug_server;
};
