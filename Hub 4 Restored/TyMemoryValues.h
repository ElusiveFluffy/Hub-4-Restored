#pragma once
#include "core.h"
#include "level.h"

namespace TyMemoryValues {

	void inline WriteHardcodedInt(UINT* address, int value) {
		DWORD oldProtection;
		//Change the memory access to ReadWrite to be able to change the hardcoded value (usually its read only)
		VirtualProtect(address, 4, PAGE_EXECUTE_READWRITE, &oldProtection);

		*address = value;

		//Set it back to the old access protection
		VirtualProtect(address, 4, oldProtection, &oldProtection);
	}
	void inline WriteHardcodedByte(BYTE* address, BYTE value) {
		DWORD oldProtection;
		//Change the memory access to ReadWrite to be able to change the hardcoded value (usually its read only)
		VirtualProtect(address, 1, PAGE_EXECUTE_READWRITE, &oldProtection);

		*address = value;

		//Set it back to the old access protection
		VirtualProtect(address, 1, oldProtection, &oldProtection);
	}

	//To disable the game info option in the credits
	void inline DisableGameInfoID(LevelCode levelID) { 
		WriteHardcodedByte((BYTE*)(Core::moduleBase + 0xE42E2), (BYTE)levelID);
	};
}