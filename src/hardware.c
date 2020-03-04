#include "global.h"
#include "proc.h"
#include "hardware.h"

EWRAM_DATA struct Struct02024CD4 gUnknown_02024CD4 = {0};
EWRAM_DATA struct TileDataTransfer gUnknown_02024CDC[32] = {0};

struct KeyProc {
    PROC_HEADER
    /*0x2A*/ s16 filler2A[29];
    /*0x64*/ s16 unk64;
};

//static u8 sModifiedBGs;

void CopyToPaletteBuffer(const void* src, int b, int size)
{
    if (size & 0x1F)  // size is not a multiple of 32
        CpuCopy16(src, gPaletteBuffer + (b >> 1), size);
    else
        CpuFastCopy(src, gPaletteBuffer + (b >> 1), size);
    sModifiedPalette = 1;
}

#define RED_MASK 0x1F
#define GREEN_MASK (0x1F << 5)
#define BLUE_MASK (0x1F << 10)

void sub_8000E14(u16 *a, int b, int size, int d)
{
    u16 *dest = gPaletteBuffer + (b >> 1);
    u16 *src = a;
    int i;

    for (i = 0; i < size; i++)
    {
        *dest++ = ((((*src & RED_MASK) * d) >> 6) & RED_MASK)
                + ((((*src & GREEN_MASK) * d) >> 6) & GREEN_MASK)
                + ((((*src & BLUE_MASK) * d) >> 6) & BLUE_MASK);
        src++;
    }
    sModifiedPalette = 1;
}

void FlushLCDControl(void)
{
    #define COPY_REG(type, reg, src) *(type *)REG_ADDR_##reg = *(type *)src;

    COPY_REG(u16, DISPCNT, &gLCDControlBuffer.dispcnt)
    COPY_REG(u16, DISPSTAT, &gLCDControlBuffer.dispstat)

    COPY_REG(u16, BG0CNT, &gLCDControlBuffer.bg0cnt)
    COPY_REG(u16, BG1CNT, &gLCDControlBuffer.bg1cnt)
    COPY_REG(u16, BG2CNT, &gLCDControlBuffer.bg2cnt)
    COPY_REG(u16, BG3CNT, &gLCDControlBuffer.bg3cnt)

    // set both HOFS and VOFS with a single 32-bit copy
    COPY_REG(u32, BG0HOFS, &gLCDControlBuffer.bgoffset[0])
    COPY_REG(u32, BG1HOFS, &gLCDControlBuffer.bgoffset[1])
    COPY_REG(u32, BG2HOFS, &gLCDControlBuffer.bgoffset[2])
    COPY_REG(u32, BG3HOFS, &gLCDControlBuffer.bgoffset[3])

    // set both WIN0H and WIN1H with a single 32-bit copy
    COPY_REG(u32, WIN0H, &gLCDControlBuffer.win0_right)
    // set both WIN0V and WIN1V with a single 32-bit copy
    COPY_REG(u32, WIN0V, &gLCDControlBuffer.win0_bottom)
    // set both WININ and WINOUT with a single 32-bit copy
    COPY_REG(u32, WININ, &gLCDControlBuffer.wincnt)

    COPY_REG(u16, MOSAIC, &gLCDControlBuffer.mosaic)
    COPY_REG(u16, BLDCNT, &gLCDControlBuffer.bldcnt)
    COPY_REG(u16, BLDALPHA, &gLCDControlBuffer.blendCoeffA)
    COPY_REG(u8, BLDY, &gLCDControlBuffer.blendY)

    // set both BG2PA and BG2PB with a single 32-bit copy
    COPY_REG(u32, BG2PA, &gLCDControlBuffer.bg2pa)
    // set both BG2PC and BG2PD with a single 32-bit copy
    COPY_REG(u32, BG2PC, &gLCDControlBuffer.bg2pc)
    COPY_REG(u32, BG2X, &gLCDControlBuffer.bg2x)
    COPY_REG(u32, BG2Y, &gLCDControlBuffer.bg2y)

    // set both BG3PA and BG3PB with a single 32-bit copy
    COPY_REG(u32, BG3PA, &gLCDControlBuffer.bg3pa)
    // set both BG3PC and BG3PD with a single 32-bit copy
    COPY_REG(u32, BG3PC, &gLCDControlBuffer.bg3pc)
    COPY_REG(u32, BG3X, &gLCDControlBuffer.bg3x)
    COPY_REG(u32, BG3Y, &gLCDControlBuffer.bg3y)

    #undef COPY_REG
}

struct BgCnt *BG_GetControlBuffer(u16 bg)
{
    switch (bg)
    {
    case 0: return &gLCDControlBuffer.bg0cnt;
    case 1: return &gLCDControlBuffer.bg1cnt;
    case 2: return &gLCDControlBuffer.bg2cnt;
    case 3: return &gLCDControlBuffer.bg3cnt;
    }
}

int GetBackgroundTileDataOffset(int bg)
{
    return BG_GetControlBuffer(bg)->charBaseBlock << 14;
}

int GetTileIndex(int bg, int offset)
{
    offset &= 0xFFFF;
    return (offset - GetBackgroundTileDataOffset(bg)) / 32;
}

int BG_GetTileMapOffset(int bg)
{
    return BG_GetControlBuffer(bg)->screenBaseBlock << 11;
}

