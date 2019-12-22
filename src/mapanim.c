#include "global.h"

#include "m4a.h"
#include "soundwrapper.h"
#include "hardware.h"
#include "fontgrp.h"
#include "proc.h"
#include "event.h"
#include "bmio.h"
#include "bmitem.h"
#include "bmunit.h"
#include "bmbattle.h"
#include "mu.h"
#include "uiutils.h"

#include "constants/characters.h"
#include "constants/classes.h"
#include "constants/items.h"
#include "constants/terrains.h"

struct MapAnimProc
{
    /* 00 */ PROC_HEADER;
};

struct MAExpBarProc
{
    /* 00 */ PROC_HEADER;

    /* 29 */ u8    pad29[0x64 - 0x29];
    /* 64 */ short expFrom;
    /* 66 */ short expTo;
    /* 68 */ short actorId;
    /* 6A */ short unk6A;
};

void sub_8011694(int item, struct Proc* parent);
void NewPopup_WeaponBroke(int item, struct Proc* parent);
void NewPopup_WRankIncrease(int wtype, struct Proc* parent);

s8 BattleUnit_ShouldDisplayWpnBroke(struct BattleUnit* bu);
s8 BattleUnit_ShouldDisplayWRankUp(struct BattleUnit* bu);

void NewMapPoisonEffect(struct Unit* unit);
void NewBlockingTimer(struct Proc* proc, int duration);

const struct ProcCmd* GetSpellAssocAlt6CPointer(int item);
const struct ProcCmd* GetItemAnim6CCode(void);

s8 sub_80835A8(int charid);
void sub_80835DC(int charid);

void DeleteBattleAnimInfoThing(void);

s8 GetSpellAssocReturnBool(int item);
void NewMapBattleInfoThing(int x, int y, struct Proc* parent);

void CallBattleQuoteEventsIfAny(int charA, int charB);

void New6C_SummonGfx(struct Proc* parent, int xMap, int yMap);

extern const struct ProcCmd gUnknown_089A35B0[];
extern const struct ProcCmd gUnknown_089A36F8[];

extern const u16 gUnknown_089A8F74[];

void sub_807A708(struct Proc* proc)
{
    if (gMapBattle.u62 == 1)
        sub_8011694(gMapBattle.actor[1].bu->weapon, (struct Proc*)(proc));
}

void DisplayWpnBrokePopup(struct Proc* proc)
{
    struct BattleUnit* bu = NULL;

    if (BattleUnit_ShouldDisplayWpnBroke(&gBattleActor))
        bu = &gBattleActor;

    if (BattleUnit_ShouldDisplayWpnBroke(&gBattleTarget))
        bu = &gBattleTarget;

    if (bu)
        NewPopup_WeaponBroke(bu->weaponBefore, proc);
}

s8 BattleUnit_ShouldDisplayWpnBroke(struct BattleUnit* bu)
{
    if (UNIT_FACTION(&bu->unit) == FACTION_BLUE)
        return DidBattleUnitBreakWeapon(bu);

    return FALSE;
}

void DisplayWRankUpPopup(struct Proc* proc)
{
    struct BattleUnit* bu = NULL;

    if (BattleUnit_ShouldDisplayWRankUp(&gBattleActor))
        bu = &gBattleActor;

    if (BattleUnit_ShouldDisplayWRankUp(&gBattleTarget))
        bu = &gBattleTarget;

    if (bu)
        NewPopup_WRankIncrease(bu->weaponType, proc);
}

s8 BattleUnit_ShouldDisplayWRankUp(struct BattleUnit* bu)
{
    if (UNIT_FACTION(&bu->unit) != FACTION_BLUE)
        return FALSE;

    if (!HasBattleUnitGainedWeaponLevel(bu))
        return FALSE;

    return TRUE;
}

void _InitFontForUIDefault(struct Proc* proc)
{
    Font_InitForUIDefault();
}

void DeleteBattleAnimInfoThing(void);

void MapAnim_Cleanup(void)
{
    MU_AllRestartAnimations();
    sub_8003D20();
    DeleteBattleAnimInfoThing();
    SetupBackgroundForWeatherMaybe();
    LoadUiFrameGraphics();
    LoadObjUIGfx();

    if (EventEngineExists())
        MU_EndAll();
}

void MapAnim_AdvanceBattleRound(void)
{
    gMapBattle.attackerActorNum = !!(gMapBattle.hitIt->info & BATTLE_HIT_INFO_RETALIATION);
    gMapBattle.defenderActorNum = 1 - gMapBattle.attackerActorNum;

    gMapBattle.hitAttributes = gMapBattle.hitIt->attributes;
    gMapBattle.hitInfo = gMapBattle.hitIt->info;
    gMapBattle.hitDamage = gMapBattle.hitIt->hpChange;

    if (gMapBattle.actorCount_maybe == 1)
    {
        gMapBattle.attackerActorNum = 0;
        gMapBattle.defenderActorNum = 0;
    }

    gMapBattle.hitIt++;
}

void MapAnim_PrepareNextBattleRound(struct Proc* proc)
{
    if (gMapBattle.hitIt->info & BATTLE_HIT_INFO_END)
    {
        Proc_ClearNativeCallback(proc);
        Proc_JumpToPointer(proc, gUnknown_089A35B0);

        return;
    }

    MapAnim_AdvanceBattleRound();

    gMapBattle.specialProcScr = GetSpellAssocAlt6CPointer(
        gMapBattle.actor[gMapBattle.attackerActorNum].bu->weaponBefore);

    Proc_ClearNativeCallback(proc);
}

void MapAnim_DisplayRoundAnim(struct Proc* proc)
{
    Proc_CreateBlockingChild(GetItemAnim6CCode(), proc);
}

void MapAnim_ShowPoisonEffectIfAny(struct Proc* proc)
{
    if (gMapBattle.hitAttributes & BATTLE_HIT_ATTR_POISON)
    {
        NewMapPoisonEffect(gMapBattle.actor[gMapBattle.defenderActorNum].unit);
        NewBlockingTimer(proc, 100);
    }
}

void MapAnim_MoveCameraOntoSubject(struct Proc* proc)
{
    EnsureCameraOntoPosition(
        proc, gMapBattle.actor[0].unit->xPos, gMapBattle.actor[0].unit->yPos);
}

void MapAnim_MoveCameraOntoTarget(struct Proc* proc)
{
    if (gMapBattle.actorCount_maybe != 1)
    {
        EnsureCameraOntoPosition(
            proc, gMapBattle.actor[1].unit->xPos, gMapBattle.actor[1].unit->yPos);
    }
}

void sub_807A984(void)
{
    int actorNum = -1;

    switch (gMapBattle.actorCount_maybe)
    {

    case 2:
        if (gMapBattle.actor[1].u0D == 0)
            actorNum = 1;

        // fallthrough

    case 1:
        if (gMapBattle.actor[0].u0D == 0)
            actorNum = 0;

        break;

    } // switch (gMapBattle.actorCount_maybe)

    if (actorNum != -1)
    {
        int charid = UNIT_CHAR_ID(gMapBattle.actor[actorNum].unit);

        switch (charid)
        {

        case CHARACTER_EIRIKA:
        case CHARACTER_EPHRAIM:
            if (CheckEventId(0x65)) // TODO: flag constants
                UnsetEventId(0x65); // TODO: flag constants

            break;

        }

        if (sub_80835A8(charid))
        {
            DeleteBattleAnimInfoThing();
            sub_80835DC(charid);
        }
    }
}

void sub_807AA00(void)
{
    int actorNum = -1;

    switch (gMapBattle.actorCount_maybe)
    {

    case 2:
        if (gMapBattle.actor[1].u0D == 0)
            actorNum = 1;

        // fallthrough

    case 1:
        if (gMapBattle.actor[0].u0D == 0)
            actorNum = 0;

        break;

    } // switch (gMapBattle.actorCount_maybe)

    if (actorNum != -1)
        MU_StartDeathFade(gMapBattle.actor[actorNum].mu);
}

void sub_807AA4C(struct Proc* proc)
{
    int actorNum = -1;

    switch (gMapBattle.actorCount_maybe)
    {

    case 2:
        if (gMapBattle.actor[1].bu->expGain != 0)
            actorNum = 1;

        // fallthrough

    case 1:
        if (gMapBattle.actor[0].bu->expGain != 0)
            actorNum = 0;

        break;

    } // switch (gMapBattle.actorCount_maybe)

    if (actorNum >= 0)
    {
        struct MAExpBarProc* expProc = (struct MAExpBarProc*) Proc_CreateBlockingChild(gUnknown_089A36F8, proc);

        expProc->expFrom = gMapBattle.actor[actorNum].bu->expPrevious;
        expProc->expTo   = gMapBattle.actor[actorNum].bu->expPrevious + gMapBattle.actor[actorNum].bu->expGain;
        expProc->actorId = actorNum;
    }
}

void sub_807AADC(struct Proc* proc)
{
    SetDefaultColorEffects();

    switch (gMapBattle.u62)
    {
    case 1:
    case 2:
        return;

    default:
        break;
    }

    if (GetSpellAssocReturnBool(gMapBattle.actor[0].bu->weaponBefore))
    {
        int y;

        if (gMapBattle.actorCount_maybe == 1)
        {
            y = gMapBattle.actor[0].unit->yPos*16 - gUnknown_0202BCB0.camera.y;

            if (y >= 112)
                y = y - 40;
            else
                y = y + 24;
        }
        else
        {
            int array[2];
            int i, actorNum;

            for (i = 0; i < gMapBattle.actorCount_maybe; ++i)
                array[i] = gMapBattle.actor[i].unit->yPos*16 - gUnknown_0202BCB0.camera.y;

            if (array[0] - array[1] >= 0)
            {
                if (array[0] - array[1] >= 80)
                    goto disp_center;
            }
            else if (array[1] - array[0] >= 80)
            {
            disp_center:
                y = 64;
                goto disp;
            }

            actorNum = array[0] > array[1] ? 0 : 1;

            if (array[actorNum] >= 112)
                y = array[1 - actorNum] - 40;
            else
                y = array[actorNum] + 24;
        }

    disp:
        NewMapBattleInfoThing(15, y / 8, proc);
    }
}

void MapAnim_CallBattleQuoteEvents(void)
{
    if (gMapBattle.actorCount_maybe == 2)
    {
        CallBattleQuoteEventsIfAny(
            UNIT_CHAR_ID(gMapBattle.actor[0].unit), UNIT_CHAR_ID(gMapBattle.actor[1].unit));
    }
}

void sub_807ABE0(int actorNum)
{
    if (UNIT_FACTION(gMapBattle.actor[0].unit) == UNIT_FACTION(gMapBattle.actor[1].unit))
        if (!BUNIT_IS_OBSTACLE(gMapBattle.actor[0].bu) && !BUNIT_IS_OBSTACLE(gMapBattle.actor[1].bu))
            return;

    // Check actor class

    if (UNIT_CLASS_ID(gMapBattle.actor[actorNum].unit) != CLASS_MYRRH)
        return;

    // Check actor status

    switch (gMapBattle.actor[actorNum].unit->statusIndex)
    {

    case UNIT_STATUS_SLEEP:
    case UNIT_STATUS_PETRIFY:
    case UNIT_STATUS_13:
        return;

    } // switch (gMapBattle.actor[actorNum].unit->statusIndex)

    // Check other actor weapon

    switch (GetItemIndex(gMapBattle.actor[actorNum ^ 1 ].bu->weaponBefore))
    {

    case ITEM_STAFF_SILENCE:
    case ITEM_STAFF_SLEEP:
    case ITEM_STAFF_BERSERK:
    case ITEM_MONSTER_STONE:
        return;

    } // switch (GetItemIndex(gMapBattle.actor[actorNum ^ 1 ].bu->weaponBefore))

    // Check actor weapon

    if (GetItemIndex(gMapBattle.actor[actorNum].bu->weaponBefore) != ITEM_DIVINESTONE)
        return;

    MU_SetPaletteId(gMapBattle.actor[actorNum].mu, BM_OBJPAL_BANIM_SPECIALMU + actorNum);
    MU_SetSpecialSprite(gMapBattle.actor[actorNum].mu, CLASS_MYRRH_TRANSFORMED, gUnknown_089A8F74);
}

void sub_807ACAC(void)
{
    switch (gMapBattle.actorCount_maybe)
    {

    case 2:
        sub_807ABE0(1);
        // fallthrough
    case 1:
        sub_807ABE0(0);
        break;

    } // switch (gMapBattle.actorCount_maybe)
}

void PlaySoundIdA0(void)
{
    PlaySoundEffect(0xA0); // TODO: song ids!
}

void sub_807ACEC(void)
{
    PlaySoundEffect(0xA0); // TODO: song ids!
}

// ==============================
// NOTE: this may be a file split
// ==============================

#include "rng.h"
#include "bmmap.h"

extern u8 gUnknown_0895F5A4[][2];
extern struct UnitDefinition gUnknown_03001C38;

extern const struct UnitDefinition gUnknown_088D1F54[];
extern struct UnitDefinition gUnknown_03001C50;

void New6C_SummonGfx_FromActionPos(struct Proc* proc)
{
    New6C_SummonGfx(proc, gActionData.xOther, gActionData.yOther);
}

void sub_807AD1C(void)
{
    u8 rand100 = DivRem(AdvanceGetLCGRNValue(), 101);

    struct Unit* unit;
    short summonerNum, i;

    // 1. Find summoner number from active unit

    summonerNum = -1;

    for (i = 0; i < 3; ++i) // TODO: sizeof(gUnknown_0895F5A4) / 2
    {
        if (UNIT_CHAR_ID(gActiveUnit) == gUnknown_0895F5A4[i][0])
        {
            summonerNum = i;
            break;
        }
    }

    if (summonerNum == -1)
        return;

    // 2. Clear existing summon

    // NOTE: this may have been a macro? (because of different i and unit?)
    {
        int i;

        for (i = 1; i < 0x40; ++i)
        {
            struct Unit* unit = GetUnit(i);

            if (UNIT_IS_VALID(unit))
            {
                if (UNIT_CHAR_ID(unit) == gUnknown_0895F5A4[summonerNum][1])
                    ClearUnit(unit);
            }
        }
    }

    // 3. Set up unit definition

    unit = NULL;

    // 3.1. Character/Class/Faction/Level/Position

    gUnknown_03001C38.charIndex       = gUnknown_0895F5A4[summonerNum][1];
    gUnknown_03001C38.classIndex      = CLASS_PHANTOM;
    gUnknown_03001C38.leaderCharIndex = CHARACTER_NONE;
    gUnknown_03001C38.autolevel       = TRUE;

    if (UNIT_FACTION(gActiveUnit) == FACTION_BLUE)
        gUnknown_03001C38.allegiance = 0;

    else if (UNIT_FACTION(gActiveUnit) == FACTION_RED)
        gUnknown_03001C38.allegiance = 2;

    else if (UNIT_FACTION(gActiveUnit) == FACTION_GREEN)
        gUnknown_03001C38.allegiance = 1;

    gUnknown_03001C38.level = gActiveUnit->level;

    gUnknown_03001C38.xPosition = gActionData.xOther;
    gUnknown_03001C38.yPosition = gActionData.yOther;

    gUnknown_03001C38.redaCount = 0;
    gUnknown_03001C38.redas = NULL;

    gUnknown_03001C38.genMonster = FALSE;
    gUnknown_03001C38.itemDrop = FALSE;

    // 3.2. Items (generated from random number)

    for (i = 0; i < UNIT_DEFINITION_ITEM_COUNT; ++i)
        gUnknown_03001C38.items[i] = ITEM_NONE;

    if (gActiveUnit->level <= 5)
    {
        gUnknown_03001C38.items[0] = ITEM_AXE_IRON;
    }
    else if (gActiveUnit->level <= 10)
    {
        if (rand100 < 6)
            gUnknown_03001C38.items[0] = ITEM_AXE_DEVIL;
        else
            gUnknown_03001C38.items[0] = ITEM_AXE_IRON;
    }
    else if (gActiveUnit->level <= 15)
    {
        if (rand100 < 6)
            gUnknown_03001C38.items[0] = ITEM_AXE_DEVIL;

        else if (rand100 >= 6 && rand100 < 26)
            gUnknown_03001C38.items[0] = ITEM_AXE_KILLER;

        else
            gUnknown_03001C38.items[0] = ITEM_AXE_IRON;
    }
    else if (gActiveUnit->level <= 20)
    {
        if (rand100 < 6)
            gUnknown_03001C38.items[0] = ITEM_AXE_DEVIL;

        else if (rand100 >= 6 && rand100 < 26)
            gUnknown_03001C38.items[0] = ITEM_AXE_KILLER;

        else if (rand100 >= 26 && rand100 < 37)
            gUnknown_03001C38.items[0] = ITEM_AXE_TOMAHAWK;

        else
            gUnknown_03001C38.items[0] = ITEM_AXE_IRON;
    }

    // 3.3. Ai (is null)

    for (i = 0; i < 4; ++i)
        gUnknown_03001C38.ai[i] = 0;

    // 4. Load unit

    unit = GetUnitFromCharId(gUnknown_0895F5A4[summonerNum][1]);

    if (unit == NULL)
    {
        struct BattleUnit bu = gBattleActor;
        LoadUnits(&gUnknown_03001C38);
        gBattleActor = bu;
    }

    // 5. Set level and weapon ranks

    unit = GetUnitFromCharId(gUnknown_0895F5A4[summonerNum][1]);

    for (i = 0; i < 4; ++i)
        unit->ranks[i] = 0;

    unit->level = gActiveUnit->level;
    unit->exp   = UNIT_EXP_DISABLED;

    if (gActiveUnit->level <= 5)
        unit->ranks[ITYPE_AXE] = WPN_EXP_D;
    else if (gActiveUnit->level <= 10)
        unit->ranks[ITYPE_AXE] = WPN_EXP_C;
    else if (gActiveUnit->level <= 15)
        unit->ranks[ITYPE_AXE] = WPN_EXP_B;
    else if (gActiveUnit->level <= 20)
        unit->ranks[ITYPE_AXE] = WPN_EXP_A;
}

struct SumProc
{
    /* 00 */ PROC_HEADER;

