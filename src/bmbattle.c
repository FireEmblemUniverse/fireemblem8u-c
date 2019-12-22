#include "global.h"

#include "constants/items.h"
#include "constants/classes.h"
#include "constants/characters.h"
#include "constants/terrains.h"

#include "rng.h"
#include "bmitem.h"
#include "bmunit.h"
#include "bmmap.h"
#include "bmreliance.h"
#include "chapterdata.h"
#include "bmtrick.h"
#include "m4a.h"
#include "soundwrapper.h"
#include "hardware.h"
#include "proc.h"
#include "mu.h"
#include "bmbattle.h"

struct WeaponTriangleRule {
    s8 attackerWeaponType;
    s8 defenderWeaponType;
    s8 hitBonus;
    s8 atkBonus;
};

static CONST_DATA struct WeaponTriangleRule sWeaponTriangleRules[] = {
    { ITYPE_SWORD, ITYPE_LANCE, -15, -1 },
    { ITYPE_SWORD, ITYPE_AXE,   +15, +1 },

    { ITYPE_LANCE, ITYPE_AXE,   -15, -1 },
    { ITYPE_LANCE, ITYPE_SWORD, +15, +1 },

    { ITYPE_AXE,   ITYPE_SWORD, -15, -1 },
    { ITYPE_AXE,   ITYPE_LANCE, +15, +1 },

    { ITYPE_ANIMA, ITYPE_DARK,  -15, -1 },
    { ITYPE_ANIMA, ITYPE_LIGHT, +15, +1 },

    { ITYPE_LIGHT, ITYPE_ANIMA, -15, -1 },
    { ITYPE_LIGHT, ITYPE_DARK,  +15, +1 },

    { ITYPE_DARK,  ITYPE_LIGHT, -15, -1 },
    { ITYPE_DARK,  ITYPE_ANIMA, +15, +1 },

    { -1 },
};

static void UpdateActorFromBattle(void);

static CONST_DATA struct ProcCmd sProcScr_BattleAnimSimpleLock[] = {
    PROC_SLEEP(1),
    PROC_CALL_ROUTINE(UpdateActorFromBattle),
    PROC_END
};

EWRAM_DATA struct BattleStats gBattleStats = {};

EWRAM_DATA struct BattleUnit gBattleActor = {};
EWRAM_DATA struct BattleUnit gBattleTarget = {};

EWRAM_DATA struct BattleHit gBattleHitArray[BATTLE_HIT_MAX] = {};
EWRAM_DATA struct BattleHit* gBattleHitIterator = 0;

static EWRAM_DATA struct {
    u8 unk00;
    u8 unk01;
    u8 unk02;
} sUnknown_0203A60C = {};

static void BattleGenerateSimulationInternal(struct Unit* actor, struct Unit* target, int x, int y, int actorWpnSlot);
static void BattleGenerateRealInternal(struct Unit* actor, struct Unit* target);

static s8 BattleRoll1RN(u16 threshold, s8 simResult);
static s8 BattleRoll2RN(u16 threshold, s8 simResult);

static void ComputeBattleUnitStats(struct BattleUnit* attacker, struct BattleUnit* defender);
static void ComputeBattleUnitEffectiveStats(struct BattleUnit* attacker, struct BattleUnit* defender);
static void ComputeBattleUnitSupportBonuses(struct BattleUnit* attacker, struct BattleUnit* defender);
static void ComputeBattleUnitDefense(struct BattleUnit* attacker, struct BattleUnit* defender);
static void ComputeBattleUnitBaseDefense(struct BattleUnit* bu);
static void ComputeBattleUnitAttack(struct BattleUnit* attacker, struct BattleUnit* defender);
static void ComputeBattleUnitSpeed(struct BattleUnit* bu);
static void ComputeBattleUnitHitRate(struct BattleUnit* bu);
static void ComputeBattleUnitAvoidRate(struct BattleUnit* bu);
static void ComputeBattleUnitCritRate(struct BattleUnit* bu);
static void ComputeBattleUnitDodgeRate(struct BattleUnit* bu);
static void ComputeBattleUnitEffectiveHitRate(struct BattleUnit* attacker, struct BattleUnit* defender);
static void ComputeBattleUnitEffectiveCritRate(struct BattleUnit* attacker, struct BattleUnit* defender);
static void ComputeBattleUnitSilencerRate(struct BattleUnit* attacker, struct BattleUnit* defender);
static void ComputeBattleUnitWeaponRankBonuses(struct BattleUnit* bu);
static void ComputeBattleUnitStatusBonuses(struct BattleUnit* bu);
static void ComputeBattleUnitSpecialWeaponStats(struct BattleUnit* attacker, struct BattleUnit* defender);

static s8 BattleGenerateRoundHits(struct BattleUnit* attacker, struct BattleUnit* defender);
static int GetBattleUnitHitCount(struct BattleUnit* attacker);
static int BattleCheckBraveEffect(struct BattleUnit* bu);

static s8 BattleCheckTriangleAttack(struct BattleUnit* attacker, struct BattleUnit* defender);
static void BattleUpdateBattleStats(struct BattleUnit* attacker, struct BattleUnit* defender);
static void BattleCheckSureShot(struct BattleUnit* attacker);
static void BattleCheckPierce(struct BattleUnit* attacker, struct BattleUnit* defender);
static void BattleCheckGreatShield(struct BattleUnit* attacker, struct BattleUnit* defender);
static s8 BattleCheckSilencer(struct BattleUnit* attacker, struct BattleUnit* defender);
static void BattleCheckPetrify(struct BattleUnit* attacker, struct BattleUnit* defender);
static void BattleGenerateHitAttributes(struct BattleUnit* attacker, struct BattleUnit* defender);
static void BattleGenerateHitTriangleAttack(struct BattleUnit* attacker, struct BattleUnit* defender);
static void BattleGenerateHitEffects(struct BattleUnit* attacker, struct BattleUnit* defender);
static s8 BattleGenerateHit(struct BattleUnit* attacker, struct BattleUnit* defender);

static int GetStatIncrease(int growth);

static int GetBattleUnitUpdatedWeaponExp(struct BattleUnit* bu);

static int GetUnitExpLevel(struct Unit* unit);
static int GetUnitRoundExp(struct Unit* actor, struct Unit* target);
static int GetUnitPowerLevel(struct Unit* unit);
static int GetUnitClassKillExpBonus(struct Unit* actor, struct Unit* target);
static int GetUnitExpMultiplier(struct Unit* actor, struct Unit* target);
static int GetUnitKillExpBonus(struct Unit* actor, struct Unit* target);
static void ModifyUnitSpecialExp(struct Unit* actor, struct Unit* target, int* exp);
static int GetBattleUnitExpGain(struct BattleUnit* actor, struct BattleUnit* target);
static void BattleApplyItemExpGains(void);
static int GetBattleUnitStaffExp(struct BattleUnit* bu);
static void BattleApplyMiscActionExpGains(void);

static void BattleApplyReaverEffect(struct BattleUnit* attacker, struct BattleUnit* defender);

static void ComputeBattleObstacleStats(void);

static void BattlePrintDebugUnitInfo(struct BattleUnit* actor, struct BattleUnit* target);
static void BattlePrintDebugHitInfo(void);

static void BattleGenerateHitScriptedDamage(struct BattleUnit* bu);
static void BattleUnwindScripted(void);

void BattleGenerateSimulationInternal(struct Unit* actor, struct Unit* target, int x, int y, int actorWpnSlot) {
    InitBattleUnit(&gBattleActor, actor);
    InitBattleUnit(&gBattleTarget, target);

    gBattleActor.unit.xPos = x;
    gBattleActor.unit.yPos = y;

    gBattleStats.range = RECT_DISTANCE(
        gBattleActor.unit.xPos, gBattleActor.unit.yPos,
        gBattleTarget.unit.xPos, gBattleTarget.unit.yPos
    );

    if (gBattleStats.config & BATTLE_CONFIG_BALLISTA)
        SetBattleUnitWeaponBallista(&gBattleActor);
    else
        SetBattleUnitWeapon(&gBattleActor, actorWpnSlot);

    SetBattleUnitWeapon(&gBattleTarget, BU_ISLOT_AUTO);

    BattleInitTargetCanCounter();
    BattleApplyWeaponTriangleEffect(&gBattleActor, &gBattleTarget);

    DisableAllLightRunes();

    SetBattleUnitTerrainBonusesAuto(&gBattleActor);
    SetBattleUnitTerrainBonusesAuto(&gBattleTarget);

    BattleGenerate(actor, target);

    EnableAllLightRunes();
}

void BattleGenerateRealInternal(struct Unit* actor, struct Unit* target) {
    InitBattleUnit(&gBattleActor, actor);
    InitBattleUnit(&gBattleTarget, target);

    gBattleStats.range = RECT_DISTANCE(
        gBattleActor.unit.xPos, gBattleActor.unit.yPos,
        gBattleTarget.unit.xPos, gBattleTarget.unit.yPos
    );

    if (gBattleStats.config & BATTLE_CONFIG_BALLISTA)
        SetBattleUnitWeaponBallista(&gBattleActor);
    else
        SetBattleUnitWeapon(&gBattleActor, BU_ISLOT_AUTO);

    SetBattleUnitWeapon(&gBattleTarget, BU_ISLOT_AUTO);

    BattleInitTargetCanCounter();
    BattleApplyWeaponTriangleEffect(&gBattleActor, &gBattleTarget);

    DisableAllLightRunes();

    SetBattleUnitTerrainBonusesAuto(&gBattleActor);
    SetBattleUnitTerrainBonusesAuto(&gBattleTarget);

    BattleGenerate(actor, target);

    EnableAllLightRunes();

    BattleUnitTargetCheckCanCounter(&gBattleTarget);
    BattleUnitTargetSetEquippedWeapon(&gBattleTarget);

    if (gBattleTarget.unit.index) {
        BattleApplyExpGains();
        sub_80A4AA4();

        sub_80A44C8(actor);
        sub_80A44C8(target);
    }
}

void BattleApplyGameStateUpdates(void) {
    BattleApplyUnitUpdates();
    BattleApplyBallistaUpdates();

    BattlePrintDebugUnitInfo(&gBattleActor, &gBattleTarget);
    BattlePrintDebugHitInfo();
}

void BattleGenerateSimulation(struct Unit* actor, struct Unit* target, int x, int y, int actorWpnSlot) {
    if (x < 0 && y < 0) {
        x = actor->xPos;
        y = actor->yPos;
    }

    gBattleStats.config = BATTLE_CONFIG_SIMULATE;
    BattleGenerateSimulationInternal(actor, target, x, y, actorWpnSlot);
}

void BattleGenerateReal(struct Unit* actor, struct Unit* target) {
    gBattleStats.config = BATTLE_CONFIG_REAL;
    BattleGenerateRealInternal(actor, target);
}

void BattleGenerateBallistaSimulation(struct Unit* actor, struct Unit* target, int x, int y) {
    gBattleStats.config = BATTLE_CONFIG_SIMULATE | BATTLE_CONFIG_BALLISTA;
    BattleGenerateSimulationInternal(actor, target, x, y, 0);
}

void BattleGenerateBallistaReal(struct Unit* actor, struct Unit* target) {
    gBattleStats.config = BATTLE_CONFIG_REAL | BATTLE_CONFIG_BALLISTA;
    BattleGenerateRealInternal(actor, target);
}

void BattleGenerate(struct Unit* actor, struct Unit* target) {
    ComputeBattleUnitStats(&gBattleActor, &gBattleTarget);
    ComputeBattleUnitStats(&gBattleTarget, &gBattleActor);

    ComputeBattleUnitEffectiveStats(&gBattleActor, &gBattleTarget);
    ComputeBattleUnitEffectiveStats(&gBattleTarget, &gBattleActor);

    if (target == NULL)
        ComputeBattleObstacleStats();

    if ((gBattleStats.config & BATTLE_CONFIG_REAL) && (gActionData.scriptedBattleHits))
        BattleUnwindScripted();
    else
        BattleUnwind();
}

