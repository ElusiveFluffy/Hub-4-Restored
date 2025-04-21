#include "pch.h"
#include "StaticProp.h"

GameObjectLoadLine_t TyStaticPropLoadLine;
GameObjectVoid_t TyStaticPropLoadDone;
GameObjectVoid_t TyStaticPropDraw;
GameObjectInit_t TyStaticPropInit;

void StaticProps::Init()
{
	//Static prop vtable functions
	TyStaticPropLoadLine = (GameObjectLoadLine_t)(Core::moduleBase + 0x1426f0);
	TyStaticPropLoadDone = (GameObjectVoid_t)(Core::moduleBase + 0x142790);
	TyStaticPropDraw = (GameObjectVoid_t)(Core::moduleBase + 0x1429f0);
	TyStaticPropInit = (GameObjectInit_t)(Core::moduleBase + 0x142670);

	//Game object desc vtable functions
	TyStaticPropDescInit = (GameObjDescInit_t)(Core::moduleBase + 0x143350);
	TyStaticPropDescLoad = (GameObjDescLoad_t)(Core::moduleBase + 0x1433a0);
}

bool StaticProp::LoadLine(KromeIniLine* pLine)
{
	return TyStaticPropLoadLine(this, pLine);
}
void StaticProp::LoadDone()
{
	TyStaticPropLoadDone(this);
}
void StaticProp::Draw()
{
	TyStaticPropDraw(this);
}
void StaticProp::Init(GameObjDesc* pDesc)
{
	TyStaticPropInit(this, pDesc);
}
