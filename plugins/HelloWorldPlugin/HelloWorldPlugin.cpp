#pragma once

#include "global.h"
#include "HelloWorldPlugin.h"

#include "RageLog.h"

REGISTER_PLUGIN(HelloWorldPlugin, "0.0.1", "Electromuis")


HelloWorldPlugin::HelloWorldPlugin()
	:subscriber(this)
{
	LOG->Info("HelloWorldPlugin loaded internal");
}

HelloWorldPlugin::~HelloWorldPlugin()
{
	LOG->Info("HelloWorldPlugin unloaded internal");
}

void HelloWorldPlugin::Stepped()
{
	LOG->Info("HelloWorldPlugin: Test plugin feels a step");
}

void HelloWorldPlugin::Update(float fDeltaTime)
{

}

HelloWorldMessageSubscriber::HelloWorldMessageSubscriber(HelloWorldPlugin* plugin)
	:plugin(plugin)
{
	SubscribeToMessage("Step");
}

HelloWorldMessageSubscriber::~HelloWorldMessageSubscriber()
{
	UnsubscribeAll();
}

void HelloWorldMessageSubscriber::HandleMessage(const Message& msg)
{
	plugin->Stepped();
}