void BattleGenerateUiStats(struct Unit* unit, s8 itemSlot) {
    gBattleStats.config = BATTLE_CONFIG_BIT2;

    gBattleTarget.weapon = 0;
    gBattleTarget.weaponAttributes = IA_NONE;
    gBattleTarget.weaponType = 0xFF;

    gBattleTarget.unit.pClassData = NULL;

    gBattleActor.wTriangleHitBonus = 0;
    gBattleActor.wTriangleDmgBonus = 0;

    if ((itemSlot >= 0) && (itemSlot < UNIT_ITEM_COUNT)) {
        struct Unit tmpUnit = *unit;

        EquipUnitItemSlot(&tmpUnit, itemSlot);
        itemSlot = 0;

        InitBattleUnit(&gBattleActor, &tmpUnit);
    } else
        InitBattleUnit(&gBattleActor, unit);

    if (gUnknown_03005280.state & GMAP_STATE_BIT0)
        SetBattleUnitTerrainBonuses(&gBattleActor, 0); // TODO: TERRAIN ID DEFINITIONS
    else
        SetBattleUnitTerrainBonusesAuto(&gBattleActor);

    SetBattleUnitWeapon(&gBattleActor, itemSlot);
    ComputeBattleUnitStats(&gBattleActor, &gBattleTarget);

    if (GetItemIndex(gBattleActor.weapon) == ITEM_SWORD_RUNESWORD) {
        gBattleActor.battleAttack -= gBattleActor.unit.pow / 2;

        gBattleActor.battleCritRate = 0;
        gBattleActor.battleEffectiveCritRate = 0;
    }

    if (!gBattleActor.weapon) {
        gBattleActor.battleAttack = 0xFF;
        gBattleActor.battleHitRate = 0xFF;
        gBattleActor.battleCritRate = 0xFF;
    }

    if (GetItemWeaponEffect(gBattleActor.weapon) == WPN_EFFECT_HPHALVE)
        gBattleActor.battleAttack = 0xFF;

    if (GetItemIndex(gBattleActor.weapon) == ITEM_MONSTER_STONE) {
        gBattleActor.battleAttack = 0xFF;
        gBattleActor.battleCritRate = 0xFF;
    }
}

s8 BattleRoll1RN(u16 threshold, s8 simResult) {
    if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
        return simResult;

    return Roll1RN(threshold);
}

s8 BattleRoll2RN(u16 threshold, s8 simResult) {
    if (gBattleStats.config & BATTLE_CONFIG_SIMULATE)
        return simResult;

    return Roll2RN(threshold);
}

void InitBattleUnit(struct BattleUnit* bu, struct Unit* unit) {
    if (!unit)
        return;

    bu->unit = *unit;

    bu->unit.maxHP = GetUnitMaxHp(unit);
    bu->unit.pow = GetUnitPower(unit);
    bu->unit.skl = GetUnitSkill(unit);
    bu->unit.spd = GetUnitSpeed(unit);
    bu->unit.def = GetUnitDefense(unit);
    bu->unit.lck = GetUnitLuck(unit);
    bu->unit.res = GetUnitResistance(unit);
    bu->unit.conBonus = UNIT_CON(unit);
    bu->unit.movBonus = UNIT_MOV(unit);

    bu->levelPrevious = bu->unit.level;
    bu->expPrevious = bu->unit.exp;

    bu->hpInitial = bu->unit.curHP;
    bu->statusOut = 0xFF;

    bu->changeHP = 0;
    bu->changePow = 0;
    bu->changeSkl = 0;
    bu->changeSpd = 0;
    bu->changeDef = 0;
    bu->changeRes = 0;
    bu->changeLck = 0;
    bu->changeCon = 0;

    gBattleActor.wexpMultiplier = 0;
    gBattleTarget.wexpMultiplier = 0;

    bu->wTriangleHitBonus = 0;
    bu->wTriangleDmgBonus = 0;

    bu->nonZeroDamage = FALSE;

    gBattleActor.weaponBroke = FALSE;
    gBattleTarget.weaponBroke = FALSE;

    gBattleActor.expGain = 0;
    gBattleTarget.expGain = 0;
}

void InitBattleUnitWithoutBonuses(struct BattleUnit* bu, struct Unit* unit) {
    InitBattleUnit(bu, unit);

    bu->unit.maxHP = unit->maxHP;
    bu->unit.pow = unit->pow;
    bu->unit.skl = unit->skl;
    bu->unit.spd = unit->spd;
    bu->unit.def = unit->def;
    bu->unit.lck = unit->lck;
    bu->unit.res = unit->res;

    bu->unit.conBonus = UNIT_CON_BASE(unit);
}

void SetBattleUnitTerrainBonuses(struct BattleUnit* bu, int terrain) {
    bu->terrainId = terrain;

    bu->terrainAvoid      = bu->unit.pClassData->pTerrainAvoidLookup[bu->terrainId];
    bu->terrainDefense    = bu->unit.pClassData->pTerrainDefenseLookup[bu->terrainId];
    bu->terrainResistance = bu->unit.pClassData->pTerrainResistanceLookup[bu->terrainId];
}

void SetBattleUnitTerrainBonusesAuto(struct BattleUnit* bu) {
    bu->terrainId = gBmMapTerrain[bu->unit.yPos][bu->unit.xPos];

    bu->terrainAvoid      = bu->unit.pClassData->pTerrainAvoidLookup[bu->terrainId];
    bu->terrainDefense    = bu->unit.pClassData->pTerrainDefenseLookup[bu->terrainId];
    bu->terrainResistance = bu->unit.pClassData->pTerrainResistanceLookup[bu->terrainId];
}

void SetBattleUnitWeapon(struct BattleUnit* bu, int itemSlot) {
    if (itemSlot == BU_ISLOT_AUTO)
        itemSlot = GetUnitEquippedWeaponSlot(&bu->unit);

    if (bu->unit.state & US_IN_BALLISTA)
        itemSlot = BU_ISLOT_BALLISTA;

    bu->canCounter = TRUE;

    switch (itemSlot) {

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        // regular item slots

        bu->weaponSlotIndex = itemSlot;
        bu->weapon = bu->unit.items[bu->weaponSlotIndex];

        break;

    case BU_ISLOT_5:
        // borrowed item?

        bu->weaponSlotIndex = 0xFF;
        bu->weapon = gUnknown_0202BCB0.itemUnk2C;

        break;

    case BU_ISLOT_ARENA_PLAYER:
        // arena player weapon

        bu->weaponSlotIndex = 0;

        bu->weapon = gUnknown_0203A8F0.playerWeapon;
        bu->canCounter = FALSE;

        break;

    case BU_ISLOT_ARENA_OPPONENT:
        // arena opponent weapon

        bu->weaponSlotIndex = 0;

        bu->weapon = gUnknown_0203A8F0.opponentWeapon;
        bu->canCounter = FALSE;

        break;

    case BU_ISLOT_BALLISTA:
        // riding ballista

        bu->weaponSlotIndex = 0xFF;

        bu->weapon = GetBallistaItemAt(bu->unit.xPos, bu->unit.yPos);
        bu->canCounter = FALSE;

        break;

    default:
        bu->weaponSlotIndex = 0xFF;

        bu->weapon = 0;
        bu->canCounter = FALSE;

        break;

    } // switch (itemSlot)

    bu->weaponBefore = bu->weapon;
    bu->weaponAttributes = GetItemAttributes(bu->weapon);
    bu->weaponType = GetItemType(bu->weapon);

    if (!(gBattleStats.config & BATTLE_CONFIG_BIT2)) {
        if (bu->weaponAttributes & IA_MAGICDAMAGE) {
            switch (GetItemIndex(bu->weapon)) {

            case ITEM_SWORD_WINDSWORD:
                if (gBattleStats.range == 2)
                    bu->weaponType = ITYPE_ANIMA;
                else
                    bu->weaponAttributes = bu->weaponAttributes &~ IA_MAGICDAMAGE;

                break;

            case ITEM_SWORD_LIGHTBRAND:
                if (gBattleStats.range == 2)
                    bu->weaponType = ITYPE_LIGHT;
                else
                    bu->weaponAttributes = bu->weaponAttributes &~ IA_MAGICDAMAGE;

                break;

            case ITEM_SWORD_RUNESWORD:
                bu->weaponType = ITYPE_DARK;
                break;

            } // switch (GetItemIndex(bu->weapon))
        } // if (bu->weaponAttributes & IA_MAGICDAMAGE)

        if (!IsItemCoveringRange(bu->weapon, gBattleStats.range) || bu->weaponSlotIndex == 0xFF) {
            bu->weapon = 0;
            bu->canCounter = FALSE;
        }

        switch (bu->unit.statusIndex) {

        case UNIT_STATUS_SLEEP:
        case UNIT_STATUS_PETRIFY:
        case UNIT_STATUS_13:
            bu->weapon = 0;
            bu->canCounter = FALSE;

            break;

        } // switch (bu->unit.statusIndex)
    }
}

void SetBattleUnitWeaponBallista(struct BattleUnit* bu) {
    bu->weapon = GetBallistaItemAt(bu->unit.xPos, bu->unit.yPos);

    bu->weaponBefore = bu->weapon;
    bu->weaponAttributes = GetItemAttributes(bu->weapon);
    bu->weaponType = GetItemType(bu->weapon);

    bu->canCounter = FALSE;
}

void sub_802A958(void) {} // unused

void ComputeBattleUnitStats(struct BattleUnit* attacker, struct BattleUnit* defender) {
    ComputeBattleUnitDefense(attacker, defender);
    ComputeBattleUnitAttack(attacker, defender);
    ComputeBattleUnitSpeed(attacker);
    ComputeBattleUnitHitRate(attacker);
    ComputeBattleUnitAvoidRate(attacker);
    ComputeBattleUnitCritRate(attacker);
    ComputeBattleUnitDodgeRate(attacker);
    ComputeBattleUnitSupportBonuses(attacker, defender);
    ComputeBattleUnitWeaponRankBonuses(attacker);
    ComputeBattleUnitStatusBonuses(attacker);
}

void ComputeBattleUnitEffectiveStats(struct BattleUnit* attacker, struct BattleUnit* defender) {
    ComputeBattleUnitEffectiveHitRate(attacker, defender);
    ComputeBattleUnitEffectiveCritRate(attacker, defender);
    ComputeBattleUnitSilencerRate(attacker, defender);
    ComputeBattleUnitSpecialWeaponStats(attacker, defender);
}

void ComputeBattleUnitSupportBonuses(struct BattleUnit* attacker, struct BattleUnit* defender) {
    if (!(gBattleStats.config & BATTLE_CONFIG_ARENA) || gUnknown_0202BCF0.chapterWeatherId) {
        struct SupportBonuses tmpBonuses;

        GetUnitSupportBonuses(&attacker->unit, &tmpBonuses);

        attacker->battleAttack    += tmpBonuses.bonusAttack;
        attacker->battleDefense   += tmpBonuses.bonusDefense;
        attacker->battleHitRate   += tmpBonuses.bonusHit;
        attacker->battleAvoidRate += tmpBonuses.bonusAvoid;
        attacker->battleCritRate  += tmpBonuses.bonusCrit;
        attacker->battleDodgeRate += tmpBonuses.bonusDodge;
    }
}

void ComputeBattleUnitDefense(struct BattleUnit* attacker, struct BattleUnit* defender) {
    if (GetItemAttributes(defender->weapon) & IA_MAGICDAMAGE)
        attacker->battleDefense = attacker->terrainResistance + attacker->unit.res;
    else if (GetItemAttributes(defender->weapon) & IA_MAGIC)
        attacker->battleDefense = attacker->terrainResistance + attacker->unit.res;
    else
        attacker->battleDefense = attacker->terrainDefense + attacker->unit.def;
}

void ComputeBattleUnitBaseDefense(struct BattleUnit* bu) {
    bu->battleDefense = bu->terrainDefense + bu->unit.def;
}

