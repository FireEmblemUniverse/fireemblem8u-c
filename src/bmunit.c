#include "global.h"

#include <string.h>

#include "constants/items.h"
#include "constants/classes.h"
#include "constants/characters.h"
#include "constants/terrains.h"

#include "bmitem.h"
#include "bmunit.h"
#include "bmmap.h"
#include "bmidoten.h"
#include "bmbattle.h"
#include "bmreliance.h"
#include "bmtrick.h"

EWRAM_DATA u8 gActiveUnitId = 0;
EWRAM_DATA struct Vec2 gActiveUnitMoveOrigin = {};

EWRAM_DATA struct Unit gUnitArrayBlue[62]  = {}; // Player units
EWRAM_DATA struct Unit gUnitArrayRed[50]   = {}; // Red units
EWRAM_DATA struct Unit gUnitArrayGreen[20] = {}; // Green units
EWRAM_DATA struct Unit gUnitArrayPurple[5] = {}; // Purple units

CONST_DATA static int sStatusNameTextIdLookup[] = {
    // TODO: TEXT ID DEFINITIONS

    [UNIT_STATUS_NONE]     = 0x536,
    [UNIT_STATUS_POISON]   = 0x514,
    [UNIT_STATUS_SLEEP]    = 0x515,
    [UNIT_STATUS_SILENCED] = 0x516,
    [UNIT_STATUS_BERSERK]  = 0x517,
    [UNIT_STATUS_ATTACK]   = 0x51B,
    [UNIT_STATUS_DEFENSE]  = 0x51C,
    [UNIT_STATUS_CRIT]     = 0x51D,
    [UNIT_STATUS_AVOID]    = 0x51E,
    [UNIT_STATUS_SICK]     = 0x518,
    [UNIT_STATUS_RECOVER]  = 0x519,
    [UNIT_STATUS_PETRIFY]  = 0x51A,
    [UNIT_STATUS_12]       = 0,
    [UNIT_STATUS_13]       = 0x51A,
};

struct Unit* CONST_DATA gUnitLookup[0x100] = { // unit lookup
    [FACTION_BLUE + 0x01] = gUnitArrayBlue + 0,
    [FACTION_BLUE + 0x02] = gUnitArrayBlue + 1,
    [FACTION_BLUE + 0x03] = gUnitArrayBlue + 2,
    [FACTION_BLUE + 0x04] = gUnitArrayBlue + 3,
    [FACTION_BLUE + 0x05] = gUnitArrayBlue + 4,
    [FACTION_BLUE + 0x06] = gUnitArrayBlue + 5,
    [FACTION_BLUE + 0x07] = gUnitArrayBlue + 6,
    [FACTION_BLUE + 0x08] = gUnitArrayBlue + 7,
    [FACTION_BLUE + 0x09] = gUnitArrayBlue + 8,
    [FACTION_BLUE + 0x0A] = gUnitArrayBlue + 9,
    [FACTION_BLUE + 0x0B] = gUnitArrayBlue + 10,
    [FACTION_BLUE + 0x0C] = gUnitArrayBlue + 11,
    [FACTION_BLUE + 0x0D] = gUnitArrayBlue + 12,
    [FACTION_BLUE + 0x0E] = gUnitArrayBlue + 13,
    [FACTION_BLUE + 0x0F] = gUnitArrayBlue + 14,
    [FACTION_BLUE + 0x10] = gUnitArrayBlue + 15,
    [FACTION_BLUE + 0x11] = gUnitArrayBlue + 16,
    [FACTION_BLUE + 0x12] = gUnitArrayBlue + 17,
    [FACTION_BLUE + 0x13] = gUnitArrayBlue + 18,
    [FACTION_BLUE + 0x14] = gUnitArrayBlue + 19,
    [FACTION_BLUE + 0x15] = gUnitArrayBlue + 20,
    [FACTION_BLUE + 0x16] = gUnitArrayBlue + 21,
    [FACTION_BLUE + 0x17] = gUnitArrayBlue + 22,
    [FACTION_BLUE + 0x18] = gUnitArrayBlue + 23,
    [FACTION_BLUE + 0x19] = gUnitArrayBlue + 24,
    [FACTION_BLUE + 0x1A] = gUnitArrayBlue + 25,
    [FACTION_BLUE + 0x1B] = gUnitArrayBlue + 26,
    [FACTION_BLUE + 0x1C] = gUnitArrayBlue + 27,
    [FACTION_BLUE + 0x1D] = gUnitArrayBlue + 28,
    [FACTION_BLUE + 0x1E] = gUnitArrayBlue + 29,
    [FACTION_BLUE + 0x1F] = gUnitArrayBlue + 30,
    [FACTION_BLUE + 0x20] = gUnitArrayBlue + 31,
    [FACTION_BLUE + 0x21] = gUnitArrayBlue + 32,
    [FACTION_BLUE + 0x22] = gUnitArrayBlue + 33,
    [FACTION_BLUE + 0x23] = gUnitArrayBlue + 34,
    [FACTION_BLUE + 0x24] = gUnitArrayBlue + 35,
    [FACTION_BLUE + 0x25] = gUnitArrayBlue + 36,
    [FACTION_BLUE + 0x26] = gUnitArrayBlue + 37,
    [FACTION_BLUE + 0x27] = gUnitArrayBlue + 38,
    [FACTION_BLUE + 0x28] = gUnitArrayBlue + 39,
    [FACTION_BLUE + 0x29] = gUnitArrayBlue + 40,
    [FACTION_BLUE + 0x2A] = gUnitArrayBlue + 41,
    [FACTION_BLUE + 0x2B] = gUnitArrayBlue + 42,
    [FACTION_BLUE + 0x2C] = gUnitArrayBlue + 43,
    [FACTION_BLUE + 0x2D] = gUnitArrayBlue + 44,
    [FACTION_BLUE + 0x2E] = gUnitArrayBlue + 45,
    [FACTION_BLUE + 0x2F] = gUnitArrayBlue + 46,
    [FACTION_BLUE + 0x30] = gUnitArrayBlue + 47,
    [FACTION_BLUE + 0x31] = gUnitArrayBlue + 48,
    [FACTION_BLUE + 0x32] = gUnitArrayBlue + 49,
    [FACTION_BLUE + 0x33] = gUnitArrayBlue + 50,
    [FACTION_BLUE + 0x34] = gUnitArrayBlue + 51,
    [FACTION_BLUE + 0x35] = gUnitArrayBlue + 52,
    [FACTION_BLUE + 0x36] = gUnitArrayBlue + 53,
    [FACTION_BLUE + 0x37] = gUnitArrayBlue + 54,
    [FACTION_BLUE + 0x38] = gUnitArrayBlue + 55,
    [FACTION_BLUE + 0x39] = gUnitArrayBlue + 56,
    [FACTION_BLUE + 0x3A] = gUnitArrayBlue + 57,
    [FACTION_BLUE + 0x3B] = gUnitArrayBlue + 58,
    [FACTION_BLUE + 0x3C] = gUnitArrayBlue + 59,
    [FACTION_BLUE + 0x3D] = gUnitArrayBlue + 60,
    [FACTION_BLUE + 0x3E] = gUnitArrayBlue + 61,

