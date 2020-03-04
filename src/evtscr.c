#include "global.h"
#include "proc.h"
#include "event.h"

#include "rng.h"
#include "hardware.h"
#include "m4a.h"
#include "soundwrapper.h"
#include "bmio.h"
#include "bmunit.h"
#include "bmmap.h"
#include "bmitem.h"
#include "bmtrick.h"
#include "bmidoten.h"

// temp rodata (TODO: move directly into the various functions that use those)
const u8 gUnknown_080D793C[3] = { 0x00, 0x40, 0x80 };

struct Struct03000430 {
	u8 unk00;
	u8 unk01;
	u8 unk02;
	u8 unk03;
};

struct Struct03000428 {
	struct Struct03000430 sub;
	u8 unk04;
	u8 unk05;
};

void Font_LoadForUI(void);
void sub_80156D4(void);
void sub_800BA34(void);

void AllocWeatherParticles(unsigned);
void RenderBmMap(void);
void SMS_UpdateFromGameData(void);
void SMS_FlushIndirect(void);

void RestartBattleMap(void);

int sub_8015A40(int);
int sub_8015A6C(int);

struct Unit* GetUnitStructFromEventParameter(s16 param);

void SetSomeRealCamPos(s8 x, s8 y, s8 thisNeedsToBeAS8);
void SetCursorMapPosition(int x, int y);

u8 EnsureCameraOntoPosition(struct Proc*, int x, int y);
void sub_8015D84(struct Proc*, s8 x, s8 y);

int GetMapChangeIdAt(int, int);

void TriggerMapChanges(u16 mapChangeId, u8 displayFlag, struct Proc* parent);
void sub_800BB48(u16 mapChangeId, u8 displayFlag, struct Proc* parent);

extern struct Unit* gActiveUnit; // TODO: gActiveUnit

// void SetWeather(u16 id);

void SetNextChapterId(int chIndex);
void SetNextGameActionId(int);

void GotoChapterWithoutSave(u16 chIndex);

void HandleAllegianceChange(struct Unit*, u8 allegiance);
struct Unit* LoadUnit(const struct UnitDefinition* def);
void GetPreferredPositionForUNIT(const struct UnitDefinition* uDef, u8* xOut, u8* yOut, s8 findNearest);

#include "chapterdata.h"

void sub_80180CC(struct Unit*, int amount); // aka ApplyBonusLevels
void sub_800F8A8(struct Unit*, const struct UnitDefinition*, u16, s8);

unsigned GetSomeEventEngineMoveRelatedBitfield(struct EventEngineProc* proc, s8);

void sub_800F698(const struct UnitDefinition* def, s16 count, u8 param);
u8 TryPrepareEventUnitMovement(struct EventEngineProc* proc, int x, int y);
void LoadUnit_800F704(const struct UnitDefinition* def, u16 b, s8 quiet, s8 d);

struct Unit* GetUnitFromCharId(int);
int IsSethLArachelMyrrhInnes(int);
s8 IsUnitSlotAvailable(int faction);

void MoveUnit_(struct Unit* unit, s8 x, s8 y, u16 flags);
void sub_8079FA8(struct Unit* unit, const void* redas, unsigned count, u16 flags);
void sub_8079D74(struct Unit* unit, const void* redas, unsigned count, u16 flags);

// TODO: #include "evtcmd_gmap.h" (?)
void sub_800B910(int, int, int);
void sub_800B954(int, int, int);
void sub_800B994(int, int, int);
void sub_800B9B8(int, int);
void sub_800BA04(int, int);

// TODO: #include "hino.h"
void sub_8013D08(int speed, struct Proc* parent); // aka StartFadeInBlack
void sub_8013D20(int speed, struct Proc* parent); // aka StartFadeOutBlack
void sub_8013D38(int speed, struct Proc* parent); // aka StartFadeInWhite
void sub_8013D50(int speed, struct Proc* parent); // aka StartFadeOutWhite
void sub_8012890(int, int, int, int, int, struct Proc*); // aka idk

// TODO: #include "face.h"
void ResetFaces(void);
u8   Face6CExists(void);
void sub_8005F38(struct Proc*);
void sub_80067E8(void);
void sub_800680C(int, int, int);
void sub_8006A30(int, int, u16);
void sub_8006A70(int, int, struct Struct03000430*);
void sub_8006AA8(int); // set dialog flag
u8   sub_8006ED8(void);
void sub_8006EF0(void);
void sub_80081A8(void);
u8   sub_80089D0(void);
int  sub_8008A00(void);

extern const struct ProcCmd gUnknown_08591154[]; // E_FACE proc

void RefreshBMapGraphics(void); // ReloadGameCoreGraphics

void BMapDispSuspend(void);
void BMapDispResume(void);

void MU_AllDisable(void);
void MU_AllEnable(void);

// ???
void sub_808A518(u16);
void sub_808AA04(int, int, u16, struct Proc*);
void sub_808AADC(const char*, int*, int*);
void sub_808E9D8(int);
void sub_808F128(int, int, int, int, int, void*, int, struct Proc*);
u8   sub_808F284(void);

extern const struct ProcCmd gUnknown_08A016E0[];

// ????

void sub_80B272C(int, int, int, struct Proc*);
void sub_80B2780(int, int, int, struct Proc*);

void sub_80B65F8(u16* buf, unsigned offset, int, int, unsigned);

// local

struct ConvoBackgroundFadeProc {
	PROC_HEADER;

	/* 29 */ u8  fadeType;
	/* 2A */ u8  unkType;

	/* 2C */ u16 bgIndex;
	/* 2E */ u16 fadeSpeed;
	/* 30 */ u16 fadeTimer;

	/* 34 */ struct EventEngineProc* pEventEngine;
};

void sub_800E640(struct EventEngineProc*);
u8 Event22_(struct EventEngineProc*); // CLEAN
u8 Event23_(struct EventEngineProc*); // Lock Game Graphics
u8 Event24_(struct EventEngineProc*); // Resume Game Graphics

extern const struct ProcCmd gUnknown_08591DE8[]; // "face witness"

extern struct Struct03000428 gUnknown_03000428;
extern struct Struct03000430 gUnknown_03000430;

extern const struct ProcCmd gUnknown_08591E58[];
extern const struct ProcCmd gUnknown_08591E00[];
extern const struct ProcCmd gUnknown_08591EB0[];

// other

void CopyDataWithPossibleUncomp(const void* source, void* target); // aka Decompress
void CallARM_FillTileRect(u16* target, const void* source, u16 tilebase); // aka BgTileMap_ApplyTSA

extern const struct {
	const void* pTileGraphics;
	const void* pTileArrangement;
	const void* pColorPalettes;
} gUnknown_0895DD1C[]; // TODO: rename to gConvoBackgroundData

// TODO: not use this?
struct EventCommandHeader {
	u16 subcommand : 4;
	u16 size       : 4;
	u16 commandId  : 8;
} __attribute__((packed));

struct EventCallReturnData {
	const u16* pEventStart;
	const u16* pEventCurrent;
};

// void SetEventId(int);
// void UnsetEventId(int);

extern struct EventCallReturnData gUnknown_03000570[];
extern u16 gUnknown_08592114[]; // gEvent_PostEnd

void sub_80125C0(struct UnitDefinition*);

extern struct UnitDefinition end[];

const struct UnitDefinition* sub_80833B0(void); // GetChapterEnemyUnitDefinitions

void ClearMapWith(u8** map, u8 value);

extern const struct ProcCmd gUnknown_0859A548[];

u8 MU_CanStart(void);

u8 Event00_NULL(struct EventEngineProc* proc) {
	return EVC_ADVANCE_CONTINUE;
}

u8 Event01_End(struct EventEngineProc* proc) {
	s8 i;

	if (proc->evStateBits & EV_STATE_ABORT)
		return EVC_END;
	
	if (((struct EventCommandHeader*)(proc->pEventCurrent))->subcommand == 1) {
		for (i = 0; i < 8; ++i) {
			gUnknown_03000570[i].pEventStart   = NULL;
			gUnknown_03000570[i].pEventCurrent = NULL;
		}
	}

	if (gUnknown_03000570->pEventStart) {
		proc->pEventIdk     = gUnknown_03000570[0].pEventStart;
		proc->pEventCurrent = gUnknown_03000570[0].pEventCurrent;

		for (i = 0; i < 7; ++i) {
			gUnknown_03000570[i].pEventStart   = gUnknown_03000570[i+1].pEventStart;
			gUnknown_03000570[i].pEventCurrent = gUnknown_03000570[i+1].pEventCurrent;
		}
		
		gUnknown_03000570[i].pEventStart   = NULL;
		gUnknown_03000570[i].pEventCurrent = NULL;

		return EVC_ADVANCE_CONTINUE;
	}

	switch (proc->execType) {

	case EV_EXEC_WORLDMAP:
		proc->execType = EV_EXEC_UNK4;
		break;
	
	case EV_EXEC_CUTSCENE:
		proc->evStateBits = (proc->evStateBits &~ EV_STATE_SKIPPING) | EV_STATE_NOSKIP;
		proc->execType = EV_EXEC_UNK5;

		proc->pEventIdk     = gUnknown_08592114;
		proc->pEventCurrent = gUnknown_08592114;

		return EVC_STOP_CONTINUE;

	}

	return EVC_END;
}

u8 Event02_EvBitAndIdMod(struct EventEngineProc* proc) {
	u8 scType, scValue;
	short index;

	scType  = (proc->pEventCurrent[0] & 0x7);
	scValue = (proc->pEventCurrent[0] & 0xF) >> 3;
	index   = proc->pEventCurrent[1];
	
	if (index < 0)
		index = gEventSlots[2];

	switch (scType) {

	case 0: // Sets/Unsets event bit
		if (scValue == 0)
			proc->evStateBits &= ~(1 << index);
		else
			proc->evStateBits |= (1 << index);
		
		break;

	case 1: // Sets/Unsets trigger
		if (scValue == 0)
			UnsetEventId(index);
		else
			SetEventId(index);

		break;

	}

	return EVC_ADVANCE_CONTINUE;
}

u8 Event03_CheckEvBitOrId(struct EventEngineProc* proc) {
	u8 sc;
	short index;

	sc    = (*(const u8*)(proc->pEventCurrent)) & 0xF;
	index = proc->pEventCurrent[1];

	if (index < 0)
		index = gEventSlots[2];
	
	switch (sc) {

	case 0:
		if (proc->evStateBits & (1 << index))
			gEventSlots[0xC] = 1;
		else
			gEventSlots[0xC] = 0;

		break;
	
	case 1:
		if (!CheckEventId(index))
			gEventSlots[0xC] = 0;
		else
			gEventSlots[0xC] = 1;

		break;

	}

	return EVC_ADVANCE_CONTINUE;
}

u8 Event04_CheckRandom(struct EventEngineProc* proc) {
	unsigned highBound;
	
	highBound = *(const u16*)(proc->pEventCurrent + 1);

	if (highBound == 0)
		gEventSlots[0xC] = 0;
	else
		gEventSlots[0xC] = NextRN_N(highBound+1);
	
	return EVC_ADVANCE_CONTINUE;
}

u8 Event05_SetSlot(struct EventEngineProc* proc) {
	unsigned slotIndex, slotValue;

	slotIndex = *(const u16*)(proc->pEventCurrent + 1);
	slotValue = *(const u32*)(proc->pEventCurrent + 2);

	gEventSlots[slotIndex] = slotValue;

	return EVC_ADVANCE_CONTINUE;
}