void ComputeBattleUnitAttack(struct BattleUnit* attacker, struct BattleUnit* defender) {
    short attack;

    attacker->battleAttack = GetItemMight(attacker->weapon) + attacker->wTriangleDmgBonus;
    attack = attacker->battleAttack;

    if (IsUnitEffectiveAgainst(&attacker->unit, &defender->unit) == TRUE)
        attack = attacker->battleAttack * 3;

    if (IsItemEffectiveAgainst(attacker->weapon, &defender->unit) == TRUE) {
        attack = attacker->battleAttack;

        switch (GetItemIndex(attacker->weapon)) {

        case ITEM_SWORD_AUDHULMA:
        case ITEM_LANCE_VIDOFNIR:
        case ITEM_AXE_GARM:
        case ITEM_BOW_NIDHOGG:
        case ITEM_ANIMA_EXCALIBUR:
        case ITEM_LIGHT_IVALDI:
        case ITEM_SWORD_SIEGLINDE:
        case ITEM_LANCE_SIEGMUND:
            attack *= 2;
            break;

        default:
            attack *= 3;
            break;

        } // switch (GetItemIndex(attacker->weapon))
    }

    attacker->battleAttack = attack;
    attacker->battleAttack += attacker->unit.pow;

    if (GetItemIndex(attacker->weapon) == ITEM_MONSTER_STONE)
        attacker->battleAttack = 0;
}

void ComputeBattleUnitSpeed(struct BattleUnit* bu) {
    int effWt = GetItemWeight(bu->weaponBefore);

    effWt -= bu->unit.conBonus;

    if (effWt < 0)
        effWt = 0;

    bu->battleSpeed = bu->unit.spd - effWt;

    if (bu->battleSpeed < 0)
        bu->battleSpeed = 0;
}

void ComputeBattleUnitHitRate(struct BattleUnit* bu) {
    bu->battleHitRate = (bu->unit.skl * 2) + GetItemHit(bu->weapon) + (bu->unit.lck / 2) + bu->wTriangleHitBonus;
}

void ComputeBattleUnitAvoidRate(struct BattleUnit* bu) {
    bu->battleAvoidRate = (bu->battleSpeed * 2) + bu->terrainAvoid + (bu->unit.lck);

    if (bu->battleAvoidRate < 0)
        bu->battleAvoidRate = 0;
}

void ComputeBattleUnitCritRate(struct BattleUnit* bu) {
    bu->battleCritRate = GetItemCrit(bu->weapon) + (bu->unit.skl / 2);

    if (UNIT_CATTRIBUTES(&bu->unit) & CA_CRITBONUS)
        bu->battleCritRate += 15;
}

void ComputeBattleUnitDodgeRate(struct BattleUnit* bu) {
    bu->battleDodgeRate = bu->unit.lck;
}

void ComputeBattleUnitEffectiveHitRate(struct BattleUnit* attacker, struct BattleUnit* defender) {
    attacker->battleEffectiveHitRate = attacker->battleHitRate - defender->battleAvoidRate;

    if (attacker->battleEffectiveHitRate > 100)
        attacker->battleEffectiveHitRate = 100;

    if (attacker->battleEffectiveHitRate < 0)
        attacker->battleEffectiveHitRate = 0;
}

void ComputeBattleUnitEffectiveCritRate(struct BattleUnit* attacker, struct BattleUnit* defender) {
    int item, i;

    attacker->battleEffectiveCritRate = attacker->battleCritRate - defender->battleDodgeRate;

    if (GetItemIndex(attacker->weapon) == ITEM_MONSTER_STONE)
        attacker->battleEffectiveCritRate = 0;

    if (attacker->battleEffectiveCritRate < 0)
        attacker->battleEffectiveCritRate = 0;

    for (i = 0; (i < UNIT_ITEM_COUNT) && (item = defender->unit.items[i]); ++i) {
        if (GetItemAttributes(item) & IA_NEGATE_CRIT) {
            attacker->battleEffectiveCritRate = 0;
            break;
        }
    }
}

void ComputeBattleUnitSilencerRate(struct BattleUnit* attacker, struct BattleUnit* defender) {
    if (!(UNIT_CATTRIBUTES(&attacker->unit) & CA_ASSASSIN))
        attacker->battleSilencerRate = 0;
    else {
        attacker->battleSilencerRate = 50;

        if (UNIT_CATTRIBUTES(&defender->unit) & CA_BOSS)
            attacker->battleSilencerRate = 25;

        if (UNIT_CATTRIBUTES(&defender->unit) & CA_NEGATE_LETHALITY)
            attacker->battleSilencerRate = 0;
    }
}

void ComputeBattleUnitWeaponRankBonuses(struct BattleUnit* bu) {
    if (bu->weapon) {
        int wType = GetItemType(bu->weapon);

        if (wType < 8 && bu->unit.ranks[wType] >= WPN_EXP_S) {
            bu->battleHitRate += 5;
            bu->battleCritRate += 5;
        }
    }
}

void ComputeBattleUnitStatusBonuses(struct BattleUnit* bu) {
    switch (bu->unit.statusIndex) {

    case UNIT_STATUS_ATTACK:
        bu->battleAttack += 10;
        break;

    case UNIT_STATUS_DEFENSE:
        bu->battleDefense += 10;
        break;

    case UNIT_STATUS_CRIT:
        bu->battleCritRate += 10;
        break;

    case UNIT_STATUS_AVOID:
        bu->battleAvoidRate += 10;
        break;

    } // switch (bu->unit.statusIndex)
}

void ComputeBattleUnitSpecialWeaponStats(struct BattleUnit* attacker, struct BattleUnit* defender) {
    if (attacker->weaponAttributes & IA_MAGICDAMAGE) {
        switch (GetItemIndex(attacker->weapon)) {

        case ITEM_SWORD_LIGHTBRAND:
        case ITEM_SWORD_RUNESWORD:
        case ITEM_SWORD_WINDSWORD:
            attacker->battleAttack -= attacker->unit.pow / 2;

            attacker->battleCritRate = 0;
            attacker->battleEffectiveCritRate = 0;

            break;

        } // switch (GetItemIndex(attacker->weapon))
    } else {
        if (GetItemWeaponEffect(attacker->weaponBefore) == WPN_EFFECT_HPHALVE) {
            attacker->battleAttack = (defender->unit.curHP + 1) >> 1;

            if (attacker->battleAttack == 0)
                attacker->battleAttack = 1;

            defender->battleDefense = 0;

            attacker->battleCritRate = 0;
            attacker->battleEffectiveCritRate = 0;
        }

        if (attacker->weaponAttributes & IA_NEGATE_DEFENSE)
            defender->battleDefense = 0;

        if (defender->unit.statusIndex == UNIT_STATUS_PETRIFY || defender->unit.statusIndex == UNIT_STATUS_13) {
            attacker->battleEffectiveHitRate = 100;

            attacker->battleEffectiveCritRate += 30;

            if (attacker->battleEffectiveCritRate > 100)
                attacker->battleEffectiveCritRate = 100;
        }
    }
}

void ClearBattleHits(void) {
    int i;

    for (i = 0; i < BATTLE_HIT_MAX; ++i) {
        gBattleHitArray[i].attributes = 0;
        gBattleHitArray[i].info = 0;
        gBattleHitArray[i].hpChange = 0;
    }

    gBattleHitIterator = gBattleHitArray;
}

void BattleUnwind(void) {
    ClearBattleHits();

    // this do { ... } while (0); is required for match
    // which is kind of neat because it implies scrapped plans for supporting some accost kind of thing

    do {
        struct BattleUnit* attacker;
        struct BattleUnit* defender;

        BattleGetBattleUnitOrder(&attacker, &defender);

        gBattleHitIterator->info |= BATTLE_HIT_INFO_BEGIN;

        if (!BattleGenerateRoundHits(attacker, defender)) {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_RETALIATE;

            if (!BattleGenerateRoundHits(defender, attacker) && BattleGetFollowUpOrder(&attacker, &defender)) {
                gBattleHitIterator->attributes = BATTLE_HIT_ATTR_FOLLOWUP;

                BattleGenerateRoundHits(attacker, defender);
            }
        }
    } while (FALSE);

    gBattleHitIterator->info |= BATTLE_HIT_INFO_END;
}

void BattleGetBattleUnitOrder(struct BattleUnit** outAttacker, struct BattleUnit** outDefender) {
    *outAttacker = &gBattleActor;
    *outDefender = &gBattleTarget;
}

s8 BattleGetFollowUpOrder(struct BattleUnit** outAttacker, struct BattleUnit** outDefender) {
    if (gBattleTarget.battleSpeed > 250)
        return FALSE;

    if (ABS(gBattleActor.battleSpeed - gBattleTarget.battleSpeed) < BATTLE_FOLLOWUP_SPEED_THRESHOLD)
        return FALSE;

    if (gBattleActor.battleSpeed > gBattleTarget.battleSpeed) {
        *outAttacker = &gBattleActor;
        *outDefender = &gBattleTarget;
    } else {
        *outAttacker = &gBattleTarget;
        *outDefender = &gBattleActor;
    }

    if (GetItemWeaponEffect((*outAttacker)->weaponBefore) == WPN_EFFECT_HPHALVE)
        return FALSE;

    if (GetItemIndex((*outAttacker)->weapon) == ITEM_MONSTER_STONE)
        return FALSE;

    return TRUE;
}

s8 BattleGenerateRoundHits(struct BattleUnit* attacker, struct BattleUnit* defender) {
    int i, count;
    u16 attrs; // NOTE: this is a bug! attrs are 19 bits in FE8 (they're 16 bits in previous games)

    if (!attacker->weapon)
        return FALSE;

    attrs = gBattleHitIterator->attributes;
    count = GetBattleUnitHitCount(attacker);

    for (i = 0; i < count; ++i) {
        gBattleHitIterator->attributes |= attrs;

        if (BattleGenerateHit(attacker, defender))
            return TRUE;
    }

    return FALSE;
}

int GetBattleUnitHitCount(struct BattleUnit* attacker) {
    int result = 1;

    result <<= BattleCheckBraveEffect(attacker);

    return result;
}

int BattleCheckBraveEffect(struct BattleUnit* attacker) {
    if (!(attacker->weaponAttributes & IA_BRAVE))
        return FALSE;

    gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_BRAVE;
    return TRUE;
}

s8 BattleCheckTriangleAttack(struct BattleUnit* attacker, struct BattleUnit* defender) {
    s8 adjacentLookup[] = {
        -1, 0,
        0, -1,
        +1, 0,
        0, +1
    };

    int i, count = 0;

    int triangleAttackAttr = CA_TRIANGLEATTACK_ANY & UNIT_CATTRIBUTES(&attacker->unit);

    int x = defender->unit.xPos;
    int y = defender->unit.yPos;

    int faction = UNIT_FACTION(&attacker->unit);

    gBattleStats.taUnitA = NULL;
    gBattleStats.taUnitB = NULL;

    for (i = 0; i < 4; ++i) {
        int uId = gBmMapUnit[adjacentLookup[i*2 + 1] + y][adjacentLookup[i*2 + 0] + x];
        struct Unit* unit;

        if (!uId)
            continue;

        unit = GetUnit(uId);

        if ((uId & 0xC0) != faction)
            continue;

        if (unit->statusIndex == UNIT_STATUS_SLEEP)
            continue;

        if (unit->statusIndex == UNIT_STATUS_PETRIFY)
            continue;

        if (unit->statusIndex == UNIT_STATUS_13)
            continue;

        if (UNIT_CLASS_ID(unit) == CLASS_WYVERN_KNIGHT_F)
            continue;

        if (UNIT_CATTRIBUTES(unit) & triangleAttackAttr) {
            ++count;

            if (!gBattleStats.taUnitA)
                gBattleStats.taUnitA = unit;
            else if (!gBattleStats.taUnitB)
                gBattleStats.taUnitB = unit;
        }
    }

    return count >= 2 ? TRUE : FALSE;
}

void BattleUpdateBattleStats(struct BattleUnit* attacker, struct BattleUnit* defender) {
    gBattleStats.attack = attacker->battleAttack;
    gBattleStats.defense = defender->battleDefense;
    gBattleStats.hitRate = attacker->battleEffectiveHitRate;
    gBattleStats.critRate = attacker->battleEffectiveCritRate;
    gBattleStats.silencerRate = attacker->battleSilencerRate;
}