    /* 29 */ u8 pad29[0x3C - 0x29];

    /* 3C */ int unk3C;
    /* 40 */ int unk40;

    /* 44 */ u8 pad44[0x64 - 0x44];

    /* 64 */ short unk64;
    /* 66 */ short unk66;
};

void sub_807AFD0(struct SumProc* proc)
{
    proc->unk64 = 0;
    proc->unk66 = 0;
}

void sub_807AFE0(struct SumProc* proc)
{
    if (proc->unk64 < 8)
        Proc_GotoLabel((struct Proc*) proc, 0); // TODO: this proc's label enum
}

s8 sub_807AFFC(struct SumProc* proc)
{
    s8 count = 0;
    int i;

    for (i = FACTION_RED + 1; i < FACTION_RED + 0x40; ++i)
    {
        struct Unit* unit = GetUnit(i);

        if (UNIT_IS_VALID(unit))
        {
            if (count >= 40)
            {
                Proc_GotoLabel((struct Proc*) proc, 1); // TODO: this proc's label enum
                return TRUE;
            }

            count++;
        }
    }

    if (proc->unk64 < 8 && proc->unk66 < 4)
        return FALSE;

    Proc_GotoLabel((struct Proc*) proc, 1); // TODO: this proc's label enum
    return TRUE;
}

void sub_807B054(void)
{
    PlaySoundEffect(0xA0); // TODO: song ids!
}

void sub_807B070(struct SumProc* proc)
{
    proc->unk64++;
    proc->unk66 = 0;

    New6C_SummonGfx(
        (struct Proc*) proc,
        proc->unk3C, proc->unk40);
}

struct SumThing
{
    s8 x, y;
    s8 boolAvailable;
    u32 unk04, unk08;
};

s8 sub_807B090(int x, int y, struct SumThing* result)
{
    struct SumThing array[9];
    u8 chosen, count = 0;

    short iy, ix;

    for (iy = y - 1; iy < y + 2; ++iy)
    {
        if (iy < 0 || gBmMapSize.y <= iy)
            continue;

        for (ix = x - 1; ix < x + 2; ++ix)
        {
            if (ix < 0 || gBmMapSize.x <= ix)
                continue;

            if (gBmMapUnit[iy][ix] != 0)
                continue; // there's a unit here!

            if (gUnknown_0202BCF0.chapterVisionRange && gBmMapFog[iy][ix] == 0)
                continue; // there's fog here!

            if (!CanUnitCrossTerrain(&gBattleActor.unit, gBmMapTerrain[iy][ix]))
                continue; // can't cross terrain!

            array[count].x = ix;
            array[count].y = iy;
            array[count].boolAvailable = TRUE;

            count++;
        }
    }

    if (!count)
        return -1;

    chosen = NextRN_N(count);

    *result = array[chosen];
    return 1;
}

void sub_807B1C0(struct SumProc* proc, s8 x, s8 y, short arg3)
{
    struct SumThing thing;

    if (sub_807AFFC(proc))
        return;

    if (sub_807B090(x, y, &thing) == -1 || thing.boolAvailable == -1)
    {
        proc->unk66++;
        Proc_GotoLabel((struct Proc*) proc, arg3);
    }
    else
    {
        proc->unk3C = thing.x;
        proc->unk40 = thing.y;

        EnsureCameraOntoPosition(
            (struct Proc*) proc,
            proc->unk3C, proc->unk40);
    }
}

void sub_807B234(struct SumProc* proc)
{
    sub_807B1C0(proc,
        gBattleActor.unit.xPos, gBattleActor.unit.yPos + 4,
        3); // TODO: proc label enums!
}

void sub_807B254(struct SumProc* proc)
{
    sub_807B1C0(proc,
        gBattleActor.unit.xPos + 4, gBattleActor.unit.yPos,
        4); // TODO: proc label enums!
}

void sub_807B274(struct SumProc* proc)
{
    sub_807B1C0(proc,
        gBattleActor.unit.xPos - 4, gBattleActor.unit.yPos,
        5); // TODO: proc label enums!
}

void sub_807B294(struct SumProc* proc)
{
    sub_807B1C0(proc,
        gBattleActor.unit.xPos, gBattleActor.unit.yPos - 4,
        6); // TODO: proc label enums!
}

void sub_807B2B4(struct SumProc* proc)
{
    u8 num = DivRem(AdvanceGetLCGRNValue(), 11);

    gUnknown_03001C50 = gUnknown_088D1F54[num];

    gUnknown_03001C50.autolevel = TRUE;
    gUnknown_03001C50.allegiance = 2;
    gUnknown_03001C50.level = 5 + num;

    gUnknown_03001C50.xPosition = proc->unk3C;
    gUnknown_03001C50.yPosition = proc->unk40;

    LoadUnits(&gUnknown_03001C50);
}

// ==============================
// NOTE: this may be a file split
// ==============================

#include "ap.h"

enum
{
    MA_FACING_OPPONENT,
    MA_FACING_DEFAULT,
    MA_FACING_UNK,
    MA_FACING_STANDING,
};

int GetFacingDirection(int xFrom, int yFrom, int xTo, int yTo);
void MapAnim_AdvanceBattleRound(void);
void SetupMapBattleAnim(struct BattleUnit* actor, struct BattleUnit* target, struct BattleHit* hit);
void sub_807B4D0(void);
void SetupMapAnimSpellData(struct BattleUnit* actor, struct BattleUnit* target, struct BattleHit* hit);
int GetSpellAssocCharCount(int item);

extern const u8 gUnknown_08205714[4];

extern const struct ProcCmd gUnknown_089A31F8[];
extern const struct ProcCmd gUnknown_089A3238[];
extern const struct ProcCmd gUnknown_089A3288[];
extern const struct ProcCmd gUnknown_089A32C8[];
extern const struct ProcCmd gUnknown_089A3398[];
extern const struct ProcCmd gUnknown_089A33C0[];
extern const struct ProcCmd gUnknown_089A34B0[];
extern const struct ProcCmd gUnknown_089A3508[];
extern const struct ProcCmd gUnknown_089A3874[];

void MakeBattleMOVEUNIT(int maActor, struct BattleUnit* bu, struct Unit* unit)
{
    if (!bu)
        return;

    gMapBattle.actor[maActor].unit = unit;
    gMapBattle.actor[maActor].bu   = bu;
    gMapBattle.actor[maActor].mu   = MU_Create(unit);

    gMapBattle.actor[maActor].mu->pAPHandle->frameTimer    = 0;
    gMapBattle.actor[maActor].mu->pAPHandle->frameInterval = 0;

    if (BUNIT_IS_OBSTACLE(bu))
        MU_Hide(gMapBattle.actor[maActor].mu);

    switch (unit->statusIndex)
    {

    case UNIT_STATUS_PETRIFY:
    case UNIT_STATUS_13:
        MU_SetPaletteId(gMapBattle.actor[maActor].mu, BM_OBJPAL_UNIT_GRAYED);
        break;

    } // switch (unit->statusIndex)
}

void SetBattleAnimFacing(int maActor, int maOpponent, int facing)
{
    int muFacing;

    switch (facing)
    {

    case MA_FACING_OPPONENT:
        muFacing = GetFacingDirection(
            gMapBattle.actor[maActor].unit->xPos,    gMapBattle.actor[maActor].unit->yPos,
            gMapBattle.actor[maOpponent].unit->xPos, gMapBattle.actor[maOpponent].unit->yPos);

        MU_SetFacing(gMapBattle.actor[maActor].mu, muFacing);

        break;

    case MA_FACING_DEFAULT:
        MU_SetDefaultFacing(gMapBattle.actor[maActor].mu);
        break;

    case MA_FACING_UNK:
        muFacing = GetFacingDirection(
            gMapBattle.actor[maActor].unit->xPos, gMapBattle.actor[maActor].unit->yPos, 0, 0);

        MU_SetFacing(gMapBattle.actor[maActor].mu, muFacing);

        break;

    } // switch (facing)
}

void SetupBattleMOVEUNITs(void)
{
    int maFacing = GetSpellAssocFacing(gMapBattle.actor[0].bu->weaponBefore);

    sub_807B4D0();

    switch (gMapBattle.actorCount_maybe)
    {

    case 2:
        if (gBattleHitArray[0].attributes & BATTLE_HIT_ATTR_TATTACK)
        {
            // In triangle attacks, both partners face the opponent too

            SetBattleAnimFacing(2, 1, maFacing);
            SetBattleAnimFacing(3, 1, maFacing);
        }

        SetBattleAnimFacing(1, 0, maFacing);

        // fallthrough

    case 1:
        SetBattleAnimFacing(0, 1, maFacing);
        break;

    } // switch (gMapBattle.actorCount_maybe)
}

void sub_807B4D0(void)
{
    u8 array[4];
    int i, j;

    int count = gMapBattle.actorCount_maybe;

    switch (gMapBattle.actorCount_maybe)
    {

    case 2:
        if (gBattleHitArray[0].attributes & BATTLE_HIT_ATTR_TATTACK)
            count += 2;

        break;

    case 1:
        break;

    } // switch (gMapBattle.actorCount_maybe)

    // Init ref array

    for (i = 0; i < count; ++i)
        array[i] = i;

    // Sort ref array

    for (i = 0; i < count-1; ++i)
    {
        for (j = i+1; j < count; ++j)
        {
            int swap = FALSE;

            if (gMapBattle.actor[array[i]].unit->yPos == gMapBattle.actor[array[j]].unit->yPos)
            {
                if (gMapBattle.actor[array[i]].unit->xPos >= gMapBattle.actor[array[j]].unit->xPos)
                    swap = TRUE;
            }
            else if (gMapBattle.actor[array[i]].unit->yPos >= gMapBattle.actor[array[j]].unit->yPos)
                swap = TRUE;

            if (swap)
            {
                u8 tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
            }
        }
    }

    // Apply

    for (i = 0; i < count; ++i)
        gMapBattle.actor[array[i]].mu->pAPHandle->objLayer = gUnknown_08205714[i];
}

void sub_807B5DC(void)
{
    gBattleActor.weaponBefore = ITEM_VULNERARY;

    gMapBattle.u5F = 0;
    gMapBattle.u62 = 0;
    gMapBattle.actorCount_maybe = 1;

    gMapBattle.hitIt = gBattleHitArray;
    MapAnim_AdvanceBattleRound();

    SetupMapBattleAnim(&gBattleActor, &gBattleTarget, gBattleHitArray);
    Proc_Create(gUnknown_089A31F8, ROOT_PROC_3);
}

void sub_807B634(void)
{
    gBattleActor.weaponBefore = ITEM_VULNERARY;

    gMapBattle.u5F = 0;
    gMapBattle.u62 = 0;
    gMapBattle.actorCount_maybe = 1;

    gMapBattle.hitIt = gBattleHitArray;
    MapAnim_AdvanceBattleRound();

    SetupMapBattleAnim(&gBattleActor, &gBattleTarget, gBattleHitArray);
    Proc_Create(gUnknown_089A3238, ROOT_PROC_3);
}

void sub_807B68C(void)
{
    gBattleActor.weaponBefore = ITEM_VULNERARY;

    gMapBattle.u5F = 0;
    gMapBattle.u62 = 0;
    gMapBattle.actorCount_maybe = 1;

    gMapBattle.hitIt = gBattleHitArray;
    MapAnim_AdvanceBattleRound();

    SetupMapBattleAnim(&gBattleActor, &gBattleTarget, gBattleHitArray);
    Proc_Create(gUnknown_089A3288, ROOT_PROC_3);
}

void BeginMapAnimForSteal(void)
{
    gBattleActor.weaponBefore = ITEM_SWORD_IRON;

    gMapBattle.u5F = 0;
    gMapBattle.u62 = 1;
    gMapBattle.actorCount_maybe = 2;

    gMapBattle.attackerActorNum = 0;
    gMapBattle.defenderActorNum = 1;

    SetupMapBattleAnim(&gBattleActor, &gBattleTarget, gBattleHitArray);
    Proc_Create(gUnknown_089A32C8, ROOT_PROC_3);
}

void BeginMapAnimForSummon(void)
{
    gBattleActor.weaponBefore = ITEM_STAFF_FORTIFY;

    gMapBattle.u5F = 0;
    gMapBattle.u62 = 2;
    gMapBattle.actorCount_maybe = 1;

    gMapBattle.attackerActorNum = 0;
    gMapBattle.defenderActorNum = 1;

    SetupMapBattleAnim(&gBattleActor, &gBattleTarget, gBattleHitArray);
    Proc_Create(gUnknown_089A3398, ROOT_PROC_3);
}

void BeginMapAnimForSummonDK(void)
{
    gBattleActor.weaponBefore = ITEM_STAFF_FORTIFY;

    gMapBattle.u5F = 0;
    gMapBattle.u62 = 2;
    gMapBattle.actorCount_maybe = 1;

    gMapBattle.attackerActorNum = 0;
    gMapBattle.defenderActorNum = 1;

    SetupMapBattleAnim(&gBattleActor, &gBattleTarget, gBattleHitArray);
    Proc_Create(gUnknown_089A33C0, ROOT_PROC_3);
}

void sub_807B7F0(void)
{
    gBattleActor.weaponBefore = ITEM_STAFF_FORTIFY;

    gMapBattle.u5F = 0;
    gMapBattle.u62 = 2;
    gMapBattle.actorCount_maybe = 1;

    gMapBattle.attackerActorNum = 0;
    gMapBattle.defenderActorNum = 0;

    SetupMapBattleAnim(&gBattleActor, &gBattleTarget, gBattleHitArray);
    Proc_Create(gUnknown_089A34B0, ROOT_PROC_3);
}

void BeginBattleMapAnims(void)
{
    if (gBattleStats.config & (BATTLE_CONFIG_REFRESH | BATTLE_CONFIG_DANCERING))
    {
        sub_807B7F0();
        return;
    }

    gMapBattle.u5F = 0;
    gMapBattle.u62 = 0;

    SetupMapAnimSpellData(&gBattleActor, &gBattleTarget, gBattleHitArray);
    SetupMapBattleAnim(&gBattleActor, &gBattleTarget, gBattleHitArray);

    if (!EventEngineExists())
        Proc_Create(gUnknown_089A3508, ROOT_PROC_3);
    else
        Proc_Create(gUnknown_089A3874, ROOT_PROC_3);
}

void SetupMapAnimSpellData(struct BattleUnit* actor, struct BattleUnit* target, struct BattleHit* hit)
{
    gMapBattle.actorCount_maybe = GetSpellAssocCharCount(actor->weaponBefore);
    gMapBattle.hitIt            = hit;
    gMapBattle.specialProcScr   = GetSpellAssocAlt6CPointer(actor->weaponBefore);
}

void SetupMapBattleAnim(struct BattleUnit* actor, struct BattleUnit* target, struct BattleHit* hit)
{
    int i;

    MakeBattleMOVEUNIT(0, actor, &actor->unit);

    if (gMapBattle.actorCount_maybe > 1)
    {
        HideUnitSMS(&gBattleTarget.unit); // NOTE: uses gBattleTarget instead of target argument
        MakeBattleMOVEUNIT(1, target, &target->unit);
    }

    if (gBattleHitArray[0].attributes & BATTLE_HIT_ATTR_TATTACK)
    {
        MakeBattleMOVEUNIT(2, actor, gBattleStats.taUnitA);
        MakeBattleMOVEUNIT(3, actor, gBattleStats.taUnitB);

        HideUnitSMS(gBattleStats.taUnitA);
        HideUnitSMS(gBattleStats.taUnitB);
    }

    SetupBattleMOVEUNITs();

    for (i = 0; i < gMapBattle.actorCount_maybe; ++i)
    {
        gMapBattle.actor[i].u0D = gMapBattle.actor[i].bu->hpInitial;
        gMapBattle.actor[i].u0C = GetUnitMaxHp(gMapBattle.actor[i].unit);
    }

    SetDefaultColorEffects();
}

int GetFacingDirection(int xFrom, int yFrom, int xTo, int yTo)
{
    if (ABS(xTo - xFrom)*2 < ABS(yTo - yFrom))
    {
        if (yFrom < yTo)
            return MU_FACING_DOWN;
        else
            return MU_FACING_UP;
    }
    else
    {
        if (xFrom < xTo)
            return MU_FACING_RIGHT;
        else
            return MU_FACING_LEFT;
    }
}

// =============================
// NOTE: Start Map Anim Info box
// =============================

struct MAInfoFrameProc
{
    PROC_HEADER;

    /* 2A */ short unk2A;
    /* 2C */ u16 unk2C;
    /* 2E */ u8 unk2E;
    /* 2F */ u8 unk2F;
    /* 30 */ struct Proc* maMain;
};

void sub_8013168(u16* arg0, u8* arg1, int arg2, int arg3, int arg4);

void sub_807BD54(struct MAInfoFrameProc* proc, int actor);
void sub_807BE1C(struct MAInfoFrameProc* proc, int, int);
void sub_8081E78(void);
void sub_80820D8(u16, u16, u16, u16);

void CallARM_FillTileRect(u16* tilemap, u16* tsa, u16 tileref);
void sub_80143D8(u16* tilemap, int arg1, int arg2, const char* cstring);

void sub_807BF54(struct MAInfoFrameProc* proc);

extern u8 gUnknown_089AD500[];

extern int gUnknown_089A3668[];

extern const struct ProcCmd gUnknown_089A3688[];

extern const u8 gUnknown_089AD868[];

extern const u16 gUnknown_08A1D79C[];

extern u8 gUnknown_089AD78C[];

extern u16 gUnknown_089A3648[];

// Those are palettes
extern u16 gUnknown_089AD648[];
extern u16 gUnknown_089AD668[];
extern u16 gUnknown_089AD688[];
extern u16 gUnknown_089AD6A8[];

extern u8* gUnknown_089A3670[][2];

extern u16 gUnknown_089A36C0[];
extern u8 gUnknown_08802D44[];
extern u8 gUnknown_088030C4[];
extern u8 gUnknown_088033C4[];
extern u16 gUnknown_08803590[];
extern u16 gUnknown_089AD498[];

