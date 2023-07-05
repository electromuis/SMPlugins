#include "global.h"
#include "AdpPlugin.h"

#include "RageLog.h"
#include "InputMapper.h"
#include "GameInput.h"
#include "RageInput.h"
#include <Model/Device.h>
#include <Model/Log.h>
#include <algorithm>

REGISTER_PLUGIN(AdpPlugin, "0.0.1", "Electromuis")

AdpPlugin::AdpPlugin()
{
	adp::Log::Init();
	adp::Device::Init();
	adp::Device::Update();
	auto pad = adp::Device::Pad();
	if (pad) {
		LOG->Trace("ADP Pad found :D");
	}
	else {
		LOG->Trace("ADP Pad not found :(");
	}
}

AdpPlugin::~AdpPlugin()
{
	adp::Device::Shutdown();
	adp::Log::Shutdown();
}


void AdpPlugin::Update(float fDeltaTime)
{
	adp::Device::Update();
}

std::vector<ButtonMapEntry> AdpPlugin::ButtonMap()
{
	std::vector<ButtonMapEntry> ret;
	auto pad = adp::Device::Pad();
	if (!pad)
		return ret;

	std::vector <InputDeviceInfo>* devices = APP_PTR(std::vector<InputDeviceInfo>);
	devices->reserve(20);

	//std::vector < InputDeviceInfo> devices(20);
	std::string deviceList = INPUTMAN->GetDisplayDevicesString();
	int searchIdx = 0;
	do {
		int dIdx = deviceList.find_first_of("(", searchIdx) - searchIdx;
		std::string dName = deviceList.substr(searchIdx, dIdx-1);

		int jIdx = deviceList.find_first_of(")", dIdx) - dIdx;
		std::string jName = deviceList.substr(dIdx+1, jIdx);


		searchIdx = deviceList.find_first_of("\n", searchIdx+1);
	} while (searchIdx != string::npos);
	
	int b = INPUTMAPPER->GetInputScheme()->m_iButtonsPerController;
	auto scheme = INPUTMAPPER->GetInputScheme();

	FOREACH_ENUM(GameController, p)
	{
		for (auto& gb : scheme->m_GameButtonInfo) {
			if (gb.m_szName == nullptr) {
				continue;
			}
		}
		FOREACH_GameButtonInScheme(INPUTMAPPER->GetInputScheme(), gb)
		{
			std::string gbName = scheme->GetGameButtonName(gb);
			if (gbName.size() == 0) {
				continue;
			}

			for (int s = 0; s < NUM_SHOWN_GAME_TO_DEVICE_SLOTS; s++)
			{
				GameInput cur_gi(p, gb);
				DeviceInput di;

				if (!INPUTMAPPER->GameToDevice(cur_gi, s, di))
					continue;

				if (di.device != DEVICE_JOY2)
					continue;

				int buttonNum = (di.button - JOY_BUTTON_1) + 1;

				for (int s = 0; s < pad->numSensors; s++) {
					auto sensor = adp::Device::Sensor(s);

					if (sensor->button != buttonNum)
						continue;

					auto lastButtonEntry = std::find_if(ret.begin(), ret.end(), [buttonNum](ButtonMapEntry& entry) {
						return entry.buttonNum == buttonNum;
					});

					int subNum = 1;
					if (lastButtonEntry != ret.end()) {
						subNum = lastButtonEntry->subNum + 1;
					}

					ret.push_back({
						gbName,
						p,
						buttonNum,
						s,
						subNum
					});
				}
			}
		}
	}

	return ret;
}

void AdpPlugin::SetThreshold(int sensorNum, float threshold)
{
	auto pad = adp::Device::Pad();
	if (!pad)
		return;

	if (sensorNum < 0 || sensorNum >= pad->numSensors)
		return;

	adp::Device::SetThreshold(sensorNum, threshold);
}

// Lua methods