void BattleCheckSureShot(struct BattleUnit* attacker) {
    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_SURESHOT)
        return;

    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_PIERCE)
        return;

    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_GREATSHLD)
        return;

    switch (UNIT_CLASS_ID(&attacker->unit)) {

    case CLASS_SNIPER:
    case CLASS_SNIPER_F:
        switch (GetItemIndex(attacker->weapon)) {

        case ITEM_BALLISTA_REGULAR:
        case ITEM_BALLISTA_LONG:
        case ITEM_BALLISTA_KILLER:
            break;

        default:
            if (BattleRoll1RN(attacker->unit.level, FALSE) == TRUE)
                gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_SURESHOT;

            break;

        } // switch (GetItemIndex(attacker->weapon))

        break;

    } // switch (UNIT_CLASS_ID(&attacker->unit))
}

void BattleCheckPierce(struct BattleUnit* attacker, struct BattleUnit* defender) {
    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_SURESHOT)
        return;

    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_PIERCE)
        return;

    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_GREATSHLD)
        return;

    switch (UNIT_CLASS_ID(&attacker->unit)) {

    case CLASS_WYVERN_KNIGHT:
    case CLASS_WYVERN_KNIGHT_F:
        if (BattleRoll1RN(attacker->unit.level, FALSE) == TRUE)
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_PIERCE;

        break;

    } // switch (UNIT_CLASS_ID(&attacker->unit))
}

void BattleCheckGreatShield(struct BattleUnit* attacker, struct BattleUnit* defender) {
    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS)
        return;

    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_SURESHOT)
        return;

    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_PIERCE)
        return;

    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_GREATSHLD)
        return;

    if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_POISON)
        return;

    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS)
        return;

    switch (UNIT_CLASS_ID(&defender->unit)) {

    case CLASS_GENERAL:
    case CLASS_GENERAL_F:
        if (BattleRoll1RN(attacker->unit.level, FALSE) == TRUE)
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_GREATSHLD;

        break;

    } // switch (UNIT_CLASS_ID(&defender->unit))
}

s8 BattleCheckSilencer(struct BattleUnit* attacker, struct BattleUnit* defender) {
    switch (UNIT_CLASS_ID(&defender->unit)) {

    case CLASS_DEMON_KING:
        return FALSE;

    case CLASS_NECROMANCER:
        if (gUnknown_0202BCF0.chapterIndex == 0x15) // TODO: CHAPTER ID CONSTANTS
            return FALSE;

        if (gUnknown_0202BCF0.chapterIndex == 0x22) // TODO: CHAPTER ID CONSTANTS
            return FALSE;

    } // switch (UNIT_CLASS_ID(&defender->unit))

    if (BattleRoll1RN(gBattleStats.silencerRate, FALSE) == TRUE)
        return TRUE;

    return FALSE;
}

void BattleCheckPetrify(struct BattleUnit* attacker, struct BattleUnit* defender) {
    if (GetItemIndex(attacker->weapon) == ITEM_MONSTER_STONE)
        gBattleStats.damage = 0;
}

void BattleGenerateHitAttributes(struct BattleUnit* attacker, struct BattleUnit* defender) {
    short attack, defense;

    gBattleStats.damage = 0;

    BattleCheckSureShot(attacker);

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_SURESHOT)) {
        if (!BattleRoll2RN(gBattleStats.hitRate, TRUE)) {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_MISS;
            return;
        }
    }

    attack = gBattleStats.attack;
    defense = gBattleStats.defense;

    BattleCheckGreatShield(attacker, defender);

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_GREATSHLD))
        BattleCheckPierce(attacker, defender);

    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_PIERCE)
        defense = 0;

    gBattleStats.damage = attack - defense;

    if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_GREATSHLD)
        gBattleStats.damage = 0;

    if (BattleRoll1RN(gBattleStats.critRate, FALSE) == TRUE) {
        if (BattleCheckSilencer(attacker, defender)) {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_SILENCER;

            gBattleStats.damage = BATTLE_MAX_DAMAGE;

            gBattleHitIterator->attributes = gBattleHitIterator->attributes &~ BATTLE_HIT_ATTR_GREATSHLD;
        } else {
            gBattleHitIterator->attributes = gBattleHitIterator->attributes | BATTLE_HIT_ATTR_CRIT;
            gBattleStats.damage = gBattleStats.damage * 3;
        }
    }

    if (gBattleStats.damage > BATTLE_MAX_DAMAGE)
        gBattleStats.damage = BATTLE_MAX_DAMAGE;

    if (gBattleStats.damage < 0)
        gBattleStats.damage = 0;

    BattleCheckPetrify(attacker, defender);

    if (gBattleStats.damage != 0)
        attacker->nonZeroDamage = TRUE;
}

void BattleGenerateHitTriangleAttack(struct BattleUnit* attacker, struct BattleUnit* defender) {
    if (!(UNIT_CATTRIBUTES(&attacker->unit) & CA_TRIANGLEATTACK_ANY))
        return;

    if (gBattleStats.range != 1)
        return;

    if (!(gBattleHitIterator->info & BATTLE_HIT_INFO_BEGIN))
        return;

    if (attacker->unit.statusIndex == UNIT_STATUS_BERSERK)
        return;

    if (gBattleStats.config & BATTLE_CONFIG_ARENA)
        return;

    if (!BattleCheckTriangleAttack(attacker, defender))
        return;

    gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_TATTACK;

    gBattleStats.critRate = 100;
    gBattleStats.hitRate  = 100;
}

void BattleGenerateHitEffects(struct BattleUnit* attacker, struct BattleUnit* defender) {
    attacker->wexpMultiplier++;

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS)) {
        if (UNIT_CLASS_ID(&defender->unit) != CLASS_DEMON_KING) {
            switch (GetItemWeaponEffect(attacker->weapon)) {

            case WPN_EFFECT_POISON:
                // Poison defender

                defender->statusOut = UNIT_STATUS_POISON;
                gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_POISON;

                // "Ungray" defender if it was petrified (as it won't be anymore)
                if (defender->unit.statusIndex == UNIT_STATUS_PETRIFY || defender->unit.statusIndex == UNIT_STATUS_13)
                    defender->unit.state = defender->unit.state &~ US_UNSELECTABLE;

                break;

            case WPN_EFFECT_HPHALVE:
                gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPHALVE;
                break;

            } // switch (GetItemWeaponEffect(attacker->weapon))
        }

        if ((GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL) && (BattleRoll1RN(31 - attacker->unit.lck, FALSE))) {
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_DEVIL;

            attacker->unit.curHP -= gBattleStats.damage;

            if (attacker->unit.curHP < 0)
                attacker->unit.curHP = 0;
        } else {
            if (gBattleStats.damage > defender->unit.curHP)
                gBattleStats.damage = defender->unit.curHP;

            defender->unit.curHP -= gBattleStats.damage;

            if (defender->unit.curHP < 0)
                defender->unit.curHP = 0;
        }

        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN) {
            if (attacker->unit.maxHP < (attacker->unit.curHP + gBattleStats.damage))
                attacker->unit.curHP = attacker->unit.maxHP;
            else
                attacker->unit.curHP += gBattleStats.damage;

            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
        }

        if (UNIT_CLASS_ID(&defender->unit) != CLASS_DEMON_KING) {
            if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_PETRIFY) {
                switch (gUnknown_0202BCF0.chapterPhaseIndex) {

                case FACTION_BLUE:
                    if (UNIT_FACTION(&defender->unit) == FACTION_BLUE)
                        defender->statusOut = UNIT_STATUS_13;
                    else
                        defender->statusOut = UNIT_STATUS_PETRIFY;

                    break;

                case FACTION_RED:
                    if (UNIT_FACTION(&defender->unit) == FACTION_RED)
                        defender->statusOut = UNIT_STATUS_13;
                    else
                        defender->statusOut = UNIT_STATUS_PETRIFY;

                    break;

                case FACTION_GREEN:
                    if (UNIT_FACTION(&defender->unit) == FACTION_GREEN)
                        defender->statusOut = UNIT_STATUS_13;
                    else
                        defender->statusOut = UNIT_STATUS_PETRIFY;

                    break;

                } // switch (gUnknown_0202BCF0.chapterPhaseIndex)

                gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_PETRIFY;
            }
        }
    }

    gBattleHitIterator->hpChange = gBattleStats.damage;

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS) || attacker->weaponAttributes & (IA_UNCOUNTERABLE | IA_MAGIC)) {
        attacker->weapon = GetItemAfterUse(attacker->weapon);

        if (!attacker->weapon)
            attacker->weaponBroke = TRUE;
    }
}

s8 BattleGenerateHit(struct BattleUnit* attacker, struct BattleUnit* defender) {
    if (attacker == &gBattleTarget)
        gBattleHitIterator->info |= BATTLE_HIT_INFO_RETALIATION;

    BattleUpdateBattleStats(attacker, defender);

    BattleGenerateHitTriangleAttack(attacker, defender);
    BattleGenerateHitAttributes(attacker, defender);
    BattleGenerateHitEffects(attacker, defender);

    if (attacker->unit.curHP == 0 || defender->unit.curHP == 0) {
        attacker->wexpMultiplier++;

        gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;

        if (gBattleTarget.unit.curHP != 0) {
            gBattleHitIterator++;
            return TRUE;
        }

        gBattleHitIterator->info |= BATTLE_HIT_INFO_KILLS_TARGET;

        gBattleHitIterator++;
        return TRUE;
    } else if (defender->statusOut == UNIT_STATUS_PETRIFY || defender->statusOut == UNIT_STATUS_13) {
        gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;

        gBattleHitIterator++;
        return TRUE;
    }

    gBattleHitIterator++;
    return FALSE;
}

void BattleApplyExpGains(void) {
    if ((UNIT_FACTION(&gBattleActor.unit) != FACTION_BLUE) || (UNIT_FACTION(&gBattleTarget.unit) != FACTION_BLUE)) {
        if (!(gUnknown_0202BCF0.chapterStateBits & CHAPTER_FLAG_7)) {
            gBattleActor.expGain  = GetBattleUnitExpGain(&gBattleActor, &gBattleTarget);
            gBattleTarget.expGain = GetBattleUnitExpGain(&gBattleTarget, &gBattleActor);

            gBattleActor.unit.exp  += gBattleActor.expGain;
            gBattleTarget.unit.exp += gBattleTarget.expGain;

            CheckBattleUnitLevelUp(&gBattleActor);
            CheckBattleUnitLevelUp(&gBattleTarget);
        }
    }
}

int GetStatIncrease(int growth) {
    int result = 0;

    while (growth > 100) {
        result++;
        growth -= 100;
    }

    if (Roll1RN(growth))
        result++;

    return result;
}

int GetAutoleveledStatIncrease(int growth, int levelCount) {
    return GetStatIncrease((growth * levelCount) + (NextRN_N((growth * levelCount) / 4) - (growth * levelCount) / 8));
}

s8 CanBattleUnitGainLevels(struct BattleUnit* bu) {
    if (gUnknown_0202BCB0.gameStateBits & 0x40)
        return TRUE;

    if (bu->unit.exp == UNIT_EXP_DISABLED)
        return FALSE;

    if (UNIT_FACTION(&bu->unit) != FACTION_BLUE)
        return FALSE;

    return TRUE;
}

