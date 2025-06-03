#pragma once

//TygerMemory
#include "colour.h"

//May be wrong for the PC version
struct Material
{
    char Name[0x20];
    int CollisionFlags;
    int TextureFilterSize;
    int Type;
    int BlendMode;
    int Flags;
    int Grass;
    int Effect;
    RGBA Colour;
    char _4c[0xc];
    char _58[4];
    char _5c[4];
    char _60[4];
    char _64[4];
};
