#include "SlyFight.h"

// TygerMemory
#include "core.h"

void SlyFight::DisableSlyShadowChange()
{
	//Easily place a certain amount of nops based on the amount of bytes set
	BYTE nops[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

	// Entering the fight
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xc81d1), &nops, 5);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xc81df), &nops, 8);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xc81f1), &nops, 5);

	// Exiting the fight
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xc8221), &nops, 5);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xc822f), &nops, 8);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xc8241), &nops, 5);
}