u8 Event06_SlotOperation(struct EventEngineProc* proc) {
	u8 sc, slotSrc1, slotSrc2, slotDest;

	sc       = (*(const u8*)(proc->pEventCurrent)) & 0xF;

	slotDest = (*(proc->pEventCurrent + 1) >> 0)   & 0xF;
	slotSrc1 = (*(proc->pEventCurrent + 1) >> 4)   & 0xF;
	slotSrc2 = (*(proc->pEventCurrent + 1) >> 8)   & 0xF;

	switch (sc) {
	
	case 0: // add
		gEventSlots[slotDest] = gEventSlots[slotSrc1] + gEventSlots[slotSrc2];
		return EVC_ADVANCE_CONTINUE;
	
	case 1: // sub
		gEventSlots[slotDest] = gEventSlots[slotSrc1] - gEventSlots[slotSrc2];
		return EVC_ADVANCE_CONTINUE;
	
	case 2: // mul
		gEventSlots[slotDest] = gEventSlots[slotSrc1] * gEventSlots[slotSrc2];
		return EVC_ADVANCE_CONTINUE;
	
	case 3: // div
		gEventSlots[slotDest] = gEventSlots[slotSrc1] / gEventSlots[slotSrc2];
		return EVC_ADVANCE_CONTINUE;
	
	case 4: // mod
		gEventSlots[slotDest] = gEventSlots[slotSrc1] % gEventSlots[slotSrc2];
		return EVC_ADVANCE_CONTINUE;
	
	case 5: // and
		gEventSlots[slotDest] = gEventSlots[slotSrc1] & gEventSlots[slotSrc2];
		return EVC_ADVANCE_CONTINUE;
	
	case 6: // orr
		gEventSlots[slotDest] = gEventSlots[slotSrc1] | gEventSlots[slotSrc2];
		return EVC_ADVANCE_CONTINUE;
	
	case 7: // xor
		gEventSlots[slotDest] = gEventSlots[slotSrc1] ^ gEventSlots[slotSrc2];
		return EVC_ADVANCE_CONTINUE;
	
	case 8: // lsl
		gEventSlots[slotDest] = gEventSlots[slotSrc1] << gEventSlots[slotSrc2];
		return EVC_ADVANCE_CONTINUE;
	
	case 9: // lsr
		gEventSlots[slotDest] = gEventSlots[slotSrc1] >> gEventSlots[slotSrc2];
		return EVC_ADVANCE_CONTINUE;
	
	default:
		return EVC_ERROR;

	}
}

u8 Event07_SlotQueueOperations(struct EventEngineProc* proc) {
	u8 sc;
	s16 index;

	sc = (*(const u8*)(proc->pEventCurrent)) & 0xF;

	switch (sc) {
	
	case 0: // Push Slot Indexed by Argument
		index = proc->pEventCurrent[1];
		SlotQueuePush(gEventSlots[index]);
		break;
	
	case 1: // Push Slot 1
		SlotQueuePush(gEventSlots[1]);
		break;
	
	case 2: // Pop in Slot Indexed by Argument
		index = proc->pEventCurrent[1];
		gEventSlots[index] = SlotQueuePop();
		break;

	}

	return EVC_ADVANCE_CONTINUE;
}

u8 Event08_Label(struct EventEngineProc* proc) {
	return EVC_ADVANCE_CONTINUE;
}

u8 Event09_Goto(struct EventEngineProc* proc) {
	u32 target;

	// 0x0820 is Label code (0x08) with size 2 and sc 0
	target = (0x0820) | (proc->pEventCurrent[1] << 16);

	// Start label search at current script start
	proc->pEventCurrent = proc->pEventIdk;

	// Compares with 32bit value for some reason
	while (*(const u32*)(proc->pEventCurrent) != target)
		proc->pEventCurrent += (proc->pEventCurrent[0] >> 4 & 0xF);

	return EVC_ADVANCE_CONTINUE;
}

u8 Event0A_Call(struct EventEngineProc* proc) {
	intptr_t target;
	s8 i;

	target = proc->pEventCurrent[2] | (proc->pEventCurrent[3] << 16);

	if (target < 0)
		target = gEventSlots[2];

	// TODO: replace 7 by ARRAY_SIZE(gUnknown_03000570)-1 ?
	for (i = 7; i > 0; --i) {
		gUnknown_03000570[i].pEventStart   = gUnknown_03000570[i-1].pEventStart;
		gUnknown_03000570[i].pEventCurrent = gUnknown_03000570[i-1].pEventCurrent;
	}

	gUnknown_03000570[0].pEventStart   = proc->pEventIdk;
	gUnknown_03000570[0].pEventCurrent = proc->pEventCurrent;

	proc->pEventIdk     = (const u16*)(target);
	proc->pEventCurrent = (const u16*)(target);

	return EVC_UNK4;
}

u8 Event0B_(struct EventEngineProc* proc) {
	u8 sc;
	unsigned execType;
	intptr_t target;

	sc       = (*(const u8*)(proc->pEventCurrent)) & 0xF;

	execType = proc->pEventCurrent[1];
	target   = (*(const u32*)(proc->pEventCurrent + 2));

	if (target < 0)
		target = gEventSlots[2];

	if (target) {
		switch (sc) {

		case 0:
			CallEvent((const u16*)(target), proc->execType);
			break;

		case 1:
			sub_8083DD8((const u16*)(target), execType);
			break;

		}
	}

	return EVC_ADVANCE_CONTINUE;
}

u8 Event0C_Branch(struct EventEngineProc* proc) {
	u8 sc;
	int cmp1, cmp2;

	sc = (*(const u8*)(proc->pEventCurrent)) & 0xF;

	cmp1 = *(proc->pEventCurrent + 2);
	cmp2 = *(proc->pEventCurrent + 3);

	cmp1 = gEventSlots[cmp1];
	cmp2 = gEventSlots[cmp2];

	switch (sc) {
	
	case 0: // beq
		if (cmp1 == cmp2)
			return Event09_Goto(proc);
		return EVC_ADVANCE_CONTINUE;
	
	case 1: // bne
		if (cmp1 != cmp2)
			return Event09_Goto(proc);
		return EVC_ADVANCE_CONTINUE;
	
	case 2: // bge
		if (cmp1 >= cmp2)
			return Event09_Goto(proc);
		return EVC_ADVANCE_CONTINUE;
	
	case 3: // bgt
		if (cmp1 > cmp2)
			return Event09_Goto(proc);
		return EVC_ADVANCE_CONTINUE;
	
	case 4: // ble
		if (cmp1 <= cmp2)
			return Event09_Goto(proc);
		return EVC_ADVANCE_CONTINUE;

	case 5: // blt
		if (cmp1 < cmp2)
			return Event09_Goto(proc);
		return EVC_ADVANCE_CONTINUE;

	}

	return EVC_ERROR;
}

u8 Event0D_AsmCall(struct EventEngineProc* proc) {
	typedef void(*ASMCFuncType)(struct EventEngineProc*);

	u8 subcode = *(const u8*)(proc->pEventCurrent) & 0xF;
	
	ASMCFuncType func = (ASMCFuncType)(
		(proc->pEventCurrent[2]) |
		(proc->pEventCurrent[3] << 0x10)
	);

	if (((subcode & 0xF) == 1) && ((proc->evStateBits >> 2) & 1))
		return EVC_ADVANCE_CONTINUE;

	(*func)(proc);

	return EVC_ADVANCE_YIELD;
}

u8 Event0E_STAL(struct EventEngineProc* proc) {
	u8 subcode;
	short stallTimer;

	subcode = *(const u8*)(proc->pEventCurrent) & 0xF;

	if (((proc->evStateBits >> 0x2) & 1)) {
		proc->evStallTimer = 0;
		return EVC_ADVANCE_CONTINUE;
	}
	
	if (((subcode & 1) && (((proc->evStateBits >> 0x3) & 1) || (gKeyStatusPtr->newKeys & B_BUTTON))))
		goto end_stall; // I can't get it to match without that (ugh)
	
	stallTimer = proc->evStallTimer;
	
	if (stallTimer <= 0) {
		proc->evStallTimer = proc->pEventCurrent[1];
	} else {
		int timeStep = 1;

		if ((!(proc->evStateBits & EV_STATE_0040)) && (subcode & 2))
			if ((gUnknown_0202BCF0.unk40_8) || ((A_BUTTON & gKeyStatusPtr->heldKeys)))
				timeStep = 4;
		
		stallTimer = stallTimer - timeStep;

		if (stallTimer <= 0) {

		end_stall:
			proc->evStallTimer = 0;
			return EVC_ADVANCE_CONTINUE;

		}

		proc->evStallTimer = stallTimer;
	}

	return EVC_STOP_YIELD;
}

// Counter Operations
u8 Event0F_(struct EventEngineProc* proc) {
	s8 newValue;

	u8       subcode  = 0xF & (*(const u8*)(proc->pEventCurrent));
	unsigned argument = proc->pEventCurrent[1];
	unsigned shift    = 4 * (*(const u8*)(proc->pEventCurrent + 1) % 8);

	switch (subcode) {

	case 0: // Check
		gEventSlots[0xC] = (gEventSlotCounter >> shift) & 0xF;

		return EVC_ADVANCE_CONTINUE;

	case 1: // Set
		// I can't get that particular part to match so I'll just go with asm
		
		// nonmatching C
		// newValue = (r5 << 0x10) >> 0x18;

		asm(
			"lsl r0, %1, #16\n"
			"lsr %0, r0, #24\n"

			: "=r"(newValue)
			: "r"(argument)
		);

		break;

	case 2: // Increment
		newValue = ((gEventSlotCounter >> shift) & 0xF) + 1;

		if (newValue > 0xF)
			newValue = 0xF;

		break;

	case 3: // Decrement
		newValue = ((gEventSlotCounter >> shift) & 0xF) - 1;

		if (newValue < 0)
			newValue = 0;

		break;

	} // switch (subcode)

	gEventSlotCounter = (
		(gEventSlotCounter &~ (0xF << shift)) |
		((newValue & 0xF) << shift)
	);

	return EVC_ADVANCE_CONTINUE;
}

u8 Event10_ModifyEvBit(struct EventEngineProc* proc) {
	unsigned evArgument = proc->pEventCurrent[1];

	if (((proc->evStateBits >> 2) & 1) && evArgument)
		proc->evStateBits = (proc->evStateBits &~ EV_STATE_SKIPPING) | EV_STATE_FADEDIN;

	switch (evArgument) {

	case 0:
		proc->evStateBits &= ~EV_STATE_NOSKIP;
		proc->evStateBits &= ~EV_STATE_0020;
		proc->evStateBits &= ~EV_STATE_0040;

		return EVC_ADVANCE_CONTINUE;

	case 1:
		proc->evStateBits |= EV_STATE_NOSKIP;
		proc->evStateBits |= EV_STATE_0020;
		proc->evStateBits |= EV_STATE_0040;

		return EVC_ADVANCE_CONTINUE;

	case 2:
		proc->evStateBits &= ~EV_STATE_NOSKIP;
		proc->evStateBits &= ~EV_STATE_0020;
		proc->evStateBits |= EV_STATE_0040;

		return EVC_ADVANCE_CONTINUE;

	case 3:
		proc->evStateBits |= EV_STATE_NOSKIP;
		proc->evStateBits &= ~EV_STATE_0020;
		proc->evStateBits &= ~EV_STATE_0040;

		return EVC_ADVANCE_CONTINUE;

	case 4:
		proc->evStateBits |= EV_STATE_NOSKIP;
		proc->evStateBits |= EV_STATE_0020;
		proc->evStateBits &= ~EV_STATE_0040;

		return EVC_ADVANCE_CONTINUE;

	default:
		return EVC_ERROR;

	} // switch (evArgument)
}

u8 Event11_SetIgnoredKeys(struct EventEngineProc* proc) {
	u8       subcode = 0xF & *(const u8*)(proc->pEventCurrent);
	unsigned mask    = proc->pEventCurrent[1];

	if (subcode == 0)
		SetKeyStatus_IgnoreMask(mask);

	return EVC_ADVANCE_CONTINUE;
}

// SOUND EVENT CODES

u8 Event12_(struct EventEngineProc* proc) {
	short evArgument;

	if ((proc->evStateBits >> 2) & 1)
		return EVC_ADVANCE_CONTINUE;

	evArgument = proc->pEventCurrent[1];

	if (evArgument < 0)
		evArgument = gEventSlots[2];

	Sound_PlaySong80024E4(evArgument, 3, NULL);
	return EVC_ADVANCE_YIELD;
}

u8 Event13_(struct EventEngineProc* proc) {
	u8    subcode;
	short evArgument;

	subcode    = 0xF & *(const u8*)(proc->pEventCurrent);
	evArgument = proc->pEventCurrent[1];

	if (evArgument < 0)
		evArgument = gEventSlots[2];

	if (evArgument != INT16_MAX) {
		if ((proc->evStateBits >> 2) & 1)
			return EVC_ADVANCE_CONTINUE;

		Sound_PlaySong8002574(evArgument, subcode, 0);
		return EVC_ADVANCE_YIELD;
	} else {
		if ((proc->evStateBits >> 2) & 1)
			subcode = 1;
		
		Sound_FadeOut800231C(subcode);
		return EVC_ADVANCE_YIELD;
	}
}

