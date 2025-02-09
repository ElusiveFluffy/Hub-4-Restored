#pragma once
#include "sound.h"
#include "colour.h"

namespace Rangs
{
	enum Boomerangs {
		Boomerang,
		Frostyrang,
		Flamerang,
		Kaboomerang,
		Doomerang,
		Megarang,
		Zoomerang,
		Infrarang,
		Zappyrang,
		Aquarang,
		Multirang,
		Chronorang,
		Smasharang
	};
	struct RangFunctionData {

	};
	struct RangPropertyFunctions {
		void* RangFunction;
		RangFunctionData* AllocatedMemory;
	};

	struct RangNameAndSoundData {
		const char* RangPropName;
		const char* UnusedThrowType;
		const char* UnusedCatchType;
		int unk1;
		int unk2;
		int unk3;
		int RangNameTextIndex;
		int unk4;
		GlobalSound ThrowSoundID;
		GlobalSound CatchSoundID;
		GlobalSound DeflectSoundID;
		GlobalSound HitSoundID;
		GlobalSound FlamerangThrowSoundID;
	};
	RangNameAndSoundData ExtraRangNameAndSounds[];

	struct RangActorData
	{
		void* SetupFunctions;
		char* RangModelName;
		float field_8;
		float field_c;
		BOOL field_10;
		BOOL field_14;
		char _18[0xc];
		char _24[8];
		char _2c[4];
		char AliasName[0x20];
		char ModelName[0x20];
		void* SetupFunction;
		char _74[0xc];
		char _80[4];
		char _84[4];
		char RangTrail[0x20];
		int AmmoCount;
		char _ac[4];
		char _b0[4];
		float RangRange;
		float RangFlightTime;
		int* SoundInfo;
		int* NameAndSoundData;
		float DoomerangPitchSpeed;
		float DoomerangTurnSpeed;
		float DoomerangSpeed;
	};

	struct RangGlowEffectParam
	{
		BOOL HasGlow;
		float PulseChangeRate;
		RGB TyGlowColour;
		RGB GroundGlowColour;
		float EntireLvlGlowIntensity;
		float GroundGlowIntensity;
		float GroundGlowFalloff;
		float GroundGlowRadius;
	};

	//Don't edit this one
	inline const int OriginalRangCount = 12;

	inline constexpr int RangCount = 13;
	inline Boomerangs CycleOrder[] = { Boomerang, Flamerang, Frostyrang, Smasharang, Zappyrang, Zoomerang, Multirang, Infrarang, Megarang, Kaboomerang, Chronorang, Doomerang, Aquarang };

	void SetupRangStructs();
	void HookRangFunctions();
};

