#pragma once
//#include "EnemyObject.h"

struct EnemyObject;

struct ActionStateFunctions
{
    void(__thiscall* ActionInit)(EnemyObject* enemyObj);
    void(__thiscall* ActionDeinit)(EnemyObject* enemyObj);
    void(__thiscall* ActionUpdate)(EnemyObject* enemyObj);
    char _c[0xc];
};

struct ActionStateManager
{
    int CurrentState;
    int NextState;
    int PreviousState;
    ActionStateFunctions* ActionFunctions;
};
