#include "global.h"

#include "proc.h"
#include "fontgrp.h"
#include "uiutils.h"
#include "mu.h"
#include "bmio.h"
#include "bmunit.h"
#include "bmmap.h"
#include "uimenu.h"

#include "event.h"

// TODO: move those where they belong when possible
void sub_800E640(struct EventEngineProc*);

static bool8 EventEngine_CanStartSkip(struct EventEngineProc*);
static void  EventEngine_StartSkip(struct EventEngineProc*);
static void  sub_800D488(struct EventEngineProc*);
static void  CallNextQueuedEvent(void); // local

#define EVENT_SLOT_COUNT 0xE

struct EnqueuedEventCall {
    const u16* events;
    u8 execType;
    s8 isUsed;
};

extern unsigned gEventSlots[EVENT_SLOT_COUNT]; // event slot values (Null (0), Vars (1-A) + Position (B) + Check (C) + QP (D))
extern unsigned gEventSlotQueue[]; // event slot queue (just an array)

extern unsigned gEventSlotCounter;

// TODO: better
extern struct EnqueuedEventCall gEventCallQueue[]; // gEventCallQueue

extern struct ProcCmd gUnknown_030005B0[4];

extern const struct ProcCmd gProc_StdEventEngine[]; // map event engine proc
extern const struct ProcCmd gProc_BattleEventEngine[]; // battle (?) event engine proc

extern const EventFuncType gEventLoCmdTable[]; // regular event functions
extern const EventFuncType gEventHiCmdTable[]; // gmap event functions

extern const struct ProcCmd gUnknown_08591DD8[]; // map event engine "witness lock" (alive while map event engine is)

// TODO: actual events

extern const u16 gEvent_DisplayBattleQuote[]; /*
    1020 0003 | EVBIT_MODIFY 3  // modifies event state bits (how?)
    1B20 FFFF | TEXTSHOW 0xFFFF // Show text in event slot 2
    1D20 0000 | TEXTEND         // Ends text
    1B22 0000 | REMA            // deletes all text-related procs (or something)
    0120 0000 | ENDA            // ends event execution
*/

extern const u16 gEvent_TriggerQueuedTileChanges[]; /*
    1020 0001 | EVBIT_MODIFY 1    // modifies event state bits (how?)
    2720 FFFD | TILECHANGE 0xFFFD // Changes tile by tile change id in event slot Queue (or something)
    0120 0000 | ENDA              // ends event execution
*/

extern const u16 gEvent_OpenChest[]; /*
    1020 0001           | EVBIT_MODIFY 1
    2720 FFFD           | TILECHANGE 0xFFFD
    0540 0007 000000FF  | SVAL s7 0xFF
    0C43 0000 0003 0007 | BGT 0 s3 s7
    3720 FFFF           | GIVEITEMTO 0xFFFF
    0920 0001           | GOTO 1
    0820 0000           | LABEL 0
    3721 FFFF           | GIVEITEMTOMAIN 0xFFFF
    0820 0001           | LABEL 1
    0120 0000           | ENDA
*/

extern const u16 gEvent_MapSupportConversation[]; /*
    1020 0003           | EVBIT_MODIFY 3
    0C40 0000 0002 0000 | BEQ 0 s2 s0
    1220 FFFF           | MUSC 0xFFFF
    0920 0001           | GOTO 1
    0820 0000           | LABEL 0
    1520 0000           | MUSI
    0820 0001           | LABEL 1
    0620 0032           | SADD $032
    1B20 FFFF           | TEXTSHOW 0xFFFF
    1D20 0000           | TEXTEND
    1B22 0000           | REMA
    3A40 000C 005A 0000 | NOTIFY 0xC 0x5A 0
    0228 0007           | EVBIT_T 7 // NoFade
    0120 0000           | ENDA
*/

extern const u16 gEvent_SupportViewerConversation[]; /*
    1020 0003           | EVBIT_MODIFY 3
    1A21 0000           | REMOVEPORTRAITS
    2140 0037 0000 0000 | BACG 0x37
    1720 0010           | FADU 16
    1B20 FFFF           | TEXTSHOW 0xFFFF
    1D20 0000           | TEXTEND
    1B22 0000           | REMA
    1721 0010           | FADI 16
    0120 0000           | ENDA
*/

extern const u16 gEvent_SkirmishRetreat[]; /*
    EVBIT_MODIFY 4
    TUTORIALTEXTBOXSTART
    SVAL sB (-1)
    TEXTSHOW 0x8FC // "Retreat?        [NL][No][X]"
    TEXTEND
    SVAL s7 TRUE
    BNE 0 sC s7
    MUSCMID 0x7FFF
    FADI 4
    MNCH 0xFFFF
    CHECK_SKIRMISH
    SVAL s1 1
    BNE 0 sC s1
    ASMC 0x37D59
    LABEL 0
    REMA
    EVBIT_T 7 // NoFade
    ENDA
*/