void sub_807B9F8(int tileNum)
{
    CopyDataWithPossibleUncomp(
        gUnknown_089AD868,
        (u8*)(VRAM) + GetBackgroundTileDataOffset(0) + 0x20*(tileNum & 0x3FF));
}

void sub_807BA28(u16* tilemap, int num, int tileref, int len, u16 blankref, int arg5)
{
    char buf[8];
    int i, j;

    for (i = sizeof(buf)-1; i >= 0; --i)
    {
        buf[i] = '0' + num % 10;
        num = num / 10;

        if (num == 0)
        {
            for (j = i - 1; j >= 0; --j)
                buf[j] = ' ';

            break;
        }
    }

    sub_8013168(tilemap, buf + sizeof(buf)-1, tileref, len, arg5);

    for (i = len - 1; i > 0 && buf[7 - i] == ' '; --i)
        tilemap[-i] = blankref;
}

void sub_807BAE4(const u8* src)
{
    sub_807B9F8(0x20);
    CopyDataWithPossibleUncomp(src, (u8*)(VRAM + 0x20 * 43)); // TODO: named constants
    ApplyPalette(gUnknown_08A1D79C, 5);
}

void sub_807BB10(u16* arg0, int* arg1, int arg2, int arg3, int arg4)
{
    int r1;

    if (*arg1 > arg3)
        r1 = arg3;
    else
        r1 = *arg1;

    *arg0 = TILEREF(arg4 + r1, arg2);
    *arg1 += 1 - arg3;

    if (*arg1 < 0)
        *arg1 = 0;
}

void sub_807BB40(u16* tilemap, int arg1, int arg2, int arg3, u16* arg4)
{
    int unk4, count = 0;
    u16* it;

    for (it = arg4; it[0]; it += 2)
        count -= 1 - it[0];

    count += 1;

    if (arg1 == arg2)
        unk4 = count;
    else
        unk4 = ((count<<8) / arg1 * arg2) >> 8;

    if (unk4 == 0 && arg2 > 0)
        unk4 = 1;

    for (it = arg4; it[0]; ++tilemap, it += 2)
        sub_807BB10(tilemap, &unk4, gUnknown_089A3668[arg3], it[0], it[1]);
}

void DeleteBattleAnimInfoThing(void)
{
    Proc_DeleteAllWithScript(gUnknown_089A3688);
}

void NewMapBattleInfoThing(int arg0, int arg1, struct Proc* parent)
{
    struct MAInfoFrameProc* proc = (struct MAInfoFrameProc*) Proc_Create(gUnknown_089A3688, ROOT_PROC_3);

    proc->unk2E = arg0;
    proc->unk2F = arg1;

    proc->maMain = parent;
}

void sub_807BBF0(void)
{
    SetPrimaryHBlankHandler(NULL);
    ClearBg0Bg1();
}

void sub_807BC00(struct MAInfoFrameProc* proc)
{
    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);

    CopyDataWithPossibleUncomp(
        gUnknown_089AD500,
        (void*)(VRAM) + GetBackgroundTileDataOffset(1) + BM_BGCHR_BANIM_IFBACK * 0x20); //< TODO: put in macro?

    sub_807BAE4(gUnknown_089AD78C);

    switch (gMapBattle.actorCount_maybe)
    {

    case 1:
        sub_807BE1C(proc, 0, -5);
        break;

    case 2:
        sub_807BE1C(proc, 0, -1);
        sub_807BE1C(proc, 1, -11);
        break;

    } // switch (gMapBattle.actorCount_maybe)

    sub_8081E78();

    sub_80820D8(
        gMapBattle.actor[0].u11*8,
        gMapBattle.actor[0].u11*8 + 0x20,
        gPaletteBuffer[0x11],
        gPaletteBuffer[0x21]);
}

void sub_807BCA8(struct MAInfoFrameProc* proc)
{
    s8 updated = FALSE;
    int i;

    for (i = 0; i < gMapBattle.actorCount_maybe; ++i)
    {
        u16 r4 = gMapBattle.actor[i].u0E;

        if (r4 > gMapBattle.actor[i].u0D*16)
            r4 = r4 - 16;

        if (r4 < gMapBattle.actor[i].u0D*16)
        {
            r4 = r4 + 4;

            if (r4 % 16 == 0)
                PlaySoundEffect(0x75); // TODO: song ids
        }

        if (r4 != gMapBattle.actor[i].u0E)
        {
            gMapBattle.actor[i].u0E = r4;
            sub_807BD54(proc, i);
            updated = TRUE;
        }
    }

    if (!updated && gMapBattle.u5F)
        gMapBattle.u5F = FALSE;
}

void sub_807BD54(struct MAInfoFrameProc* proc, int a)
{
    int dummy = gMapBattle.actor[a].u0E/16;
    int r6 = (dummy >= 100);

    sub_807BA28(
        gBG0TilemapBuffer + TILEMAP_INDEX(
            gMapBattle.actor[a].u10 + 3,
            gMapBattle.actor[a].u11 + 3),
        gMapBattle.actor[a].u0E/16,
        TILEREF(32, BM_BGPAL_BANIM_UNK5), 3, 0, r6);

    sub_807BB40(
        gBG0TilemapBuffer + TILEMAP_INDEX(
            gMapBattle.actor[a].u10 + 4,
            gMapBattle.actor[a].u11 + 3),
        gMapBattle.actor[a].u0C,
        gMapBattle.actor[a].u0E/16,
        0, gUnknown_089A3648);

    BG_EnableSyncByMask(BG0_SYNC_BIT);
}

u16* sub_807BDD0(struct Unit* unit)
{
    switch (UNIT_FACTION(unit))
    {

    case FACTION_BLUE:
        return gUnknown_089AD648;

    case FACTION_RED:
        return gUnknown_089AD668;

    case FACTION_GREEN:
        return gUnknown_089AD688;

    case FACTION_PURPLE:
        return gUnknown_089AD6A8;

    } // switch (UNIT_FACTION(unit))

    return NULL;
}

void sub_807BE1C(struct MAInfoFrameProc* proc, int a, int arg2)
{
    gMapBattle.actor[a].u10 = proc->unk2E + arg2;
    gMapBattle.actor[a].u11 = proc->unk2F;

    ApplyPalette(
        sub_807BDD0(gMapBattle.actor[a].unit),
        BM_BGPAL_BANIM_IFBACK + a);

    CopyDataWithPossibleUncomp(
        gUnknown_089A3670[gMapBattle.actorCount_maybe][a], gUnknown_02020188);

    CallARM_FillTileRect(
        TILEMAP_LOCATED(gBG1TilemapBuffer,
            gMapBattle.actor[a].u10,
            gMapBattle.actor[a].u11),
        (u16*) gUnknown_02020188,
        BM_BGCHR_BANIM_IFBACK | TILEREF(0, BM_BGPAL_BANIM_IFBACK + a));

    BG_EnableSyncByMask(BG1_SYNC_BIT);

    sub_80143D8(
        TILEMAP_LOCATED(gBG0TilemapBuffer,
            gMapBattle.actor[a].u10 + 2,
            gMapBattle.actor[a].u11 + 1),
        0, 9,
        GetStringFromIndex(UNIT_NAME_ID(gMapBattle.actor[a].unit)));

    BG_EnableSyncByMask(BG0_SYNC_BIT);

    gMapBattle.actor[a].u0E = gMapBattle.actor[a].u0D*16;

    sub_807BD54(proc, a);
}

void sub_807BEF8(struct MAInfoFrameProc* proc)
{
    proc->unk2A = 0;

    sub_807BF54(proc);

    SetWinEnable(1, 0, 0);

    SetWin0Layers(1, 1, 1, 1, 1);
    SetWOutLayers(0, 0, 1, 1, 1);
}

void sub_807BF54(struct MAInfoFrameProc* proc)
{
    // TODO: SetWin0PtA macro?
    gLCDControlBuffer.win0_left   = 0;
    gLCDControlBuffer.win0_top    = (proc->unk2F+2)*8 - proc->unk2A;

    // TODO: SetWin0PtB macro?
    gLCDControlBuffer.win0_right  = 240; // TODO: SCREEN_WIDTH?
    gLCDControlBuffer.win0_bottom = (proc->unk2F+2)*8 + proc->unk2A;

    proc->unk2A += 2;

    if (proc->unk2A > 0x10)
    {
        SetWinEnable(0, 0, 0);
        Proc_ClearNativeCallback((struct Proc*) proc);
    }
}

// ============================
// NOTE: Start Map Anim Exp bar
// ============================

void sub_807C1AC(struct MAExpBarProc* proc);
void sub_807F10C(int actor, struct Proc* parent);

void sub_807BFC4(int arg0, int arg1, int arg2)
{
    sub_807BA28(
        TILEMAP_LOCATED(gBG0TilemapBuffer,
            arg0 + 2,
            arg1 + 1),
        arg2, 0x5225, 2, 0x522F, FALSE);

    sub_807BB40(
        TILEMAP_LOCATED(gBG0TilemapBuffer,
            arg0 + 3,
            arg1 + 1),
        99, arg2, 0, gUnknown_089A36C0);

    BG_EnableSyncByMask(BG0_SYNC_BIT);
}

void sub_807C02C(struct MAExpBarProc* proc)
{
    BG_SetPosition(0, 0, 0);
    BG_SetPosition(1, 0, 0);

    // TODO: ApplyTileImages macros?
    // TODO: BM_BGCHR_...?

    RegisterTileGraphics(
        gUnknown_08802D44,
        (void*)(VRAM) + GetBackgroundTileDataOffset(0) + 512 * 0x20,
        13 * 0x20);

    RegisterTileGraphics(
        gUnknown_088030C4,
        (void*)(VRAM) + GetBackgroundTileDataOffset(0) + 525 * 0x20,
        24 * 0x20);

    RegisterTileGraphics(
        gUnknown_088033C4,
        (void*)(VRAM) + GetBackgroundTileDataOffset(0) + 549 * 0x20,
        11 * 0x20);

    ApplyPalette(gUnknown_08803590, BM_BGPAL_BANIM_UNK5);

    CallARM_FillTileRect(
        TILEMAP_LOCATED(gBG0TilemapBuffer, 6, 8),
        gUnknown_089AD498,
        TILEREF(512, BM_BGPAL_BANIM_UNK5));

    sub_807BFC4(6, 8, proc->expFrom);
}

void sub_807C0DC(void)
{
    PlaySoundEffect(0x74); // TODO: song ids
}

void sub_807C0F8(struct MAExpBarProc* proc)
{
    proc->expFrom++;

    if (proc->expFrom >= 100)
        proc->expFrom = 0;

    sub_807BFC4(6, 8, proc->expFrom);

    if (proc->expFrom == proc->expTo % 100)
    {
        Proc_ClearNativeCallback((struct Proc*) proc);
        m4aSongNumStop(0x74); // TODO: song ids
    }
}

void sub_807C14C(struct MAExpBarProc* proc)
{
    proc->unk6A = 0;

    sub_807C1AC(proc);

    SetWinEnable(1, 0, 0);

    SetWin0Layers(1, 1, 1, 1, 1);
    SetWOutLayers(0, 0, 1, 1, 1);
}

void sub_807C1AC(struct MAExpBarProc* proc)
{
    // TODO: SetWin0PtA macro?
    gLCDControlBuffer.win0_left   = 0;
    gLCDControlBuffer.win0_top    = 76 - proc->unk6A;

    // TODO: SetWin0PtB macro?
    gLCDControlBuffer.win0_right  = 240; // TODO: SCREEN_WIDTH?
    gLCDControlBuffer.win0_bottom = 76 + proc->unk6A;

    proc->unk6A += 2;

    if (proc->unk6A > 12)
    {
        SetWinEnable(0, 0, 0);
        Proc_ClearNativeCallback((struct Proc*) proc);
    }
}

void sub_807C210(struct MAExpBarProc* proc)
{
    if (proc->expTo >= 100)
        sub_807F10C(proc->actorId, (struct Proc*) proc);
}

// ===================================================
// NOTE: Start Oh fcuk debug stuff I didn't know about
// ===================================================

struct MADebugProc
{
    PROC_HEADER;

    /* 29 */ u8 pad29[0x64 - 0x29];
    /* 64 */ short unk64;
    /* 66 */ short unk66;
};

struct MADebugInfoEntry
{
    /* 00 */ short data[10];
    /* 14 */ struct TextHandle text[10];
};

struct MADebugInfo
{
    /* 00 */ u8 pad00[8];
    /* 08 */ struct MADebugInfoEntry e[2];
};

struct Unk089A3798
{
    /* 00 */ u8 a; s8 b, c, d, e; u8 f, g;
};

void sub_801443C(u16* tilemap, int color, const char* cstring);

s8 sub_807C8FC(void);

extern struct Unk089A3798 CONST_DATA gUnknown_089A3798[];

extern CONST_DATA struct MADebugInfo* gUnknown_089A3810;

extern CONST_DATA struct ProcCmd gUnknown_089A3814[];

extern char* CONST_DATA gUnknown_089A3770[];
extern char* CONST_DATA gUnknown_089A37E8[];

void sub_807C230(void)
{
    Proc_Create(gUnknown_089A3814, ROOT_PROC_3);
}

void sub_807C244(int num, int arg1, int arg2)
{
    const struct CharacterData* charData = GetCharacterData(gUnknown_089A3810->e[num].data[0]);
    const struct ClassData* classData = GetClassData(gUnknown_089A3810->e[num].data[3]);

    int charId = gUnknown_089A3810->e[num].data[0];
    int classId = gUnknown_089A3810->e[num].data[3];
    int unk = gUnknown_089A3810->e[num].data[4];

    switch (arg1)
    {

    case 0:
        Text_Clear(&gUnknown_089A3810->e[num].text[0]);

        Text_InsertNumberOr2Dashes(
            &gUnknown_089A3810->e[num].text[0],
            16, arg2, charId);

        DrawTextInline(
            &gUnknown_089A3810->e[num].text[0],
            TILEMAP_LOCATED(gBG0TilemapBuffer, num*12 + 6, 0),
            arg2, 24, 0, GetStringFromIndex(charData->nameTextId));

        BG_EnableSyncByMask(BG0_SYNC_BIT);

        break;

    case 1:
        Text_Clear(&gUnknown_089A3810->e[num].text[1]);

        Text_InsertNumberOr2Dashes(
            &gUnknown_089A3810->e[num].text[1],
            8, arg2, gUnknown_089A3810->e[num].data[1]);

        Text_Draw(
            &gUnknown_089A3810->e[num].text[1],
            TILEMAP_LOCATED(gBG0TilemapBuffer, num*12 + 7, 2));

        BG_EnableSyncByMask(BG0_SYNC_BIT);

        break;

    case 2:
        Text_Clear(&gUnknown_089A3810->e[num].text[2]);

        Text_InsertNumberOr2Dashes(
            &gUnknown_089A3810->e[num].text[2],
            8, arg2, gUnknown_089A3810->e[num].data[2]);

        Text_Draw(
            &gUnknown_089A3810->e[num].text[2],
            TILEMAP_LOCATED(gBG0TilemapBuffer, num*12 + 10, 2));

        BG_EnableSyncByMask(BG0_SYNC_BIT);

        break;

    case 3:
        Text_Clear(&gUnknown_089A3810->e[num].text[3]);

        Text_InsertNumberOr2Dashes(
            &gUnknown_089A3810->e[num].text[3],
            16, arg2, classId);

        DrawTextInline(
            &gUnknown_089A3810->e[num].text[3],
            TILEMAP_LOCATED(gBG0TilemapBuffer, num*12 + 6, 4),
            arg2, 24, 0, GetStringFromIndex(classData->nameTextId));

        BG_EnableSyncByMask(BG0_SYNC_BIT);

        break;

    case 4:
        Text_Clear(&gUnknown_089A3810->e[num].text[4]);

        Text_InsertNumberOr2Dashes(
            &gUnknown_089A3810->e[num].text[4],
            16, arg2, unk);

        DrawTextInline(
            &gUnknown_089A3810->e[num].text[4],
            TILEMAP_LOCATED(gBG0TilemapBuffer, num*12 + 6, 6),
            arg2, 24, 0, GetItemName(gUnknown_089A3810->e[num].data[4]));

        BG_EnableSyncByMask(BG0_SYNC_BIT);

        break;

    case 5 ... 9:
        Text_Clear(&gUnknown_089A3810->e[num].text[arg1]);

        Text_InsertNumberOr2Dashes(
            &gUnknown_089A3810->e[num].text[arg1],
            8, arg2, gUnknown_089A3810->e[num].data[arg1]);

        DrawTextInline(
            &gUnknown_089A3810->e[num].text[arg1],
            TILEMAP_LOCATED(gBG0TilemapBuffer, num*12 - 57, arg1*2),
            arg2, 16, 0,
            gUnknown_089A3770[gUnknown_089A3810->e[num].data[arg1]]);

        BG_EnableSyncByMask(BG0_SYNC_BIT);

        break;

    } // switch (arg1)
}

void sub_807C4F0(struct MADebugProc* proc)
{
    Proc_DeleteAllWithScript(gUnknown_0859AA5C);

    proc->unk64 = 0;
    proc->unk66 = 0;

    gUnknown_089A3810->e[0].data[3] = 1;
    gUnknown_089A3810->e[0].data[0] = 1;
    gUnknown_089A3810->e[0].data[4] = 1;
    gUnknown_089A3810->e[0].data[1] = 4;
    gUnknown_089A3810->e[0].data[2] = 8;

    gUnknown_089A3810->e[1].data[3] = 1;
    gUnknown_089A3810->e[1].data[0] = 2;
    gUnknown_089A3810->e[1].data[4] = 2;
    gUnknown_089A3810->e[1].data[1] = 5;
    gUnknown_089A3810->e[1].data[2] = 8;

    gUnknown_089A3810->e[0].data[5] = 1;
    gUnknown_089A3810->e[0].data[6] = 5;
    gUnknown_089A3810->e[0].data[7] = 0;
    gUnknown_089A3810->e[0].data[8] = 0;
    gUnknown_089A3810->e[0].data[9] = 0;

    gUnknown_089A3810->e[1].data[5] = 1;
    gUnknown_089A3810->e[1].data[6] = 0;
    gUnknown_089A3810->e[1].data[7] = 0;
    gUnknown_089A3810->e[1].data[8] = 0;
    gUnknown_089A3810->e[1].data[9] = 0;
}