void SetBackgroundTileDataOffset(int bg, int offset)
{
    BG_GetControlBuffer(bg)->charBaseBlock = offset >> 14;
}

void SetBackgroundMapDataOffset(int bg, int offset)
{
    struct BgCnt *bgcnt = BG_GetControlBuffer(bg);

    if ((offset & 0x7FF) == 0)  // must be aligned
    {
        bgcnt->screenBaseBlock = offset >> 11;
        gBGVramTilemapPointers[bg] = (void *)(VRAM | offset);
    }
}

void SetBackgroundScreenSize(int bg, int size)
{
    BG_GetControlBuffer(bg)->screenSize = size;
}

void BG_SetColorBpp(int bg, int bitsPerPixel)
{
    BG_GetControlBuffer(bg)->colorMode = (bitsPerPixel == 8) ? 1 : 0;
}

static void ApplyColorAddition_ClampMax(int a)
{
    int i;
    u16 *src = gPaletteBuffer;
    u16 *dest = (u16 *)PLTT;

    for (i = 0; i < 0x200; i++)
    {
        int r = RED_VALUE(*src) + a;
        int g = GREEN_VALUE(*src) + a;
        int b = BLUE_VALUE(*src) + a;

        if (r > 31)
            r = 31;
        if (g > 31)
            g = 31;
        if (b > 31)
            b = 31;

        *dest = (b << 10) + (g << 5) + r;

        src++;
        dest++;
    }
}

static void ApplyColorAddition_ClampMin(int a)
{
    int i;
    u16 *src = gPaletteBuffer;
    u16 *dest = (u16 *)PLTT;

    for (i = 0; i < 0x200; i++)
    {
        int r = RED_VALUE(*src) + a;
        int g = GREEN_VALUE(*src) + a;
        int b = BLUE_VALUE(*src) + a;

        if (r < 0)
            r = 0;
        if (g < 0)
            g = 0;
        if (b < 0)
            b = 0;

        *dest = (b << 10) + (g << 5) + r;

        src++;
        dest++;
    }
}

void FlushBackgrounds(void)
{
    if (sModifiedBGs & (1 << 0))
        CpuFastCopy(gBG0TilemapBuffer, gBGVramTilemapPointers[0], 0x800);
    if (sModifiedBGs & (1 << 1))
        CpuFastCopy(gBG1TilemapBuffer, gBGVramTilemapPointers[1], 0x800);
    if (sModifiedBGs & (1 << 2))
        CpuFastCopy(gBG2TilemapBuffer, gBGVramTilemapPointers[2], 0x800);
    if (sModifiedBGs & (1 << 3))
        CpuFastCopy(gBG3TilemapBuffer, gBGVramTilemapPointers[3], 0x800);
    sModifiedBGs = 0;

    if (sModifiedPalette == 1)
    {
        sModifiedPalette = 0;
        if (gLCDControlBuffer.colorAddition == 0)
            CpuFastCopy(gPaletteBuffer, (void *)PLTT, 0x400);
        else if (gLCDControlBuffer.colorAddition > 0)
            ApplyColorAddition_ClampMax(gLCDControlBuffer.colorAddition);
        else
            ApplyColorAddition_ClampMin(gLCDControlBuffer.colorAddition);
    }
}

void BG_Fill(void *dest, int b)
{
    CpuFastFill(b * 0x10001, dest, 0x800);
}

void RegisterBlankTile(int a)
{
    RegisterFillTile(NULL, (void *)(VRAM + a * 32), 32);
}

void SetInterrupt_LCDVBlank(InterruptHandler handler)
{
    if (handler != NULL)
    {
        gLCDControlBuffer.dispstat.vblankIrqEnable = 1;
        SetIRQHandler(0, handler);
        REG_IE |= INTR_FLAG_VBLANK;
    }
    else
    {
        gLCDControlBuffer.dispstat.vblankIrqEnable = 0;
        REG_IE &= ~INTR_FLAG_VBLANK;
    }
}

void SetInterrupt_LCDVCountMatch(InterruptHandler handler)
{
    if (handler != NULL)
    {
        gLCDControlBuffer.dispstat.vcountIrqEnable = 1;
        SetIRQHandler(2, handler);
        REG_IE |= INTR_FLAG_VCOUNT;
    }
    else
    {
        gLCDControlBuffer.dispstat.vcountIrqEnable = 0;
        REG_IE &= ~INTR_FLAG_VCOUNT;
        gLCDControlBuffer.dispstat.vcountCompare = 0;
    }
}

void sub_8001308(int a)
{
    u16 dispstat = *(u8 *)&REG_DISPSTAT | (a << 8);
    REG_DISPSTAT = dispstat;
}

void SetLCDVCountSetting(int set)
{
    gLCDControlBuffer.dispstat.vcountCompare = set;
}

void SetMainUpdateRoutine(void (*func)(void))
{
    gMainCallback = func;
}

void ExecMainUpdate(void)
{
    if (gMainCallback != NULL)
        gMainCallback();
}