u8 Event14_(struct EventEngineProc* proc) {
	u8 subcode       = 0xF & *(const u8*)(proc->pEventCurrent);
	short evArgument = proc->pEventCurrent[1];

	switch (subcode) {

	case 0:
		if (((proc->evStateBits >> 2) & 1) || ((proc->evStateBits >> 3) & 1))
			return EVC_ADVANCE_CONTINUE;

		if (evArgument < 0)
			evArgument = gEventSlots[2];

		sub_8002620(evArgument);

		break;

	case 1:
		DeleteAll6CWaitMusicRelated();
		sub_80026BC(evArgument);

		break;

	} // switch (subcode)

	return EVC_ADVANCE_YIELD;
}

u8 Event15_(struct EventEngineProc* proc) {
	u8 subcode = 0xF & *(const u8*)(proc->pEventCurrent);

	switch (subcode) {
	
	case 0:
		if ((proc->evStateBits >> 2) & 1)
			return EVC_ADVANCE_CONTINUE;

		ISuspectThisToBeMusicRelated_8002730(0x100, 0x90, 10, (struct Proc*)(proc));

		break;

	case 1:
		if ((proc->evStateBits >> 2) & 1) {
			Sound_SetVolume80022EC(0x100);
			return EVC_ADVANCE_CONTINUE;
		}

		ISuspectThisToBeMusicRelated_8002730(0x90, 0x100, 10, (struct Proc*)(proc));

		break;

	} // switch (subcode)

	return EVC_ADVANCE_YIELD;
}

u8 Event16_(struct EventEngineProc* proc) {
	short evArgument;

	if (((proc->evStateBits >> 2) & 1) || ((proc->evStateBits >> 3) & 1))
		return EVC_ADVANCE_CONTINUE;

	evArgument = proc->pEventCurrent[1];

	if (evArgument < 0)
		evArgument = gEventSlots[2];

	if (!gUnknown_0202BCF0.unk41_2)
		m4aSongNumStart(evArgument);

	return EVC_ADVANCE_CONTINUE;
}

// Fade
u8 Event17_(struct EventEngineProc* proc) {
	u8       subcode;
	unsigned evArgument;

	if ((proc->evStateBits >> 2) & 1)
		return EVC_ADVANCE_CONTINUE;

	subcode    = 0xF & *(const u8*)(proc->pEventCurrent);
	evArgument = proc->pEventCurrent[1];

	switch (subcode) {

	case 0: // FADU
		sub_8013D20(evArgument, (struct Proc*)(proc)); // StartFadeOutBlack

		proc->evStateBits &= ~EV_STATE_FADEDIN;

		gLCDControlBuffer.dispcnt.bg0_on = TRUE;
		gLCDControlBuffer.dispcnt.bg1_on = TRUE;
		gLCDControlBuffer.dispcnt.bg2_on = TRUE;
		gLCDControlBuffer.dispcnt.bg3_on = TRUE;
		gLCDControlBuffer.dispcnt.obj_on = TRUE;

		return EVC_ADVANCE_YIELD;

	case 1: // FADI
		sub_8013D08(evArgument, (struct Proc*)(proc)); // StartFadeInBlack

		proc->evStateBits |= EV_STATE_FADEDIN;

		return EVC_ADVANCE_YIELD;

	case 2: // FAWU
		sub_8013D50(evArgument, (struct Proc*)(proc)); // StartFadeOutWhite

		proc->evStateBits &= ~EV_STATE_FADEDIN;

		gLCDControlBuffer.dispcnt.bg0_on = TRUE;
		gLCDControlBuffer.dispcnt.bg1_on = TRUE;
		gLCDControlBuffer.dispcnt.bg2_on = TRUE;
		gLCDControlBuffer.dispcnt.bg3_on = TRUE;
		gLCDControlBuffer.dispcnt.obj_on = TRUE;

		return EVC_ADVANCE_YIELD;

	case 3: // FAWI
		sub_8013D38(evArgument, (struct Proc*)(proc)); // StartFadeInWhite

		proc->evStateBits |= EV_STATE_FADEDIN;

		return EVC_ADVANCE_YIELD;

	default:
		return EVC_ERROR;

	} // switch (subcode)
}

// I think this is related to some color filtering effect (?)
// Messes with palettes
u8 Event18_(struct EventEngineProc* proc) {
	unsigned evArgument2, evArgument3, evArgument4, evArgument5;
	unsigned mask;
	u8 subcode, evArgumentBitStart, evArgumentBitCount;

	subcode = 0xF & *(const u8*)(proc->pEventCurrent);

	{
		// This is weird but needed to match
		unsigned read      = proc->pEventCurrent[1];
		evArgumentBitStart = *(const u8*)(proc->pEventCurrent + 1);
		evArgumentBitCount = read >> 8;
	}

	evArgument2 = proc->pEventCurrent[2];
	evArgument3 = proc->pEventCurrent[3];
	evArgument4 = proc->pEventCurrent[4];
	evArgument5 = proc->pEventCurrent[5];

	switch (subcode) {

	case 0:
		sub_80127C4();
		return EVC_ADVANCE_YIELD;

	case 1:
		sub_8012824();
		return EVC_ADVANCE_YIELD;

	case 2: {
		s8 i;

		if (((proc->evStateBits >> 2) & 1) || (proc->evStateBits & EV_STATE_FADEDIN))
			evArgument2 = 0;

		mask = 0;

		for (i = evArgumentBitCount; i > 0; --i) {
			s8 tmp = evArgumentBitStart;
			mask = mask | (1 << tmp);
			evArgumentBitStart = tmp + 1;
		}

		sub_8012890(evArgument2, mask, evArgument3, evArgument4, evArgument5, (struct Proc*)(proc));

		return EVC_ADVANCE_YIELD;
	}

	default:
		return EVC_ERROR;

	} // switch (subcode)
}

// Various Checks
u8 Event19_(struct EventEngineProc* proc) {
	u8 subcode = 0xF & *(const u8*)(proc->pEventCurrent);

	switch (subcode) {

	case 0: // Check Mode
		gEventSlots[0xC] = gUnknown_0202BCF0.chapterModeIndex;
		break;

	case 1: // Check (Next?) Chapter Index
		gEventSlots[0xC] = proc->chapterIndex;
		break;

	case 2: // Check Difficult Mode
		if (!(gUnknown_0202BCF0.chapterStateBits & 0x40))
			gEventSlots[0xC] = FALSE;
		else 
			gEventSlots[0xC] = TRUE;

		break;

	case 3: // Check Turn Number
		gEventSlots[0xC] = gUnknown_0202BCF0.chapterTurnNumber;
		break;

	case 4: // Check ?
		gEventSlots[0xC] = CountRedUnits();
		break;

	case 5: // Check ?
		gEventSlots[0xC] = CountGreenUnits();
		break;

	case 6: // Check Chapter Type?
		gEventSlots[0xC] = GetChapterThing();
		break;

	case 7: // Check Some option or difficult mode
		if (gUnknown_0202BCF0.unk42_6 || (gUnknown_0202BCF0.chapterStateBits & 0x40))
			gEventSlots[0xC] = FALSE;
		else
			gEventSlots[0xC] = TRUE;

		break;

	case 8: // Check gold
		gEventSlots[0xC] = GetPartyGoldAmount();
		break;

	case 9: // Check ?
		gEventSlots[0xC] = sub_80845E4(proc->pEventStart);
		break;

	case 10: // Check ?
		if (gUnknown_0202BCF0.chapterStateBits & 0x20)
			gEventSlots[0xC] = TRUE;
		else 
			gEventSlots[0xC] = FALSE;

		break;

	default:
		break;

	} // switch (subcode)

	return EVC_ADVANCE_CONTINUE;
}

u8 Event1A_TEXTSTART(struct EventEngineProc* proc) {
	u8 subcode = 0xF & *(const u8*)(proc->pEventCurrent);

	if (subcode != proc->activeTextType && subcode != 5) {
		sub_8006A7C();
		sub_808F270();
		sub_808BB74();

		if (proc->execType == EV_EXEC_CUTSCENE)
			sub_800BCDC(proc->mapSpritePalIdOverride);

		sub_800E640(proc);
	}

	proc->activeTextType = subcode;

	switch (subcode) {

	case 0:
	case 3:
	case 4:
	case 5:
		return EVC_ADVANCE_YIELD;
	
	case 1:
	case 2:
		return Event23_(proc);

	default:
		return EVC_ERROR;

	} // switch (subcode)
}

void sub_800E210(struct EventEngineProc* proc, u16 stringIndex, s8 b) {
	if (b == TRUE)
		sub_800680C(0x80, 2, 1);

	if ((proc->evStateBits & EV_STATE_0040) == 1) { // ?????
		proc->overwrittenTextSpeed = gUnknown_0202BCF0.cfgTextSpeed;
		gUnknown_0202BCF0.cfgTextSpeed = 1;
	} else {
		proc->overwrittenTextSpeed = 0xFF;
	}

	sub_8006A30(1, 1, stringIndex);

	if (proc->evStateBits & EV_STATE_0020)
		sub_8006AA8(4);

	if (proc->evStateBits & EV_STATE_0040)
		sub_8006AA8(8);
}

void sub_800E290(struct EventEngineProc* proc, u16 stringIndex, unsigned flags) {
	flags |= 0x0400;

	if (proc->evStateBits & EV_STATE_0020)
		flags |= 0x0040;

	if (proc->evStateBits & EV_STATE_0040)
		flags |= 0x2000 | 0x0800 | 0x0020;

	gLCDControlBuffer.dispcnt.win0_on   = FALSE;
	gLCDControlBuffer.dispcnt.win1_on   = FALSE;
	gLCDControlBuffer.dispcnt.objWin_on = FALSE;

	LoadObjUIGfx();
	sub_800680C(0x80, 0, 1);
	BG_EnableSyncByMask(1); // bg0

	sub_808F128(
		3,
		0x12,
		0x14,
		4,
		stringIndex,
		OBJ_VRAM0 + 0x1000,
		-1,
		NULL // parent proc
	);

	sub_808E9D8(flags);
}

void sub_800E31C(struct EventEngineProc* proc, u16 stringIndex, unsigned flags) {
	u8 x, y;

	x = ((u16*)(gEventSlots + 0xB))[0];
	y = ((u16*)(gEventSlots + 0xB))[1];

	if (x == (u8)(-1) && y == (u8)(-1)) {
		x = 0;
		y = 0;
		flags |= 0x0100;
	} else {
		int boxWidth, boxHeight;
		GetStringFromIndex(stringIndex);

		sub_808AADC(sub_800A2A4(), &boxWidth, &boxHeight);

		if (x == (u8)(-1))
			x = (224 - boxWidth) / 2 - 8;
		else if (y == (u8)(-1))
			y = (144 - boxHeight) / 2;
	}

	if (proc->evStateBits & EV_STATE_0020)
		flags |= 0x0080;

	if (proc->evStateBits & EV_STATE_0040)
		flags |= 0x0008;

	sub_808AA04(x, y, stringIndex, NULL);
	sub_808A518(flags);
}

#ifdef NONMATCHING

// Not matching yet

u8 Event1B_TEXTSHOW(struct EventEngineProc* proc) {
	u8       subcode    = 0xF & *(const u8*)(proc->pEventCurrent);
	short    evArgument = proc->pEventCurrent[1];
	unsigned zeroFlag   = 0;

	if (subcode != 2) {
		if (evArgument < 0)
			evArgument = gEventSlots[2];

		if (evArgument == 0)
			return EVC_ADVANCE_CONTINUE;
	}

	switch (subcode) {

	case 0:
		proc->evStateBits &= ~EV_STATE_0008;

		if ((proc->evStateBits >> 2) & 1)
			break;

		switch (proc->activeTextType) {

		case 0:
			sub_800E210(proc, evArgument, TRUE);
			break;

		case 1:
			sub_800E210(proc, evArgument, TRUE);
			break;

		case 2:
			sub_800E290(proc, evArgument, zeroFlag);
			break;

		case 3:
			sub_800E31C(proc, evArgument, 0x0010);
			break;

		case 4:
			sub_800E31C(proc, evArgument, zeroFlag);
			break;

		case 5:
			sub_800E31C(proc, evArgument, (0x0010 | 0x0020));
			break;

		} // switch (proc->activeTextType)

		break;

	case 1:
		if ((proc->evStateBits >> 2) & 1)
			break;

		if ((proc->evStateBits >> 3) & 1)
			break;

		switch (proc->activeTextType) {

		case 0:
			sub_800E210(proc, evArgument, FALSE);
			break;

		case 1:
			sub_800E210(proc, evArgument, FALSE);
			break;

		case 2:
			sub_800E290(proc, evArgument, zeroFlag);
			break;

		case 3:
			sub_800E31C(proc, evArgument, 0x0010);
			break;

		case 4:
			sub_800E31C(proc, evArgument, zeroFlag);
			break;

		case 5:
			sub_800E31C(proc, evArgument, (0x0010 | 0x0020));
			break;

		} // switch (proc->activeTextType)

		break;

	case 2: // REMA
		proc->evStateBits &= ~EV_STATE_0008;

		sub_8006A7C();
		sub_808F270();
		sub_808BB74();

		if (proc->execType == EV_EXEC_CUTSCENE)
			sub_800BCDC(proc->mapSpritePalIdOverride);

		sub_800E640(proc);

		return EVC_ADVANCE_YIELD;

	} // switch (subcode)

	return EVC_ADVANCE_CONTINUE;
}

