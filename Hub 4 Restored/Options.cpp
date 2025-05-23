#include "pch.h"
#include "Options.h"
#include "MinHook.h"
#include "UIStructs.h"
#include "TygerFrameworkAPI.hpp"
#include "ini.h"

#include <filesystem>
namespace fs = std::filesystem;

//TygerMemory
#include "core.h"
#include "sound.h"

typedef int(__fastcall* GamepadSettingsUI_t)(ControlOptionsUIButtons* options);
GamepadSettingsUI_t Original_InitGamePadSettings;
GamepadSettingsUI_t Original_DrawGamepadOptionsMenu;

typedef UIButton* (__thiscall* InitUIButton_t)(UIButtonGroup* buttonGroup, int entryCount);
InitUIButton_t InitUIButton;

//The descriptor can be null and it'll use the standard one
typedef void(__thiscall* SetUIButtonText_t)(UIButton* button, int translationTextID, UIButtonDescriptor* descriptor, void* unk1);
SetUIButtonText_t SetUIButtonText;

typedef void(__thiscall* SetUIPosAndFlags_t)(UIButton* button, Vector4f* pos, int flags);
SetUIPosAndFlags_t SetUIPosAndFlags;

typedef void(__thiscall* SetUITextFontSize_t)(UIButton* button, float fontSize);
SetUITextFontSize_t SetUITextFontSize;

typedef void(__thiscall* UISetSelection_t)(UIButtonGroup* buttons, int maybeFlag);
UISetSelection_t UISetSelection;

typedef void(__fastcall* UIButtonGroupFunc_t)(UIButtonGroup* buttons);
UIButtonGroupFunc_t DrawUIButtonGroup;
UIButtonGroupFunc_t UISelectNext;
UIButtonGroupFunc_t UIButtonsUpdate;

UIButtonGroup GamepadThrowDirButtons{};

//The specific bytes to set depending on the state of the option
BYTE ThrowTyBody[] = { 0x75, 0x0C };
BYTE ThrowCamera[] = { 0x90, 0x90 };

void HandleGamepadOptionSelection() {
	ControlOptionsUIButtons* options = (ControlOptionsUIButtons*)(Core::moduleBase + 0x2623D0);
	int currentSelection = options->GamepadOptions.CurrentSelection;

	switch (currentSelection) {
	//Vibration
	case 1:
		Sound::PlayTySoundByIndex(GlobalSound::FrontendClick);
		UISelectNext(&options->GamepadVibration);
		options->GamepadVibrationEnabled = options->GamepadVibration.CurrentSelection;
		break;
	//Throw direction
	case 2:
		Sound::PlayTySoundByIndex(GlobalSound::FrontendClick);
		UISelectNext(&GamepadThrowDirButtons);
		Options::GamepadThrowDirection = (Options::ThrowDirection)GamepadThrowDirButtons.CurrentSelection;

		BYTE* codeBytes;
		if (Options::GamepadThrowDirection == Options::TyBody)
			codeBytes = ThrowTyBody;
		else
			codeBytes = ThrowCamera;
		Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x16a6bc), codeBytes, 2);
		Options::SaveOptionsToIni();
		break;
	}

	//Make sure esi and eax is correct as this is run partway through a function
	__asm {
		lea esi, [options]
		mov eax, currentSelection
	}
}

int __fastcall InitGamepadSettingsUI(ControlOptionsUIButtons* options) {
	if (API::IsInitialized())
		API::LogPluginMessage("Setting up Custom Ingame Option");

	int buttonCount = 4;
	//Init it the menu to change first, since it got jumped over to make it easier to avoid a error
	InitUIButton(&options->GamepadOptions, buttonCount);

	//Set the back button as the bottom option (the new button needs to be initialized before running the original function)
	SetUIButtonText(&options->GamepadOptions.Buttons[buttonCount - 1], 0x55, nullptr, nullptr);
	Vector4f* gamepadOptionsStart = (Vector4f*)(Core::moduleBase + 0x202b70);
	Vector4f pos = *gamepadOptionsStart;
	pos.y = pos.y + (50 * (buttonCount - 1));
	SetUIPosAndFlags(&options->GamepadOptions.Buttons[buttonCount - 1], &pos, 5);

	//Run the original
	int returnVal = Original_InitGamePadSettings(options);

	//Edit the size of the vibration option to be a bit smaller
	float fontSize = 120;
	options->GamepadOptions.Buttons[1].Text.FontSize = fontSize;

	//Set the custom button
	SetUIButtonText(&options->GamepadOptions.Buttons[2], 0x37C, nullptr, nullptr);
	pos = *gamepadOptionsStart;
	pos.x = 300.0f;
	pos.y = pos.y + (50 * 2);
	SetUIPosAndFlags(&options->GamepadOptions.Buttons[2], &pos, 6);
	//Font size has to be bigger because of the text being made smaller by the game
	SetUITextFontSize(&options->GamepadOptions.Buttons[2], fontSize * 2.3f);

	//Add the Ty's body/camera text
	UIButtonDescriptor* whiteText = (UIButtonDescriptor*)(Core::moduleBase + 0x264170);
	InitUIButton(&GamepadThrowDirButtons, 2);
	pos.x = 388.0f;
	SetUIButtonText(GamepadThrowDirButtons.Buttons, 0x2df, whiteText, nullptr);
	SetUIPosAndFlags(GamepadThrowDirButtons.Buttons, &pos, 5);
	SetUITextFontSize(GamepadThrowDirButtons.Buttons, 108.800003f);
	GamepadThrowDirButtons.Buttons->Scale = 0.800000012f;

	pos.x += 136.0f;
	SetUIButtonText(&GamepadThrowDirButtons.Buttons[1], 0x2e0, whiteText, nullptr);
	SetUIPosAndFlags(&GamepadThrowDirButtons.Buttons[1], &pos, 5);
	SetUITextFontSize(&GamepadThrowDirButtons.Buttons[1], 108.800003f);
	GamepadThrowDirButtons.Buttons[1].Scale = 0.800000012f;

	UISetSelection(&GamepadThrowDirButtons, 0);

	return returnVal;
}

