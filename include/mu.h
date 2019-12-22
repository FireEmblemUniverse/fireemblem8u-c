#ifndef GUARD_MU_H
#define GUARD_MU_H

/*
    "MOVEUNIT" proc and related functions.
    Handles managing and displaying moving map sprites.
*/

enum {
    // MU Magic Constants

    // Maximum simultaneous MU count
    MU_MAX_COUNT = 4,

    // Maximum move command count
    MU_COMMAND_MAX_COUNT = 0x40,

    // Maximum size of uncompressed MU sprite sheet
    MU_GFX_MAX_SIZE = 0x2200,

    // Default base obj tile index for storing sprites in VRAM
    MU_BASE_OBJ_TILE = 0x380,

    // obj palette id used for MU fade effects
    MU_FADE_OBJ_PAL = 5,

    // Number of bits used to keep track of subpixels
    MU_SUBPIXEL_PRECISION = 4,
};

enum {
    // Possible MU States

    MU_STATE_NOSTATE,
    MU_STATE_NONACTIVE,
    MU_STATE_MOVEMENT,
    MU_STATE_WAITING,
    MU_STATE_UNK4,
    MU_STATE_BUMPING,
    MU_STATE_UI_DISPLAY,
    MU_STATE_DEATHFADE,
};

enum {
    // MU command identifiers

    MU_COMMAND_END = -1, // end

    MU_COMMAND_MOVE_BASE,

    MU_COMMAND_MOVE_LEFT  = MU_COMMAND_MOVE_BASE + FACING_LEFT,
    MU_COMMAND_MOVE_RIGHT = MU_COMMAND_MOVE_BASE + FACING_RIGHT,
    MU_COMMAND_MOVE_DOWN  = MU_COMMAND_MOVE_BASE + FACING_DOWN,
    MU_COMMAND_MOVE_UP    = MU_COMMAND_MOVE_BASE + FACING_UP,

    MU_COMMAND_HALT,

    MU_COMMAND_FACE_BASE,

    MU_COMMAND_FACE_LEFT  = MU_COMMAND_FACE_BASE + FACING_LEFT,
    MU_COMMAND_FACE_RIGHT = MU_COMMAND_FACE_BASE + FACING_RIGHT,
    MU_COMMAND_FACE_DOWN  = MU_COMMAND_FACE_BASE + FACING_DOWN,
    MU_COMMAND_FACE_UP    = MU_COMMAND_FACE_BASE + FACING_UP,

    MU_COMMAND_WAIT,
    MU_COMMAND_BUMP,
    MU_COMMAND_UNK11,
    MU_COMMAND_SET_SPEED,

    MU_COMMAND_CAMERA_ON,
    MU_COMMAND_CAMERA_OFF,
};

enum {
    // MU facing identifiers

    MU_FACING_LEFT  = FACING_LEFT,
    MU_FACING_RIGHT = FACING_RIGHT,
    MU_FACING_DOWN  = FACING_DOWN,
    MU_FACING_UP    = FACING_UP,

    MU_FACING_SELECTED = 4,

    // TODO: Which is MU_FACING_DANCING? 5? 6?

    MU_FACING_UNK11    = 11, // The facing id at start
    MU_FACING_STANDING = 15,
};

enum {
    // MU flash identifiers

    MU_FLASH_0,
    MU_FLASH_1,
    MU_FLASH_2,
    MU_FLASH_3,
    MU_FLASH_4,
    MU_FLASH_5,
};

struct MUConfig;

struct MUProc {
    PROC_HEADER;

    /* 2C */ struct Unit* pUnit;
    /* 30 */ struct APHandle* pAPHandle;
    /* 34 */ struct MUConfig* pMUConfig;
    /* 38 */ void* pGfxVRAM;

    /* 3C */ u8 muIndex;
    /* 3D */ u8 _u3D;
    /* 3E */ u8 boolAttractCamera;
    /* 3F */ u8 stateId;
    /* 40 */ u8 boolIsHidden;
    /* 41 */ u8 displayedClassId;
    /* 42 */ s8 facingId;
    /* 43 */ u8 stepSoundTimer;
    /* 44 */ u8 boolForceMaxSpeed;
    /* 46 */ u16 objPriorityBits;
    /* 48 */ u16 moveTimer;
    /* 4A */ short moveConfig;

