#pragma once

enum MKMessageType
{
    ResolveMSG = 0x1,
    UpdateAttachment = 0x3,
    AttachObject = 0x4,
    DetachObject = 0x5,
    BoomerangMsg = 0x8,
    ExplosionMsg = 0x9,
    Activate = 0xa,
    Deactivate = 0xb,
    Enable = 0xc,
    Disable = 0xd,
    Show = 0xe,
    Hide = 0xf,
    Break = 0x14
};

struct MKMessage {
    MKMessageType MsgID;
};