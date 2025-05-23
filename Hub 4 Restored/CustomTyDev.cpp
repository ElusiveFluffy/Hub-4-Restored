#include "pch.h"
#include "CustomTyDev.h"
#include "MinHook.h"
#include "TyFunctions.h"
#include "SaveFile.h"
#include "TygerFrameworkAPI.hpp"

//Tyger Memory
#include "core.h"

typedef bool(__fastcall* SetSaveFileData_t)(int arg1);
SetSaveFileData_t Original_SetSaveFileData;

bool __fastcall CustomTyDev::SetSaveFileData(int arg1)
{
	bool isTyDev = Original_SetSaveFileData(arg1);

	if (!isTyDev)
		return isTyDev;

	//Additional save file edits
	Hub4SaveDataStruct* saveData = SaveFile::GetHub4SaveData();
	saveData->AttributeData.GotAquarang = true;
	saveData->AttributeData.GotZappyrang = true;
	saveData->AttributeData.GotZoomerang = true;
	saveData->AttributeData.GotMultirang = true;
	saveData->AttributeData.GotInfrarang = true;
	saveData->AttributeData.GotMegarang = true;
	saveData->AttributeData.GotKaboomerang = true;
	saveData->AttributeData.GotChronorang = true;
	saveData->AttributeData.GotDoomerang = true;
	saveData->AttributeData.GotSmasharang = true;

	//Skips the welcome to rainbow cliffs cutscene. The game will increment this to 2 and the cutscene only plays when its 1
	saveData->LevelData[(int)LevelCode::Z1].TimesEntered = 1;

	return isTyDev;
}

void CustomTyDev::OverrideTyDev()
{
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x14eac0), &SetSaveFileData, reinterpret_cast<LPVOID*>(&Original_SetSaveFileData));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create Custom Ty Dev Function Hook, With the Error: " + error, Error);
		return;
	}
}