    [FACTION_RED + 0x01] = gUnitArrayRed + 0,
    [FACTION_RED + 0x02] = gUnitArrayRed + 1,
    [FACTION_RED + 0x03] = gUnitArrayRed + 2,
    [FACTION_RED + 0x04] = gUnitArrayRed + 3,
    [FACTION_RED + 0x05] = gUnitArrayRed + 4,
    [FACTION_RED + 0x06] = gUnitArrayRed + 5,
    [FACTION_RED + 0x07] = gUnitArrayRed + 6,
    [FACTION_RED + 0x08] = gUnitArrayRed + 7,
    [FACTION_RED + 0x09] = gUnitArrayRed + 8,
    [FACTION_RED + 0x0A] = gUnitArrayRed + 9,
    [FACTION_RED + 0x0B] = gUnitArrayRed + 10,
    [FACTION_RED + 0x0C] = gUnitArrayRed + 11,
    [FACTION_RED + 0x0D] = gUnitArrayRed + 12,
    [FACTION_RED + 0x0E] = gUnitArrayRed + 13,
    [FACTION_RED + 0x0F] = gUnitArrayRed + 14,
    [FACTION_RED + 0x10] = gUnitArrayRed + 15,
    [FACTION_RED + 0x11] = gUnitArrayRed + 16,
    [FACTION_RED + 0x12] = gUnitArrayRed + 17,
    [FACTION_RED + 0x13] = gUnitArrayRed + 18,
    [FACTION_RED + 0x14] = gUnitArrayRed + 19,
    [FACTION_RED + 0x15] = gUnitArrayRed + 20,
    [FACTION_RED + 0x16] = gUnitArrayRed + 21,
    [FACTION_RED + 0x17] = gUnitArrayRed + 22,
    [FACTION_RED + 0x18] = gUnitArrayRed + 23,
    [FACTION_RED + 0x19] = gUnitArrayRed + 24,
    [FACTION_RED + 0x1A] = gUnitArrayRed + 25,
    [FACTION_RED + 0x1B] = gUnitArrayRed + 26,
    [FACTION_RED + 0x1C] = gUnitArrayRed + 27,
    [FACTION_RED + 0x1D] = gUnitArrayRed + 28,
    [FACTION_RED + 0x1E] = gUnitArrayRed + 29,
    [FACTION_RED + 0x1F] = gUnitArrayRed + 30,
    [FACTION_RED + 0x20] = gUnitArrayRed + 31,
    [FACTION_RED + 0x21] = gUnitArrayRed + 32,
    [FACTION_RED + 0x22] = gUnitArrayRed + 33,
    [FACTION_RED + 0x23] = gUnitArrayRed + 34,
    [FACTION_RED + 0x24] = gUnitArrayRed + 35,
    [FACTION_RED + 0x25] = gUnitArrayRed + 36,
    [FACTION_RED + 0x26] = gUnitArrayRed + 37,
    [FACTION_RED + 0x27] = gUnitArrayRed + 38,
    [FACTION_RED + 0x28] = gUnitArrayRed + 39,
    [FACTION_RED + 0x29] = gUnitArrayRed + 40,
    [FACTION_RED + 0x2A] = gUnitArrayRed + 41,
    [FACTION_RED + 0x2B] = gUnitArrayRed + 42,
    [FACTION_RED + 0x2C] = gUnitArrayRed + 43,
    [FACTION_RED + 0x2D] = gUnitArrayRed + 44,
    [FACTION_RED + 0x2E] = gUnitArrayRed + 45,
    [FACTION_RED + 0x2F] = gUnitArrayRed + 46,
    [FACTION_RED + 0x30] = gUnitArrayRed + 47,
    [FACTION_RED + 0x31] = gUnitArrayRed + 48,
    [FACTION_RED + 0x32] = gUnitArrayRed + 49,

    [FACTION_GREEN + 0x01] = gUnitArrayGreen + 0,
    [FACTION_GREEN + 0x02] = gUnitArrayGreen + 1,
    [FACTION_GREEN + 0x03] = gUnitArrayGreen + 2,
    [FACTION_GREEN + 0x04] = gUnitArrayGreen + 3,
    [FACTION_GREEN + 0x05] = gUnitArrayGreen + 4,
    [FACTION_GREEN + 0x06] = gUnitArrayGreen + 5,
    [FACTION_GREEN + 0x07] = gUnitArrayGreen + 6,
    [FACTION_GREEN + 0x08] = gUnitArrayGreen + 7,
    [FACTION_GREEN + 0x09] = gUnitArrayGreen + 8,
    [FACTION_GREEN + 0x0A] = gUnitArrayGreen + 9,
    [FACTION_GREEN + 0x0B] = gUnitArrayGreen + 10,
    [FACTION_GREEN + 0x0C] = gUnitArrayGreen + 11,
    [FACTION_GREEN + 0x0D] = gUnitArrayGreen + 12,
    [FACTION_GREEN + 0x0E] = gUnitArrayGreen + 13,
    [FACTION_GREEN + 0x0F] = gUnitArrayGreen + 14,
    [FACTION_GREEN + 0x10] = gUnitArrayGreen + 15,
    [FACTION_GREEN + 0x11] = gUnitArrayGreen + 16,
    [FACTION_GREEN + 0x12] = gUnitArrayGreen + 17,
    [FACTION_GREEN + 0x13] = gUnitArrayGreen + 18,
    [FACTION_GREEN + 0x14] = gUnitArrayGreen + 19,

    [FACTION_PURPLE + 0x01] = gUnitArrayPurple + 0,
    [FACTION_PURPLE + 0x02] = gUnitArrayPurple + 1,
    [FACTION_PURPLE + 0x03] = gUnitArrayPurple + 2,
    [FACTION_PURPLE + 0x04] = gUnitArrayPurple + 3,
    [FACTION_PURPLE + 0x05] = gUnitArrayPurple + 4,
};

inline struct Unit* GetUnit(int id) {
    return gUnitLookup[id & 0xFF];
}

inline const struct ClassData* GetClassData(int classId) {
    if (classId < 1)
        return NULL;

    return gClassData + (classId - 1);
}

inline const struct CharacterData* GetCharacterData(int charId) {
    if (charId < 1)
        return NULL;

    return gCharacterData + (charId - 1);
}

void ClearUnits(void) {
    int i;

    for (i = 0; i < 0x100; ++i) {
        struct Unit* unit = GetUnit(i);

        if (unit) {
            ClearUnit(unit);
            unit->index = i;
        }
    }
}

