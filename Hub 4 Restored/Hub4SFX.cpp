#include "pch.h"
#include "Hub4SFX.h"

//TygerMemory
#include "core.h"

void Hub4SFX::UpdateGlobalSoundCount()
{
	//Each element seems to be 0xC bytes long (original is 0x2244, 0x2244 / 0x2db = 0xC)
	int heapMemAllocSize = 0xC * GlobalSound::SFX_COUNT;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x152b34), &heapMemAllocSize, 4);
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x152b3e), &heapMemAllocSize, 4);
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x153b03), &heapMemAllocSize, 4);

	int sfxCount = GlobalSound::SFX_COUNT;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x152c2a), &sfxCount, 4);
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x152b8b), &sfxCount, 4);
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x152c0d), &sfxCount, 4);
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x152c2b), &sfxCount, 4);
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x11fbda), &sfxCount, 4);
	//This one needs to be negative
	int negativeSFXCount = sfxCount * -1;
	Core::SetReadOnlyValue((int*)(Core::moduleBase + 0x152c1a), &negativeSFXCount, 4);
}