void sub_807C568(struct MADebugProc* proc)
{
    int i, j;

    MU_EndAll();
    Font_InitForUIDefault();

    SetSpecialColorEffectsParameters(2, 8, 8, 0);

    sub_8001ED0(0, 1, 0, 0, 0);
    sub_8001F0C(0, 0, 1, 1, 1);

    // TODO: SetWinEnable macro?
    gLCDControlBuffer.dispcnt.win0_on   = FALSE;
    gLCDControlBuffer.dispcnt.win1_on   = FALSE;
    gLCDControlBuffer.dispcnt.objWin_on = FALSE;

    DrawUiFrame2(0, 0, 29, 19, 1); // TODO: UI_STYLE...

    for (i = 0; gUnknown_089A37E8[i]; ++i)
        sub_801443C(
            TILEMAP_LOCATED(gBG0TilemapBuffer, 1, i*2), 0, gUnknown_089A37E8[i]);

    for (i = 0; i < 10; ++i)
    {
        for (j = 0; j < 2; ++j)
        {
            Text_Allocate(&gUnknown_089A3810->e[j].text[i], gUnknown_089A3798[i].a);

            if (j == proc->unk64 && i == proc->unk66)
                sub_807C244(j, i, TEXT_COLOR_NORMAL);
            else
                sub_807C244(j, i, TEXT_COLOR_GRAY);
        }
    }

    BG_EnableSyncByMask(BG0_SYNC_BIT);
}

void sub_807C680(struct MADebugProc* proc)
{
    int inc;

    int oldActor = proc->unk64;
    int oldField = proc->unk66;

    if (gKeyStatusPtr->newKeys & START_BUTTON)
    {
        if (!sub_807C8FC())
            return;

        Proc_ClearNativeCallback((struct Proc*) proc);
    }

    inc = (gKeyStatusPtr->heldKeys & R_BUTTON) ? 10 : 1;

    if (gKeyStatusPtr->repeatedKeys & A_BUTTON)
    {
        gUnknown_089A3810->e[proc->unk64].data[proc->unk66] += inc;

        if (gUnknown_089A3810->e[proc->unk64].data[proc->unk66] >= gUnknown_089A3798[proc->unk66].g)
        {
            if (inc == 1)
                gUnknown_089A3810->e[proc->unk64].data[proc->unk66] = gUnknown_089A3798[proc->unk66].f;
            else
                gUnknown_089A3810->e[proc->unk64].data[proc->unk66] = gUnknown_089A3798[proc->unk66].g - 1;
        }
    }

    if (gKeyStatusPtr->repeatedKeys & B_BUTTON)
    {
        gUnknown_089A3810->e[proc->unk64].data[proc->unk66] -= inc;

        if (gUnknown_089A3810->e[proc->unk64].data[proc->unk66] < gUnknown_089A3798[proc->unk66].f)
        {
            if (inc == 1)
                gUnknown_089A3810->e[proc->unk64].data[proc->unk66] = gUnknown_089A3798[proc->unk66].g - 1;
            else
                gUnknown_089A3810->e[proc->unk64].data[proc->unk66] = gUnknown_089A3798[proc->unk66].f;
        }
    }

    if (gKeyStatusPtr->repeatedKeys & DPAD_LEFT)
    {
        if (proc->unk66 != 2)
            proc->unk64 = 1 - proc->unk64;

        proc->unk66 = gUnknown_089A3798[proc->unk66].d;
    }

    if (gKeyStatusPtr->repeatedKeys & DPAD_RIGHT)
    {
        if (proc->unk66 != 1)
            proc->unk64 = 1 - proc->unk64;

        proc->unk66 = gUnknown_089A3798[proc->unk66].e;
    }

    if (gKeyStatusPtr->repeatedKeys & DPAD_UP)
        proc->unk66 = gUnknown_089A3798[proc->unk66].b;

    if (gKeyStatusPtr->repeatedKeys & DPAD_DOWN)
        proc->unk66 = gUnknown_089A3798[proc->unk66].c;

    if (gKeyStatusPtr->repeatedKeys & DPAD_ANY)
        sub_807C244(oldActor, oldField, TEXT_COLOR_GRAY);

    if (gKeyStatusPtr->repeatedKeys & (DPAD_ANY | A_BUTTON | B_BUTTON))
        sub_807C244(proc->unk64, proc->unk66, TEXT_COLOR_NORMAL);
}

void sub_807C8A0(struct BattleUnit* bu, int dActor)
{
    bu->hpInitial  = 30;
    bu->unit.maxHP = 60;

    bu->unit.pCharacterData = GetCharacterData(gUnknown_089A3810->e[dActor].data[0]);
    bu->unit.pClassData     = GetClassData(gUnknown_089A3810->e[dActor].data[3]);

    bu->unit.xPos = gUnknown_089A3810->e[dActor].data[1];
    bu->unit.yPos = gUnknown_089A3810->e[dActor].data[2];

    bu->weaponBefore = gUnknown_089A3810->e[dActor].data[4];
    bu->expGain = 0;
}

s8 sub_807C8FC(void)
{
    // There's some gross variable reuse going on here

    int hitnum, actnum, unk, i;

    struct BattleHit* hit = gBattleHitArray;

    sub_807C8A0(&gBattleActor, 0);
    sub_807C8A0(&gBattleTarget, 1);

    ClearBattleHits();

    unk = 0;

    for (hitnum = 0; hitnum < 5; ++hitnum)
    {
        for (actnum = 0; actnum < 2; ++actnum)
        {
            if (gUnknown_089A3810->e[actnum].data[5 + hitnum] != 0)
            {
                unk = 1;
                break;
            }
        }

        if (unk)
            break;
    }

    if (hitnum == 5 && actnum == 2)
        return FALSE;

    for (i = hitnum*2+actnum; i < 10; ++i)
    {
        hitnum = i / 2;
        actnum = i & 1;

        hit->info = actnum<<3;

        unk = gUnknown_089A3810->e[actnum].data[5 + hitnum];

        switch (unk)
        {

        case 5 ... 8:
            hit->attributes |= BATTLE_HIT_ATTR_CRIT;
            hit->hpChange = 20;
            break;

        case 1 ... 4:
            hit->hpChange = 10;
            break;

        case 9:
            hit->attributes |= BATTLE_HIT_ATTR_MISS;
            break;

        } // switch (unk)

        switch (unk)
        {

        case 2:
        case 6:
            hit->attributes |= BATTLE_HIT_ATTR_DEVIL;
            break;

        case 3:
        case 7:
            hit->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
            break;

        case 4:
        case 8:
            hit->attributes |= BATTLE_HIT_ATTR_POISON;
            break;

        } // switch (unk)

        if (unk != 0)
            ++hit;
    }

    hit->info |= BATTLE_HIT_INFO_END;
    return TRUE;
}

void sub_807CAA8(void)
{
    BG_Fill(gBG0TilemapBuffer, TILEREF(0, 0));
    BG_Fill(gBG1TilemapBuffer, TILEREF(0, 0));
    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT);

    BeginBattleMapAnims();
}

// ============================================
// NOTE: We back to your regular map anim stuff
// ============================================

#include <stdlib.h>

#include "ap.h"
#include "bmtrick.h"
#include "face.h"

struct MAEffectProc
{
    /* 00 */ PROC_HEADER;
    /* 2C */ struct Unit* unit;
    /* 30 */ int xDisplay;
    /* 34 */ int yDisplay;
    /* 38 */ u8 pad38[0x40 - 0x38];
    /* 40 */ u16 unk40;
    /* 42 */ u16 unk42;
    /* 44 */ u16 unk44;
    /* 44 */ u8 pad46[0x48 - 0x46];
    /* 48 */ short unk48;
    /* 4A */ short unk4A;
    /* 4C */ u8 pad4C[0x50 - 0x4C];
    /* 50 */ const void* img;
    /* 54 */ const void* pal;
    /* 58 */ u16 unk58;
    /* 5A */ u8 pad5A[0x64 - 0x5A];
    /* 64 */ short unk64;
};

struct MAAnotherProc
{
    /* 00 */ PROC_HEADER;
    /* 2C */ int unk2C;
    /* 30 */ int unk30;
    /* 34 */ u8 pad34[0x44 - 0x34];
    /* 44 */ short unk44;
    /* 46 */ u8 pad46[0x54 - 0x46];
    /* 54 */ int unk54;
    /* 58 */ int unk58;
    /* 5C */ u8 pad5C[0x64 - 0x5C];
    /* 64 */ short unk64;
};

struct MALevelUpProc
{
    /* 00 */ PROC_HEADER;

    /* 2A */ short pad2A;
    /* 2C */ short pad2C;
    /* 2E */ short actor;
    /* 30 */ u8 unk30;
    /* 31 */ u8 unk31;
    /* 32 */ short unk32;
};

struct MAFrameShakeProc
{
    PROC_HEADER;

    /* 29 */ u8 pad29[0x64 - 0x29];
    /* 64 */ short unk64;
};

struct MAStarProc
{
    PROC_HEADER;

    /* 2A */ short xCenter;
    /* 2C */ short yCenter;
    /* 2E */ short unk2E;
    /* 30 */ short unk30;

    /* 29 */ u8    pad32[0x36 - 0x32];

    /* 36 */ u16 unk36;
    /* 38 */ u16 unk38;
    /* 3A */ u16 unk3A;
    /* 3C */ u16 unk3C;
    /* 3E */ u16 unk3E;
    /* 40 */ u16 unk40;
};

struct Unk03005090
{
    /* 00 */ u16 unk00;
    /* 02 */ u16 unk02;
    /* 04 */ int pad04;
};

void sub_8013AA4(int);
void sub_80144CC(const u16* pal, int off, int len, int unk, struct Proc* proc);
void sub_8014560(u16* tilemap, int x, int y, int tileref, int, int);
void sub_80146A0(u16* tilemap, int x, int y, int tileref, int, int, const void*, int);
void sub_801474C(u16* tilemap, int x, int y, int tileref, int, int, const void*, int);
void sub_8014930(void* vram, int, int);

void NewPaletteAnimator_(const u16* pal, int off, int len, int unk, struct Proc* proc);
void DeleteAllPaletteAnimator(void);

void sub_807CF30(struct MAEffectProc* proc);
void sub_807D328(struct MAEffectProc* proc);
void sub_807D360(int, int, int, int, int, struct Proc* parent);
void sub_807E978(void);
void sub_807E978(void);
void sub_807EA20(int, int, int, struct Proc* proc);
void LoadSparkGfx(void);
void sub_8081E78(void);
void sub_8081EAC(void);
void sub_8081F24(int, int, int);
void sub_8081F58(void);
void sub_8081FA8(void);
void sub_808218C(int, int, int, int, const void*);
void sub_808320C(int, int);

extern u8 CONST_DATA gUnknown_089AC0A4[]; // ma miss img
extern u16 CONST_DATA gUnknown_089AC194[]; // ma miss ap

extern u8 CONST_DATA gUnknown_089AC2FC[]; // ma no damage img
extern u16 CONST_DATA gUnknown_089AC440[]; // ma no damage ap

extern struct ProcCmd CONST_DATA gUnknown_089A3924[]; // ma wall break proc scr

extern u8 CONST_DATA gUnknown_089ADA80[]; // ma wall break img
extern u16 CONST_DATA gUnknown_089ADD0C[]; // ma wall break pal
extern u16 CONST_DATA gUnknown_089A6FD8[]; // ma wall break ap

extern struct ProcCmd CONST_DATA gUnknown_089A3944[]; // ma poison proc scr

extern u8 CONST_DATA gUnknown_089ADEB0[]; // ma poison img
extern u16 CONST_DATA gUnknown_089AE204[]; // ma poison pal
extern u16 CONST_DATA gUnknown_089A6F40[]; // ma poison ap

extern struct ProcCmd CONST_DATA gUnknown_089A3964[]; // ma another poison proc scr?

extern struct ProcCmd CONST_DATA gUnknown_089A398C[];

extern u8 CONST_DATA gUnknown_089B7610[];
extern u16 CONST_DATA gUnknown_089B80C4[];

extern const u8* CONST_DATA gUnknown_089A39C4[];

extern struct UnitDefinition gUnknown_03001C68; // egg hatching unit buf

extern struct ProcCmd CONST_DATA gUnknown_089A39E0[];

extern u8 CONST_DATA gUnknown_089AF950[];

extern u16 CONST_DATA gUnknown_089AFFB8[];
extern u16 CONST_DATA gUnknown_089AFF78[];

extern u8 CONST_DATA gUnknown_089AFFF8[];
extern short CONST_DATA gUnknown_089A3A18[];

extern struct ProcCmd CONST_DATA gUnknown_089A3A40[];

extern struct ProcCmd CONST_DATA gUnknown_089A3A80[];

extern u8 CONST_DATA gUnknown_089B35D0[];
extern u16 CONST_DATA gUnknown_089B4BFC[];

struct Unk0820579C { s8 framenum; u8 duration, alpha; };
extern struct Unk0820579C CONST_DATA gUnknown_0820579C[];

extern CONST_DATA u8* CONST_DATA gUnknown_089A3AB8[];

extern struct ProcCmd CONST_DATA gUnknown_089A3B3C[];

extern u8 CONST_DATA gUnknown_089AF310[]; // this is ?
extern u8 CONST_DATA gUnknown_089A3B6C[]; // this is an actual array of u8s, not arbitrary data

extern struct ProcCmd CONST_DATA gUnknown_089A3B84[];

extern struct ProcCmd CONST_DATA gUnknown_089A3BA4[];

extern struct ProcCmd CONST_DATA gUnknown_089A3BDC[];

extern u8 CONST_DATA gUnknown_089AE224[];
extern u16 CONST_DATA gUnknown_089AE484[];

extern u8 CONST_DATA gUnknown_089AE4A4[];
extern u8 CONST_DATA gUnknown_089A3C0C[];

extern struct ProcCmd CONST_DATA gUnknown_089A3C24[];

extern u8 CONST_DATA gUnknown_089B03D4[];
extern u16 CONST_DATA gUnknown_089B068C[];
extern u16 CONST_DATA gUnknown_089A61F8[]; // ap

extern struct ProcCmd CONST_DATA gUnknown_089A3C6C[];

extern u8 CONST_DATA gUnknown_089B1E10[];
extern u16 CONST_DATA gUnknown_089B20E8[];
extern u16 CONST_DATA gUnknown_089B2108[];

extern struct ProcCmd CONST_DATA gUnknown_089A3C94[];

extern u8 CONST_DATA gUnknown_089B0AD0[];
extern u16 CONST_DATA gUnknown_086810B8[];

extern u8 CONST_DATA gUnknown_089B17A4[];
extern u8 CONST_DATA gUnknown_08205824[]; // actual array of u8s

extern const u8 gUnknown_08205833[];

extern struct ProcCmd CONST_DATA gUnknown_089A3CD4[];

extern u16 CONST_DATA gUnknown_0867B5A4[];

extern u8 CONST_DATA gUnknown_089B0864[];
extern const u8 gUnknown_08205846[];

extern struct ProcCmd CONST_DATA gUnknown_089A3D14[];

extern u8 CONST_DATA gUnknown_08686F84[];
extern u16 CONST_DATA gUnknown_0868716C[];
extern u16 CONST_DATA gUnknown_089B1A10[];

extern struct ProcCmd CONST_DATA gUnknown_089A3D54[];

extern u8 CONST_DATA gUnknown_089B6AA0[];
extern u16 CONST_DATA gUnknown_089B73D4[];

extern u8 CONST_DATA* CONST_DATA gUnknown_089A3D8C[];

extern struct ProcCmd CONST_DATA gUnknown_089A3DB4[];

extern struct ProcCmd CONST_DATA gUnknown_089A52FC[];

extern u8 CONST_DATA gUnknown_089A3DFC[];

extern struct ProcCmd CONST_DATA gUnknown_089A3E04[];

extern u8 CONST_DATA gUnknown_089B2618[]; // img (1d)
extern u8 CONST_DATA gUnknown_089B21D8[]; // img (2d)
extern u16 CONST_DATA gUnknown_089B2728[]; // pal
extern u16 CONST_DATA gUnknown_089B2748[]; // ap

extern struct ProcCmd CONST_DATA gUnknown_089A3E44[];

extern u8 CONST_DATA gUnknown_089B2880[]; // img
extern u16 CONST_DATA gUnknown_0868DF5C[]; // pal

extern u8 CONST_DATA gUnknown_089B343C[];
extern const u8 gUnknown_08205855[];

extern struct ProcCmd CONST_DATA gUnknown_089A3E6C[];

extern u8 CONST_DATA gUnknown_089B0720[];
extern u8 CONST_DATA gUnknown_089B0840[];
extern u8 CONST_DATA gUnknown_089B06AC[];
extern u16 CONST_DATA gUnknown_089B0700[];
extern u16 CONST_DATA gUnknown_089B0820[];

extern u16 CONST_DATA gUnknown_089A6254[]; // ap

extern u16 CONST_DATA gUnknown_089A5314[]; // ap
extern u8 CONST_DATA gUnknown_089ACC98[];

void sub_801498C(const void*, void*, int);

extern struct ProcCmd CONST_DATA gUnknown_089A3F74[]; // MALevelUp

extern u8 gUnknown_02022968[];
extern u16 gUnknown_03005110[];
extern u16 CONST_DATA gUnknown_0859A120[];

extern u16 CONST_DATA gUnknown_089AC5CC[];

extern u8 CONST_DATA gUnknown_089AC794[]; // spr img
extern u16 CONST_DATA gUnknown_089AC9A8[]; // 3 palettes
extern u16 CONST_DATA gUnknown_089A5A6C[]; // ap

extern struct ProcCmd CONST_DATA gUnknown_089A4034[];

extern struct ProcCmd CONST_DATA gUnknown_089A404C[];
extern struct ProcCmd CONST_DATA gUnknown_089A4064[]; // shakes bg0

extern u8 CONST_DATA gUnknown_089AE7A4[]; // spark img
extern u16 CONST_DATA gUnknown_089AE7C4[]; // spark pal