extern const u16 gEvent_SuspendPrompt[]; /*
    EVBIT_MODIFY 4
    TEXTSTART
    TEXTSHOW 0x8FE
    TEXTEND
    SVAL s7 TRUE
    BNE 0 sC s7
    ASMC 0xB5D5D // Save Suspended game
    TEXTSHOW2 0x8FF
    TEXTEND
    MUSCMID 0x7FFF
    FADI 4
    MNTS 0
    LABEL 0
    REMA
    EVBIT_T 7
    ENDA
*/

extern const u16 gEvent_GameOver[]; /* Game Over Events?
    1020 0004           | EVBIT_MODIFY 4
    0D40 0000 08085375  | ASMC sub_8085374 // game over
    0120 0000           | ENDA
*/

extern const struct ProcCmd gUnknown_08591540[]; // extern

void _MarkSomethingInMenu(void) {
    FreezeMenu();
}

void EventEngine_OnUpdate(struct EventEngineProc* proc) {
    if (DoesBMXFADEExist())
        return;
    
    if (EventEngine_CanStartSkip(proc) && (gKeyStatusPtr->newKeys & START_BUTTON)) {
        EventEngine_StartSkip(proc);
        return;
    }

    if (proc->execType != EV_EXEC_WORLDMAP && proc->execType != EV_EXEC_UNK4) {
        gLCDControlBuffer.dispcnt.bg0_on = TRUE;
        gLCDControlBuffer.dispcnt.bg1_on = TRUE;
        gLCDControlBuffer.dispcnt.bg2_on = TRUE;
        gLCDControlBuffer.dispcnt.bg3_on = TRUE;
        gLCDControlBuffer.dispcnt.obj_on = TRUE;
    }

    if (proc->pCallback) {
        proc->pCallback(proc);
        return;
    }

    proc->evStateBits |= EV_STATE_0002;

    while (TRUE) {
        unsigned evCode;
        EventFuncType evFunc;

        // Event Slot 0
        gEventSlots[0] = 0;

        evCode = (*proc->pEventCurrent) >> 8;
        evFunc = (evCode < 0x80) ? gEventLoCmdTable[evCode] : gEventHiCmdTable[evCode - 0x80];

        switch (evFunc(proc)) {
        case 0:
            proc->pEventCurrent += ((*proc->pEventCurrent) >> 4)&0xF;
        case 1:
        case 4:
        case 6:
            break;
        
        case 2:
            proc->pEventCurrent += ((*proc->pEventCurrent) >> 4)&0xF;
        case 3:
            return;
        
        case 5:
            Proc_ClearNativeCallback((struct Proc*)(proc));
            return;
        }
    }
}

void EventEngine_OnEnd(struct EventEngineProc* proc) {
    SetFont(NULL);
    Font_LoadForUI();
    LoadUiFrameGraphics();

    switch (proc->execType) {
    case EV_EXEC_UNK4:
        break;
    
    case EV_EXEC_UNK5:
        LoadGameCoreGfx();
        UnpackChapterMapPalette();
        sub_800BCDC(proc->mapSpritePalIdOverride);

        if (proc->evStateBits & EV_STATE_CHANGEGM) {
            MU_EndAll();
            EndBMapMain();
            memset((u8*)(gEventCallQueue), 0, 0x80);
        }

    case EV_EXEC_GAMEPLAY:
        SubSkipThread2();
        ResumeMenu();
        sub_800BB98();
        ClearCutsceneUnits();

        break;
    }

    if (proc->execType != EV_EXEC_QUIET) {
        sub_8006A7C(); // EndDialogueInterpreter
        sub_808F270(); // End some thing
        sub_808BB74(); // End some more things

        if (proc->execType == EV_EXEC_CUTSCENE)
            sub_800BCDC(proc->mapSpritePalIdOverride);
        
        sub_800E640(proc);
    }

    if (!(proc->evStateBits & EV_STATE_ABORT))
        CallNextQueuedEvent();
}

void EnqueueEventCall(const u16* events, u8 execType) {
    struct EnqueuedEventCall* it;
    u8 i;

    it = gEventCallQueue;

    for (i = -1; ++i <= 0xf;) {
        if (!it->isUsed) {
            it->events   = events;
            it->execType = execType;
            it->isUsed   = TRUE;

            break;
        }

        ++it;
    }
}

void CallNextQueuedEvent(void) {
    struct EnqueuedEventCall* it;
    u8 i;

    it = gEventCallQueue;

    if (it->isUsed == 1)
        EventEngine_Create(it->events, it->execType);
    
    for (i = 0; i < 15; ++i, ++it)
        *it = *(it + 1);
    
    it->events   = NULL;
    it->execType = 0;
    it->isUsed   = 0;
}