void CheckBattleUnitLevelUp(struct BattleUnit* bu) {
    if (CanBattleUnitGainLevels(bu) && bu->unit.exp >= 100) {
        int growthBonus, statGainTotal;

        bu->unit.exp -= 100;
        bu->unit.level++;

        if (UNIT_CATTRIBUTES(&bu->unit) & CA_MAXLEVEL10) {
            if (bu->unit.level == 10) {
                bu->expGain -= bu->unit.exp;
                bu->unit.exp = UNIT_EXP_DISABLED;
            }
        } else if (bu->unit.level == 20) {
            bu->expGain -= bu->unit.exp;
            bu->unit.exp = UNIT_EXP_DISABLED;
        }

        growthBonus = (bu->unit.state & US_GROWTH_BOOST) ? 5: 0;
        statGainTotal = 0;

        bu->changeHP  = GetStatIncrease(bu->unit.pCharacterData->growthHP + growthBonus);
        statGainTotal += bu->changeHP;

        bu->changePow = GetStatIncrease(bu->unit.pCharacterData->growthPow + growthBonus);
        statGainTotal += bu->changePow;

        bu->changeSkl = GetStatIncrease(bu->unit.pCharacterData->growthSkl + growthBonus);
        statGainTotal += bu->changeSkl;

        bu->changeSpd = GetStatIncrease(bu->unit.pCharacterData->growthSpd + growthBonus);
        statGainTotal += bu->changeSpd;

        bu->changeDef = GetStatIncrease(bu->unit.pCharacterData->growthDef + growthBonus);
        statGainTotal += bu->changeDef;

        bu->changeRes = GetStatIncrease(bu->unit.pCharacterData->growthRes + growthBonus);
        statGainTotal += bu->changeRes;

        bu->changeLck = GetStatIncrease(bu->unit.pCharacterData->growthLck + growthBonus);
        statGainTotal += bu->changeLck;

        if (statGainTotal == 0) {
            for (statGainTotal = 0; statGainTotal < 2; ++statGainTotal) {
                bu->changeHP = GetStatIncrease(bu->unit.pCharacterData->growthHP);

                if (bu->changeHP)
                    break;

                bu->changePow = GetStatIncrease(bu->unit.pCharacterData->growthPow);

                if (bu->changePow)
                    break;

                bu->changeSkl = GetStatIncrease(bu->unit.pCharacterData->growthSkl);

                if (bu->changeSkl)
                    break;

                bu->changeSpd = GetStatIncrease(bu->unit.pCharacterData->growthSpd);

                if (bu->changeSpd)
                    break;

                bu->changeDef = GetStatIncrease(bu->unit.pCharacterData->growthDef);

                if (bu->changeDef)
                    break;

                bu->changeRes = GetStatIncrease(bu->unit.pCharacterData->growthRes);

                if (bu->changeRes)
                    break;

                bu->changeLck = GetStatIncrease(bu->unit.pCharacterData->growthLck);

                if (bu->changeLck)
                    break;
            }
        }

        CheckBattleUnitStatCaps(GetUnit(bu->unit.index), bu);
    }
}

void ApplyUnitDefaultPromotion(struct Unit* unit) {
    const struct ClassData* promotedClass = GetClassData(unit->pClassData->promotion);

    int baseClassId = UNIT_CLASS_ID(unit);
    int promClassId = promotedClass->number;

    int i;

    // Apply stat ups

    unit->maxHP += promotedClass->promotionHp;

    if (unit->maxHP > promotedClass->maxHP)
        unit->maxHP = promotedClass->maxHP;

    unit->pow += promotedClass->promotionPow;

    if (unit->pow > promotedClass->maxPow)
        unit->pow = promotedClass->maxPow;

    unit->skl += promotedClass->promotionSkl;

    if (unit->skl > promotedClass->maxSkl)
        unit->skl = promotedClass->maxSkl;

    unit->spd += promotedClass->promotionSpd;

    if (unit->spd > promotedClass->maxSpd)
        unit->spd = promotedClass->maxSpd;

    unit->def += promotedClass->promotionDef;

    if (unit->def > promotedClass->maxDef)
        unit->def = promotedClass->maxDef;

    unit->res += promotedClass->promotionRes;

    if (unit->res > promotedClass->maxRes)
        unit->res = promotedClass->maxRes;

    // Remove base class' base wexp from unit wexp
    for (i = 0; i < 8; ++i)
        unit->ranks[i] -= unit->pClassData->baseRanks[i];

    // Update unit class
    unit->pClassData = promotedClass;

    // Add promoted class' base wexp to unit wexp
    for (i = 0; i < 8; ++i) {
        int wexp = unit->ranks[i];

        wexp += unit->pClassData->baseRanks[i];

        if (wexp > WPN_EXP_S)
            wexp = WPN_EXP_S;

        unit->ranks[i] = wexp;
    }

    // If Pupil -> Shaman promotion, set Anima rank to 0
    if (baseClassId == CLASS_PUPIL && promClassId == CLASS_SHAMAN)
        unit->ranks[ITYPE_ANIMA] = 0;

    unit->level = 1;
    unit->exp   = 0;

    unit->curHP += promotedClass->promotionHp;

    if (unit->curHP > GetUnitMaxHp(unit))
        unit->curHP = GetUnitMaxHp(unit);
}

void ApplyUnitPromotion(struct Unit* unit, u8 classId) {
    const struct ClassData* promotedClass = GetClassData(classId);

    int baseClassId = UNIT_CLASS_ID(unit);
    int promClassId = promotedClass->number;

    int i;

    // Apply stat ups

    unit->maxHP += promotedClass->promotionHp;

    if (unit->maxHP > promotedClass->maxHP)
        unit->maxHP = promotedClass->maxHP;

    unit->pow += promotedClass->promotionPow;

    if (unit->pow > promotedClass->maxPow)
        unit->pow = promotedClass->maxPow;

    unit->skl += promotedClass->promotionSkl;

    if (unit->skl > promotedClass->maxSkl)
        unit->skl = promotedClass->maxSkl;

    unit->spd += promotedClass->promotionSpd;

    if (unit->spd > promotedClass->maxSpd)
        unit->spd = promotedClass->maxSpd;

    unit->def += promotedClass->promotionDef;

    if (unit->def > promotedClass->maxDef)
        unit->def = promotedClass->maxDef;

    unit->res += promotedClass->promotionRes;

    if (unit->res > promotedClass->maxRes)
        unit->res = promotedClass->maxRes;

    // Remove base class' base wexp from unit wexp
    for (i = 0; i < 8; ++i)
        unit->ranks[i] -= unit->pClassData->baseRanks[i];

    // Update unit class
    unit->pClassData = promotedClass;

    // Add promoted class' base wexp to unit wexp
    for (i = 0; i < 8; ++i) {
        int wexp = unit->ranks[i];

        wexp += unit->pClassData->baseRanks[i];

        if (wexp > WPN_EXP_S)
            wexp = WPN_EXP_S;

        unit->ranks[i] = wexp;
    }

    // If Pupil -> Shaman promotion, set Anima rank to 0
    if (baseClassId == CLASS_PUPIL && promClassId == CLASS_SHAMAN)
        unit->ranks[ITYPE_ANIMA] = 0;

    unit->level = 1;
    unit->exp   = 0;

    unit->curHP += promotedClass->promotionHp;

    if (unit->curHP > GetUnitMaxHp(unit))
        unit->curHP = GetUnitMaxHp(unit);
}

void GenerateBattleUnitStatGainsComparatively(struct BattleUnit* bu, struct Unit* unit) {
    bu->changeHP  = bu->unit.maxHP - unit->maxHP;
    bu->changePow = bu->unit.pow   - unit->pow;
    bu->changeSkl = bu->unit.skl   - unit->skl;
    bu->changeSpd = bu->unit.spd   - unit->spd;
    bu->changeDef = bu->unit.def   - unit->def;
    bu->changeRes = bu->unit.res   - unit->res;
    bu->changeLck = bu->unit.lck   - unit->lck;

    if (bu->unit.conBonus != unit->conBonus)
        bu->changeCon = bu->unit.conBonus - unit->conBonus;
    else {
        bu->changeCon = 0;
        bu->unit.conBonus = unit->conBonus;
    }
}

void CheckBattleUnitStatCaps(struct Unit* unit, struct BattleUnit* bu) {
    if ((unit->maxHP + bu->changeHP) > UNIT_MHP_MAX(unit))
        bu->changeHP = UNIT_MHP_MAX(unit) - unit->maxHP;

    if ((unit->pow + bu->changePow) > UNIT_POW_MAX(unit))
        bu->changePow = UNIT_POW_MAX(unit) - unit->pow;

    if ((unit->skl + bu->changeSkl) > UNIT_SKL_MAX(unit))
        bu->changeSkl = UNIT_SKL_MAX(unit) - unit->skl;

    if ((unit->spd + bu->changeSpd) > UNIT_SPD_MAX(unit))
        bu->changeSpd = UNIT_SPD_MAX(unit) - unit->spd;

    if ((unit->def + bu->changeDef) > UNIT_DEF_MAX(unit))
        bu->changeDef = UNIT_DEF_MAX(unit) - unit->def;

    if ((unit->res + bu->changeRes) > UNIT_RES_MAX(unit))
        bu->changeRes = UNIT_RES_MAX(unit) - unit->res;

    if ((unit->lck + bu->changeLck) > UNIT_LCK_MAX(unit))
        bu->changeLck = UNIT_LCK_MAX(unit) - unit->lck;
}

void BattleApplyUnitUpdates(void) {
    struct Unit* actor  = GetUnit(gBattleActor.unit.index);
    struct Unit* target = GetUnit(gBattleTarget.unit.index);

    if (gBattleActor.canCounter)
        gBattleActor.unit.items[gBattleActor.weaponSlotIndex] = gBattleActor.weapon;

    if (gBattleTarget.canCounter)
        gBattleTarget.unit.items[gBattleTarget.weaponSlotIndex] = gBattleTarget.weapon;

    UpdateUnitFromBattle(actor, &gBattleActor);

    if (target)
        UpdateUnitFromBattle(target, &gBattleTarget);
    else
        UpdateObstacleFromBattle(&gBattleTarget);
}

// unused?
s8 sub_802C0B0(void) {
    return TRUE;
}

int GetBattleUnitUpdatedWeaponExp(struct BattleUnit* bu) {
    int i, result;

    if (UNIT_FACTION(&bu->unit) != FACTION_BLUE)
        return -1;

    if (bu->unit.curHP == 0)
        return -1;

    if (gUnknown_0202BCF0.chapterStateBits & CHAPTER_FLAG_7)
        return -1;

    if (gUnknown_0202BCB0.gameStateBits & 0x40) // TODO: GAME STATE BITS CONSTANTS
        return -1;

    if (!(gBattleStats.config & BATTLE_CONFIG_ARENA)) {
        if (!bu->canCounter)
            return -1;

        if (!(bu->weaponAttributes & IA_REQUIRES_WEXP))
            return -1;

        if (bu->weaponAttributes & (IA_MAGICDAMAGE | IA_LOCK_3))
            return -1;
    }

    result = bu->unit.ranks[bu->weaponType];
    result += GetItemAwardedExp(bu->weapon) * bu->wexpMultiplier;

    for (i = 0; i < 8; ++i) {
        if (i == bu->weaponType)
            continue;

        if (bu->unit.pClassData->baseRanks[i] == WPN_EXP_S)
            continue;

        if (bu->unit.ranks[i] < WPN_EXP_S)
            continue;

        if (result >= WPN_EXP_S)
            result = (WPN_EXP_S - 1);

        break;
    }

    if (UNIT_CATTRIBUTES(&bu->unit) & CA_PROMOTED) {
        if (result > WPN_EXP_S)
            result = WPN_EXP_S;
    } else if (UNIT_CATTRIBUTES(&bu->unit) & CA_MAXLEVEL10) {
        if (result > WPN_EXP_C)
            result = WPN_EXP_C;
    } else {
        if (result > WPN_EXP_A)
            result = WPN_EXP_A;
    }

    return result;
}

s8 HasBattleUnitGainedWeaponLevel(struct BattleUnit* bu) {
    int oldWexp = bu->unit.ranks[bu->weaponType];
    int newWexp = GetBattleUnitUpdatedWeaponExp(bu);

    if (newWexp < 0)
        return FALSE;

    return GetWeaponLevelFromExp(oldWexp) != GetWeaponLevelFromExp(newWexp);
}

