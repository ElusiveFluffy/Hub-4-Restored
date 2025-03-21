// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "TygerFrameworkAPI.hpp"
#include "core.h"
#include "PluginCore.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        PluginCore::EarlyInit();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

EXTERN_C void TygerFrameworkPluginRequiredVersion(TygerFrameworkPluginVersion* version) {
    //Specifiy the version number defined in the API
    version->Major = TygerFrameworkPluginVersion_Major;
    version->Minor = TygerFrameworkPluginVersion_Minor;
    version->Patch = TygerFrameworkPluginVersion_Patch;

    version->CompatibleGames = { 1 };
}

EXTERN_C bool TygerFrameworkPluginInitialize(TygerFrameworkPluginInitializeParam* param) {

    API::Initialize(param);

    //Subcribe functions to TygerFramework events
    API::AddOnTyInitialized(PluginCore::OnTyInitialized);
    API::AddTickBeforeGame(PluginCore::Tick);

    return PluginCore::Setup();
}