extern struct Unk03005090 gUnknown_03005090[];

extern struct ProcCmd CONST_DATA gUnknown_089A407C[]; // wrap star effect thingy

extern struct ProcCmd CONST_DATA gUnknown_089A4394[]; // unk

// TODO: use those also elsewhere
#define UNIT_XTILE(aUnit) (((aUnit)->xPos - (gUnknown_0202BCB0.camera.x >> 4))*2)
#define UNIT_YTILE(aUnit) (((aUnit)->yPos - (gUnknown_0202BCB0.camera.y >> 4))*2)

void sub_807CAD0(struct Proc* proc)
{
    if (gMapBattle.hitIt->info & BATTLE_HIT_INFO_END)
    {
        Proc_GotoLabel(proc, 1); // TODO: label definitions
        return;
    }

    MapAnim_AdvanceBattleRound();
    Proc_ClearNativeCallback(proc);
}

void sub_807CB04(void)
{
    MU_AllRestartAnimations();
    sub_8003D20();
    DeleteBattleAnimInfoThing();
    SetupBackgroundForWeatherMaybe();
    LoadLegacyUiFrameGraphics();
    LoadObjUIGfx();
}

void MapAnim_BeginMISSAnim(struct Unit* unit)
{
    CopyDataWithPossibleUncomp(
        gUnknown_089AC0A4,
        OBJ_VRAM0 + 0x20 * BM_OBJCHR_BANIM_EFFECT);

    APProc_Create(
        gUnknown_089AC194,
        16*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)) + 8,
        16*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)) + 16,
        TILEREF(BM_OBJCHR_BANIM_EFFECT, 0), 0, 2);
}

void MapAnim_BeginNODAMAGEAnim(struct Unit* unit)
{
    CopyDataWithPossibleUncomp(
        gUnknown_089AC2FC,
        OBJ_VRAM0 + 0x20 * BM_OBJCHR_BANIM_EFFECT);

    APProc_Create(
        gUnknown_089AC440,
        16*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)) + 8,
        16*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)) + 16,
        TILEREF(BM_OBJCHR_BANIM_EFFECT, 0), 0, 2);
}

void MapAnim_BeginWallBreakAnim(struct Unit* unit, int unk)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3924, ROOT_PROC_3);

    proc->unit = unit;

    proc->xDisplay = 16*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)) + 8;
    proc->yDisplay = 16*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)) - 8;

    proc->unk48 = unk ^ 1;
}

void WallBreakAnim_Init(struct MAEffectProc* proc)
{
    CopyDataWithPossibleUncomp(
        gUnknown_089ADA80,
        OBJ_VRAM0 + 0x20 * BM_OBJCHR_BANIM_EFFECT);

    ApplyPalette(
        gUnknown_089ADD0C,
        16 + BM_OBJPAL_BANIM_EFFECT1);

    APProc_Create(
        gUnknown_089A6FD8,
        proc->xDisplay, proc->yDisplay + 16,
        TILEREF(BM_OBJCHR_BANIM_EFFECT, BM_OBJPAL_BANIM_EFFECT1),
        proc->unk48, 2);
}

void NewMapPoisonEffect(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3944, ROOT_PROC_3);

    proc->unit = unit;

    proc->xDisplay = 8*(1+2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)));
    proc->yDisplay = 8*(1+2*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)));
}

void MapAnim_BeginPoisonAnim(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0xB7, proc->xDisplay); // TODO: song ids

    CopyDataWithPossibleUncomp(
        gUnknown_089ADEB0,
        OBJ_VRAM0 + 0x20 * BM_OBJCHR_BANIM_EFFECT2);

    ApplyPalette(
        gUnknown_089AE204,
        16 + BM_OBJPAL_BANIM_EFFECT2);

    APProc_Create(
        gUnknown_089A6F40,
        proc->xDisplay - 8, proc->yDisplay + 8,
        TILEREF(BM_OBJCHR_BANIM_EFFECT2, BM_OBJPAL_BANIM_EFFECT2),
        0, 2);
}

void sub_807CD18(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3964, ROOT_PROC_3);

    proc->unit = unit;

    proc->xDisplay = 8*(1+2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)));
    proc->yDisplay = 8*(1+2*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)));
}

void sub_807CD60(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0xB7, proc->xDisplay); // TODO: song ids

    CopyDataWithPossibleUncomp(
        gUnknown_089ADEB0,
        OBJ_VRAM0 + 0x20 * BM_OBJCHR_BANIM_EFFECT2);

    ApplyPalette(
        gUnknown_089AE204,
        16 + BM_OBJPAL_BANIM_EFFECT2);

    APProc_Create(
        gUnknown_089A6F40,
        proc->xDisplay - 8, proc->yDisplay + 8,
        TILEREF(BM_OBJCHR_BANIM_EFFECT2, BM_OBJPAL_BANIM_EFFECT2),
        0, 2);
}

void sub_807CDB8(void)
{
    RefreshEntityBmMaps();
    RenderBmMap();
    SMS_UpdateFromGameData();
    MU_EndAll();
}

void sub_807CDD0(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A398C, ROOT_PROC_3);

    proc->unit = unit;

    proc->xDisplay = 8*(1+2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)));
    proc->yDisplay = 8*(1+2*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)));
}

void sub_807CE18(struct MAEffectProc* proc)
{
    sub_807E978();

    BG_SetPosition(2, 0, 0);

    // TODO: BM_BANIM_BGCHR_...
    CopyDataWithPossibleUncomp(
        gUnknown_089B7610,
        (void*)(VRAM) + GetBackgroundTileDataOffset(2) + BM_BGCHR_BANIM_UNK160 * 0x20);

    ApplyPalette(
        gUnknown_089B80C4,
        BM_BGPAL_BANIM_UNK4);

    SetSpecialColorEffectsParameters(1, 16, 16, 0);

    proc->unk40 = 0;
    proc->unk42 = 0;

    EnablePaletteSync();
}

void sub_807CE78(struct MAEffectProc* proc)
{
    if (proc->unk42 == 0)
    {
        if (proc->unk40 == 0)
            PlaySpacialSoundMaybe(0x3CA, proc->xDisplay); // TODO: song ids

        else if (proc->unk40 == 1)
            sub_807CF30(proc);

        else if (proc->unk40 > 6)
        {
            Proc_ClearNativeCallback((struct Proc*) proc);
            return;
        }

        CopyDataWithPossibleUncomp(
            gUnknown_089A39C4[proc->unk40],
            gUnknown_02020188);

        sub_800159C(
            gBG2TilemapBuffer,
            (u16*) gUnknown_02020188,
            proc->xDisplay/8 - 8,
            proc->yDisplay/8 - 7,
            TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4));

        BG_EnableSyncByMask(BG2_SYNC_BIT);

        proc->unk40++;
        proc->unk42 = 5;
    }

    proc->unk42--;
}

void sub_807CF30(struct MAEffectProc* proc)
{
    int unused = DivRem(AdvanceGetLCGRNValue(), 101);

    struct Trap* trap = GetTrapAt(proc->unit->xPos, proc->unit->yPos);

    int level = trap
        ? (u8) trap->data[TRAP_EXTDATA_EGG_LEVEL]
        : 1;

    u8 i;

    gUnknown_03001C68.charIndex = CHARACTER_MONSTER_BA;
    gUnknown_03001C68.classIndex = CLASS_GORGON;
    gUnknown_03001C68.leaderCharIndex = 0;
    gUnknown_03001C68.autolevel = TRUE;

    if (UNIT_FACTION(proc->unit) == FACTION_BLUE)
        gUnknown_03001C68.allegiance = 0;

    else if (UNIT_FACTION(proc->unit) == FACTION_RED)
        gUnknown_03001C68.allegiance = 2;

    else if (UNIT_FACTION(proc->unit) == FACTION_GREEN)
        gUnknown_03001C68.allegiance = 1;

    gUnknown_03001C68.level = level;

    gUnknown_03001C68.xPosition = proc->unit->xPos;
    gUnknown_03001C68.yPosition = proc->unit->yPos;

    gUnknown_03001C68.redaCount = 0;
    gUnknown_03001C68.redas = NULL;

    gUnknown_03001C68.genMonster = FALSE;
    gUnknown_03001C68.itemDrop = FALSE;

    for (i = 0; i < UNIT_DEFINITION_ITEM_COUNT; ++i)
        gUnknown_03001C68.items[i] = 0;

    for (i = 0; i < UNIT_DEFINITION_ITEM_COUNT; ++i)
        if (proc->unit->items[i+1])
            gUnknown_03001C68.items[i] = proc->unit->items[i+1];

    if (proc->unit->state & US_DROP_ITEM)
        gUnknown_03001C68.itemDrop = TRUE;

    gUnknown_03001C68.ai[0] = proc->unit->ai1;
    gUnknown_03001C68.ai[1] = proc->unit->ai2;
    gUnknown_03001C68.ai[2] = proc->unit->ai1data; // this looks incorrect
    gUnknown_03001C68.ai[3] = proc->unit->ai1data>>8; // this is 0

    LoadUnits(&gUnknown_03001C68);
    GetUnitFromCharId(CHARACTER_MONSTER_BA);

    ClearUnit(GetUnit(proc->unit->index));

    RefreshEntityBmMaps();
    RenderBmMap();
    SMS_UpdateFromGameData();
    MU_EndAll();
}

void sub_807D09C(void)
{
    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);
}

void sub_807D0B4(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A39E0, ROOT_PROC_3);

    proc->unit = unit;

    proc->xDisplay = 8*(1+2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)));
    proc->yDisplay = 8*(1+2*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)));
}

void sub_807D0FC(struct MAEffectProc* proc)
{
    sub_807E978();

    BG_SetPosition(2, 0, 0);

    // TODO: BM_BANIM_BGCHR_...
    CopyDataWithPossibleUncomp(
        gUnknown_089AF950,
        (void*)(VRAM) + GetBackgroundTileDataOffset(2) + BM_BGCHR_BANIM_UNK160 * 0x20);

    SetSpecialColorEffectsParameters(1, 16, 16, 0);

    sub_8001ED0(0, 0, 1, 0, 0);
    sub_8001F48(0);

    sub_8001F0C(0, 0, 0, 1, 1);
    sub_8001F64(1);

    proc->unk40 = 0;
    proc->unk42 = 0;

    if (GetItemIndex(gMapBattle.actor[0].bu->weaponBefore) == ITEM_STAFF_LATONA)
        sub_80144CC(gUnknown_089AFFB8, 0x80, 0x20, 2, (struct Proc*) proc);
    else
        sub_80144CC(gUnknown_089AFF78, 0x80, 0x20, 2, (struct Proc*) proc);
}

void sub_807D1B4(struct MAEffectProc* proc)
{
    if (proc->unk42 > 2)
    {
        DeleteAllPaletteAnimator();
        NewPaletteAnimator_(gUnknown_089AFF78, 0x80, 0x20, 4, (struct Proc*) proc);

        if (GetItemIndex(gMapBattle.actor[0].bu->weaponBefore) == ITEM_STAFF_LATONA)
            NewPaletteAnimator_(gUnknown_089AFFB8, 0x80, 0x20, 4, (struct Proc*) proc);
        else
            NewPaletteAnimator_(gUnknown_089AFF78, 0x80, 0x20, 4, (struct Proc*) proc);

        CopyDataWithPossibleUncomp(
            gUnknown_089AFFF8,
            (void*)(VRAM) + GetBackgroundTileDataOffset(2) + BM_BGCHR_BANIM_UNK160 * 0x20);

        sub_807D360(
            proc->xDisplay/8 - 4,
            proc->yDisplay/8 - 4,
            8, 60, 0, (struct Proc*) proc);

        Proc_ClearNativeCallback((struct Proc*) proc);

        PlaySpacialSoundMaybe(0x8C, proc->xDisplay); // TODO: song ids
    }
    else
    {
        int xOff = gUnknown_089A3A18[proc->unk42*2+0];
        int yOff = gUnknown_089A3A18[proc->unk42*2+1];

        sub_807D360(
            proc->xDisplay/8 + xOff - 3,
            proc->yDisplay/8 + yOff - 3,
            6, 10, 8, (struct Proc*) proc);

        PlaySpacialSoundMaybe(0x89, proc->xDisplay); // TODO: song ids

        proc->unk42++;
    }
}

void sub_807D2C8(void)
{
    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);
}

void sub_807D2E0(struct MAEffectProc* proc)
{
    DeleteAllPaletteAnimator();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    sub_8001ED0(1, 1, 1, 1, 1);
    sub_8001F48(1);

    proc->unk64 = 0x40;

    sub_807D328(proc);
}

void sub_807D328(struct MAEffectProc* proc)
{
    SetSpecialColorEffectsParameters(2, 0, 0, (proc->unk64--) >> 2);

    if (proc->unk64 == 0)
    {
        SetDefaultColorEffects();
        Proc_ClearNativeCallback((struct Proc*) proc);
    }
}

void sub_807D360(int a, int b, int c, int d, int e, struct Proc* parent)
{
    struct MAAnotherProc* proc =
        (struct MAAnotherProc*) Proc_CreateBlockingChild(gUnknown_089A3A40, parent);

    proc->unk2C = a;
    proc->unk30 = b;
    proc->unk54 = c;
    proc->unk58 = e;
    proc->unk44 = d;
}

void sub_807D39C(void)
{
    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);
}

void sub_807D3B4(void)
{
    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);
}

void sub_807D3CC(struct MAAnotherProc* proc)
{
    sub_8014560(
        gBG2TilemapBuffer,
        proc->unk2C, proc->unk30,
        TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4),
        proc->unk54, proc->unk54);

    BG_EnableSyncByMask(BG2_SYNC_BIT);

    SetSpecialColorEffectsParameters(1, 0, 0x10, 0);

    proc->unk64 = 0;
}

void sub_807D40C(struct MAAnotherProc* proc)
{
    proc->unk64 += 2;

    SetSpecialColorEffectsParameters(1, proc->unk64, 0x10, 0);

    if (proc->unk64 > 7)
    {
        proc->unk64 = 0;
        Proc_ClearNativeCallback((struct Proc*) proc);
    }
}

void sub_807D440(struct MAAnotherProc* proc)
{
    proc->unk44--;

    if (proc->unk44 == -1)
        Proc_ClearNativeCallback((struct Proc*) proc);
}

void sub_807D464(struct MAAnotherProc* proc)
{
    if (proc->unk58 == 0)
    {
        Proc_ClearNativeCallback((struct Proc*) proc);
        return;
    }

    SetSpecialColorEffectsParameters(1, sub_8012DCC(0, 8, 0, proc->unk64++, proc->unk58), 0x10, 0);

    if (proc->unk64 >= proc->unk58)
    {
        proc->unk64 = 0;

        BG_Fill(gBG2TilemapBuffer, 0);
        BG_EnableSyncByMask(BG2_SYNC_BIT);

        Proc_ClearNativeCallback((struct Proc*) proc);
    }
}

void sub_807D4D4(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3A80, ROOT_PROC_3);

    proc->unit = unit;

    proc->xDisplay = 8*(1+2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)));
    proc->yDisplay = 8*(1+2*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)));
}

void sub_807D51C(struct MAEffectProc* proc)
{
    sub_807E978();

    BG_SetPosition(2, 0, 0);

    CopyDataWithPossibleUncomp(
        gUnknown_089B35D0,
        (void*)(VRAM) + GetBackgroundTileDataOffset(2) + BM_BGCHR_BANIM_UNK160 * 0x20);

    ApplyPalette(
        gUnknown_089B4BFC,
        BM_BGPAL_BANIM_UNK4);

    SetSpecialColorEffectsParameters(1, 0x10, 0x10, 0);

    proc->unk40 = 0;
    proc->unk42 = 0;

    EnablePaletteSync();
}

void sub_807D57C(struct MAEffectProc* proc)
{
    if (proc->unk42 == 0)
    {
        if (proc->unk40 == 0)
            PlaySpacialSoundMaybe(0x384, proc->xDisplay); // TODO: song ids

        else if (proc->unk40 > 33)
        {
            Proc_ClearNativeCallback((struct Proc*) proc);
            return;
        }

        if (gUnknown_0820579C[proc->unk40].framenum < 0)
        {
            BG_Fill(gBG2TilemapBuffer, 0);
            BG_EnableSyncByMask(BG2_SYNC_BIT);
        }
        else
        {
            CopyDataWithPossibleUncomp(
                gUnknown_089A3AB8[gUnknown_0820579C[proc->unk40].framenum],
                gUnknown_02020188);

            sub_800159C(
                gBG2TilemapBuffer,
                (u16*) gUnknown_02020188,
                proc->xDisplay/8 - 9,
                proc->yDisplay/8 - 9,
                TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4));

            BG_EnableSyncByMask(BG2_SYNC_BIT);
        }

        SetSpecialColorEffectsParameters(1, gUnknown_0820579C[proc->unk40].alpha, 0x10, 0);

        proc->unk42 = gUnknown_0820579C[proc->unk40].duration;
        proc->unk40++;
    }

    proc->unk42--;
}

void sub_807D670(void)
{
    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);
}

void sub_807D688(struct Unit* unit, const void* img, const void* pal)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3B3C, ROOT_PROC_3);

    proc->unit = unit;

    proc->xDisplay = 8*(1+2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)));
    proc->yDisplay = 8*(1+2*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)));

    proc->img = img;
    proc->pal = pal;
}

void sub_807D6D8(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0xB6, proc->xDisplay); // TODO: song ids!

    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 1;
    gLCDControlBuffer.bg2cnt.priority = 1;
    gLCDControlBuffer.bg3cnt.priority = 2;

    BG_SetPosition(2, 0, 0);

    CopyDataWithPossibleUncomp(
        proc->img,
        (void*)(VRAM) + GetBackgroundTileDataOffset(2) + BM_BGCHR_BANIM_UNK160 * 0x20);

    ApplyPalette(
        proc->pal,
        BM_BGPAL_BANIM_UNK4);

    sub_807E978();

    SetSpecialColorEffectsParameters(1, 0x10, 0x10, 0);

    proc->unk40 = 0;
}