void UpdateUnitFromBattle(struct Unit* unit, struct BattleUnit* bu) {
    int tmp;

    unit->level = bu->unit.level;
    unit->exp   = bu->unit.exp;
    unit->curHP = bu->unit.curHP;
    unit->state = bu->unit.state;

    gUnknown_03003060 = UNIT_ARENA_LEVEL(unit);

    if (bu->statusOut >= 0)
        SetUnitStatus(unit, bu->statusOut);

    unit->maxHP += bu->changeHP;
    unit->pow   += bu->changePow;
    unit->skl   += bu->changeSkl;
    unit->spd   += bu->changeSpd;
    unit->def   += bu->changeDef;
    unit->res   += bu->changeRes;
    unit->lck   += bu->changeLck;

    UnitCheckStatCaps(unit);

    tmp = GetBattleUnitUpdatedWeaponExp(bu);

    if (tmp > 0)
        unit->ranks[bu->weaponType] = tmp;

    for (tmp = 0; tmp < UNIT_ITEM_COUNT; ++tmp)
        unit->items[tmp] = bu->unit.items[tmp];

    UnitRemoveInvalidItems(unit);

    if (bu->expGain)
        BWL_AddExpGained(UNIT_CHAR_ID(unit), bu->expGain);
}

void UpdateUnitDuringBattle(struct Unit* unit, struct BattleUnit* bu) {
    int wexp;

    unit->curHP = bu->unit.curHP;

    wexp = GetBattleUnitUpdatedWeaponExp(bu);

    if (wexp > 0)
        unit->ranks[bu->weaponType] = wexp;
}

void BattleApplyBallistaUpdates(void) {
    if (gBattleStats.config & BATTLE_CONFIG_BALLISTA) {
        int uses = GetItemUses(gBattleActor.weapon);
        GetTrap(gBattleActor.unit.ballistaIndex)->data[TRAP_EXTDATA_BLST_ITEMUSES] = uses;
    }
}

// ???
void sub_802C334(void) {
    sUnknown_0203A60C.unk00 = 0;
    sUnknown_0203A60C.unk01 = 0;
    sUnknown_0203A60C.unk02 = 0;
}

int GetUnitExpLevel(struct Unit* unit) {
    int result = unit->level;

    if (UNIT_CATTRIBUTES(unit) & CA_PROMOTED)
        result += 20;

    return result;
}

int GetUnitRoundExp(struct Unit* actor, struct Unit* target) {
    int expLevel;

    expLevel = GetUnitExpLevel(actor);
    expLevel = expLevel - GetUnitExpLevel(target);
    expLevel = 31 - expLevel;

    if (expLevel < 0)
        expLevel = 0;

    return expLevel / actor->pClassData->classRelativePower;
}

int GetUnitPowerLevel(struct Unit* unit) {
    int result = unit->level * unit->pClassData->classRelativePower;

    if ((UNIT_CATTRIBUTES(unit) & CA_PROMOTED) && unit->pClassData->promotion)
        result = result + 20 * GetClassData(unit->pClassData->promotion)->classRelativePower;

    return result;
}

int GetUnitClassKillExpBonus(struct Unit* actor, struct Unit* target) {
    int result = 0;

    if (UNIT_CATTRIBUTES(target) & CA_THIEF)
        result += 20;

    if (UNIT_CATTRIBUTES(target) & CA_BOSS)
        result += 40;

    if (UNIT_CLASS_ID(target) == CLASS_ENTOUMBED)
        result += 40;

    return result;
}

int GetUnitExpMultiplier(struct Unit* actor, struct Unit* target) {
    int i;

    if (!(UNIT_CATTRIBUTES(actor) & CA_ASSASSIN))
        return 1;

    for (i = 0; i < BATTLE_HIT_MAX; ++i)
        if (gBattleHitArray[i].attributes & BATTLE_HIT_ATTR_SILENCER)
            return 2;

    return 1;
}

int GetUnitKillExpBonus(struct Unit* actor, struct Unit* target) {
    int result;

    if (target->curHP != 0)
        return 0;

    result = 20;

    // TODO: All the definitions
    if ((gUnknown_0202BCB0.gameStateBits & 0x40) || (gUnknown_0202BCF0.chapterModeIndex != 1)) {
        result = GetUnitPowerLevel(target);

        result += 20;
        result -= GetUnitPowerLevel(actor);
    } else {
        int local = GetUnitPowerLevel(target);

        if (local <= GetUnitPowerLevel(actor))
            local = GetUnitPowerLevel(target) - GetUnitPowerLevel(actor) / 2;
        else
            local = GetUnitPowerLevel(target) - GetUnitPowerLevel(actor);

        result += local;
    }

    result += GetUnitClassKillExpBonus(actor, target);
    result *= GetUnitExpMultiplier(actor, target);

    if (result < 0)
        result = 0;

    return result;
}

void ModifyUnitSpecialExp(struct Unit* actor, struct Unit* target, int* exp) {
    if (UNIT_IS_GORGON_EGG(target)) {
        if (target->curHP == 0)
            *exp = 50;
        else
            *exp = 0;
    }

    if (UNIT_CLASS_ID(target) == CLASS_DEMON_KING)
        if (target->curHP == 0)
            *exp = 0;

    if (UNIT_CLASS_ID(actor) == CLASS_PHANTOM)
        *exp = 0;
}

int GetBattleUnitExpGain(struct BattleUnit* actor, struct BattleUnit* target) {
    int result;

    if (!CanBattleUnitGainLevels(actor) || (actor->unit.curHP == 0) || UNIT_CATTRIBUTES(&target->unit) & CA_NEGATE_LETHALITY)
        return 0;

    if (!actor->nonZeroDamage)
        return 1;

    result = GetUnitRoundExp(&actor->unit, &target->unit);
    result += GetUnitKillExpBonus(&actor->unit, &target->unit);

    if (result > 100)
        result = 100;

    if (result < 1)
        result = 1;

    ModifyUnitSpecialExp(&actor->unit, &target->unit, &result);

    return result;
}

void BattleApplyItemExpGains(void) {
    if (!(gUnknown_0202BCF0.chapterStateBits & CHAPTER_FLAG_7)) {
        if (gBattleActor.weaponAttributes & IA_STAFF) {
            if (UNIT_FACTION(&gBattleActor.unit) == FACTION_BLUE)
                gBattleActor.wexpMultiplier++;

            gBattleActor.expGain = GetBattleUnitStaffExp(&gBattleActor);
            gBattleActor.unit.exp += gBattleActor.expGain;

            CheckBattleUnitLevelUp(&gBattleActor);
        } else if ((gBattleActor.weaponType == ITYPE_12) && (gBattleActor.unit.exp != UNIT_EXP_DISABLED)) {
            gBattleActor.expGain = 20;
            gBattleActor.unit.exp += 20;

            CheckBattleUnitLevelUp(&gBattleActor);
        }
    }
}

int GetBattleUnitStaffExp(struct BattleUnit* bu) {
    int result;

    if (!CanBattleUnitGainLevels(bu))
        return 0;

    if (gBattleHitArray->attributes & BATTLE_HIT_ATTR_MISS)
        return 1;

    result = 10 + GetItemCostPerUse(bu->weapon) / 20;

    if (UNIT_CATTRIBUTES(&bu->unit) & CA_PROMOTED)
        result = result / 2;

    if (result > 100)
        result = 100;

    return result;
}

void BattleApplyMiscActionExpGains(void) {
    if (UNIT_FACTION(&gBattleActor.unit) != FACTION_BLUE)
        return;

    if (!CanBattleUnitGainLevels(&gBattleActor))
        return;

    if (gUnknown_0202BCF0.chapterStateBits & CHAPTER_FLAG_7)
        return;

    gBattleActor.expGain = 10;
    gBattleActor.unit.exp += 10;

    CheckBattleUnitLevelUp(&gBattleActor);
}

void BattleUnitTargetSetEquippedWeapon(struct BattleUnit* bu) {
    int i;

    if (bu->weaponBefore)
        return;

    bu->weaponBefore = GetUnitEquippedWeapon(&bu->unit);

    if (bu->weaponBefore)
        return;

    if (!UnitHasMagicRank(&bu->unit))
        return;

    for (i = 0; i < UNIT_ITEM_COUNT; ++i) {
        if (CanUnitUseStaff(&bu->unit, bu->unit.items[i]) == TRUE) {
            bu->weaponBefore = bu->unit.items[i];
            break;
        }
    }
}

void BattleUnitTargetCheckCanCounter(struct BattleUnit* bu) {
    if (!bu->canCounter) {
        bu->battleAttack = 0xFF;
        bu->battleHitRate = 0xFF;
        bu->battleEffectiveHitRate = 0xFF;
        bu->battleCritRate = 0xFF;
        bu->battleEffectiveCritRate = 0xFF;
    }
}

void BattleApplyReaverEffect(struct BattleUnit* attacker, struct BattleUnit* defender) {
    if (!(attacker->weaponAttributes & IA_REVERTTRIANGLE) || !(defender->weaponAttributes & IA_REVERTTRIANGLE)) {
        attacker->wTriangleHitBonus = -(attacker->wTriangleHitBonus * 2);
        attacker->wTriangleDmgBonus = -(attacker->wTriangleDmgBonus * 2);
        defender->wTriangleHitBonus = -(defender->wTriangleHitBonus * 2);
        defender->wTriangleDmgBonus = -(defender->wTriangleDmgBonus * 2);
    }
}

void BattleApplyWeaponTriangleEffect(struct BattleUnit* attacker, struct BattleUnit* defender) {
    const struct WeaponTriangleRule* it;

    for (it = sWeaponTriangleRules; it->attackerWeaponType >= 0; ++it) {
        if ((attacker->weaponType == it->attackerWeaponType) && (defender->weaponType == it->defenderWeaponType)) {
            attacker->wTriangleHitBonus = it->hitBonus;
            attacker->wTriangleDmgBonus = it->atkBonus;

            defender->wTriangleHitBonus = -it->hitBonus;
            defender->wTriangleDmgBonus = -it->atkBonus;

            break;
        }
    }

    if (attacker->weaponAttributes & IA_REVERTTRIANGLE)
        BattleApplyReaverEffect(attacker, defender);

    if (defender->weaponAttributes & IA_REVERTTRIANGLE)
        BattleApplyReaverEffect(attacker, defender);
}

void BattleInitTargetCanCounter(void) {
    // Target cannot counter if it is a gorgon egg

    if (UNIT_IS_GORGON_EGG(&gBattleTarget.unit)) {
        gBattleTarget.weapon = 0;
        gBattleTarget.canCounter = FALSE;
    }

    // Target cannot counter if either units are using "uncounterable" weapons

    if ((gBattleActor.weaponAttributes | gBattleTarget.weaponAttributes) & IA_UNCOUNTERABLE) {
        gBattleTarget.weapon = 0;
        gBattleTarget.canCounter = FALSE;
    }

    // Target cannot counter if a berserked player unit is attacking another player unit

    if (gBattleActor.unit.statusIndex == UNIT_STATUS_BERSERK) {
        if ((UNIT_FACTION(&gBattleActor.unit) == FACTION_BLUE) && (UNIT_FACTION(&gBattleTarget.unit) == FACTION_BLUE)) {
            gBattleTarget.weapon = 0;
            gBattleTarget.canCounter = FALSE;
        }
    }
}

void InitObstacleBattleUnit(void) {
    ClearUnit(&gBattleTarget.unit);

    gBattleTarget.unit.index = 0;

    gBattleTarget.unit.pClassData = GetClassData(CLASS_OBSTACLE);

    gBattleTarget.unit.maxHP = GetROMChapterStruct(gUnknown_0202BCF0.chapterIndex)->mapCrackedWallHeath;
    gBattleTarget.unit.curHP = gActionData.trapType; // TODO: better

    gBattleTarget.unit.xPos  = gActionData.xOther;
    gBattleTarget.unit.yPos  = gActionData.yOther;

    switch (gBmMapTerrain[gBattleTarget.unit.yPos][gBattleTarget.unit.xPos]) {

    case TERRAIN_WALL_1B:
        gBattleTarget.unit.pCharacterData = GetCharacterData(CHARACTER_WALL);

        break;

    case TERRAIN_SNAG:
        gBattleTarget.unit.pCharacterData = GetCharacterData(CHARACTER_SNAG);
        gBattleTarget.unit.maxHP = 20;

        break;

    } // switch (gBmMapTerrain[gBattleTarget.unit.yPos][gBattleTarget.unit.xPos])
}

