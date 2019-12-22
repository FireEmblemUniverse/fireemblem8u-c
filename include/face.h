#ifndef GUARD_FACE_H
#define GUARD_FACE_H

// For now, this is an orphan header
// as face.s hasn't been decompiled yet
// but other things require things defined here

// I'm not putting it in types.h/varibales.h because
// it would force a dependency on proc.h.
// -Stan

#include "proc.h"

struct PortraitData
{
    /* 00 */ const void* pPortraitGraphics;
    /* 04 */ const void* pMiniPortraitGraphics;
    /* 08 */ const u16* pPortraitPalette;
    /* 0C */ const void* unk0C;
    /* 10 */ const void* unk10;
    /* 14 */ const void* unk14;
    /* 18 */ u8 blinkBehaviorKind;
    /* More */
};

struct FaceProc
{
    /* 00 */ PROC_HEADER;

    /* 2C */ const struct PortraitData* portrait;
    /* 30 */ u32 state;
    /* 34 */ short xDisplay;
    /* 36 */ short yDisplay;
    /* 38 */ const u16* sprite;
    /* 3C */ u16 tileref;
    /* 3E */ u16 portraitId;
    /* 40 */ u8 faceId;
    /* 41 */ u8 spriteDepth;

    /* 44 */ struct Proc* unk44;
    /* 48 */ struct Proc* blinkCnt;
};

extern struct FaceProc* gUnknown_03004980[4];

#endif // GUARD_FACE_H