int __fastcall DrawGamepadOptionsMenu(ControlOptionsUIButtons* options) {
	//Not usually where the game does this, but this is easier
	UIButtonsUpdate(&GamepadThrowDirButtons);

	//Draw the buttons
	DrawUIButtonGroup(&GamepadThrowDirButtons);
	return Original_DrawGamepadOptionsMenu(options);
}

void HookGamepadSettings() {
	MH_STATUS minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0xed140), &InitGamepadSettingsUI, reinterpret_cast<LPVOID*>(&Original_InitGamePadSettings));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		return;
	}
	minHookStatus = MH_CreateHook((LPVOID*)(Core::moduleBase + 0xeaa00), &DrawGamepadOptionsMenu, reinterpret_cast<LPVOID*>(&Original_DrawGamepadOptionsMenu));
	if (minHookStatus != MH_OK) {
		std::string error = MH_StatusToString(minHookStatus);
		return;
	}
}

void EditHandleGamepadOptionsFunc() {
	//Edit the handle gamepad options function to be able to work with my extra button (added a nop on the end to not break the code for the last 2 function call replacements)
	BYTE functionCall[] = { 0xE8, 0, 0, 0, 0, 0x90 };
	//Function calls are relative from the end of the opcode (why there is a -5)
	int functionOffset = (int)&HandleGamepadOptionSelection - (int)(Core::moduleBase + 0xea90f) - 5;
	memcpy(&functionCall[1], &functionOffset, 4);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xea90f), &functionCall, 5);

	//2 below for pressing left/right
	functionOffset = (int)&HandleGamepadOptionSelection - (int)(Core::moduleBase + 0xea989) - 5;
	memcpy(&functionCall[1], &functionOffset, 4);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xea989), &functionCall, 6);

	functionOffset = (int)&HandleGamepadOptionSelection - (int)(Core::moduleBase + 0xea94c) - 5;
	memcpy(&functionCall[1], &functionOffset, 4);
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xea94c), &functionCall, 6);

	//Change both from a jne to a jmp to skip running the original logic for the vibration setting button
	BYTE jumpOpcode = 0xEB;
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xea992), &jumpOpcode, 1);
	BYTE jumpOpcode2[] = { 0xE9, 0x83, 0x00, 0x00, 0x00 };
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xea955), &jumpOpcode2, 5);


	//Change the back option to check button index 3 instead of 2
	BYTE cmpCheck = 3;
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xea916), &cmpCheck, 1);
}

void Options::SetupExtraGamepadOption()
{
	//Jump past the mem allocation function, in the gamepad options init function, as its easier for me to allocate it myself before the function runs
	BYTE jumpOpcode[] = { 0xEB, 0x0B };
	Core::SetReadOnlyValue((void*)(Core::moduleBase + 0xed161), &jumpOpcode, 2);

	EditHandleGamepadOptionsFunc();

	//Set functions from the game
	InitUIButton = (InitUIButton_t)(Core::moduleBase + 0xf55c0);
	SetUIButtonText = (SetUIButtonText_t)(Core::moduleBase + 0xf54c0);
	SetUIPosAndFlags = (SetUIPosAndFlags_t)(Core::moduleBase + 0xf5f10);
	SetUITextFontSize = (SetUITextFontSize_t)(Core::moduleBase + 0xf6220);
	UISetSelection = (UISetSelection_t)(Core::moduleBase + 0xf58e0);
	DrawUIButtonGroup = (UIButtonGroupFunc_t)(Core::moduleBase + 0xf53d0);
	UISelectNext = (UIButtonGroupFunc_t)(Core::moduleBase + 0xf59d0);
	UIButtonsUpdate = (UIButtonGroupFunc_t)(Core::moduleBase + 0xf6650);

	HookGamepadSettings();
}

std::string iniPath = "Plugins\\Hub 4 Restored Settings.ini";
std::string GamepadSection = "Gamepad";

void Options::LoadOptionsFromIni()
{
	if (!fs::exists(iniPath))
		return;

	API::LogPluginMessage("Loading Settings From ini");

	ini::File settings = ini::open(iniPath);
	if (settings.has_section(GamepadSection)) {
		ini::Section gamepad = settings[GamepadSection];

		if (gamepad.has_key("ThrowDirection")) {
			GamepadThrowDirection = (ThrowDirection)gamepad.get<int>("ThrowDirection");

			BYTE* codeBytes;
			if (GamepadThrowDirection == TyBody)
				codeBytes = ThrowTyBody;
			else
				codeBytes = ThrowCamera;
			Core::SetReadOnlyValue((void*)(Core::moduleBase + 0x16a6bc), codeBytes, 2);

			UISetSelection(&GamepadThrowDirButtons, GamepadThrowDirection);
		}
	}
}

void Options::SaveOptionsToIni()
{
	ini::File settings;

	//Create Gamepad section
	settings.add_section(GamepadSection);
	settings[GamepadSection].set<int>("ThrowDirection", GamepadThrowDirection);

	//Save the settings
	settings.write(iniPath);
}