void ComputeBattleObstacleStats(void) {
    gBattleActor.battleEffectiveHitRate = 100;
    gBattleActor.battleEffectiveCritRate = 0;

    gBattleTarget.battleSpeed = 0xFF;
    gBattleTarget.hpInitial = gBattleTarget.unit.curHP;

    gBattleTarget.wTriangleHitBonus = 0;
    gBattleTarget.wTriangleDmgBonus = 0;
}

void UpdateObstacleFromBattle(struct BattleUnit* bu) {
    struct Trap* trap = GetTrapAt(bu->unit.xPos, bu->unit.yPos);

    trap->extra = bu->unit.curHP;

    if (trap->extra == 0) {
        int mapChangeId = GetMapChangeIdAt(bu->unit.xPos, bu->unit.yPos);

        if (gBmMapTerrain[bu->unit.yPos][bu->unit.xPos] == TERRAIN_SNAG)
            PlaySoundEffect(0x2D7); // TODO: Sound id constants

        RenderBmMapOnBg2();

        ApplyMapChangesById(mapChangeId);

        // This is kind of jank: it sets trap type to 0 (TRAP_NONE) which should be the terminating id
        // But then immediately calls the map change trap adding routine, which would effectively replace
        // the 0-id trap with the new map change trap, even if it is not actually the end of the trap array

        trap->type = TRAP_NONE;
        EnableMapChange(mapChangeId);

        RefreshTerrainBmMap();
        UpdateRoofedUnits();
        RenderBmMap();

        NewBMXFADE(FALSE);
    }
}

void BeginBattleAnimations(void) {
    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(1 << 2);

    gPaletteBuffer[0] = 0;
    EnablePaletteSync();

    RenderBmMap();

    if (sub_8055BC4()) {
        sub_804FD48(0);
        BeginAnimsOnBattleAnimations();
    } else {
        MU_EndAll();
        RenderBmMap();
        BeginBattleMapAnims();

        gBattleStats.config |= BATTLE_CONFIG_MAPANIMS;
    }
}

int GetUnitSoloBattleAnimType(struct Unit* unit) {
    // TODO: battle anim type constants

    if (unit->state & US_SOLOANIM_1)
        return 0;

    if (unit->state & US_SOLOANIM_2)
        return 3;

    return 1;
}

int GetBattleAnimType(void) {
    // TODO: battle anim type constants

    // If not solo anim, return global type
    if (gUnknown_0202BCF0.unk42_2 != 2)
        return gUnknown_0202BCF0.unk42_2;

    // If both units are players, use actor solo anim type
    if (UNIT_FACTION(&gBattleActor.unit) == FACTION_BLUE)
        if (UNIT_FACTION(&gBattleTarget.unit) == FACTION_BLUE)
            return GetUnitSoloBattleAnimType(&gBattleActor.unit);

    // If neither are players, return 1
    if (UNIT_FACTION(&gBattleActor.unit) != FACTION_BLUE)
        if (UNIT_FACTION(&gBattleTarget.unit) != FACTION_BLUE)
            return 1;

    // Return solo anim type for the one that is a player unit
    if (UNIT_FACTION(&gBattleActor.unit) == FACTION_BLUE)
        return GetUnitSoloBattleAnimType(&gBattleActor.unit);
    else
        return GetUnitSoloBattleAnimType(&gBattleTarget.unit);
}

void BattlePrintDebugUnitInfo(struct BattleUnit* actor, struct BattleUnit* target) {
    // prints battle unit information to debug output
}

void BattlePrintDebugHitInfo(void) {
    struct BattleHit* it;

    for (it = gBattleHitArray; !(it->info & BATTLE_HIT_INFO_END); ++it) {
        // prints battle rounds information to debug output
    }
}

void BattleInitItemEffect(struct Unit* actor, int itemSlot) {
    int item = actor->items[itemSlot];

    if (itemSlot < 0)
        item = 0;

    gBattleStats.config = 0;

    InitBattleUnit(&gBattleActor, actor);

    SetBattleUnitTerrainBonusesAuto(&gBattleActor);
    ComputeBattleUnitBaseDefense(&gBattleActor);
    ComputeBattleUnitSupportBonuses(&gBattleActor, NULL);

    gBattleActor.battleAttack = 0xFF;
    gBattleActor.battleEffectiveHitRate = 100;
    gBattleActor.battleEffectiveCritRate = 0xFF;

    gBattleActor.weapon = item;
    gBattleActor.weaponBefore = item;
    gBattleActor.weaponSlotIndex = itemSlot;
    gBattleActor.weaponType = GetItemType(item);
    gBattleActor.weaponAttributes = GetItemAttributes(item);

    gBattleActor.canCounter = TRUE;
    gBattleActor.hasItemEffectTarget = FALSE;

    gBattleActor.statusOut = -1;
    gBattleTarget.statusOut = -1;

    ClearBattleHits();
}

void BattleInitItemEffectTarget(struct Unit* unit) {
    InitBattleUnit(&gBattleTarget, unit);

    SetBattleUnitTerrainBonusesAuto(&gBattleTarget);
    ComputeBattleUnitBaseDefense(&gBattleTarget);
    ComputeBattleUnitSupportBonuses(&gBattleTarget, NULL);

    gBattleTarget.battleAttack = 0xFF;
    gBattleTarget.battleEffectiveHitRate = 0xFF;
    gBattleTarget.battleEffectiveCritRate = 0xFF;

    gBattleTarget.weaponBefore = 0;

    BattleUnitTargetSetEquippedWeapon(&gBattleTarget);

    gBattleActor.hasItemEffectTarget = TRUE;
}

void UpdateActorFromBattle(void) {
    UpdateUnitFromBattle(GetUnit(gBattleActor.unit.index), &gBattleActor);
}

void BattleApplyMiscAction(struct Proc* proc) {
    BattleApplyMiscActionExpGains();
    Proc_CreateBlockingChild(sProcScr_BattleAnimSimpleLock, proc);
}

void BattleApplyItemEffect(struct Proc* proc) {
    (++gBattleHitIterator)->info = BATTLE_HIT_INFO_END;

    BattleApplyItemExpGains();

    if (gBattleActor.canCounter) {
        if (GetItemAttributes(gBattleActor.weapon) & IA_STAFF)
            gBattleActor.weaponBroke = TRUE;

        gBattleActor.weapon = GetItemAfterUse(gBattleActor.weapon);
        gBattleActor.unit.items[gBattleActor.weaponSlotIndex] = gBattleActor.weapon;

        if (gBattleActor.weapon)
            gBattleActor.weaponBroke = FALSE;
    }

    Proc_CreateBlockingChild(sProcScr_BattleAnimSimpleLock, proc);
}

int GetOffensiveStaffAccuracy(struct Unit* actor, struct Unit* target) {
    int baseAccuracy = (GetUnitPower(actor) - GetUnitResistance(target)) * 5;
    int unitSkill = GetUnitSkill(actor);
    int distance = RECT_DISTANCE(actor->xPos, actor->yPos, target->xPos, target->yPos);

    int result;

    if (UNIT_CLASS_ID(actor) == CLASS_DEMON_KING)
        result = (baseAccuracy + unitSkill) - distance * 2;
    else
        result = (baseAccuracy + 30 + unitSkill) - distance * 2;

    if ((UNIT_CLASS_ID(target) == CLASS_DEMON_KING) || (UNIT_CHAR_ID(target) == CHARACTER_LYON) || (UNIT_CHAR_ID(target) == CHARACTER_LYON_FINAL))
        return 0;

    if (result < 0)
        result = 0;

    if (result > 100)
        result = 100;

    return result;
}

void BattleGenerateArena(struct Unit* actor) {
    struct Unit* target = gUnknown_0203A8F0.opponentUnit;
    int something = gUnknown_0202BCB0.unk3C;

    gBattleStats.config = BATTLE_CONFIG_REAL | BATTLE_CONFIG_ARENA;

    InitBattleUnit(&gBattleActor, actor);
    InitBattleUnit(&gBattleTarget, target);

    if (gActionData.trapType) {
        gBattleTarget.unit.curHP = gActionData.trapType;
        gBattleTarget.hpInitial = gActionData.trapType;
    }

    gBattleStats.range = gUnknown_0203A8F0.range;

    gBattleTarget.unit.xPos = gBattleActor.unit.xPos + gUnknown_0203A8F0.range;
    gBattleTarget.unit.yPos = gBattleActor.unit.yPos;

    SetBattleUnitWeapon(&gBattleActor, BU_ISLOT_ARENA_PLAYER);
    SetBattleUnitWeapon(&gBattleTarget, BU_ISLOT_ARENA_OPPONENT);

    BattleApplyWeaponTriangleEffect(&gBattleActor, &gBattleTarget);

    gActionData.suspendPointType = SUSPEND_POINT_DURINGARENA;
    SaveSuspendedGame(SAVE_BLOCK_SUSPEND_BASE);

    SetBattleUnitTerrainBonusesAuto(&gBattleActor);
    SetBattleUnitTerrainBonuses(&gBattleTarget, 8); // TODO: terrain id constants

    BattleGenerate(actor, target);

    if (gBattleTarget.unit.curHP == 0)
        BattleApplyExpGains();

    UpdateUnitDuringBattle(actor, &gBattleActor);

    if (!something || (gBattleTarget.unit.curHP == 0)) {
        sub_80A4AA4();

        actor->state = (actor->state &~ (US_BIT17 | US_BIT18 | US_BIT19))
            + ((((UNIT_ARENA_LEVEL(actor) + 1) <= 7) ? (UNIT_ARENA_LEVEL(actor) + 1) << 17 : 7 << 17));

        gUnknown_03003060 = UNIT_ARENA_LEVEL(actor);
    }

    BattlePrintDebugUnitInfo(&gBattleActor, &gBattleTarget);
}

s8 BattleIsTriangleAttack(void) {
    return (gBattleHitArray->attributes & BATTLE_HIT_ATTR_TATTACK) != 0;
}

s8 DidBattleUnitBreakWeapon(struct BattleUnit* bu) {
    if (bu->unit.curHP == 0)
        return FALSE;

    return bu->weaponBroke;
}

void SetScriptedBattle(struct BattleHit* hits) {
    gActionData.scriptedBattleHits = hits;
}

void BattleGenerateHitScriptedDamage(struct BattleUnit* bu) {
    gBattleStats.damage = 0;

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS)) {
        if (gBattleHitIterator->hpChange == 0) {
            gBattleStats.damage = gBattleStats.attack - gBattleStats.defense;

            if (gBattleHitIterator->attributes & BATTLE_HIT_ATTR_CRIT)
                gBattleStats.damage = 3 * gBattleStats.damage;
        } else
            gBattleStats.damage = gBattleHitIterator->hpChange;

        if (gBattleStats.damage > BATTLE_MAX_DAMAGE)
            gBattleStats.damage = BATTLE_MAX_DAMAGE;

        if (gBattleStats.damage < 0)
            gBattleStats.damage = 0;

        if (gBattleStats.damage != 0)
            bu->nonZeroDamage = TRUE;
    }
}

#if NONMATCHING

void BattleUnwindScripted(void) {
    struct BattleUnit* attacker;
    struct BattleUnit* defender;

    struct BattleHit* itIn;
    struct BattleHit* itOut;

    itIn = gActionData.scriptedBattleHits;
    itOut = gBattleHitArray;

    while (!(itIn->info & BATTLE_HIT_INFO_END))
        *itOut++ = *itIn++;

    *itOut = *itIn;

    for (gBattleHitIterator = gBattleHitArray; !(gBattleHitIterator->info & BATTLE_HIT_INFO_END); ++gBattleHitIterator) {
        if (gBattleHitIterator->info & BATTLE_HIT_INFO_RETALIATION) {
            attacker = &gBattleTarget;
            defender = &gBattleActor;
        } else {
            attacker = &gBattleActor;
            defender = &gBattleTarget;
        }

        BattleUpdateBattleStats(attacker, defender);
        BattleGenerateHitScriptedDamage(attacker);
        BattleGenerateHitEffects(attacker, defender);

        if ((attacker->unit.curHP == 0) || (defender->unit.curHP == 0)) {
            attacker->wexpMultiplier++;

            gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;

            if (gBattleTarget.unit.curHP == 0)
                gBattleHitIterator->info |= BATTLE_HIT_INFO_KILLS_TARGET;

            (gBattleHitIterator + 1)->info = BATTLE_HIT_INFO_END;

            break;
        }

        if (
            (defender->unit.statusIndex == UNIT_STATUS_PETRIFY) ||
            (defender->unit.statusIndex == UNIT_STATUS_13) ||
            (defender->statusOut == UNIT_STATUS_PETRIFY) ||
            (defender->statusOut == UNIT_STATUS_13)
        ) {
            attacker->wexpMultiplier++;

            gBattleHitIterator->info |= BATTLE_HIT_INFO_FINISHES;

            (gBattleHitIterator + 1)->info = BATTLE_HIT_INFO_END;

            break;
        }
    }

    gActionData.scriptedBattleHits = NULL;
}