void ClearUnit(struct Unit* unit) {
    u8 id = unit->index;
    CpuFill16(0, unit, sizeof(struct Unit));
    unit->index = id;
}

void CopyUnit(struct Unit* from, struct Unit* to) {
    u8 id = to->index;
    memcpy(to, from, sizeof(struct Unit));
    to->index = id;
}

struct Unit* GetFreeUnit(int faction) {
    int i, last = (faction + 0x40);

    for (i = faction + 1; i < last; ++i) {
        struct Unit* unit = GetUnit(i);

        if (unit->pCharacterData == NULL)
            return unit;
    }

    return NULL;
}

struct Unit* GetFreeBlueUnit(const struct UnitDefinition* uDef) {
    int i, last = 0x40;

    // This is ?? and is completely useless but it's required to produce matching asm
    if (uDef->charIndex == GetPlayerLeaderUnitId())
        ++i;

    for (i = 1; i < last; ++i) {
        struct Unit* unit = GetUnit(i);

        if (unit->pCharacterData == NULL)
            return unit;
    }

    return NULL;
}

inline int GetUnitMaxHp(struct Unit* unit) {
    return unit->maxHP + GetItemHpBonus(GetUnitEquippedWeapon(unit));
}

inline int GetUnitCurrentHp(struct Unit* unit) {
    if (unit->curHP > GetUnitMaxHp(unit))
        unit->curHP = GetUnitMaxHp(unit);

    return unit->curHP;
}

inline int GetUnitPower(struct Unit* unit) {
    return unit->pow + GetItemPowBonus(GetUnitEquippedWeapon(unit));
}

inline int GetUnitSkill(struct Unit* unit) {
    int item = GetUnitEquippedWeapon(unit);

    if (unit->state & US_RESCUING)
        return unit->skl / 2 + GetItemSklBonus(item);

    return unit->skl + GetItemSklBonus(item);
}

inline int GetUnitSpeed(struct Unit* unit) {
    int item = GetUnitEquippedWeapon(unit);

    if (unit->state & US_RESCUING)
        return unit->spd / 2 + GetItemSpdBonus(item);

    return unit->spd + GetItemSpdBonus(item);
}

inline int GetUnitDefense(struct Unit* unit) {
    return unit->def + GetItemDefBonus(GetUnitEquippedWeapon(unit));
}

inline int GetUnitResistance(struct Unit* unit) {
    return unit->res + GetItemResBonus(GetUnitEquippedWeapon(unit)) + unit->barrierDuration;
}

inline int GetUnitLuck(struct Unit* unit) {
    return unit->lck + GetItemLckBonus(GetUnitEquippedWeapon(unit));
}

inline int GetUnitPortraitId(struct Unit* unit) {
    if (unit->pCharacterData->portraitId) {
        // TODO: PORTRAIT_LYON?, CHAPTER definitions
        if (gUnknown_0202BCF0.chapterIndex == 0x22 && unit->pCharacterData->portraitId == 0x4A)
            return 0x46;

        return unit->pCharacterData->portraitId;
    }

    if (unit->pClassData->defaultPortraitId)
        return unit->pClassData->defaultPortraitId;

    return 0;
}

inline int GetUnitMiniPortraitId(struct Unit* unit) {
    if (unit->pCharacterData->miniPortrait)
        return 0x7F00 + unit->pCharacterData->miniPortrait;

    return GetUnitPortraitId(unit);
}

inline int GetUnitLeaderCharId(struct Unit* unit) {
    if (!(unit->index & 0xC0))
        return 0;

    return UNIT_LEADER_CHARACTER(unit);
}

inline void SetUnitLeaderCharId(struct Unit* unit, int charId) {
    UNIT_LEADER_CHARACTER(unit) = charId;
}

inline void SetUnitHp(struct Unit* unit, int value) {
    unit->curHP = value;

    if (unit->curHP > GetUnitMaxHp(unit))
        unit->curHP = GetUnitMaxHp(unit);
}

inline void AddUnitHp(struct Unit* unit, int amount) {
    int hp = unit->curHP;

    hp += amount;

    if (hp > GetUnitMaxHp(unit))
        hp = GetUnitMaxHp(unit);

    if (hp < 0)
        hp = 0;

    unit->curHP = hp;
}

int GetUnitFogViewRange(struct Unit* unit) {
    int result = gUnknown_0202BCF0.chapterVisionRange;

    if (UNIT_CATTRIBUTES(unit) & CA_THIEF)
        result += 5;

    return result + unit->torchDuration;
}

void SetUnitStatus(struct Unit* unit, int status) {
    if (status == 0) {
        unit->statusIndex    = 0;
        unit->statusDuration = 0;
    } else {
        unit->statusIndex    = status;
        unit->statusDuration = 5;
    }
}

void SetUnitStatusExt(struct Unit* unit, int status, int duration) {
    unit->statusIndex    = status;
    unit->statusDuration = duration;
}

inline char* GetUnitStatusName(struct Unit* unit) {
    return GetStringFromIndex(sStatusNameTextIdLookup[unit->statusIndex]);
}

int GetUnitSMSId(struct Unit* unit) {
    if (!(unit->state & US_IN_BALLISTA))
        return unit->pClassData->SMSId;

    switch (GetTrap(unit->ballistaIndex)->extra) {

        // TODO: SMS id definitions

    case ITEM_BALLISTA_REGULAR:
        return 0x58;

    case ITEM_BALLISTA_LONG:
        return 0x59;

    case ITEM_BALLISTA_KILLER:
        return 0x5A;

    default:
        return 0;

    } // switch (GetTrap(unit->ballistaIndex)->data[TRAP_EXTDATA_BLST_ITEMID])
}

s8 UnitAddItem(struct Unit* unit, int item) {
    int i;

    for (i = 0; i < UNIT_ITEM_COUNT; ++i) {
        if (unit->items[i] == 0) {
            unit->items[i] = item;
            return TRUE;
        }
    }

    return FALSE;
}

inline void UnitRemoveItem(struct Unit* unit, int slot) {
    unit->items[slot] = 0;
    UnitRemoveInvalidItems(unit);
}

void UnitClearInventory(struct Unit* unit) {
    int i;

    for (i = 0; i < UNIT_ITEM_COUNT; ++i)
        unit->items[i] = 0;
}

void UnitRemoveInvalidItems(struct Unit* unit) {
    u16 items[UNIT_ITEM_COUNT + 1], i;
    u16* it = items;

    // Build item buffer by iterating through unit's items and skipping blanks

    for (i = 0; i < UNIT_ITEM_COUNT; ++i) {
        if (unit->items[i])
            *it++ = unit->items[i];

        unit->items[i] = 0; // Null the item from the unit
    }

    *it = 0; // null-terminate buffer

    // Write buffered items

    for (i = 0; i < UNIT_ITEM_COUNT; ++i) {
        if (!items[i])
            return; // Stop now if we reached end of buffer

        unit->items[i] = items[i];
    }
}