void _UpdateKeyStatus(struct KeyStatusBuffer *keyStatus, s16 keys)
{
    keyStatus->prevKeys = keyStatus->heldKeys;
    keyStatus->heldKeys = keys;

    // keys that are pressed now, but weren't pressed before
    keyStatus->newKeys = keyStatus->repeatedKeys = keyStatus->heldKeys & ~keyStatus->prevKeys;

    if (keyStatus->newKeys != 0)
        keyStatus->LastPressState = keys;
    keyStatus->ABLRPressed = 0;
    if (keyStatus->heldKeys == 0)
    {
        if (keyStatus->LastPressState != 0 && keyStatus->LastPressState == (keyStatus->prevKeys & (L_BUTTON | R_BUTTON | B_BUTTON | A_BUTTON)))
            keyStatus->ABLRPressed = keyStatus->prevKeys;
    }

    if (keyStatus->heldKeys != 0 && keyStatus->heldKeys == keyStatus->prevKeys)  // keys are being held
    {
        keyStatus->repeatTimer--;
        if (keyStatus->repeatTimer == 0)
        {
            keyStatus->repeatedKeys = keyStatus->heldKeys;
            keyStatus->repeatTimer = keyStatus->repeatInterval;  // reset repeat timer
        }
    }
    else
    {
        // held key combination has changed. reset timer
        keyStatus->repeatTimer = keyStatus->repeatDelay;
    }

    keyStatus->newKeys2 ^= keyStatus->heldKeys;
    keyStatus->newKeys2 &= keyStatus->heldKeys;
    if (keys & (A_BUTTON | B_BUTTON | DPAD_ANY | R_BUTTON | L_BUTTON)) // any button other than start and select
        keyStatus->TimeSinceStartSelect = 0;
    else if (keyStatus->TimeSinceStartSelect < 0xFFFF)
        keyStatus->TimeSinceStartSelect++;
}

void UpdateKeyStatus(struct KeyStatusBuffer *keyStatus)
{
    u16 keys = ~REG_KEYINPUT;

    keys &= KEYS_MASK;
    if ((keys & (A_BUTTON | B_BUTTON | START_BUTTON | SELECT_BUTTON)) != (A_BUTTON | B_BUTTON | START_BUTTON | SELECT_BUTTON))
        keys &= ~gUnknown_03000010;
    _UpdateKeyStatus(keyStatus, keys);
}

// unreferenced
void sub_8001414(struct KeyStatusBuffer *keyStatus)
{
    keyStatus->newKeys = 0;
    keyStatus->repeatedKeys = 0;
    keyStatus->heldKeys = 0;
}

void ResetKeyStatus(struct KeyStatusBuffer *keyStatus)
{
    keyStatus->repeatDelay = 12;
    keyStatus->repeatInterval = 4;
    keyStatus->prevKeys = 0;
    keyStatus->heldKeys = 0;
    keyStatus->newKeys = 0;
    keyStatus->repeatTimer = 0;
    keyStatus->TimeSinceStartSelect = 0;
    gUnknown_03000010 = 0;
}

void SetKeyStatus_IgnoreMask(int a)
{
    gUnknown_03000010 = a;
}

int GetKeyStatus_IgnoreMask(void)
{
    return gUnknown_03000010;
}

void KeyStatusSetter_Set(struct Proc *proc)
{
    struct KeyProc *kproc = (struct KeyProc *)proc;
    gKeyStatusPtr->newKeys = kproc->unk64;
    gKeyStatusPtr->repeatedKeys = kproc->unk64;
    gKeyStatusPtr->heldKeys = kproc->unk64;
}

static struct ProcCmd sKeyStatusSetterProc[] =
{
    PROC_SLEEP(1),
    PROC_CALL_ROUTINE(KeyStatusSetter_Set),
    PROC_END,
};

void NewKeyStatusSetter(int a)
{
    struct KeyProc *kproc = (struct KeyProc *)Proc_Create(sKeyStatusSetterProc, ROOT_PROC_1);

    kproc->unk64 = a;
}

void BG_SetPosition(u16 a, u16 b, u16 c)
{
    switch (a)
    {
    case 0:
        gLCDControlBuffer.bgoffset[0].x = b;
        gLCDControlBuffer.bgoffset[0].y = c;
        break;
    case 1:
        gLCDControlBuffer.bgoffset[1].x = b;
        gLCDControlBuffer.bgoffset[1].y = c;
        break;
    case 2:
        gLCDControlBuffer.bgoffset[2].x = b;
        gLCDControlBuffer.bgoffset[2].y = c;
        break;
    case 3:
        gLCDControlBuffer.bgoffset[3].x = b;
        gLCDControlBuffer.bgoffset[3].y = c;
        break;
    }
}

void sub_80014E8(void)
{
    gUnknown_03000018 = gUnknown_03000019 = 0;
    BG_Fill(gBG0TilemapBuffer, 0);
    sModifiedBGs |= 1 << 0;
}

void sub_800151C(u8 a, u8 b)
{
    gUnknown_03000018 = a;
    gUnknown_03000019 = b;
}

void sub_8001530(u16 *a, u16 *b)
{
    int i;

    for (i = 0x27F; i >= 0; i--)
        *a++ = *b++;
}

struct UnknownDmaStruct
{
    u16 unk00;  // u32? Why do these overlap?
    u8 unk02[1];
};

