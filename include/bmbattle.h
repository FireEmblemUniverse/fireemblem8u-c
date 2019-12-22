#ifndef GUARD_BMBATTLE_H
#define GUARD_BMBATTLE_H

enum {
    // Battle-related magic constants

    // Maximum amount of hits that can happen in a battle (-1, because of the terminating "hit")
    BATTLE_HIT_MAX = 7,

    // Followup attack (double attack) attack speed difference threshold
    BATTLE_FOLLOWUP_SPEED_THRESHOLD = 4,

    // Maximum battle damage
    BATTLE_MAX_DAMAGE = 127,
};

struct BattleUnit {
    /* 00 */ struct Unit unit;

    /* 48 */ u16 weapon;
    /* 4A */ u16 weaponBefore;
    /* 4C */ u32 weaponAttributes;
    /* 50 */ u8 weaponType;
    /* 51 */ u8 weaponSlotIndex;

    /* 52 */ s8 canCounter;

    /* 53 */ s8 wTriangleHitBonus;
    /* 54 */ s8 wTriangleDmgBonus;

    /* 55 */ u8 terrainId;
    /* 56 */ s8 terrainDefense;
    /* 57 */ s8 terrainAvoid;
    /* 58 */ s8 terrainResistance;
    /* 59 */ /* pad */

    /* 5A */ short battleAttack;
    /* 5C */ short battleDefense;
    /* 5E */ short battleSpeed;
    /* 60 */ short battleHitRate;
    /* 62 */ short battleAvoidRate;
    /* 64 */ short battleEffectiveHitRate;
    /* 66 */ short battleCritRate;
    /* 68 */ short battleDodgeRate;
    /* 6A */ short battleEffectiveCritRate;
    /* 6C */ short battleSilencerRate;

    /* 6E */ s8 expGain;
    /* 6F */ s8 statusOut;
    /* 70 */ s8 levelPrevious;
    /* 71 */ s8 expPrevious;

    /* 72 */ u8 hpInitial;

    /* 73 */ s8 changeHP;
    /* 74 */ s8 changePow;
    /* 75 */ s8 changeSkl;
    /* 76 */ s8 changeSpd;
    /* 77 */ s8 changeDef;
    /* 78 */ s8 changeRes;
    /* 79 */ s8 changeLck;
    /* 7A */ s8 changeCon;

    /* 7B */ s8 wexpMultiplier;
    /* 7C */ s8 nonZeroDamage;
    /* 7D */ s8 weaponBroke;

    /* 7E */ s8 hasItemEffectTarget;
    /* 7F */ /* pad */
};

struct BattleStats {
    /* 00 */ u16 config;

    /* 02 */ u8 range;

    /* 04 */ short damage;
    /* 06 */ short attack;
    /* 08 */ short defense;
    /* 0A */ short hitRate;
    /* 0C */ short critRate;
    /* 0E */ short silencerRate;

    /* 10 */ struct Unit* taUnitA;
    /* 14 */ struct Unit* taUnitB;
};

enum {
    // For use with BattleStats:config

    BATTLE_CONFIG_REAL = (1 << 0),
    BATTLE_CONFIG_SIMULATE = (1 << 1),
    BATTLE_CONFIG_BIT2 = (1 << 2),
    BATTLE_CONFIG_BALLISTA = (1 << 3),
    BATTLE_CONFIG_BIT4 = (1 << 4),
    BATTLE_CONFIG_ARENA = (1 << 5),
    BATTLE_CONFIG_REFRESH = (1 << 6),
    BATTLE_CONFIG_MAPANIMS = (1 << 7),
    BATTLE_CONFIG_PROMOTION = (1 << 8),
    BATTLE_CONFIG_DANCERING = (1 << 9),
};

struct BattleHit {
    /* 00:18 */ unsigned attributes : 19;
    /* 19:23 */ unsigned info       : 5;
    /* 24:31 */ signed   hpChange   : 8;
};

enum {
    BATTLE_HIT_ATTR_CRIT      = (1 << 0),
    BATTLE_HIT_ATTR_MISS      = (1 << 1),
    BATTLE_HIT_ATTR_FOLLOWUP  = (1 << 2),
    BATTLE_HIT_ATTR_RETALIATE = (1 << 3),
    BATTLE_HIT_ATTR_BRAVE     = (1 << 4),
    BATTLE_HIT_ATTR_5         = (1 << 5), // unused?
    BATTLE_HIT_ATTR_POISON    = (1 << 6),
    BATTLE_HIT_ATTR_DEVIL     = (1 << 7),
    BATTLE_HIT_ATTR_HPSTEAL   = (1 << 8),
    BATTLE_HIT_ATTR_HPHALVE   = (1 << 9),
    BATTLE_HIT_ATTR_TATTACK   = (1 << 10), // triangle attack!
    BATTLE_HIT_ATTR_SILENCER  = (1 << 11),
    BATTLE_HIT_ATTR_12        = (1 << 12), // unused?
    BATTLE_HIT_ATTR_PETRIFY   = (1 << 13),
    BATTLE_HIT_ATTR_SURESHOT  = (1 << 14),
    BATTLE_HIT_ATTR_GREATSHLD = (1 << 15),
    BATTLE_HIT_ATTR_PIERCE    = (1 << 16),
    BATTLE_HIT_ATTR_17        = (1 << 17), // unused?
    BATTLE_HIT_ATTR_18        = (1 << 18), // unused?
};

