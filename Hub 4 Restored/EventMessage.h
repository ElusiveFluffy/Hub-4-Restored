#pragma once
#include "MKMessage.h"
struct EventMessage
{
    union
    {
        struct MKProp* pTargetObj;
        int TargetId;
    };
    MKMessage Message;
};
