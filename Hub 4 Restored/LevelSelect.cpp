#include "pch.h"
#pragma comment(lib,"Xinput9_1_0.lib")
#include "Xinput.h"
#include "WinUser.h"
#include "LevelSelect.h"
#include "NumUtil.h"

//TygerMemory
#include "sound.h"

float xPosStart = 50.0f;
float yPosStart = 165.0f;

float HorizontalSpacing = 270.0f;

void LevelSelect::SetupMultiColumn()
{
	UIButtonGroup* buttonGroup = GetUIButtonGroup();

	float xPos = xPosStart;
	float yPos = yPosStart; //+18 per row

	for (int i = 0; i < 24; i++)
	{

		if ((i % 8) == 0 && i != 0) {
			xPos += HorizontalSpacing;
			yPos = yPosStart;
		}
		else if ((i % 4) == 0 && i != 0)
			yPos += 30.0f;

		buttonGroup->Buttons[i].Text.X = xPos;
		buttonGroup->Buttons[i].Text.Y = yPos;

		yPos += 18.0f;
	}
}

//Makes it so it won't change option every frame
bool LeftOrRightStillDown;

void LevelSelect::HandleInput()
{
	if ((GetKeyState(VK_RIGHT) & 0x8000) || (GetKeyState('D') & 0x8000)) {
		MoveSelectionHorizontally(true);
		return;
	}
	if ((GetKeyState(VK_LEFT) & 0x8000) || (GetKeyState('A') & 0x8000)) {
		MoveSelectionHorizontally(false);
		return;
	}

	XINPUT_STATE controllerState;
	if (XInputGetState(0, &controllerState) == ERROR_SUCCESS)
	{
		//Joystick is about 75% of the way out
		if (controllerState.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_RIGHT || controllerState.Gamepad.sThumbLX > 24575)
		{
			MoveSelectionHorizontally(true);
			return;
		}
		if(controllerState.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_LEFT || controllerState.Gamepad.sThumbLX < -24575)
		{
			MoveSelectionHorizontally(false);
			return;
		}
	}

	LeftOrRightStillDown = false;
}

void LevelSelect::MoveSelectionHorizontally(bool pressedRight)
{
	if (LeftOrRightStillDown)
		return;

	UIButtonGroup* buttonGroup = GetUIButtonGroup();
	//Old one
	UIButton* currentSelectedButton = &buttonGroup->Buttons[buttonGroup->CurrentSelection];
	currentSelectedButton->Text.Colour = currentSelectedButton->Descriptor->DeselectedColour;
	//Smoothly change size
	currentSelectedButton->PulseSizeRadians = 0;
	//Deselect it (disables the pulsing)
	currentSelectedButton->Selected = false;

	//New one
	if (pressedRight)
		buttonGroup->CurrentSelection = NumUtil::Wrap(buttonGroup->CurrentSelection + 8, 24);
	else
		buttonGroup->CurrentSelection = NumUtil::Wrap(buttonGroup->CurrentSelection - 8, 24);

	currentSelectedButton = &buttonGroup->Buttons[buttonGroup->CurrentSelection];
	currentSelectedButton->Text.Colour = currentSelectedButton->Descriptor->SelectedColour;
	//Smoothly change size (do it for this one too, just in case, something the game does too)
	currentSelectedButton->PulseSizeRadians = 0;
	//Select it (enables the pulsing)
	currentSelectedButton->Selected = true;

	//Menu change option sound
	Sound::PlayTySoundByIndex(GlobalSound::FrontendClick);

	LeftOrRightStillDown = true;
}