int GetUnitItemCount(struct Unit* unit) {
    int i;

    for (i = (UNIT_ITEM_COUNT - 1); i >= 0; --i)
        if (unit->items[i])
            return i + 1;

    return 0;
}

s8 UnitHasItem(struct Unit* unit, int item) {
    int i;

    item = GetItemIndex(item);

    for (i = 0; (i < UNIT_ITEM_COUNT) && unit->items[i]; ++i)
        if (GetItemIndex(unit->items[i]) == item)
            return TRUE;

    return FALSE;
}

int LoadUnits(const struct UnitDefinition* uDef) {
    int count = 0;

    while (uDef->charIndex) {
        LoadUnit(uDef);

        uDef++;
        count++;
    }

    return count;
}

void sub_8017A54(struct Unit* unit) { // unused
    if (unit->pow >= 4)
        unit->pow /= 2;

    if (unit->def >= 4)
        unit->def /= 2;

    if (unit->res >= 4)
        unit->res /= 2;
}

s8 CanClassWieldWeaponType(u8 classId, u8 wpnType) {
    if (GetClassData(classId)->baseRanks[wpnType])
        return TRUE;
    else
        return FALSE;
}

struct Unit* LoadUnit(const struct UnitDefinition* uDef) {
    struct UnitDefinition buf;

    struct Unit* unit = NULL;

    if (uDef->genMonster) {
        u32 packedItems;
        u16 item1, item2;

        u16 monsterClass = GenerateMonsterClass(uDef->classIndex);

        buf = *uDef;

        buf.autolevel = TRUE;
        buf.classIndex = monsterClass;
        buf.level = GenerateMonsterLevel(uDef->level);

        packedItems = GenerateMonsterItems(monsterClass);

        // ew
        item1 = packedItems >> 16;
        item2 = packedItems & 0xFFFF;

        buf.items[0] = item1;
        buf.items[1] = item2;
        buf.items[2] = 0;
        buf.items[3] = 0;

        if ((GetItemWeaponEffect(item1) == 1) || !item2)
            buf.itemDrop = FALSE;
        else
            buf.itemDrop = TRUE;

        if (item1 == ITEM_MONSTER_SHADOWSHT || item1 == ITEM_MONSTER_STONE) {
            // Add another weapon item if weapon is either Shadowshot or Stone

            buf.items[2] = buf.items[1];

            switch (monsterClass) {

            case CLASS_MOGALL:
                buf.items[1] = ITEM_MONSTER_EVILEYE;
                break;

            case CLASS_ARCH_MOGALL:
                buf.items[1] = ITEM_MONSTER_CRIMSNEYE;
                break;

            case CLASS_GORGON:
                buf.items[1] = ITEM_MONSTER_DEMONSURG;

            } // switch (monsterClass)
        }

        if (CanClassWieldWeaponType(monsterClass, ITYPE_BOW) == TRUE) {
            // TODO: AI BIT DEFINITIONS
            buf.ai[2] = buf.ai[2] & (1 | 2 | 4);
            buf.ai[2] = buf.ai[2] | (8 | 32);
        }

        uDef = &buf;
    } // (uDef->genMonster)

    switch (uDef->allegiance) {

        // TODO: unit definition faction constants

    case 0:
        unit = GetFreeBlueUnit(uDef);
        break;

    case 2:
        unit = GetFreeUnit(FACTION_RED);
        break;

    case 1:
        unit = GetFreeUnit(FACTION_GREEN);
        break;

    } // switch (uDef->allegiance)

    if (!unit)
        return NULL;

    ClearUnit(unit);

    UnitInitFromDefinition(unit, uDef);
    UnitLoadStatsFromChracter(unit, unit->pCharacterData);
    UnitHideIfUnderRoof(unit);

    if (UNIT_IS_GORGON_EGG(unit))
        SetUnitStatus(unit, UNIT_STATUS_RECOVER);

    if (uDef->autolevel) {
        if (UNIT_FACTION(unit) == FACTION_BLUE) {
            UnitAutolevelRealistic(unit);
            UnitAutolevelWExp(unit, uDef);
        } else {
            if ((UNIT_CATTRIBUTES(unit) & CA_BOSS) || (UNIT_CHAR_ID(unit) < 0x40)) {
                struct Unit* unit2 = GetFreeUnit(0);

                CopyUnit(unit, unit2);

                unit2->exp = 0;
                UnitAutolevelRealistic(unit2);

                ClearUnit(unit);
                CopyUnit(unit2, unit);

                ClearUnit(unit2);

                unit->exp   = UNIT_EXP_DISABLED;
                unit->level = uDef->level;
            } else
                UnitAutolevel(unit);

            UnitAutolevelWExp(unit, uDef);
            SetUnitLeaderCharId(unit, uDef->leaderCharIndex);
        }

        if (UNIT_IS_GORGON_EGG(unit))
            unit->maxHP = (unit->level + 1) * 5;
    } // if (uDef->autolevel)

    FixROMUnitStructPtr(unit);
    UnitLoadSupports(unit);

    if (uDef->itemDrop)
        unit->state |= US_DROP_ITEM;

    UnitCheckStatCaps(unit);

    unit->curHP = GetUnitMaxHp(unit);

    if (UNIT_IS_GORGON_EGG(unit))
        SetUnitHp(unit, 5);

    return unit;
}

void UnitInitFromDefinition(struct Unit* unit, const struct UnitDefinition* uDef) {
    unit->pCharacterData = GetCharacterData(uDef->charIndex);

    if (uDef->classIndex)
        unit->pClassData = GetClassData(uDef->classIndex);
    else // such an overlooked feature
        unit->pClassData = GetClassData(unit->pCharacterData->defaultClass);

    unit->level = uDef->level;

    GetPreferredPositionForUNIT(uDef, &unit->xPos, &unit->yPos, FALSE);

    if (UNIT_IS_GORGON_EGG(unit)) {
        int i;

        // For gorgon eggs, set first item to zero
        // And store the other item ids in slots 1 through 4 for later initialization

        unit->items[0] = 0;

        for (i = 0; i < UNIT_DEFINITION_ITEM_COUNT; ++i)
            unit->items[i + 1] = uDef->items[i];
    } else {
        int i;

        for (i = 0; (i < UNIT_DEFINITION_ITEM_COUNT) && (uDef->items[i]); ++i)
            UnitAddItem(unit, MakeNewItem(uDef->items[i]));
    }

    CharStoreAI(unit, uDef);
}

void UnitLoadItemsFromDefinition(struct Unit* unit, const struct UnitDefinition* uDef) {
    int i;

    UnitClearInventory(unit);

    for (i = 0; (i < UNIT_DEFINITION_ITEM_COUNT) && (uDef->items[i]); ++i)
        UnitAddItem(unit, MakeNewItem(uDef->items[i]));
}

