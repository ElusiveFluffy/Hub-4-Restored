#pragma once
#include "Model.h"
#include "MKMessage.h"
#include "KromeIni.h"
#include "MKMemory.h"
#include "TyFunctions.h"

struct MKProp;
struct MKPropDescriptor
{
    char* AliasName;
    float MaxUpdateDist;
    float MaxDrawDist;
    UINT SearchMask;
    int DrawLayer;
    UINT Flags;
    BoundingVolume* pVolume;
    int unk;
    float MaxScissorDist;
    MKPropDescriptor* pNext;
    MKProp* pProps;
};

struct GameObjDesc;
struct MKProp
{
    MKPropDescriptor* pDescriptor;
    Model* pModel;
    Matrix* pLocalToWorld;
    int Flags;
    int UniqueID;
    char _18[0xc];
    char _24[4];
    char _28[4];
    float DistSquared;
    MKProp* pNext;
    MKProp* pPrev;
    MKProp* pPrevUpdated;
    MKProp* pNextUpdated;
    //Seems to be unused most of the time
    MKProp* pNextOfThisType;
};

struct GameObject;
typedef bool(__thiscall* GameObjectLoadLine_t)(GameObject* prop, KromeIniLine* pLine);
typedef void(__thiscall* GameObjectVoid_t)(GameObject* prop);
typedef void(__thiscall* GameObjectMsg_t)(GameObject* prop, MKMessage* msg);
typedef void(__thiscall* GameObjectInit_t)(GameObject* prop, GameObjDesc* pDesc);
inline GameObjectLoadLine_t DefaultLoadLine;
inline GameObjectVoid_t DefaultDraw;
inline GameObjectMsg_t DefaultMessage;
inline GameObjectInit_t DefaultInit;
inline GameObjectVoid_t DefaultDeinit;
struct GameObject : MKProp
{
    virtual bool LoadLine(KromeIniLine* pLine) {
        return DefaultLoadLine(this, pLine);
    }
    virtual void LoadDone() {
        return;
    }
    virtual void Reset() {
        return;
    }
    virtual void Update() {
        return;
    }
    virtual void Draw() {
        DefaultDraw(this);
    }
    virtual void Message(MKMessage* pMsg) {
        DefaultMessage(this, pMsg);
    }
    virtual void Init(GameObjDesc* pDesc) {
        DefaultInit(this, pDesc);
    }
    virtual void Deinit() {
        DefaultDeinit(this);
    }
};

enum ModuleOverrideFuncFlags
{
    NoOverride = 0,
    Module_UpdateOverride = 1 << 0,
    Module_DrawOverride = 1 << 1,
    Module_AllocateOverride = 1 << 3
};
DEFINE_ENUM_FLAG_OPERATORS(ModuleOverrideFuncFlags)
struct ModuleInfoVTable
{
    void(__thiscall* Init)();
    void(*ConstructObject)(MKProp* ptr);
};

struct ModuleInfoBase;
struct ModuleInfoBaseObject
{
    void (*InitModule)();
    void (*DeinitModule)();
    void (*pUpdateModule)();
    void (*pDrawModule)();
    int* (*pAllocate)();
    void (*pDeallocate)(void* gameObject);
    int entityCount;
    int InstanceSize;
    bool Update;
    char _21[3];
    ModuleOverrideFuncFlags Flags;
    ModuleInfoBase* pNext;
};

struct ModuleInfoBase {
    virtual void Init() = 0;
    virtual void* ConstructObject(void*) = 0;

    void Reset();

    ModuleInfoBaseObject* pData;
};
typedef void(*AddToModuleList_t)(ModuleInfoBase* pModule);
inline AddToModuleList_t AddToModuleList;

template <typename T>
struct ModuleInfo : ModuleInfoBase
{
    //Same functions from the game
    virtual void Init() {
        if (pData == 0)
        	pData = (ModuleInfoBaseObject*)MKMemory::Heap_MemAlloc(0x2c);
        
        Reset();
        
        pData->Flags = NoOverride;
        //Most get assigned to this
        auto blankFunction = (TyFunctions::VoidFunction_t)(Core::moduleBase + 0x1a300);
        pData->InitModule = blankFunction;
        pData->DeinitModule = blankFunction;
        pData->pUpdateModule = blankFunction;
        pData->pDrawModule = blankFunction;
        pData->pAllocate = (int* (*)())(Core::moduleBase + 0x70f70);
        pData->pDeallocate = (void(*)(void*))(Core::moduleBase + 0x1a300);
        
        pData->InstanceSize = sizeof(T);

        if (pData->pUpdateModule != blankFunction) {
            pData->Flags |= Module_UpdateOverride;
        }

        if (pData->pDrawModule != blankFunction) {
            pData->Flags |= Module_DrawOverride;
        }

        if (pData->pAllocate != (int* (*)())(Core::moduleBase + 0x70f70)) {
            pData->Flags |= Module_AllocateOverride;
        }
        
        AddToModuleList(this);
    }
    virtual void* ConstructObject(void* ptr) {
        return new(ptr) T;
    }
};

typedef void(__thiscall* GameObjDescInit_t)(GameObjDesc* gameObjDesc, ModuleInfoBase* moduleInfo, const char* mdlName, const char* aliasName, int searchMask, int flags);
typedef void(__thiscall* GameObjDescLoad_t)(GameObjDesc* gameObjDesc, KromeIni* pIni);
inline GameObjDescInit_t DefaultGameObjDescInit;
inline GameObjDescLoad_t DefaultGameObjDescLoad;
struct GameObjDesc
{
    virtual void Init(ModuleInfoBase* moduleInfo, const char* mdlName, const char* aliasName, int searchMask, int flags) {
        DefaultGameObjDescInit(this, moduleInfo, mdlName, aliasName, searchMask, flags);
    }
    virtual void Load(KromeIni* pIni) {
        DefaultGameObjDescLoad(this, pIni);
    }
    virtual void* ConstructObject(void* mem) {
        return pModule->ConstructObject(mem);
    }
    MKPropDescriptor MkPropDesc;
    char AliasName[0x20];
    char ModelName[0x20];
    ModuleInfoBase* pModule;
    int InstanceCount;
    GameObject* pInstances;
    char _7c[4];
    char _80[4];
    GameObjDesc* pPrevious;
};

 namespace GameObj
 {
    inline GameObjDesc** PreviousGameObj;

    void EarlyInit();
    void APIInit();
};

typedef void(__thiscall* GetMKPropRange_t)(GameObjDesc* gameObjDesc, GameObject* pOutProp[2]);
typedef void(__thiscall* SetPreviousGameObject_t)(GameObjDesc** previousGameObjectDesc, GameObjDesc* gameObjectDesc);
inline SetPreviousGameObject_t SetPreviousGameObject;