enum {
    BATTLE_HIT_INFO_BEGIN        = (1 << 0),
    BATTLE_HIT_INFO_FINISHES     = (1 << 1),
    BATTLE_HIT_INFO_KILLS_TARGET = (1 << 2),
    BATTLE_HIT_INFO_RETALIATION  = (1 << 3),
    BATTLE_HIT_INFO_END          = (1 << 4),
};

enum {
    // Special item slot identifiers understood by SetBattleUnitWeapon and other functions

    BU_ISLOT_AUTO           = -1,
    BU_ISLOT_5              = UNIT_ITEM_COUNT + 0,
    BU_ISLOT_ARENA_PLAYER   = UNIT_ITEM_COUNT + 1,
    BU_ISLOT_ARENA_OPPONENT = UNIT_ITEM_COUNT + 2,
    BU_ISLOT_BALLISTA       = UNIT_ITEM_COUNT + 3,
};

// Public objects

extern struct BattleStats gBattleStats;

extern struct BattleUnit gBattleActor;
extern struct BattleUnit gBattleTarget;

extern struct BattleHit gBattleHitArray[BATTLE_HIT_MAX];
extern struct BattleHit* gBattleHitIterator;

// Public functions

void BattleApplyGameStateUpdates(void);
void BattleGenerateSimulation(struct Unit* actor, struct Unit* target, int x, int y, int actorWpnSlot);
void BattleGenerateReal(struct Unit* actor, struct Unit* target);
void BattleGenerateBallistaSimulation(struct Unit* actor, struct Unit* target, int x, int y);
void BattleGenerateBallistaReal(struct Unit* actor, struct Unit* target);
void BattleGenerate(struct Unit* actor, struct Unit* target);
void BattleGenerateUiStats(struct Unit* unit, s8 itemSlot);
void InitBattleUnit(struct BattleUnit* bUnit, struct Unit* unit);
void InitBattleUnitWithoutBonuses(struct BattleUnit* bu, struct Unit* unit);
void SetBattleUnitTerrainBonuses(struct BattleUnit* bu, int terrain);
void SetBattleUnitTerrainBonusesAuto(struct BattleUnit* bu);
void SetBattleUnitWeapon(struct BattleUnit* bu, int itemSlot);
void SetBattleUnitWeaponBallista(struct BattleUnit* bu);
void sub_802A958(void); // unused?
void ClearBattleHits(void);
void BattleUnwind(void);
void BattleGetBattleUnitOrder(struct BattleUnit** outAttacker, struct BattleUnit** outDefender);
s8 BattleGetFollowUpOrder(struct BattleUnit** outAttacker, struct BattleUnit** outDefender);
void BattleApplyExpGains(void);
int GetAutoleveledStatIncrease(int growth, int levelCount);
s8 CanBattleUnitGainLevels(struct BattleUnit* bu);
void CheckBattleUnitLevelUp(struct BattleUnit* bu);
void ApplyUnitDefaultPromotion(struct Unit* unit);
void ApplyUnitPromotion(struct Unit* unit, u8 classId);
void GenerateBattleUnitStatGainsComparatively(struct BattleUnit* bu, struct Unit* unit);
void CheckBattleUnitStatCaps(struct Unit* unit, struct BattleUnit* bu);
void BattleApplyUnitUpdates(void);
s8 sub_802C0B0(void); // unused?
s8 HasBattleUnitGainedWeaponLevel(struct BattleUnit* bu);
void UpdateUnitFromBattle(struct Unit* unit, struct BattleUnit* bu);
void UpdateUnitDuringBattle(struct Unit* unit, struct BattleUnit* bu);
void BattleApplyBallistaUpdates(void);
void sub_802C334(void);
void BattleUnitTargetSetEquippedWeapon(struct BattleUnit* bu);
void BattleUnitTargetCheckCanCounter(struct BattleUnit* bu);
void BattleApplyWeaponTriangleEffect(struct BattleUnit* actor, struct BattleUnit* target);
void BattleInitTargetCanCounter(void);
void InitObstacleBattleUnit(void);
void UpdateObstacleFromBattle(struct BattleUnit* bu);
void BeginBattleAnimations(void);
int GetUnitSoloBattleAnimType(struct Unit* unit);
int GetBattleAnimType(void);
void BattleInitItemEffect(struct Unit* actor, int itemSlot);
void BattleInitItemEffectTarget(struct Unit* unit);
void BattleApplyMiscAction(struct Proc* proc);
void BattleApplyItemEffect(struct Proc* proc);
int GetOffensiveStaffAccuracy(struct Unit* actor, struct Unit* target);
void BattleGenerateArena(struct Unit* unit);
s8 BattleIsTriangleAttack(void);
s8 DidBattleUnitBreakWeapon(struct BattleUnit* bu);
void SetScriptedBattle(struct BattleHit* hits);
void UnitLevelUp(struct Unit* unit);
void BattleHitAdvance(void);
void BattleHitTerminate(void);

#define BUNIT_IS_OBSTACLE(aBu) (((aBu)->terrainId == TERRAIN_WALL_1B) || ((aBu)->terrainId == TERRAIN_SNAG))

#endif // GUARD_BMBATTLE_H