#else // !NONMATCHING

__attribute__((naked))
u8 Event1B_TEXTSHOW(struct EventEngineProc* proc) {
	asm(
		".syntax unified\n"

		"push {r4, r5, lr}\n"
		"adds r4, r0, #0\n"
		"ldr r1, [r4, #0x38]\n"
		"ldrb r0, [r1]\n"
		"movs r2, #0xf\n"
		"ands r2, r0\n"
		"ldrh r3, [r1, #2]\n"
		"movs r5, #0\n"
		"cmp r2, #2\n"
		"beq _0800E3EC\n"
		"lsls r0, r3, #0x10\n"
		"cmp r0, #0\n"
		"bge _0800E3E6\n"
		"ldr r0, _0800E3FC  @ gEventSlots\n"
		"ldrh r3, [r0, #8]\n"
	"_0800E3E6:\n"
		"cmp r3, #0\n"
		"bne _0800E3EC\n"
		"b _0800E514\n"
	"_0800E3EC:\n"
		"cmp r2, #1\n"
		"beq _0800E462\n"
		"cmp r2, #1\n"
		"bgt _0800E400\n"
		"cmp r2, #0\n"
		"beq _0800E406\n"
		"b _0800E514\n"
		".align 2, 0\n"
	"_0800E3FC: .4byte gEventSlots\n"
	"_0800E400:\n"
		"cmp r2, #2\n"
		"beq _0800E4DE\n"
		"b _0800E514\n"
	"_0800E406:\n"
		"ldrh r0, [r4, #0x3c]\n"
		"ldr r1, _0800E430  @ 0x0000FFF7\n"
		"ands r1, r0\n"
		"strh r1, [r4, #0x3c]\n"
		"lsrs r1, r1, #2\n"
		"movs r0, #1\n"
		"ands r1, r0\n"
		"cmp r1, #0\n"
		"beq _0800E41A\n"
		"b _0800E514\n"
	"_0800E41A:\n"
		"adds r0, r4, #0\n"
		"adds r0, #0x42\n"
		"ldrb r0, [r0]\n"
		"cmp r0, #5\n"
		"bhi _0800E514\n"
		"lsls r0, r0, #2\n"
		"ldr r1, _0800E434  @ _0800E438\n"
		"adds r0, r0, r1\n"
		"ldr r0, [r0]\n"
		"mov pc, r0\n"
		".align 2, 0\n"
	"_0800E430: .4byte 0x0000FFF7\n"
	"_0800E434: .4byte _0800E438\n"
	"_0800E438: @ jump table\n"
		".4byte _0800E450 @ case 0\n"
		".4byte _0800E450 @ case 1\n"
		".4byte _0800E4B4 @ case 2\n"
		".4byte _0800E45C @ case 3\n"
		".4byte _0800E4C6 @ case 4\n"
		".4byte _0800E4D2 @ case 5\n"
	"_0800E450:\n"
		"adds r1, r3, #0\n"
		"adds r0, r4, #0\n"
		"movs r2, #1\n"
		"bl sub_800E210\n"
		"b _0800E514\n"
	"_0800E45C:\n"
		"adds r1, r3, #0\n"
		"movs r2, #0x10\n"
		"b _0800E4D6\n"
	"_0800E462:\n"
		"ldrh r0, [r4, #0x3c]\n"
		"lsls r1, r0, #0x10\n"
		"lsrs r0, r1, #0x12\n"
		"movs r2, #1\n"
		"ands r0, r2\n"
		"cmp r0, #0\n"
		"bne _0800E514\n"
		"lsrs r0, r1, #0x13\n"
		"ands r0, r2\n"
		"cmp r0, #0\n"
		"bne _0800E514\n"
		"adds r0, r4, #0\n"
		"adds r0, #0x42\n"
		"ldrb r0, [r0]\n"
		"cmp r0, #5\n"
		"bhi _0800E514\n"
		"lsls r0, r0, #2\n"
		"ldr r1, _0800E48C  @ _0800E490\n"
		"adds r0, r0, r1\n"
		"ldr r0, [r0]\n"
		"mov pc, r0\n"
		".align 2, 0\n"
	"_0800E48C: .4byte _0800E490\n"
	"_0800E490: @ jump table\n"
		".4byte _0800E4A8 @ case 0\n"
		".4byte _0800E4A8 @ case 1\n"
		".4byte _0800E4B4 @ case 2\n"
		".4byte _0800E4C0 @ case 3\n"
		".4byte _0800E4C6 @ case 4\n"
		".4byte _0800E4D2 @ case 5\n"
	"_0800E4A8:\n"
		"adds r1, r3, #0\n"
		"adds r0, r4, #0\n"
		"movs r2, #0\n"
		"bl sub_800E210\n"
		"b _0800E514\n"
	"_0800E4B4:\n"
		"adds r1, r3, #0\n"
		"adds r0, r4, #0\n"
		"adds r2, r5, #0\n"
		"bl sub_800E290\n"
		"b _0800E514\n"
	"_0800E4C0:\n"
		"adds r1, r3, #0\n"
		"movs r2, #0x10\n"
		"b _0800E4D6\n"
	"_0800E4C6:\n"
		"adds r1, r3, #0\n"
		"adds r0, r4, #0\n"
		"adds r2, r5, #0\n"
		"bl sub_800E31C\n"
		"b _0800E514\n"
	"_0800E4D2:\n"
		"adds r1, r3, #0\n"
		"movs r2, #0x30\n"
	"_0800E4D6:\n"
		"adds r0, r4, #0\n"
		"bl sub_800E31C\n"
		"b _0800E514\n"
	"_0800E4DE:\n"
		"ldrh r0, [r4, #0x3c]\n"
		"ldr r1, _0800E510  @ 0x0000FFF7\n"
		"ands r1, r0\n"
		"strh r1, [r4, #0x3c]\n"
		"bl sub_8006A7C\n"
		"bl sub_808F270\n"
		"bl sub_808BB74\n"
		"adds r0, r4, #0\n"
		"adds r0, #0x41\n"
		"ldrb r0, [r0]\n"
		"cmp r0, #1\n"
		"bne _0800E506\n"
		"adds r0, r4, #0\n"
		"adds r0, #0x44\n"
		"ldrh r0, [r0]\n"
		"bl sub_800BCDC\n"
	"_0800E506:\n"
		"adds r0, r4, #0\n"
		"bl sub_800E640\n"
		"movs r0, #2\n"
		"b _0800E516\n"
		".align 2, 0\n"
	"_0800E510: .4byte 0x0000FFF7\n"
	"_0800E514:\n"
		"movs r0, #0\n"
	"_0800E516:\n"
		"pop {r4, r5}\n"
		"pop {r1}\n"
		"bx r1\n"

		".syntax divided\n"
	);
}

#endif // !NONMATCHING

u8 Event1C_TEXTCONT(struct EventEngineProc* proc) {
	if (((proc->evStateBits >> 2) & 1)) {
		sub_8006A7C();
		sub_808F270();
		sub_808BB74();

		if (proc->execType == EV_EXEC_CUTSCENE)
			sub_800BCDC(proc->mapSpritePalIdOverride);

		sub_800E640(proc);
	} else {
		sub_8006EF0();
	}
	
	return EVC_ADVANCE_YIELD;
}

u8 Event1D_TEXTEND(struct EventEngineProc* proc) {
	if (((proc->evStateBits >> 2) & 1)) {
		sub_8006A7C();
		sub_808F270();
		sub_808BB74();

		if (proc->execType == EV_EXEC_CUTSCENE)
			sub_800BCDC(proc->mapSpritePalIdOverride);

		sub_800E640(proc);

		gEventSlots[0xC] = 0;
	} else {
		unsigned flag = FALSE;

		if ((sub_80089D0() && !sub_8006ED8()) || sub_808F284() || Proc_Find(gUnknown_08A016E0))
			flag = TRUE;

		if (flag == TRUE) {
			switch (proc->execType) {

			case EV_EXEC_WORLDMAP:
			case EV_EXEC_UNK4:
				return EVC_STOP_YIELD;

			default:
				switch (proc->activeTextType) {

				case 0:
				case 1:
				case 2:
					SetSpecialColorEffectsParameters(1, 0x10, 1, 0);
					break;

				case 3:
				case 4:
				case 5:
					break;

				} // switch (proc->activeTextType)

				return EVC_STOP_YIELD;

			} // switch (proc->execType)
		} else {
			gEventSlots[0xC] = sub_8008A00();
		}
	}

	if (proc->overwrittenTextSpeed != -1)
		gUnknown_0202BCF0.cfgTextSpeed = proc->overwrittenTextSpeed;

	return EVC_ADVANCE_YIELD;
}

void sub_800E640(struct EventEngineProc* proc) {
	if (proc->evStateBits & EV_STATE_FADEDIN) {
		sub_80081A8();
		Proc_DeleteAllWithScript(gUnknown_08591154); // end all faces
		ResetFaces();
		sub_80067E8();
	} else if (Face6CExists()) {
		sub_80081A8();
		Proc_ForEachWithScript(gUnknown_08591154, (ProcFunc)(sub_8005F38));
		Proc_CreateBlockingChild(gUnknown_08591DE8, (struct Proc*)(proc));
	}
}

void _WhileFace6CExists(struct Proc* proc) {
	if (!Face6CExists())
		Proc_ClearNativeCallback(proc);
}

u8 Event1E_(struct EventEngineProc* proc) {
	u8       subcode    = 0xF & *(const u8*)(proc->pEventCurrent);
	short    evArgument = proc->pEventCurrent[1];

	if (evArgument == -1)
		evArgument = gEventSlots[2];

	if (evArgument != -3) {
		if (evArgument != -2) {
			if (((proc->evStateBits >> 2) & 1)) // is skipping
				return EVC_ADVANCE_CONTINUE;

			gUnknown_03000428.sub.unk00 = subcode + 8;
			gUnknown_03000428.sub.unk01 = 0x10;
			gUnknown_03000428.sub.unk02 = evArgument;
			gUnknown_03000428.sub.unk03 = 1;

			gUnknown_03000428.unk04 = 1;
			gUnknown_03000428.unk05 = 0;

			sub_800680C(0x80, 2, 1);
		} else {
			gUnknown_03000428.sub.unk00 = subcode + 8;
			gUnknown_03000428.sub.unk01 = 0x11;
			gUnknown_03000428.sub.unk02 = 1;
			gUnknown_03000428.sub.unk03 = 0;

			sub_80081A8();
		}
	} else {
		sub_8006A7C();
		sub_808F270();
		sub_808BB74();

		if (proc->execType == EV_EXEC_CUTSCENE)
			sub_800BCDC(proc->mapSpritePalIdOverride);

		sub_800E640(proc);

		return EVC_ADVANCE_YIELD;
	}

	sub_8006A70(0, 0, &gUnknown_03000428.sub);
	return EVC_ADVANCE_CONTINUE;
}

u8 Event1F_(struct EventEngineProc* proc) {
	unsigned evX, evY;

	if (((proc->evStateBits >> 2) & 1)) // is skipping
		return EVC_ADVANCE_CONTINUE;

	if (((proc->evStateBits >> 3) & 1)) // is skipping dialogue?
		return EVC_ADVANCE_CONTINUE;

	{
		// I wonder if I can get this to be better
		unsigned read = proc->pEventCurrent[1];
		evX           = *(const u8*)(proc->pEventCurrent + 1);
		evY           = read >> 8;
	}

	gUnknown_03000430.unk00 = evX + 8;
	gUnknown_03000430.unk01 = 0x80;
	gUnknown_03000430.unk02 = evY + 10;
	gUnknown_03000430.unk03 = 0;

	sub_8006A70(0, 0, &gUnknown_03000430);
	return EVC_ADVANCE_CONTINUE;
}

u8 Event20_(struct EventEngineProc* proc) {
	sub_80081A8();
	return EVC_ADVANCE_CONTINUE;
}

