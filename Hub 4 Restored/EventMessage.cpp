#include "EventMessage.h"

//TygerMemory
#include "core.h"

using EventMessageLoadLine_t = bool(__thiscall*)(EventMessage* pEventMessage, KromeIniLine* pLine, const char* fieldName);
using EventMessageThisCall_t = void(__thiscall*)(EventMessage* pEventMessage);

EventMessageLoadLine_t EventMessageLoadLine = nullptr;
EventMessageThisCall_t EventMessageInit = nullptr;
EventMessageThisCall_t EventMessageResolve = nullptr;
EventMessageThisCall_t EventMessageSend = nullptr;

void EventMessage::Init()
{
    if (!EventMessageInit)
        EventMessageInit = (EventMessageThisCall_t)(Core::moduleBase + 0x10d410);
    EventMessageInit(this);
}

bool EventMessage::LoadLine(KromeIniLine* pLine, const char* pFieldName)
{
    if (!EventMessageLoadLine)
        EventMessageLoadLine = (EventMessageLoadLine_t)(Core::moduleBase + 0x10d420);
    return EventMessageLoadLine(this, pLine, pFieldName);
}

void EventMessage::Resolve()
{
    if (!EventMessageResolve)
        EventMessageResolve = (EventMessageThisCall_t)(Core::moduleBase + 0x10d4b0);
    EventMessageResolve(this);
}

void EventMessage::Send()
{
    if (!EventMessageSend)
        EventMessageSend = (EventMessageThisCall_t)(Core::moduleBase + 0x10d4e0);
    EventMessageSend(this);
}
