#ifndef GUARD_BMIO_H
#define GUARD_BMIO_H

enum
{
    BM_BGPAL_6 = 6,
    BM_BGPAL_TILESET_BASE = 7,

    BM_BGPAL_BANIM_IFBACK = 1, /* size: 2 */
    BM_BGPAL_BANIM_UNK4 = 4,
    BM_BGPAL_BANIM_UNK5 = 5,
};

enum
{
    BM_BGCHR_BANIM_IFBACK = 1,
    BM_BGCHR_BANIM_UNK160 = 0x160,
    BM_BGCHR_BANIM_UNK200 = 0x200,

    BM_OBJCHR_BANIM_EFFECT = 384,
    BM_OBJCHR_BANIM_EFFECT2 = 448,
};

enum
{
    BM_OBJPAL_1  = 1,
    BM_OBJPAL_10 = 10,

    BM_OBJPAL_BANIM_EFFECT1 = 3,
    BM_OBJPAL_BANIM_EFFECT2 = 4,

    BM_OBJPAL_BANIM_SPECIALMU = 8, /* size: 2 */

    BM_OBJPAL_UNIT_GRAYED = 15,
};

struct TileGfxAnim
{
    /* 00 */ u16         time;
    /* 02 */ u16         size;
    /* 04 */ const void* data;
};

struct TilePalAnim
{
    /* 00 */ const void* data;
    /* 04 */ u8          time;
    /* 05 */ u8          colorCount;
    /* 06 */ u8          colorStart;
};

struct GameCtrlProc
{
    PROC_HEADER;
};

struct BMapMainProc
{
    PROC_HEADER;

    /* 29 */ u8 pad29[0x54 - 0x29];
    /* 54 */ struct GameCtrlProc* gameCtrl;
};

void BMapVSync_Start(void);
void BMapVSync_End(void);
void BMapDispSuspend(void);
void BMapDispResume(void);
void AllocWeatherParticles(unsigned weatherId);
void WfxFlamesInitGradientPublic(void);
void DisableMapPaletteAnimations(void);
void ResetMapPaletteAnimations(void);
void SetWeather(unsigned weatherId);

// this may have been two (or more?) source files
// if this is to be split, this the starting point.

u8 GetTextDisplaySpeed(void);
int IsFirstPlaythrough(void);
void InitPlaythroughState(int isDifficult, s8);
void StartBattleMap(struct GameCtrlProc* gameCtrl);
void RestartBattleMap(void);
void GameCtrl_StartResumedGame(struct GameCtrlProc* gameCtrl);
void RefreshBMapDisplay_FromBattle(void);
void BMapDispResume_FromBattleDelayed(void);
void RefreshBMapGraphics(void);
struct BMapMainProc* StartBMapMain(struct GameCtrlProc* gameCtrl);
void EndBMapMain(void);
void ChapterChangeUnitCleanup(void);
void MapMain_ResumeFromPhaseIdle(struct BMapMainProc* mapMain);
void MapMain_ResumeFromAction(struct BMapMainProc* mapMain);
void MapMain_ResumeFromBskPhase(struct BMapMainProc* mapMain);
void MapMain_ResumeFromArenaFight(struct BMapMainProc* mapMain);
void MapMain_ResumeFromPhaseChange(struct BMapMainProc* mapMain);
void GameCtrl_DeclareCompletedChapter(void);
void GameCtrl_DeclareCompletedPlaythrough(void);
char* GetTacticianName(void);
void SetTacticianName(const char* newName);

#endif // GUARD_BMIO_H