void sub_807D760(struct MAEffectProc* proc)
{
    sub_80146A0(
        gBG2TilemapBuffer,
        proc->xDisplay/8 - 3, proc->yDisplay/8 - 3,
        TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4),
        6, 6,
        gUnknown_089AF310,
        gUnknown_089A3B6C[proc->unk40/2]);

    BG_EnableSyncByMask(BG2_SYNC_BIT);

    proc->unk40++;

    if (gUnknown_089A3B6C[proc->unk40/2] == 0xFF)
        Proc_ClearNativeCallback((struct Proc*) proc);
}

void sub_807D7D8(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3B84, ROOT_PROC_3);

    proc->xDisplay = 16*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)) + 8;
    proc->yDisplay = 16*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)) + 8;
}

void sub_807D818(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0x10F, proc->xDisplay); // TODO: song ids!
}

void NewMapAnimEffectAnimator(struct Unit* unit, int b, int c, u16 d)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3BA4, ROOT_PROC_3);

    proc->unit = unit;

    proc->img = (void*) b;
    proc->pal = (void*) c;

    proc->unk58 = d;
}

void sub_807D860(struct MAEffectProc* proc)
{
    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 1;
    gLCDControlBuffer.bg2cnt.priority = 1;
    gLCDControlBuffer.bg3cnt.priority = 2;

    BG_SetPosition(2, 0, 0);

    CopyDataWithPossibleUncomp(
        proc->img,
        (void*)(VRAM) + GetBackgroundTileDataOffset(2) + BM_BGCHR_BANIM_UNK160 * 0x20);

    sub_8014560(
        gBG2TilemapBuffer,
        2*(proc->unit->xPos - (gUnknown_0202BCB0.camera.x>>4)) - 2,
        2*(proc->unit->yPos - (gUnknown_0202BCB0.camera.y>>4)) - 2,
        TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4),
        6, 6);

    BG_EnableSyncByMask(BG2_SYNC_BIT);

    NewPaletteAnimator_(proc->pal, 0x20 * BM_BGPAL_BANIM_UNK4, 0x20, 4, (struct Proc*) proc);

    proc->unk40 = 0;
    proc->unk42 = 0x10;

    PlaySpacialSoundMaybe(proc->unk58, 8*(1+2*(proc->unit->xPos - (gUnknown_0202BCB0.camera.x>>4))));
}

void sub_807D944(struct MAEffectProc* proc)
{
    proc->unk40++;

    if (proc->unk40 == 0x10)
        Proc_ClearNativeCallback((struct Proc*) proc);

    proc->unk42 = 0x16 - proc->unk40;

    if (proc->unk42 > 0x10)
        proc->unk42 = 0x10;

    SetSpecialColorEffectsParameters(1, proc->unk40, proc->unk42, 0);

    sub_8001ED0(0, 0, 1, 0, 0);
    sub_8001F48(0);

    sub_8001F0C(0, 0, 0, 1, 1);
    sub_8001F64(1);
}

void sub_807D9B8(struct MAEffectProc* proc)
{
    proc->unk40--;

    if (proc->unk40 == 0)
        Proc_ClearNativeCallback((struct Proc*) proc);

    proc->unk42 = 0x16 - proc->unk40;

    if (proc->unk42 > 0x10)
        proc->unk42 = 0x10;

    SetSpecialColorEffectsParameters(1, proc->unk40, proc->unk42, 0);

    sub_8001ED0(0, 0, 1, 0, 0);
    sub_8001F48(0);

    sub_8001F0C(0, 0, 0, 1, 1);
    sub_8001F64(1);
}

void sub_807DA2C(void)
{
    DeleteAllPaletteAnimator();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    SetDefaultColorEffects();
    SetWinEnable(0, 0, 0);
}

void sub_807DA68(void)
{
    SetPrimaryHBlankHandler(NULL);

    DeleteAllPaletteAnimator();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    SetDefaultColorEffects();
    SetWinEnable(0, 0, 0);
}

void New6C_SomethingFlashy(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3BDC, ROOT_PROC_3);

    proc->unit = unit;

    proc->xDisplay = 2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4));
    proc->yDisplay = 2*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4));
}

void sub_807DAE8(struct MAEffectProc* proc)
{
    BG_SetPosition(2, 0, 0);

    CopyDataWithPossibleUncomp(
        gUnknown_089AE224,
        (void*)(VRAM) + GetBackgroundTileDataOffset(2) + BM_BGCHR_BANIM_UNK160 * 0x20);

    ApplyPalette(
        gUnknown_089AE484,
        BM_BGPAL_BANIM_UNK4);

    LoadSparkGfx();

    proc->unk40 = 0;
}

void sub_807DB30(struct MAEffectProc* proc)
{
    sub_80146A0(
        gBG2TilemapBuffer,
        proc->xDisplay - 1, proc->yDisplay - 3,
        TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4),
        4, 6,
        gUnknown_089AE4A4,
        gUnknown_089A3C0C[proc->unk40/2]);

    BG_EnableSyncByMask(BG2_SYNC_BIT);

    proc->unk40++;

    if (gUnknown_089A3C0C[proc->unk40/2] == 0xFF)
        Proc_ClearNativeCallback((struct Proc*) proc);

    sub_807E978();

    SetSpecialColorEffectsParameters(1, 12, 12, 0);
}

void sub_807DBA8(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3C24, ROOT_PROC_3);

    proc->unit = unit;

    proc->xDisplay = 16*gActionData.xOther - gUnknown_0202BCB0.camera.x;
    proc->yDisplay = 16*gActionData.yOther - gUnknown_0202BCB0.camera.y;
}

void sub_807DBE4(struct MAEffectProc* proc)
{
    PlaySoundEffect(0xB3); // TODO: song ids!

    CopyDataWithPossibleUncomp(
        gUnknown_089B03D4,
        OBJ_VRAM0 + 0x20 * BM_OBJCHR_BANIM_EFFECT2);

    ApplyPalette(
        gUnknown_089B068C,
        16 + BM_OBJPAL_BANIM_EFFECT2);

    sub_8013AA4(4);

    sub_8014930(
        (void*)(VRAM) + GetBackgroundTileDataOffset(2) + 0x20 * BM_BGCHR_BANIM_UNK160,
        0x10, 0xFFFF);

    sub_8014930(
        gBG2TilemapBuffer,
        0x400, TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4));

    BG_EnableSyncByMask(BG2_SYNC_BIT);

    proc->unk40 = 0;
    proc->unk42 = 0;

    if (proc->xDisplay + 4 >= 0)
        APProc_Create(
            gUnknown_089A61F8,
            proc->xDisplay+4, proc->yDisplay,
            TILEREF(BM_OBJCHR_BANIM_EFFECT2, BM_OBJPAL_BANIM_EFFECT2),
            0, 2);

    sub_8081E78();
    sub_8081EAC();
    sub_807E978();

    SetSpecialColorEffectsParameters(1, 0, 0x10, 0);
}

void sub_807DCA8(struct MAEffectProc* proc)
{
    int tmp;

    int unk = sub_8012DCC(5, 1, 0xA0, proc->unk40, 0x50);
    sub_8081F24(proc->xDisplay + 8 , proc->yDisplay + 8, unk);

    proc->unk40++;

    tmp = proc->unk40*16/40;

    if (tmp >= 0x10)
        tmp = 0x10;

    SetSpecialColorEffectsParameters(1, tmp, 0x10, 0);

    if (proc->unk40 >= 40)
    {
        Proc_ClearNativeCallback((struct Proc*) proc);
        APProc_DeleteAll();
    }
}

void sub_807DD0C(struct MAEffectProc* proc)
{
    int tmp;

    int unk = sub_8012DCC(5, 1, 0xA0, proc->unk40, 0x50);
    sub_8081F24(proc->xDisplay + 8 , proc->yDisplay + 8, unk);

    proc->unk40++;

    tmp = 0x10 - (proc->unk40 - 40)*0x10/30;

    if (tmp <= 0)
        tmp = 0;

    SetSpecialColorEffectsParameters(1, tmp, 0x10, 0);

    if (proc->unk40 >= 70)
        Proc_ClearNativeCallback((struct Proc*) proc);
}

void sub_807DD74(void)
{
    sub_8081F58();
}

void sub_807DD80(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3C6C, ROOT_PROC_3);

    proc->unit = unit;

    proc->xDisplay = 8*(1+2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)));
    proc->yDisplay = 8*(1+2*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)));
}

void sub_807DDC8(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0x87, proc->xDisplay); // TODO: song ids

    BG_SetPosition(2, 0, 0);
    sub_807E978();

    CopyDataWithPossibleUncomp(
        gUnknown_089B1E10,
        OBJ_VRAM0 + 0x20 * BM_OBJCHR_BANIM_EFFECT2);

    ApplyPalette(
        gUnknown_089B20E8,
        16 + BM_OBJPAL_BANIM_EFFECT2);

    APProc_Create(
        gUnknown_089B2108,
        proc->xDisplay, proc->yDisplay,
        TILEREF(BM_OBJCHR_BANIM_EFFECT2, BM_OBJPAL_BANIM_EFFECT2),
        0, 2);

    proc->unk48 = 1;
}

void sub_807DE30(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3C94, ROOT_PROC_3);

    proc->xDisplay = 16*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)) + 8;
    proc->yDisplay = 16*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)) + 8;
}

void sub_807DE70(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0x86, proc->xDisplay);
}

void sub_807DE80(struct MAEffectProc* proc)
{
    BG_SetPosition(2, 0, 0);
    sub_807E978();

    SetSpecialColorEffectsParameters(1, 0x10, 0x10, 0);

    CopyDataWithPossibleUncomp(
        gUnknown_089B0AD0,
        (void*)(VRAM) + GetBackgroundTileDataOffset(2) + BM_BGCHR_BANIM_UNK160 * 0x20);

    ApplyPalette(
        gUnknown_086810B8,
        BM_BGPAL_BANIM_UNK4);

    proc->unk48 = 0;
    proc->unk4A = 0;
}

void sub_807DEDC(struct MAEffectProc* proc)
{
    sub_801474C(
        gBG2TilemapBuffer,
        proc->xDisplay/8 - 2,
        proc->yDisplay/8 - 9,
        TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4),
        4, 11,
        gUnknown_089B17A4,
        gUnknown_08205824[proc->unk48++]);

    BG_EnableSyncByMask(BG2_SYNC_BIT);

    if (gUnknown_08205824[proc->unk48] == 0xFF)
        Proc_ClearNativeCallback((struct Proc*) proc);
}

void sub_807DF5C(struct MAEffectProc* proc)
{
    u8 array[0x13];
    memcpy(array, gUnknown_08205833, sizeof array); // TODO: inline array

    SetSpecialColorEffectsParameters(1, array[proc->unk4A++], 0x10, 0);

    if (array[proc->unk4A] == 0xFF)
    {
        proc->unk4A = 0;
        Proc_ClearNativeCallback((struct Proc*) proc);
    }
}

void sub_807DFAC(struct MAEffectProc* proc)
{
    SetSpecialColorEffectsParameters(1, sub_8012DCC(0, 0x10, 0, proc->unk4A++, 30), 0x10, 0);

    if (proc->unk4A > 30)
        Proc_ClearNativeCallback((struct Proc*) proc);
}

void sub_807DFF4(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3CD4, ROOT_PROC_3);

    proc->xDisplay = 8*(1+2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)));
    proc->yDisplay = 8*(1+2*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)));
}

void sub_807E038(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0x82, proc->xDisplay);

    ApplyPalette(
        gUnknown_0867B5A4,
        BM_BGPAL_BANIM_UNK4);
}

void sub_807E054(struct MAEffectProc* proc)
{
    sub_801474C(
        gBG2TilemapBuffer,
        proc->xDisplay/8 - 2,
        proc->yDisplay/8 - 9,
        TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4),
        4, 11,
        gUnknown_089B0864,
        gUnknown_08205846[proc->unk48++]);

    BG_EnableSyncByMask(BG2_SYNC_BIT);

    if (gUnknown_08205846[proc->unk48] == 0xFF)
        Proc_ClearNativeCallback((struct Proc*) proc);
}

void sub_807E0D4(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3D14, ROOT_PROC_3);

    proc->xDisplay = 8*(1+2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)));
    proc->yDisplay = 8*(1+2*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)));
}

void sub_807E118(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0x85, proc->xDisplay); // TODO: song ids

    BG_SetPosition(2, 0, 0);
    sub_807E978();

    CopyDataWithPossibleUncomp(
        gUnknown_08686F84,
        OBJ_VRAM0 + 0x20 * BM_OBJCHR_BANIM_EFFECT2);

    ApplyPalette(
        gUnknown_0868716C,
        16 + BM_OBJPAL_BANIM_EFFECT2);

    APProc_Create(
        gUnknown_089B1A10,
        proc->xDisplay, proc->yDisplay - 16,
        TILEREF(BM_OBJCHR_BANIM_EFFECT2, BM_OBJPAL_BANIM_EFFECT2),
        0, 2);
}

void sub_807E17C(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0x85, proc->xDisplay); // TODO: song ids

    APProc_Create(
        gUnknown_089B1A10,
        proc->xDisplay, proc->yDisplay - 8,
        TILEREF(BM_OBJCHR_BANIM_EFFECT2, BM_OBJPAL_BANIM_EFFECT2),
        0, 2);
}

void sub_807E1B0(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0x85, proc->xDisplay); // TODO: song ids

    APProc_Create(
        gUnknown_089B1A10,
        proc->xDisplay, proc->yDisplay,
        TILEREF(BM_OBJCHR_BANIM_EFFECT2, BM_OBJPAL_BANIM_EFFECT2),
        0, 2);
}

void sub_807E1E4(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3D54, ROOT_PROC_3);

    proc->unit = unit;

    proc->xDisplay = 8*(1+2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)));
    proc->yDisplay = 8*(1+2*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)));
}

void sub_807E22C(struct MAEffectProc* proc)
{
    sub_807E978();

    BG_SetPosition(2, 0, 0);

    CopyDataWithPossibleUncomp(
        gUnknown_089B6AA0,
        (void*)(VRAM) + GetBackgroundTileDataOffset(2) + BM_BGCHR_BANIM_UNK160 * 0x20);

    ApplyPalette(
        gUnknown_089B73D4,
        BM_BGPAL_BANIM_UNK4);

    SetSpecialColorEffectsParameters(1, 0x10, 0x10, 0);

    proc->unk40 = 0;
    proc->unk42 = 0;

    EnablePaletteSync();
}

void sub_807E28C(struct MAEffectProc* proc)
{
    if (proc->unk42 == 0)
    {
        if (proc->unk40 == 0)
            PlaySpacialSoundMaybe(0x3BA, proc->xDisplay); // TODO: song ids

        else if (proc->unk40 >= 10)
            Proc_ClearNativeCallback((struct Proc*) proc);

        CopyDataWithPossibleUncomp(
            gUnknown_089A3D8C[proc->unk40],
            gUnknown_02020188);

        sub_800159C(
            gBG2TilemapBuffer,
            (u16*) gUnknown_02020188,
            proc->xDisplay/8 - 4,
            proc->yDisplay/8 - 4,
            TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4));

        BG_EnableSyncByMask(BG2_SYNC_BIT);

        proc->unk40++;
        proc->unk42 = 4;
    }

    proc->unk42--;
}

void sub_807E334(void)
{
    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);
}

void sub_807E34C(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3DB4, ROOT_PROC_3);

    proc->xDisplay = 8*(1+2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)));
    proc->yDisplay = 16*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)) + 18;
}

void sub_807E390(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0xFD, proc->xDisplay); // TODO: song ids

    sub_8081E78();
    sub_8081EAC();

    SetPrimaryHBlankHandler(sub_8081FA8);

    sub_807E978();

    SetSpecialColorEffectsParameters(1, 0x10, 0x10, 0);

    CallARM_FillTileRect(
        gBG2TilemapBuffer,
        (u16*) gUnknown_02020188,
        TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4));

    BG_EnableSyncByMask(BG2_SYNC_BIT);

    sub_807EA20(2, 0, 1, (struct Proc*) proc);
    Proc_Create(gUnknown_089A52FC, (struct Proc*) proc);

    proc->unk48 = 0;
    proc->unk4A = 0;
}

void sub_807E404(struct MAEffectProc* proc)
{
    if (proc->unk48 >= 12)
    {
        proc->unk48--;
        Proc_ClearNativeCallback((struct Proc*) proc);
    }

    sub_808218C(
        proc->xDisplay,
        proc->yDisplay,
        ++proc->unk48, 12,
        gUnknown_089A3DFC);
}

void sub_807E448(struct MAEffectProc* proc)
{
    if (proc->unk48 <= 0)
    {
        proc->unk48++;
        Proc_ClearNativeCallback((struct Proc*) proc);
    }

    sub_808218C(
        proc->xDisplay,
        proc->yDisplay,
        --proc->unk48, 12,
        gUnknown_089A3DFC);
}

void sub_807E48C(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3E04, ROOT_PROC_3);

    proc->xDisplay = 8*(1+2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)));
    proc->yDisplay = 8*(1+2*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)));
}

void sub_807E4D0(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0x83, proc->xDisplay); // TODO: song ids

    BG_SetPosition(2, 0, 0);

    sub_807E978();

    CopyDataWithPossibleUncomp(
        gUnknown_089B2618,
        (void*)(VRAM) + GetBackgroundTileDataOffset(2) + BM_BGCHR_BANIM_UNK160 * 0x20);

    CopyDataWithPossibleUncomp(
        gUnknown_089B21D8,
        OBJ_VRAM0 + BM_OBJCHR_BANIM_EFFECT2 * 0x20);

    ApplyPalette(
        gUnknown_089B2728,
        BM_BGPAL_BANIM_UNK4);

    ApplyPalette(
        gUnknown_089B2728,
        16 + BM_OBJPAL_BANIM_EFFECT2);

    APProc_Create(
        gUnknown_089B2748,
        proc->xDisplay, proc->yDisplay | 0x400,
        TILEREF(BM_OBJCHR_BANIM_EFFECT2, BM_OBJPAL_BANIM_EFFECT2),
        0, 2);

    proc->unk48 = 0;

    sub_8001ED0(0, 0, 0, 0, 1);
    sub_8001F48(0);

    SetSpecialColorEffectsParameters(0, 0x10, 0x10, 0);
}