u8 sub_800E7D0(u8 mode, u16 bgIndex) {
	BG_SetPosition(0, 0, 0);
	BG_SetPosition(1, 0, 0);
	BG_SetPosition(2, 0, 0);
	BG_SetPosition(3, 0, 0);

	switch (mode) {

	case 0:
		return EVC_ERROR;

	case 3:
		return EVC_ERROR;

	case 4:
		return EVC_ERROR;

	case 5:
		return EVC_ERROR;

	case 1:
		// Randomize background (for support viewers)
		if (bgIndex == 0x37) // TODO: use an enum for convo backgrounds
			bgIndex = NextRN_N(0x35);

		// Loading Background Tile Graphics

		CopyDataWithPossibleUncomp(
			gUnknown_0895DD1C[bgIndex].pTileGraphics,
			(void*)(VRAM + GetBackgroundTileDataOffset(3))
		);

		// Loading Background Tile Arrangement

		CallARM_FillTileRect(
			(u16*)(gBG3TilemapBuffer),
			gUnknown_0895DD1C[bgIndex].pTileArrangement,
			0x8000 // base palette is bg palette 8
		);

		// Loading Background Palettes

		CopyToPaletteBuffer(
			gUnknown_0895DD1C[bgIndex].pColorPalettes,
			0x100, // bg pal 8
			0x100  // 8 palettes
		);

		BG_EnableSyncByMask(1 << 3);
		EnablePaletteSync();

		gPaletteBuffer[0] = 0;

		return EVC_ADVANCE_YIELD;

	case 2:
		sub_80B65F8(
			(u16*)(gBG3TilemapBuffer),
			GetBackgroundTileDataOffset(3),
			8,
			8,
			bgIndex
		);

		BG_EnableSyncByMask(1 << 3);
		EnablePaletteSync();

		return EVC_ADVANCE_YIELD;

	default:
		return EVC_ADVANCE_YIELD;

	} // switch (mode)
}

u8 Event21_(struct EventEngineProc* proc) {
	unsigned evArgument2_a, evArgument2_b, evArgument3;
	struct ConvoBackgroundFadeProc* otherProc;

	int   subcode    = 0xF & *(const u8*)(proc->pEventCurrent);
	short evArgument = proc->pEventCurrent[1];

	if (evArgument < 0)
		evArgument = gEventSlots[2];

	switch (subcode) {

	case 0:
		return sub_800E7D0(proc->activeTextType, evArgument);

	case 1: {
		evArgument2_a = proc->pEventCurrent[2];
		evArgument3   = proc->pEventCurrent[3];

		switch (proc->activeTextType) {

		case 0:
		case 3:
		case 4:
		case 5:
			switch (evArgument2_a) {

			case 0:
			case 3:
			case 4:
			case 5:
				return EVC_ERROR;

			case 1:
			case 2:
				if (((proc->evStateBits >> 2) & 1)) // is skipping
					return EVC_ADVANCE_CONTINUE;

				otherProc = (struct ConvoBackgroundFadeProc*) Proc_CreateBlockingChild(gUnknown_08591E58, (struct Proc*)(proc));
				otherProc->fadeType = 1;

				break;

			} // switch (a)

			break;

		case 1:
		case 2:
			switch (evArgument2_a) {

			case 0:
			case 3:
			case 4:
			case 5:
				if (((proc->evStateBits >> 2) & 1)) // is skipping
					return Event22_(proc); // CLEAN

				otherProc = (struct ConvoBackgroundFadeProc*) Proc_CreateBlockingChild(gUnknown_08591EB0, (struct Proc*)(proc));
				otherProc->fadeType = 2;

				break;

			case 1:
			case 2:
				if (((proc->evStateBits >> 2) & 1)) // is skipping
					return EVC_ADVANCE_CONTINUE;

				otherProc = (struct ConvoBackgroundFadeProc*) Proc_CreateBlockingChild(gUnknown_08591E00, (struct Proc*)(proc));
				otherProc->fadeType = 0;

				break;

			}

			break;

		}

		otherProc->unkType = evArgument2_a;
		otherProc->bgIndex = evArgument;
		otherProc->fadeSpeed = evArgument3;
		otherProc->fadeTimer = 0;
		otherProc->pEventEngine = proc;

		break;
	}

	case 2: {
		evArgument2_b = proc->pEventCurrent[2];
		evArgument3   = proc->pEventCurrent[3];

		if (((proc->evStateBits >> 2) & 1)) // is skipping
			return EVC_ADVANCE_CONTINUE;

		if (sub_800E7D0(proc->activeTextType, evArgument) == EVC_ERROR)
			return EVC_ERROR;

		sub_80B2780(evArgument3, 0, evArgument2_b, (struct Proc*)(proc));

		break;
	}

	case 3: {
		evArgument2_b = proc->pEventCurrent[2];
		evArgument3   = proc->pEventCurrent[3];

		if (((proc->evStateBits >> 2) & 1)) // is skipping
			return EVC_ADVANCE_CONTINUE;

		sub_80B272C(evArgument3, 0, evArgument2_b, (struct Proc*)(proc));

		break;
	}

	} // switch (subcode)

	return EVC_ADVANCE_YIELD;
}

void sub_800EA84(struct ConvoBackgroundFadeProc* proc) {
	gLCDControlBuffer.dispcnt.bg0_on = FALSE;
	gLCDControlBuffer.dispcnt.bg1_on = FALSE;
	gLCDControlBuffer.dispcnt.bg2_on = FALSE;
	gLCDControlBuffer.dispcnt.bg3_on = TRUE;
	gLCDControlBuffer.dispcnt.obj_on = TRUE;

	BG_SetPosition(2, 0, 0);

	switch (proc->fadeType) {

	case 0:
		SetSpecialColorEffectsParameters(1, 0, 0x10, 0);

		// First: BG3; Second: BG2
		sub_8001ED0(FALSE, FALSE, FALSE, TRUE, FALSE); // SetColorEffectFirstTarget
		sub_8001F0C(FALSE, FALSE, TRUE, FALSE, FALSE); // SetColorEffectSecondTarget

		// BG0 > BG1 > BG3 > BG2
		gLCDControlBuffer.bg0cnt.priority = 0;
		gLCDControlBuffer.bg1cnt.priority = 1;
		gLCDControlBuffer.bg2cnt.priority = 3;
		gLCDControlBuffer.bg3cnt.priority = 2;

		break;

	case 1:
		SetSpecialColorEffectsParameters(1, 0, 0x10, 0);

		// First: BG2; Second: BG3|OBJ
		sub_8001ED0(FALSE, FALSE, TRUE, FALSE, FALSE); // SetColorEffectFirstTarget
		sub_8001F0C(FALSE, FALSE, FALSE, TRUE, TRUE);  // SetColorEffectSecondTarget

		// BG2 > BG0 > BG1 > BG3
		gLCDControlBuffer.bg0cnt.priority = 1;
		gLCDControlBuffer.bg1cnt.priority = 2;
		gLCDControlBuffer.bg2cnt.priority = 0;
		gLCDControlBuffer.bg3cnt.priority = 3;

		break;

	case 2:
		SetSpecialColorEffectsParameters(1, 0x10, 0, 0);

		// First: BG2; Second: BG3|OBJ
		sub_8001ED0(FALSE, FALSE, TRUE, FALSE, FALSE); // SetColorEffectFirstTarget
		sub_8001F0C(FALSE, FALSE, FALSE, TRUE, TRUE);  // SetColorEffectSecondTarget

		// BG2 > BG0 > BG1 > BG3
		gLCDControlBuffer.bg0cnt.priority = 1;
		gLCDControlBuffer.bg1cnt.priority = 2;
		gLCDControlBuffer.bg2cnt.priority = 0;
		gLCDControlBuffer.bg3cnt.priority = 3;

		break;

	}

	sub_8001F48(TRUE);
	sub_8001F64(FALSE);

	sub_800BA04(0, 6);
}

void sub_800EBB0(struct ConvoBackgroundFadeProc* proc) {
	sub_800B910(3, 2, 10);
	sub_800B954(3, 2, 1);
	sub_800B994(8, 0, 6);
	sub_800B9B8(2, -8);

	gLCDControlBuffer.dispcnt.bg0_on = FALSE;
	gLCDControlBuffer.dispcnt.bg1_on = FALSE;
	gLCDControlBuffer.dispcnt.bg2_on = TRUE;
	gLCDControlBuffer.dispcnt.bg3_on = TRUE;
	gLCDControlBuffer.dispcnt.obj_on = TRUE;
}

void sub_800EC00(struct ConvoBackgroundFadeProc* proc) {
	sub_800B910(2, 3, 10);
	sub_800B954(2, 3, 1);
	sub_800B994(0, 8, 6);
	sub_800B9B8(3, 8);

	gLCDControlBuffer.dispcnt.bg0_on = FALSE;
	gLCDControlBuffer.dispcnt.bg1_on = FALSE;
	gLCDControlBuffer.dispcnt.bg2_on = TRUE;
	gLCDControlBuffer.dispcnt.bg3_on = TRUE;
	gLCDControlBuffer.dispcnt.obj_on = TRUE;
}

void sub_800EC50(struct ConvoBackgroundFadeProc* proc) {
	switch (proc->unkType) {

	case 0:
	case 3:
	case 4:
	case 5:
		while (1) {} // oh

	case 1:
		if (proc->bgIndex == 0x37) // TODO: use an enum for convo backgrounds
			proc->bgIndex = NextRN_N(0x35);

		// Loading Background Tile Graphics

		CopyDataWithPossibleUncomp(
			gUnknown_0895DD1C[proc->bgIndex].pTileGraphics,
			(void*)(VRAM + GetBackgroundTileDataOffset(2))
		);

		// Loading Background Tile Arrangement

		CallARM_FillTileRect(
			(u16*)(gBG2TilemapBuffer),
			gUnknown_0895DD1C[proc->bgIndex].pTileArrangement,
			0 // base palette is bg palette 0
		);

		// Loading Background Palettes

		CopyToPaletteBuffer(
			gUnknown_0895DD1C[proc->bgIndex].pColorPalettes,
			0x000, // bg pal 0
			0x0C0  // 7 palettes
		);

		BG_EnableSyncByMask(1 << 2);
		EnablePaletteSync();

		gPaletteBuffer[0] = 0;

		break;

	case 2:
		sub_80B65F8(
			(u16*)(gBG2TilemapBuffer),
			GetBackgroundTileDataOffset(2),
			0,
			6,
			proc->bgIndex
		);

		BG_EnableSyncByMask(1 << 2);
		EnablePaletteSync();

		break;

	}

	gLCDControlBuffer.dispcnt.bg0_on = FALSE;
	gLCDControlBuffer.dispcnt.bg1_on = FALSE;
	gLCDControlBuffer.dispcnt.bg2_on = TRUE;
	gLCDControlBuffer.dispcnt.bg3_on = TRUE;
	gLCDControlBuffer.dispcnt.obj_on = TRUE;
}

void sub_800ED50(struct ConvoBackgroundFadeProc* proc) {
	switch (proc->unkType) {

	case 0:
	case 3:
	case 4:
	case 5:
		while (1) {} // oh

	case 1:
		if (proc->bgIndex == 0x37) // TODO: use an enum for convo backgrounds
			proc->bgIndex = NextRN_N(0x35);

		// Loading Background Tile Graphics

		CopyDataWithPossibleUncomp(
			gUnknown_0895DD1C[proc->bgIndex].pTileGraphics,
			(void*)(VRAM + GetBackgroundTileDataOffset(3))
		);

		// Loading Background Tile Arrangement

		CallARM_FillTileRect(
			(u16*)(gBG3TilemapBuffer),
			gUnknown_0895DD1C[proc->bgIndex].pTileArrangement,
			0x8000 // base palette is bg palette 8
		);

		// Loading Background Palettes

		CopyToPaletteBuffer(
			gUnknown_0895DD1C[proc->bgIndex].pColorPalettes,
			0x100, // bg pal 8
			0x0C0  // 7 palettes
		);

		BG_EnableSyncByMask(1 << 3);
		EnablePaletteSync();

		gPaletteBuffer[0] = 0;

		break;

	case 2:
		sub_80B65F8(
			(u16*)(gBG3TilemapBuffer),
			GetBackgroundTileDataOffset(3),
			8,
			6,
			proc->bgIndex
		);

		BG_EnableSyncByMask(1 << 3);
		EnablePaletteSync();

		break;

	}

	gLCDControlBuffer.dispcnt.bg0_on = FALSE;
	gLCDControlBuffer.dispcnt.bg1_on = FALSE;
	gLCDControlBuffer.dispcnt.bg2_on = TRUE;
	gLCDControlBuffer.dispcnt.bg3_on = TRUE;
	gLCDControlBuffer.dispcnt.obj_on = TRUE;
}

