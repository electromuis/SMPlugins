#pragma once

#include "config.hpp"
#include "global.h"
#include "PluginManager.h"
#include "MessageManager.h"

#include <string>

struct ButtonMapEntry {
	std::string name;
	int player;
	int buttonNum;
	int sensorNum;
	int subNum;
};

class AdpPlugin : public PluginBase {
public:
	AdpPlugin();
	~AdpPlugin();

	void Update(float fDeltaTime);
	void SetThreshold(int sensorNum, float threshold);
	std::vector<PluginRegType>* GetLuaFunctions();
	std::vector<ButtonMapEntry> ButtonMap();
};