void sub_807E584(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0x84, proc->xDisplay);

    sub_8014560(
        gBG2TilemapBuffer,
        proc->xDisplay/8 - 2,
        proc->yDisplay/8 - 2,
        TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4),
        4, 4);

    BG_EnableSyncByMask(BG2_SYNC_BIT);

    sub_8001ED0(0, 0, 1, 0, 0);
    sub_8001F48(0);

    SetSpecialColorEffectsParameters(1, 0x10, 0x10, 0);
}

void sub_807E5F0(struct MAEffectProc* proc)
{
    SetSpecialColorEffectsParameters(1, sub_8012DCC(0, 0x10, 0, proc->unk48++, 30), 0x10, 0);

    if (proc->unk48 >= 30)
        Proc_ClearNativeCallback((struct Proc*) proc);
}

void sub_807E638(struct Unit* unit)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3E44, ROOT_PROC_3);

    proc->xDisplay = 8*(1+2*(unit->xPos - (gUnknown_0202BCB0.camera.x>>4)));
    proc->yDisplay = 8*(1+2*(unit->yPos - (gUnknown_0202BCB0.camera.y>>4)));
}

void sub_807E67C(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0x88, proc->xDisplay);

    BG_SetPosition(2, 0, 0);

    sub_807E978();

    SetSpecialColorEffectsParameters(1, 0x10, 0x10, 0);

    CopyDataWithPossibleUncomp(
        gUnknown_089B2880,
        (void*)(VRAM) + GetBackgroundTileDataOffset(2) + BM_BGCHR_BANIM_UNK160 * 0x20);

    ApplyPalette(
        gUnknown_0868DF5C,
        BM_BGPAL_BANIM_UNK4);

    proc->unk48 = 0;
    proc->unk4A = 0;
}

void sub_807E6E0(struct MAEffectProc* proc)
{
    sub_801474C(
        gBG2TilemapBuffer,
        proc->xDisplay/8 - 2,
        proc->yDisplay/8 - 8,
        TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4),
        4, 10,
        gUnknown_089B343C,
        gUnknown_08205855[proc->unk48++]);

    BG_EnableSyncByMask(BG2_SYNC_BIT);

    if (gUnknown_08205855[proc->unk48] == 0xFF)
        Proc_ClearNativeCallback((struct Proc*) proc);
}

void sub_807E760(int xMap, int yMap)
{
    struct MAEffectProc* proc =
        (struct MAEffectProc*) Proc_Create(gUnknown_089A3E6C, ROOT_PROC_3);

    proc->xDisplay = 16*(xMap - (gUnknown_0202BCB0.camera.x>>4)) + 8;
    proc->yDisplay = 16*(yMap - (gUnknown_0202BCB0.camera.y>>4)) + 8;
}

void sub_807E79C(void)
{
    GetUnit(gActionData.subjectIndex)->state |= US_HIDDEN;
    sub_808320C((s8) gActionData.xOther, (s8) gActionData.yOther);
}

void sub_807E7C4(void)
{
    GetUnit(gActionData.subjectIndex)->state &= ~US_HIDDEN;
}

void sub_807E7E0(struct MAEffectProc* proc)
{
    PlaySpacialSoundMaybe(0x8D, proc->xDisplay); // TODO: song ids!

    BG_SetPosition(2, 0, 0);

    CopyDataWithPossibleUncomp(
        gUnknown_089B0720,
        (void*)(VRAM) + GetBackgroundTileDataOffset(2) + BM_BGCHR_BANIM_UNK160 * 0x20);

    sub_801474C(
        gBG2TilemapBuffer,
        proc->xDisplay/8 - 2,
        proc->yDisplay/8 - 2,
        TILEREF(BM_BGCHR_BANIM_UNK160, BM_BGPAL_BANIM_UNK4),
        4, 4,
        gUnknown_089B0840,
        0);

    BG_EnableSyncByMask(BG2_SYNC_BIT);

    CopyDataWithPossibleUncomp(
        gUnknown_089B06AC,
        OBJ_VRAM0 + BM_OBJCHR_BANIM_EFFECT2 * 0x20);

    ApplyPalette(
        gUnknown_089B0700,
        16 + BM_OBJPAL_BANIM_EFFECT2);

    sub_80144CC(gUnknown_089B0820, 0x80, 0x20, 4, (struct Proc*) proc);

    sub_8081E78();
    sub_8081EAC();
    sub_807E978();

    SetSpecialColorEffectsParameters(1, 0x10, 0x10, 0);

    proc->unk48 = 1;
}

void sub_807E8B0(struct MAEffectProc* proc)
{
    int unk = sub_8012DCC(5, 1, 0x10, proc->unk48, 30);

    proc->unk48++;

    sub_8081F24(proc->xDisplay, proc->yDisplay, unk);

    if (proc->unk48 >= 30)
    {
        proc->unk48 = 0;

        Proc_ClearNativeCallback((struct Proc*) proc);

        APProc_Create(
            gUnknown_089A6254,
            proc->xDisplay, proc->yDisplay,
            TILEREF(BM_OBJCHR_BANIM_EFFECT2, BM_OBJPAL_BANIM_EFFECT2),
            0, 2);

        APProc_Create(
            gUnknown_089A6254,
            proc->xDisplay, proc->yDisplay,
            TILEREF(BM_OBJCHR_BANIM_EFFECT2, BM_OBJPAL_BANIM_EFFECT2),
            1, 2);
    }
}

void sub_807E934(struct MAEffectProc* proc)
{
    int unk = sub_8012DCC(5, 0x10, 0, proc->unk48, 30);

    proc->unk48++;

    sub_8081F24(proc->xDisplay, proc->yDisplay, unk);

    if (proc->unk48 >= 30)
        Proc_ClearNativeCallback((struct Proc*) proc);
}

void sub_807E978(void)
{
    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 0;
    gLCDControlBuffer.bg2cnt.priority = 0;
    gLCDControlBuffer.bg3cnt.priority = 2;

    sub_8001ED0(0, 0, 1, 0, 0);
    sub_8001F48(0);

    sub_8001F0C(0, 0, 0, 1, 1);
    sub_8001F64(1);

    gLCDControlBuffer.wincnt.win0_enableBlend = 1;
    gLCDControlBuffer.wincnt.wout_enableBlend = 0;

    SetWin0Layers(1, 1, 1, 1, 1);
    SetWOutLayers(1, 1, 0, 1, 1);
}

// =========================================
// NOTE: this may be the start of a new file
// =========================================

struct MAUnkProc
{
    /* 00 */ PROC_HEADER;

    /* 29 */ u8 pad29[0x54 - 0x29];
    /* 54 */ int unk54;
    /* 58 */ int unk58;
    /* 5C */ u8 pad5C[0x64 - 0x5C];
    /* 64 */ short unk64;
    /* 66 */ short unk66;
    /* 68 */ short unk68;
    /* 6A */ short unk6A;
};

extern struct ProcCmd CONST_DATA gUnknown_089A3EC4[];

void sub_807EA20(int a, int b, int c, struct Proc* parent)
{
    struct MAUnkProc* proc =
        (struct MAUnkProc*) Proc_Create(gUnknown_089A3EC4, parent);

    proc->unk58 = a;
    proc->unk64 = 0;
    proc->unk66 = b;
    proc->unk68 = 0;
    proc->unk6A = c;
}

void sub_807EA50(void)
{
    Proc_DeleteAllWithScript(gUnknown_089A3EC4);
}

void sub_807EA60(struct MAUnkProc* proc)
{
    BG_SetPosition(proc->unk58, proc->unk64, proc->unk68);

    proc->unk64 += proc->unk66;
    proc->unk68 += proc->unk6A;
}

extern u8 CONST_DATA gUnknown_088035B0[]; // img
extern u8 CONST_DATA gUnknown_088039E8[]; // ???
extern u16 CONST_DATA gUnknown_08803B10[]; // pal

void sub_80149F0(const void*, u16* tilemap, int, int tileref);

struct Unk089A3ED4
{
    u8 x, y;
    const int* msgid[2];
};

extern struct Unk089A3ED4 CONST_DATA gUnknown_089A3ED4[];

int GetSomeStatBase(int actor, int statid);
int GetSomeStatUp(int actor, int statid);

struct MAStatGainEffectProc
{
    /* 00 */ PROC_HEADER;
    /* 2A */ u16 unk2A;
    /* 2C */ u16 unk2C;
    /* 2E */ u16 unk2E;
};

extern struct ProcCmd CONST_DATA gUnknown_089A3F4C[];

extern u8 CONST_DATA gUnknown_089ACA08[]; // img
extern u16 CONST_DATA gUnknown_089AC9A8[]; // pal

extern struct ProcCmd CONST_DATA gUnknown_089A3F5C[];

void sub_807EA98(int actor, int xTile, int yTile)
{
    int i;

    BG_Fill(gBG1TilemapBuffer, 0);

    CopyDataWithPossibleUncomp(
        gUnknown_088035B0,
        (void*)(VRAM) + GetBackgroundTileDataOffset(1) + BM_BGCHR_BANIM_UNK200 * 0x20);

    CopyDataWithPossibleUncomp(
        gUnknown_088039E8,
        gUnknown_02020188);

    sub_80149F0(
        gUnknown_02020188,
        gBG1TilemapBuffer,
        0x380, // TODO: ???
        TILEREF(BM_BGCHR_BANIM_UNK200, BM_BGPAL_BANIM_UNK5));

    ApplyPalette(
        gUnknown_08803B10,
        BM_BGPAL_BANIM_UNK5);

    sub_801443C(
        TILEMAP_LOCATED(
            gBG0TilemapBuffer,
            xTile + 2, yTile),
        TEXT_COLOR_NORMAL,
        GetStringFromIndex(UNIT_CLASS_NAME_ID(gMapBattle.actor[actor].unit)));

    for (i = 0; gUnknown_089A3ED4[i].x != 0xFF; ++i)
    {
        // whew
        sub_80143D8(
            TILEMAP_LOCATED(
                gBG0TilemapBuffer,
                xTile + gUnknown_089A3ED4[i].x,
                yTile + gUnknown_089A3ED4[i].y),
            3, 3,
            GetStringFromIndex(
                *(gUnknown_089A3ED4[i].msgid[
                    UnitHasMagicRank(gMapBattle.actor[actor].unit) == TRUE
                        ? 1 : 0])));
    }

    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT);
}

void sub_807EBA4(int actor, int xTile, int yTile, int statid, s8 isPostGain)
{
    u16* const tilemap = TILEMAP_LOCATED(
        gBG0TilemapBuffer,
        xTile + gUnknown_089A3ED4[statid].x + 4,
        yTile + gUnknown_089A3ED4[statid].y);

    int stat = GetSomeStatBase(actor, statid);

    if (isPostGain)
        stat += GetSomeStatUp(actor, statid);

    DrawDecNumber(tilemap, 2, stat);
}

int GetSomeStatUp(int actor, int statid)
{
    switch (statid)
    {

    case 0: return 1;
    case 1: return gMapBattle.actor[actor].bu->changeHP;
    case 2: return gMapBattle.actor[actor].bu->changePow;
    case 3: return gMapBattle.actor[actor].bu->changeSkl;
    case 4: return gMapBattle.actor[actor].bu->changeSpd;
    case 5: return gMapBattle.actor[actor].bu->changeLck;
    case 6: return gMapBattle.actor[actor].bu->changeDef;
    case 7: return gMapBattle.actor[actor].bu->changeRes;
    case 8: return gMapBattle.actor[actor].bu->changeCon;

    default:
        return 0;

    } // switch (statid)
}

int GetSomeStatBase(int actor, int statid)
{
    // ?? this seems unnecessary but maybe I'm missing something
    struct Unit* const unit = GetUnit(gMapBattle.actor[actor].unit->index);

    switch (statid)
    {

    case 0: return gMapBattle.actor[actor].bu->levelPrevious;
    case 1: return unit->maxHP;
    case 2: return unit->pow;
    case 3: return unit->skl;
    case 4: return unit->spd;
    case 5: return unit->lck;
    case 6: return unit->def;
    case 7: return unit->res;
    case 8: return UNIT_CON_BASE(unit);

    default:
        return 0;

    } // switch (statid)
}

void sub_807EDEC(void)
{
    APProc_DeleteAll();
}

void sub_807EDF8(int chr, int pal, int unk, struct Proc* parent)
{
    struct MAStatGainEffectProc* proc =
        (struct MAStatGainEffectProc*) Proc_Create(gUnknown_089A3F4C, parent);

    proc->unk2A = chr;
    proc->unk2C = pal;
    proc->unk2E = unk;

    CopyDataWithPossibleUncomp(
        gUnknown_089ACA08,
        OBJ_VRAM0 + (0x3FF & chr) * 0x20);

    ApplyPalette(
        gUnknown_089AC9A8,
        16 + pal);

    ApplyPalette(
        gUnknown_089AC9A8,
        16 + pal + 1);

    ((struct MAUnkProc*) Proc_Create(gUnknown_089A3F5C, (struct Proc*) proc))->unk64 = pal;
}

void sub_807EE74(void)
{
    Proc_DeleteAllWithScript(gUnknown_089A3F4C);
}

void sub_807EE84(int arg0, int arg1, int arg2, int arg3)
{
    int r6;
    const u8* unk = gUnknown_089ACC98;

    struct MAStatGainEffectProc* proc =
        (struct MAStatGainEffectProc*) Proc_Find(gUnknown_089A3F4C);

    int chrBase = proc->unk2A;
    int chr     = proc->unk2A + 2*(arg2-1);

    if (arg2 == 0)
    {
        APProc_Create(
            gUnknown_089A5314,
            arg0 - 18, arg1 - 4,
            chrBase + ((u16)(proc->unk2C & 0xF)) * 0x1000 + ((u16)(proc->unk2E & 3)) * 0x400,
            0, 2);
    }
    else
    {
        if (arg3 > 0)
            r6 = 0;
        else
            r6 = 1;

        APProc_Create(
            gUnknown_089A5314,
            arg0, arg1,
            chrBase + (0xF & (proc->unk2C + r6)) * 0x1000 + ((u16)(proc->unk2E & 3)) * 0x400,
            r6 + 1, 2);

        APProc_Create(
            gUnknown_089A5314,
            arg0 - 3, arg1,
            chr + ((u16)(proc->unk2C & 0xF)) * 0x1000 + ((u16)(proc->unk2E & 3)) * 0x400,
            r6 + 3, 2);

        if (arg3 > 0)
        {
            APProc_Create(
                gUnknown_089A5314,
                arg0 - 18, arg1 - 4,
                chrBase + ((u16)(proc->unk2C & 0xF)) * 0x1000 + ((u16)(proc->unk2E & 3)) * 0x400,
                0, 2);
        }

        if (arg3 < 0)
        {
            sub_801498C(
                &unk[32 << 5],
                OBJ_VRAM0 + (((chr + 0x4C) & 0x3FF) << 5),
                0x20);
        }

        sub_801498C(
            &unk[(abs(arg3) & 0x3FF) << 5],
            OBJ_VRAM0 + (((chr + 0x2D) & 0x3FF) << 5),
            0x20);

        sub_801498C(
            &unk[((abs(arg3) + 32) & 0x3FF) << 5],
            OBJ_VRAM0 + ((0x3FF & (chr + 0x4D)) << 5),
            0x20);
    }
}

void sub_807EFF0(int arg0, int arg1, int arg2)
{
    const u8* unk = gUnknown_089ACC98;

    int r9 = abs(arg2);

    struct MAStatGainEffectProc* proc =
        (struct MAStatGainEffectProc*) Proc_Find(gUnknown_089A3F4C);

    int r6 = proc->unk2A;

    int sp8;
    int r2;

    if (arg2 >= 0)
    {
        sp8 = r6 + (r9-1)*2;
        r2 = 5;
    }
    else
    {
        sp8 = r6 + 16;
        r2 = 6;
    }

    APProc_Create(
        gUnknown_089A5314,
        arg0, arg1,
        r6 + (0xF & (proc->unk2C)) * 0x1000 + ((u16)(proc->unk2E & 3)) * 0x400,
        r2, 2);

    APProc_Create(
        gUnknown_089A5314,
        arg0 - 3, arg1,
        sp8 + (0xF & (proc->unk2C)) * 0x1000 + ((u16)(proc->unk2E & 3)) * 0x400,
        3, 2);

    APProc_Create(
        gUnknown_089A5314,
        arg0 - 18, arg1 - 4,
        r6 + (0xF & (proc->unk2C)) * 0x1000 + ((u16)(proc->unk2E & 3)) * 0x400,
        0, 2);

    sub_801498C(
        &unk[(r9 & 0x3FF) << 5],
        OBJ_VRAM0 + ((0x3FF & (sp8 + 0x2D)) << 5),
        0x20);

    sub_801498C(
        &unk[((r9 + 0x20) & 0x3FF) << 5],
        OBJ_VRAM0 + ((0x3FF & (sp8 + 0x4D)) << 5),
        0x20);
}

void sub_807F10C(int actor, struct Proc* parent)
{
    // TODO
    struct MALevelUpProc* proc =
        (struct MALevelUpProc*) Proc_CreateBlockingChild(gUnknown_089A3F74, parent);

    proc->actor = actor;
}

void sub_807F124(void)
{
    SetWinEnable(1, 0, 0);

    // TODO: SetWin0PtA macro?
    gLCDControlBuffer.win0_left   = 0;
    gLCDControlBuffer.win0_top    = 0;

    // TODO: SetWin0PtB macro?
    gLCDControlBuffer.win0_right  = 240; // TODO: SCREEN_WIDTH?
    gLCDControlBuffer.win0_bottom = 48;

    SetWin0Layers(0, 0, 1, 1, 1);
    SetWOutLayers(1, 1, 1, 1, 1);
}

void sub_807F190(void)
{
    SetWinEnable(0, 0, 0);
}

