#include "global.h"

#include "constants/items.h"
#include "constants/classes.h"

#include "proc.h"
#include "ap.h"
#include "m4a.h"
#include "soundwrapper.h"
#include "hardware.h"
#include "bmio.h"
#include "bmunit.h"
#include "bmmap.h"
#include "bmtrick.h"
#include "bmbattle.h"
#include "mu.h"

/*
    "MOVEUNIT" proc and related functions.
    Handles managing and displaying moving map sprites.
*/

struct MUStepSoundProc {
    PROC_HEADER;

    /* 29 */ u8 _pad29[0x58 - 0x29];

    /* 58 */ unsigned idSound1;
    /* 5C */ unsigned idSound2;
    /* 60 */ unsigned u60_buggedmaybe; // Checked for in MU_StartStepSfx but never initialized
    /* 64 */ short xSound1;
    /* 66 */ short xSound2;
};

struct MUFogBumpFxProc {
    PROC_HEADER;

    /* 2C */ int xDisplay;
    /* 30 */ int yDisplay;

    /* 34 */ u8 _pad34[0x50 - 0x34];
    /* 50 */ struct APHandle* pAPHandle;

    /* 54 */ u8 _pad54[0x64 - 0x54];
    /* 64 */ short timer;
};

struct MUEffectProc {
    PROC_HEADER;

    /* 29 */ u8 _pad29[0x54 - 0x29];
    /* 54 */ struct MUProc* pMUProc;

    /* 58 */ u8 _pad58[0x64 - 0x58];
    /* 64 */ short timeLeft;
    /* 66 */ short frameIndex;
};

struct MUFlashEffectProc {
    PROC_HEADER;

    /* 2C */ struct MUProc* pMUProc;
    /* 30 */ u8 timer;
};

typedef void(*MUStateHandlerFunc)(struct MUProc*);

static struct MUProc* MU_CreateInternal(u16 x, u16 y, u16 classIndex, int objTileId, unsigned palId);

static void MU_StepSound_OnInit(struct MUStepSoundProc* proc);
static void MU_StepSound_OnFirstSound(struct MUStepSoundProc* proc);
static void MU_StepSound_OnSecondSound(struct MUStepSoundProc* proc);

static void MU_8078894(struct MUProc* proc);

static void MU_InterpretCommandScript(struct MUProc* proc);

static void MU_FogBumpFx_FirstFrame(struct MUFogBumpFxProc* proc);
static void MU_FogBumpFx_TransitionInLoop(struct MUFogBumpFxProc* proc);
static void MU_FogBumpFx_DisplayLoop(struct MUFogBumpFxProc* proc);

static void MU_State_WaitForFogBump(struct MUProc* proc);
static void MU_State_StartMovement(struct MUProc* proc);
static void MU_State_WaitForTimer(struct MUProc* proc);
static void MU_State_None(struct MUProc* proc);
static void MU_State_NoOp(struct MUProc* proc);
static void MU_State_DuringMovement(struct MUProc* proc);

static void MU_AdvanceStepSfx(struct MUProc* proc);

static void MU_OnLoop(struct MUProc* proc);
static void MU_OnEnd(struct MUProc* proc);

static void MU_EndInternal(struct MUProc* proc);

static void MU_80790CC(struct MUProc* proc);

static struct MUConfig* MU_GenerateConfigDefault(int objTileId, u8* outIndex);
static struct MUConfig* MU_GenerateConfigOther(int objTileId, u8* outIndex);

static void MU_DisplayAsSMS(struct MUProc* proc);
static void MU_DisplayAsMMS(struct MUProc* proc);

static u16 MU_GetMovementSpeed(struct MUProc* proc);

static void* MU_GetGfxBufferById(int muIndex);

static const void* MU_GetSheetGfx(struct MUProc* proc);
static const void* MU_GetAnimationByClassId(u16 classId);

static void MU_DeathFade_OnLoop(struct MUEffectProc* proc);

static void MU_BlinkEffect_OnLoop(struct MUEffectProc* proc);

static void MU_SetupPixelEffect(u32* data, int frame);

static void MU_PixelEffect_OnLoop(struct MUEffectProc* proc);

static void MU_807988C(struct MUEffectProc* proc);

static void MU_EndSelectionApAnim(int argAp);
static void MU_EndRefaceApAnim(int argAp);
static void MU_EndFasterApAnim(int argAp);

static void MU_CritFlash_Init(struct MUFlashEffectProc* proc);
static void MU_CritFlash_SetFadedPalette(struct MUFlashEffectProc* proc);
static void MU_CritFlash_SetRegularPalette(struct MUFlashEffectProc* proc);
static void MU_CritFlash_StartFadeBack_maybe(struct MUFlashEffectProc* proc);
static void MU_CritFlash_SpriteShakeLoop(struct MUFlashEffectProc* proc);
static void MU_CritFlash_RestorePalette(struct MUFlashEffectProc* proc);

static void MU_HitFlash_RestorePalette(struct MUFlashEffectProc* proc);

static void MU_ForceSetMaxMoveSpeed(struct Proc* proc);

static struct MUProc* MU_GetByIndex(int muIndex);

#define MU_GetDisplayXOrg(proc) ((((proc)->xSubPosition + (proc)->xSubOffset) >> MU_SUBPIXEL_PRECISION) + 8)
#define MU_GetDisplayYOrg(proc) ((((proc)->ySubPosition + (proc)->ySubOffset) >> MU_SUBPIXEL_PRECISION) + 8)

#define MU_AdvanceGetCommand(proc) (proc->pMUConfig->commands[proc->pMUConfig->currentCommand++])

// NON-CONST DATA

// Buffer for graphics
// I do not know how to handle this :/
extern u8 gMUGfxBuffer[MU_GFX_MAX_SIZE * MU_MAX_COUNT];

static struct MUConfig sMUConfigArray[MU_MAX_COUNT];

static CONST_DATA u16* sMUFlashColorLookup[] = {
    gUnknown_0859A140,
    gUnknown_0859A120,
    gUnknown_0859A160,
    gUnknown_0859A180,
    gUnknown_0859A1A0,
    gUnknown_0859A1C0,
};

// gProc_MUStepSound
static CONST_DATA struct ProcCmd sProcScr_MUStepSound[] = {
    PROC_CALL_ROUTINE(MU_StepSound_OnInit),

    PROC_SLEEP(0),
    PROC_CALL_ROUTINE(MU_StepSound_OnFirstSound),

    PROC_SLEEP(0),
    PROC_CALL_ROUTINE(MU_StepSound_OnSecondSound),

    PROC_END
};

static CONST_DATA struct ProcCmd sProcScr_MUFogBumpFx[] = {
    PROC_CALL_ROUTINE(MU_FogBumpFx_FirstFrame),
    PROC_LOOP_ROUTINE(MU_FogBumpFx_TransitionInLoop),
    PROC_LOOP_ROUTINE(MU_FogBumpFx_DisplayLoop),

    PROC_END
};

// gDirectionMoveOffsetLookup
static CONST_DATA short sDirectionMoveOffsetLookup[] = {
    -1,  0, // left
    +1,  0, // right
    0,  +1, // up
    0,  -1, // down
};

// START MU SFX DEFINITIONS

