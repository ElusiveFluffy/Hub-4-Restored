#pragma once
#include "savedata.h"

#pragma pack(push, 1)
struct Hub4AttributeData {
	bool LearntToSwim;
	bool LearntToDive;
	bool GotSecondRang;
	bool GotExtraHealth;
	bool GotBoomerang;
	bool GotFrostyrang;
	bool GotFlamerang;
	bool GotKaboomerang;
	bool GotDoomerang;
	bool GotMegarang;
	bool GotZoomerang;
	bool GotInfrarang;
	bool GotZappyrang;
	bool GotAquarang;
	bool GotMultirang;
	bool GotChronorang;
	bool GotSmasharang;
	bool GotExtraTechnorang;
	bool GotExtraTechnorang2;
};

struct SaveFileHeader {
	int Size;
	int Magic;
	LevelCode SavedLevel;
	char PercentageCompletion;
	char ThunderEggCount;
	bool IsHardcore;
	bool IsDevMode;
};

struct Hub4SaveDataStruct {
	SaveFileHeader Header;
	LevelData LevelData[24];
	ZoneData ZoneData[6];
	short Unk2;
	LevelCode CurrentLevel;
	LevelCode PreviousLevel;
	Zone CurrentZone;
	Rang CurrentRang;
	Hub4AttributeData AttributeData;
	short Padding;
	bool Talismans[5];
	short Unk3;
	short Lives;
	char PictureFrames[0x41];
	char FmvsWatched;
	int PlayTimeSeconds;
	int EnemiesBitten;
	int EnemiesZapped;
	int EnemiesFlamed;
	int CricketBatsBurnt;
	bool CheatsUsed;
	bool CheatsUsed2;
	bool SpeedrunValid;
	bool Unk5;
	int SkinId;
};

struct SaveFileSelectStruct
{
	SaveFileHeader Header;
	char _10[0x30];
	char _40[0x40];
	char _80[0x25];
	bool Invalid;
	char _a6[2];
};
#pragma pack(pop)


enum class SaveGameState {
	AtleastOneValid = 2,
	NoneValid = 7
};

class SaveFile
{
public:
	static Hub4SaveDataStruct* GetHub4SaveData();
	static void SaveFileInit();

private:
	static void SetupFileVerification();
	static void FixFileVerification();
	static void SetSaveFileNames();
	static void ShiftSaveData();
};

