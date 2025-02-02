#pragma once
namespace PluginCore
{
	bool Setup();
	void EarlyInit();
	void OnTyInitialized();
	void Tick(float deltaSeconds);
};