void UnitLoadStatsFromChracter(struct Unit* unit, const struct CharacterData* character) {
    int i;

    unit->maxHP = character->baseHP + unit->pClassData->baseHP;
    unit->pow   = character->basePow + unit->pClassData->basePow;
    unit->skl   = character->baseSkl + unit->pClassData->baseSkl;
    unit->spd   = character->baseSpd + unit->pClassData->baseSpd;
    unit->def   = character->baseDef + unit->pClassData->baseDef;
    unit->res   = character->baseRes + unit->pClassData->baseRes;
    unit->lck   = character->baseLck;

    unit->conBonus = 0;

    for (i = 0; i < 8; ++i) {
        unit->ranks[i] = unit->pClassData->baseRanks[i];

        if (unit->pCharacterData->baseRanks[i])
            unit->ranks[i] = unit->pCharacterData->baseRanks[i];
    }

    if (UNIT_FACTION(unit) == FACTION_BLUE && (unit->level != UNIT_LEVEL_MAX))
        unit->exp = 0;
    else
        unit->exp = UNIT_EXP_DISABLED;
}

void FixROMUnitStructPtr(struct Unit* unit) {
    // TODO: investigate why

    if (UNIT_CATTRIBUTES(unit) & CA_BIT_23)
        unit->pCharacterData = GetCharacterData(UNIT_CHAR_ID(unit) - 1);
}

void UnitLoadSupports(struct Unit* unit) {
    int i, count = GetUnitSupporterCount(unit);

    for (i = 0; i < count; ++i)
        unit->supports[i] = GetUnitSupporterInitialExp(unit, i);
}

void UnitAutolevelWExp(struct Unit* unit, const struct UnitDefinition* uDef) {
    if (uDef->autolevel) {
        int i;

        for (i = 0; i < GetUnitItemCount(unit); ++i) {
            int wType, item = unit->items[i];

            if (!(GetItemAttributes(item) & IA_REQUIRES_WEXP))
                continue;

            if (GetItemAttributes(item) & IA_WEAPON)
                if (CanUnitUseWeapon(unit, item))
                    continue;

            if (GetItemAttributes(item) & IA_STAFF)
                if (CanUnitUseStaff(unit, item))
                    continue;

            if (GetItemAttributes(item) & IA_LOCK_ANY)
                continue;

            wType = GetItemType(item);

            if (unit->ranks[wType] == 0)
                item = 0;

            unit->ranks[wType] = GetItemRequiredExp(item);
        }
    }
}

void UnitAutolevelCore(struct Unit* unit, u8 classId, int levelCount) {
    if (levelCount) {
        unit->maxHP += GetAutoleveledStatIncrease(unit->pClassData->growthHP,  levelCount);
        unit->pow   += GetAutoleveledStatIncrease(unit->pClassData->growthPow, levelCount);
        unit->skl   += GetAutoleveledStatIncrease(unit->pClassData->growthSkl, levelCount);
        unit->spd   += GetAutoleveledStatIncrease(unit->pClassData->growthSpd, levelCount);
        unit->def   += GetAutoleveledStatIncrease(unit->pClassData->growthDef, levelCount);
        unit->res   += GetAutoleveledStatIncrease(unit->pClassData->growthRes, levelCount);
        unit->lck   += GetAutoleveledStatIncrease(unit->pClassData->growthLck, levelCount);
    }
}

void UnitAutolevelPenalty(struct Unit* unit, u8 classId, int levelCount) {
    int level = unit->level;

    if (levelCount && level > unit->pCharacterData->baseLevel) {
        levelCount = level - levelCount;

        unit->maxHP = unit->pCharacterData->baseHP  + unit->pClassData->baseHP;
        unit->pow   = unit->pCharacterData->basePow + unit->pClassData->basePow;
        unit->skl   = unit->pCharacterData->baseSkl + unit->pClassData->baseSkl;
        unit->spd   = unit->pCharacterData->baseSpd + unit->pClassData->baseSpd;
        unit->def   = unit->pCharacterData->baseDef + unit->pClassData->baseDef;
        unit->res   = unit->pCharacterData->baseRes + unit->pClassData->baseRes;
        unit->lck   = unit->pCharacterData->baseLck;

        if (levelCount > unit->pCharacterData->baseLevel) {
            unit->level = levelCount;
            UnitAutolevel(unit);
            unit->level = level;
        }
    }
}

void UnitApplyBonusLevels(struct Unit* unit, int levelCount) {
    if (levelCount && !UNIT_IS_GORGON_EGG(unit)) {
        if (levelCount > 0)
            UnitAutolevelCore(unit, UNIT_CLASS_ID(unit), levelCount);
        else if (levelCount < 0)
            UnitAutolevelPenalty(unit, UNIT_CLASS_ID(unit), -levelCount);

        UnitCheckStatCaps(unit);

        unit->curHP = GetUnitMaxHp(unit);
    }
}

void UnitAutolevel(struct Unit* unit) {
    if (UNIT_CATTRIBUTES(unit) & CA_PROMOTED)
        UnitAutolevelCore(unit, unit->pClassData->promotion, GetCurrentPromotedLevelBonus());

    UnitAutolevelCore(unit, UNIT_CLASS_ID(unit), unit->level - 1);
}

void UnitAutolevelRealistic(struct Unit* unit) {
    struct BattleUnit tmpBattleUnit;
    short levelsLeft;

    tmpBattleUnit.expGain = 0;

    levelsLeft = (unit->level - unit->pCharacterData->baseLevel);

    if (levelsLeft) {
        for (unit->level -= levelsLeft; levelsLeft > 0; --levelsLeft) {
            InitBattleUnit(&tmpBattleUnit, unit);

            tmpBattleUnit.unit.exp += 100;
            CheckBattleUnitLevelUp(&tmpBattleUnit);

            UpdateUnitFromBattle(unit, &tmpBattleUnit);
        }
    }
}

void UnitCheckStatCaps(struct Unit* unit) {
    if (unit->maxHP > UNIT_MHP_MAX(unit))
        unit->maxHP = UNIT_MHP_MAX(unit);

    if (unit->pow > UNIT_POW_MAX(unit))
        unit->pow = UNIT_POW_MAX(unit);

    if (unit->skl > UNIT_SKL_MAX(unit))
        unit->skl = UNIT_SKL_MAX(unit);

    if (unit->spd > UNIT_SPD_MAX(unit))
        unit->spd = UNIT_SPD_MAX(unit);

    if (unit->def > UNIT_DEF_MAX(unit))
        unit->def = UNIT_DEF_MAX(unit);

    if (unit->res > UNIT_RES_MAX(unit))
        unit->res = UNIT_RES_MAX(unit);

    if (unit->lck > UNIT_LCK_MAX(unit))
        unit->lck = UNIT_LCK_MAX(unit);

    if (unit->conBonus > (UNIT_CON_MAX(unit) - UNIT_CON_BASE(unit)))
        unit->conBonus = (UNIT_CON_MAX(unit) - UNIT_CON_BASE(unit));

    if (unit->movBonus > (UNIT_MOV_MAX(unit) - UNIT_MOV_BASE(unit)))
        unit->movBonus = (UNIT_MOV_MAX(unit) - UNIT_MOV_BASE(unit));
}

