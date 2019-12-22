#ifndef GUARD_TYPES_H
#define GUARD_TYPES_H

// Forward declarations for common types
struct Proc;
struct Unit;

// Forward declarations for types that may need to be declared elsewhere later?
struct BattleUnit; // currently in bmunit.h
struct UnitDefinition; // currently in bmunit.h

// Type definitions for types without any other home :/
struct BattleHit;

struct BgCoords
{
    u16 x;
    u16 y;
};

struct Struct03003080
{
    /*0x00*/ struct DispCnt dispcnt;
    /*0x04*/ struct DispStat dispstat;
    /*0x08*/ u8 filler8[4];
    /*0x0C*/ struct BgCnt bg0cnt;
    /*0x10*/ struct BgCnt bg1cnt;
    /*0x14*/ struct BgCnt bg2cnt;
    /*0x18*/ struct BgCnt bg3cnt;
    /*0x1C*/ struct BgCoords bgoffset[4];
    /*0x2C*/ u8 win0_right, win0_left;
    /*0x2C*/ u8 win1_right, win1_left;
    /*0x30*/ u8 win0_bottom, win0_top;
    /*0x30*/ u8 win1_bottom, win1_top;
    /*0x34*/ struct WinCnt wincnt;
    /*0x38*/ u16 mosaic;
             u8 filler3A[2];
    /*0x3C*/ struct BlendCnt bldcnt;
    /*0x40*/ u8 filler40[4];
    /*0x44*/ u8 blendCoeffA;
    /*0x45*/ u8 blendCoeffB;
    /*0x46*/ u8 blendY;
    /*0x48*/ u16 bg2pa;
    /*0x4A*/ u16 bg2pb;
    /*0x4C*/ u16 bg2pc;
    /*0x4E*/ u16 bg2pd;
    /*0x50*/ u32 bg2x;
    /*0x54*/ u32 bg2y;
    /*0x58*/ u16 bg3pa;
    /*0x5A*/ u16 bg3pb;
    /*0x5C*/ u16 bg3pc;
    /*0x5E*/ u16 bg3pd;
    /*0x60*/ u32 bg3x;
    /*0x64*/ u32 bg3y;
    /*0x68*/ s8 colorAddition;
};

struct Struct0858791C
{
    u8 filler0[4];
    u16 unk2;
};

struct TileDataTransfer
{
    void *src;
    void *dest;
    u16 size;
    u16 mode;
};

struct OamDataTransfer
{
    void *src;
    void *dest;
    u16 unk8;
    u16 count;
};

struct Struct02024CD4
{
    int unk0;
    int unk4;
};

struct KeyStatusBuffer
{
    u8 repeatDelay;     // initial delay before generating auto-repeat presses
    u8 repeatInterval;  // time between auto-repeat presses
    u8 repeatTimer;     // (decreased by one each frame, reset to repeatDelay when Presses change and repeatInterval when reaches 0)
    u16 heldKeys;       // keys that are currently held down
    u16 repeatedKeys;   // auto-repeated keys
    u16 newKeys;        // keys that went down this frame
    u16 prevKeys;       // keys that were held down last frame
    u16 LastPressState;
    bool16 ABLRPressed; // 1 for Release (A B L R Only), 0 Otherwise
    u16 newKeys2;
    u16 TimeSinceStartSelect; // Time since last Non-Start Non-Select Button was pressed
};

typedef void (*InterruptHandler)(void);

struct Vec2 { short x, y; };
struct Vec2u { u16 x, y; };

struct Struct0202BCB0 // Game State Struct
{
    /* 00 */ u8  mainLoopEndedFlag;

    /* 01 */ s8  gameLogicSemaphore;
    /* 02 */ s8  gameGfxSemaphore;

    /* 03 */ u8  _unk04;

    /* 04 */ u8  gameStateBits;

    /* 05 */ u8  _unk05;

    /* 06 */ u16 prevVCount;

    /* 08 */ u32 _unk08;

    /* 0C */ struct Vec2 camera;
    /* 10 */ struct Vec2 cameraPrevious;
    /* 14 */ struct Vec2 playerCursor;
    /* 18 */ struct Vec2 unk18;
    /* 1C */ struct Vec2 unk1C;
    /* 20 */ struct Vec2 playerCursorDisplay;
    /* 24 */ struct Vec2u mapRenderOrigin;
    /* 28 */ struct Vec2 unk28;

    /* 2C */ u16 itemUnk2C;
    /* 2E */ u16 itemUnk2E;

    /* 30 */ u8 _pad30[0x3C - 0x30];

