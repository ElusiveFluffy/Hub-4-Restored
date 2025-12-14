#pragma once
#include "EventMessage.h"

enum ConditionIDEnum
{
    CanSwim = 0,
    TwoRang = 1,
    CheckTEIndex3 = 2,
    DoneA = 3,
    DoneB = 4,
    DoneC = 5,
    DoneD = 6,
    DoneE = 7,
    FirstEntry = 8,
    HaveAll = 9,
    GotAqua = 10,
    BeenA = 11,
    BeenB = 12,
    BeenC = 13,
    BeenE = 14,
    GateE = 15,
    Always = 16,
    // vvv New Conditions vvv
    BeenD = 17
};

struct ConditionalScript
{
    char _0[0x40];
    char _40[0x14];
    EventMessage EventMessages[0xa];
    ConditionIDEnum ConditionID;
    bool ExecuteOnStart;
    bool Negative;
};