struct Unit* GetUnitFromCharId(int charId) {
    int i;

    for (i = 1; i < 0x100; ++i) {
        struct Unit* unit = GetUnit(i);

        if (UNIT_IS_VALID(unit) && UNIT_CHAR_ID(unit) == charId)
            return unit;
    }

    return NULL;
}

struct Unit* GetUnitFromCharIdAndFaction(int charId, int faction) {
    int i, last = faction + 0x40;

    for (i = faction + 1; i < last; ++i) {
        struct Unit* unit = GetUnit(i);

        if (UNIT_IS_VALID(unit) && UNIT_CHAR_ID(unit) == charId)
            return unit;
    }

    return NULL;
}

s8 CanUnitRescue(struct Unit* actor, struct Unit* target) {
    int actorAid  = GetUnitAid(actor);
    int targetCon = UNIT_CON(target);

    return (actorAid >= targetCon) ? TRUE : FALSE;
}

void UnitRescue(struct Unit* actor, struct Unit* target) {
    actor->state  |= US_RESCUING;
    target->state |= US_RESCUED | US_HIDDEN;

    actor->rescueOtherUnit = target->index;
    target->rescueOtherUnit = actor->index;

    target->xPos = actor->xPos;
    target->yPos = actor->yPos;
}

void UnitDrop(struct Unit* actor, int xTarget, int yTarget) {
    struct Unit* target = GetUnit(actor->rescueOtherUnit);

    actor->state = actor->state &~ (US_RESCUING | US_RESCUED);
    target->state = target->state &~ (US_RESCUING | US_RESCUED | US_HIDDEN);

    if (UNIT_FACTION(target) == gUnknown_0202BCF0.chapterPhaseIndex)
        target->state |= US_UNSELECTABLE; // TODO: US_GRAYED

    actor->rescueOtherUnit = 0;
    target->rescueOtherUnit = 0;

    target->xPos = xTarget;
    target->yPos = yTarget;
}

s8 UnitGive(struct Unit* actor, struct Unit* target) {
    struct Unit* rescuee = GetUnit(actor->rescueOtherUnit);

    // no used be needed to match etc
    int couldGive = CanUnitRescue(target, rescuee);

    UnitDrop(actor, 0, 0);
    UnitRescue(target, rescuee);

    // return couldGive; // devs probably forgot to add this
}

inline char* GetUnitRescueName(struct Unit* unit) {
    if (!unit->rescueOtherUnit)
        return GetStringFromIndex(sStatusNameTextIdLookup[0]);

    return GetStringFromIndex(GetUnit(unit->rescueOtherUnit)->pCharacterData->nameTextId);
}

void UnitKill(struct Unit* unit) {
    if (UNIT_FACTION(unit) == FACTION_BLUE) {
        if (UNIT_IS_PHANTOM(unit))
            unit->pCharacterData = NULL;
        else {
            unit->state |= US_DEAD | US_HIDDEN;
            ClearUnitSupports(unit);
        }
    } else
        unit->pCharacterData = NULL;
}

void UnitChangeFaction(struct Unit* unit, int faction) {
    struct Unit* newUnit = GetFreeUnit(faction);

    if (gActiveUnit == unit)
        gActiveUnit = newUnit;

    CopyUnit(unit, newUnit);
    ClearUnit(unit);

    if (newUnit->exp == UNIT_EXP_DISABLED) {
        if ((faction == FACTION_BLUE) && (newUnit->level != UNIT_LEVEL_MAX))
            newUnit->exp = 0;
        else
            newUnit->exp = UNIT_EXP_DISABLED;
    }

    newUnit->state = newUnit->state &~ US_DROP_ITEM;

    if (newUnit->rescueOtherUnit)
        GetUnit(newUnit->rescueOtherUnit)->rescueOtherUnit = newUnit->index;
}

inline int CanUnitCrossTerrain(struct Unit* unit, int terrain) {
    const s8* lookup = GetUnitMovementCost(unit);
    return (lookup[terrain] > 0) ? TRUE : FALSE;
}

void UnitFinalizeMovement(struct Unit* unit) {
    if (unit->state & US_RESCUING) {
        struct Unit* rescuee = GetUnit(unit->rescueOtherUnit);

        rescuee->xPos = unit->xPos;
        rescuee->yPos = unit->yPos;
    }

    if (unit->state & US_IN_BALLISTA) {
        struct Trap* trap = GetTrap(unit->ballistaIndex);

        trap->xPos = unit->xPos;
        trap->yPos = unit->yPos;
    }
}

void UnitGetDeathDropLocation(struct Unit* unit, int* xOut, int* yOut) {
    int iy, ix, minDistance = 9999;
    struct Unit* rescuee = GetUnit(unit->rescueOtherUnit);

    // Fill the movement map
    GenerateExtendedMovementMap(unit->xPos, unit->yPos, gUnknown_0880BB96);

    // Put the active unit on the unit map (kinda, just marking its spot)
    gBmMapUnit[gActiveUnit->yPos][gActiveUnit->xPos] = 0xFF;

    // Remove the actor unit from the unit map (why?)
    gBmMapUnit[unit->yPos][unit->xPos] = 0;

    for (iy = gBmMapSize.y - 1; iy >= 0; --iy) {
        for (ix = gBmMapSize.x - 1; ix >= 0; --ix) {
            int distance;

            if (gBmMapMovement[iy][ix] > MAP_MOVEMENT_MAX)
                continue;

            if (gBmMapUnit[iy][ix] != 0)
                continue;

            if (gBmMapHidden[iy][ix] & HIDDEN_BIT_UNIT)
                continue;

            if (!CanUnitCrossTerrain(rescuee, gBmMapTerrain[iy][ix]))
                continue;

            distance = RECT_DISTANCE(ix, iy, unit->xPos, unit->yPos);

            if (minDistance >= distance) {
                minDistance = distance;

                *xOut = ix;
                *yOut = iy;
            }
        }
    }

    // Remove the active unit from the unit map again
    gBmMapUnit[gActiveUnit->yPos][gActiveUnit->xPos] = 0;
}

