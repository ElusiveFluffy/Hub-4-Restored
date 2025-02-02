#include "pch.h"
#include "TygerFrameworkAPI.hpp"
#include "TyMemoryValues.h"
#include "TyFunctions.h"
#include "PluginCore.h"
#include "LevelSelect.h"
#include "Collectables.h"
#include "Levels.h"
#include "Rangs.h"

#include "MinHook.h"

//TygerMemory
#include "core.h"
#include "minimap.h"
#include "gamestate.h"
#include "hero.h"

void InitMinHook() {
    MH_STATUS minHookStatus = MH_Initialize();
    if (minHookStatus != MH_OK) {
        std::string error = MH_StatusToString(minHookStatus);
        API::LogPluginMessage("Failed to Initialize Minhook, With the Error: " + error, Error);
    }
}

bool PluginCore::Setup()
{
    InitMinHook();

    TyMemoryValues::DisableGameInfoID(LevelCode::E3);
    TyFunctions::SetFuntions();

    Collectables::Setup();
    Levels::HookFunctions();

    Rangs::HookCycleFunctions();

    return true;
}

void PluginCore::EarlyInit()
{
    Core::initialize((HMODULE)GetModuleHandle(0));
    Rangs::SetupRangStructs();
}

void PluginCore::OnTyInitialized()
{
    LevelSelect::SetupMultiColumn();
    //Setup minimap
    Minimap::SetDisabledMapLevel((char)LevelCode::E3);
    Minimap::SetMapTransform(LevelCode::Z3, Minimap::GetMapTransform(LevelCode::E2));
    Minimap::SetMapTransform(LevelCode::E2, Minimap::GetMapTransform(LevelCode::D2));
    Minimap::SetMapTransform(LevelCode::E3, Minimap::GetMapTransform(LevelCode::D1));
    Minimap::SetMapTransform(LevelCode::D1, Minimap::GetMapTransform(LevelCode::B3));
}

void PluginCore::Tick(float deltaSeconds)
{
    if (GameState::getPauseMenuScreen() == PauseMenuScreen::ChangeLevel)
        LevelSelect::HandleInput();
}