void sub_800EE54(struct ConvoBackgroundFadeProc* proc) {
	switch (proc->unkType) {

	case 0:
	case 3:
	case 4:
	case 5:
		UnpackChapterMapGraphics(gUnknown_0202BCF0.chapterIndex);
		AllocWeatherParticles(gUnknown_0202BCF0.chapterWeatherId);
		RenderBmMap();
		SMS_UpdateFromGameData();

		sub_800BCDC(proc->pEventEngine->mapSpritePalIdOverride);
		SMS_FlushIndirect();

		Event24_(proc->pEventEngine);
		break;

	case 1:
	case 2:
		while (1) {} // oh

	}

	gLCDControlBuffer.dispcnt.bg0_on = FALSE;
	gLCDControlBuffer.dispcnt.bg1_on = FALSE;
	gLCDControlBuffer.dispcnt.bg2_on = TRUE;
	gLCDControlBuffer.dispcnt.bg3_on = TRUE;
	gLCDControlBuffer.dispcnt.obj_on = TRUE;
}

void sub_800EEE8(struct ConvoBackgroundFadeProc* proc) {
	unsigned currentFadeLevel = (proc->fadeTimer += proc->fadeSpeed) / 16;

	switch (proc->fadeType) {

	case 0:
	case 1:
		SetSpecialColorEffectsParameters(1, currentFadeLevel, 0x10 - currentFadeLevel, 0);
		break;

	case 2:
		SetSpecialColorEffectsParameters(1, 0x10 - currentFadeLevel, currentFadeLevel, 0);
		break;

	}

	if (currentFadeLevel >= 0x10)
		Proc_ClearNativeCallback((struct Proc*)(proc));
}

void sub_800EF48(struct ConvoBackgroundFadeProc* proc) {
	switch (proc->fadeType) {

	case 0:
		gLCDControlBuffer.dispcnt.bg0_on = FALSE;
		gLCDControlBuffer.dispcnt.bg1_on = FALSE;
		gLCDControlBuffer.dispcnt.bg2_on = FALSE;
		gLCDControlBuffer.dispcnt.bg3_on = TRUE;
		gLCDControlBuffer.dispcnt.obj_on = TRUE;

		break;

	case 1:
		gLCDControlBuffer.dispcnt.bg0_on = FALSE;
		gLCDControlBuffer.dispcnt.bg1_on = FALSE;
		gLCDControlBuffer.dispcnt.bg2_on = FALSE;
		gLCDControlBuffer.dispcnt.bg3_on = TRUE;
		gLCDControlBuffer.dispcnt.obj_on = FALSE;

		Event23_(proc->pEventEngine);

		break;

	case 2:
		gLCDControlBuffer.dispcnt.bg0_on = FALSE;
		gLCDControlBuffer.dispcnt.bg1_on = FALSE;
		gLCDControlBuffer.dispcnt.bg2_on = FALSE;
		gLCDControlBuffer.dispcnt.bg3_on = TRUE;
		gLCDControlBuffer.dispcnt.obj_on = TRUE;

		Font_LoadForUI();
		sub_80156D4();

		break;

	}

	// BG0 > BG1 > BG2 > BG3
	gLCDControlBuffer.bg0cnt.priority = 0;
	gLCDControlBuffer.bg1cnt.priority = 1;
	gLCDControlBuffer.bg2cnt.priority = 2;
	gLCDControlBuffer.bg3cnt.priority = 3;

	BG_SetPosition(2, 0, 0);

	SetSpecialColorEffectsParameters(0, 0x10, 0, 0);

	sub_8001ED0(FALSE, FALSE, FALSE, FALSE, FALSE); // SetColorEffectFirstTarget
	sub_8001F0C(FALSE, FALSE, FALSE, FALSE, FALSE); // SetColorEffectSecondTarget

	sub_8001F48(TRUE);
	sub_8001F64(TRUE);

	sub_800BA34();

	BG_Fill(gBG2TilemapBuffer, 0);
	BG_EnableSyncByMask(1 << 2);

	CpuFastFill(0, (void*)(VRAM + GetBackgroundTileDataOffset(0)), 0x20);
	CpuFastFill(0, (void*)(VRAM + GetBackgroundTileDataOffset(1)), 0x20);
	CpuFastFill(0, (void*)(VRAM + GetBackgroundTileDataOffset(2)), 0x20);
}

u8 Event22_(struct EventEngineProc* proc) {
	RefreshBMapGraphics(); // ReloadGameCoreGraphics
	sub_800BCDC(proc->mapSpritePalIdOverride);

	BG_Fill(gBG0TilemapBuffer, 0);
	BG_Fill(gBG1TilemapBuffer, 0);

	BG_EnableSyncByMask(1 << 0);
	BG_EnableSyncByMask(1 << 1);

	sub_80081A8();

	Proc_DeleteAllWithScript(gUnknown_08591154); // end all faces
	ResetFaces();

	sub_80067E8();

	return Event24_(proc);
}

u8 Event23_(struct EventEngineProc* proc) {
	if (!(proc->evStateBits & EV_STATE_GFXLOCKED)) {
		BMapDispSuspend();
		MU_AllDisable();
	}

	proc->evStateBits |= EV_STATE_GFXLOCKED;
	return EVC_ADVANCE_YIELD;
}

u8 Event24_(struct EventEngineProc* proc) {
	if (proc->evStateBits & EV_STATE_GFXLOCKED) {
		BMapDispResume();
		MU_AllEnable();
	}

	proc->evStateBits &= ~EV_STATE_GFXLOCKED;
	return EVC_ADVANCE_YIELD;
}

u8 Event25_(struct EventEngineProc* proc) {
	u8 x, y;
	short chIndex;

	{
		// order needed to match
		const u16* current = proc->pEventCurrent;

		x = ((u16*)(gEventSlots + 0xB))[0];
		y = ((u16*)(gEventSlots + 0xB))[1];

		chIndex = current[1];
	}

	if (chIndex < 0)
		chIndex = gEventSlots[2];

	// ensure gfx are unlocked
	Event24_(proc);

	gUnknown_0202BCF0.chapterIndex = chIndex;

	RestartBattleMap();

	gUnknown_0202BCB0.camera.x = sub_8015A40(x * 16);
	gUnknown_0202BCB0.camera.y = sub_8015A6C(y * 16);

	RefreshEntityBmMaps();
	RenderBmMap();
	SMS_UpdateFromGameData();
	RefreshBMapGraphics();

	sub_800BCDC(proc->mapSpritePalIdOverride);

	BG_Fill(gBG0TilemapBuffer, 0);
	BG_Fill(gBG1TilemapBuffer, 0);

	BG_EnableSyncByMask(1 << 0);
	BG_EnableSyncByMask(1 << 1);

	return EVC_ADVANCE_YIELD;
}

u8 Event26_CameraControlMaybe(struct EventEngineProc* proc) {
	s8 x, y;

	u8 subcode = (0x7 & proc->pEventCurrent[0]);
	u8 sc2     = (0xF & proc->pEventCurrent[0]) >> 3;

	switch (subcode) {

	case 0: // position
		y = (proc->pEventCurrent[1] >> 8);
		x = *(const u8*)(proc->pEventCurrent + 1);

		if (x < 0 || y < 0) {
			x = ((u16*)(gEventSlots + 0xB))[0];
			y = ((u16*)(gEventSlots + 0xB))[1];
		}

		break;

	case 1: { // unit
		struct Unit* unit = GetUnitStructFromEventParameter(proc->pEventCurrent[1]);

		if (!unit)
			return EVC_ERROR;

		x = unit->xPos;
		y = unit->yPos;

		break;
	}

	} // switch (subcode)

	if (((proc->evStateBits >> 2) & 1) || (proc->evStateBits & EV_STATE_FADEDIN)) {
		SetSomeRealCamPos(x, y, sc2);
		SetCursorMapPosition(x, y);
		RenderBmMap();

		return EVC_ADVANCE_CONTINUE;
	} else {
		if (!sc2)
			EnsureCameraOntoPosition((struct Proc*)(proc), x, y);
		else
			sub_8015D84((struct Proc*)(proc), x, y);

		SetCursorMapPosition(x, y);

		return EVC_ADVANCE_YIELD;
	}
}

u8 Event27_MapChange(struct EventEngineProc* proc) {
	u8 i;
	u8 count = 1;

	unsigned* mapChangeIt;

	u8    subcode     = 0xF & *(const u8*)(proc->pEventCurrent);
	short mapChangeId = proc->pEventCurrent[1];

	switch (mapChangeId) {

	case (-1): // "at position in Slot B"
		mapChangeId = GetMapChangeIdAt(
			((u16*)(gEventSlots + 0xB))[0],
			((u16*)(gEventSlots + 0xB))[1]
		);

		if (mapChangeId < 0)
			return EVC_ERROR;

		break;

	case (-2): // "at position of active unit"
		mapChangeId = GetMapChangeIdAt(
			(u8)(gActiveUnit->xPos),
			(u8)(gActiveUnit->yPos)
		);

		if (mapChangeId < 0)
			return EVC_ERROR;

		break;

	case (-3):
		mapChangeIt = gEventSlotQueue;

		mapChangeId = *mapChangeIt++;
		count = gEventSlots[13]; // qp

		break;

	} // switch (mapChangeId)

	if (proc->evStateBits & EV_STATE_FADEDIN) {
		// Quietly apply tile changes

		for (i = 0; i < count; ++i) {
			switch (subcode) {

			case 0:
				TriggerMapChanges(mapChangeId, FALSE, (struct Proc*)(proc));
				break;

			case 1:
				sub_800BB48(mapChangeId, FALSE, (struct Proc*)(proc));
				break;

			} // switch (subcode)

			mapChangeId = *mapChangeIt++; // ??? potentially not initialized?
		}
	} else {
		// Display tile changes

		u8 doDisplay = TRUE;

		for (i = 0; i < count; ++i) {
			switch (subcode) {

			case 0:
				TriggerMapChanges(mapChangeId, doDisplay, (struct Proc*)(proc));
				break;

			case 1:
				sub_800BB48(mapChangeId, doDisplay, (struct Proc*)(proc));
				break;

			} // switch (subcode)

			mapChangeId = *mapChangeIt++; // ??? potentially not initialized?
			doDisplay   = FALSE;          // Only display the first listed map change
		}
	}

	return EVC_ADVANCE_YIELD;
}

u8 Event28_ChangeWeather(struct EventEngineProc* proc) {
	SetWeather(proc->pEventCurrent[1]);
	return EVC_ADVANCE_YIELD;
}

#ifdef NONMATCHING

void sub_800BAA8(s16, u8);

u8 Event29_SetFogVision(struct EventEngineProc* proc) {
	const struct {
		u8  params;
		u8  code;
		u16 evArgument;
	}* const ev = (const void*)(proc->pEventCurrent);

	u8    doDisplay = ev->params & 0xF;
	short newVision = ev->evArgument;

	if ((proc->evStateBits >> 2) & 1)
		doDisplay = FALSE;

	sub_800BAA8(newVision, doDisplay);
	return EVC_ADVANCE_YIELD;
}

#else // !NONMATCHING

__attribute__((naked))
u8 Event29_SetFogVision(struct EventEngineProc* proc) {
	asm(
		".syntax unified\n"

		"push {r4, lr}\n"
		"adds r2, r0, #0\n"
		"ldr r1, [r2, #0x38]\n"
		"ldrb r0, [r1]\n"
		"movs r3, #0xf\n"
		"ands r3, r0\n"
		"ldrh r4, [r1, #2]\n"
		"ldrh r0, [r2, #0x3c]\n"
		"lsrs r0, r0, #2\n"
		"movs r1, #1\n"
		"ands r0, r1\n"
		"cmp r0, #0\n"
		"beq _0800F418\n"
		"movs r3, #0\n"
	"_0800F418:\n"
		"lsls r0, r4, #0x10\n"
		"asrs r0, r0, #0x10\n"
		"adds r1, r3, #0\n"
		"bl sub_800BAA8\n"
		"movs r0, #2\n"
		"pop {r4}\n"
		"pop {r1}\n"
		"bx r1\n"

		".syntax divided\n"
	);
}

#endif // !NONMATCHING