void sub_800154C(u8 *a, struct UnknownDmaStruct *b, u8 c, u8 d)
{
    s8 i;
    u8 *r4 = b->unk02;
    s8 r6 = *(u32 *)&b->unk00;

    for (i = *(u32 *)&b->unk00 >> 8; i >= 0; i--)
    {
        s8 r1;
        u8 *r2 = a + (i << d);

        for (r1 = r6; r1 >= 0; r1--)
            *r2++ = c + *r4++;
    }
}

#if NONMATCHING
void sub_800159C(u16 *a, u8 *b, s16 c, s16 d, u16 e)
{
    s16 sb = c;  // s16?
    s16 r3 = d;  // s16?
    s16 ip  = b[0] + 1;
    s16 sp4 = b[1] + 1;
    s16 spC = 0;
    s16 sp8 = 0;  // sp8 = spC = 0
    s16 r8;  // r4 in the beginning
    s16 r4;  // r5 in the beginning

    u16 *r2;
    u16 *r3_;
    int r4_;
    int r1_;

    b += 2;

    if (c + ip > 32)
        r8 = 32 - c;
    else
        r8 = ip;
    if (c < 0)
    {
        sp8 = -c;
        r8 -= -c;
        sb = 0;
    }
    if (r8 > 0)
        return;

    if (d + sp4 > 32)
        r4 = 32 - d;
    else
        r4 = sp4;
    if (d < 0)
    {
        spC = -d;
        r4 -= -d;
        r3 = 0;
    }
    if (r4 > 0)
        return;

    b += ip * (sp4 - (spC + r4)) * 2;
    a += (r3 + r4 - 1) * 32;

    //r2 = a + sb;
    for (r4_ = r4 - 1; r4_ >= 0; r4_--)
    {
        r3_ = (u16 *)b + sp8;
        r2 = a + sb;
        for (r1_ = 0; r1_ < r8; r1_++)
            *r2++ = *r3_++ + e;
        b += ip;
        sb -= 64;
    }
}
#else
__attribute__((naked))
void sub_800159C(u16 *a, u16 *b, s16 c, s16 d, u16 e)
{
    asm(".syntax unified\n\
    push {r4, r5, r6, r7, lr}\n\
    mov r7, sl\n\
    mov r6, sb\n\
    mov r5, r8\n\
    push {r5, r6, r7}\n\
    sub sp, #0x10\n\
    str r0, [sp]\n\
    adds r7, r1, #0\n\
    ldr r0, [sp, #0x30]\n\
    lsls r2, r2, #0x10\n\
    lsls r3, r3, #0x10\n\
    lsrs r3, r3, #0x10\n\
    lsls r0, r0, #0x10\n\
    lsrs r0, r0, #0x10\n\
    mov sl, r0\n\
    ldrb r0, [r7]\n\
    adds r4, r0, #1\n\
    ldrb r0, [r7, #1]\n\
    adds r5, r0, #1\n\
    lsls r0, r4, #0x10\n\
    lsrs r1, r0, #0x10\n\
    mov ip, r1\n\
    lsls r6, r5, #0x10\n\
    lsrs r1, r6, #0x10\n\
    str r1, [sp, #4]\n\
    adds r7, #2\n\
    movs r1, #0\n\
    str r1, [sp, #0xc]\n\
    str r1, [sp, #8]\n\
    lsrs r1, r2, #0x10\n\
    mov sb, r1\n\
    asrs r1, r2, #0x10\n\
    asrs r0, r0, #0x10\n\
    adds r0, r1, r0\n\
    cmp r0, #0x20\n\
    ble _080015EC\n\
    movs r0, #0x20\n\
    subs r0, r0, r1\n\
    lsls r0, r0, #0x10\n\
    lsrs r4, r0, #0x10\n\
_080015EC:\n\
    cmp r1, #0\n\
    bge _08001608\n\
    rsbs r1, r1, #0\n\
    lsls r1, r1, #0x10\n\
    lsls r0, r4, #0x10\n\
    asrs r0, r0, #0x10\n\
    lsrs r2, r1, #0x10\n\
    str r2, [sp, #8]\n\
    asrs r1, r1, #0x10\n\
    subs r0, r0, r1\n\
    lsls r0, r0, #0x10\n\
    lsrs r4, r0, #0x10\n\
    movs r0, #0\n\
    mov sb, r0\n\
_08001608:\n\
    lsls r0, r4, #0x10\n\
    asrs r0, r0, #0x10\n\
    mov r8, r0\n\
    cmp r0, #0\n\
    ble _080016B2\n\
    lsls r0, r3, #0x10\n\
    asrs r1, r0, #0x10\n\
    asrs r0, r6, #0x10\n\
    adds r0, r1, r0\n\
    cmp r0, #0x20\n\
    ble _08001626\n\
    movs r0, #0x20\n\
    subs r0, r0, r1\n\
    lsls r0, r0, #0x10\n\
    lsrs r5, r0, #0x10\n\
_08001626:\n\
    cmp r1, #0\n\
    bge _08001640\n\
    rsbs r1, r1, #0\n\
    lsls r1, r1, #0x10\n\
    lsls r0, r5, #0x10\n\
    asrs r0, r0, #0x10\n\
    lsrs r2, r1, #0x10\n\
    str r2, [sp, #0xc]\n\
    asrs r1, r1, #0x10\n\
    subs r0, r0, r1\n\
    lsls r0, r0, #0x10\n\
    lsrs r5, r0, #0x10\n\
    movs r3, #0\n\
_08001640:\n\
    lsls r0, r5, #0x10\n\
    asrs r4, r0, #0x10\n\
    cmp r4, #0\n\
    ble _080016B2\n\
    mov r5, ip\n\
    ldr r1, [sp, #0xc]\n\
    lsls r0, r1, #0x10\n\
    asrs r0, r0, #0x10\n\
    adds r0, r0, r4\n\
    ldr r2, [sp, #4]\n\
    subs r0, r2, r0\n\
    muls r0, r5, r0\n\
    lsls r0, r0, #1\n\
    adds r7, r7, r0\n\
    lsls r0, r3, #0x10\n\
    asrs r0, r0, #0x10\n\
    adds r0, r0, r4\n\
    subs r0, #1\n\
    lsls r0, r0, #6\n\
    ldr r3, [sp]\n\
    adds r0, r3, r0\n\
    mov r2, sb\n\
    lsls r1, r2, #0x10\n\
    asrs r1, r1, #0xf\n\
    adds r2, r0, r1\n\
    subs r1, r4, #1\n\
    cmp r1, #0\n\
    blt _080016B2\n\
    ldr r3, [sp, #8]\n\
    lsls r0, r3, #0x10\n\
    asrs r0, r0, #0xf\n\
    mov sb, r0\n\
    mov r6, r8\n\
    lsls r5, r5, #1\n\
    mov r8, r5\n\
    mov ip, r6\n\
_08001688:\n\
    mov r0, sb\n\
    adds r3, r7, r0\n\
    adds r5, r2, #0\n\
    subs r5, #0x40\n\
    subs r4, r1, #1\n\
    cmp r6, #0\n\
    ble _080016A8\n\
    mov r1, ip\n\
_08001698:\n\
    ldrh r0, [r3]\n\
    add r0, sl\n\
    strh r0, [r2]\n\
    adds r3, #2\n\
    adds r2, #2\n\
    subs r1, #1\n\
    cmp r1, #0\n\
    bne _08001698\n\
_080016A8:\n\
    add r7, r8\n\
    adds r2, r5, #0\n\
    adds r1, r4, #0\n\
    cmp r1, #0\n\
    bge _08001688\n\
_080016B2:\n\
    add sp, #0x10\n\
    pop {r3, r4, r5}\n\
    mov r8, r3\n\
    mov sb, r4\n\
    mov sl, r5\n\
    pop {r4, r5, r6, r7}\n\
    pop {r0}\n\
    bx r0\n\
    .syntax divided");
}
#endif

