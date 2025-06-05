#pragma once
#include "Rangs.h"

enum MessageID
{
    MSG_Deinit = -2,
    MSG_Reset = -1,
    MSG_Resolve = 0x1,
    MSG_UpdateAttachment = 0x3,
    MSG_AttachObject = 0x4,
    MSG_DetachObject = 0x5,
    MSG_BoomerangMsg = 0x8,
    MSG_ExplosionMsg = 0x9,
    MSG_Activate = 0xa,
    MSG_Deactivate = 0xb,
    MSG_Enable = 0xc,
    MSG_Disable = 0xd,
    MSG_Show = 0xe,
    MSG_Hide = 0xf,
    MSG_Open = 0x10,
    MSG_Close = 0x11,
    MSG_Toggle = 0x12,
    MSG_Spawn = 0x13,
    MSG_Shatter = 0x14,
    MSG_Fall = 0x15,
    MSG_ObjectiveIncrement = 0x16,
    MSG_LearntToSwim = 0x17,
    MSG_LearntToDive = 0x18,
    MSG_GotBothRangs = 0x19,
    MSG_WeatherLightning = 0x1a,
    MSG_WeatherStormy = 0x1b,
    MSG_WeatherRain = 0x1c,
    MSG_WeatherSnow = 0x1d,
    MSG_WeatherPlankton = 0x1e,
    MSG_WeatherFoggy = 0x1f,
    MSG_Start = 0x20,
    MSG_GotAquarang = 0x21,
    MSG_GotFlamerang = 0x22,
    MSG_GotFrostyrang = 0x23,
    MSG_GotSmasharang = 0x24,
    MSG_GotZappyrang = 0x25,
    MSG_GotZoomerang = 0x26,
    MSG_GotMultirang = 0x27,
    MSG_GotInfrarang = 0x28,
    MSG_GotMegarang = 0x29,
    MSG_GotKaboomarang = 0x2a,
    MSG_GotChronorang = 0x2b,
    MSG_GotDoomarang = 0x2c,
    MSG_RequestElementalRang = 0x2d,
    MSG_RequestTechnoRang = 0x2e,
    MSG_RequestTalisman = 0x2f,
    MSG_Abort = 0x30,
    MSG_GotExtraHealth = 0x31,
    MSG_FallingOffGeoFluffy = 0x32,
    MSG_TimeAttackStart = 0x33,
    MSG_TimeAttackEnd = 0x34,
    MSG_EasterEggAchievement = 0x35
};

struct MKMessage {
    MessageID MsgID;
};

struct BoomerangMKMessage : MKMessage {
    Rangs::Boomerang* HitRang;
    bool unk;
};