    /* 3C */ u8 unk3C;
    /* 3D */ u8 unk3D;
    /* 3E */ u8 unk3E;
    /* 3F */ s8 unk3F;
};

struct Struct0202BCF0 { // Chapter Data Struct
    /* 00 */ u32 unk0; // a time value
    /* 04 */ u32 unk4; // a time value

    /* 08 */ u32 partyGoldAmount;
    /* 0C */ u8  gameSaveSlot;

    /* 0D */ u8  chapterVisionRange; // 0 means no fog
    /* 0E */ s8  chapterIndex;

    /* 0F */ u8  chapterPhaseIndex; // 0x00 = Player phase, 0x40 = NPC phase, 0x80 = Enemy phase (0xC0 = link arena 4th team?)

    /* 10 */ u16 chapterTurnNumber;

    /* 12 */ u8  xCursor, yCursor; // map cursor position

    /* 14 */ u8  chapterStateBits; // +0x04 = postgame, +0x10 = in prep screen, +0x40 = difficult mode, +0x80 = link arena?
    /* 15 */ u8  chapterWeatherId;
    /* 16 */ u16 chapterTotalSupportGain;

    /* 18 */ u8 playthroughIdentifier;
    /* 19 */ u8 unk19;
    /* 1A */ u8 lastUnitSortType;

    /* 1B */ u8  chapterModeIndex; // 1 for tutorial (ch0-8), 2 for Eirika route, 3 for Ephraim route

    // character identifiers indexed by weapon type.
    // has to do with allowing unusable weapons to be used
    /* 1C */ u8  unk1C[4];

    /* 20 */ char playerName[0x40 - 0x20]; // unused outside of link arena (was tactician name in FE7); Size unknown

    // option bits
    u32 unk40_1:1; // 1
    u32 unk40_2:1; // 1
    u32 unk40_3:2; // 2
    u32 unk40_5:1; // 1
    u32 cfgTextSpeed:2;
    u32 unk40_8:1; // 1
    u32 unk41_1:1; // 1
    u32 unk41_2:1; // 1
    u32 cfgWindowColor:2;
    u32 unk41_5:1; // 1
    u32 unk41_6:1; // unk
    u32 unk41_7:1; // 1
    u32 unk41_8:1; // 1
    u32 unk42_1:1; // unk
    u32 unk42_2:2; // 2
    u32 unk42_4:2; // 2
    u32 unk42_6:1; // 1
    u32 unk42_7:1; // unk
    u32 unk42_8:2; // 2 (!)
    u32 unk43_2:2; // 2
    u32 unk43_4:5; // unk

    u8  unk44[0x48 - 0x44];

    u16 unk48;

    unsigned unk4A_1 : 1;
    unsigned unk4A_2 : 3;
    unsigned unk4A_5 : 4;
    u8 unk4B;
};

/**
 * Use with Struct0202BCF0 field chapterStateBits
 */
enum
{
    CHAPTER_FLAG_0          = (1 << 0),
    CHAPTER_FLAG_1          = (1 << 1),
    CHAPTER_FLAG_POSTGAME   = (1 << 2),
    CHAPTER_FLAG_3          = (1 << 3),
    CHAPTER_FLAG_PREPSCREEN = (1 << 4),
    CHAPTER_FLAG_5          = (1 << 5),
    CHAPTER_FLAG_DIFFICULT  = (1 << 6),
    CHAPTER_FLAG_7          = (1 << 7)
};

struct TextBuffer0202A6AC
{
    u8 buffer0202A6AC[0x555];
    u8 buffer0202AC01[0x555];
    u8 buffer0202B156[0x356];
    u8 buffer0202B4AC[0x100];
    u8 buffer0202B5AC[1];  // unknown length
};

struct ActionData
{
    // unknown stuff (sometimes RNs are pushed here) (maybe an union?)
    /* 00 */ u16 _u00[3];
    /* 06 */ u16 unk6;

    /* 08 */ u16 unk08[2];

    /* 0C */ u8 subjectIndex;
    /* 0D */ u8 targetIndex;

    /* 0E */ u8 xMove;
    /* 0F */ u8 yMove;

    /* 10 */ u8 moveCount;

    /* 11 */ u8 unitActionType;

    // maybe from this onwards it's an union?

    /* 12 */ u8 itemSlotIndex;

    /* 13 */ u8 xOther;
    /* 14 */ u8 yOther;

    /* 15 */ u8 trapType;

    /* 16 */ u8 suspendPointType;

    /* 18 */ struct BattleHit* scriptedBattleHits;

    /* 1C+ TODO (sizeof(struct ActionData) == 0x38) */
};

