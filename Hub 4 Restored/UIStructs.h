#pragma once

struct ByteRGBA
{
    char R;
    char G;
    char B;
    char A;
};

struct UIText
{
    float X;
    float Y;
    float Unk1;
    float FontHeight;
    float Roll;
    float ScaleX;
    float ScaleY;
    int Flags;
    ByteRGBA Colour;
    void* Font;
    char* Text;
    int StringIdx;
};

struct UIButtonDescriptor
{
    bool ShouldPulse;
    char padding[0x3];
    ByteRGBA DeselectedColour;
    ByteRGBA SelectedColour;
    ByteRGBA DisabledColour;
};

struct UIButton
{
    UIButtonDescriptor* Descriptor;
    void* Unk1;
    UIText Text;
    char _38[8];
    char _40[0x40];
    char _80[0x40];
    char _c0[0x40];
    char _100[0x40];
    char _140[0x20];
    float PulseSizeRadians;
    bool Enabled;
    bool Selected;
    bool Update;
    char _167[1];
};

struct UIButtonGroup
{
    int EntryCount;
    int CurrentSelection;
    UIButton* Buttons;
    int Unk1;
    void* SelectionRangModel;
};