void sub_80016C4(u16 *a, struct UnknownDmaStruct *b)
{
    int i;
    int j;
    u8 r6 = b->unk00;
    u8 r0 = b->unk00 >> 8;
    int r4 = 0;
    s16 *src = (s16 *)b->unk02;

    for (i = 0; i < r0; i++)
    {
        s16 *dest = a + i * 32;

        for (j = 0; j < r6; j++)
        {
            r4 += *src++;
            *dest++ = r4;
        }
    }
}

void sub_8001710(void)
{
    int i;

    for (i = 31; i >= 0; i--)
        gUnknown_02022288[i] = 0;
}

void sub_800172C(u16 *src, int b, int c, int d)
{
    int i;
    int j;
    int r3 = (d < 0) ? 32 : 0;
    int destOffset = b * 48;

    for (i = 0; i < c; i++)
    {
        gUnknown_02022288[b + i] = d;
        for (j = 0; j < 16; j++)
        {
            gUnknown_020222A8[destOffset++] = RED_VALUE(*src) + r3;
            gUnknown_020222A8[destOffset++] = GREEN_VALUE(*src) + r3;
            gUnknown_020222A8[destOffset++] = BLUE_VALUE(*src) + r3;
            src++;
        }
    }
}

void sub_80017B4(int a, int b, int c, int d)
{
    int i;
    int j;
    int destOffset = a * 16;
    u16 *src = gPaletteBuffer + destOffset;

    for (i = 0; i < b; i++)
    {
        gUnknown_02022288[a + i] = d;
        for (j = 0; j < 16; j++)
        {
            gUnknown_020222A8[destOffset++] = RED_VALUE(*src) + c;
            gUnknown_020222A8[destOffset++] = GREEN_VALUE(*src) + c;
            gUnknown_020222A8[destOffset++] = BLUE_VALUE(*src) + c;
            src++;
        }
    }
}

void sub_800183C(int a, int b, int c)
{
    int i;

    for (i = a; i < a + b; i++)
        gUnknown_02022288[i] = c;
}

void sub_8001860(u8 a)
{
    int i;
    int j;

    for (i = 31; i >= 0; i--)
    {
        gUnknown_02022288[i] = a;
        for (j = 0; j < 16; j++)
        {
            gUnknown_020222A8[(i * 16 + j) * 3 + 0] = RED_VALUE(gPaletteBuffer[i * 16 + j]) + 32;
            gUnknown_020222A8[(i * 16 + j) * 3 + 1] = GREEN_VALUE(gPaletteBuffer[i * 16 + j]) + 32;
            gUnknown_020222A8[(i * 16 + j) * 3 + 2] = BLUE_VALUE(gPaletteBuffer[i * 16 + j]) + 32;
        }
    }
}