void sub_807F1AC(struct MALevelUpProc* proc)
{
    int i;

    sub_8003D20();

    BG_Fill(gBG0TilemapBuffer, 0);

    sub_807EA98(proc->actor, 1, 1);

    for (i = 0; i < 9; ++i)
        sub_807EBA4(proc->actor, 1, 1, i, FALSE);

    BG_EnableSyncByMask(BG0_SYNC_BIT);

    proc->unk30 = 0;
    proc->unk31 = 0;

    proc->unk32 = -144;

    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 1;
    gLCDControlBuffer.bg2cnt.priority = 1;
    gLCDControlBuffer.bg3cnt.priority = 2;

    SetDefaultColorEffects();

    SetWinEnable(0, 0, 0);

    BG_SetPosition(0, 0, proc->unk32);
    BG_SetPosition(1, 0, proc->unk32);

    NewFace(0, gMapBattle.actor[proc->actor].unit->pCharacterData->portraitId, 184, 32 - proc->unk32, 0x1042);
    gUnknown_03004980[0]->yDisplay = 32 - proc->unk32;

    sub_807EDF8(BM_BGCHR_BANIM_UNK200, 3, 1, (struct Proc*) proc);
}

void sub_807F2B4(struct MAUnkProc* proc)
{
    proc->unk54 = 0;
}

void sub_807F2BC(struct MAUnkProc* proc)
{
    const u16* unk = gUnknown_089AC5CC;

    proc->unk54++;

    if ((proc->unk54 % 4) == 0)
    {
        int color = ((proc->unk54 >> 2) & 0xF);

        CopyToPaletteBuffer(unk + color,        (proc->unk64 + 0x10) * 0x20 + 9*sizeof(u16), 14);
        CopyToPaletteBuffer(unk + color + 0x20, (proc->unk64 + 0x11) * 0x20 + 9*sizeof(u16), 14);
    }
}

void sub_807F30C(struct MALevelUpProc* proc)
{
    proc->unk32 += 8;

    BG_SetPosition(0, 0, proc->unk32);
    BG_SetPosition(1, 0, proc->unk32);

    gUnknown_03004980[0]->yDisplay = 32 - proc->unk32;

    if (proc->unk32 >= -48)
        Proc_ClearNativeCallback((struct Proc*) proc);
}

void sub_807F354(struct MALevelUpProc* proc)
{
    proc->unk32 -= 8;

    BG_SetPosition(0, 0, proc->unk32);
    BG_SetPosition(1, 0, proc->unk32);

    gUnknown_03004980[0]->yDisplay = 32 - proc->unk32;

    if (proc->unk32 <= -144)
        Proc_ClearNativeCallback((struct Proc*) proc);
}

void sub_807F39C(struct MALevelUpProc* proc)
{
    int statid;

    if (proc->unk31 != 0)
    {
        proc->unk31--;
        return;
    }

    for (statid = proc->unk30; statid < 9; ++statid)
    {
        if (GetSomeStatUp(proc->actor, statid) != 0)
            break;
    }

    if (statid >= 9)
    {
        Proc_ClearNativeCallback((struct Proc*) proc);
        return;
    }

    sub_807EBA4(proc->actor, 1, 1, statid, TRUE);
    BG_EnableSyncByMask(BG0_SYNC_BIT);

    sub_807EE84(
        0x3E + gUnknown_089A3ED4[statid].x * 8,
        0x17 + gUnknown_089A3ED4[statid].y * 8 - proc->unk32,
        statid, GetSomeStatUp(proc->actor, statid));

    if (statid == 0)
    {
        PlaySoundEffect(0x02CD); // TODO: song ids
    }
    else
    {
        PlaySoundEffect(0x0076); // TODO: song ids
    }

    proc->unk30 = statid+1;
    proc->unk31 = 20;
}

void sub_807F478(struct MALevelUpProc* proc)
{
    ISuspectThisToBeMusicRelated_8002730(0x100, 0x80, 0x10, (struct Proc*) proc);
}

void sub_807F48C(struct MALevelUpProc* proc)
{
    int x, y;

    CopyDataWithPossibleUncomp(
        gUnknown_089AC794,
        OBJ_VRAM0 + BM_OBJCHR_BANIM_EFFECT2*0x20);

    ApplyPalettes(gUnknown_089AC9A8, 0x10 + BM_OBJPAL_BANIM_EFFECT1, 3);

    x = (UNIT_XTILE(gMapBattle.actor[proc->actor].unit) << 3) + 16;
    y = (UNIT_YTILE(gMapBattle.actor[proc->actor].unit) << 3) - 8;

    if (UNIT_YTILE(gMapBattle.actor[proc->actor].unit) < 4)
        y += 32;

    if (UNIT_XTILE(gMapBattle.actor[proc->actor].unit) < 4)
        x = 48;

    if (UNIT_XTILE(gMapBattle.actor[proc->actor].unit) > 25)
        x = 208;

    APProc_Create(
        gUnknown_089A5A6C,
        x, y, TILEREF(BM_OBJCHR_BANIM_EFFECT2, BM_OBJPAL_BANIM_EFFECT1),
        0, 2);

    PlaySoundEffect(0x5B);
}

void sub_807F53C(void)
{
    APProc_DeleteAll();
}

void sub_807F548(struct Proc* proc)
{
    ISuspectThisToBeMusicRelated_8002730(0x80, 0x100, 0x10, proc);
}

void sub_807F55C(void)
{
    sub_8010E50();
}

void sub_807F568(struct Proc* parent)
{
    if (parent)
        Proc_CreateBlockingChild(gUnknown_089A4034, parent);
    else
        Proc_Create(gUnknown_089A4034, ROOT_PROC_3);
}

void sub_807F58C(struct Proc* proc)
{
    int i;

    CpuFastCopy(gUnknown_02022968, gUnknown_03005110, 0x140);

    for (i = 0; i < 10; ++i)
        sub_8013998(sub_8013928(gUnknown_0859A120, i + 6, 0x3C, proc), 15);
}

void sub_807F5C8(struct Proc* parent)
{
    if (parent)
        Proc_CreateBlockingChild(gUnknown_089A404C, parent);
    else
        Proc_Create(gUnknown_089A404C, ROOT_PROC_3);
}

void sub_807F5EC(struct Proc* proc)
{
    int i;

    for (i = 0; i < 10; ++i)
        sub_8013928(gUnknown_03005110 + 0x10*i, i + 6, 15, proc);
}

void NewBG0Shaker(void)
{
    Proc_Create(gUnknown_089A4064, ROOT_PROC_3);
}

void BG0Shaker_Init(struct MAFrameShakeProc* proc)
{
    proc->unk64 = 0;
}

void BG0Shaker_Loop(struct MAFrameShakeProc* proc)
{
    BG_SetPosition(0,
        DivRem(AdvanceGetLCGRNValue(), 9) - 4,
        DivRem(AdvanceGetLCGRNValue(), 9) - 4);

    BG_SetPosition(1,
        DivRem(AdvanceGetLCGRNValue(), 9) - 4,
        DivRem(AdvanceGetLCGRNValue(), 9) - 4);

    if (proc->unk64++ > 15)
    {
        BG_SetPosition(0, 0, 0);
        BG_SetPosition(1, 0, 0);

        Proc_ClearNativeCallback((struct Proc*) proc);
    }
}

void LoadSparkGfx(void)
{
    CopyDataWithPossibleUncomp(
        gUnknown_089AE7A4,
        OBJ_VRAM0 + BM_OBJCHR_BANIM_EFFECT2*0x20);

    ApplyPalette(gUnknown_089AE7C4, 0x10 + BM_OBJPAL_BANIM_EFFECT2);
}

void sub_807F6E8(int x, int y)
{
    if (x < -4)
        return;

    if (x > 235)
        return;

    if (y < -4)
        return;

    if (y > 155)
        return;

    CallARM_PushToSecondaryOAM(
        (x - 4) & 0x1FF,
        (y - 4) & 0xFF,
        gUnknown_08590F44,
        TILEREF(BM_OBJCHR_BANIM_EFFECT2, BM_OBJPAL_BANIM_EFFECT2));
}

void sub_807F724(int xCenter, int yCenter, int distance, int angle)
{
    sub_807F6E8(
        xCenter + ((SIN(angle)*distance) >> 12),
        yCenter + ((COS(angle)*distance) >> 12));
}

void sub_807F758(struct MAStarProc* proc)
{
    int i;

    LoadSparkGfx();

    for (i = 0; i < 16; ++i)
    {
        gUnknown_03005090[i].unk00 = 0x10;
        gUnknown_03005090[i].unk02 = i << 8;
    }

    proc->unk36 = 0;
    proc->unk38 = 0;
    proc->unk3A = proc->unk3C;
}

void sub_807F788(struct MAStarProc* proc)
{
    int i, unk = sub_8012DCC(5, proc->unk2E, proc->unk30, proc->unk3A, proc->unk3E) * 16;

    proc->unk36 = unk;
    proc->unk38 = unk >> 1;

    for (i = 0; i < 16; ++i)
    {
        sub_807F724(
            proc->xCenter, proc->yCenter,
            (proc->unk36 + gUnknown_03005090[i].unk00) >> 4,
            (proc->unk38 + gUnknown_03005090[i].unk02) >> 4);
    }

    proc->unk3A++;

    if (proc->unk3A > proc->unk40)
        Proc_ClearNativeCallback((struct Proc*) proc);
}

void StartStarRotationEffect(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6)
{
    struct MAStarProc* proc =
        (struct MAStarProc*) Proc_Create(gUnknown_089A407C, ROOT_PROC_3);

    proc->xCenter = arg0;
    proc->yCenter = arg1;
    proc->unk2E = arg2;
    proc->unk30 = arg3;
    proc->unk3C = arg4;
    proc->unk3E = arg5;
    proc->unk40 = arg6;
}

void StartStarExplosionEffect(int x, int y)
{
    StartStarRotationEffect(x, y, 1, 0xC8, 0, 0x50, 0x28);
}

void StartStarImplosionEffect(int x, int y)
{
    StartStarRotationEffect(x, y, 0xC8, 1, 0, 0x3C, 0x37);
}

extern struct ProcCmd CONST_DATA gUnknown_089A434C[];

void sub_807F878(struct Proc* parent)
{
    if (parent)
        Proc_CreateBlockingChild(gUnknown_089A434C, parent);
    else
        Proc_Create(gUnknown_089A434C, ROOT_PROC_3);
}

void sub_807F89C(struct MAEffectProc* proc)
{
    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 0;
    gLCDControlBuffer.bg2cnt.priority = 0;
    gLCDControlBuffer.bg3cnt.priority = 2;

    gLCDControlBuffer.dispcnt.bg0_on = 0;
    gLCDControlBuffer.dispcnt.bg1_on = 0;
    gLCDControlBuffer.dispcnt.bg2_on = 1;
    gLCDControlBuffer.dispcnt.bg3_on = 0;
    gLCDControlBuffer.dispcnt.obj_on = 0;

    gLCDControlBuffer.wincnt.win0_enableBlend = 0;
    gLCDControlBuffer.wincnt.win1_enableBlend = 0;

    sub_8001ED0(1, 0, 0, 0, 0);
    sub_8001F48(0);

    sub_8001F0C(0, 0, 1, 0, 0);
    sub_8001F64(1);

    SetSpecialColorEffectsParameters(1, 0x10, 0x10, 0);

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(2, 0, 0);

    proc->unk40 = 0;
    proc->unk42 = 0;
    proc->unk44 = 0;
}

struct Unk089A40AC
{
    /* 00 */ const void* unk00;
    /* 04 */ const void* unk04;
    /* 08 */ const void* unk08;
};

extern struct Unk089A40AC CONST_DATA gUnknown_089A40AC[];
extern struct Unk089A40AC CONST_DATA gUnknown_089A419C[];
extern int const gUnknown_08205884[];
extern u8 const gUnknown_0820588C[];
extern u8 const gUnknown_0820588E[];

void sub_807F964(struct MAEffectProc* proc)
{
    if (proc->unk42 == 0)
    {
        if (proc->unk40 == 0)
        {
            PlaySpacialSoundMaybe(0x140, proc->xDisplay);
        }
        else if (proc->unk40 > 0x13)
        {
            proc->unk40 = 0;
            proc->unk42 = 0;
            proc->unk44 = 1;

            gLCDControlBuffer.dispcnt.bg0_on = 0;
            gLCDControlBuffer.dispcnt.bg1_on = 0;
            gLCDControlBuffer.dispcnt.bg2_on = 0;
            gLCDControlBuffer.dispcnt.bg3_on = 0;
            gLCDControlBuffer.dispcnt.obj_on = 0;

            Proc_ClearNativeCallback((struct Proc*) proc);
            return;
        }

        CopyDataWithPossibleUncomp(
            gUnknown_089A40AC[proc->unk40].unk00,
            (void*) VRAM + gUnknown_08205884[proc->unk44]*0x20);

        CopyDataWithPossibleUncomp(
            gUnknown_089A40AC[proc->unk40].unk08,
            gUnknown_02020188);

        sub_800159C(
            gBG2TilemapBuffer,
            (u16*) gUnknown_02020188,
            0, 0,
            gUnknown_08205884[proc->unk44] | (gUnknown_0820588C[proc->unk44] << 12));

        BG_EnableSyncByMask(BG2_SYNC_BIT);

        ApplyPalette(gUnknown_089A40AC[proc->unk40].unk04, gUnknown_0820588C[proc->unk44]);
        EnablePaletteSync();

        proc->unk40++;
        proc->unk42 = 3;
        proc->unk44 = proc->unk44 ^ 1;
    }

    proc->unk42--;
}

void sub_807FAA0(struct MAEffectProc* proc)
{
    if (proc->unk42 == 0)
    {
        CopyDataWithPossibleUncomp(
            gUnknown_089A419C[proc->unk40].unk00,
            (void*) VRAM + gUnknown_08205884[proc->unk44]*0x20);

        CopyDataWithPossibleUncomp(
            gUnknown_089A419C[proc->unk40].unk08,
            gUnknown_02020188);

        sub_800159C(
            gBG2TilemapBuffer,
            (u16*) gUnknown_02020188,
            0, 0,
            gUnknown_08205884[proc->unk44] | (gUnknown_0820588C[proc->unk44] << 12));

        BG_EnableSyncByMask(BG2_SYNC_BIT);

        ApplyPalette(gUnknown_089A419C[proc->unk40].unk04, gUnknown_0820588C[proc->unk44]);
        EnablePaletteSync();

        if (proc->unk40 == 0)
        {
            gLCDControlBuffer.dispcnt.bg0_on = 0;
            gLCDControlBuffer.dispcnt.bg1_on = 0;
            gLCDControlBuffer.dispcnt.bg2_on = 1;
            gLCDControlBuffer.dispcnt.bg3_on = 0;
            gLCDControlBuffer.dispcnt.obj_on = 0;
        }
        else if (proc->unk40 > 0x16)
        {
            Proc_ClearNativeCallback((struct Proc*) proc);
        }

        proc->unk42 = gUnknown_0820588E[proc->unk40];
        proc->unk40++;
        proc->unk44 = proc->unk44 ^ 1;
    }

    proc->unk42--;
}

void sub_807FBCC(struct MAEffectProc* proc)
{
    if (proc->unk42 == 0)
    {
        proc->unk40 = 0;

        CpuFastFill(-1,
            (void*) VRAM + 0x20*0x2FF, 0x20);

        TileMap_FillRect(gBG0TilemapBuffer, 30, 20, TILEREF(0x2FF, 3));
        BG_EnableSyncByMask(BG0_SYNC_BIT);

        gPaletteBuffer[0x3F] = 0;
        EnablePaletteSync();

        gLCDControlBuffer.dispcnt.bg0_on = 1;
        gLCDControlBuffer.dispcnt.bg1_on = 0;
        gLCDControlBuffer.dispcnt.bg2_on = 1;
        gLCDControlBuffer.dispcnt.bg3_on = 0;
        gLCDControlBuffer.dispcnt.obj_on = 0;

        Proc_ClearNativeCallback((struct Proc*) proc);
    }

    proc->unk42--;
}

void sub_807FC58(struct MAEffectProc* proc)
{
    u16 brightness = proc->unk40*4;

    if (proc->unk40 < 8)
    {
        gPaletteBuffer[0x3F] = RGB(brightness, brightness, brightness);
    }
    else
    {
        gPaletteBuffer[0x3F] = RGB(31, 31, 31);
        Proc_ClearNativeCallback((struct Proc*) proc);
    }

    EnablePaletteSync();

    proc->unk40++;
}

void sub_807FCA8(void)
{
    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);
}

void sub_807FCC0(struct Proc* parent)
{
    if (parent)
    {
        Proc_CreateBlockingChild(gUnknown_089A4394, parent);
    }
    else
    {
        Proc_Create(gUnknown_089A4394, ROOT_PROC_3);
    }
}

void sub_807FCE4(struct MAEffectProc* proc)
{
    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 0;
    gLCDControlBuffer.bg2cnt.priority = 0;
    gLCDControlBuffer.bg3cnt.priority = 2;

    gLCDControlBuffer.dispcnt.bg0_on = 1;
    gLCDControlBuffer.dispcnt.bg1_on = 0;
    gLCDControlBuffer.dispcnt.bg2_on = 1;
    gLCDControlBuffer.dispcnt.bg3_on = 1;
    gLCDControlBuffer.dispcnt.obj_on = 1;

    gLCDControlBuffer.wincnt.win0_enableBlend = 0;
    gLCDControlBuffer.wincnt.win1_enableBlend = 0;

    sub_8001ED0(1, 0, 1, 0, 0);
    sub_8001F48(0);

    sub_8001F0C(0, 0, 0, 1, 1);
    sub_8001F64(1);

    SetSpecialColorEffectsParameters(1, 0x10, 0x10, 0);

    BG_SetPosition(0, 0, 0);
    BG_SetPosition(2, 0, 0);

    gPaletteBuffer[0x3F] = RGB(31, 31, 31);
    EnablePaletteSync();

    proc->unk40 = 0;
    proc->unk42 = 0;
    proc->unk44 = 0;

    proc->unk48 = 119;
}

void sub_807FDC8(struct MAEffectProc* proc)
{
    int brightness = proc->unk48 * 32 / 120;
    gPaletteBuffer[0x3F] = RGB(brightness, brightness, brightness);

    EnablePaletteSync();

    proc->unk48--;

    if (proc->unk48 < 31)
        Proc_ClearNativeCallback((struct Proc*) proc);
}
