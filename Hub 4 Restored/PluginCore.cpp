#include "pch.h"
#include "TygerFrameworkAPI.hpp"
#include "TyMemoryValues.h"
#include "PluginCore.h"
#include "LevelSelect.h"

#include "core.h"
#include "menu.h"

bool PluginCore::Setup()
{
    Core::initialize((HMODULE)API::Get()->param()->TyHModule);
    TyMemoryValues::DisableGameInfoID(LevelCode::E3);
    return true;
}

void PluginCore::OnTyInitialized()
{
    LevelSelect::SetupMultiColumn();
}

void PluginCore::Tick(float deltaSeconds)
{
    if (Menu::GetMenuIndex() == MenuType::LevelSelect)
        LevelSelect::HandleInput();
}