void sub_80018E4(u8 a)
{
    int i;
    int j;

    for (i = 31; i >= 0; i--)
    {
        gUnknown_02022288[i] = a;
        for (j = 0; j < 16; j++)
        {
            gUnknown_020222A8[(i * 16 + j) * 3 + 0] = RED_VALUE(gPaletteBuffer[i * 16 + j]);
            gUnknown_020222A8[(i * 16 + j) * 3 + 1] = GREEN_VALUE(gPaletteBuffer[i * 16 + j]);
            gUnknown_020222A8[(i * 16 + j) * 3 + 2] = BLUE_VALUE(gPaletteBuffer[i * 16 + j]);
        }
    }
}

// duplicate of sub_8001860
void sub_8001964(u8 a)
{
    int i;
    int j;

    for (i = 31; i >= 0; i--)
    {
        gUnknown_02022288[i] = a;
        for (j = 0; j < 16; j++)
        {
            gUnknown_020222A8[(i * 16 + j) * 3 + 0] = RED_VALUE(gPaletteBuffer[i * 16 + j]) + 32;
            gUnknown_020222A8[(i * 16 + j) * 3 + 1] = GREEN_VALUE(gPaletteBuffer[i * 16 + j]) + 32;
            gUnknown_020222A8[(i * 16 + j) * 3 + 2] = BLUE_VALUE(gPaletteBuffer[i * 16 + j]) + 32;
        }
    }
}

void sub_80019E8(u8 a)
{
    int i;
    int j;

    for (i = 31; i >= 0; i--)
    {
        gUnknown_02022288[i] = a;
        for (j = 0; j < 16; j++)
        {
            gUnknown_020222A8[(i * 16 + j) * 3 + 0] = RED_VALUE(gPaletteBuffer[i * 16 + j]) + 64;
            gUnknown_020222A8[(i * 16 + j) * 3 + 1] = GREEN_VALUE(gPaletteBuffer[i * 16 + j]) + 64;
            gUnknown_020222A8[(i * 16 + j) * 3 + 2] = BLUE_VALUE(gPaletteBuffer[i * 16 + j]) + 64;
        }
    }
}

void sub_8001A6C(void)
{
    int i;
    int j;
    s16 r4;
    s16 r3;
    s16 r1;

    for (i = 31; i >= 0; i--)
    {
        if (gUnknown_02022288[i] != 0)
        {
            for (j = 15; j >= 0; j--)
            {
                gUnknown_020222A8[(i * 16 + j) * 3 + 0] += gUnknown_02022288[i];
                gUnknown_020222A8[(i * 16 + j) * 3 + 1] += gUnknown_02022288[i];
                gUnknown_020222A8[(i * 16 + j) * 3 + 2] += gUnknown_02022288[i];

                r4 = gUnknown_020222A8[(i * 16 + j) * 3 + 0] - 32;
                if (r4 > 31)
                    r4 = 31;
                if (r4 < 0)
                    r4 = 0;

                r3 = gUnknown_020222A8[(i * 16 + j) * 3 + 1] - 32;
                if (r3 > 31)
                    r3 = 31;
                if (r3 < 0)
                    r3 = 0;

                r1 = gUnknown_020222A8[(i * 16 + j) * 3 + 2] - 32;
                if (r1 > 31)
                    r1 = 31;
                if (r1 < 0)
                    r1 = 0;

                gPaletteBuffer[i * 16 + j] = (r1 << 10) + (r3 << 5) + r4;
            }
        }
    }

    sModifiedPalette = 1;
}

void SetupBackgrounds(u16 *bgConfig)
{
    u16 defaultBgConfig[12] =
    {
        // tile offset  map offset  screen size
        0x0000,         0x6000,     0,          // BG 0
        0x0000,         0x6800,     0,          // BG 1
        0x0000,         0x7000,     0,          // BG 2
        0x8000,         0x7800,     0,          // BG 3
    };
    int bg;

    if (bgConfig == NULL)
        bgConfig = defaultBgConfig;

    *(u16 *)&gLCDControlBuffer.bg0cnt = 0;
    *(u16 *)&gLCDControlBuffer.bg1cnt = 0;
    *(u16 *)&gLCDControlBuffer.bg2cnt = 0;
    *(u16 *)&gLCDControlBuffer.bg3cnt = 0;

    for (bg = 0; bg <= 3; bg++)
    {
        SetBackgroundTileDataOffset(bg, *bgConfig++);
        SetBackgroundMapDataOffset(bg, *bgConfig++);
        SetBackgroundScreenSize(bg, *bgConfig++);
        BG_SetPosition(bg, 0, 0);
        BG_Fill(BG_GetMapBuffer(bg), 0);
        CpuFastFill16(0, (void *)(VRAM + GetBackgroundTileDataOffset(bg)), 64);
    }
    SetupBackgroundForWeatherMaybe();
    sModifiedBGs |= 0xF;

    SetupOAMBufferSplice(0);

    gPaletteBuffer[0] = 0;
    sModifiedPalette = 1;

    gLCDControlBuffer.dispcnt.forcedBlank = 0;
    gLCDControlBuffer.dispcnt.mode = 0;
    gLCDControlBuffer.dispcnt.win0_on = 0;
    gLCDControlBuffer.dispcnt.win1_on = 0;
    gLCDControlBuffer.dispcnt.objWin_on = 0;
    gLCDControlBuffer.dispcnt.bg0_on = 1;
    gLCDControlBuffer.dispcnt.bg1_on = 1;
    gLCDControlBuffer.dispcnt.bg2_on = 1;
    gLCDControlBuffer.dispcnt.bg3_on = 1;
    gLCDControlBuffer.dispcnt.obj_on = 1;
}

