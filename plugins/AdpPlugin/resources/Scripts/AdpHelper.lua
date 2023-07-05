local pl = PLUGINMAN:GetPlugin("AdpPlugin")

local sensorMap = {}

function AdpPlayerOptionLines()
	if not pl then
		Trace("ADP not loaded")
		return {}
	end

	sensorMap = {}

	local mapRows = pl:ButtonMapRows()
	local ret = {}

	for r=1,mapRows do
		local mapRow = pl:ButtonMap(r)
		local rowName = "Adp" .. mapRow["Name"] .. mapRow["SubNumber"]

		ret[#ret+1] = rowName
		sensorMap[rowName] = mapRow
	end

	ret[#ret+1] = "ScreenAfterPlayerOptions4"

	return table.concat(ret, ",") --> "a,b,c"
end

function AdpOptionSensor(optionName, pn)
	if not sensorMap[optionName] then
		Trace("No sensor for " .. optionName)
		return nil
	end

	local sensorNum = sensorMap[optionName]["Sensor"]
	local sensorState = pl:SensorState(sensorNum)
	return sensorState
end

function AdpOptionSensorSet(optionName, threshold, pn)
	if not sensorMap[optionName] then
		Trace("No sensor for " .. optionName)
		return
	end

	local sensorNum = sensorMap[optionName]["Sensor"]
	pl:SensorSetThreshold(sensorNum, threshold)
end



Trace("AdpHelper.lua loaded")