void UnitBeginAction(struct Unit* unit) {
    gActiveUnit = unit;
    gActiveUnitId = unit->index;

    gActiveUnitMoveOrigin.x = unit->xPos;
    gActiveUnitMoveOrigin.y = unit->yPos;

    gActionData.subjectIndex = unit->index;
    gActionData.unitActionType = 0;
    gActionData.moveCount = 0;

    gUnknown_0202BCB0.unk3D = 0;
    gUnknown_0202BCB0.unk3F = 0xFF;

    sub_802C334();

    gActiveUnit->state |= US_HIDDEN;
    gBmMapUnit[unit->yPos][unit->xPos] = 0;
}

void UnitBeginCantoAction(struct Unit* unit) {
    gActiveUnit = unit;
    gActiveUnitId = unit->index;

    gActiveUnitMoveOrigin.x = unit->xPos;
    gActiveUnitMoveOrigin.y = unit->yPos;

    gActionData.unitActionType = 0;

    gUnknown_0202BCB0.unk3D = 0;

    sub_802C334();

    gActiveUnit->state |= US_HIDDEN;
    gBmMapUnit[unit->yPos][unit->xPos] = 0;
}

void MoveActiveUnit(int x, int y) {
    gActiveUnit->xPos = x;
    gActiveUnit->yPos = y;

    gActiveUnit->state |= US_UNSELECTABLE;

    BWL_AddTilesMoved(UNIT_CHAR_ID(gActiveUnit), gActionData.moveCount);

    if (GetUnitCurrentHp(gActiveUnit) != 0)
        gActiveUnit->state = gActiveUnit->state &~ US_HIDDEN;

    UnitFinalizeMovement(gActiveUnit);
}

void ClearActiveFactionGrayedStates(void) {
    int i;

    if (gUnknown_0202BCF0.chapterPhaseIndex == FACTION_BLUE) {
        int i;

        for (i = 1; i < 0x40; ++i) {
            struct Unit* unit = GetUnit(i);

            if (!UNIT_IS_VALID(unit))
                continue;

            if (UNIT_CATTRIBUTES(unit) & CA_SUPPLY)
                continue;

            if (unit->state & (US_UNAVAILABLE | US_UNSELECTABLE))
                continue;

            StoreSomeUnitSetFlags(UNIT_CHAR_ID(unit));
        }
    }

    for (i = gUnknown_0202BCF0.chapterPhaseIndex + 1; i < gUnknown_0202BCF0.chapterPhaseIndex + 0x40; ++i) {
        struct Unit* unit = GetUnit(i);

        if (UNIT_IS_VALID(unit))
            unit->state = unit->state &~ (US_UNSELECTABLE | US_HAS_MOVED | US_HAS_MOVED_AI);
    }
}

void TickActiveFactionTurn(void) {
    int i, displayMapChange = FALSE;

    InitTargets(0, 0);

    for (i = gUnknown_0202BCF0.chapterPhaseIndex + 1; i < gUnknown_0202BCF0.chapterPhaseIndex + 0x40; ++i) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_UNAVAILABLE | US_RESCUED))
            continue;

        if (unit->barrierDuration != 0)
            unit->barrierDuration--;

        if (unit->torchDuration != 0) {
            unit->torchDuration--;
            displayMapChange = TRUE;
        }

        if (unit->statusDuration != 0) {
            if (unit->statusIndex != UNIT_STATUS_RECOVER)
                unit->statusDuration--;

            if (unit->statusDuration == 0)
                AddTarget(unit->xPos, unit->yPos, unit->index, 0);
        }
    }

    if (displayMapChange) {
        RenderBmMapOnBg2();
        RefreshEntityBmMaps();
        RenderBmMap();
        NewBMXFADE(TRUE);
        SMS_UpdateFromGameData();
    }
}

void SetAllUnitNotBackSprite(void) {
    int i;

    for (i = 1; i < 0xC0; ++i) {
        struct Unit* unit = GetUnit(i);

        if (UNIT_IS_VALID(unit))
            unit->state = unit->state &~ US_BIT8;
    }
}

void UnitUpdateUsedItem(struct Unit* unit, int itemSlot) {
    if (unit->items[itemSlot]) {
        unit->items[itemSlot] = GetItemAfterUse(unit->items[itemSlot]);
        UnitRemoveInvalidItems(unit);
    }
}

int GetUnitAid(struct Unit* unit) {
    if (!(UNIT_CATTRIBUTES(unit) & CA_MOUNTEDAID))
        return UNIT_CON(unit) - 1;

    if (UNIT_CATTRIBUTES(unit) & CA_FEMALE)
        return 20 - UNIT_CON(unit);
    else
        return 25 - UNIT_CON(unit);
}

int GetUnitMagBy2Range(struct Unit* unit) {
    if (UNIT_CHAR_ID(unit) == CHARACTER_FOMORTIIS) {
        return GetItemMaxRange(ITEM_NIGHTMARE);
    } else {
        int result = GetUnitPower(unit) / 2;

        if (result < 5)
            result = 5;

        return result;
    }
}

s8 UnitHasMagicRank(struct Unit* unit) {
    u8 combinedRanks = 0; 

    combinedRanks |= unit->ranks[ITYPE_STAFF];
    combinedRanks |= unit->ranks[ITYPE_ANIMA];
    combinedRanks |= unit->ranks[ITYPE_LIGHT];
    combinedRanks |= unit->ranks[ITYPE_DARK];

    return combinedRanks ? TRUE : FALSE;
}

void sub_8018A7C(struct Unit* unit, int x, int y) {
    if (!(unit->state & US_UNDER_A_ROOF)) {
        unit->state = unit->state &~ (US_HIDDEN | US_NOT_DEPLOYED);

        unit->xPos = x;
        unit->yPos = y;
    }
}

int GetUnitKeyItemSlotForTerrain(struct Unit* unit, int terrain) {
    int slot, item = 0;

    if (UNIT_CATTRIBUTES(unit) & CA_THIEF) {
        int slot = GetUnitItemSlot(unit, ITEM_LOCKPICK);

        if (slot >= 0)
            return slot;
    }

    switch (terrain) {

    case TERRAIN_CHEST_21:
        slot = GetUnitItemSlot(unit, ITEM_CHESTKEY);

        if (slot < 0)
            slot = GetUnitItemSlot(unit, ITEM_CHESTKEY_BUNDLE);

        return slot;

    case TERRAIN_DOOR:
        item = ITEM_DOORKEY;
        break;

    } // switch (terrain)

    return GetUnitItemSlot(unit, item);
}

int GetUnitAidIconId(u32 attributes) {
    // TODO: use icon id constants

    if (attributes & CA_MOUNTED)
        return 0x81;

    if (attributes & CA_PEGASUS)
        return 0x82;

    if (attributes & CA_WYVERN)
        return 0x83;

    return (-1);
}

int GetUnitWeaponUsabilityBits(struct Unit* unit) {
    int i, item, result = 0;

    for (i = 0; (i < UNIT_ITEM_COUNT) && (item = unit->items[i]); ++i) {
        if ((GetItemAttributes(item) & IA_WEAPON) && CanUnitUseWeapon(unit, item))
            result |= UNIT_USEBIT_WEAPON;

        if ((GetItemAttributes(item) & IA_STAFF) && CanUnitUseStaff(unit, item))
            result |= UNIT_USEBIT_STAFF;
    }

    return result;
}

