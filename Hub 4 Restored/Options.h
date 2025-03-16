#pragma once
namespace Options
{
	enum ThrowDirection {
		TyBody,
		Camera
	};

	void SetupExtraGamepadOption();
	inline ThrowDirection GamepadThrowDirection;
};