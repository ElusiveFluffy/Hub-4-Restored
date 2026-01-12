#include "pch.h"
#include "SaveFile.h"
#include "TygerFrameworkAPI.hpp"

//TygerMemory
#include "core.h"

#include <MinHook.h>

char save1[] = "Hub 4 Game 1";
char save2[] = "Hub 4 Game 2";
char save3[] = "Hub 4 Game 3";

Hub4SaveDataStruct* SaveFile::GetHub4SaveData()
{
	return (Hub4SaveDataStruct*)(*(int*)(Core::moduleBase + 0x288730));
}

void SaveFile::SetSaveFileNames()
{
	int* saveFilePtrStart = (int*)(Core::moduleBase + 0x277274);

	saveFilePtrStart[0] = (int)&save1;
	saveFilePtrStart[1] = (int)&save2;
	saveFilePtrStart[2] = (int)&save3;
}

using InitSaveFileUI_t = int(*)();
InitSaveFileUI_t Original_InitSaveFileUI;

// Fix bug where the function only checks if the first save game is valid
static int InitSaveFileUI() {
	bool LoadGameSelected = *(bool*)(Core::moduleBase + 0x28dcaa);
	SaveGameState* state = (SaveGameState*)(Core::moduleBase + 0x28dca0);
	// Just to make sure that its load game calling it, since this gets called in a couple places.
	// And only my edited/fixed file verification sets the state to none valid before calling the function
	if (LoadGameSelected && *state == SaveGameState::NoneValid)
	{
		SaveFileSelectStruct* saveSelectStructs = (SaveFileSelectStruct*)(Core::moduleBase + 0x28dcd8);
		// Check if atleast one is valid
		for (int i = 0; i < 3; i++)
		{
			if (!saveSelectStructs[i].Invalid)
			{
				*state = SaveGameState::AtleastOneValid;
				break;
			}
		}
	}

	return Original_InitSaveFileUI();
}

void SaveFile::SetupFileVerification()
{
	FixFileVerification();

	// Now change the magic number so only hub 4 restored save files can be loaded with my mod
	int hub4SaveMagic = 0x801ee;
	// Verify function one
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xf6fd1), &hub4SaveMagic, 4);

	// Create save file
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xf6f27), &hub4SaveMagic, 4);

	// Init save data?
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xf6fab), &hub4SaveMagic, 4);

	// Save game data
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xf7453), &hub4SaveMagic, 4);
}

void SaveFile::FixFileVerification()
{
	// Change jne to a je to flip the if for the early exit
	BYTE jeOpCode = 0x74;
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x1707df), &jeOpCode, 1);

	// Move the invalid variable set down to where the state for atleast one valid would be set
	BYTE nops[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	BYTE setEsiToOne[] = { 0xc6, 0x06, 0x01 };
	// Remove the original esi set
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x1707e1), &nops, 3);
	// Clear the last unneeded bytes from the move dword op code
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x17082c), &nops, 8);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x17082a), &setEsiToOne, 3);

	// Change the jump to not jump out of the while loop early
	BYTE jumpDistance = 0xAE;
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x170835), &jumpDistance, 1);

	// Need to hook the function to add aditional logic to finish of the edited validity check
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0x16fc60), &InitSaveFileUI, reinterpret_cast<LPVOID*>(&Original_InitSaveFileUI));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		API::LogPluginMessage("Failed to Create the init save file UI function hook, With the Error: " + error, Error);
		return;
	}
}

void SaveFile::ShiftSaveData()
{
	//Shift it over the unused talisman placed bools
	int offsetAmount = 5;
	//Talismans
	int* codeAddress = (int*)(Core::moduleBase + 0xd8953);
	int newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);

	codeAddress = (int*)(Core::moduleBase + 0xd89b5);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);

	codeAddress = (int*)(Core::moduleBase + 0xd8a17);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);


	codeAddress = (int*)(Core::moduleBase + 0xdc1f4);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);

	codeAddress = (int*)(Core::moduleBase + 0xdc20e);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);

	codeAddress = (int*)(Core::moduleBase + 0xdc228);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);

	codeAddress = (int*)(Core::moduleBase + 0xdc242);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);

	//Game totals counted ones
	codeAddress = (int*)(Core::moduleBase + 0xf7837);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);

	codeAddress = (int*)(Core::moduleBase + 0xf7840);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);

	codeAddress = (int*)(Core::moduleBase + 0xf784d);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);

	codeAddress = (int*)(Core::moduleBase + 0xf785a);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);

	codeAddress = (int*)(Core::moduleBase + 0xf7867);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);


	codeAddress = (int*)(Core::moduleBase + 0xf7cc9);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);


	codeAddress = (int*)(Core::moduleBase + 0x10c308);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);

	codeAddress = (int*)(Core::moduleBase + 0x10c38d);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);

	codeAddress = (int*)(Core::moduleBase + 0x10c413);
	newOffset = *codeAddress + offsetAmount;
	Core::SetReadOnlyValue(codeAddress, &newOffset, 4);
}

void SaveFile::SaveFileInit()
{
	SetSaveFileNames();
	ShiftSaveData();
	SetupFileVerification();
}
