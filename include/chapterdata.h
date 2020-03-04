#ifndef GUARD_CHAPTERDATA_H
#define GUARD_CHAPTERDATA_H

struct ROMChapterData {
    /* 00 */ const char* internalName;

    /* 04 */ u8  mapObj1Id;
    /* 05 */ u8  mapObj2Id;
    /* 06 */ u8  mapPaletteId;
    /* 07 */ u8  mapTileConfigId;
    /* 08 */ u8  mapMainLayerId;
    /* 09 */ u8  mapTileAnim1Id;
    /* 0A */ u8  mapTileAnim2Id;
    /* 0B */ u8  mapChangeLayerId;

    /* 0C */ u8  initialFogLevel;
    /* 0D */ u8  _unk0D;

    /* 0E */ u8 _unk10[0x12 - 0x0E];

    /* 12 */ u8 initialWeather;
    /* 13 */ u8 battleTileSet;

    // This may need a type change.
    /* 14 */ u8 easyModeLevelMalus      : 4;
    /* 14 */ u8 difficultModeLevelBonus : 4;
    /* 14 */ u8 normalModeLevelMalus    : 4;

    // 0 for blue phase, 1 for red phase, 2 for green phase, 3-5 unknown/maybe unused
    // 6 for blue/green alt, 7 for red alt
    /* 16 */ u16 mapSongIndices[8];

    /* 26 */ u8 _unk26[0x2C - 0x26];

    /* 2C */ u8 mapCrackedWallHeath;

    /* 2D */ u8 _unk2D[0x70 - 0x2D];
    /* 70 */ u32 unk70;

    /* 74 */ u8 mapEventDataId;
    /* 75 */ u8 gmapEventId;

    /* 76 */ u8 _unk76[0x80 - 0x76];

    /* 80 */ u8 prepScreenNumber;

    /* 81 */ u8 _unk81[0x86 - 0x81];

    /* 86 */ u8 victorySongEnemyThreshold;
    /* 87 */ u8 boolFadeToBlack;

    /* 88 */ u16 statusObjectiveTextId;
    /* 8A */ u16 goalWindowTextId;
    /* 8C */ u8 goalWindowDataType;
    /* 8D */ u8 goalWindowEndTurnNumber;
    /* 8E */ u8 protectCharacterIndex;

    /* 8F */ u8 xMarkedTile;
    /* 90 */ u8 yMarkedTile;

    /* 91 */ u8 _unk91[0x94 - 0x91];
};

extern const struct ROMChapterData gChapterDataTable[];
extern const void** gUnknown_08A1FB34;
extern const void* gChapterDataAssetTable[];

const struct ROMChapterData* GetROMChapterStruct(unsigned chIndex);
const void* GetChapterMapPointer(unsigned chIndex);
const void* GetChapterMapChangesPointer(unsigned chIndex);
const void* GetChapterEventDataPointer(unsigned chIndex);
const char* sub_80346E0(unsigned chIndex);
int IsDifficultMode(void);

#endif // GUARD_CHAPTERDATA_H
