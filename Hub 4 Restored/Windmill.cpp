#include "pch.h"
#include "Windmill.h"
#include "MinHook.h"
#include "Model.h"
#include "GameObject.h"

//TygerMemory
#include "core.h"
#include "level.h"

typedef Model**(*Windmill_End_t)();
Windmill_End_t OriginalWindmill_End;

//The windmill is done differently from regular props, this is like load done
Model** Windmill_End() {
	Model** model = OriginalWindmill_End();

	if (Level::getCurrentLevel() == LevelCode::D1)
		(*model)->Colour = D1PropColour;

	return model;
}

void Windmill::HookFunctions()
{
	MH_CreateHook((LPVOID*)(Core::moduleBase + 0x109560), &Windmill_End, reinterpret_cast<LPVOID*>(&OriginalWindmill_End));
}