void CallEvent(const u16* events, u8 execType) {
    bool8 found = Proc_Find(gProc_StdEventEngine) != 0;

    if (found)
        EnqueueEventCall(events, execType);
    else
        EventEngine_Create(events, execType);
}

struct EventEngineProc* EventEngine_Create(const u16* events, u8 execType) {
    struct EventEngineProc* proc;

    proc = (struct EventEngineProc*) Proc_Create(gProc_StdEventEngine, ROOT_PROC_3);

    proc->pCallback      = NULL;

    proc->pEventStart    = events;
    proc->pEventIdk      = events;
    proc->pEventCurrent  = events;

    proc->evStateBits    = EV_STATE_NONE;
    proc->evStallTimer   = 0;

    proc->overwrittenTextSpeed = -1;
    proc->execType       = execType;
    proc->activeTextType = 0;
    proc->chapterIndex   = -1;

    proc->mapSpritePalIdOverride = 0x000;

    proc->pUnitLoadData  = NULL;
    proc->unitLoadCount  = 0;
    proc->idk4E          = 0;

    if (gLCDControlBuffer.blendY == 0x10)
        proc->evStateBits |= EV_STATE_FADEDIN;

    switch (execType) {
    case EV_EXEC_WORLDMAP:
        break;

    case EV_EXEC_CUTSCENE:
    case EV_EXEC_GAMEPLAY:
        proc->chapterIndex = gUnknown_0202BCF0.chapterIndex;
        AddSkipThread2();
        break;
    }

    return proc;
}

void EventEngine_CreateBattle(const u16* events) {
    struct EventEngineProc* proc;

    proc = (struct EventEngineProc*) Proc_Create(gProc_BattleEventEngine, ROOT_PROC_3);

    proc->pCallback     = NULL;

    proc->pEventStart    = events;
    proc->pEventIdk      = events;
    proc->pEventCurrent  = events;

    proc->evStateBits    = EV_STATE_NONE;
    proc->evStallTimer   = 0;

    proc->overwrittenTextSpeed = -1;
    proc->execType       = EV_EXEC_QUIET;
    proc->activeTextType = 0;
    
    proc->mapSpritePalIdOverride = 0x000;

    proc->pUnitLoadData  = NULL;
    proc->unitLoadCount  = 0;
    proc->idk4E          = 0;

    proc->evStateBits |= EV_STATE_ABORT;
}

s8 EventEngineExists(void) {
    return Proc_Find(gProc_StdEventEngine) ? TRUE : FALSE;
}

s8 BattleEventEngineExists(void) {
    return Proc_Find(gProc_BattleEventEngine) ? TRUE : FALSE;
}

void DeleteEventEngines(void) {
    Proc_DeleteAllWithScript(gProc_StdEventEngine);
    Proc_DeleteAllWithScript(gProc_BattleEventEngine);
}

void sub_800D1E4(struct Proc* proc) {
    Proc_CreateBlockingChild(gUnknown_08591DD8, proc);
}

void SetEventSlotC(unsigned value) {
    gEventSlots[0xC] = value;
}

void sub_800D204(void) {} // nullsub

int sub_800D208(void) {
    struct EventEngineProc* proc;

    if (!(proc = (struct EventEngineProc*) Proc_Find(gProc_StdEventEngine)))
        if (!(proc = (struct EventEngineProc*) Proc_Find(gProc_BattleEventEngine)))
            return FALSE;
    
    switch (proc->activeTextType) {

    case 0:
        return FALSE;

    case 1:
        return TRUE;

    case 2:
        return TRUE;

    case 3:
        return FALSE;

    case 4:
        return FALSE;

    case 5:
        return FALSE;

    }

    return FALSE;
}

void CallBattleQuoteEvent(u16 textIndex) {
    // Battle quote (unused?)
    CallEvent(gEvent_DisplayBattleQuote, EV_EXEC_GAMEPLAY);

    gEventSlots[0x02] = textIndex;
}

void CallBattleQuoteEventInBattle(u16 textIndex) {
    // Battle quote
    EventEngine_CreateBattle(gEvent_DisplayBattleQuote);

    gEventSlots[0x2] = textIndex;
}

void CallTileChangeEvent(u16 tileChangeIndex) {
    // Generic tile change events?
    CallEvent(gEvent_TriggerQueuedTileChanges, EV_EXEC_GAMEPLAY);

    gEventSlots[0xD] = 1; // qp
    gEventSlotQueue[0]   = tileChangeIndex;
}

