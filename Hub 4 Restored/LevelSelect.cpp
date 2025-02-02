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
	//0x168 bytes per one
	int UIButtonPosPtr = GetButtonInfoStart() + 8;

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

		*(float*)UIButtonPosPtr = xPos;
		*(float*)(UIButtonPosPtr + 4) = yPos;

		yPos += 18.0f;

		UIButtonPosPtr += 0x168;
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

	int UIButtonsPtr = GetButtonInfoStart();
	//Old one
	int currentSelectedPtr = UIButtonsPtr + (0x168 * *GetSelectedLevelPtr());
	*(int*)(currentSelectedPtr + 40) = GetTextDeselectedColour();
	//Smoothly change size
	*((int*)(currentSelectedPtr + 0x160)) = 0;
	//Deselect it (disables the pulsing)
	*(bool*)(currentSelectedPtr + 357) = false;

	//New one
	int newLevelIndex = 0;
	if (pressedRight)
		newLevelIndex = NumUtil::Wrap(*GetSelectedLevelPtr() + 8, 24);
	else
		newLevelIndex = NumUtil::Wrap(*GetSelectedLevelPtr() - 8, 24);

	*GetSelectedLevelPtr() = newLevelIndex;
	currentSelectedPtr = UIButtonsPtr + (0x168 * newLevelIndex);
	*(int*)(currentSelectedPtr + 40) = GetTextSelectedColour();
	//Smoothly change size (do it for this one too, just in case, something the game does too)
	*((int*)(currentSelectedPtr + 0x160)) = 0;
	//Select it (enables the pulsing)
	*(bool*)(currentSelectedPtr + 357) = true;

	//Menu change option sound
	Sound::PlayTySoundByIndex(FrontendClick);

	LeftOrRightStillDown = true;
}