    // Coordinates are in 16th of pixel
    /* 4C */ short xSubPosition;
    /* 4E */ short ySubPosition;
    /* 50 */ short xSubOffset;
    /* 52 */ short ySubOffset;
};

struct MUConfig {
    /* 00 */ u8  muIndex;
    /* 01 */ u8  paletteIndex;
    /* 02 */ u16 objTileIndex;
    /* 04 */ u8  currentCommand;
    /* 05 */ s8  commands[MU_COMMAND_MAX_COUNT];
    /* 45 */ // 3 byte padding
    /* 48 */ struct MUProc* pMUProc;
};

extern CONST_DATA struct ProcCmd gProcScr_MoveUnit[];
extern CONST_DATA struct ProcCmd gProcScr_MUDeathFade[];
extern CONST_DATA struct ProcCmd gProcScr_MUBlinkEffect[];
extern CONST_DATA struct ProcCmd gProcScr_MU_89A2CF8[];

// Buffer for MU graphics
// what to do with this?
extern u8 gMUGfxBuffer[];

// FUNCTIONS

void MU_Init();

struct MUProc* MU_CreateExt(struct Unit* pUnit, unsigned classIndex, unsigned palId);
struct MUProc* MU_Create(struct Unit* pUnit);

void MU_ManualUpdate(struct MUProc* proc);

void MU_EnableAttractCamera(struct MUProc* proc);
void MU_DisableAttractCamera(struct MUProc* proc);

struct MUProc* MU_CreateForUI(struct Unit* pUnit, int x, int y);

void MU_8078524(struct MUProc* proc);

void MU_SetFacing(struct MUProc* proc, int facingId);
void MU_SetDefaultFacing(struct MUProc* proc);
void MU_SetDefaultFacing_Auto(void);
void MU_StartMoveScript_Auto(const u8 commands[MU_COMMAND_MAX_COUNT]);

int MU_Exists(void);
int MU_IsAnyActive(void);

int MU_IsActive(struct MUProc* proc);
void MU_StartMoveScript(struct MUProc* proc, const u8 commands[MU_COMMAND_MAX_COUNT]);

struct MUProc* MU_CreateScripted(u16 x, u16 y, u16 classIndex, unsigned palId, const u8 commands[MU_COMMAND_MAX_COUNT]);

void MU_StartStepSfx(int soundId, int b, int hPosition);

void MU_PlayStepSfx(struct MUProc* proc);

void MU_StartFogBumpFx(int x, int y);
u8 MU_IsFogBumpFxActive(void);

void MU_EndAll(void);
void MU_End(struct MUProc* proc);

void MU_AllDisable(void);
void MU_AllEnable(void);

void MU_GetComputedEndPosition(int* xOut, int* yOut, const u8* commands);

int MU_CanStart(void);

void MU_AllRestartAnimations(void);

u8 MU_ComputeDisplayPosition(struct MUProc* proc, struct Vec2* out);

void MU_SetMoveConfig(struct MUProc* proc, u16 config);

void MU_StartDeathFade(struct MUProc* muProc);
void MU_StartBlinkEffect(struct MUProc* muProc);
void MU_StartPixelEffect(struct MUProc* muProc);

void MU_Hide(struct MUProc* proc);
void MU_Show(struct MUProc* proc);

void MU_SetDisplayPosition(struct MUProc* proc, int x, int y);
void MU_SetDisplayOffset(struct MUProc* proc, int xOff, int yOff);

void MU_StartFlashFade(struct MUProc* proc, int flashType);
void MU_8079858(struct MUProc* muProc);

void MU_StartActionAnim(struct MUProc* proc);
void MU_StartDelayedFaceTarget(struct MUProc* proc);
void MU_StartFastMoveAnim(struct MUProc* proc);
void MU_StartCritFlash(struct MUProc* muProc, int flashType);
void MU_StartHitFlash(struct MUProc* muProc, int flashType);

void MU_AllForceSetMaxMoveSpeed(void);

void MU_SetSpecialSprite(struct MUProc* proc, int displayedClassId, const u16* palette);

void MU_SetPaletteId(struct MUProc* proc, unsigned paletteId);

struct MUProc* MU_GetByUnit(struct Unit* unit);

void MU_SortObjLayers(void);

#endif // GUARD_MU_H
