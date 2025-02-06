#pragma once
#include "savedata.h"

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

struct Hub4SaveDataStruct {
	int Size;
	int Magic;
	LevelCode SavedLevel;
	char PercentageCompletion;
	char ThunderEggCount;
	bool IsHardcore;
	bool IsDevMode;
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

namespace SaveFile
{
	Hub4SaveDataStruct* GetHub4SaveData();
	void SetSaveFileNames();
	void ShiftSaveData();
	void SaveFileInit();
};

