#pragma once
#include "sound.h"
#include "colour.h"

typedef int BOOL;
namespace Rangs
{
	enum Boomerangs {
		Ironbark,
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

	struct BoomerangStaticInfo {
		const char* RangPropName;
		const char* UnusedThrowType;
		const char* UnusedCatchType;
		int unk1;
		int unk2;
		int unk3;
		int RangNameTextIndex;
		int RangDescTextIndex;
		GlobalSound ThrowSoundID;
		GlobalSound CatchSoundID;
		GlobalSound DeflectSoundID;
		GlobalSound HitSoundID;
		GlobalSound FlamerangThrowSoundID;
	};
	BoomerangStaticInfo ExtraBoomerangInfo[];

	struct BoomerangDescriptor
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
		BoomerangStaticInfo* BoomerangInfo;
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

#pragma pack(push, 1)
	struct Boomerang
	{
		void* VTable;
		BoomerangDescriptor* RangInfo;
		char _8[0x14];
		char _1c[0x24];
		char _40[4];
		Vector4f field_44;
		char _54[4];
		Boomerangs RangIndex;
		bool HitSomething;
		bool RenderParticles;
		char _5e[2];
		Vector4f RangPos;
		void* RangActor;
		char _74[4];
		char _78[4];
		char _7c[4];
		char _80[8];
		float PercentOfSplineTravelled;
		float SplineLength;
		char field_90;
		char _91[3];
		char _94[4];
		char _98[4];
		char _9c[4];
		float field_a0;
		float field_a4;
		char _a8[4];
	};
#pragma pack(pop)

	//Don't edit this one
	inline const int OriginalRangCount = 12;

	inline constexpr int RangCount = 13;
	inline Boomerangs CycleOrder[] = { Ironbark, Flamerang, Frostyrang, Smasharang, Zappyrang, Zoomerang, Multirang, Infrarang, Megarang, Kaboomerang, Chronorang, Doomerang, Aquarang };

	void SetupRangStructs();
	void HookRangFunctions();
	Boomerangs GetCurrentRang();
	inline bool IsCurrentRang(Boomerangs rangToCheck) {
		return GetCurrentRang() == rangToCheck;
	}
	void RangSpecificSetup();
};