void CallChestOpeningEvent(u16 tileChangeIndex, u16 idr) {
    // Chest opening events?
    CallEvent(gEvent_OpenChest, EV_EXEC_GAMEPLAY);

    gEventSlots[0xD] = 1; // qp
    gEventSlotQueue[0]   = tileChangeIndex;
    gEventSlots[0x3] = idr;
}

void CallMapSupportEvent(u16 musicIndex, u16 textIndex) {
    // Calls text with music (just quiets music when id is 0)
    // On-map supports?
    CallEvent(gEvent_MapSupportConversation, EV_EXEC_CUTSCENE);

    gEventSlots[0x2] = musicIndex;
    gEventSlots[0x3] = textIndex;
}

void CallSupportViewerEvent(u16 textIndex) {
    // Calls text with random background (support viewer?)
    CallEvent(gEvent_SupportViewerConversation, EV_EXEC_QUIET);

    gEventSlots[0x2] = textIndex;
}

void CallRetreatPromptEvent(void) {
    // Calls Retreat events
    CallEvent(gEvent_SkirmishRetreat, EV_EXEC_CUTSCENE);
    
    gEventSlots[0x2] = gUnknown_0202BCF0.chapterIndex;
}

void CallSuspendPromptEvent(void) {
    // Calls Suspend events
    CallEvent(gEvent_SuspendPrompt, EV_EXEC_CUTSCENE);
}

void CallGameOverEvent(void) {
    // Calls Game Over
    EventEngine_Create(gEvent_GameOver, EV_EXEC_GAMEPLAY);
}

bool8 EventEngine_CanStartSkip(struct EventEngineProc* proc) { // Events_CanSkip
    if (!(proc->evStateBits & EV_STATE_0002))
        return FALSE;

    // Why is this check so different??
    if (((proc->evStateBits >> 2) & 1)) // & EV_STATE_SKIPPING
        return FALSE;

    if ((proc->evStateBits & EV_STATE_NOSKIP))
        return FALSE;

    if (IsBattleDeamonActive())
        return FALSE;

    if (Proc_Find(gProcScr_MUDeathFade))
        return FALSE;

    return TRUE;
}

void sub_800D3E4(void) {
    struct EventEngineProc* proc;

    if (!(proc = (struct EventEngineProc*) Proc_Find(gProc_StdEventEngine)))
        if (!(proc = (struct EventEngineProc*) Proc_Find(gProc_BattleEventEngine)))
            return;
    
    proc->evStateBits |= EV_STATE_0008;
}

void EventEngine_StartSkip(struct EventEngineProc* proc) {
    proc->evStateBits |= EV_STATE_SKIPPING;

    if (!GetZero()) {
        if (sub_80BA054() == TRUE) // World Map check
            sub_800D488(proc);
        else if (!(proc->evStateBits & EV_STATE_NOFADE)) {
            if (proc->evStateBits & EV_STATE_FADEDIN)
                sub_800D488(proc);
            else
                sub_8013D08(0x40, (struct Proc*)(proc));
            
            proc->evStateBits |= EV_STATE_FADEDIN;
        }
    }

    if (proc->execType == EV_EXEC_WORLDMAP)
        sub_80BA424();
    
    Proc_BlockEachWithMark(5);
}

void sub_800D488(struct EventEngineProc* unused) {
    sub_80141B0(); // disables layers
    Proc_DeleteAllWithScript(gUnknown_08591540);
}

void SetEventTriggerState(u16 triggerId, bool8 value) {
    if (!value)
        UnsetEventId(triggerId);
    else
        SetEventId(triggerId);
}

int GetEventTriggerState(u16 triggerId) {
    if (!CheckEventId(triggerId))
        return FALSE;
    return TRUE;
}

struct Proc* sub_800D4D4(struct Proc* parent, void(*init)(struct Proc*), void(*loop)(struct Proc*), void(*dest)(struct Proc*)) {
    struct ProcCmd code[] = {
        PROC_SET_DESTRUCTOR(dest),
        PROC_CALL_ROUTINE(init),
        PROC_LOOP_ROUTINE(loop),
        PROC_END
    };
    
    memcpy(gUnknown_030005B0, code, sizeof code);
    return Proc_Create(gUnknown_030005B0, parent);
}

void sub_800D524(void) {} // nullsub

void SlotQueuePush(unsigned value) {
    gEventSlotQueue[gEventSlots[0xD]] = value;
    gEventSlots[0xD]++;
}

unsigned SlotQueuePop(void) {
    s16 i;
    unsigned result;
    
    result = gEventSlotQueue[0];

    gEventSlots[0xD]--;

    for (i = 0; i < gEventSlots[0xD]; ++i)
        gEventSlotQueue[i] = gEventSlotQueue[i+1];

    return result;
}

void SetEventSlotCounter(unsigned value) {
    gEventSlotCounter = value;
}

unsigned GetEventSlotCounter(void) {
    return gEventSlotCounter;
}
