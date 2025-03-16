#pragma once
namespace Options
{
	enum ThrowDirection {
		TyBody,
		Camera
	};

	void SetupExtraGamepadOption();
	void LoadOptionsFromIni();
	void SaveOptionsToIni();
	inline ThrowDirection GamepadThrowDirection;
};