int GetCombinedEnemyWeaponUsabilityBits(void) {
    int i, result = 0;

    for (i = 0x81; i < 0xC0; ++i) {
        struct Unit* unit = GetUnit(i);

        if (UNIT_IS_VALID(unit))
            result |= GetUnitWeaponUsabilityBits(unit);
    }

    return result;
}

s8 CanUnitMove(void) {
    s8 adjLookup[4 * 2] = {
        -1, 0,
        0, -1,
        +1, 0,
        0, +1,
    };

    int move = UNIT_MOV(gActiveUnit) - gActionData.moveCount;

    int xUnit = gActiveUnit->xPos;
    int yUnit = gActiveUnit->yPos;

    int i;

    for (i = 0; i < 4; ++i) {
        int xLocal = xUnit + adjLookup[i*2 + 0];
        int yLocal = yUnit + adjLookup[i*2 + 1];

        int cost;

        if (gBmMapUnit[yLocal][xLocal] & FACTION_RED)
            continue;

        cost = GetUnitMovementCost(gActiveUnit)[gBmMapTerrain[yLocal][xLocal]];

        if ((cost < 0) || (cost > move))
            continue;

        return TRUE;
    }

    return FALSE;
}

s8 IsPositionMagicSealed(int x, int y) {
    int i;

    for (i = 0x81; i < 0xC0; ++i) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (!(UNIT_CATTRIBUTES(unit) & CA_MAGICSEAL))
            continue;

        if (RECT_DISTANCE(unit->xPos, unit->yPos, x, y) <= 10)
            return TRUE;
    }

    return FALSE;
}

s8 IsUnitMagicSealed(struct Unit* unit) {
    if (unit->statusIndex == UNIT_STATUS_SILENCED)
        return TRUE;

    if (IsPositionMagicSealed(unit->xPos, unit->yPos))
        return TRUE;

    return FALSE;
}

int GetUnitLastItem(struct Unit* unit) {
    return unit->items[GetUnitItemCount(unit) - 1];
}

const s8* GetUnitMovementCost(struct Unit* unit) {
    if (unit->state & US_IN_BALLISTA)
        return gUnknown_0880BC18;

    switch (gUnknown_0202BCF0.chapterWeatherId) {

    case WEATHER_RAIN:
        return unit->pClassData->pMovCostTable[1];

    case WEATHER_SNOW:
    case WEATHER_SNOWSTORM:
        return unit->pClassData->pMovCostTable[2];

    default:
        return unit->pClassData->pMovCostTable[0];

    } // switch (gUnknown_0202BCF0.chapterWeatherId)
}

int GetClassSMSId(int classId) {
    return GetClassData(classId)->SMSId;
}

void UpdatePrevDeployStates(void) {
    int i;

    for (i = 1; i < 0x40; ++i) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & US_NOT_DEPLOYED)
            unit->state = unit->state | US_BIT21;
        else
            unit->state = unit->state &~ US_BIT21;

        if (unit->state & US_BIT16)
            unit->state = unit->state | US_BIT26;
        else
            unit->state = unit->state &~ US_BIT26;
    }

    if (gUnknown_0202BCF0.chapterStateBits & CHAPTER_FLAG_PREPSCREEN)
        StoreUnitWordStructs();

    ResetAllPlayerUnitState();
}

void LoadUnitPrepScreenPositions(void) {
    int i;

    ResetAllPlayerUnitState();
    ClearCutsceneUnits();

    for (i = 1; i < 0x40; ++i) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & US_BIT21)
            unit->state = unit->state | US_NOT_DEPLOYED;
        else
            unit->state = unit->state &~ US_NOT_DEPLOYED;

        if (unit->state & US_BIT26)
            unit->state = unit->state | US_BIT16;
        else
            unit->state = unit->state &~ US_BIT16;

        unit->state |= US_HIDDEN;
    }

    if (gUnknown_0202BCF0.chapterStateBits & CHAPTER_FLAG_PREPSCREEN)
        LoadUnitWordStructs();
}

void ClearTemporaryUnits(void) {
    int i;

    // player units
    for (i = 1; i < 0x40; ++i) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        unit->state |= US_HIDDEN;

        if (UNIT_IS_PHANTOM(unit))
            ClearUnit(unit);
    }

    // red units
    for (i = 0x81; i < 0xC0; ++i) {
        struct Unit* unit = GetUnit(i);

        if (UNIT_IS_VALID(unit))
            ClearUnit(unit);
    }

    // green units
    for (i = 0x41; i < 0x80; ++i) {
        struct Unit* unit = GetUnit(i);

        if (UNIT_IS_VALID(unit))
            ClearUnit(unit);
    }

    RefreshEntityBmMaps();
    SMS_UpdateFromGameData();
}

s8 IsUnitSlotAvailable(int faction) {
    int i;

    for (i = faction + 1; i < faction + 0x40; ++i)
        if (GetUnit(i)->pCharacterData == NULL)
            return TRUE;

    return FALSE;
}

void sub_8018F80(void) {
    int i;

    for (i = 1; i < 0x40; ++i) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & US_DEAD)
            continue;

        unit->state |= US_HIDDEN;
    }
}

void sub_8018FC0(void) {
    int i;

    for (i = 0x41; i < 0xC0; ++i) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        ClearUnit(unit);
    }
}

u16 CountAvailableBlueUnits(void) {
    int i;

    u16 result = 0;

    for (i = 1; i < 0x40; ++i) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & US_UNAVAILABLE)
            continue;

        ++result;
    }

    return result;
}

int CountRedUnits(void) {
    int i;

    u16 result = 0;

    for (i = 0x81; i < 0xC0; ++i) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & US_DEAD)
            continue;

        ++result;
    }

    return result;
}

int CountGreenUnits(void) {
    int i;

    u16 result = 0;

    for (i = 0x41; i < 0x80; ++i) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & US_DEAD)
            continue;

        ++result;
    }

    return result;
}

void ClearCutsceneUnits(void) {
    int i;

    for (i = 1; i < 0x40; ++i) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & US_BIT22) {
            if (unit->state & US_DEAD)
                unit->state = unit->state &~ US_BIT22;
            else
                ClearUnit(unit);
        }
    }
}

void sub_8019108(void) {
    int i;

    for (i = 1; i < 0x40; ++i) {
        struct Unit* unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        unit->state = unit->state &~ (US_UNSELECTABLE | US_RESCUING | US_RESCUED);
        unit->rescueOtherUnit = 0;

        SetUnitStatus(unit, 0);
    }

    RefreshEntityBmMaps();
    SMS_UpdateFromGameData();
}
