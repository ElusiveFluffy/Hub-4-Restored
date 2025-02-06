#include "pch.h"
#include "SaveFile.h"

//TygerMemory
#include "core.h"

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
}
