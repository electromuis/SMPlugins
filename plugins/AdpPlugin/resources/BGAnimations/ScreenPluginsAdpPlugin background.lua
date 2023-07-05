local pl = PLUGINMAN:GetPlugin("AdpPlugin")
if not pl then
	Trace("ADP not loaded")
	return Def.ActorFrame{}
end

local pad = pl:PadState()
if not pad then
	Trace("No pad connected")
	return Def.ActorFrame{}
end

local mapRows = pl:ButtonMapRows()

for r=1,mapRows do
	local mapRow = pl:ButtonMap(r)
	SM(mapRow)
end

local af = Def.ActorFrame{
	OnCommand=function(self)
		self:queuecommand("Update")
	end,
	UpdateCommand=function(self)
		self:sleep(1/30)
		self:queuecommand("Update")

		for s=1,pad["NumSensors"] do
			self:GetChild("Sensor"..s):queuecommand("Update")
		end
	end
}

for s=1,pad["NumSensors"] do
	af[#af+1] = Def.Quad{
		Name="Sensor"..s,
		InitCommand=function(self)
			self:y(40 + s*55)
			self:zoomto(SCREEN_WIDTH*0.8, 50)
			self:diffuse(0,0,0,0.5)
		end,
		UpdateCommand=function(self)
			local sensor = pl:SensorState(s-1)

			if sensor["Pressed"] then
				self:diffuse(0,0.5,0,1)
			else
				self:diffuse(0.5,0,0,1)
			end

			self:SetWidth(sensor["Value"] * 3)
		end
	}
end

return af
