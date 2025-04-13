#include "pch.h"
#include "GameObject.h"
#include "Smashrock.h"
#include "SmashCrate.h"

void GameObject::EarlyInit()
{
	SmashCrate::EarlyInit();
}

void GameObject::APIInit()
{
	Smashrock::SmashrockInit();
	SmashCrate::HookFunctions();
}
