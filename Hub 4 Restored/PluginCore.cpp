#include "pch.h"
#include "TygerFrameworkAPI.hpp"
#include "TyMemoryValues.h"
#include "PluginCore.h"
#include "LevelSelect.h"

//TygerMemory
#include "core.h"
#include "minimap.h"
#include "gamestate.h"

bool PluginCore::Setup()
{
    Core::initialize((HMODULE)API::Get()->param()->TyHModule);
    TyMemoryValues::DisableGameInfoID(LevelCode::E3);
    return true;
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
