#include "MainRenderer.h"
#include <MinHook.h>
#include "TygerFrameworkAPI.hpp"
#include "TyFunctions.h"
#include <unordered_set>

//TygerMemory
#include "core.h"

using TyMainRenderer_t = int(*)();
TyMainRenderer_t Original_MainRenderer = nullptr;
std::unordered_set<Model*> Models{};

void WaitMainRenderer() {
	Original_MainRenderer();
	for (auto& model : Models) {
		model->Draw();
	}
}

bool MainRenderer::HookFunctions()
{
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x77350), &WaitMainRenderer, reinterpret_cast<LPVOID*>(&Original_MainRenderer));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Main Render Function Hook, With the Error: " + error, Error);
		return false;
	}
	return true;
}

void MainRenderer::AddModel(Model* pModel)
{
	if (pModel)
		Models.emplace(pModel);
}

void MainRenderer::RemoveModel(Model* pModel)
{
	Models.erase(pModel);
}