enum
{
    FACING_LEFT  = 0,
    FACING_RIGHT = 1,
    FACING_DOWN  = 2,
    FACING_UP    = 3,
};

enum
{
    // 0x00?
    UNIT_ACTION_WAIT = 0x01,
    UNIT_ACTION_COMBAT = 0x02,
    UNIT_ACTION_STAFF = 0x03,
    UNIT_ACTION_DANCE = 0x04,
    // 0x05?
    UNIT_ACTION_STEAL = 0x06,
    UNIT_ACTION_SUMMON = 0x07,
    UNIT_ACTION_SUMMON_DK = 0x08,
    UNIT_ACTION_RESCUE = 0x09,
    UNIT_ACTION_DROP = 0x0A,
    UNIT_ACTION_TAKE = 0x0B,
    UNIT_ACTION_GIVE = 0x0C,
    // 0x0D?
    UNIT_ACTION_TALK = 0x0E,
    UNIT_ACTION_SUPPORT = 0x0F,
    UNIT_ACTION_VISIT = 0x10,
    UNIT_ACTION_SEIZE = 0x11,
    UNIT_ACTION_DOOR = 0x12,
    // 0x13?
    UNIT_ACTION_CHEST = 0x14,
    UNIT_ACTION_PICK = 0x15,
    // 0x16?
    UNIT_ACTION_SHOPPED = 0x17,
    // 0x18?
    UNIT_ACTION_ARENA = 0x19,
    UNIT_ACTION_USE_ITEM = 0x1A,
    UNIT_ACTION_TRADED = 0x1B,
    // 0x1C?
    UNIT_ACTION_TRADED_1D = 0x1D,
    UNIT_ACTION_TRAPPED = 0x1E,
    // 0x1F?
    // 0x20?
    UNIT_ACTION_RIDE_BALLISTA = 0x21,
    UNIT_ACTION_EXIT_BALLISTA = 0x22
};

enum
{
    SUSPEND_POINT_PLAYERIDLE = 0,
    SUSPEND_POINT_DURINGACTION = 1,
    SUSPEND_POINT_CPPHASE = 2,
    SUSPEND_POINT_BSKPHASE = 3,
    SUSPEND_POINT_DURINGARENA = 4,
    SUSPEND_POINT_5 = 5,
    SUSPEND_POINT_6 = 6,
    SUSPEND_POINT_7 = 7,
    SUSPEND_POINT_8 = 8,
    SUSPEND_POINT_PHASECHANGE = 9
};

enum
{
    GAME_ACTION_3 = 3
};

enum
{
    WEATHER_NONE = 0,
    WEATHER_SNOW = 1,
    WEATHER_SNOWSTORM = 2,
    WEATHER_3 = 3,
    WEATHER_RAIN = 4,
    WEATHER_FLAMES = 5,
    WEATHER_SANDSTORM = 6,
    WEATHER_CLOUDS = 7
};

struct UnknownStructCTC
{
    struct UnknownStructCTC *unk0;
    s16 unk4;
    s16 unk6;
    u16 unk8;
    u8 fillerA[2];
    const void *unkC;
};

struct SMSHandle
{
    /* 00 */ struct SMSHandle* pNext;

    /* 04 */ short xDisplay;
    /* 06 */ short yDisplay;

    /* 08 */ u16 oam2Base;

    /* 0A */ u8 _u0A;
    /* 0B */ s8 config;
};

struct MapAnimActorState
{
    /* 00 */ struct Unit* unit;
    /* 04 */ struct BattleUnit* bu;
    /* 08 */ struct MUProc* mu;
    /* 0C */ u8 u0C;
    /* 0D */ u8 u0D;
    /* 0E */ u16 u0E;
    /* 10 */ u8 u10;
    /* 11 */ u8 u11;
    /* 12 */ u8 u12;
    /* 13 */ u8 u13;
};

struct MapAnimState
{
    /* 00 */ struct MapAnimActorState actor[4];

    /* 50 */ struct BattleHit* hitIt;
    /* 54 */ const struct ProcCmd* specialProcScr;
    /* 58 */ u8 attackerActorNum;
    /* 59 */ u8 defenderActorNum;
    /* 5A */ u16 hitAttributes; /* NOTE: this is a bug! hit attribures are 19bits */
    /* 5C */ u8 hitInfo;
    /* 5D */ s8 hitDamage;
    /* 5E */ u8 actorCount_maybe;
    /* 5F */ u8 u5F;
    /* 60 */ u8 u60;
    /* 61 */ u8 u61;
    /* 62 */ u8 u62;
};

