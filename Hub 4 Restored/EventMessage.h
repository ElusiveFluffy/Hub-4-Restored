#pragma once
#include "MKMessage.h"

struct MKProp;
struct KromeIniLine;
struct EventMessage
{
    void Init();
    bool LoadLine(KromeIniLine* pLine, const char* pFieldName);
    void Resolve();
    void Send();
    union
    {
        MKProp* pTargetObj;
        int TargetId = -1;
    };
    MKMessage Message;
};