u8 Event2A_MoveToChapter(struct EventEngineProc* proc) {
	const struct {
		u8 params;
		u8 code;
		u16 evArgument;
	}* const ev = (const void*)(proc->pEventCurrent);

	u8    subcode = ev->params & 0xF;
	short chIndex = ev->evArgument;

	if (chIndex < 0)
		chIndex = gEventSlots[2];

	switch (subcode) {

	case 0:
		SetNextGameActionId(0);
		proc->evStateBits |= EV_STATE_CHANGEGM;

		break;

	case 1:
		SetNextChapterId(chIndex);

		gUnknown_0202BCF0.unk4A_2 = 1;

		SetNextGameActionId(1);
		proc->evStateBits |= EV_STATE_CHANGEGM;

		break;

	case 2:
		SetNextChapterId(chIndex);

		gUnknown_0202BCF0.unk4A_2 = 2;

		SetNextGameActionId(2);
		proc->evStateBits |= EV_STATE_CHANGEGM;

		break;

	case 3:
		GotoChapterWithoutSave(chIndex);
		break;

	case 4:
		gUnknown_0202BCF0.unk4A_2 = 3;

		SetNextGameActionId(3);
		proc->evStateBits |= EV_STATE_CHANGEGM;

		break;

	} // switch (subcode)

	proc->evStateBits |= EV_STATE_CHANGECH;

	DeleteAll6CWaitMusicRelated();
	Sound_FadeOut800231C(4);

	return EVC_ADVANCE_CONTINUE;
}

u16 sub_800F50C(const struct UnitDefinition* unitDefition) {
	u16 result = 0;

	while (unitDefition->charIndex) {
		++result;
		++unitDefition;
	}

	return result;
}

s8 ShouldUNITBeLoaded(const struct UnitDefinition* unitDefinition, u8 unk) {
	const u8 tmp = unk; // needed to match :/

	if (unitDefinition->allegiance == 0 && tmp != 1) {
		struct Unit* unit = GetUnitFromCharId(unitDefinition->charIndex);

		if (unit && (unit->state & 0x00000004)) {
			switch (tmp) {

			case 0:
				return FALSE;

			case 2:
				if (IsSethLArachelMyrrhInnes(unitDefinition->charIndex))
					break;

				return FALSE;

			}
		}
	}

	if (unitDefinition->allegiance != 2)
		return TRUE;

	if (!unitDefinition->sumFlag && !unitDefinition->redaCount) {
		if (gBmMapUnit[unitDefinition->yPosition][unitDefinition->xPosition])
			return FALSE;
	}

	if (IsUnitSlotAvailable(0x80))
		return TRUE;

	return FALSE;
}

void sub_800F5B8(struct EventEngineProc* proc) {
	const struct UnitDefinition* pUnitDefinition = proc->pUnitLoadData;

	short count    = proc->unitLoadCount;
	u8 param = proc->unitLoadParameter;

	u16 something = GetSomeEventEngineMoveRelatedBitfield(proc, TRUE);

	s8 r3 = FALSE;

	if (param == 2)
		r3 = TRUE;

	if ((proc->evStateBits >> 2) & 1) {
		sub_800F698(pUnitDefinition, count, param);
	} else {
		for (; pUnitDefinition->charIndex && count > 0; ++pUnitDefinition) {
			if (ShouldUNITBeLoaded(pUnitDefinition, param) != TRUE)
				continue;

			if (!TryPrepareEventUnitMovement(proc, pUnitDefinition->xPosition, pUnitDefinition->yPosition)) {
				proc->pUnitLoadData = pUnitDefinition;
				proc->unitLoadCount = count;
				return;
			}

			LoadUnit_800F704(pUnitDefinition, something, FALSE, r3);
			count--;
		}
	}

	proc->unitLoadCount = 0;
	proc->idk4E         = 0;
	proc->pCallback     = NULL;
}

void sub_800F698(const struct UnitDefinition* def, s16 count, u8 param) {
	s8 r3 = FALSE;

	if (param == 2)
		r3 = TRUE;

	for (; def->charIndex && count > 0; ++def) {
		if (ShouldUNITBeLoaded(def, param) != TRUE)
			continue;

		LoadUnit_800F704(def, 1, TRUE, r3);
		count--;
	}
}

void LoadUnit_800F704(const struct UnitDefinition* def, u16 b, s8 quiet, s8 d) {
	struct Unit* unit;

	u8 allegianceLookup[3];
	memcpy(allegianceLookup, gUnknown_080D793C, sizeof allegianceLookup);

	// u8 allegianceLookup[] = { 0x00, 0x40, 0x80 }; // TODO

	if (def->allegiance == 0) {
		unit = GetUnitFromCharIdAndFaction(def->charIndex, 0x00);
	} else {
		unit = GetUnitFromCharIdAndFaction(def->charIndex, 0x00);

		if (unit) {
			UnitChangeFaction(unit, allegianceLookup[def->allegiance]);
			unit = GetUnitFromCharId(def->charIndex);
		}
	}

	if (!unit) {
		unit = LoadUnit(def);

		if ((d == 1) && (def->allegiance == 0))
			unit->state |= 0x00400000;
	} else if (def->allegiance == 0) {
		s8 x, y;

		unit->state &= ~0x00000002;

		if (d == 1) {
			if (unit->state & 0x00000004)
				unit->state |= 0x00400000;
		} else {
			if (unit->state & 0x00400000)
				unit->state &= ~0x00400000;
		}

		GetPreferredPositionForUNIT(def, &x, &y, 0);

		if ((s8)(unit->xPos) == x && (s8)(unit->yPos) == y)
			b &= ~0x0001;
	}

	unit->xPos = def->xPosition;
	unit->yPos = def->yPosition;

	if (def->allegiance == 2 && unit->pCharacterData->number >= 0x3C) {
		if (!gUnknown_0202BCF0.unk42_6) {
			if (!(gUnknown_0202BCF0.chapterStateBits & 0x40))
				UnitApplyBonusLevels(
					unit,
					-GetROMChapterStruct(gUnknown_0202BCF0.chapterIndex)->easyModeLevelMalus
				);
			else
				goto hard_mode;
		} else {
			if (gUnknown_0202BCF0.chapterStateBits & 0x40)
			hard_mode:
				UnitApplyBonusLevels(
					unit,
					GetROMChapterStruct(gUnknown_0202BCF0.chapterIndex)->difficultModeLevelBonus
				);
			else
				UnitApplyBonusLevels(
					unit,
					-GetROMChapterStruct(gUnknown_0202BCF0.chapterIndex)->normalModeLevelMalus
				);
		}
	}

	sub_800F8A8(
		unit,
		def,
		b,
		quiet
	);
}

void sub_800F8A8(struct Unit* unit, const struct UnitDefinition* unitDefition, u16 flags, s8 unk) {
	if (!unit)
		return;

	if (unitDefition->sumFlag == 1)
		flags |= 0x0002;

	if (!unitDefition->redaCount) {
		MoveUnit_(unit, unitDefition->xPosition, unitDefition->yPosition, flags);
		return;
	}

	if (unk == 1 || (unit->state & 0x00000080))
		sub_8079FA8(unit, unitDefition->redas, unitDefition->redaCount, flags);
	else
		sub_8079D74(unit, unitDefition->redas, unitDefition->redaCount, flags);
}

#if NONMATCHING

struct UnitDefinition* sub_800F914(const struct UnitDefinition* source, short count, u8 arg2, s8 arg3, s8 arg4) {
	struct UnitDefinition* result;

	u8  array[0x40];
	u16 arraySize = 0;

	struct {
		unsigned loBits, hiBits;
	} mask;

	u16 i = 0;

	if (arg2) {
		const struct UnitDefinition* itSource = source;

		for (; i < count; i++) {
			if (itSource->sumFlag) {
				array[arraySize] = i;
				arraySize++;
			}

			itSource++;
		}

		i = Div((arraySize * arg2) + 50, 100);
	}

	#define MASK_BIT_GET(i) (((i) < 0x20) ? (mask.loBits & (1 << (i))) : (mask.hiBits & (1 << ((i)-0x20))))
	#define MASK_BIT_SET(i) (((i) < 0x20) ? (mask.loBits |= (1 << (i))) : (mask.hiBits |= (1 << ((i)-0x20))))

	mask.loBits = 0;
	mask.hiBits = 0;

	while (i) {
		unsigned index = array[NextRN_N(arraySize)];

		if (!MASK_BIT_GET(index)) {
			MASK_BIT_SET(index);
			i--;
		}
	}

	result = end;

	for (i = 0; i < count; i++) {
		if (MASK_BIT_GET(i))
			continue;

		*result = source[i];
		result->sumFlag = FALSE;
		result++;
	}

	for (i = 0; i < count; i++) {
		if (!MASK_BIT_GET(i))
			continue;

		*result = source[i];
		result->sumFlag = TRUE;
		result++;
	}

	result->charIndex = 0; // marks the end of the unit block

	if (arg4 == TRUE) {
		result = end;

		for (i = 0; i < count; i++) {
			result->redaCount = 0;
			result->redas     = NULL;

			result++;
		}
	}

	result = end;

	if (arg3 == TRUE)
		sub_80125C0(result);

	#undef MASK_BIT_GET
	#undef MASK_BIT_SET

	return result;
}

#else // #if !NONMATCHING