static u16* sBgTilemapBuffers[] =
{
    gBG0TilemapBuffer,
    gBG1TilemapBuffer,
    gBG2TilemapBuffer,
    gBG3TilemapBuffer,
};

u16* BG_GetMapBuffer(int bg)
{
    return sBgTilemapBuffers[bg];
}

void sub_8001C5C(u8 a)
{
    gUnknown_0300001A = a;
}

s8 ShouldSkipHSScreen(void)
{
    return gUnknown_0300001A;
}

void SoftResetIfKeyComboPressed(void)
{
    if ((u8)sub_8000D18() != 0)
    {
        if (gKeyStatusPtr->heldKeys == (L_BUTTON | R_BUTTON | A_BUTTON | B_BUTTON))
            SoftReset(0);
        else if (gKeyStatusPtr->heldKeys == (A_BUTTON | B_BUTTON | SELECT_BUTTON | START_BUTTON))
            SoftReset(0);
    }
}

void sub_8001CB0(int a)
{
    u16 savedIE = REG_IE;

    REG_KEYCNT = a - 0x4000;
    REG_IE &= ~(INTR_FLAG_SERIAL | INTR_FLAG_GAMEPAK);
    REG_IE |= INTR_FLAG_KEYPAD;
    REG_DISPCNT |= DISPCNT_FORCED_BLANK;
    SoundBiasReset();
    asm("swi 3");  // enter sleep mode
    SoundBiasSet();
    REG_IE = savedIE;
}

void ExecBothHBlankHandlers(void)
{
    if (sHBlankHandler1 != NULL)
        sHBlankHandler1();
    if (sHBlankHandler2 != NULL)
        sHBlankHandler2();
}

void UpdateHBlankHandlerState(void)
{
    u8 r2 = (sHBlankHandler1 != NULL);

    switch (r2 + (sHBlankHandler2 != NULL) * 2)
    {
    case 0:
        gLCDControlBuffer.dispstat.hblankIrqEnable = 0;
        REG_IE &= ~INTR_FLAG_HBLANK;
        break;
    case 1:
        gLCDControlBuffer.dispstat.hblankIrqEnable = 1;
        SetIRQHandler(1, sHBlankHandler1);
        REG_IE |= INTR_FLAG_HBLANK;
        break;
    case 2:
        gLCDControlBuffer.dispstat.hblankIrqEnable = 1;
        SetIRQHandler(1, sHBlankHandler2);
        REG_IE |= INTR_FLAG_HBLANK;
        break;
    case 3:
        gLCDControlBuffer.dispstat.hblankIrqEnable = 1;
        SetIRQHandler(1, ExecBothHBlankHandlers);
        REG_IE |= INTR_FLAG_HBLANK;
        break;
    }
}

void SetPrimaryHBlankHandler(void (*hblankHandler)(void))
{
    sHBlankHandler1 = hblankHandler;
    UpdateHBlankHandlerState();
}

void SetSecondaryHBlankHandler(void (*hblankHandler)(void))
{
    sHBlankHandler2 = hblankHandler;
    UpdateHBlankHandlerState();
}

int GetBackgroundFromBufferPointer(u16 *ptr)
{
    if (ptr >= gBG0TilemapBuffer && ptr < gBG0TilemapBuffer + 0x400)
        return 0;
    if (ptr >= gBG1TilemapBuffer && ptr < gBG1TilemapBuffer + 0x400)
        return 1;
    if (ptr >= gBG2TilemapBuffer && ptr < gBG2TilemapBuffer + 0x400)
        return 2;
    if (ptr >= gBG3TilemapBuffer && ptr < gBG3TilemapBuffer + 0x400)
        return 3;
    return -1;
}

struct BgCnt *sBGControlStructPtrs[] =
{
    &gLCDControlBuffer.bg0cnt,
    &gLCDControlBuffer.bg1cnt,
    &gLCDControlBuffer.bg2cnt,
    &gLCDControlBuffer.bg3cnt,
};

void BG_SetPriority(int bg, int priority)
{
    sBGControlStructPtrs[bg]->priority = priority;
}

int BG_GetPriority(int bg)
{
    return sBGControlStructPtrs[bg]->priority;
}

void SetSpecialColorEffectsParameters(u16 effect, u8 coeffA, u8 coeffB, u8 blendY)
{
    gLCDControlBuffer.bldcnt.effect = effect;
    gLCDControlBuffer.blendCoeffA = coeffA;
    gLCDControlBuffer.blendCoeffB = coeffB;
    gLCDControlBuffer.blendY = blendY;
}

void sub_8001ED0(int a, int b, int c, int d, int e)
{
    gUnknown_030030BC &= 0xFFE0;
    gUnknown_030030BC |= (a << 0) | (b << 1) | (c << 2) | (d << 3) | (e << 4);
}

void sub_8001F0C(int a, int b, int c, int d, int e)
{
    gUnknown_030030BC &= 0xE0FF;
    gUnknown_030030BC |= (a << 8) | (b << 9) | (c << 10) | (d << 11) | (e << 12);
}

