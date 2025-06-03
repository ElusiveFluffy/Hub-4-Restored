#pragma once
#include "StaticProp.h"
#include "Animation.h"
#include "TightStructPacker.h"

TIGHT_PACK_BEGIN
struct AnimatingProp : StaticProp
{
    char _60[4];
    char _64[4];
    RGBA AnimColourMaybe;
    MKAnimScript AnimScript;
};
TIGHT_PACK_END