// feet sounds
CONST_DATA u16 gMUSfxDef_Foot[] = {
    0x10, // loop duration (in frames)
    0x02, // idk

    0x96, // TODO: USE SOUND ID DEFINITION

    // 7 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,

    0x97, // TODO: USE SOUND ID DEFINITION

    // 7 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};

// heavy sounds
CONST_DATA u16 gMUSfxDef_Heavy[] = {
    0x20, // loop duration (in frames)
    0x02, // idk

    0xA4, // TODO: USE SOUND ID DEFINITION

    // 15 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,

    0xA5, // TODO: USE SOUND ID DEFINITION

    // 15 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};

// mounted sounds
CONST_DATA u16 gMUSfxDef_Mounted[] = {
    0x15, // loop duration (in frames)
    0x03, // idk

    0x9A, // TODO: USE SOUND ID DEFINITION

    // 2 frames of no sounds
    0x00, 0x00,

    0x9B, // TODO: USE SOUND ID DEFINITION

    // 6 frames of no sounds
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,

    0x9C, // TODO: USE SOUND ID DEFINITION

    // 10 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};

// wyvern sounds
CONST_DATA u16 gMUSfxDef_Wyvern[] = {
    0x14, // loop duration (in frames)
    0x01, // idk

    0xA0, // TODO: USE SOUND ID DEFINITION

    // 19 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};

// pegasi sounds
CONST_DATA u16 gMUSfxDef_Pegasus[] = {
    0x14, // loop duration (in frames)
    0x01, // idk

    0xA6, // TODO: USE SOUND ID DEFINITION

    // 19 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};

// unused sounds
CONST_DATA u16 gMUSfxDef_Unused_089A2A86[] = {
    0x14, // loop duration (in frames)
    0x01, // idk

    0x2E0, // TODO: USE SOUND ID DEFINITION

    // 19 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};

// zombie sounds
CONST_DATA u16 gMUSfxDef_Zombie[] = {
    0x0F, // loop duration (in frames)
    0x01, // idk

    0x302, // TODO: USE SOUND ID DEFINITION

    // 14 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};

// skelly sounds
CONST_DATA u16 gMUSfxDef_Skeleton[] = {
    0x0F, // loop duration (in frames)
    0x01, // idk

    0x304, // TODO: USE SOUND ID DEFINITION

    // 14 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};

// mogall sounds
CONST_DATA u16 gMUSfxDef_Mogall[] = {
    0x14, // loop duration (in frames)
    0x01, // idk

    0x306, // TODO: USE SOUND ID DEFINITION

    // 19 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};

// spider sounds
CONST_DATA u16 gMUSfxDef_Spider[] = {
    0x0A, // loop duration (in frames)
    0x01, // idk

    0x308, // TODO: USE SOUND ID DEFINITION

    // 9 frames of no sounds
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};

// dog sounds
CONST_DATA u16 gMUSfxDef_Dog[] = {
    0x15, // loop duration (in frames)
    0x01, // idk

    0x30A, // TODO: USE SOUND ID DEFINITION

    // 2 frames of no sounds
    0x00, 0x00,

    0x30A, // TODO: USE SOUND ID DEFINITION

    // 6 frames of no sounds
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,

    0x30A, // TODO: USE SOUND ID DEFINITION

    // 10 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};

// gorgon sounds
CONST_DATA u16 gMUSfxDef_Gorgon[] = {
    0x0F, // loop duration (in frames)
    0x01, // idk

    0x30C, // TODO: USE SOUND ID DEFINITION

    // 14 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};

// Unused/Unreferenced step sounds
CONST_DATA u16 gMUSfxDef_Unused_089A2BCE[] = {
    0x20, // loop duration (in frames)
    0x02, // idk

    0x2E0, // TODO: USE SOUND ID DEFINITION

    // 31 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};

// boat sounds
CONST_DATA u16 gMUSfxDef_Boat[] = {
    0x18, // loop duration (in frames)
    0x01, // idk

    0x30F, // TODO: USE SOUND ID DEFINITION

    // 11 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,

    0x30E, // TODO: USE SOUND ID DEFINITION

    // 11 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};

// myrrh sounds
CONST_DATA u16 gMUSfxDef_Myrrh[] = {
    0x10, // loop duration (in frames)
    0x01, // idk

    0x3BB, // TODO: USE SOUND ID DEFINITION

    // 15 frames of no sounds
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};

// END MU SFX DEFINITIONS

// MU state handler function pointer lookup
static CONST_DATA MUStateHandlerFunc sMUStateHandlerFuncLookup[] = {
    MU_State_None,
    MU_State_NoOp,
    MU_State_DuringMovement,
    MU_State_WaitForTimer,
    MU_State_StartMovement,
    MU_State_WaitForFogBump,
    MU_State_NoOp,
    MU_State_NoOp,
};

CONST_DATA struct ProcCmd gProcScr_MoveUnit[] = {
    PROC_SET_NAME("MOVEUNIT"),
    PROC_SET_MARK(4),

    PROC_SET_DESTRUCTOR(MU_OnEnd),
    PROC_LOOP_ROUTINE(MU_OnLoop)
};

// obj tile id offset by MU id
static CONST_DATA u16 sMUObjTileOffsetLookup_Default[MU_MAX_COUNT] = {
    0x0000, 0x0010, 0x0008, 0x0018
};

// obj tile id offset by MU id (variant?)
static CONST_DATA u16 sMUObjTileOffsetLookup_Other[MU_MAX_COUNT] = { 
    0x0000, 0x0008, 0x0004, 0x0010
};

// Base Movement Speed Lookup
static CONST_DATA u8 sMUBaseMoveSpeedLookup[2] = {
    2, // fast class base movement speed
    1  // slow class base movement speed
};

// Buffer index by MU index (config) lookup
static CONST_DATA u8 sMUBufferIdLookup[MU_MAX_COUNT+1] = {
    0, // dummy entry (this array is 1-indexed)

    0, 2, 1, 3
};

// gProc_MUDeathFadeEffect
CONST_DATA struct ProcCmd gProcScr_MUDeathFade[] = {
    PROC_LOOP_ROUTINE(MU_DeathFade_OnLoop),
    PROC_SLEEP(15),

    PROC_END
};

// gProc_MUBlinkingEffect
CONST_DATA struct ProcCmd gProcScr_MUBlinkEffect[] = {
    PROC_LOOP_ROUTINE(MU_BlinkEffect_OnLoop),
    PROC_END
};

// Doesn't match without being volatile
// :/
static CONST_DATA vu8 sUnknown_089A2CA8[0x40] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F
};

static CONST_DATA struct ProcCmd sProcScr_MUPixelEffect[] = {
    PROC_LOOP_ROUTINE(MU_PixelEffect_OnLoop),
    PROC_END
};

CONST_DATA struct ProcCmd gProcScr_MU_89A2CF8[] = {
    PROC_SLEEP(8),
    PROC_CALL_ROUTINE(MU_807988C),

    PROC_END
};

static CONST_DATA struct ProcCmd sProcScr_MUCritFlash[] = {
    PROC_CALL_ROUTINE(MU_CritFlash_Init),
    PROC_SLEEP(1),

    PROC_CALL_ROUTINE(MU_CritFlash_SetFadedPalette),
    PROC_SLEEP(2),

    PROC_CALL_ROUTINE(MU_CritFlash_SetRegularPalette),
    PROC_SLEEP(3),

    PROC_CALL_ROUTINE(MU_CritFlash_SetFadedPalette),
    PROC_SLEEP(2),

    PROC_CALL_ROUTINE(MU_CritFlash_SetRegularPalette),
    PROC_SLEEP(3),

    PROC_CALL_ROUTINE(MU_CritFlash_SetFadedPalette),
    PROC_SLEEP(1),

    PROC_CALL_ROUTINE(MU_CritFlash_StartFadeBack_maybe),
    PROC_LOOP_ROUTINE(MU_CritFlash_SpriteShakeLoop),

    PROC_SLEEP(17),
    PROC_CALL_ROUTINE(MU_CritFlash_RestorePalette),

    PROC_END
};

static CONST_DATA struct ProcCmd sProcScr_MUHitFlash[] = {
    PROC_SLEEP(17),
    PROC_CALL_ROUTINE(MU_HitFlash_RestorePalette),
    PROC_END
};

// END CONST DATA

void MU_Init(void) {
    int i;

    for (i = 0; i < MU_MAX_COUNT; ++i)
        sMUConfigArray[i].muIndex = 0;
}

struct MUProc* MU_CreateExt(struct Unit* pUnit, unsigned classIndex, unsigned palId) {
    struct MUProc* proc = MU_CreateInternal(
        pUnit->xPos,
        pUnit->yPos,

        classIndex,

        -1,
        palId
    );

    proc->pUnit = pUnit;
    proc->boolAttractCamera = TRUE;

    return proc;
}

struct MUProc* MU_Create(struct Unit* pUnit) {
    struct MUProc* proc;

    unsigned classIndex = UNIT_CLASS_ID(pUnit);

    if (pUnit->state & US_IN_BALLISTA) {
        struct Trap* blst = GetTrap(pUnit->ballistaIndex);

        switch (blst->extra) {

        case ITEM_BALLISTA_REGULAR:
            classIndex = CLASS_BLST_REGULAR_USED;
            break;

        case ITEM_BALLISTA_LONG:
            classIndex = CLASS_BLST_LONG_USED;
            break;

        case ITEM_BALLISTA_KILLER:
            classIndex = CLASS_BLST_KILLER_USED;
            break;

        } // switch (blst->extra)
    }

    proc = MU_CreateInternal(
        pUnit->xPos,
        pUnit->yPos,

        classIndex,

        -1,
        GetUnitMapSpritePaletteIndex(pUnit)
    );

    proc->pUnit = pUnit;
    proc->boolAttractCamera = TRUE;

    return proc;
}

void MU_ManualUpdate(struct MUProc* proc) {
    MU_OnLoop(proc);
}

void MU_EnableAttractCamera(struct MUProc* proc) {
    proc->boolAttractCamera = TRUE;
}

void MU_DisableAttractCamera(struct MUProc* proc) {
    proc->boolAttractCamera = FALSE;
}

struct MUProc* MU_CreateForUI(struct Unit* pUnit, int x, int y) {
    struct MUProc* proc = MU_Create(pUnit);

    if (!proc)
        return NULL;

    proc->xSubPosition = x << MU_SUBPIXEL_PRECISION;
    proc->ySubPosition = y << MU_SUBPIXEL_PRECISION;

    proc->stateId = MU_STATE_UI_DISPLAY;

    return proc;
}

void MU_8078524(struct MUProc* proc) {
    SMS_80266F0(
        GetClassSMSId(proc->displayedClassId),
        proc->muIndex
    );
}

static struct MUProc* MU_CreateInternal(u16 x, u16 y, u16 classIndex, int objTileId, unsigned palId) {
    struct MUConfig* config;
    struct MUProc* proc;
    struct APHandle* ap;

    u8 soundTimer = 0;
    u8 muIndex = 0;

    if (objTileId == -1)
        config = MU_GenerateConfigDefault(objTileId = MU_BASE_OBJ_TILE, &muIndex);
    else
        config = MU_GenerateConfigOther(objTileId, &muIndex);

    if (!config)
        return NULL;

    if (Proc_Find(gProcScr_MoveUnit))
        soundTimer = -2;

    proc = (struct MUProc*) Proc_Create(gProcScr_MoveUnit, ROOT_PROC_5);

    if (!proc)
        return NULL;

    proc->pUnit = NULL;
    proc->stateId = MU_STATE_NONACTIVE;

    proc->xSubPosition = (x * 16) << MU_SUBPIXEL_PRECISION;
    proc->ySubPosition = (y * 16) << MU_SUBPIXEL_PRECISION;

    proc->xSubOffset = 0;
    proc->ySubOffset = 0;

    proc->facingId = MU_FACING_UNK11;

    proc->moveTimer = 0;
    proc->stepSoundTimer = soundTimer;

    proc->displayedClassId = classIndex;
    proc->boolIsHidden = 0;

    proc->pGfxVRAM = OBJ_VRAM0 + (0x20 * objTileId);

    proc->muIndex = muIndex;

    proc->objPriorityBits = 0x800;

    proc->moveConfig = 0;
    proc->boolForceMaxSpeed = FALSE;

    config->paletteIndex = palId;

    ap = AP_Create(MU_GetAnimationByClassId(classIndex), 10);
    AP_SwitchAnimation(ap, MU_FACING_SELECTED);

    CopyDataWithPossibleUncomp(
        MU_GetSheetGfx(proc),
        MU_GetGfxBufferById(config->muIndex)
    );

    ap->pGraphics = MU_GetGfxBufferById(config->muIndex);
    ap->tileBase = ((config->paletteIndex & 0xF) << 12) + config->objTileIndex + proc->objPriorityBits;

    proc->pAPHandle = ap;
    proc->pMUConfig = config;

    config->pMUProc = proc;

    return proc;
}

void MU_SetFacing(struct MUProc* proc, int facingId) {
    proc->facingId = facingId;

    if (facingId == MU_FACING_STANDING)
        sub_8027068(proc->muIndex, proc->pGfxVRAM);
    else
        AP_SwitchAnimation(proc->pAPHandle, proc->facingId);
}

void MU_SetDefaultFacing(struct MUProc* proc) {
    if (GetClassData(proc->displayedClassId)->attributes & CA_MOUNTEDAID)
        MU_SetFacing(proc, 1);
    else
        MU_SetFacing(proc, 2);
}

void MU_SetDefaultFacing_Auto(void) {
    struct MUProc* proc = (struct MUProc*) Proc_Find(gProcScr_MoveUnit);

    if (proc)
        MU_SetDefaultFacing(proc);
}

void MU_StartMoveScript_Auto(const u8 commands[MU_COMMAND_MAX_COUNT]) {
    struct MUProc* proc = (struct MUProc*) Proc_Find(gProcScr_MoveUnit);

    if (proc)
        MU_StartMoveScript(proc, commands);
}

int MU_Exists(void) {
    return Proc_Find(gProcScr_MoveUnit) ? TRUE : FALSE;
}

#if NONMATCHING

int MU_IsAnyActive(void) {
    struct MUProc* proc;
    int i;

    for (i = 0; i < MU_MAX_COUNT; ++i) {
        if (sMUConfigArray[i].muIndex) {
			proc = sMUConfigArray[i].pMUProc;

            if (proc->stateId != MU_STATE_NONACTIVE)
				break;
        }
    }

    if (i >= 4)
        return FALSE;

    return TRUE;
}

#else // NONMATCHING

__attribute__((naked))
int MU_IsAnyActive(void) {
    asm(
        ".syntax unified\n"

        "push {lr}\n"
        "movs r3, #0\n"
        "ldr r0, _08078764  @ sMUConfigArray\n"
        "adds r2, r0, #0\n"
        "adds r2, #0x48\n"
        "adds r1, r0, #0\n"
    "_08078744:\n"
        "ldrb r0, [r1]\n"
        "cmp r0, #0\n"
        "beq _08078754\n"
        "ldr r0, [r2]\n"
        "adds r0, #0x3f\n"
        "ldrb r0, [r0]\n"
        "cmp r0, #1\n"
        "bne _08078768\n"
    "_08078754:\n"
        "adds r2, #0x4c\n"
        "adds r1, #0x4c\n"
        "adds r3, #1\n"
        "cmp r3, #3\n"
        "ble _08078744\n"
        "movs r0, #0\n"
        "b _0807876A\n"
        ".align 2, 0\n"
    "_08078764: .4byte sMUConfigArray\n"
    "_08078768:\n"
        "movs r0, #1\n"
    "_0807876A:\n"
        "pop {r1}\n"
        "bx r1\n"

        ".syntax divided\n"
    );
}

#endif // NONMATCHING

int MU_IsActive(struct MUProc* proc) {
    if (proc->pMUConfig->muIndex && proc->stateId != MU_STATE_NONACTIVE)
        return TRUE;

    return FALSE;
}

void MU_StartMoveScript(struct MUProc* proc, const u8 commands[MU_COMMAND_MAX_COUNT]) {
    int i;

    for (i = 0; i < 0x40; ++i)
        proc->pMUConfig->commands[i] = commands[i];

    proc->pMUConfig->currentCommand = 0;
    proc->stateId = MU_STATE_MOVEMENT;

    MU_PlayStepSfx(proc);
}

struct MUProc* MU_CreateScripted(u16 x, u16 y, u16 classIndex, unsigned palId, const u8 commands[MU_COMMAND_MAX_COUNT]) {
    struct MUProc* proc = MU_CreateInternal(x, y, classIndex, -1, palId);

    if (!proc)
        return NULL;

    MU_StartMoveScript(proc, commands);
    return proc;
}

static void MU_StepSound_OnInit(struct MUStepSoundProc* proc) {
    proc->idSound1 = 0;
    proc->xSound1 = 0;

    proc->idSound2 = 0;
    proc->xSound2 = 0;
}

static void MU_StepSound_OnFirstSound(struct MUStepSoundProc* proc) {
    PlaySpacialSoundMaybe(proc->idSound1, proc->xSound1);
}

static void MU_StepSound_OnSecondSound(struct MUStepSoundProc* proc) {
    if (proc->idSound2)
        PlaySpacialSoundMaybe(proc->idSound2, proc->xSound2);
}

void MU_StartStepSfx(int soundId, int b, int hPosition) {
    struct MUStepSoundProc* proc;

    proc = (struct MUStepSoundProc*) Proc_Find(sProcScr_MUStepSound);

    if (!proc)
        proc = (struct MUStepSoundProc*) Proc_Create(sProcScr_MUStepSound, ROOT_PROC_3);

    if (!proc->idSound1) {
        proc->idSound1 = soundId;
        proc->xSound1 = hPosition;
    } else if (!proc->u60_buggedmaybe) { // TODO: FIXME: Is this a bug? u60_buggedmaybe is never initialized
        proc->idSound2 = soundId + b;
        proc->xSound2 = hPosition;
    }
}

void MU_PlayStepSfx(struct MUProc* proc) {
    MU_AdvanceStepSfx(proc);
}

static void MU_8078894(struct MUProc* proc) {}

static void MU_InterpretCommandScript(struct MUProc* proc) {
    while (TRUE) {
        short command = MU_AdvanceGetCommand(proc);

        switch (command) {

        case MU_COMMAND_WAIT:
            proc->moveTimer = MU_AdvanceGetCommand(proc);
            proc->stateId = MU_STATE_WAITING;

            return;

        case MU_COMMAND_BUMP:
            MU_8078894(proc);

            proc->stateId = MU_STATE_BUMPING;

            MU_StartFogBumpFx(
                (proc->xSubPosition >> MU_SUBPIXEL_PRECISION) - gUnknown_0202BCB0.camera.x,
                (proc->ySubPosition >> MU_SUBPIXEL_PRECISION) - gUnknown_0202BCB0.camera.y
            );

            return;

        case MU_COMMAND_HALT:
            MU_80790CC(proc);

            return;

        case MU_COMMAND_END:
            MU_8078894(proc);
            MU_End(proc);

            return;

        case MU_COMMAND_MOVE_LEFT:
        case MU_COMMAND_MOVE_RIGHT:
        case MU_COMMAND_MOVE_DOWN:
        case MU_COMMAND_MOVE_UP:
            command = command - MU_COMMAND_MOVE_BASE;

            if (command != proc->facingId) {
                MU_GetAnimationByClassId(proc->displayedClassId); // TODO: FIXME: is this a bug?
                MU_SetFacing(proc, command);

                proc->stateId = MU_STATE_MOVEMENT;
            }

            return;

        case MU_COMMAND_FACE_LEFT:
        case MU_COMMAND_FACE_RIGHT:
        case MU_COMMAND_FACE_DOWN:
        case MU_COMMAND_FACE_UP:
            command = command - MU_COMMAND_FACE_BASE;

            if (command != proc->facingId) {
                MU_GetAnimationByClassId(proc->displayedClassId); // TODO: FIXME: is this a bug?
                MU_SetFacing(proc, command);
            }

            break;

        case MU_COMMAND_SET_SPEED:
            proc->moveConfig = MU_AdvanceGetCommand(proc);
            break;

        case MU_COMMAND_CAMERA_ON:
            MU_EnableAttractCamera(proc);
            break;

        case MU_COMMAND_CAMERA_OFF:
            MU_DisableAttractCamera(proc);
            break;

        } // switch (command)
    } // while (TRUE)
}

void MU_StartFogBumpFx(int x, int y) {
    struct APHandle* ap;
    struct MUFogBumpFxProc* proc;

    CopyDataWithPossibleUncomp(
        gUnknown_089ADD4C,
        OBJ_VRAM0 + 0x20 * 0x180
    );

    ap = AP_Create(gUnknown_089A8EF8, 2);

    ap->tileBase = 0x1180;
    AP_SwitchAnimation(ap, MU_FACING_LEFT);

    proc = (struct MUFogBumpFxProc*) Proc_Create(sProcScr_MUFogBumpFx, ROOT_PROC_3);

    proc->pAPHandle = ap;

    proc->xDisplay = x + 8;
    proc->yDisplay = y - 4;
}

static void MU_FogBumpFx_FirstFrame(struct MUFogBumpFxProc* proc) {
    PlaySoundEffect(0x77);

    proc->timer = 0;

    // TODO: maybe a macro that takes angle/xScale/yScale?

    WriteOAMRotScaleData(
        0,  // oam rotscale index

        Div(+COS(0) * 16, 0x200), // pa
        Div(-SIN(0) * 16, 0x200), // pb
        Div(+SIN(0) * 16, 0x200), // pc
        Div(+COS(0) * 16, 0x200)  // pd
    );
}

static void MU_FogBumpFx_TransitionInLoop(struct MUFogBumpFxProc* proc) {
    int scale;

    if (proc->timer++ >= 8)
        Proc_ClearNativeCallback((struct Proc*) proc);

    scale = sub_8012DCC(5, 0x200, 0x100, proc->timer, 8);

    WriteOAMRotScaleData(
        0,  // oam rotscale index

        Div(+COS(0) * 16, scale), // pa
        Div(-SIN(0) * 16, scale), // pb
        Div(+SIN(0) * 16, scale), // pc
        Div(+COS(0) * 16, scale)  // pd
    );

    AP_Update(
        proc->pAPHandle,

        (proc->xDisplay - 8),
        (proc->yDisplay - 8) | 0x300
    );
}

static void MU_FogBumpFx_DisplayLoop(struct MUFogBumpFxProc* proc) {
    if (proc->timer++ >= 40)
        Proc_ClearNativeCallback((struct Proc*) proc);

    AP_Update(
        proc->pAPHandle,

        (proc->xDisplay),
        (proc->yDisplay) | 0x100
    );
}

u8 MU_IsFogBumpFxActive(void) {
    return Proc_Find(sProcScr_MUFogBumpFx) ? TRUE : FALSE;
}

static void MU_State_WaitForFogBump(struct MUProc* proc) {
    if (!MU_IsFogBumpFxActive())
        proc->stateId = MU_STATE_WAITING;
}

static void MU_State_StartMovement(struct MUProc* proc) {
    proc->stateId = MU_STATE_MOVEMENT;
}

static void MU_State_WaitForTimer(struct MUProc* proc) {
    if (proc->moveTimer == 0)
        proc->stateId = MU_STATE_MOVEMENT;
    else
        proc->moveTimer--;
}

static void MU_State_None(struct MUProc* proc) {}

static void MU_State_NoOp(struct MUProc* proc) {}

static void MU_State_DuringMovement(struct MUProc* proc) {
    unsigned moveSpeed = MU_GetMovementSpeed(proc);

    proc->moveTimer = moveSpeed + proc->moveTimer;

    proc->xSubPosition += moveSpeed * sDirectionMoveOffsetLookup[proc->facingId * 2 + 0];
    proc->ySubPosition += moveSpeed * sDirectionMoveOffsetLookup[proc->facingId * 2 + 1];

    if ((proc->moveTimer / 16) >= 16) {
        proc->moveTimer -= 0x100;

        proc->xSubPosition -= proc->moveTimer * sDirectionMoveOffsetLookup[proc->facingId * 2 + 0];
        proc->ySubPosition -= proc->moveTimer * sDirectionMoveOffsetLookup[proc->facingId * 2 + 1];

        proc->moveTimer = 0;

        proc->xSubPosition &= ~0xF;
        proc->ySubPosition &= ~0xF;
    }

    if (proc->boolAttractCamera && !Proc_Find(gUnknown_0859A548)) {
        gUnknown_0202BCB0.camera.x = GetSomeAdjustedCameraX(proc->xSubPosition >> MU_SUBPIXEL_PRECISION);
        gUnknown_0202BCB0.camera.y = GetSomeAdjustedCameraY(proc->ySubPosition >> MU_SUBPIXEL_PRECISION);
    }

    if (!(proc->moveConfig & 0x80))
        MU_AdvanceStepSfx(proc);
}

static void MU_AdvanceStepSfx(struct MUProc* proc) {
    const u16* pStepSoundDefinition;

    unsigned cursor;
    struct Vec2 position;

    if (GetClassData(proc->displayedClassId)->attributes & CA_MOUNTEDAID) {
        switch (proc->displayedClassId) {

        case CLASS_WYVERN_RIDER:
        case CLASS_WYVERN_RIDER_F:
        case CLASS_WYVERN_LORD:
        case CLASS_WYVERN_LORD_F:
        case CLASS_WYVERN_KNIGHT:
        case CLASS_WYVERN_KNIGHT_F:
            pStepSoundDefinition = gMUSfxDef_Wyvern;
            break;

        case CLASS_MOGALL:
        case CLASS_ARCH_MOGALL:
            pStepSoundDefinition = gMUSfxDef_Mogall;
            break;

        case CLASS_PEGASUS_KNIGHT:
        case CLASS_FALCON_KNIGHT:
            pStepSoundDefinition = gMUSfxDef_Pegasus;
            break;

        default: // Any other mounted class
            pStepSoundDefinition = gMUSfxDef_Mounted;
            break;

        } // proc->displayedClassId
    } else {
        switch (proc->displayedClassId) {

        case CLASS_REVENANT:
        case CLASS_ENTOUMBED:
            pStepSoundDefinition = gMUSfxDef_Zombie;
            break;

        case CLASS_BONEWALKER:
        case CLASS_BONEWALKER_BOW:
        case CLASS_WIGHT:
        case CLASS_WIGHT_BOW:
            pStepSoundDefinition = gMUSfxDef_Skeleton;
            break;

        case CLASS_BAEL:
        case CLASS_ELDER_BAEL:
            pStepSoundDefinition = gMUSfxDef_Spider;
            break;

        case CLASS_MAUTHEDOOG:
        case CLASS_GWYLLGI:
            pStepSoundDefinition = gMUSfxDef_Dog;
            break;

        case CLASS_TARVOS:
        case CLASS_MAELDUIN:
            pStepSoundDefinition = gMUSfxDef_Mounted;
            break;

        case CLASS_MOGALL:
        case CLASS_ARCH_MOGALL:
            pStepSoundDefinition = gMUSfxDef_Mogall;
            break;

        case CLASS_GORGON:
            pStepSoundDefinition = gMUSfxDef_Gorgon;
            break;

        case CLASS_GARGOYLE:
        case CLASS_DEATHGOYLE:
            pStepSoundDefinition = gMUSfxDef_Wyvern;
            break;

        case CLASS_ARMOR_KNIGHT:
        case CLASS_ARMOR_KNIGHT_F:
        case CLASS_GENERAL:
        case CLASS_GENERAL_F:
        case CLASS_MANAKETE_2:
        case CLASS_CYCLOPS:
        case CLASS_DRACO_ZOMBIE:
        case CLASS_DEMON_KING:
        case CLASS_BLST_REGULAR_USED:
        case CLASS_BLST_LONG_USED:
        case CLASS_BLST_KILLER_USED:
            pStepSoundDefinition = gMUSfxDef_Heavy;
            break;

        case CLASS_FLEET:
            pStepSoundDefinition = gMUSfxDef_Boat;
            break;

        case CLASS_MYRRH:
            pStepSoundDefinition = gMUSfxDef_Myrrh;
            break;

        case CLASS_FALLEN_PRINCE:
        case CLASS_FALLEN_PEER:
            return; // no sounds

        default: // Any other non-mounted class
            pStepSoundDefinition = gMUSfxDef_Foot;
            break;

        } // switch (proc->displayedClassId)
    }

    cursor = DivRem(proc->stepSoundTimer++, pStepSoundDefinition[0]);
    MU_ComputeDisplayPosition(proc, &position);

    if (pStepSoundDefinition[2 + cursor]) {
        MU_StartStepSfx(
            pStepSoundDefinition[2 + cursor], // sound id
            pStepSoundDefinition[1], // something
            position.x // horizontal position
        );
    }
}

static void MU_OnLoop(struct MUProc* proc) {
    if (proc->stateId) {
        if (proc->moveTimer == 0)
            if ((u8)(proc->stateId - MU_STATE_MOVEMENT) <= (MU_STATE_WAITING - MU_STATE_MOVEMENT))
                MU_InterpretCommandScript(proc);

        sMUStateHandlerFuncLookup[proc->stateId](proc);
    }

    if (proc->facingId == MU_FACING_STANDING)
        MU_DisplayAsSMS(proc);
    else
        MU_DisplayAsMMS(proc);
}

static void MU_OnEnd(struct MUProc* proc) {
    proc->pMUConfig->muIndex = 0;
    AP_Delete(proc->pAPHandle);
}

void MU_EndAll(void) {
    Proc_DeleteAllWithScript(gProcScr_MoveUnit);
}

void MU_End(struct MUProc* proc) {
    MU_EndInternal(proc);
}

static void MU_EndInternal(struct MUProc* proc) {
    Proc_Delete((struct Proc*) proc);
}

static void MU_80790CC(struct MUProc* proc) {
    MU_8078894(proc);
    proc->stateId = MU_STATE_NONACTIVE;
}

void MU_AllDisable(void) {
    Proc_BlockEachWithMark(4);
}

void MU_AllEnable(void) {
    Proc_UnblockEachWithMark(4);
}

void MU_GetComputedEndPosition(int* xOut, int* yOut, const u8* commands) {
    while (TRUE) {
        switch (*commands++) {

        case MU_COMMAND_END:
            return;

        case MU_COMMAND_MOVE_LEFT:
            (*xOut)--;
            break;

        case MU_COMMAND_MOVE_RIGHT:
            (*xOut)++;
            break;

        case MU_COMMAND_MOVE_UP:
            (*yOut)--;
            break;

        case MU_COMMAND_MOVE_DOWN:
            (*yOut)++;
            break;

        case MU_COMMAND_HALT:
            return;

        case MU_COMMAND_WAIT:
            commands++;
            break;

        }
    }
}

int MU_CanStart(void) {
    int i;

    for (i = 0; i < MU_MAX_COUNT; ++i)
        if (sMUConfigArray[i].muIndex == 0)
            return TRUE;

    return FALSE;
}

void MU_AllRestartAnimations(void) {
    int i;

    for (i = 0; i < MU_MAX_COUNT; ++i) {
        if (sMUConfigArray[i].muIndex) {
            sMUConfigArray[i].pMUProc->pAPHandle->frameTimer    = 0;
            sMUConfigArray[i].pMUProc->pAPHandle->frameInterval = 0x100;
        }
    }
}

static struct MUConfig* MU_GenerateConfigDefault(int objTileId, u8* outIndex) {
    int i;

    for (i = 0; i < MU_MAX_COUNT; ++i) {
        if (sMUConfigArray[i].muIndex)
            continue;

        sMUConfigArray[i].muIndex = i + 1;
        sMUConfigArray[i].objTileIndex = sMUObjTileOffsetLookup_Default[i] + objTileId;

        *outIndex = i;

        return sMUConfigArray + i;
    }

    return NULL;
}

static struct MUConfig* MU_GenerateConfigOther(int objTileId, u8* outIndex) {
    int i;

    for (i = 0; i < MU_MAX_COUNT; ++i) {
        if (sMUConfigArray[i].muIndex)
            continue;

        sMUConfigArray[i].muIndex = i + 1;
        sMUConfigArray[i].objTileIndex = sMUObjTileOffsetLookup_Other[i] + objTileId;

        *outIndex = i;

        return sMUConfigArray + i;
    }

    return NULL;
}

u8 MU_ComputeDisplayPosition(struct MUProc* proc, struct Vec2* out) {
    if (proc->stateId == MU_STATE_UI_DISPLAY) {
        out->x = (proc->xSubPosition + proc->xSubOffset) >> MU_SUBPIXEL_PRECISION;
        out->y = (proc->ySubPosition + proc->ySubOffset) >> MU_SUBPIXEL_PRECISION;
    } else {
        short x = ((proc->xSubPosition + proc->xSubOffset) >> MU_SUBPIXEL_PRECISION) - gUnknown_0202BCB0.camera.x + 8;
        short y = ((proc->ySubPosition + proc->ySubOffset) >> MU_SUBPIXEL_PRECISION) - gUnknown_0202BCB0.camera.y + 8;

        out->x = x;
        out->y = y + 8;

        if ((u16)(x + 16) > 0x110)
            return FALSE;

        if (y < -0x10)
            return FALSE;

        if (y > 0xB0)
            return FALSE;
    }

    return TRUE;
}

static void MU_DisplayAsSMS(struct MUProc* proc) {
    if (!proc->boolIsHidden) {
        struct Vec2 position;

        if (!MU_ComputeDisplayPosition(proc, &position))
            return;

        position.x &= 0x01FF;
        position.y &= 0x00FF;

        if (proc->stateId == MU_STATE_DEATHFADE)
            position.y |= 0x400;

        sub_8026FF4(
            proc->muIndex,
            proc->pGfxVRAM
        );

        sub_8027DB4(
            proc->pAPHandle->objLayer,

            position.x - 8,
            position.y - 16,

            ((((unsigned)(proc->pGfxVRAM - OBJ_VRAM0) & 0x1FFFF) >> 5)
                | ((proc->pMUConfig->paletteIndex & 0xF) << 12))
                + proc->objPriorityBits,

            proc->displayedClassId,
            proc->muIndex
        );
    }
}

static void MU_DisplayAsMMS(struct MUProc* proc) {
    if (!proc->boolIsHidden) {
        struct Vec2 position;

        if (!MU_ComputeDisplayPosition(proc, &position))
            return;

        position.x &= 0x01FF;
        position.y &= 0x00FF;

        if (proc->stateId != MU_STATE_UI_DISPLAY)
            if (proc->pUnit && UNIT_FACTION(proc->pUnit) == FACTION_RED)
                if (gUnknown_0202BCF0.chapterVisionRange != 0)
                    if (!gBmMapFog[MU_GetDisplayYOrg(proc) >> 4][MU_GetDisplayXOrg(proc) >> 4])
                        return; // whew

        if (proc->stateId == MU_STATE_DEATHFADE)
            position.y |= 0x400;

        AP_Update(
            proc->pAPHandle,

            position.x,
            position.y
        );
    }
}

static u16 MU_GetMovementSpeed(struct MUProc* proc) {
    int config = proc->moveConfig;

    if (config & 0x80)
        config += 0x80; // I don't really get that one

    if (proc->boolForceMaxSpeed)
        return 0x100;

    if (config != 0x40) {
        if (config != 0x00) {
            int speed = config;

            if (speed & 0x40)
                speed ^= 0x40;
            else if (gUnknown_0202BCF0.unk40_8 || (gKeyStatusPtr->heldKeys & A_BUTTON))
                speed *= 4;

            if (speed > 0x80)
                speed = 0x80;

            return speed;
        }

        if (!IsFirstPlaythrough() && (gKeyStatusPtr->heldKeys & A_BUTTON))
            return 0x80;

        if (gUnknown_0202BCF0.unk40_8)
            return 0x40;
    }

    return 16 * sMUBaseMoveSpeedLookup[GetClassData(proc->displayedClassId)->slowWalking];
}

void MU_SetMoveConfig(struct MUProc* proc, u16 config) {
    if (config > 0x100)
        proc->moveConfig = 0x100;
    else
        proc->moveConfig = config;
}

static void* MU_GetGfxBufferById(int muIndex) {
    return gMUGfxBuffer + (sMUBufferIdLookup[muIndex] * MU_GFX_MAX_SIZE);
}

static const void* MU_GetSheetGfx(struct MUProc* proc) {
    return gMMSDataTable[proc->displayedClassId - 1].pGraphics;
}

static const void* MU_GetAnimationByClassId(u16 classId) {
    return gMMSDataTable[classId - 1].pAnimation;
}

void MU_StartDeathFade(struct MUProc* muProc) {
    struct MUEffectProc* proc;

    muProc->stateId = MU_STATE_DEATHFADE;

    proc = (struct MUEffectProc*) Proc_Create(gProcScr_MUDeathFade, (struct Proc*) muProc);

    proc->pMUProc = muProc;
    proc->timeLeft = 0x20;

    SetSpecialColorEffectsParameters(0, 0x10, 0x10, 0);

    muProc->pAPHandle->frameTimer = 0;
    muProc->pAPHandle->frameInterval = 0;

    MU_StartHitFlash(muProc, 0);

    muProc->pAPHandle->objLayer = 0xD;

    PlaySoundEffect(0xD6); // TODO: SOUND DEFINITIONS

    if (muProc->pUnit->state & US_IN_BALLISTA) {
        TryRemoveUnitFromBallista(muProc->pUnit);
        HideUnitSMS(muProc->pUnit);
    }
}

static void MU_DeathFade_OnLoop(struct MUEffectProc* proc) {
    short time = (proc->timeLeft--) >> 1;

    SetSpecialColorEffectsParameters(0, (u8) time, 0x10, 0);

    if (proc->timeLeft == 0) {
        MU_End(proc->pMUProc);
        Proc_ClearNativeCallback((struct Proc*) proc);
    }
}

static void MU_BlinkEffect_OnLoop(struct MUEffectProc* proc) {
    struct MUProc* muProc = (struct MUProc*) proc->parent;
    int boolHidden = FALSE;

    if ((proc->timeLeft & 0x7) < 4)
        boolHidden = TRUE;

    muProc->boolIsHidden = boolHidden;

    if (--proc->timeLeft < 0) {
        Proc_ClearNativeCallback((struct Proc*) proc);
        muProc->boolIsHidden = TRUE;
    }
}

void MU_StartBlinkEffect(struct MUProc* muProc) {
    struct MUEffectProc* proc;

    muProc->stateId = MU_STATE_DEATHFADE;

    proc = (struct MUEffectProc*) Proc_Create(gProcScr_MUBlinkEffect, (struct Proc*) muProc);

    proc->pMUProc = muProc;
    proc->timeLeft = 0x40;

    muProc->pAPHandle->frameTimer = 0;
    muProc->pAPHandle->frameInterval = 0;

    PlaySoundEffect(0xD6); // TODO: SOUND DEFINITIONS
}

static void MU_SetupPixelEffect(u32* data, int frame) {
    static u32 sKeptPixelsWordMask;
    static u32 sClearedPixelWordMask;

    int i, j;

    int pixel = sUnknown_089A2CA8[frame] % 8;
    int wordId = sUnknown_089A2CA8[frame] / 8;

    sKeptPixelsWordMask = ~(sClearedPixelWordMask = (0xF << (pixel * 4)));

    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            data[wordId] &= sKeptPixelsWordMask;

            data += 8;
        }

        data += 0xE0;
    }
}

static void MU_PixelEffect_OnLoop(struct MUEffectProc* proc) {
    MU_SetupPixelEffect(
        MU_GetGfxBufferById(((struct MUProc*)(proc->parent))->muIndex),
        proc->frameIndex
    );

    proc->frameIndex++;

    // TODO: FIXME: This may be bugged?
    RegisterTileGraphics(
        gMUGfxBuffer,
        OBJ_VRAM0 + (MU_BASE_OBJ_TILE * 0x20),
        (0x80 * 0x20)
    );

    if (--proc->timeLeft == 0) {
        MU_End(proc->pMUProc);
        Proc_ClearNativeCallback((struct Proc*) proc);
    }
}

void MU_StartPixelEffect(struct MUProc* muProc) {
    struct MUEffectProc* proc;

    muProc->stateId = MU_STATE_DEATHFADE;

    proc = (struct MUEffectProc*) Proc_Create(sProcScr_MUPixelEffect, (struct Proc*) muProc);

    proc->pMUProc = muProc;

    proc->timeLeft = 0x40;
    proc->frameIndex = 0;

    muProc->pAPHandle->frameTimer = 0;
    muProc->pAPHandle->frameInterval = 0;

    PlaySoundEffect(0xD6); // TODO: SOUND DEFINITIONS
}

void MU_Hide(struct MUProc* proc) {
    proc->boolIsHidden = TRUE;
}

void MU_Show(struct MUProc* proc) {
    proc->boolIsHidden = FALSE;
}

void MU_SetDisplayPosition(struct MUProc* proc, int x, int y) {
    proc->xSubPosition = x << MU_SUBPIXEL_PRECISION;
    proc->ySubPosition = y << MU_SUBPIXEL_PRECISION;
}

void MU_SetDisplayOffset(struct MUProc* proc, int xOff, int yOff) {
    proc->xSubOffset = xOff << MU_SUBPIXEL_PRECISION;
    proc->ySubOffset = yOff << MU_SUBPIXEL_PRECISION;
}

void MU_StartFlashFade(struct MUProc* proc, int flashType) {
    proc->pAPHandle->tileBase =
        proc->pMUConfig->objTileIndex + (MU_FADE_OBJ_PAL << 12) + proc->objPriorityBits;

    CopyToPaletteBuffer(
        gPaletteBuffer + (0x10 * (0x10 + proc->pMUConfig->paletteIndex)),
        (0x10 + MU_FADE_OBJ_PAL) * 0x20, 0x20
    );

    sub_8013928(
        sMUFlashColorLookup[flashType],
        0x15, 8, (struct Proc*) proc
    );
}

void MU_8079858(struct MUProc* muProc) {
    struct MUEffectProc* proc;

    sub_8013928(
        gPaletteBuffer + (0x10 * (0x10 + muProc->pMUConfig->paletteIndex)),
        0x15, 8, (struct Proc*) muProc
    );

    proc = (struct MUEffectProc*) Proc_Create(gProcScr_MU_89A2CF8, ROOT_PROC_3);

    proc->pMUProc = muProc;
}

static void MU_807988C(struct MUEffectProc* proc) {
    struct MUProc* muProc = proc->pMUProc;

    muProc->pAPHandle->tileBase =
        ((muProc->pMUConfig->paletteIndex & 0xF) << 12) + muProc->pMUConfig->objTileIndex + muProc->objPriorityBits;
}

void MU_StartActionAnim(struct MUProc* proc) {
    AP_SwitchAnimation(proc->pAPHandle, MU_FACING_SELECTED);

    proc->pAPHandle->frameTimer    = 0;
    proc->pAPHandle->frameInterval = 0x100;

    SetupFutureCall(MU_EndSelectionApAnim, (int) proc->pAPHandle, 30);
}

static void MU_EndSelectionApAnim(int argAp) {
    struct APHandle* ap = (struct APHandle*) argAp;

    ap->frameTimer    = 0;
    ap->frameInterval = 0;
}

void MU_StartDelayedFaceTarget(struct MUProc* proc) {
    proc->pAPHandle->frameTimer    = 0;
    proc->pAPHandle->frameInterval = 0x100;

    SetupFutureCall(MU_EndRefaceApAnim, (int) proc->pAPHandle, 30);
}

static void MU_EndRefaceApAnim(int argAp) {
    struct APHandle* ap = (struct APHandle*) argAp;

    int actorA = gMapBattle.attackerActorNum;
    int actorB = 1 - actorA;

    SetBattleAnimFacing(
        actorA, actorB,
        GetSpellAssocFacing(gMapBattle.actor[0].bu->weaponBefore)
    );

    ap->frameTimer    = 0;
    ap->frameInterval = 0;
}

void MU_StartFastMoveAnim(struct MUProc* proc) {
    proc->pAPHandle->frameTimer    = 0;
    proc->pAPHandle->frameInterval = 0x40;

    SetupFutureCall(MU_EndFasterApAnim, (int) proc->pAPHandle, 20);
}

static void MU_EndFasterApAnim(int argAp) {
    struct APHandle* ap = (struct APHandle*) argAp;

    ap->frameTimer    = 0;
    ap->frameInterval = 0;
}

void MU_StartCritFlash(struct MUProc* muProc, int flashType) {
    struct MUFlashEffectProc* proc;

    CopyToPaletteBuffer(
        sMUFlashColorLookup[flashType],
        (0x10 + MU_FADE_OBJ_PAL) * 0x20, 0x20
    );

    proc = (struct MUFlashEffectProc*) Proc_Create(sProcScr_MUCritFlash, (struct Proc*) muProc);

    proc->pMUProc = muProc;
}

static void MU_CritFlash_Init(struct MUFlashEffectProc* proc) {
    proc->timer = 0;
}

static void MU_CritFlash_SetFadedPalette(struct MUFlashEffectProc* proc) {
    struct MUProc* muProc = proc->pMUProc;

    muProc->pAPHandle->tileBase =
        (MU_FADE_OBJ_PAL << 12) + muProc->pMUConfig->objTileIndex + muProc->objPriorityBits;
}

static void MU_CritFlash_SetRegularPalette(struct MUFlashEffectProc* proc) {
    struct MUProc* muProc = proc->pMUProc;

    muProc->pAPHandle->tileBase =
        ((muProc->pMUConfig->paletteIndex & 0xF) << 12) + muProc->pMUConfig->objTileIndex + muProc->objPriorityBits;
}

static void MU_CritFlash_StartFadeBack_maybe(struct MUFlashEffectProc* proc) {
    sub_8013928(
        gPaletteBuffer + 0x10 * (0x10 + proc->pMUProc->pMUConfig->paletteIndex),
        0x15, 0x14, (struct Proc*) proc
    );
}

static void MU_CritFlash_SpriteShakeLoop(struct MUFlashEffectProc* proc) {
    proc->timer++;

    MU_SetDisplayOffset(proc->pMUProc, ((proc->timer & 1) ? 2 : -2), 0);

    if (proc->timer >= 12) {
        MU_SetDisplayOffset(proc->pMUProc, 0, 0);
        Proc_ClearNativeCallback((struct Proc*) proc);
    }
}

static void MU_CritFlash_RestorePalette(struct MUFlashEffectProc* proc) {
    struct MUProc* muProc = proc->pMUProc;

    muProc->pAPHandle->tileBase =
        ((muProc->pMUConfig->paletteIndex & 0xF) << 12) + muProc->pMUConfig->objTileIndex + muProc->objPriorityBits;
}

void MU_StartHitFlash(struct MUProc* muProc, int flashType) {
    struct MUFlashEffectProc* proc;

    CopyToPaletteBuffer(
        sMUFlashColorLookup[flashType],
        (0x10 + MU_FADE_OBJ_PAL) * 0x20, 0x20
    );

    muProc->pAPHandle->tileBase =
        (MU_FADE_OBJ_PAL << 12) + muProc->pMUConfig->objTileIndex + muProc->objPriorityBits;

    sub_8013928(
        gPaletteBuffer + 0x10 * (0x10 + muProc->pMUConfig->paletteIndex),
        0x15, 0x14, (struct Proc*) muProc
    );

    proc = (struct MUFlashEffectProc*) Proc_Create(sProcScr_MUHitFlash, (struct Proc*) muProc);

    proc->pMUProc = muProc;
}

static void MU_HitFlash_RestorePalette(struct MUFlashEffectProc* proc) {
    struct MUProc* muProc = proc->pMUProc;

    muProc->pAPHandle->tileBase =
        ((muProc->pMUConfig->paletteIndex & 0xF) << 12) + muProc->pMUConfig->objTileIndex + muProc->objPriorityBits;
}

void MU_AllForceSetMaxMoveSpeed(void) {
    Proc_ForEachWithScript(gProcScr_MoveUnit, MU_ForceSetMaxMoveSpeed);
}

static void MU_ForceSetMaxMoveSpeed(struct Proc* proc) {
    ((struct MUProc*)(proc))->boolForceMaxSpeed = TRUE;
}

void MU_SetSpecialSprite(struct MUProc* proc, int displayedClassId, const u16* palette) {
    proc->pAPHandle->frameTimer = 0;
    proc->pAPHandle->frameInterval = 0;

    proc->displayedClassId = displayedClassId;

    AP_SetDefinition(
        proc->pAPHandle,
        MU_GetAnimationByClassId(proc->displayedClassId)
    );

    CopyDataWithPossibleUncomp(
        MU_GetSheetGfx(proc),
        MU_GetGfxBufferById(proc->pMUConfig->muIndex)
    );

    CopyToPaletteBuffer(palette, (0x20 * (0x10 + proc->pMUConfig->paletteIndex)), 0x20);
}

void MU_SetPaletteId(struct MUProc* proc, unsigned paletteId) {
    proc->pMUConfig->paletteIndex = paletteId;

    proc->pAPHandle->tileBase =
        proc->pMUConfig->objTileIndex + ((paletteId % 0x10) << 12) + proc->objPriorityBits;
}

static struct MUProc* MU_GetByIndex(int muIndex) {
    if (!sMUConfigArray[muIndex].muIndex)
        return NULL;

    return sMUConfigArray[muIndex].pMUProc;
}

struct MUProc* MU_GetByUnit(struct Unit* unit) {
    int i;

    for (i = 0; i < MU_MAX_COUNT; ++i) {
        struct MUProc* proc = MU_GetByIndex(i);

        if (proc->pUnit == unit)
            return proc;
    }

    return NULL;
}

void MU_SortObjLayers(void) {
    struct MUProc* procs[MU_MAX_COUNT];

    s8 i, j;
    s8 count;

    // Clear proc list
    CpuFill32(0, procs, MU_MAX_COUNT * sizeof(struct MUProc*));
    count = 0;

    // Building proc list
    for (i = 0; i < MU_MAX_COUNT; ++i) {
        struct MUProc* proc = MU_GetByIndex(i);

        if (proc) {
            procs[count] = proc;
            count++;
        }
    }

    // Sorting proc list
    for (i = 0; i < (count - 1); ++i) {
        for (j = (i + 1); j < count; ++j) {
            if (procs[j]->ySubPosition < procs[i]->ySubPosition) {
                struct MUProc* tmp = procs[i];

                procs[i] = procs[j];
                procs[j] = tmp;
            }
        }
    }

    // Set obj layer based on order
    for (i = 0; i < count; ++i)
        procs[i]->pAPHandle->objLayer = 10 - i;
}
