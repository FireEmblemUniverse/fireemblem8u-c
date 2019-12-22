#ifndef GUARD_AP_H
#define GUARD_AP_H


struct APHandle {
    /* 00 */ const u16* pDefinition;      // Pointer to Definition Root
    /* 04 */ const u16* pFrameData;       // Pointer to Frame Data Array
    /* 08 */ const u16* pAnimDataStart;   // Pointer to Current Anim Data (Start, where we go back on loop)
    /* 0C */ const u16* pAnimDataCurrent; // Pointer to Current Anim Data (Cursor, where we are now)
    /* 10 */ const u16* pCurrentObjData;  // Pointer to Current Obj Data
    /* 14 */ const u16* pCurrentRotScale; // Pointer to Current Rot/Scale Data
    /* 18 */ s16 frameTimer;              // Cycle Timer
    /* 1A */ u16 frameInterval;           // Cycle Time Step (0x100 is one frame)
    /* 1C */ u16 subframeTimer;           // Sub frame time offset or something
    /* 1E */ u16 objLayer;                // HiObj layer
    /* 20 */ u8  gfxNeedsUpdate;          // bool defining whether gfx needs update
    /* 21 */ u8  rotScaleIndex;           // Rotation/Scale OAM Index
    /* 22 */ u16 tileBase;                // OAM Extra Data (Tile Index Root & OAM2 Stuff)
    /* 24 */ const void* pGraphics;       // Pointer to graphics (if any)
};


void AP_ClearAll();
struct APHandle* AP_Create(const void* apDefinition, u16 aObjNode);
void AP_Delete(struct APHandle* handle);
s8   AP_Update(struct APHandle* handle, int x, int y);
void AP_Display(struct APHandle*, int, int);
s8   AP_ExecFrame(struct APHandle*);
void AP_QueueObjRotScale(struct APHandle*);
void AP_SwitchAnimation(struct APHandle* handle, int index);
void AP_SetDefinition(struct APHandle* handle, const u16* definition);
void AP_QueueObjGraphics(struct APHandle*);
void AP_LoadDefinition(struct APHandle*, const u16*);
void AP_ExecDummyFrame(struct APHandle*);
void AP_Init(struct APHandle*, const u16*, u16);
struct APHandle* AP_Find(const u16* definition);
struct APProc* APProc_Create(const void* apDefinition, int xPos, int yPos, int tileBase, int anim, u16 aObjNode);
// ??? APProc_OnUpdate(???);
// ??? APProc_OnEnd(???);
// ??? APProc_SetParameters(???);
// ??? APProc_Delete(???);
void APProc_DeleteAll(void);
// ??? APProc_Exists(???);

#endif // GUARD_AP_H
