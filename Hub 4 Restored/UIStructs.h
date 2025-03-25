#pragma once
//TygerMemory
#include "vector4f.h"

// 0x80/128 is basically 1 in a 0-1 colour scale
struct ByteRGBA
{
    BYTE R;
    BYTE G;
    BYTE B;
    BYTE A;
};

struct UIText
{
    float X;
    float Y;
    float FontSize;
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
    char _140[0xc];
    char _14c[0x10];
    float Scale;
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

struct UIHeading
{
    void* DrawHeaderFuncPtr;
    struct UIText Header;
};

// A little messy but thats because I copied it from a struct made in binary ninja 
// and it detected some of the variables 
struct ControlOptionsUIButtons
{
    int CurrentMenu;
    int NextMenu;
    char _8[4];
    char _c[4];
    Vector4f RangModelPosition;
    char _20[0x20];
    char _40[0x18];
    UIButtonGroup ControlsUIButtons;
    UIButtonGroup RangSelectionWheelOption;
    UIHeading ControlsHeader;
    char _b4[4];
    UIHeading GamepadHeader;
    UIHeading GamepadConfigHeader;
    UIButtonGroup GamepadOptions;
    UIButtonGroup GamepadVibration;
    UIButtonGroup GamepadTypeLetters;
    UIHeading GamepadTypeHeader;
    char _190[4];
    UIHeading GamepadButtonText;
    bool GamepadVibrationEnabled;
    char _1c9[3];
    int GamepadTypeConfig;
    char _1d0[0x30];
    char _200[4];
    char _204[0x34];
    UIButtonGroup KeyboardMouseUIButtons;
    UIButtonGroup KeybindUIButtons;
    // The text that shows what the current keybind is is a button too,
    // it's selection copys the selection of KeybindUIButtons
    UIButtonGroup KeybindBoundKeyUI;
    char _274[0xc];
    char _280[0x40];
    char _2c0[0x40];
    char _300[0x40];
    char _340[0x40];
    char _380[0x40];
    char _3c0[0x40];
    char _400[0x40];
    char _440[0x40];
    char _480[0x40];
    char _4c0[0x30];
    char _4f0[0x10];
    char _500[0x40];
    char _540[0x40];
    char _580[0x40];
    char _5c0[0x30];
    char _5f0[0x10];
    char _600[0x40];
    char _640[0x40];
    char _680[0x40];
    char _6c0[0x30];
    char _6f0[0x10];
    char _700[0x40];
    char _740[0x40];
    char _780[0x40];
    char _7c0[0x30];
    char _7f0[0x10];
    char _800[0x40];
    char _840[0x40];
    char _880[0x40];
    char _8c0[0x30];
    char _8f0[0x10];
    char _900[0x40];
    char _940[0x40];
    char _980[0x40];
    char _9c0[0x30];
    char _9f0[0x10];
    char _a00[0x40];
    char _a40[0x40];
    char _a80[0x40];
    char _ac0[0x30];
    char _af0[0x10];
    char _b00[0x40];
    char _b40[0x40];
    char _b80[0x40];
    char _bc0[0x30];
    char _bf0[0x10];
    char _c00[0x40];
    char _c40[0x40];
    char _c80[0x40];
    char _cc0[0x30];
    char _cf0[0x10];
    char _d00[0x40];
    char _d40[0x40];
    char _d80[0x40];
    char _dc0[0x30];
    char _df0[0x10];
    char _e00[0x40];
    char _e40[0x40];
    char _e80[0x40];
    char _ec0[0x30];
    char _ef0[0x10];
    char _f00[0x40];
    char _f40[0x40];
    char _f80[0x40];
    char _fc0[0x30];
    char _ff0[0x10];
    char _1000[0x40];
    char _1040[0x40];
    char _1080[0x40];
    char _10c0[0x30];
    char _10f0[0x10];
    char _1100[0x40];
    char _1140[0x40];
    char _1180[0x40];
    char _11c0[0x30];
    char _11f0[0x10];
    char _1200[0x40];
    char _1240[0x40];
    char _1280[0x40];
    char _12c0[0x40];
    char _1300[0x2c];
    UIHeading CameraHeader;
    UIButtonGroup CameraSettings;
    UIButtonGroup TyViewDirection;
    UIButtonGroup TyViewLR;
    UIButtonGroup TyViewUD;
    char _13b0[0x10];
    char _13c0[0x40];
    char _1400[0x40];
    char _1440[0x40];
    char _1480[0x40];
    char _14c0[0x40];
    char _1500[0x40];
    char _1540[0x40];
    char _1580[0x40];
    char _15c0[0x40];
    char _1600[0x2c];
    UIButtonGroup CameraRotationLR;
    UIButtonGroup CameraRotationUD;
    char _1654[0x2c];
    char _1680[0x40];
    char _16c0[0x40];
    char _1700[0x40];
    char _1740[0x40];
    char _1780[0x40];
    char _17c0[0x40];
    char _1800[0x40];
    char _1840[0x40];
    char _1880[0x40];
    char _18c0[0x10];
    UIButtonGroup TrackBodyOnJump;
    UIButtonGroup AutoCenterCamera;
    char _18f8[1];
    bool TyViewLRFlipped;
    bool TyViewUDFlipped;
    char _18fb[1];
    bool CameraLRFlipped;
    bool CameraUDFlipped;
    char _18fe[1];
    bool TrackBodyEnabled;
    bool AutoCenterCameraEnabled;
    char _1901[3];
    BOOL TyViewOptionState;
};