__attribute__((naked))
struct UnitDefinition* sub_800F914(const struct UnitDefinition* source, short count, u8 arg2, s8 arg3, s8 arg4) {
	asm(
		".syntax unified\n"

		"push {r4, r5, r6, r7, lr}\n"
		"mov r7, sl\n"
		"mov r6, r9\n"
		"mov r5, r8\n"
		"push {r5, r6, r7}\n"
		"sub sp, #0x60\n"
		"adds r6, r0, #0\n"
		"ldr r0, [sp, #0x80]\n"
		"lsls r1, r1, #0x10\n"
		"lsrs r1, r1, #0x10\n"
		"mov sl, r1\n"
		"lsls r2, r2, #0x18\n"
		"lsrs r2, r2, #0x18\n"
		"mov ip, r2\n"
		"lsls r3, r3, #0x18\n"
		"lsrs r3, r3, #0x18\n"
		"str r3, [sp, #0x40]\n"
		"lsls r0, r0, #0x18\n"
		"lsrs r0, r0, #0x18\n"
		"str r0, [sp, #0x44]\n"
		"movs r0, #0\n"
		"mov r8, r0\n"
		"movs r3, #0\n"
		"cmp r2, #0\n"
		"beq _0800F994\n"
		"str r6, [sp, #0x54]\n"
		"lsls r0, r1, #0x10\n"
		"asrs r0, r0, #0x10\n"
		"cmp r3, r0\n"
		"bge _0800F982\n"
		"movs r1, #0x40\n"
		"mov r9, r1\n"
		"adds r7, r0, #0\n"
	"_0800F956:\n"
		"ldr r2, [sp, #0x54]\n"
		"ldrb r1, [r2, #5]\n"
		"mov r0, r9\n"
		"ands r0, r1\n"
		"cmp r0, #0\n"
		"beq _0800F972\n"
		"mov r0, sp\n"
		"add r0, r8\n"
		"strb r3, [r0]\n"
		"mov r0, r8\n"
		"adds r0, #1\n"
		"lsls r0, r0, #0x10\n"
		"lsrs r0, r0, #0x10\n"
		"mov r8, r0\n"
	"_0800F972:\n"
		"ldr r4, [sp, #0x54]\n"
		"adds r4, #0x14\n"
		"str r4, [sp, #0x54]\n"
		"adds r0, r3, #1\n"
		"lsls r0, r0, #0x10\n"
		"lsrs r3, r0, #0x10\n"
		"cmp r3, r7\n"
		"blt _0800F956\n"
	"_0800F982:\n"
		"mov r5, ip\n"
		"mov r0, r8\n"
		"muls r0, r5, r0\n"
		"adds r0, #0x32\n"
		"movs r1, #0x64\n"
		"bl Div\n"
		"lsls r0, r0, #0x10\n"
		"lsrs r3, r0, #0x10\n"
	"_0800F994:\n"
		"movs r0, #0\n"
		"str r0, [sp, #0x58]\n"
		"str r0, [sp, #0x5c]\n"
		"mov r1, sl\n"
		"lsls r1, r1, #0x10\n"
		"mov r9, r1\n"
		"ldr r2, [sp, #0x44]\n"
		"lsls r2, r2, #0x18\n"
		"str r2, [sp, #0x4c]\n"
		"ldr r4, [sp, #0x40]\n"
		"lsls r4, r4, #0x18\n"
		"str r4, [sp, #0x48]\n"
		"cmp r3, #0\n"
		"beq _0800FA0C\n"
		"movs r7, #1\n"
	"_0800F9B2:\n"
		"mov r0, r8\n"
		"str r3, [sp, #0x50]\n"
		"bl NextRN_N\n"
		"lsls r0, r0, #0x10\n"
		"lsrs r2, r0, #0x10\n"
		"mov r5, sp\n"
		"adds r0, r5, r2\n"
		"ldrb r2, [r0]\n"
		"ldr r3, [sp, #0x50]\n"
		"cmp r2, #0x1f\n"
		"bhi _0800F9D4\n"
		"adds r0, r7, #0\n"
		"lsls r0, r2\n"
		"ldr r1, [sp, #0x58]\n"
		"ands r0, r1\n"
		"b _0800F9E0\n"
	"_0800F9D4:\n"
		"adds r1, r2, #0\n"
		"subs r1, #0x20\n"
		"adds r0, r7, #0\n"
		"lsls r0, r1\n"
		"ldr r4, [sp, #0x5c]\n"
		"ands r0, r4\n"
	"_0800F9E0:\n"
		"cmp r0, #0\n"
		"bne _0800FA08\n"
		"cmp r2, #0x1f\n"
		"bhi _0800F9F4\n"
		"adds r0, r7, #0\n"
		"lsls r0, r2\n"
		"ldr r5, [sp, #0x58]\n"
		"orrs r5, r0\n"
		"str r5, [sp, #0x58]\n"
		"b _0800FA02\n"
	"_0800F9F4:\n"
		"adds r1, r2, #0\n"
		"subs r1, #0x20\n"
		"adds r0, r7, #0\n"
		"lsls r0, r1\n"
		"ldr r1, [sp, #0x5c]\n"
		"orrs r1, r0\n"
		"str r1, [sp, #0x5c]\n"
	"_0800FA02:\n"
		"subs r0, r3, #1\n"
		"lsls r0, r0, #0x10\n"
		"lsrs r3, r0, #0x10\n"
	"_0800FA08:\n"
		"cmp r3, #0\n"
		"bne _0800F9B2\n"
	"_0800FA0C:\n"
		"str r6, [sp, #0x54]\n"
		"ldr r6, _0800FA34  @ end\n"
		"movs r3, #0\n"
		"mov r2, r9\n"
		"asrs r0, r2, #0x10\n"
		"mov ip, r6\n"
		"cmp r3, r0\n"
		"bge _0800FA70\n"
		"movs r4, #0x41\n"
		"negs r4, r4\n"
		"mov sl, r4\n"
		"mov r8, r0\n"
	"_0800FA24:\n"
		"cmp r3, #0x1f\n"
		"bhi _0800FA38\n"
		"movs r0, #1\n"
		"lsls r0, r3\n"
		"ldr r5, [sp, #0x58]\n"
		"ands r0, r5\n"
		"b _0800FA44\n"
		".align 2, 0\n"
	"_0800FA34: .4byte end\n"
	"_0800FA38:\n"
		"adds r1, r3, #0\n"
		"subs r1, #0x20\n"
		"movs r0, #1\n"
		"lsls r0, r1\n"
		"ldr r1, [sp, #0x5c]\n"
		"ands r0, r1\n"
	"_0800FA44:\n"
		"cmp r0, #0\n"
		"bne _0800FA66\n"
		"lsls r0, r3, #2\n"
		"adds r0, r0, r3\n"
		"lsls r0, r0, #2\n"
		"adds r1, r6, #0\n"
		"ldr r2, [sp, #0x54]\n"
		"adds r0, r0, r2\n"
		"ldm r0!, {r2, r4, r5}\n"
		"stm r1!, {r2, r4, r5}\n"
		"ldm r0!, {r4, r5}\n"
		"stm r1!, {r4, r5}\n"
		"ldrb r1, [r6, #5]\n"
		"mov r0, sl\n"
		"ands r0, r1\n"
		"strb r0, [r6, #5]\n"
		"adds r6, #0x14\n"
	"_0800FA66:\n"
		"adds r0, r3, #1\n"
		"lsls r0, r0, #0x10\n"
		"lsrs r3, r0, #0x10\n"
		"cmp r3, r8\n"
		"blt _0800FA24\n"
	"_0800FA70:\n"
		"movs r3, #0\n"
		"mov r1, r9\n"
		"asrs r0, r1, #0x10\n"
		"cmp r3, r0\n"
		"bge _0800FACA\n"
		"movs r2, #1\n"
		"mov sl, r2\n"
		"mov r8, r0\n"
	"_0800FA80:\n"
		"cmp r3, #0x1f\n"
		"bhi _0800FA92\n"
		"mov r0, sl\n"
		"lsls r0, r3\n"
		"ldr r4, [sp, #0x58]\n"
		"ands r0, r4\n"
		"cmp r0, #0\n"
		"bne _0800FAA2\n"
		"b _0800FAC0\n"
	"_0800FA92:\n"
		"adds r1, r3, #0\n"
		"subs r1, #0x20\n"
		"mov r0, sl\n"
		"lsls r0, r1\n"
		"ldr r5, [sp, #0x5c]\n"
		"ands r0, r5\n"
		"cmp r0, #0\n"
		"beq _0800FAC0\n"
	"_0800FAA2:\n"
		"lsls r0, r3, #2\n"
		"adds r0, r0, r3\n"
		"lsls r0, r0, #2\n"
		"adds r1, r6, #0\n"
		"ldr r2, [sp, #0x54]\n"
		"adds r0, r0, r2\n"
		"ldm r0!, {r2, r4, r5}\n"
		"stm r1!, {r2, r4, r5}\n"
		"ldm r0!, {r4, r5}\n"
		"stm r1!, {r4, r5}\n"
		"ldrb r0, [r6, #5]\n"
		"movs r1, #0x40\n"
		"orrs r0, r1\n"
		"strb r0, [r6, #5]\n"
		"adds r6, #0x14\n"
	"_0800FAC0:\n"
		"adds r0, r3, #1\n"
		"lsls r0, r0, #0x10\n"
		"lsrs r3, r0, #0x10\n"
		"cmp r3, r8\n"
		"blt _0800FA80\n"
	"_0800FACA:\n"
		"movs r0, #0\n"
		"strb r0, [r6]\n"
		"ldr r1, [sp, #0x4c]\n"
		"asrs r0, r1, #0x18\n"
		"cmp r0, #1\n"
		"bne _0800FAF6\n"
		"mov r6, ip\n"
		"movs r3, #0\n"
		"mov r2, r9\n"
		"asrs r0, r2, #0x10\n"
		"cmp r3, r0\n"
		"bge _0800FAF6\n"
		"movs r2, #0\n"
		"adds r1, r0, #0\n"
	"_0800FAE6:\n"
		"strb r2, [r6, #7]\n"
		"str r2, [r6, #8]\n"
		"adds r6, #0x14\n"
		"adds r0, r3, #1\n"
		"lsls r0, r0, #0x10\n"
		"lsrs r3, r0, #0x10\n"
		"cmp r3, r1\n"
		"blt _0800FAE6\n"
	"_0800FAF6:\n"
		"mov r6, ip\n"
		"ldr r4, [sp, #0x48]\n"
		"asrs r0, r4, #0x18\n"
		"cmp r0, #1\n"
		"bne _0800FB06\n"
		"adds r0, r6, #0\n"
		"bl sub_80125C0\n"
	"_0800FB06:\n"
		"adds r0, r6, #0\n"
		"add sp, #0x60\n"
		"pop {r3, r4, r5}\n"
		"mov r8, r3\n"
		"mov r9, r4\n"
		"mov sl, r5\n"
		"pop {r4, r5, r6, r7}\n"
		"pop {r1}\n"
		"bx r1\n"

		".syntax divided\n"
	);
}

#endif // !NONMATCHING

u8 Event2B_(struct EventEngineProc* proc) {
	const struct {
		u8 params;
		u8 code;
		u16 evArgument;
	}* const ev = (const void*)(proc->pEventCurrent);

	u8    subcode  = ev->params & 0xF;
	short argument = ev->evArgument;

	if (argument < 0)
		argument = gEventSlots[2];

	switch (subcode) {

	case 0: {
		proc->unitLoadCount = argument;
		break;
	}

	case 1: {
		if (argument < 0)
			argument = 0;

		if (argument > 100)
			argument = 100;

		proc->idk4E = argument;

		break;
	}

	case 2: {
		proc->unk4F_7 = TRUE;
		break;
	}

	} // switch (subcode)

	return EVC_ADVANCE_CONTINUE;
}

u8 Event2C_LoadUnits(struct EventEngineProc* proc) {
	const struct {
		u8 params;
		u8 code;
		u8 evArgument;
		const struct UnitDefinition* pUnitDefinitions;
	}* const ev = (const void*)(proc->pEventCurrent);

	u8 subcode  = ev->params & 0xF;
	u8 argument = ev->evArgument;
	const struct UnitDefinition* ud = ev->pUnitDefinitions;

	u16 count = proc->unitLoadCount;

	switch (subcode) {

	case 1:
		argument = 2;

	case 0:
	case 2:
		if ((int)(ud) < 0)
			ud = (const struct UnitDefinition*)(gEventSlots[2]);

		break;

	case 3:
		ud = sub_80833B0();

		break;

	} // switch (subcode)

	if (count == 0)
		count = sub_800F50C(ud);

	ud = sub_800F914(ud, count, proc->idk4E, subcode == 2, proc->unk4F_7);

	BmMapFill(gBmMapUnk, 0);

	if (EVENT_IS_SKIPPING(proc) || (proc->evStateBits & EV_STATE_FADEDIN)) {
		sub_800F698(ud, count, argument);

		proc->unitLoadCount = 0;
		proc->idk4E = 0;
	} else {
		proc->pUnitLoadData = ud;
		proc->unitLoadCount = count;
		proc->unitLoadParameter = argument;

		proc->pCallback = sub_800F5B8;
		proc->pCallback(proc);
	}

	proc->unk4F_7 = FALSE;
	return EVC_ADVANCE_YIELD;
}

u8 TryPrepareEventUnitMovement(struct EventEngineProc* proc, int x, int y) {
	if (proc->evStateBits & EV_STATE_UNITCAM) {
		if (Proc_Find(gUnknown_0859A548))
			return FALSE; // Camera proc already exists

		if (EnsureCameraOntoPosition((struct Proc*)(proc), x, y))
			return FALSE; // Failed to start camera movement
	}

	if (!MU_CanStart())
		return FALSE; // No room to make MU for the moving unit

	return TRUE; // Yay!
}

unsigned GetSomeEventEngineMoveRelatedBitfield(struct EventEngineProc* proc, s8 unk) {
	u16 result = 0;

	// TODO: use some enum constants

	if (unk == TRUE)
		result = 1;

	if (proc->evStateBits & EV_STATE_UNITCAM)
		result = result | 4;

	if (proc->evStateBits & EV_STATE_0040)
		result = result | 8;

	return result;
}

u8 Event2D_(struct EventEngineProc* proc) {
	const struct {
		u8 params;
		u8 code;
		u16 evArgument;
	}* const ev = (const void*)(proc->pEventCurrent);

	int palId = ev->evArgument;

	sub_800BCDC(palId);
	proc->mapSpritePalIdOverride = palId;

	return EVC_ADVANCE_CONTINUE;
}

/*
struct Unit* GetUnitStruct(int id);

u8 Event2E_CheckAt(struct EventEngineProc* proc) {
	const struct {
		u8 params;
		u8 code;
		u16 evArgument;
	}* const ev = (const void*)(proc->pEventCurrent);

	u8 subcode = ev->params & 0xF;
	struct Unit* unit;
	int x, y; {
		// I hate this
		unsigned read = proc->pEventCurrent[1];
		x = *(const u8*)(proc->pEventCurrent + 1);
		y = read >> 8;
	}

	switch (subcode) {

	case 0: {
		unsigned unitId = gBmMapUnit[y][x];

		if (unitId == 0) {
			gEventSlots[0xC] = 0;
			return EVC_ADVANCE_CONTINUE;
		}

		unit = GetUnitStruct(unitId);
		break;
	}

	case 1: {
		unit = gActiveUnit;
		break;
	}

	} // switch (subcode)

	if (!unit) {
		gEventSlots[0xC] = 0;
		return EVC_ADVANCE_CONTINUE;
	}

	gEventSlots[0xC] = unit->pCharacterData->index;
	return EVC_ADVANCE_CONTINUE;
}
*/