static int LuaPadState(LoadedPlugin* p, lua_State* L)
{
	AdpPlugin* me = (AdpPlugin*)p->GetPlugin();

	auto pad = adp::Device::Pad();
	if (!pad) {
		lua_pushnil(L);
	}
	else {
		lua_newtable(L);

		lua_pushstring(L, pad->name.c_str());
		lua_setfield(L, -2, "Name");

		lua_pushinteger(L, pad->numButtons);
		lua_setfield(L, -2, "NumButtons");

		lua_pushinteger(L, pad->numSensors);
		lua_setfield(L, -2, "NumSensors");
	}

	return 1;
}

static int LuaSensorState(LoadedPlugin* p, lua_State* L)
{
	AdpPlugin* me = (AdpPlugin*)p->GetPlugin();

	auto pad = adp::Device::Pad();
	if (!pad) {
		lua_pushnil(L);
		return 1;
	}

	int sensorNum = 0;
	if (!LuaHelpers::Pop<int>(L, sensorNum)) {
		lua_pushnil(L);
		return 1;
	}

	if (sensorNum < 0 || sensorNum >= pad->numSensors) {
		lua_pushnil(L);
		return 1;
	}

	auto sensorState = adp::Device::Sensor(sensorNum);
	if (!sensorState) {
		lua_pushnil(L);
		return 1;
	}

	lua_newtable(L);

	lua_pushnumber(L, sensorNum);
	lua_setfield(L, -2, "Index");

	lua_pushnumber(L, sensorState->value);
	lua_setfield(L, -2, "Value");

	lua_pushnumber(L, sensorState->threshold);
	lua_setfield(L, -2, "Threshold");

	lua_pushboolean(L, sensorState->pressed);
	lua_setfield(L, -2, "Pressed");

	lua_pushinteger(L, sensorState->button);
	lua_setfield(L, -2, "Button");

	return 1;
}

static int LuaButtonMap(LoadedPlugin* p, lua_State* L)
{
	AdpPlugin* me = (AdpPlugin*)p->GetPlugin();
	auto map = me->ButtonMap();
	int index = 0;
	if (!LuaHelpers::Pop<int>(L, index) || index > map.size() || index < 1) {
		lua_pushnil(L);
		return 1;
	}

	ButtonMapEntry m = map.at(index-1);
	lua_newtable(L);

	lua_pushstring(L, m.name.c_str());
	lua_setfield(L, -2, "Name");

	lua_pushinteger(L, m.buttonNum);
	lua_setfield(L, -2, "Button");

	lua_pushinteger(L, m.sensorNum);
	lua_setfield(L, -2, "Sensor");

	lua_pushinteger(L, m.player);
	lua_setfield(L, -2, "Player");

	lua_pushinteger(L, m.subNum);
	lua_setfield(L, -2, "SubNumber");
	
	return 1;
}

static int LuaButtonMapRows(LoadedPlugin* p, lua_State* L)
{
	AdpPlugin* me = (AdpPlugin*)p->GetPlugin();
	auto map = me->ButtonMap();

	lua_pushinteger(L, map.size());
	return 1;
}

static int LuaSensorSetThreshold(LoadedPlugin* p, lua_State* L)
{
	AdpPlugin* me = (AdpPlugin*)p->GetPlugin();
	int sensorNum;
	float threshold;

	if (!LuaHelpers::Pop<float>(L, threshold))
		return 1;

	if (!LuaHelpers::Pop<int>(L, sensorNum))
		return 1;

	me->SetThreshold(sensorNum, threshold);
}

std::vector<PluginRegType>* AdpPlugin::GetLuaFunctions()
{
	std::vector<PluginRegType>* out = new std::vector<PluginRegType>();

	out->push_back({ "PadState", LuaPadState });
	out->push_back({ "SensorState", LuaSensorState });
	out->push_back({ "ButtonMap", LuaButtonMap });
	out->push_back({ "ButtonMapRows", LuaButtonMapRows });
	out->push_back({ "SensorSetThreshold", LuaSensorSetThreshold });

	return out;
}