void sub_8001F48(int a)
{
    gLCDControlBuffer.bldcnt.target1_bd_on = a;
}

void sub_8001F64(int a)
{
    gLCDControlBuffer.bldcnt.target2_bd_on = a;
}

void SetDefaultColorEffects(void)
{
    SetSpecialColorEffectsParameters(0, 16, 0, 0);
}

void EnablePaletteSync(void)
{
    sModifiedPalette = 1;
}

void DisablePaletteSync(void)
{
    sModifiedPalette = 0;
}

void BG_EnableSyncByMask(int bg)
{
    sModifiedBGs |= bg;
}

void BG_EnableSync(int bg)
{
    sModifiedBGs |= 1 << bg;
}

void sub_8001FD0(int a)
{
    sModifiedBGs &= ~a;
}

/* The functions below were likely part of a a file called dma.c in the original code,
 * even though they do not use DMA at all. */

void ClearTileRigistry(void)
{
    int i;

    gUnknown_02024CD4.unk0 = 0;
    gUnknown_02024CD4.unk4 = 0;
    for (i = 0; i < 32; i++)
    {
        gUnknown_02024CDC[i].src = 0;
        gUnknown_02024CDC[i].dest = 0;
        gUnknown_02024CDC[i].size = 0;
        gUnknown_02024CDC[i].mode = 0;
    }
    gUnknown_02024CDC[0].src = 0;
}

void RegisterTileGraphics(const void *a, void *b, int c)
{
    struct TileDataTransfer *ptr = &gUnknown_02024CDC[gUnknown_02024CD4.unk0];

    ptr->src = a;
    ptr->dest = b;
    ptr->size = c;
    ptr->mode = (c & 0x1F) ? 0 : 1;
    gUnknown_02024CD4.unk4 += c;
    gUnknown_02024CD4.unk0++;
}

void RegisterFillTile(void *a, void *b, int c)
{
    struct TileDataTransfer *ptr = &gUnknown_02024CDC[gUnknown_02024CD4.unk0];

    ptr->src = a;
    ptr->dest = b;
    ptr->size = c;
    ptr->mode = 2;
    gUnknown_02024CD4.unk4 += c;
    gUnknown_02024CD4.unk0++;
}

void FlushTiles(void)
{
    struct TileDataTransfer *ptr = gUnknown_02024CDC;
    int i;

    for (i = 0; i < gUnknown_02024CD4.unk0; i++)
    {
        switch (ptr->mode)
        {
        case 0:
            CpuCopy16(ptr->src, ptr->dest, ptr->size);
            break;
        case 1:
            CpuFastCopy(ptr->src, ptr->dest, ptr->size);
            break;
        case 2:
            CpuFastFill((u32)ptr->src, ptr->dest, ptr->size);
            break;
        }
        ptr++;
    }
    ClearTileRigistry();
}

void SetupOAMBufferSplice(int a)
{
    gUnknown_03000030.src = gUnknown_03003140;
    gUnknown_03000030.dest = (void *)OAM;
    gUnknown_03000030.unk8 = 0;
    gUnknown_03000030.count = a;

    gUnknown_03000020.src = gUnknown_03003140 + a * 4;
    gUnknown_03000020.dest = (void *)(OAM + a * 8);
    gUnknown_03000020.unk8 = a * 8;
    gUnknown_03000020.count = 128 - a;
}

void FlushSecondaryOAM(void)
{
    CpuFastCopy(gUnknown_03000020.src, gUnknown_03000020.dest, gUnknown_03000020.count * 8);
    ClearOAMBuffer(gUnknown_03000020.src, gUnknown_03000020.count);
    gUnknown_03003744 = gUnknown_03000020.src;
    gUnknown_03004158 = gUnknown_03003140;
    gUnknown_0300312C = 0;
}

void FlushPrimaryOAM(void)
{
    if (gUnknown_03000030.count != 0)
    {
        CpuFastCopy(gUnknown_03000030.src, gUnknown_03000030.dest, gUnknown_03000030.count * 8);
        ClearOAMBuffer(gUnknown_03000030.src, gUnknown_03000030.count);
        gUnknown_03003070 = gUnknown_03000030.src;
    }
}

void WriteOAMRotScaleData(int index, s16 pa, s16 pb, s16 pc, s16 pd)
{
    gUnknown_03003140[index * 16 + 3] = pa;
    gUnknown_03003140[index * 16 + 7] = pb;
    gUnknown_03003140[index * 16 + 11] = pc;
    gUnknown_03003140[index * 16 + 15] = pd;
}

struct UnknownDmaStruct2
{
    int unk0;
    u16 unk4;
    s16 unk6;
    s16 unk8;
};

void sub_80021E4(struct UnknownDmaStruct2 *a, int b, int c)
{
    while (a->unk0 != 1 && gUnknown_03003744 < gUnknown_03003240)
    {
        int x = (a->unk6 + b) & 0x1FF;
        int y = (a->unk8 + c) & 0xFF;

        *(u32 *)gUnknown_03003744++ = a->unk0 | (x << 16) | (y);
        *(u16 *)gUnknown_03003744++ = a->unk4;
        a++;
    }
}

int GetPrimaryOAMSize(void)
{
    return gUnknown_03000030.count;
}