struct MMSData
{
    const void* pGraphics;
    const void* pAnimation;
};

struct ArenaData
{
    /* 00 */ struct Unit* playerUnit;
    /* 04 */ struct Unit* opponentUnit;
    /* 08 */ short unk08;
    /* 0A */ u8 unk0A;
    /* 0B */ u8 unk0B;
    /* 0C */ u8 range;
    /* 0D */ u8 playerWpnType;
    /* 0E */ u8 opponentWpnType;
    /* 0F */ u8 playerClassId;
    /* 10 */ u8 opponentClassId;
    /* 11 */ u8 playerLevel;
    /* 12 */ u8 oppenentLevel;
    /* 13 */ s8 playerIsMagic;
    /* 14 */ s8 opponentIsMagic;
    /* 16 */ short playerPowerWeight;
    /* 18 */ short opponentPowerWeight;
    /* 1A */ u16 playerWeapon;
    /* 1C */ u16 opponentWeapon;
};

struct GMapData
{
    /* 00 */ u8 state;
    /* 01 */ u8 unk01;
    /* 02 */ short xCamera;
    /* 04 */ short yCamera;
    /* 08 */ u32 unk08;
    /* 0C */ u32 unk0C;
    /* 10 */ struct { u8 state, location; u16 unk; } unk10[4];
    /* 20 */ struct { u8 state, location; u16 unk; } unk20[4];
    /* 30 */ struct { u8 unk; } unk30[0x1D];
};

enum
{
    // For use with GMapData:state

    GMAP_STATE_BIT0 = (1 << 0),
    GMAP_STATE_BIT1 = (1 << 1),
    GMAP_STATE_BIT2 = (1 << 2),
    GMAP_STATE_BIT3 = (1 << 3),
    GMAP_STATE_BIT4 = (1 << 4),
    GMAP_STATE_BIT5 = (1 << 5),
    GMAP_STATE_BIT6 = (1 << 6),
    GMAP_STATE_BIT7 = (1 << 7),
};

struct MapChange
{
    /* 00 */ s8 id;
    /* 01 */ u8 xOrigin;
    /* 02 */ u8 yOrigin;
    /* 03 */ u8 xSize;
    /* 04 */ u8 ySize;
    /* 08 */ const void* data;
};

struct UnitUsageStats
{
	/* 000 */ unsigned lossAmt     : 8;
	/* 008 */ unsigned favval      : 16;
	/* 024 */ unsigned actAmt      : 8;
	/* 032 */ unsigned statViewAmt : 8;
	/* 040 */ unsigned deathLoc    : 6;
	/* 046 */ unsigned deathTurn   : 10;
	/* 056 */ unsigned deployAmt   : 6;
	/* 062 */ unsigned moveAmt     : 10;
	/* 072 */ unsigned deathCause  : 4;
	/* 076 */ unsigned expGained   : 12;
	/* 088 */ unsigned winAmt      : 10;
	/* 098 */ unsigned battleAmt   : 12;
	/* 110 */ unsigned killerPid   : 9;
	/* 119 */ unsigned deathSkirm  : 1;
	/* 120 */ /* 8bit pad */
};

enum { UNIT_SUPPORT_MAX_COUNT = 7 };

enum
{
    SAVE_BLOCK_SAVE_BASE      = 0,
    SAVE_BLOCK_SAVE1          = SAVE_BLOCK_SAVE_BASE + 0,
    SAVE_BLOCK_SAVE2          = SAVE_BLOCK_SAVE_BASE + 1,
    SAVE_BLOCK_SAVE3          = SAVE_BLOCK_SAVE_BASE + 2,

    SAVE_BLOCK_SUSPEND_BASE   = 3,
    SAVE_BLOCK_SUSPEND        = SAVE_BLOCK_SUSPEND_BASE + 0,
    SAVE_BLOCK_SUSPEND_BACKUP = SAVE_BLOCK_SUSPEND_BASE + 1,
    // TODO: 5 & 6
};

// TODO: move to bmcontainer.h
enum { CONVOY_ITEM_COUNT = 100 };

enum
{
    FACE_BLINK_NORMAL = 1,
    FACE_BLINK_CLOSED = 6,
};

struct FaceData
{
    /* 00 */ const u8*  img;
    /* 04 */ const u8*  imgChibi;
    /* 08 */ const u16* pal;
    /* 0C */ const u8*  imgMouth;
    /* 10 */ const u8*  imgCard;
    /* 14 */ u8 xMouth, yMouth;
    /* 16 */ u8 xEyes,  yEyes;
    /* 18 */ u8 blinkKind;
};

#endif // GUARD_TYPES_H