#else // if !NONMATCHING

__attribute__((naked))
void BattleUnwindScripted(void) {
    /* :( */

    asm("\n\
        .syntax unified\n\
        push {r4, r5, r6, r7, lr}\n\
        ldr r0, _0802CFA4  @ gActionData\n\
        ldr r3, [r0, #0x18]\n\
        ldr r4, _0802CFA8  @ gBattleHitArray\n\
        ldr r2, [r3]\n\
        lsls r0, r2, #8\n\
        lsrs r0, r0, #0x1b\n\
        movs r1, #0x10\n\
        ands r0, r1\n\
        adds r5, r4, #0\n\
        ldr r6, _0802CFAC  @ gBattleHitIterator\n\
        cmp r0, #0\n\
        bne _0802CF76\n\
    _0802CF66:\n\
        stm r4!, {r2}\n\
        adds r3, #4\n\
        ldr r2, [r3]\n\
        lsls r0, r2, #8\n\
        lsrs r0, r0, #0x1b\n\
        ands r0, r1\n\
        cmp r0, #0\n\
        beq _0802CF66\n\
    _0802CF76:\n\
        ldr r0, [r3]\n\
        str r0, [r4]\n\
        str r5, [r6]\n\
        ldr r0, [r5]\n\
        lsls r0, r0, #8\n\
        lsrs r0, r0, #0x1b\n\
        movs r1, #0x10\n\
        ands r0, r1\n\
        cmp r0, #0\n\
        beq _0802CF8C\n\
        b _0802D0AC\n\
    _0802CF8C:\n\
        movs r7, #7\n\
    _0802CF8E:\n\
        ldr r0, [r6]\n\
        ldr r0, [r0]\n\
        lsls r0, r0, #8\n\
        lsrs r0, r0, #0x1b\n\
        movs r1, #8\n\
        ands r0, r1\n\
        cmp r0, #0\n\
        beq _0802CFB8\n\
        ldr r4, _0802CFB0  @ gBattleTarget\n\
        ldr r5, _0802CFB4  @ gBattleActor\n\
        b _0802CFBC\n\
        .align 2, 0\n\
    _0802CFA4: .4byte gActionData\n\
    _0802CFA8: .4byte gBattleHitArray\n\
    _0802CFAC: .4byte gBattleHitIterator\n\
    _0802CFB0: .4byte gBattleTarget\n\
    _0802CFB4: .4byte gBattleActor\n\
    _0802CFB8:\n\
        ldr r4, _0802D034  @ gBattleActor\n\
        ldr r5, _0802D038  @ gBattleTarget\n\
    _0802CFBC:\n\
        adds r0, r4, #0\n\
        adds r1, r5, #0\n\
        bl BattleUpdateBattleStats\n\
        adds r0, r4, #0\n\
        bl BattleGenerateHitScriptedDamage\n\
        adds r0, r4, #0\n\
        adds r1, r5, #0\n\
        bl BattleGenerateHitEffects\n\
        movs r0, #0x13\n\
        ldrsb r0, [r4, r0]\n\
        cmp r0, #0\n\
        beq _0802CFE2\n\
        movs r0, #0x13\n\
        ldrsb r0, [r5, r0]\n\
        cmp r0, #0\n\
        bne _0802D040\n\
    _0802CFE2:\n\
        adds r1, r4, #0\n\
        adds r1, #0x7b\n\
        ldrb r0, [r1]\n\
        adds r0, #1\n\
        strb r0, [r1]\n\
        ldr r5, _0802D03C  @ gBattleHitIterator\n\
        ldr r3, [r5]\n\
        ldr r1, [r3]\n\
        lsls r1, r1, #8\n\
        lsrs r1, r1, #0x1b\n\
        movs r0, #2\n\
        orrs r1, r0\n\
        lsls r1, r1, #3\n\
        ldrb r2, [r3, #2]\n\
        movs r4, #7\n\
        adds r0, r4, #0\n\
        ands r0, r2\n\
        orrs r0, r1\n\
        strb r0, [r3, #2]\n\
        ldr r0, _0802D038  @ gBattleTarget\n\
        ldrb r0, [r0, #0x13]\n\
        lsls r0, r0, #0x18\n\
        asrs r0, r0, #0x18\n\
        cmp r0, #0\n\
        bne _0802D02C\n\
        ldr r3, [r5]\n\
        ldr r1, [r3]\n\
        lsls r1, r1, #8\n\
        lsrs r1, r1, #0x1b\n\
        movs r0, #4\n\
        orrs r1, r0\n\
        lsls r1, r1, #3\n\
        ldrb r2, [r3, #2]\n\
        adds r0, r4, #0\n\
        ands r0, r2\n\
        orrs r0, r1\n\
        strb r0, [r3, #2]\n\
    _0802D02C:\n\
        ldr r2, [r5]\n\
        ldrb r1, [r2, #6]\n\
        adds r0, r4, #0\n\
        b _0802D08C\n\
        .align 2, 0\n\
    _0802D034: .4byte gBattleActor\n\
    _0802D038: .4byte gBattleTarget\n\
    _0802D03C: .4byte gBattleHitIterator\n\
    _0802D040:\n\
        adds r0, r5, #0\n\
        adds r0, #0x30\n\
        ldrb r0, [r0]\n\
        movs r1, #0xf\n\
        ands r1, r0\n\
        cmp r1, #0xb\n\
        beq _0802D064\n\
        cmp r1, #0xd\n\
        beq _0802D064\n\
        adds r0, r5, #0\n\
        adds r0, #0x6f\n\
        ldrb r0, [r0]\n\
        lsls r0, r0, #0x18\n\
        asrs r0, r0, #0x18\n\
        cmp r0, #0xb\n\
        beq _0802D064\n\
        cmp r0, #0xd\n\
        bne _0802D096\n\
    _0802D064:\n\
        adds r1, r4, #0\n\
        adds r1, #0x7b\n\
        ldrb r0, [r1]\n\
        adds r0, #1\n\
        strb r0, [r1]\n\
        ldr r3, [r6]\n\
        ldr r1, [r3]\n\
        lsls r1, r1, #8\n\
        lsrs r1, r1, #0x1b\n\
        movs r0, #2\n\
        orrs r1, r0\n\
        lsls r1, r1, #3\n\
        ldrb r2, [r3, #2]\n\
        adds r0, r7, #0\n\
        ands r0, r2\n\
        orrs r0, r1\n\
        strb r0, [r3, #2]\n\
        ldr r2, [r6]\n\
        ldrb r1, [r2, #6]\n\
        adds r0, r7, #0\n\
    _0802D08C:\n\
        ands r0, r1\n\
        movs r1, #0x80\n\
        orrs r0, r1\n\
        strb r0, [r2, #6]\n\
        b _0802D0AC\n\
    _0802D096:\n\
        ldr r1, [r6]\n\
        adds r0, r1, #4\n\
        str r0, [r6]\n\
        ldr r0, [r1, #4]\n\
        lsls r0, r0, #8\n\
        lsrs r0, r0, #0x1b\n\
        movs r1, #0x10\n\
        ands r0, r1\n\
        cmp r0, #0\n\
        bne _0802D0AC\n\
        b _0802CF8E\n\
    _0802D0AC:\n\
        ldr r1, _0802D0B8  @ gActionData\n\
        movs r0, #0\n\
        str r0, [r1, #0x18]\n\
        pop {r4, r5, r6, r7}\n\
        pop {r0}\n\
        bx r0\n\
        .align 2, 0\n\
    _0802D0B8: .4byte gActionData\n\
        .syntax divided\n\
    ");
}

#endif // !NONMATCHING

void UnitLevelUp(struct Unit* unit) {
    if (unit->level != 20) {
        int hpGain, powGain, sklGain, spdGain, defGain, resGain, lckGain;
        int growthBonus;
        int totalGain;

        unit->exp = 0;
        unit->level++;

        if (unit->level == 20)
            unit->exp = UNIT_EXP_DISABLED;

        growthBonus = (unit->state & US_GROWTH_BOOST) ? 5: 0;
        totalGain = 0;

        hpGain  = GetStatIncrease(growthBonus + unit->pCharacterData->growthHP);
        totalGain += hpGain;

        powGain = GetStatIncrease(growthBonus + unit->pCharacterData->growthPow);
        totalGain += powGain;

        sklGain = GetStatIncrease(growthBonus + unit->pCharacterData->growthSkl);
        totalGain += sklGain;

        spdGain = GetStatIncrease(growthBonus + unit->pCharacterData->growthSpd);
        totalGain += spdGain;

        defGain = GetStatIncrease(growthBonus + unit->pCharacterData->growthDef);
        totalGain += defGain;

        resGain = GetStatIncrease(growthBonus + unit->pCharacterData->growthRes);
        totalGain += resGain;

        lckGain = GetStatIncrease(growthBonus + unit->pCharacterData->growthLck);
        totalGain += lckGain;

        if (totalGain == 0) {
            for (totalGain = 0; totalGain < 2; ++totalGain) {
                hpGain = GetStatIncrease(unit->pCharacterData->growthHP);

                if (hpGain)
                    break;

                powGain = GetStatIncrease(unit->pCharacterData->growthPow);

                if (powGain)
                    break;

                sklGain = GetStatIncrease(unit->pCharacterData->growthSkl);

                if (sklGain)
                    break;

                spdGain = GetStatIncrease(unit->pCharacterData->growthSpd);

                if (spdGain)
                    break;

                defGain = GetStatIncrease(unit->pCharacterData->growthDef);

                if (defGain)
                    break;

                resGain = GetStatIncrease(unit->pCharacterData->growthRes);

                if (resGain)
                    break;

                lckGain = GetStatIncrease(unit->pCharacterData->growthLck);

                if (lckGain)
                    break;
            }
        }

        if ((unit->maxHP + hpGain) > UNIT_MHP_MAX(unit))
            hpGain = UNIT_MHP_MAX(unit) - unit->maxHP;

        if ((unit->pow + powGain) > UNIT_POW_MAX(unit))
            powGain = UNIT_POW_MAX(unit) - unit->pow;

        if ((unit->skl + sklGain) > UNIT_SKL_MAX(unit))
            sklGain = UNIT_SKL_MAX(unit) - unit->skl;

        if ((unit->spd + spdGain) > UNIT_SPD_MAX(unit))
            spdGain = UNIT_SPD_MAX(unit) - unit->spd;

        if ((unit->def + defGain) > UNIT_DEF_MAX(unit))
            defGain = UNIT_DEF_MAX(unit) - unit->def;

        if ((unit->res + resGain) > UNIT_RES_MAX(unit))
            resGain = UNIT_RES_MAX(unit) - unit->res;

        if ((unit->lck + lckGain) > UNIT_LCK_MAX(unit))
            lckGain = UNIT_LCK_MAX(unit) - unit->lck;

        unit->maxHP += hpGain;
        unit->pow += powGain;
        unit->skl += sklGain;
        unit->spd += spdGain;
        unit->def += defGain;
        unit->res += resGain;
        unit->lck += lckGain;
    }
}

void BattleHitAdvance(void) {
    gBattleHitIterator++;
}

void BattleHitTerminate(void) {
    gBattleHitIterator++;
    gBattleHitIterator->info = BATTLE_HIT_INFO_END;
}
