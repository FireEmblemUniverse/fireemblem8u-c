	.INCLUDE "macro.inc"

	.SYNTAX UNIFIED

	@ Everything related to map battle animations
	@ Needs further splitting

	THUMB_FUNC_START sub_807FE0C
sub_807FE0C: @ 0x0807FE0C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	adds r6, r0, #0
	adds r0, #0x42
	ldrh r2, [r0]
	cmp r2, #0
	bne _0807FF20
	subs r0, #2
	ldrh r1, [r0]
	mov r9, r0
	cmp r1, #0
	beq _0807FE3A
	cmp r1, #0xb
	bls _0807FE58
	strh r2, [r0]
	adds r0, r6, #0
	bl Proc_ClearNativeCallback
	b _0807FFC4
_0807FE3A:
	ldr r2, _0807FFD4  @ gLCDControlBuffer
	ldrb r1, [r2, #1]
	movs r0, #2
	negs r0, r0
	ands r0, r1
	movs r1, #3
	negs r1, r1
	ands r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2, #1]
_0807FE58:
	ldr r0, _0807FFD8  @ gUnknown_089A42BC
	mov sl, r0
	mov r8, r9
	mov r2, r8
	ldrh r1, [r2]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	add r0, sl
	ldr r0, [r0]
	ldr r5, _0807FFDC  @ gUnknown_08205884
	adds r7, r6, #0
	adds r7, #0x44
	ldrh r1, [r7]
	lsls r1, r1, #2
	adds r1, r1, r5
	ldr r1, [r1]
	lsls r1, r1, #5
	movs r3, #0xc0
	lsls r3, r3, #0x13
	adds r1, r1, r3
	bl CopyDataWithPossibleUncomp
	mov r0, r8
	ldrh r1, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	mov r1, sl
	adds r1, #8
	adds r0, r0, r1
	ldr r0, [r0]
	ldr r4, _0807FFE0  @ gUnknown_02020188
	adds r1, r4, #0
	bl CopyDataWithPossibleUncomp
	ldr r0, _0807FFE4  @ gBG2TilemapBuffer
	ldrh r3, [r7]
	lsls r1, r3, #2
	adds r1, r1, r5
	ldr r2, [r1]
	ldr r5, _0807FFE8  @ gUnknown_0820588C
	adds r3, r3, r5
	ldrb r1, [r3]
	lsls r1, r1, #0xc
	orrs r2, r1
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	str r2, [sp]
	adds r1, r4, #0
	movs r2, #0
	movs r3, #0
	bl sub_800159C
	movs r0, #4
	bl BG_EnableSyncByMask
	adds r0, r6, #0
	adds r0, #0x48
	movs r1, #0
	ldrsh r0, [r0, r1]
	cmp r0, #0
	bge _0807FEFA
	mov r2, r8
	ldrh r1, [r2]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	mov r1, sl
	adds r1, #4
	adds r0, r0, r1
	ldr r0, [r0]
	ldrh r1, [r7]
	adds r1, r1, r5
	ldrb r1, [r1]
	lsls r1, r1, #5
	movs r2, #0x20
	bl CopyToPaletteBuffer
	bl EnablePaletteSync
_0807FEFA:
	mov r3, r9
	ldrh r0, [r3]
	adds r1, r6, #0
	adds r1, #0x4a
	strh r0, [r1]
	ldrh r0, [r3]
	adds r0, #1
	strh r0, [r3]
	subs r1, #8
	movs r0, #4
	strh r0, [r1]
	ldrh r1, [r7]
	adds r0, r6, #0
	adds r0, #0x4c
	strh r1, [r0]
	ldrh r0, [r7]
	movs r1, #1
	eors r0, r1
	strh r0, [r7]
_0807FF20:
	adds r1, r6, #0
	adds r1, #0x42
	ldrh r0, [r1]
	subs r0, #1
	strh r0, [r1]
	adds r0, r6, #0
	adds r0, #0x48
	movs r2, #0
	ldrsh r1, [r0, r2]
	mov r9, r0
	cmp r1, #0
	blt _0807FFC4
	adds r0, r1, #0
	lsls r0, r0, #5
	movs r1, #0xf0
	bl __divsi3
	adds r7, r0, #0
	ldr r2, _0807FFD8  @ gUnknown_089A42BC
	adds r0, r6, #0
	adds r0, #0x4a
	movs r3, #0
	ldrsh r1, [r0, r3]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, #4
	adds r0, r0, r2
	ldr r2, [r0]
	ldr r1, _0807FFE8  @ gUnknown_0820588C
	adds r0, r6, #0
	adds r0, #0x4c
	movs r3, #0
	ldrsh r0, [r0, r3]
	adds r0, r0, r1
	ldrb r0, [r0]
	lsls r0, r0, #5
	ldr r1, _0807FFEC  @ gPaletteBuffer
	adds r0, r0, r1
	movs r1, #0x1f
	mov r8, r1
	adds r5, r0, #2
	adds r2, #2
	movs r6, #0xe
_0807FF78:
	ldrh r0, [r2]
	movs r4, #0x1f
	ands r4, r0
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x15
	mov r1, r8
	ands r3, r1
	lsrs r1, r0, #0x1a
	mov r0, r8
	ands r1, r0
	adds r4, r4, r7
	cmp r4, #0x1f
	bls _0807FF94
	movs r4, #0x1f
_0807FF94:
	adds r3, r3, r7
	cmp r3, #0x1f
	bls _0807FF9C
	movs r3, #0x1f
_0807FF9C:
	adds r1, r1, r7
	cmp r1, #0x1f
	bls _0807FFA4
	movs r1, #0x1f
_0807FFA4:
	lsls r0, r3, #5
	orrs r4, r0
	lsls r0, r1, #0xa
	orrs r4, r0
	strh r4, [r5]
	adds r5, #2
	adds r2, #2
	subs r6, #1
	cmp r6, #0
	bge _0807FF78
	bl EnablePaletteSync
	mov r1, r9
	ldrh r0, [r1]
	subs r0, #1
	strh r0, [r1]
_0807FFC4:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807FFD4: .4byte gLCDControlBuffer
_0807FFD8: .4byte gUnknown_089A42BC
_0807FFDC: .4byte gUnknown_08205884
_0807FFE0: .4byte gUnknown_02020188
_0807FFE4: .4byte gBG2TilemapBuffer
_0807FFE8: .4byte gUnknown_0820588C
_0807FFEC: .4byte gPaletteBuffer

	THUMB_FUNC_START sub_807FFF0
sub_807FFF0: @ 0x0807FFF0
	push {lr}
	ldr r0, _0808000C  @ gBG0TilemapBuffer
	movs r1, #0
	bl BG_Fill
	ldr r0, _08080010  @ gBG2TilemapBuffer
	movs r1, #0
	bl BG_Fill
	movs r0, #5
	bl BG_EnableSyncByMask
	pop {r0}
	bx r0
	.align 2, 0
_0808000C: .4byte gBG0TilemapBuffer
_08080010: .4byte gBG2TilemapBuffer

	THUMB_FUNC_START sub_8080014
sub_8080014: @ 0x08080014
	push {lr}
	adds r1, r0, #0
	cmp r1, #0
	beq _08080028
	ldr r0, _08080024  @ gUnknown_089A4434
	bl Proc_CreateBlockingChild
	b _08080030
	.align 2, 0
_08080024: .4byte gUnknown_089A4434
_08080028:
	ldr r0, _08080034  @ gUnknown_089A4434
	movs r1, #3
	bl Proc_Create
_08080030:
	pop {r0}
	bx r0
	.align 2, 0
_08080034: .4byte gUnknown_089A4434

	THUMB_FUNC_START sub_8080038
sub_8080038: @ 0x08080038
	push {lr}
	movs r0, #0
	bl SetSecondaryHBlankHandler
	ldr r0, _0808004C  @ gUnknown_089A448C
	bl Proc_DeleteAllWithScript
	pop {r0}
	bx r0
	.align 2, 0
_0808004C: .4byte gUnknown_089A448C

	THUMB_FUNC_START sub_8080050
sub_8080050: @ 0x08080050
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r6, r0, #0
	ldr r2, _08080128  @ gLCDControlBuffer
	ldrb r3, [r2, #0xc]
	movs r1, #4
	negs r1, r1
	adds r0, r1, #0
	ands r0, r3
	strb r0, [r2, #0xc]
	ldrb r3, [r2, #0x10]
	adds r0, r1, #0
	ands r0, r3
	strb r0, [r2, #0x10]
	ldrb r3, [r2, #0x14]
	adds r0, r1, #0
	ands r0, r3
	strb r0, [r2, #0x14]
	ldrb r0, [r2, #0x18]
	ands r1, r0
	movs r0, #2
	orrs r1, r0
	strb r1, [r2, #0x18]
	ldrb r1, [r2, #1]
	movs r0, #2
	negs r0, r0
	ands r0, r1
	movs r1, #3
	negs r1, r1
	ands r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2, #1]
	adds r4, r2, #0
	adds r4, #0x34
	ldrb r3, [r4]
	movs r1, #0x21
	negs r1, r1
	adds r0, r1, #0
	ands r0, r3
	strb r0, [r4]
	adds r2, #0x35
	ldrb r0, [r2]
	ands r1, r0
	strb r1, [r2]
	movs r5, #0
	str r5, [sp]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #0
	bl sub_8001ED0
	movs r0, #0
	bl sub_8001F48
	movs r0, #1
	str r0, [sp]
	movs r0, #0
	movs r1, #0
	movs r2, #0
	movs r3, #1
	bl sub_8001F0C
	movs r0, #1
	bl sub_8001F64
	movs r0, #1
	movs r1, #0x10
	movs r2, #0x10
	movs r3, #0
	bl SetSpecialColorEffectsParameters
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl BG_SetPosition
	adds r0, r6, #0
	adds r0, #0x40
	movs r4, #0
	strh r5, [r0]
	adds r0, #2
	strh r5, [r0]
	adds r0, #2
	strh r5, [r0]
	ldr r0, _0808012C  @ gUnknown_03001C7C
	strb r4, [r0]
	ldr r0, _08080130  @ gUnknown_089A448C
	movs r1, #0
	bl Proc_Create
	adds r1, r0, #0
	adds r1, #0x29
	strb r4, [r1]
	adds r0, #0x2a
	strb r4, [r0]
	ldr r0, _08080134  @ sub_8080408
	bl SetSecondaryHBlankHandler
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08080128: .4byte gLCDControlBuffer
_0808012C: .4byte gUnknown_03001C7C
_08080130: .4byte gUnknown_089A448C
_08080134: .4byte sub_8080408

	THUMB_FUNC_START sub_8080138
sub_8080138: @ 0x08080138
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	mov r9, r0
	adds r0, #0x42
	str r0, [sp, #4]
	ldrh r1, [r0]
	str r1, [sp, #8]
	cmp r1, #0
	beq _08080154
	b _0808026C
_08080154:
	mov r7, r9
	adds r7, #0x40
	ldrh r0, [r7]
	lsls r0, r0, #2
	ldr r2, _0808023C  @ gUnknown_082058B4
	adds r0, r0, r2
	ldrb r0, [r0]
	ldr r1, _08080240  @ gUnknown_089A43D4
	mov sl, r1
	lsls r4, r0, #1
	adds r4, r4, r0
	lsls r4, r4, #2
	adds r0, r4, r1
	ldr r0, [r0]
	ldr r2, _08080244  @ gUnknown_082058A8
	mov r8, r2
	mov r5, r9
	adds r5, #0x44
	ldrh r1, [r5]
	lsls r1, r1, #2
	add r1, r8
	ldr r1, [r1]
	lsls r1, r1, #5
	movs r2, #0xc0
	lsls r2, r2, #0x13
	adds r1, r1, r2
	bl CopyDataWithPossibleUncomp
	mov r0, sl
	adds r0, #8
	adds r0, r4, r0
	ldr r0, [r0]
	ldr r6, _08080248  @ gUnknown_02020188
	adds r1, r6, #0
	bl CopyDataWithPossibleUncomp
	ldr r0, _0808024C  @ gBG2TilemapBuffer
	ldrh r3, [r5]
	lsls r1, r3, #2
	add r1, r8
	ldr r2, [r1]
	ldr r1, _08080250  @ gUnknown_082058B0
	mov r8, r1
	add r3, r8
	ldrb r1, [r3]
	lsls r1, r1, #0xc
	orrs r2, r1
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	str r2, [sp]
	adds r1, r6, #0
	movs r2, #0
	movs r3, #0
	bl sub_800159C
	movs r0, #4
	bl BG_EnableSyncByMask
	ldr r2, _08080254  @ gUnknown_089A43D8
	adds r4, r4, r2
	ldr r0, [r4]
	ldrh r1, [r5]
	add r1, r8
	ldrb r1, [r1]
	lsls r1, r1, #5
	movs r2, #0x20
	bl CopyToPaletteBuffer
	bl EnablePaletteSync
	ldr r0, _08080258  @ gUnknown_089A448C
	bl Proc_Find
	adds r6, r0, #0
	adds r6, #0x29
	movs r1, #1
	mov r8, r1
	mov r2, r8
	strb r2, [r6]
	ldrh r1, [r7]
	lsls r1, r1, #2
	ldr r2, _0808023C  @ gUnknown_082058B4
	adds r1, r1, r2
	ldrb r1, [r1, #2]
	adds r4, r0, #0
	adds r4, #0x2a
	strb r1, [r4]
	ldrh r0, [r7]
	cmp r0, #0
	bne _08080212
	ldr r0, _0808025C  @ 0x0000013F
	mov r2, r9
	ldr r1, [r2, #0x30]
	bl PlaySpacialSoundMaybe
_08080212:
	ldrh r0, [r7]
	lsls r0, r0, #2
	ldr r1, _0808023C  @ gUnknown_082058B4
	adds r0, r0, r1
	ldrb r0, [r0, #1]
	ldr r2, [sp, #4]
	strh r0, [r2]
	ldrh r0, [r7]
	cmp r0, #6
	bls _08080260
	mov r0, r8
	strb r0, [r6]
	movs r0, #0
	strb r0, [r4]
	mov r1, sp
	ldrh r1, [r1, #8]
	strh r1, [r7]
	mov r0, r9
	bl Proc_ClearNativeCallback
	b _0808026C
	.align 2, 0
_0808023C: .4byte gUnknown_082058B4
_08080240: .4byte gUnknown_089A43D4
_08080244: .4byte gUnknown_082058A8
_08080248: .4byte gUnknown_02020188
_0808024C: .4byte gBG2TilemapBuffer
_08080250: .4byte gUnknown_082058B0
_08080254: .4byte gUnknown_089A43D8
_08080258: .4byte gUnknown_089A448C
_0808025C: .4byte 0x0000013F
_08080260:
	adds r0, #1
	strh r0, [r7]
	ldrh r0, [r5]
	movs r1, #1
	eors r0, r1
	strh r0, [r5]
_0808026C:
	mov r1, r9
	adds r1, #0x42
	ldrh r0, [r1]
	subs r0, #1
	strh r0, [r1]
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_8080288
sub_8080288: @ 0x08080288
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #0x42
	ldrh r0, [r1]
	adds r6, r0, #0
	cmp r6, #0
	bne _08080378
	adds r4, r5, #0
	adds r4, #0x40
	ldrh r1, [r4]
	lsls r0, r1, #0x12
	lsrs r7, r0, #0x10
	mov r9, r4
	cmp r1, #7
	bhi _08080330
	movs r6, #1
	adds r5, #0x44
	mov r8, r5
	ldr r0, _08080328  @ gUnknown_082058B0
	mov sl, r0
	movs r1, #0x1f
	mov ip, r1
_080802C0:
	mov r1, r8
	ldrh r0, [r1]
	add r0, sl
	ldrb r0, [r0]
	lsls r0, r0, #4
	adds r0, r0, r6
	lsls r0, r0, #1
	ldr r1, _0808032C  @ gPaletteBuffer
	adds r5, r0, r1
	ldrh r1, [r5]
	adds r4, r1, #0
	movs r0, #0x1f
	ands r0, r1
	adds r0, r7, r0
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
	cmp r2, #0x1f
	bls _080802E6
	movs r2, #0x1f
_080802E6:
	lsrs r0, r1, #5
	mov r1, ip
	ands r0, r1
	adds r0, r7, r0
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #0x1f
	bls _080802F8
	movs r3, #0x1f
_080802F8:
	lsrs r0, r4, #0xa
	mov r1, ip
	ands r0, r1
	adds r0, r7, r0
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0x1f
	bls _0808030A
	movs r1, #0x1f
_0808030A:
	lsls r0, r3, #5
	orrs r2, r0
	lsls r0, r1, #0xa
	orrs r2, r0
	strh r2, [r5]
	adds r6, #1
	cmp r6, #0xf
	ble _080802C0
	bl EnablePaletteSync
	mov r1, r9
	ldrh r0, [r1]
	adds r0, #1
	b _0808037A
	.align 2, 0
_08080328: .4byte gUnknown_082058B0
_0808032C: .4byte gPaletteBuffer
_08080330:
	movs r0, #1
	str r0, [sp]
	movs r0, #0
	movs r1, #0
	movs r2, #0
	movs r3, #1
	bl sub_8001ED0
	movs r0, #2
	movs r1, #0x10
	movs r2, #0x10
	movs r3, #0x10
	bl SetSpecialColorEffectsParameters
	ldr r2, _08080374  @ gLCDControlBuffer
	ldrb r1, [r2, #1]
	movs r0, #2
	negs r0, r0
	ands r0, r1
	movs r1, #3
	negs r1, r1
	ands r0, r1
	subs r1, #2
	ands r0, r1
	movs r1, #8
	orrs r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2, #1]
	strh r6, [r4]
	adds r0, r5, #0
	bl Proc_ClearNativeCallback
	b _0808037C
	.align 2, 0
_08080374: .4byte gLCDControlBuffer
_08080378:
	subs r0, #1
_0808037A:
	strh r0, [r1]
_0808037C:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_808038C
sub_808038C: @ 0x0808038C
	push {r4, lr}
	adds r4, r0, #0
	adds r1, r4, #0
	adds r1, #0x40
	ldrh r0, [r1]
	cmp r0, #0x81
	bhi _080803B6
	lsls r0, r0, #4
	movs r1, #0x82
	bl __divsi3
	movs r3, #0x10
	subs r3, r3, r0
	lsls r3, r3, #0x18
	lsrs r3, r3, #0x18
	movs r0, #2
	movs r1, #0x10
	movs r2, #0x10
	bl SetSpecialColorEffectsParameters
	b _080803C8
_080803B6:
	movs r0, #2
	movs r1, #0x10
	movs r2, #0x10
	movs r3, #0
	bl SetSpecialColorEffectsParameters
	adds r0, r4, #0
	bl Proc_ClearNativeCallback
_080803C8:
	adds r1, r4, #0
	adds r1, #0x40
	ldrh r0, [r1]
	adds r0, #1
	strh r0, [r1]
	pop {r4}
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_80803D8
sub_80803D8: @ 0x080803D8
	push {lr}
	ldr r0, _080803FC  @ gBG0TilemapBuffer
	movs r1, #0
	bl BG_Fill
	ldr r0, _08080400  @ gBG1TilemapBuffer
	movs r1, #0
	bl BG_Fill
	ldr r0, _08080404  @ gBG2TilemapBuffer
	movs r1, #0
	bl BG_Fill
	movs r0, #7
	bl BG_EnableSyncByMask
	pop {r0}
	bx r0
	.align 2, 0
_080803FC: .4byte gBG0TilemapBuffer
_08080400: .4byte gBG1TilemapBuffer
_08080404: .4byte gBG2TilemapBuffer

	THUMB_FUNC_START sub_8080408
sub_8080408: @ 0x08080408
	push {lr}
	ldr r0, _0808043C  @ gUnknown_03001C7C
	ldrb r0, [r0]
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	cmp r0, #0
	beq _08080438
	ldr r0, _08080440  @ 0x04000006
	ldrh r0, [r0]
	adds r0, #1
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
	cmp r2, #0xe4
	bne _08080426
	movs r2, #0
_08080426:
	cmp r2, #0x9f
	bhi _08080438
	ldr r0, _08080444  @ 0x0400001A
	lsrs r1, r2, #1
	subs r1, r1, r2
	ldr r3, _08080448  @ 0x000001FF
	adds r2, r3, #0
	ands r1, r2
	strh r1, [r0]
_08080438:
	pop {r0}
	bx r0
	.align 2, 0
_0808043C: .4byte gUnknown_03001C7C
_08080440: .4byte 0x04000006
_08080444: .4byte 0x0400001A
_08080448: .4byte 0x000001FF

	THUMB_FUNC_START sub_808044C
sub_808044C: @ 0x0808044C
	push {lr}
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #0x29
	movs r0, #0
	ldrsb r0, [r3, r0]
	cmp r0, #0
	beq _0808046A
	ldr r1, _08080470  @ gUnknown_03001C7C
	adds r0, r2, #0
	adds r0, #0x2a
	ldrb r0, [r0]
	strb r0, [r1]
	movs r0, #0
	strb r0, [r3]
_0808046A:
	pop {r0}
	bx r0
	.align 2, 0
_08080470: .4byte gUnknown_03001C7C

	THUMB_FUNC_START sub_8080474
sub_8080474: @ 0x08080474
	push {lr}
	adds r1, r0, #0
	cmp r1, #0
	beq _08080488
	ldr r0, _08080484  @ gUnknown_089A45DC
	bl Proc_CreateBlockingChild
	b _08080490
	.align 2, 0
_08080484: .4byte gUnknown_089A45DC
_08080488:
	ldr r0, _08080494  @ gUnknown_089A45DC
	movs r1, #3
	bl Proc_Create
_08080490:
	pop {r0}
	bx r0
	.align 2, 0
_08080494: .4byte gUnknown_089A45DC

	THUMB_FUNC_START sub_8080498
sub_8080498: @ 0x08080498
	push {r4, r5, r6, lr}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	sub sp, #4
	adds r6, r1, #0
	ldr r1, _0808051C  @ gUnknown_089A44A4
	mov r8, r1
	lsls r5, r0, #1
	adds r5, r5, r0
	lsls r5, r5, #2
	adds r0, r5, r1
	ldr r0, [r0]
	ldr r2, _08080520  @ gUnknown_082058D4
	lsls r1, r6, #2
	adds r1, r1, r2
	ldr r4, [r1]
	lsls r1, r4, #5
	movs r2, #0xc0
	lsls r2, r2, #0x13
	adds r1, r1, r2
	bl CopyDataWithPossibleUncomp
	mov r0, r8
	adds r0, #8
	adds r0, r5, r0
	ldr r0, [r0]
	ldr r1, _08080524  @ gUnknown_02020188
	mov r9, r1
	bl CopyDataWithPossibleUncomp
	ldr r0, _08080528  @ gBG2TilemapBuffer
	ldr r1, _0808052C  @ gUnknown_082058DC
	adds r6, r6, r1
	ldrb r6, [r6]
	lsls r1, r6, #0xc
	orrs r4, r1
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	str r4, [sp]
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl sub_800159C
	movs r0, #4
	bl BG_EnableSyncByMask
	movs r2, #4
	add r8, r2
	add r5, r8
	ldr r0, [r5]
	lsls r6, r6, #5
	adds r1, r6, #0
	movs r2, #0x20
	bl CopyToPaletteBuffer
	bl EnablePaletteSync
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0808051C: .4byte gUnknown_089A44A4
_08080520: .4byte gUnknown_082058D4
_08080524: .4byte gUnknown_02020188
_08080528: .4byte gBG2TilemapBuffer
_0808052C: .4byte gUnknown_082058DC

	THUMB_FUNC_START sub_8080530
sub_8080530: @ 0x08080530
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #4
	mov r8, r1
	ldr r6, _08080598  @ gUnknown_089A44A4
	lsls r4, r0, #1
	adds r4, r4, r0
	lsls r4, r4, #2
	adds r0, r4, r6
	ldr r0, [r0]
	ldr r2, _0808059C  @ gUnknown_082058D4
	lsls r1, r1, #2
	adds r1, r1, r2
	ldr r5, [r1]
	lsls r1, r5, #5
	movs r2, #0xc0
	lsls r2, r2, #0x13
	adds r1, r1, r2
	bl CopyDataWithPossibleUncomp
	adds r6, #8
	adds r4, r4, r6
	ldr r0, [r4]
	ldr r4, _080805A0  @ gUnknown_02020188
	adds r1, r4, #0
	bl CopyDataWithPossibleUncomp
	ldr r0, _080805A4  @ gBG2TilemapBuffer
	ldr r1, _080805A8  @ gUnknown_082058DC
	add r8, r1
	mov r3, r8
	ldrb r1, [r3]
	lsls r1, r1, #0xc
	orrs r5, r1
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	str r5, [sp]
	adds r1, r4, #0
	movs r2, #0
	movs r3, #0
	bl sub_800159C
	movs r0, #4
	bl BG_EnableSyncByMask
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08080598: .4byte gUnknown_089A44A4
_0808059C: .4byte gUnknown_082058D4
_080805A0: .4byte gUnknown_02020188
_080805A4: .4byte gBG2TilemapBuffer
_080805A8: .4byte gUnknown_082058DC

	THUMB_FUNC_START sub_80805AC
sub_80805AC: @ 0x080805AC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r2, #0
	ldr r3, _08080648  @ gUnknown_089A44A4
	lsls r2, r0, #1
	adds r2, r2, r0
	lsls r2, r2, #2
	adds r3, #4
	adds r2, r2, r3
	ldr r0, _0808064C  @ gUnknown_082058DC
	adds r1, r1, r0
	ldrb r0, [r1]
	lsls r0, r0, #5
	ldr r1, _08080650  @ gPaletteBuffer
	movs r3, #0x1f
	mov r8, r3
	movs r6, #0x1f
	adds r5, r0, r1
	ldr r4, [r2]
	movs r0, #0xf
	mov ip, r0
_080805D8:
	ldrh r1, [r4]
	adds r3, r1, #0
	adds r2, r1, #0
	ands r2, r6
	subs r0, r6, r2
	muls r0, r7, r0
	cmp r0, #0
	bge _080805EA
	adds r0, #0x1f
_080805EA:
	asrs r0, r0, #5
	adds r0, r2, r0
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
	lsrs r1, r1, #5
	mov r0, r8
	ands r1, r0
	subs r0, r6, r1
	muls r0, r7, r0
	cmp r0, #0
	bge _08080602
	adds r0, #0x1f
_08080602:
	asrs r0, r0, #5
	adds r0, r1, r0
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	lsrs r3, r3, #0xa
	mov r0, r8
	ands r3, r0
	subs r0, r6, r3
	muls r0, r7, r0
	cmp r0, #0
	bge _0808061A
	adds r0, #0x1f
_0808061A:
	asrs r0, r0, #5
	adds r0, r3, r0
	lsls r0, r0, #0x10
	lsls r1, r1, #5
	orrs r2, r1
	lsrs r0, r0, #6
	orrs r2, r0
	strh r2, [r5]
	adds r5, #2
	adds r4, #2
	movs r3, #1
	negs r3, r3
	add ip, r3
	mov r0, ip
	cmp r0, #0
	bge _080805D8
	bl EnablePaletteSync
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08080648: .4byte gUnknown_089A44A4
_0808064C: .4byte gUnknown_082058DC
_08080650: .4byte gPaletteBuffer

	THUMB_FUNC_START sub_8080654
sub_8080654: @ 0x08080654
	push {lr}
	ldr r0, [r0, #0x50]
	bl Proc_Delete
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_8080660
sub_8080660: @ 0x08080660
	push {r4, r5, lr}
	sub sp, #4
	adds r5, r0, #0
	ldr r2, _08080728  @ gLCDControlBuffer
	ldrb r3, [r2, #0xc]
	movs r1, #4
	negs r1, r1
	adds r0, r1, #0
	ands r0, r3
	strb r0, [r2, #0xc]
	ldrb r3, [r2, #0x10]
	adds r0, r1, #0
	ands r0, r3
	strb r0, [r2, #0x10]
	ldrb r3, [r2, #0x14]
	adds r0, r1, #0
	ands r0, r3
	strb r0, [r2, #0x14]
	ldrb r0, [r2, #0x18]
	ands r1, r0
	movs r0, #2
	orrs r1, r0
	strb r1, [r2, #0x18]
	ldrb r1, [r2, #1]
	movs r0, #2
	negs r0, r0
	ands r0, r1
	movs r1, #3
	negs r1, r1
	ands r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2, #1]
	adds r4, r2, #0
	adds r4, #0x34
	ldrb r3, [r4]
	movs r1, #0x21
	negs r1, r1
	adds r0, r1, #0
	ands r0, r3
	strb r0, [r4]
	adds r2, #0x35
	ldrb r0, [r2]
	ands r1, r0
	strb r1, [r2]
	movs r4, #0
	str r4, [sp]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #0
	bl sub_8001ED0
	movs r0, #0
	bl sub_8001F48
	movs r0, #1
	str r0, [sp]
	movs r0, #0
	movs r1, #0
	movs r2, #0
	movs r3, #1
	bl sub_8001F0C
	movs r0, #1
	bl sub_8001F64
	movs r0, #1
	movs r1, #0x10
	movs r2, #0x10
	movs r3, #0
	bl SetSpecialColorEffectsParameters
	movs r0, #2
	movs r1, #8
	movs r2, #8
	bl BG_SetPosition
	ldr r0, _0808072C  @ gUnknown_089A4644
	adds r1, r5, #0
	bl Proc_Create
	str r0, [r5, #0x50]
	adds r0, r5, #0
	adds r0, #0x40
	strh r4, [r0]
	adds r0, #2
	strh r4, [r0]
	adds r0, #2
	strh r4, [r0]
	adds r0, #2
	strh r4, [r0]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08080728: .4byte gLCDControlBuffer
_0808072C: .4byte gUnknown_089A4644

	THUMB_FUNC_START sub_8080730
sub_8080730: @ 0x08080730
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r0, #0
	movs r0, #0x42
	adds r0, r0, r4
	mov r8, r0
	ldrh r6, [r0]
	cmp r6, #0
	bne _080807A6
	ldr r1, _080807BC  @ gUnknown_082058DE
	adds r5, r4, #0
	adds r5, #0x40
	ldrh r0, [r5]
	adds r0, r0, r1
	ldrb r0, [r0]
	adds r7, r4, #0
	adds r7, #0x44
	ldrh r1, [r7]
	bl sub_8080498
	ldrh r0, [r5]
	cmp r0, #0
	bne _08080768
	ldr r0, _080807C0  @ 0x00000141
	ldr r1, [r4, #0x30]
	bl PlaySpacialSoundMaybe
_08080768:
	ldrh r0, [r5]
	adds r0, #1
	strh r0, [r5]
	movs r0, #5
	mov r1, r8
	strh r0, [r1]
	ldrh r0, [r5]
	cmp r0, #3
	bls _0808079E
	strh r6, [r5]
	adds r0, r4, #0
	adds r0, #0x46
	strh r6, [r0]
	adds r0, #2
	strh r6, [r0]
	adds r0, #2
	strh r6, [r0]
	ldr r1, _080807C4  @ gUnknown_0202BCB0
	movs r2, #0xc
	ldrsh r0, [r1, r2]
	str r0, [r4, #0x30]
	movs r2, #0xe
	ldrsh r0, [r1, r2]
	str r0, [r4, #0x34]
	adds r0, r4, #0
	bl Proc_ClearNativeCallback
_0808079E:
	ldrh r0, [r7]
	movs r1, #1
	eors r0, r1
	strh r0, [r7]
_080807A6:
	adds r1, r4, #0
	adds r1, #0x42
	ldrh r0, [r1]
	subs r0, #1
	strh r0, [r1]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080807BC: .4byte gUnknown_082058DE
_080807C0: .4byte 0x00000141
_080807C4: .4byte gUnknown_0202BCB0

	THUMB_FUNC_START sub_80807C8
sub_80807C8: @ 0x080807C8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r0, #0x46
	ldrh r0, [r0]
	cmp r0, #5
	bne _080807EC
	ldr r0, [r5, #0x50]
	adds r0, #0x29
	movs r2, #0
	movs r1, #1
	strb r1, [r0]
	ldr r0, [r5, #0x50]
	strh r2, [r0, #0x2a]
	ldr r1, [r5, #0x50]
	movs r0, #2
	b _08080818
_080807EC:
	cmp r0, #8
	bne _08080802
	ldr r0, [r5, #0x50]
	adds r0, #0x29
	movs r1, #1
	strb r1, [r0]
	ldr r0, [r5, #0x50]
	strh r1, [r0, #0x2a]
	ldr r1, [r5, #0x50]
	movs r0, #3
	b _08080818
_08080802:
	cmp r0, #0xb
	bne _0808081A
	ldr r0, [r5, #0x50]
	adds r0, #0x29
	movs r1, #1
	strb r1, [r0]
	ldr r1, [r5, #0x50]
	movs r0, #2
	strh r0, [r1, #0x2a]
	ldr r1, [r5, #0x50]
	movs r0, #4
_08080818:
	strh r0, [r1, #0x34]
_0808081A:
	adds r0, r5, #0
	adds r0, #0x42
	ldrh r7, [r0]
	mov r8, r0
	cmp r7, #0
	bne _0808087E
	ldr r1, _08080864  @ gUnknown_082058E2
	adds r6, r5, #0
	adds r6, #0x40
	ldrh r0, [r6]
	adds r0, r0, r1
	ldrb r0, [r0]
	adds r4, r5, #0
	adds r4, #0x44
	ldrh r1, [r4]
	bl sub_8080498
	ldrh r0, [r6]
	adds r0, #1
	strh r0, [r6]
	movs r0, #4
	mov r1, r8
	strh r0, [r1]
	adds r1, r5, #0
	adds r1, #0x46
	ldrh r0, [r1]
	cmp r0, #0x15
	bls _08080868
	ldrh r0, [r6]
	cmp r0, #1
	bls _08080876
	strh r7, [r6]
	strh r7, [r1]
	adds r0, r5, #0
	bl Proc_ClearNativeCallback
	b _08080876
	.align 2, 0
_08080864: .4byte gUnknown_082058E2
_08080868:
	ldrh r0, [r6]
	cmp r0, #1
	bls _08080876
	strh r7, [r6]
	ldrh r0, [r1]
	adds r0, #1
	strh r0, [r1]
_08080876:
	ldrh r0, [r4]
	movs r1, #1
	eors r0, r1
	strh r0, [r4]
_0808087E:
	mov r1, r8
	ldrh r0, [r1]
	subs r0, #1
	strh r0, [r1]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_8080890
sub_8080890: @ 0x08080890
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r0, #0x42
	adds r0, r0, r6
	mov r8, r0
	ldrh r7, [r0]
	cmp r7, #0
	bne _080808EC
	ldr r1, _080808DC  @ gUnknown_082058E4
	adds r4, r6, #0
	adds r4, #0x40
	ldrh r0, [r4]
	adds r0, r0, r1
	ldrb r0, [r0]
	adds r5, r6, #0
	adds r5, #0x44
	ldrh r1, [r5]
	bl sub_8080498
	movs r0, #3
	mov r1, r8
	strh r0, [r1]
	ldrh r0, [r4]
	cmp r0, #0xa
	bls _080808E0
	strh r7, [r4]
	adds r0, r6, #0
	adds r0, #0x46
	strh r7, [r0]
	ldrh r0, [r1]
	subs r0, #1
	strh r0, [r1]
	adds r0, r6, #0
	bl Proc_ClearNativeCallback
	b _080808F6
	.align 2, 0
_080808DC: .4byte gUnknown_082058E4
_080808E0:
	adds r0, #1
	strh r0, [r4]
	ldrh r0, [r5]
	movs r1, #1
	eors r0, r1
	strh r0, [r5]
_080808EC:
	adds r1, r6, #0
	adds r1, #0x42
	ldrh r0, [r1]
	subs r0, #1
	strh r0, [r1]
_080808F6:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_8080900
sub_8080900: @ 0x08080900
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r2, r0, #0
	adds r1, r2, #0
	adds r1, #0x42
	ldrh r0, [r1]
	adds r4, r0, #0
	cmp r4, #0
	bne _080809C4
	adds r3, r2, #0
	adds r3, #0x40
	ldrh r1, [r3]
	lsls r0, r1, #0x12
	lsrs r7, r0, #0x10
	mov r9, r3
	cmp r1, #7
	bhi _080809A4
	movs r6, #1
	adds r2, #0x44
	mov r8, r2
	ldr r0, _0808099C  @ gUnknown_082058DC
	mov sl, r0
	movs r1, #0x1f
	mov ip, r1
_08080936:
	mov r1, r8
	ldrh r0, [r1]
	add r0, sl
	ldrb r0, [r0]
	lsls r0, r0, #4
	adds r0, r0, r6
	lsls r0, r0, #1
	ldr r1, _080809A0  @ gPaletteBuffer
	adds r5, r0, r1
	ldrh r1, [r5]
	adds r4, r1, #0
	movs r0, #0x1f
	ands r0, r1
	adds r0, r7, r0
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
	cmp r2, #0x1f
	bls _0808095C
	movs r2, #0x1f
_0808095C:
	lsrs r0, r1, #5
	mov r1, ip
	ands r0, r1
	adds r0, r7, r0
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #0x1f
	bls _0808096E
	movs r3, #0x1f
_0808096E:
	lsrs r0, r4, #0xa
	mov r1, ip
	ands r0, r1
	adds r0, r7, r0
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
	cmp r1, #0x1f
	bls _08080980
	movs r1, #0x1f
_08080980:
	lsls r0, r3, #5
	orrs r2, r0
	lsls r0, r1, #0xa
	orrs r2, r0
	strh r2, [r5]
	adds r6, #1
	cmp r6, #0xf
	ble _08080936
	bl EnablePaletteSync
	mov r1, r9
	ldrh r0, [r1]
	adds r0, #1
	b _080809C6
	.align 2, 0
_0808099C: .4byte gUnknown_082058DC
_080809A0: .4byte gPaletteBuffer
_080809A4:
	strh r4, [r3]
	adds r0, r2, #0
	adds r0, #0x46
	strh r4, [r0]
	adds r0, #2
	strh r4, [r0]
	adds r1, r2, #0
	adds r1, #0x4a
	ldr r0, _080809C0  @ 0x0000FFFF
	strh r0, [r1]
	adds r0, r2, #0
	bl Proc_ClearNativeCallback
	b _080809C8
	.align 2, 0
_080809C0: .4byte 0x0000FFFF
_080809C4:
	subs r0, #1
_080809C6:
	strh r0, [r1]
_080809C8:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_80809D8
sub_80809D8: @ 0x080809D8
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	adds r0, #0x46
	ldrh r0, [r0]
	cmp r0, #2
	bne _080809F8
	ldr r0, [r6, #0x50]
	adds r0, #0x29
	movs r1, #1
	strb r1, [r0]
	ldr r0, [r6, #0x50]
	strh r1, [r0, #0x2a]
	b _08080A1C
_080809F8:
	cmp r0, #3
	bne _08080A0A
	ldr r0, [r6, #0x50]
	adds r0, #0x29
	movs r1, #1
	strb r1, [r0]
	ldr r0, [r6, #0x50]
	strh r1, [r0, #0x2a]
	b _08080A36
_08080A0A:
	cmp r0, #4
	bne _08080A24
	ldr r0, [r6, #0x50]
	adds r0, #0x29
	movs r2, #0
	movs r1, #1
	strb r1, [r0]
	ldr r0, [r6, #0x50]
	strh r2, [r0, #0x2a]
_08080A1C:
	ldr r1, [r6, #0x50]
	movs r0, #3
	strh r0, [r1, #0x34]
	b _08080A4E
_08080A24:
	cmp r0, #5
	bne _08080A3E
	ldr r0, [r6, #0x50]
	adds r0, #0x29
	movs r2, #0
	movs r1, #1
	strb r1, [r0]
	ldr r0, [r6, #0x50]
	strh r2, [r0, #0x2a]
_08080A36:
	ldr r1, [r6, #0x50]
	movs r0, #4
	strh r0, [r1, #0x34]
	b _08080A4E
_08080A3E:
	cmp r0, #6
	bne _08080A4E
	ldr r0, [r6, #0x50]
	adds r0, #0x29
	movs r1, #0
	strb r1, [r0]
	ldr r0, [r6, #0x50]
	strh r1, [r0, #0x2a]
_08080A4E:
	adds r0, r6, #0
	adds r0, #0x42
	ldrh r7, [r0]
	mov r8, r0
	movs r0, #0x4a
	adds r0, r0, r6
	mov r9, r0
	cmp r7, #0
	bne _08080ABE
	ldr r1, _08080AA4  @ gUnknown_082058F0
	adds r5, r6, #0
	adds r5, #0x40
	ldrh r0, [r5]
	adds r0, r0, r1
	ldrb r0, [r0]
	adds r4, r6, #0
	adds r4, #0x44
	ldrh r1, [r4]
	bl sub_8080530
	ldrh r0, [r5]
	mov r1, r9
	strh r0, [r1]
	ldrh r0, [r5]
	adds r0, #1
	strh r0, [r5]
	movs r0, #4
	mov r3, r8
	strh r0, [r3]
	adds r1, r6, #0
	adds r1, #0x46
	ldrh r0, [r1]
	cmp r0, #0xa
	bls _08080AA8
	ldrh r0, [r5]
	cmp r0, #3
	bls _08080AB6
	strh r7, [r5]
	strh r7, [r1]
	adds r0, r6, #0
	bl Proc_ClearNativeCallback
	b _08080AB6
	.align 2, 0
_08080AA4: .4byte gUnknown_082058F0
_08080AA8:
	ldrh r0, [r5]
	cmp r0, #3
	bls _08080AB6
	strh r7, [r5]
	ldrh r0, [r1]
	adds r0, #1
	strh r0, [r1]
_08080AB6:
	ldrh r0, [r4]
	movs r1, #1
	eors r0, r1
	strh r0, [r4]
_08080ABE:
	mov r4, r8
	ldrh r0, [r4]
	subs r0, #1
	strh r0, [r4]
	adds r2, r6, #0
	adds r2, #0x48
	ldrh r1, [r2]
	movs r3, #0
	ldrsh r0, [r2, r3]
	cmp r0, #0x95
	bgt _08080AD8
	adds r0, r1, #1
	strh r0, [r2]
_08080AD8:
	mov r4, r9
	movs r1, #0
	ldrsh r0, [r4, r1]
	cmp r0, #0
	blt _08080B06
	ldr r1, _08080B14  @ gUnknown_082058F0
	adds r0, r0, r1
	ldrb r5, [r0]
	adds r0, r6, #0
	adds r0, #0x44
	ldrh r4, [r0]
	movs r1, #0
	ldrsh r0, [r2, r1]
	lsls r0, r0, #5
	movs r1, #0x96
	bl __divsi3
	movs r2, #0x20
	subs r2, r2, r0
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_80805AC
_08080B06:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08080B14: .4byte gUnknown_082058F0

	THUMB_FUNC_START sub_8080B18
sub_8080B18: @ 0x08080B18
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #0x42
	adds r0, r0, r5
	mov r8, r0
	ldrh r7, [r0]
	cmp r7, #0
	bne _08080B6A
	ldr r1, _08080B80  @ gUnknown_082058F4
	adds r4, r5, #0
	adds r4, #0x40
	ldrh r0, [r4]
	adds r0, r0, r1
	ldrb r0, [r0]
	adds r6, r5, #0
	adds r6, #0x44
	ldrh r1, [r6]
	bl sub_8080498
	ldrh r0, [r4]
	adds r0, #1
	strh r0, [r4]
	movs r0, #5
	mov r1, r8
	strh r0, [r1]
	ldrh r0, [r4]
	cmp r0, #3
	bls _08080B62
	strh r7, [r4]
	adds r0, r5, #0
	adds r0, #0x46
	strh r7, [r0]
	adds r0, r5, #0
	bl Proc_ClearNativeCallback
_08080B62:
	ldrh r0, [r6]
	movs r1, #1
	eors r0, r1
	strh r0, [r6]
_08080B6A:
	adds r1, r5, #0
	adds r1, #0x42
	ldrh r0, [r1]
	subs r0, #1
	strh r0, [r1]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08080B80: .4byte gUnknown_082058F4

	THUMB_FUNC_START sub_8080B84
sub_8080B84: @ 0x08080B84
	push {lr}
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl BG_SetPosition
	ldr r0, _08080BA4  @ gBG2TilemapBuffer
	movs r1, #0
	bl BG_Fill
	movs r0, #7
	bl BG_EnableSyncByMask
	pop {r0}
	bx r0
	.align 2, 0
_08080BA4: .4byte gBG2TilemapBuffer

	THUMB_FUNC_START sub_8080BA8
sub_8080BA8: @ 0x08080BA8
	push {r4, lr}
	adds r1, r0, #0
	adds r1, #0x29
	movs r3, #0
	strb r3, [r1]
	movs r4, #0
	strh r3, [r0, #0x2a]
	ldr r2, _08080BD4  @ gUnknown_0202BCB0
	ldrh r1, [r2, #0xc]
	strh r1, [r0, #0x2c]
	ldrh r1, [r2, #0xe]
	strh r1, [r0, #0x2e]
	strh r3, [r0, #0x30]
	strh r3, [r0, #0x32]
	movs r1, #4
	strh r1, [r0, #0x34]
	strh r3, [r0, #0x36]
	adds r0, #0x38
	strb r4, [r0]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08080BD4: .4byte gUnknown_0202BCB0

	THUMB_FUNC_START sub_8080BD8
sub_8080BD8: @ 0x08080BD8
	push {r4, r5, r6, lr}
	mov ip, r0
	adds r0, #0x29
	ldrb r0, [r0]
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	cmp r0, #0
	bne _08080C04
	ldr r0, _08080C00  @ gUnknown_0202BCB0
	mov r2, ip
	ldrh r1, [r2, #0x2c]
	strh r1, [r0, #0xc]
	ldrh r1, [r2, #0x2e]
	strh r1, [r0, #0xe]
	movs r0, #2
	movs r1, #8
	movs r2, #8
	bl BG_SetPosition
	b _08080D1C
	.align 2, 0
_08080C00: .4byte gUnknown_0202BCB0
_08080C04:
	mov r3, ip
	ldrh r1, [r3, #0x36]
	movs r4, #0x36
	ldrsh r0, [r3, r4]
	cmp r0, #0
	ble _08080C16
	subs r0, r1, #1
	strh r0, [r3, #0x36]
	b _08080D1C
_08080C16:
	mov r5, ip
	ldrh r0, [r5, #0x34]
	strh r0, [r5, #0x36]
	movs r6, #0x2a
	ldrsh r0, [r5, r6]
	cmp r0, #1
	beq _08080C60
	cmp r0, #1
	bgt _08080C2E
	cmp r0, #0
	beq _08080C34
	b _08080CB8
_08080C2E:
	cmp r0, #2
	beq _08080C8C
	b _08080CB8
_08080C34:
	ldr r2, _08080C5C  @ gUnknown_082058F8
	mov r1, ip
	adds r1, #0x38
	ldrb r0, [r1]
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0]
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	mov r3, ip
	strh r0, [r3, #0x30]
	ldrb r0, [r1]
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #1]
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	strh r0, [r3, #0x32]
	b _08080CE2
	.align 2, 0
_08080C5C: .4byte gUnknown_082058F8
_08080C60:
	ldr r2, _08080C88  @ gUnknown_08205918
	mov r1, ip
	adds r1, #0x38
	ldrb r0, [r1]
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0]
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	mov r4, ip
	strh r0, [r4, #0x30]
	ldrb r0, [r1]
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #1]
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	strh r0, [r4, #0x32]
	b _08080CE2
	.align 2, 0
_08080C88: .4byte gUnknown_08205918
_08080C8C:
	ldr r2, _08080CB4  @ gUnknown_08205938
	mov r1, ip
	adds r1, #0x38
	ldrb r0, [r1]
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0]
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	mov r5, ip
	strh r0, [r5, #0x30]
	ldrb r0, [r1]
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrb r0, [r0, #1]
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	strh r0, [r5, #0x32]
	b _08080CE2
	.align 2, 0
_08080CB4: .4byte gUnknown_08205938
_08080CB8:
	ldr r3, _08080D24  @ gUnknown_08205918
	mov r1, ip
	adds r1, #0x38
	ldrb r0, [r1]
	lsls r0, r0, #2
	adds r0, r0, r3
	ldrb r0, [r0]
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	mov r6, ip
	ldrh r2, [r6, #0x2a]
	muls r0, r2, r0
	strh r0, [r6, #0x30]
	ldrb r0, [r1]
	lsls r0, r0, #2
	adds r0, r0, r3
	ldrb r0, [r0, #1]
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	muls r0, r2, r0
	strh r0, [r6, #0x32]
_08080CE2:
	adds r4, r1, #0
	ldr r3, _08080D28  @ gUnknown_0202BCB0
	mov r0, ip
	ldrh r1, [r0, #0x30]
	ldrh r2, [r0, #0x2c]
	adds r0, r1, r2
	strh r0, [r3, #0xc]
	mov r5, ip
	ldrh r2, [r5, #0x32]
	ldrh r6, [r5, #0x2e]
	adds r0, r2, r6
	strh r0, [r3, #0xe]
	adds r1, #8
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	adds r2, #8
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	movs r0, #2
	bl BG_SetPosition
	ldrb r0, [r4]
	adds r0, #1
	strb r0, [r4]
	ldrb r0, [r4]
	cmp r0, #8
	blt _08080D1C
	movs r0, #0
	strb r0, [r4]
_08080D1C:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08080D24: .4byte gUnknown_08205918
_08080D28: .4byte gUnknown_0202BCB0

	THUMB_FUNC_START New6C_SummonGfx
New6C_SummonGfx: @ 0x08080D2C
	push {r4, r5, lr}
	adds r3, r0, #0
	adds r4, r1, #0
	adds r5, r2, #0
	ldr r0, _08080D64  @ gUnknown_089A46AC
	adds r1, r3, #0
	bl Proc_CreateBlockingChild
	ldr r2, _08080D68  @ gUnknown_0202BCB0
	ldrh r1, [r2, #0xc]
	lsls r1, r1, #0x10
	asrs r1, r1, #0x14
	subs r4, r4, r1
	lsls r4, r4, #1
	adds r4, #1
	lsls r4, r4, #3
	str r4, [r0, #0x30]
	ldrh r1, [r2, #0xe]
	lsls r1, r1, #0x10
	asrs r1, r1, #0x14
	subs r5, r5, r1
	lsls r5, r5, #1
	adds r5, #1
	lsls r5, r5, #3
	str r5, [r0, #0x34]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08080D64: .4byte gUnknown_089A46AC
_08080D68: .4byte gUnknown_0202BCB0

	THUMB_FUNC_START sub_8080D6C
sub_8080D6C: @ 0x08080D6C
	push {r4, r5, lr}
	adds r5, r0, #0
	bl sub_807E978
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl BG_SetPosition
	ldr r4, _08080DC0  @ gUnknown_089E714C
	movs r0, #2
	bl GetBackgroundTileDataOffset
	adds r1, r0, #0
	ldr r0, _08080DC4  @ 0x06002C00
	adds r1, r1, r0
	adds r0, r4, #0
	bl CopyDataWithPossibleUncomp
	ldr r0, _08080DC8  @ gUnknown_089E7DEC
	movs r1, #0x80
	movs r2, #0x20
	bl CopyToPaletteBuffer
	movs r0, #1
	movs r1, #0x10
	movs r2, #0x10
	movs r3, #0
	bl SetSpecialColorEffectsParameters
	adds r0, r5, #0
	adds r0, #0x40
	movs r1, #0
	strh r1, [r0]
	adds r5, #0x42
	strh r1, [r5]
	bl EnablePaletteSync
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08080DC0: .4byte gUnknown_089E714C
_08080DC4: .4byte 0x06002C00
_08080DC8: .4byte gUnknown_089E7DEC

	THUMB_FUNC_START sub_8080DCC
sub_8080DCC: @ 0x08080DCC
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	adds r0, #0x42
	ldrh r0, [r0]
	cmp r0, #0
	bne _08080E62
	adds r0, r4, #0
	adds r0, #0x40
	ldrh r1, [r0]
	adds r5, r0, #0
	cmp r1, #0
	bne _08080DF4
	ldr r0, _08080DF0  @ 0x000003BF
	ldr r1, [r4, #0x30]
	bl PlaySpacialSoundMaybe
	b _08080E0E
	.align 2, 0
_08080DF0: .4byte 0x000003BF
_08080DF4:
	cmp r1, #0xc
	bne _08080E02
	bl RefreshEntityBmMaps
	bl SMS_UpdateFromGameData
	b _08080E0E
_08080E02:
	cmp r1, #0x11
	bls _08080E0E
	adds r0, r4, #0
	bl Proc_ClearNativeCallback
	b _08080E6C
_08080E0E:
	ldr r0, _08080E74  @ gUnknown_089A4664
	ldrh r1, [r5]
	lsls r1, r1, #2
	adds r1, r1, r0
	ldr r0, [r1]
	ldr r6, _08080E78  @ gUnknown_02020188
	adds r1, r6, #0
	bl CopyDataWithPossibleUncomp
	ldr r1, _08080E7C  @ gBG2TilemapBuffer
	ldr r0, [r4, #0x30]
	cmp r0, #0
	bge _08080E2A
	adds r0, #7
_08080E2A:
	asrs r0, r0, #3
	subs r0, #4
	lsls r0, r0, #0x10
	asrs r2, r0, #0x10
	ldr r3, [r4, #0x34]
	cmp r3, #0
	bge _08080E3A
	adds r3, #7
_08080E3A:
	asrs r3, r3, #3
	subs r3, #6
	lsls r3, r3, #0x10
	asrs r3, r3, #0x10
	ldr r0, _08080E80  @ 0x00004160
	str r0, [sp]
	adds r0, r1, #0
	adds r1, r6, #0
	bl sub_800159C
	movs r0, #4
	bl BG_EnableSyncByMask
	ldrh r0, [r5]
	adds r0, #1
	strh r0, [r5]
	adds r1, r4, #0
	adds r1, #0x42
	movs r0, #4
	strh r0, [r1]
_08080E62:
	adds r1, r4, #0
	adds r1, #0x42
	ldrh r0, [r1]
	subs r0, #1
	strh r0, [r1]
_08080E6C:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08080E74: .4byte gUnknown_089A4664
_08080E78: .4byte gUnknown_02020188
_08080E7C: .4byte gBG2TilemapBuffer
_08080E80: .4byte 0x00004160

	THUMB_FUNC_START sub_8080E84
sub_8080E84: @ 0x08080E84
	push {lr}
	ldr r0, _08080E98  @ gBG2TilemapBuffer
	movs r1, #0
	bl BG_Fill
	movs r0, #4
	bl BG_EnableSyncByMask
	pop {r0}
	bx r0
	.align 2, 0
_08080E98: .4byte gBG2TilemapBuffer

	THUMB_FUNC_START sub_8080E9C
sub_8080E9C: @ 0x08080E9C
	push {r4, lr}
	adds r2, r0, #0
	adds r4, r1, #0
	ldr r0, _08080EDC  @ gUnknown_089A46DC
	adds r1, r2, #0
	bl Proc_CreateBlockingChild
	str r4, [r0, #0x2c]
	movs r2, #0x10
	ldrsb r2, [r4, r2]
	ldr r3, _08080EE0  @ gUnknown_0202BCB0
	ldrh r1, [r3, #0xc]
	lsls r1, r1, #0x10
	asrs r1, r1, #0x14
	subs r2, r2, r1
	lsls r2, r2, #1
	adds r2, #1
	lsls r2, r2, #3
	str r2, [r0, #0x30]
	movs r2, #0x11
	ldrsb r2, [r4, r2]
	ldrh r1, [r3, #0xe]
	lsls r1, r1, #0x10
	asrs r1, r1, #0x14
	subs r2, r2, r1
	lsls r2, r2, #1
	adds r2, #1
	lsls r2, r2, #3
	str r2, [r0, #0x34]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08080EDC: .4byte gUnknown_089A46DC
_08080EE0: .4byte gUnknown_0202BCB0

	THUMB_FUNC_START sub_8080EE4
sub_8080EE4: @ 0x08080EE4
	push {r4, r5, lr}
	adds r5, r0, #0
	bl sub_807E978
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl BG_SetPosition
	ldr r4, _08080F38  @ gUnknown_089E714C
	movs r0, #2
	bl GetBackgroundTileDataOffset
	adds r1, r0, #0
	ldr r0, _08080F3C  @ 0x06002C00
	adds r1, r1, r0
	adds r0, r4, #0
	bl CopyDataWithPossibleUncomp
	ldr r0, _08080F40  @ gUnknown_089E7DEC
	movs r1, #0x80
	movs r2, #0x20
	bl CopyToPaletteBuffer
	movs r0, #1
	movs r1, #0x10
	movs r2, #0x10
	movs r3, #0
	bl SetSpecialColorEffectsParameters
	adds r0, r5, #0
	adds r0, #0x40
	movs r1, #0
	strh r1, [r0]
	adds r5, #0x42
	strh r1, [r5]
	bl EnablePaletteSync
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08080F38: .4byte gUnknown_089E714C
_08080F3C: .4byte 0x06002C00
_08080F40: .4byte gUnknown_089E7DEC

	THUMB_FUNC_START sub_8080F44
sub_8080F44: @ 0x08080F44
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	adds r0, #0x42
	ldrh r0, [r0]
	cmp r0, #0
	bne _08080FE6
	adds r0, r4, #0
	adds r0, #0x40
	ldrh r1, [r0]
	adds r5, r0, #0
	cmp r1, #0
	bne _08080F6C
	ldr r0, _08080F68  @ 0x000003BF
	ldr r1, [r4, #0x30]
	bl PlaySpacialSoundMaybe
	b _08080F92
	.align 2, 0
_08080F68: .4byte 0x000003BF
_08080F6C:
	cmp r1, #0xc
	bne _08080F86
	ldr r0, [r4, #0x2c]
	ldr r1, [r0, #0xc]
	movs r2, #2
	negs r2, r2
	ands r1, r2
	str r1, [r0, #0xc]
	bl RefreshEntityBmMaps
	bl SMS_UpdateFromGameData
	b _08080F92
_08080F86:
	cmp r1, #0x11
	bls _08080F92
	adds r0, r4, #0
	bl Proc_ClearNativeCallback
	b _08080FF0
_08080F92:
	ldr r0, _08080FF8  @ gUnknown_089A4664
	ldrh r1, [r5]
	lsls r1, r1, #2
	adds r1, r1, r0
	ldr r0, [r1]
	ldr r6, _08080FFC  @ gUnknown_02020188
	adds r1, r6, #0
	bl CopyDataWithPossibleUncomp
	ldr r1, _08081000  @ gBG2TilemapBuffer
	ldr r0, [r4, #0x30]
	cmp r0, #0
	bge _08080FAE
	adds r0, #7
_08080FAE:
	asrs r0, r0, #3
	subs r0, #4
	lsls r0, r0, #0x10
	asrs r2, r0, #0x10
	ldr r3, [r4, #0x34]
	cmp r3, #0
	bge _08080FBE
	adds r3, #7
_08080FBE:
	asrs r3, r3, #3
	subs r3, #6
	lsls r3, r3, #0x10
	asrs r3, r3, #0x10
	ldr r0, _08081004  @ 0x00004160
	str r0, [sp]
	adds r0, r1, #0
	adds r1, r6, #0
	bl sub_800159C
	movs r0, #4
	bl BG_EnableSyncByMask
	ldrh r0, [r5]
	adds r0, #1
	strh r0, [r5]
	adds r1, r4, #0
	adds r1, #0x42
	movs r0, #4
	strh r0, [r1]
_08080FE6:
	adds r1, r4, #0
	adds r1, #0x42
	ldrh r0, [r1]
	subs r0, #1
	strh r0, [r1]
_08080FF0:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08080FF8: .4byte gUnknown_089A4664
_08080FFC: .4byte gUnknown_02020188
_08081000: .4byte gBG2TilemapBuffer
_08081004: .4byte 0x00004160

	THUMB_FUNC_START sub_8081008
sub_8081008: @ 0x08081008
	push {lr}
	ldr r0, _0808101C  @ gBG2TilemapBuffer
	movs r1, #0
	bl BG_Fill
	movs r0, #4
	bl BG_EnableSyncByMask
	pop {r0}
	bx r0
	.align 2, 0
_0808101C: .4byte gBG2TilemapBuffer

	THUMB_FUNC_START sub_8081020
sub_8081020: @ 0x08081020
	push {r4, lr}
	adds r2, r0, #0
	adds r4, r1, #0
	ldr r0, _08081060  @ gUnknown_089A470C
	adds r1, r2, #0
	bl Proc_Create
	str r4, [r0, #0x2c]
	movs r2, #0x10
	ldrsb r2, [r4, r2]
	ldr r3, _08081064  @ gUnknown_0202BCB0
	ldrh r1, [r3, #0xc]
	lsls r1, r1, #0x10
	asrs r1, r1, #0x14
	subs r2, r2, r1
	lsls r2, r2, #1
	adds r2, #1
	lsls r2, r2, #3
	str r2, [r0, #0x30]
	movs r2, #0x11
	ldrsb r2, [r4, r2]
	ldrh r1, [r3, #0xe]
	lsls r1, r1, #0x10
	asrs r1, r1, #0x14
	subs r2, r2, r1
	lsls r2, r2, #1
	adds r2, #1
	lsls r2, r2, #3
	str r2, [r0, #0x34]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08081060: .4byte gUnknown_089A470C
_08081064: .4byte gUnknown_0202BCB0

	THUMB_FUNC_START sub_8081068
sub_8081068: @ 0x08081068
	push {lr}
	ldr r0, _08081074  @ gUnknown_089A470C
	bl Proc_DeleteAllWithScript
	pop {r0}
	bx r0
	.align 2, 0
_08081074: .4byte gUnknown_089A470C

	THUMB_FUNC_START sub_8081078
sub_8081078: @ 0x08081078
	push {r4, r5, lr}
	adds r5, r0, #0
	bl sub_807E978
	ldr r2, _080810F0  @ gLCDControlBuffer
	ldrb r3, [r2, #0xc]
	movs r1, #4
	negs r1, r1
	adds r0, r1, #0
	ands r0, r3
	strb r0, [r2, #0xc]
	ldrb r0, [r2, #0x10]
	ands r1, r0
	movs r0, #1
	orrs r1, r0
	strb r1, [r2, #0x10]
	ldrb r0, [r2, #0x14]
	movs r1, #3
	orrs r0, r1
	strb r0, [r2, #0x14]
	ldrb r0, [r2, #0x18]
	orrs r0, r1
	strb r0, [r2, #0x18]
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl BG_SetPosition
	ldr r4, _080810F4  @ gUnknown_089E714C
	movs r0, #2
	bl GetBackgroundTileDataOffset
	adds r1, r0, #0
	ldr r0, _080810F8  @ 0x06002C00
	adds r1, r1, r0
	adds r0, r4, #0
	bl CopyDataWithPossibleUncomp
	ldr r0, _080810FC  @ gUnknown_089E7DEC
	movs r1, #0x80
	movs r2, #0x20
	bl CopyToPaletteBuffer
	movs r0, #1
	movs r1, #0x10
	movs r2, #0x10
	movs r3, #0
	bl SetSpecialColorEffectsParameters
	adds r0, r5, #0
	adds r0, #0x40
	movs r1, #0
	strh r1, [r0]
	adds r5, #0x42
	strh r1, [r5]
	bl EnablePaletteSync
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080810F0: .4byte gLCDControlBuffer
_080810F4: .4byte gUnknown_089E714C
_080810F8: .4byte 0x06002C00
_080810FC: .4byte gUnknown_089E7DEC

	THUMB_FUNC_START sub_8081100
sub_8081100: @ 0x08081100
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	adds r0, #0x42
	ldrh r0, [r0]
	cmp r0, #0
	bne _08081188
	adds r0, r4, #0
	adds r0, #0x40
	ldrh r1, [r0]
	adds r5, r0, #0
	cmp r1, #0
	bne _08081128
	ldr r0, _08081124  @ 0x000003BF
	ldr r1, [r4, #0x30]
	bl PlaySpacialSoundMaybe
	b _08081134
	.align 2, 0
_08081124: .4byte 0x000003BF
_08081128:
	cmp r1, #9
	bls _08081134
	adds r0, r4, #0
	bl Proc_ClearNativeCallback
	b _08081192
_08081134:
	ldr r0, _0808119C  @ gUnknown_089A4664
	ldrh r1, [r5]
	lsls r1, r1, #2
	adds r1, r1, r0
	ldr r0, [r1]
	ldr r6, _080811A0  @ gUnknown_02020188
	adds r1, r6, #0
	bl CopyDataWithPossibleUncomp
	ldr r1, _080811A4  @ gBG2TilemapBuffer
	ldr r0, [r4, #0x30]
	cmp r0, #0
	bge _08081150
	adds r0, #7
_08081150:
	asrs r0, r0, #3
	subs r0, #4
	lsls r0, r0, #0x10
	asrs r2, r0, #0x10
	ldr r3, [r4, #0x34]
	cmp r3, #0
	bge _08081160
	adds r3, #7
_08081160:
	asrs r3, r3, #3
	subs r3, #6
	lsls r3, r3, #0x10
	asrs r3, r3, #0x10
	ldr r0, _080811A8  @ 0x00004160
	str r0, [sp]
	adds r0, r1, #0
	adds r1, r6, #0
	bl sub_800159C
	movs r0, #4
	bl BG_EnableSyncByMask
	ldrh r0, [r5]
	adds r0, #1
	strh r0, [r5]
	adds r1, r4, #0
	adds r1, #0x42
	movs r0, #4
	strh r0, [r1]
_08081188:
	adds r1, r4, #0
	adds r1, #0x42
	ldrh r0, [r1]
	subs r0, #1
	strh r0, [r1]
_08081192:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0808119C: .4byte gUnknown_089A4664
_080811A0: .4byte gUnknown_02020188
_080811A4: .4byte gBG2TilemapBuffer
_080811A8: .4byte 0x00004160

	THUMB_FUNC_START sub_80811AC
sub_80811AC: @ 0x080811AC
	push {lr}
	bl DeleteAllPaletteAnimator
	ldr r2, _080811CC  @ gLCDControlBuffer
	ldrb r1, [r2, #1]
	movs r0, #0x21
	negs r0, r0
	ands r0, r1
	movs r1, #0x41
	negs r1, r1
	ands r0, r1
	movs r1, #0x7f
	ands r0, r1
	strb r0, [r2, #1]
	pop {r0}
	bx r0
	.align 2, 0
_080811CC: .4byte gLCDControlBuffer

	THUMB_FUNC_START sub_80811D0
sub_80811D0: @ 0x080811D0
	push {r4, lr}
	adds r2, r0, #0
	adds r4, r1, #0
	ldr r0, _080811E8  @ gUnknown_089A4734
	adds r1, r2, #0
	bl Proc_CreateBlockingChild
	adds r0, #0x42
	strh r4, [r0]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080811E8: .4byte gUnknown_089A4734

	THUMB_FUNC_START sub_80811EC
sub_80811EC: @ 0x080811EC
	push {r4, lr}
	adds r4, r0, #0
	movs r0, #1
	movs r1, #0x10
	movs r2, #0x10
	movs r3, #0
	bl SetSpecialColorEffectsParameters
	adds r4, #0x40
	movs r0, #0
	strh r0, [r4]
	pop {r4}
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_8081208
sub_8081208: @ 0x08081208
	push {r4, lr}
	adds r4, r0, #0
	adds r1, r4, #0
	adds r1, #0x40
	ldrh r0, [r1]
	adds r0, #1
	strh r0, [r1]
	adds r2, r4, #0
	adds r2, #0x42
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	ldrh r3, [r2]
	cmp r0, r3
	bcs _08081242
	ldrh r0, [r1]
	lsls r0, r0, #4
	ldrh r1, [r2]
	bl __divsi3
	movs r1, #0x10
	subs r1, r1, r0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	movs r0, #1
	movs r2, #0x10
	movs r3, #0
	bl SetSpecialColorEffectsParameters
	b _08081254
_08081242:
	movs r0, #1
	movs r1, #0
	movs r2, #0x10
	movs r3, #0
	bl SetSpecialColorEffectsParameters
	adds r0, r4, #0
	bl Proc_ClearNativeCallback
_08081254:
	pop {r4}
	pop {r0}
	bx r0

	THUMB_FUNC_START nullsub_58
nullsub_58: @ 0x0808125C
	bx lr

	THUMB_FUNC_START GetItemAnim6CCode
GetItemAnim6CCode: @ 0x08081260
	push {lr}
	ldr r0, _08081270  @ gMapBattle
	ldr r0, [r0, #0x54]
	cmp r0, #0
	bne _0808126C
	ldr r0, _08081274  @ gUnknown_089A4764
_0808126C:
	pop {r1}
	bx r1
	.align 2, 0
_08081270: .4byte gMapBattle
_08081274: .4byte gUnknown_089A4764

	THUMB_FUNC_START MapAnim_AnimateSubjectIdle
MapAnim_AnimateSubjectIdle: @ 0x08081278
	push {lr}
	ldr r2, _08081298  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x58
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, #8
	adds r0, r0, r2
	ldr r0, [r0]
	bl MU_StartActionAnim
	pop {r0}
	bx r0
	.align 2, 0
_08081298: .4byte gMapBattle

	THUMB_FUNC_START MapAnim_SubjectResetAnim
MapAnim_SubjectResetAnim: @ 0x0808129C
	push {lr}
	ldr r2, _080812BC  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x58
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, #8
	adds r0, r0, r2
	ldr r0, [r0]
	bl MU_StartDelayedFaceTarget
	pop {r0}
	bx r0
	.align 2, 0
_080812BC: .4byte gMapBattle

	THUMB_FUNC_START sub_80812C0
sub_80812C0: @ 0x080812C0
	push {lr}
	ldr r2, _080812E8  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x58
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r0, [r0, #4]
	ldrb r0, [r0, #4]
	cmp r0, #0x4d
	bne _080812F0
	ldr r0, _080812EC  @ sub_8081348
	movs r1, #9
	bl SetupFutureCall2
	b _080812F8
	.align 2, 0
_080812E8: .4byte gMapBattle
_080812EC: .4byte sub_8081348
_080812F0:
	ldr r0, _08081340  @ sub_8081384
	movs r1, #0xc
	bl SetupFutureCall2
_080812F8:
	ldr r2, _08081344  @ gMapBattle
	adds r3, r2, #0
	adds r3, #0x58
	ldrb r1, [r3]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, #8
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r1, [r0, #0x30]
	movs r0, #0
	strh r0, [r1, #0x18]
	ldrb r1, [r3]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r1, [r0, #0x30]
	movs r0, #0x80
	lsls r0, r0, #1
	strh r0, [r1, #0x1a]
	ldrb r1, [r3]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r0, [r0, #0x30]
	movs r1, #5
	bl AP_SwitchAnimation
	pop {r0}
	bx r0
	.align 2, 0
_08081340: .4byte sub_8081384
_08081344: .4byte gMapBattle

	THUMB_FUNC_START sub_8081348
sub_8081348: @ 0x08081348
	push {lr}
	ldr r0, _08081378  @ 0x000002D5
	ldr r3, _0808137C  @ gMapBattle
	adds r1, r3, #0
	adds r1, #0x58
	ldrb r2, [r1]
	lsls r1, r2, #2
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	ldr r1, [r1]
	ldrb r1, [r1, #0x10]
	lsls r1, r1, #0x18
	asrs r1, r1, #0x18
	lsls r1, r1, #4
	ldr r2, _08081380  @ gUnknown_0202BCB0
	movs r3, #0xc
	ldrsh r2, [r2, r3]
	subs r1, r1, r2
	bl PlaySpacialSoundMaybe
	pop {r0}
	bx r0
	.align 2, 0
_08081378: .4byte 0x000002D5
_0808137C: .4byte gMapBattle
_08081380: .4byte gUnknown_0202BCB0

	THUMB_FUNC_START sub_8081384
sub_8081384: @ 0x08081384
	push {lr}
	ldr r0, _080813B4  @ 0x000002D6
	ldr r3, _080813B8  @ gMapBattle
	adds r1, r3, #0
	adds r1, #0x58
	ldrb r2, [r1]
	lsls r1, r2, #2
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	ldr r1, [r1]
	ldrb r1, [r1, #0x10]
	lsls r1, r1, #0x18
	asrs r1, r1, #0x18
	lsls r1, r1, #4
	ldr r2, _080813BC  @ gUnknown_0202BCB0
	movs r3, #0xc
	ldrsh r2, [r2, r3]
	subs r1, r1, r2
	bl PlaySpacialSoundMaybe
	pop {r0}
	bx r0
	.align 2, 0
_080813B4: .4byte 0x000002D6
_080813B8: .4byte gMapBattle
_080813BC: .4byte gUnknown_0202BCB0

	THUMB_FUNC_START sub_80813C0
sub_80813C0: @ 0x080813C0
	push {r4, lr}
	ldr r2, _080813F4  @ gMapBattle
	adds r4, r2, #0
	adds r4, #0x58
	ldrb r1, [r4]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, #8
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r0, [r0, #0x30]
	movs r3, #0
	strh r3, [r0, #0x18]
	ldrb r1, [r4]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r0, [r0, #0x30]
	strh r3, [r0, #0x1a]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080813F4: .4byte gMapBattle

	THUMB_FUNC_START MapAnim_BeginSubjectFastAnim
MapAnim_BeginSubjectFastAnim: @ 0x080813F8
	push {lr}
	ldr r2, _08081418  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x58
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, #8
	adds r0, r0, r2
	ldr r0, [r0]
	bl MU_StartFastMoveAnim
	pop {r0}
	bx r0
	.align 2, 0
_08081418: .4byte gMapBattle

	THUMB_FUNC_START sub_808141C
sub_808141C: @ 0x0808141C
	push {r4, lr}
	adds r3, r0, #0
	adds r4, r1, #0
	adds r0, #0x4c
	movs r2, #0
	ldrsh r1, [r0, r2]
	adds r0, r4, #0
	adds r0, #0x4c
	movs r2, #0
	ldrsh r0, [r0, r2]
	subs r0, r0, r1
	cmp r0, #0
	bgt _08081442
	adds r2, r1, #0
	cmp r0, #0
	bge _0808143E
	subs r2, #0x10
_0808143E:
	adds r1, r2, #0
	b _08081444
_08081442:
	adds r1, #0x10
_08081444:
	adds r0, r3, #0
	adds r0, #0x4c
	strh r1, [r0]
	adds r1, r3, #0
	adds r1, #0x4e
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r4, #0
	adds r0, #0x4e
	movs r3, #0
	ldrsh r0, [r0, r3]
	subs r0, r0, r2
	cmp r0, #0
	bgt _0808146C
	adds r3, r2, #0
	cmp r0, #0
	bge _08081468
	subs r3, #0x10
_08081468:
	adds r2, r3, #0
	b _0808146E
_0808146C:
	adds r2, #0x10
_0808146E:
	strh r2, [r1]
	pop {r4}
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_8081478
sub_8081478: @ 0x08081478
	push {r4, lr}
	adds r3, r0, #0
	adds r4, r1, #0
	adds r0, #0x4c
	movs r2, #0
	ldrsh r1, [r0, r2]
	adds r0, r4, #0
	adds r0, #0x4c
	movs r2, #0
	ldrsh r0, [r0, r2]
	subs r0, r1, r0
	cmp r0, #0
	bgt _0808149E
	adds r2, r1, #0
	cmp r0, #0
	bge _0808149A
	subs r2, #0x10
_0808149A:
	adds r1, r2, #0
	b _080814A0
_0808149E:
	adds r1, #0x10
_080814A0:
	adds r0, r3, #0
	adds r0, #0x4c
	strh r1, [r0]
	adds r1, r3, #0
	adds r1, #0x4e
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r4, #0
	adds r0, #0x4e
	movs r3, #0
	ldrsh r0, [r0, r3]
	subs r0, r2, r0
	cmp r0, #0
	bgt _080814C8
	adds r3, r2, #0
	cmp r0, #0
	bge _080814C4
	subs r3, #0x10
_080814C4:
	adds r2, r3, #0
	b _080814CA
_080814C8:
	adds r2, #0x10
_080814CA:
	strh r2, [r1]
	pop {r4}
	pop {r0}
	bx r0

	THUMB_FUNC_START MapAnim_MoveSubjectsTowardsTarget
MapAnim_MoveSubjectsTowardsTarget: @ 0x080814D4
	push {r4, r5, lr}
	ldr r4, _08081530  @ gMapBattle
	adds r0, r4, #0
	adds r0, #0x58
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, r4, #0
	adds r2, #8
	adds r0, r0, r2
	ldr r3, [r0]
	adds r0, r4, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r5, [r0]
	adds r0, r3, #0
	adds r1, r5, #0
	bl sub_808141C
	adds r0, r4, #0
	adds r0, #0x5a
	ldrh r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #3
	ands r0, r1
	cmp r0, #0
	beq _08081528
	ldr r3, [r4, #0x30]
	adds r0, r3, #0
	adds r1, r5, #0
	bl sub_808141C
	ldr r3, [r4, #0x44]
	adds r0, r3, #0
	adds r1, r5, #0
	bl sub_808141C
_08081528:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08081530: .4byte gMapBattle

	THUMB_FUNC_START MapAnim_MoveSubjectsAwayFromTarget
MapAnim_MoveSubjectsAwayFromTarget: @ 0x08081534
	push {r4, r5, lr}
	ldr r4, _08081590  @ gMapBattle
	adds r0, r4, #0
	adds r0, #0x58
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, r4, #0
	adds r2, #8
	adds r0, r0, r2
	ldr r3, [r0]
	adds r0, r4, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r5, [r0]
	adds r0, r3, #0
	adds r1, r5, #0
	bl sub_8081478
	adds r0, r4, #0
	adds r0, #0x5a
	ldrh r1, [r0]
	movs r0, #0x80
	lsls r0, r0, #3
	ands r0, r1
	cmp r0, #0
	beq _08081588
	ldr r3, [r4, #0x30]
	adds r0, r3, #0
	adds r1, r5, #0
	bl sub_8081478
	ldr r3, [r4, #0x44]
	adds r0, r3, #0
	adds r1, r5, #0
	bl sub_8081478
_08081588:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08081590: .4byte gMapBattle

	THUMB_FUNC_START MapAnim_MoveCameraOnSubject
MapAnim_MoveCameraOnSubject: @ 0x08081594
	push {lr}
	ldr r3, _080815BC  @ gMapBattle
	adds r1, r3, #0
	adds r1, #0x58
	ldrb r2, [r1]
	lsls r1, r2, #2
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	ldr r2, [r1]
	movs r1, #0x10
	ldrsb r1, [r2, r1]
	ldrb r2, [r2, #0x11]
	lsls r2, r2, #0x18
	asrs r2, r2, #0x18
	bl EnsureCameraOntoPosition
	pop {r0}
	bx r0
	.align 2, 0
_080815BC: .4byte gMapBattle

	THUMB_FUNC_START MapAnim_MoveCameraOnTarget
MapAnim_MoveCameraOnTarget: @ 0x080815C0
	push {lr}
	ldr r3, _080815E8  @ gMapBattle
	adds r1, r3, #0
	adds r1, #0x59
	ldrb r2, [r1]
	lsls r1, r2, #2
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	ldr r2, [r1]
	movs r1, #0x10
	ldrsb r1, [r2, r1]
	ldrb r2, [r2, #0x11]
	lsls r2, r2, #0x18
	asrs r2, r2, #0x18
	bl EnsureCameraOntoPosition
	pop {r0}
	bx r0
	.align 2, 0
_080815E8: .4byte gMapBattle

	THUMB_FUNC_START sub_80815EC
sub_80815EC: @ 0x080815EC
	push {r4, lr}
	ldr r2, _08081614  @ gMapBattle
	ldr r1, _08081618  @ gBattleTarget
	adds r3, r1, #0
	adds r3, #0x73
	ldrb r3, [r3]
	adds r4, r2, #0
	adds r4, #0x60
	strb r3, [r4]
	adds r1, #0x74
	ldrb r1, [r1]
	adds r2, #0x61
	strb r1, [r2]
	ldrb r1, [r4]
	ldrb r2, [r2]
	bl EnsureCameraOntoPosition
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08081614: .4byte gMapBattle
_08081618: .4byte gBattleTarget

	THUMB_FUNC_START MapAnim_BeginRoundSpecificAnims
MapAnim_BeginRoundSpecificAnims: @ 0x0808161C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r1, _08081648  @ gMapBattle
	adds r0, r1, #0
	adds r0, #0x58
	ldrb r0, [r0]
	mov sl, r0
	adds r0, r1, #0
	adds r0, #0x5a
	ldrh r2, [r0]
	movs r0, #0x80
	ands r0, r2
	adds r7, r1, #0
	cmp r0, #0
	beq _0808164C
	mov r5, sl
	b _08081652
	.align 2, 0
_08081648: .4byte gMapBattle
_0808164C:
	adds r0, r7, #0
	adds r0, #0x59
	ldrb r5, [r0]
_08081652:
	adds r6, r7, #0
	mov r0, sl
	lsls r4, r0, #2
	adds r0, r4, r0
	lsls r0, r0, #2
	adds r1, r6, #4
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x4a
	ldrh r0, [r0]
	bl GetSpellAssocReturnBool
	lsls r0, r0, #0x18
	str r4, [sp]
	cmp r0, #0
	bne _08081692
	adds r0, r6, #0
	adds r0, #0x5a
	ldrh r1, [r0]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	bne _08081682
	b _080818C4
_08081682:
	lsls r0, r5, #2
	adds r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r6
	ldr r0, [r0]
	bl MapAnim_BeginMISSAnim
	b _080818C4
_08081692:
	adds r1, r6, #0
	adds r1, #0x5f
	movs r0, #1
	strb r0, [r1]
	ldr r0, [r6]
	adds r0, #0x30
	ldrb r1, [r0]
	movs r0, #0xf
	ands r0, r1
	cmp r0, #0xa
	bne _080816BA
	adds r0, r6, #0
	adds r0, #0x5d
	movs r1, #0
	ldrsb r1, [r0, r1]
	negs r1, r1
	mov r0, sl
	bl sub_80818D8
	b _080816C8
_080816BA:
	adds r0, r7, #0
	adds r0, #0x5d
	movs r1, #0
	ldrsb r1, [r0, r1]
	adds r0, r5, #0
	bl sub_80818D8
_080816C8:
	ldr r6, _08081728  @ gMapBattle
	adds r4, r6, #0
	adds r4, #0x5a
	ldrh r1, [r4]
	movs r0, #0x80
	lsls r0, r0, #1
	ands r0, r1
	cmp r0, #0
	beq _080816EA
	adds r0, r6, #0
	adds r0, #0x5d
	movs r1, #0
	ldrsb r1, [r0, r1]
	negs r1, r1
	mov r0, sl
	bl sub_80818D8
_080816EA:
	adds r0, r6, #0
	adds r0, #0x5d
	movs r2, #0
	ldrsb r2, [r0, r2]
	cmp r2, #0
	bge _080816F8
	b _080818C4
_080816F8:
	ldrh r1, [r4]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _08081730
	lsls r4, r5, #2
	adds r4, r4, r5
	lsls r4, r4, #2
	adds r4, r4, r6
	ldr r0, [r4]
	movs r1, #0x10
	ldrsb r1, [r0, r1]
	lsls r1, r1, #4
	ldr r0, _0808172C  @ gUnknown_0202BCB0
	movs r2, #0xc
	ldrsh r0, [r0, r2]
	subs r1, r1, r0
	movs r0, #0xc8
	bl PlaySpacialSoundMaybe
	ldr r0, [r4]
	bl MapAnim_BeginMISSAnim
	b _080818C4
	.align 2, 0
_08081728: .4byte gMapBattle
_0808172C: .4byte gUnknown_0202BCB0
_08081730:
	cmp r2, #0
	bne _08081770
	movs r0, #0x80
	lsls r0, r0, #6
	ands r0, r1
	cmp r0, #0
	beq _08081740
	b _080818C4
_08081740:
	ldr r0, _08081768  @ 0x000002CE
	lsls r4, r5, #2
	adds r4, r4, r5
	lsls r4, r4, #2
	adds r4, r4, r6
	ldr r1, [r4]
	ldrb r1, [r1, #0x10]
	lsls r1, r1, #0x18
	asrs r1, r1, #0x18
	lsls r1, r1, #4
	ldr r2, _0808176C  @ gUnknown_0202BCB0
	movs r3, #0xc
	ldrsh r2, [r2, r3]
	subs r1, r1, r2
	bl PlaySpacialSoundMaybe
	ldr r0, [r4]
	bl MapAnim_BeginNODAMAGEAnim
	b _080818C4
	.align 2, 0
_08081768: .4byte 0x000002CE
_0808176C: .4byte gUnknown_0202BCB0
_08081770:
	movs r3, #0
	lsls r2, r5, #2
	adds r0, r2, r5
	lsls r0, r0, #2
	adds r1, r6, #4
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x55
	ldrb r0, [r0]
	adds r4, r2, #0
	cmp r0, #0x1b
	beq _0808178C
	cmp r0, #0x33
	bne _0808178E
_0808178C:
	movs r3, #1
_0808178E:
	cmp r3, #0
	beq _080817CA
	ldr r2, _080817B4  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x5c
	ldrb r1, [r0]
	movs r0, #2
	ands r0, r1
	cmp r0, #0
	beq _080817B8
	movs r6, #0xaf
	adds r0, r4, r5
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	movs r1, #1
	bl MapAnim_BeginWallBreakAnim
	b _080817FA
	.align 2, 0
_080817B4: .4byte gMapBattle
_080817B8:
	movs r6, #0xb0
	adds r0, r4, r5
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	movs r1, #0
	bl MapAnim_BeginWallBreakAnim
	b _080817FA
_080817CA:
	ldr r2, _080817E0  @ gMapBattle
	ldr r0, [r2]
	adds r0, #0x30
	ldrb r1, [r0]
	movs r0, #0xf
	ands r0, r1
	cmp r0, #0xa
	bne _080817E8
	ldr r6, _080817E4  @ 0x000003C9
	b _080817FA
	.align 2, 0
_080817E0: .4byte gMapBattle
_080817E4: .4byte 0x000003C9
_080817E8:
	adds r0, r2, #0
	adds r0, #0x5c
	ldrb r1, [r0]
	movs r0, #2
	ands r0, r1
	movs r6, #0xd2
	cmp r0, #0
	beq _080817FA
	movs r6, #0xd5
_080817FA:
	ldr r7, _08081878  @ gMapBattle
	adds r0, r7, #0
	adds r0, #0x5a
	ldrh r1, [r0]
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08081880
	adds r4, r4, r5
	lsls r4, r4, #2
	adds r0, r4, r7
	mov r9, r0
	ldr r0, [r0]
	movs r1, #0x10
	ldrsb r1, [r0, r1]
	lsls r1, r1, #4
	ldr r2, _0808187C  @ gUnknown_0202BCB0
	mov r8, r2
	movs r3, #0xc
	ldrsh r0, [r2, r3]
	subs r1, r1, r0
	adds r0, r6, #0
	bl PlaySpacialSoundMaybe
	adds r6, r7, #0
	adds r6, #8
	adds r4, r4, r6
	ldr r5, [r4]
	ldr r4, [sp]
	add r4, sl
	lsls r4, r4, #2
	adds r0, r7, #4
	adds r0, r4, r0
	ldr r0, [r0]
	adds r0, #0x4a
	ldrh r0, [r0]
	bl GetSpellAssocFlashColor
	adds r1, r0, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r0, r5, #0
	bl MU_StartCritFlash
	bl NewBG0Shaker
	mov r1, r9
	ldr r0, [r1]
	movs r1, #0x10
	ldrsb r1, [r0, r1]
	lsls r1, r1, #4
	mov r2, r8
	movs r3, #0xc
	ldrsh r0, [r2, r3]
	subs r1, r1, r0
	movs r0, #0xd8
	bl PlaySpacialSoundMaybe
	adds r4, r4, r6
	ldr r0, [r4]
	bl MU_StartFastMoveAnim
	b _080818C4
	.align 2, 0
_08081878: .4byte gMapBattle
_0808187C: .4byte gUnknown_0202BCB0
_08081880:
	adds r4, r4, r5
	lsls r4, r4, #2
	adds r0, r4, r7
	ldr r0, [r0]
	movs r1, #0x10
	ldrsb r1, [r0, r1]
	lsls r1, r1, #4
	ldr r0, _080818D4  @ gUnknown_0202BCB0
	movs r2, #0xc
	ldrsh r0, [r0, r2]
	subs r1, r1, r0
	adds r0, r6, #0
	bl PlaySpacialSoundMaybe
	adds r0, r7, #0
	adds r0, #8
	adds r4, r4, r0
	ldr r4, [r4]
	ldr r0, [sp]
	add r0, sl
	lsls r0, r0, #2
	adds r1, r7, #4
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x4a
	ldrh r0, [r0]
	bl GetSpellAssocFlashColor
	adds r1, r0, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r0, r4, #0
	bl MU_StartHitFlash
_080818C4:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080818D4: .4byte gUnknown_0202BCB0

	THUMB_FUNC_START sub_80818D8
sub_80818D8: @ 0x080818D8
	push {r4, lr}
	adds r3, r0, #0
	adds r4, r1, #0
	ldr r1, _080818F4  @ gMapBattle
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #2
	adds r2, r0, r1
	ldrb r0, [r2, #0xd]
	cmp r0, r4
	bgt _080818F8
	movs r0, #0
	b _080818FA
	.align 2, 0
_080818F4: .4byte gMapBattle
_080818F8:
	subs r0, r0, r4
_080818FA:
	strb r0, [r2, #0xd]
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #2
	adds r1, r0, r1
	ldrb r2, [r1, #0xc]
	ldrb r0, [r1, #0xd]
	cmp r0, r2
	bls _0808190E
	strb r2, [r1, #0xd]
_0808190E:
	pop {r4}
	pop {r0}
	bx r0

	THUMB_FUNC_START MapAnim_WaitForHPToEndChangingMaybe
MapAnim_WaitForHPToEndChangingMaybe: @ 0x08081914
	push {lr}
	adds r1, r0, #0
	ldr r0, _0808192C  @ gMapBattle
	adds r0, #0x5f
	ldrb r0, [r0]
	cmp r0, #0
	bne _08081928
	adds r0, r1, #0
	bl Proc_ClearNativeCallback
_08081928:
	pop {r0}
	bx r0
	.align 2, 0
_0808192C: .4byte gMapBattle

	THUMB_FUNC_START MapAnim_PoisonEffectOnTarget
MapAnim_PoisonEffectOnTarget: @ 0x08081930
	push {lr}
	ldr r2, _0808194C  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	bl NewMapPoisonEffect
	pop {r0}
	bx r0
	.align 2, 0
_0808194C: .4byte gMapBattle

	THUMB_FUNC_START sub_8081950
sub_8081950: @ 0x08081950
	push {lr}
	ldr r2, _0808196C  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	bl sub_807CD18
	pop {r0}
	bx r0
	.align 2, 0
_0808196C: .4byte gMapBattle

	THUMB_FUNC_START sub_8081970
sub_8081970: @ 0x08081970
	push {lr}
	ldr r2, _08081994  @ gMapBattle
	ldrb r0, [r2, #0xd]
	ldrb r1, [r2, #0xc]
	cmp r0, r1
	bcc _08081998
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r0, [r0]
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r1, r1, r2
	ldr r0, [r1]
	bl sub_807CDD0
	b _0808199E
	.align 2, 0
_08081994: .4byte gMapBattle
_08081998:
	ldr r0, _080819A4  @ gUnknown_089A5124
	bl Proc_DeleteAllWithScript
_0808199E:
	pop {r0}
	bx r0
	.align 2, 0
_080819A4: .4byte gUnknown_089A5124

	THUMB_FUNC_START sub_80819A8
sub_80819A8: @ 0x080819A8
	push {lr}
	ldr r2, _080819C4  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	bl sub_807E48C
	pop {r0}
	bx r0
	.align 2, 0
_080819C4: .4byte gMapBattle

	THUMB_FUNC_START sub_80819C8
sub_80819C8: @ 0x080819C8
	push {lr}
	ldr r2, _080819E4  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	bl sub_807E638
	pop {r0}
	bx r0
	.align 2, 0
_080819E4: .4byte gMapBattle

	THUMB_FUNC_START sub_80819E8
sub_80819E8: @ 0x080819E8
	push {lr}
	ldr r2, _08081A04  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x58
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	bl sub_807D0B4
	pop {r0}
	bx r0
	.align 2, 0
_08081A04: .4byte gMapBattle

	THUMB_FUNC_START sub_8081A08
sub_8081A08: @ 0x08081A08
	push {lr}
	ldr r2, _08081A24  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x58
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	bl sub_807D4D4
	pop {r0}
	bx r0
	.align 2, 0
_08081A24: .4byte gMapBattle

	THUMB_FUNC_START sub_8081A28
sub_8081A28: @ 0x08081A28
	push {lr}
	ldr r2, _08081A48  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x58
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r1, _08081A4C  @ gUnknown_089AE804
	ldr r2, _08081A50  @ gUnknown_089AF930
	bl sub_807D688
	pop {r0}
	bx r0
	.align 2, 0
_08081A48: .4byte gMapBattle
_08081A4C: .4byte gUnknown_089AE804
_08081A50: .4byte gUnknown_089AF930

	THUMB_FUNC_START sub_8081A54
sub_8081A54: @ 0x08081A54
	push {lr}
	ldr r2, _08081A74  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x58
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r1, _08081A78  @ gUnknown_089AE804
	ldr r2, _08081A7C  @ gUnknown_089AF910
	bl sub_807D688
	pop {r0}
	bx r0
	.align 2, 0
_08081A74: .4byte gMapBattle
_08081A78: .4byte gUnknown_089AE804
_08081A7C: .4byte gUnknown_089AF910

	THUMB_FUNC_START sub_8081A80
sub_8081A80: @ 0x08081A80
	push {lr}
	ldr r2, _08081AA4  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r1, _08081AA8  @ gUnknown_089AFCBC
	ldr r2, _08081AAC  @ gUnknown_089AFF78
	movs r3, #0x8b
	bl NewMapAnimEffectAnimator
	pop {r0}
	bx r0
	.align 2, 0
_08081AA4: .4byte gMapBattle
_08081AA8: .4byte gUnknown_089AFCBC
_08081AAC: .4byte gUnknown_089AFF78

	THUMB_FUNC_START sub_8081AB0
sub_8081AB0: @ 0x08081AB0
	push {lr}
	ldr r2, _08081AD4  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r1, _08081AD8  @ gUnknown_089AF950
	ldr r2, _08081ADC  @ gUnknown_089AFF78
	movs r3, #0x89
	bl NewMapAnimEffectAnimator
	pop {r0}
	bx r0
	.align 2, 0
_08081AD4: .4byte gMapBattle
_08081AD8: .4byte gUnknown_089AF950
_08081ADC: .4byte gUnknown_089AFF78

	THUMB_FUNC_START sub_8081AE0
sub_8081AE0: @ 0x08081AE0
	push {lr}
	ldr r2, _08081B04  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r1, _08081B08  @ gUnknown_089AFAC4
	ldr r2, _08081B0C  @ gUnknown_089AFF78
	movs r3, #0x8a
	bl NewMapAnimEffectAnimator
	pop {r0}
	bx r0
	.align 2, 0
_08081B04: .4byte gMapBattle
_08081B08: .4byte gUnknown_089AFAC4
_08081B0C: .4byte gUnknown_089AFF78

	THUMB_FUNC_START sub_8081B10
sub_8081B10: @ 0x08081B10
	push {lr}
	ldr r2, _08081B34  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r1, _08081B38  @ gUnknown_089AFCBC
	ldr r2, _08081B3C  @ gUnknown_089AFF78
	movs r3, #0x8b
	bl NewMapAnimEffectAnimator
	pop {r0}
	bx r0
	.align 2, 0
_08081B34: .4byte gMapBattle
_08081B38: .4byte gUnknown_089AFCBC
_08081B3C: .4byte gUnknown_089AFF78

	THUMB_FUNC_START sub_8081B40
sub_8081B40: @ 0x08081B40
	push {lr}
	ldr r2, _08081B64  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	ldr r1, _08081B68  @ gUnknown_089AF950
	ldr r2, _08081B6C  @ gUnknown_089AFF78
	movs r3, #0x89
	bl NewMapAnimEffectAnimator
	pop {r0}
	bx r0
	.align 2, 0
_08081B64: .4byte gMapBattle
_08081B68: .4byte gUnknown_089AF950
_08081B6C: .4byte gUnknown_089AFF78

	THUMB_FUNC_START sub_8081B70
sub_8081B70: @ 0x08081B70
	push {lr}
	ldr r0, _08081BC0  @ gUnknown_0202BCF0
	adds r0, #0x41
	ldrb r0, [r0]
	lsls r0, r0, #0x1e
	cmp r0, #0
	blt _08081B84
	movs r0, #0xb4
	bl m4aSongNumStart
_08081B84:
	ldr r3, _08081BC4  @ gMapBattle
	ldr r1, _08081BC8  @ gBattleTarget
	adds r0, r1, #0
	adds r0, #0x73
	ldrb r2, [r0]
	adds r0, r3, #0
	adds r0, #0x60
	strb r2, [r0]
	adds r1, #0x74
	ldrb r0, [r1]
	adds r1, r3, #0
	adds r1, #0x61
	strb r0, [r1]
	adds r0, r3, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r3
	ldr r0, [r0]
	movs r1, #0x10
	ldrsb r1, [r0, r1]
	movs r2, #0x11
	ldrsb r2, [r0, r2]
	bl New6C_SomethingFlashy
	pop {r0}
	bx r0
	.align 2, 0
_08081BC0: .4byte gUnknown_0202BCF0
_08081BC4: .4byte gMapBattle
_08081BC8: .4byte gBattleTarget

	THUMB_FUNC_START sub_8081BCC
sub_8081BCC: @ 0x08081BCC
	push {lr}
	ldr r2, _08081BF4  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	adds r1, r2, #0
	adds r1, #0x60
	ldrb r1, [r1]
	adds r2, #0x61
	ldrb r2, [r2]
	bl New6C_SomethingFlashy
	pop {r0}
	bx r0
	.align 2, 0
_08081BF4: .4byte gMapBattle

	THUMB_FUNC_START sub_8081BF8
sub_8081BF8: @ 0x08081BF8
	push {lr}
	ldr r2, _08081C14  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x58
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	bl sub_807DBA8
	pop {r0}
	bx r0
	.align 2, 0
_08081C14: .4byte gMapBattle

	THUMB_FUNC_START sub_8081C18
sub_8081C18: @ 0x08081C18
	push {lr}
	ldr r1, _08081C30  @ gMapBattle
	adds r0, r1, #0
	adds r0, #0x60
	ldrb r0, [r0]
	adds r1, #0x61
	ldrb r1, [r1]
	bl sub_807E760
	pop {r0}
	bx r0
	.align 2, 0
_08081C30: .4byte gMapBattle

	THUMB_FUNC_START sub_8081C34
sub_8081C34: @ 0x08081C34
	push {lr}
	ldr r2, _08081C50  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	bl sub_807DD80
	pop {r0}
	bx r0
	.align 2, 0
_08081C50: .4byte gMapBattle

	THUMB_FUNC_START sub_8081C54
sub_8081C54: @ 0x08081C54
	push {lr}
	ldr r2, _08081C70  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	bl sub_807DFF4
	pop {r0}
	bx r0
	.align 2, 0
_08081C70: .4byte gMapBattle

	THUMB_FUNC_START sub_8081C74
sub_8081C74: @ 0x08081C74
	push {lr}
	ldr r2, _08081C90  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	bl sub_807E0D4
	pop {r0}
	bx r0
	.align 2, 0
_08081C90: .4byte gMapBattle

	THUMB_FUNC_START sub_8081C94
sub_8081C94: @ 0x08081C94
	push {lr}
	ldr r2, _08081CB0  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	bl sub_807E1E4
	pop {r0}
	bx r0
	.align 2, 0
_08081CB0: .4byte gMapBattle

	THUMB_FUNC_START sub_8081CB4
sub_8081CB4: @ 0x08081CB4
	push {lr}
	ldr r2, _08081CD0  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r0, [r0]
	bl sub_807DE30
	pop {r0}
	bx r0
	.align 2, 0
_08081CD0: .4byte gMapBattle

	THUMB_FUNC_START sub_8081CD4
sub_8081CD4: @ 0x08081CD4
	push {lr}
	ldr r2, _08081CF4  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, #8
	adds r0, r0, r2
	ldr r0, [r0]
	movs r1, #0
	bl MU_StartFlashFade
	pop {r0}
	bx r0
	.align 2, 0
_08081CF4: .4byte gMapBattle

	THUMB_FUNC_START sub_8081CF8
sub_8081CF8: @ 0x08081CF8
	push {lr}
	ldr r2, _08081D18  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, #8
	adds r0, r0, r2
	ldr r0, [r0]
	bl MU_8079858
	pop {r0}
	bx r0
	.align 2, 0
_08081D18: .4byte gMapBattle

	THUMB_FUNC_START sub_8081D1C
sub_8081D1C: @ 0x08081D1C
	push {lr}
	ldr r2, _08081D3C  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, #8
	adds r0, r0, r2
	ldr r0, [r0]
	bl MU_Hide
	pop {r0}
	bx r0
	.align 2, 0
_08081D3C: .4byte gMapBattle

	THUMB_FUNC_START sub_8081D40
sub_8081D40: @ 0x08081D40
	push {r4, lr}
	ldr r2, _08081D7C  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r3, [r0]
	movs r0, #0x10
	ldrsb r0, [r3, r0]
	lsls r0, r0, #4
	ldr r2, _08081D80  @ gUnknown_0202BCB0
	movs r4, #0xc
	ldrsh r1, [r2, r4]
	subs r0, r0, r1
	adds r0, #8
	movs r1, #0x11
	ldrsb r1, [r3, r1]
	lsls r1, r1, #4
	movs r3, #0xe
	ldrsh r2, [r2, r3]
	subs r1, r1, r2
	adds r1, #8
	bl StartStarExplosionEffect
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08081D7C: .4byte gMapBattle
_08081D80: .4byte gUnknown_0202BCB0

	THUMB_FUNC_START sub_8081D84
sub_8081D84: @ 0x08081D84
	push {r4, lr}
	ldr r0, _08081DD4  @ gUnknown_0202BCF0
	adds r0, #0x41
	ldrb r0, [r0]
	lsls r0, r0, #0x1e
	cmp r0, #0
	blt _08081D98
	movs r0, #0xb5
	bl m4aSongNumStart
_08081D98:
	ldr r2, _08081DD8  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r3, [r0]
	movs r0, #0x10
	ldrsb r0, [r3, r0]
	lsls r0, r0, #4
	ldr r2, _08081DDC  @ gUnknown_0202BCB0
	movs r4, #0xc
	ldrsh r1, [r2, r4]
	subs r0, r0, r1
	adds r0, #8
	movs r1, #0x11
	ldrsb r1, [r3, r1]
	lsls r1, r1, #4
	movs r3, #0xe
	ldrsh r2, [r2, r3]
	subs r1, r1, r2
	adds r1, #8
	bl StartStarImplosionEffect
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08081DD4: .4byte gUnknown_0202BCF0
_08081DD8: .4byte gMapBattle
_08081DDC: .4byte gUnknown_0202BCB0

	THUMB_FUNC_START sub_8081DE0
sub_8081DE0: @ 0x08081DE0
	push {lr}
	ldr r2, _08081E00  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r2, #8
	adds r0, r0, r2
	ldr r0, [r0]
	bl MU_Show
	pop {r0}
	bx r0
	.align 2, 0
_08081E00: .4byte gMapBattle

	THUMB_FUNC_START sub_8081E04
sub_8081E04: @ 0x08081E04
	push {r4, r5, r6, lr}
	ldr r2, _08081E44  @ gMapBattle
	adds r0, r2, #0
	adds r0, #0x59
	ldrb r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r1, r0, r2
	ldr r6, [r1]
	adds r1, r2, #0
	adds r1, #8
	adds r0, r0, r1
	ldr r0, [r0]
	adds r5, r2, #0
	adds r5, #0x60
	ldrb r1, [r5]
	lsls r1, r1, #4
	adds r4, r2, #0
	adds r4, #0x61
	ldrb r2, [r4]
	lsls r2, r2, #4
	bl MU_SetDisplayPosition
	ldrb r0, [r5]
	strb r0, [r6, #0x10]
	ldrb r0, [r4]
	strb r0, [r6, #0x11]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08081E44: .4byte gMapBattle

	THUMB_FUNC_START sub_8081E48
sub_8081E48: @ 0x08081E48
	push {lr}
	bl sub_807F568
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_8081E54
sub_8081E54: @ 0x08081E54
	push {lr}
	bl sub_807F5C8
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_8081E60
sub_8081E60: @ 0x08081E60
	push {lr}
	bl RefreshEntityBmMaps
	bl RenderBmMap
	bl SMS_UpdateFromGameData
	bl MU_EndAll
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_8081E78
sub_8081E78: @ 0x08081E78
	push {r4, r5, lr}
	ldr r4, _08081EA0  @ gUnknown_0203E254
	adds r0, r4, #0
	bl sub_80823A0
	movs r0, #0xa0
	lsls r0, r0, #2
	adds r5, r4, r0
	adds r0, r5, #0
	bl sub_80823A0
	ldr r0, _08081EA4  @ gUnknown_0203E754
	str r4, [r0]
	str r5, [r0, #4]
	ldr r0, _08081EA8  @ gUnknown_0203E75C
	str r4, [r0]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08081EA0: .4byte gUnknown_0203E254
_08081EA4: .4byte gUnknown_0203E754
_08081EA8: .4byte gUnknown_0203E75C

	THUMB_FUNC_START sub_8081EAC
sub_8081EAC: @ 0x08081EAC
	push {lr}
	ldr r3, _08081F1C  @ gLCDControlBuffer
	ldrb r0, [r3, #1]
	movs r1, #0x20
	orrs r0, r1
	movs r1, #0x41
	negs r1, r1
	ands r0, r1
	movs r1, #0x7f
	ands r0, r1
	strb r0, [r3, #1]
	adds r1, r3, #0
	adds r1, #0x2d
	movs r0, #0
	strb r0, [r1]
	adds r1, #4
	strb r0, [r1]
	subs r1, #5
	movs r0, #0xf0
	strb r0, [r1]
	adds r1, #4
	movs r0, #0xa0
	strb r0, [r1]
	adds r2, r3, #0
	adds r2, #0x34
	ldrb r1, [r2]
	subs r0, #0xa2
	ands r0, r1
	movs r1, #3
	negs r1, r1
	ands r0, r1
	subs r1, #2
	ands r0, r1
	subs r1, #4
	ands r0, r1
	subs r1, #8
	ands r0, r1
	strb r0, [r2]
	adds r2, #2
	ldrb r0, [r2]
	movs r1, #1
	orrs r0, r1
	movs r1, #2
	orrs r0, r1
	movs r1, #4
	orrs r0, r1
	movs r1, #8
	orrs r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r2]
	ldr r0, _08081F20  @ sub_8081F64
	bl SetPrimaryHBlankHandler
	pop {r0}
	bx r0
	.align 2, 0
_08081F1C: .4byte gLCDControlBuffer
_08081F20: .4byte sub_8081F64

	THUMB_FUNC_START sub_8081F24
sub_8081F24: @ 0x08081F24
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r5, r0, #0
	adds r6, r1, #0
	mov r8, r2
	ldr r4, _08081F54  @ gUnknown_0203E754
	ldr r0, [r4, #4]
	bl sub_80823A0
	ldr r0, [r4, #4]
	adds r1, r5, #0
	adds r2, r6, #0
	mov r3, r8
	bl sub_80823FC
	bl sub_8082390
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08081F54: .4byte gUnknown_0203E754

	THUMB_FUNC_START sub_8081F58
sub_8081F58: @ 0x08081F58
	push {lr}
	movs r0, #0
	bl SetPrimaryHBlankHandler
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_8081F64
sub_8081F64: @ 0x08081F64
	push {lr}
	ldr r0, _08081F7C  @ 0x04000006
	ldrh r0, [r0]
	adds r3, r0, #0
	cmp r3, #0x9f
	bls _08081F88
	ldr r0, _08081F80  @ gUnknown_0203E75C
	ldr r1, _08081F84  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r3, #0
	b _08081F90
	.align 2, 0
_08081F7C: .4byte 0x04000006
_08081F80: .4byte gUnknown_0203E75C
_08081F84: .4byte gUnknown_0203E754
_08081F88:
	adds r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	ldr r0, _08081FA0  @ gUnknown_0203E75C
_08081F90:
	ldr r2, _08081FA4  @ 0x04000040
	ldr r1, [r0]
	lsls r0, r3, #1
	adds r0, r0, r1
	ldrh r0, [r0]
	strh r0, [r2]
	pop {r0}
	bx r0
	.align 2, 0
_08081FA0: .4byte gUnknown_0203E75C
_08081FA4: .4byte 0x04000040

	THUMB_FUNC_START sub_8081FA8
sub_8081FA8: @ 0x08081FA8
	push {lr}
	ldr r0, _08081FC0  @ 0x04000006
	ldrh r0, [r0]
	adds r3, r0, #0
	cmp r3, #0x9f
	bls _08081FCC
	ldr r0, _08081FC4  @ gUnknown_0203E75C
	ldr r1, _08081FC8  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r3, #0
	b _08081FD4
	.align 2, 0
_08081FC0: .4byte 0x04000006
_08081FC4: .4byte gUnknown_0203E75C
_08081FC8: .4byte gUnknown_0203E754
_08081FCC:
	adds r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	ldr r0, _08081FF0  @ gUnknown_0203E75C
_08081FD4:
	ldr r2, _08081FF4  @ 0x04000040
	ldr r1, [r0]
	lsls r0, r3, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	strh r1, [r2]
	ldr r1, _08081FF8  @ 0x04000018
	movs r2, #0xa0
	lsls r2, r2, #1
	adds r0, r0, r2
	ldrh r0, [r0]
	strh r0, [r1]
	pop {r0}
	bx r0
	.align 2, 0
_08081FF0: .4byte gUnknown_0203E75C
_08081FF4: .4byte 0x04000040
_08081FF8: .4byte 0x04000018

	THUMB_FUNC_START sub_8081FFC
sub_8081FFC: @ 0x08081FFC
	push {lr}
	ldr r0, _08082014  @ 0x04000006
	ldrh r0, [r0]
	adds r3, r0, #0
	cmp r3, #0x9f
	bls _08082020
	ldr r0, _08082018  @ gUnknown_0203E75C
	ldr r1, _0808201C  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r3, #0
	b _08082028
	.align 2, 0
_08082014: .4byte 0x04000006
_08082018: .4byte gUnknown_0203E75C
_0808201C: .4byte gUnknown_0203E754
_08082020:
	adds r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	ldr r0, _08082044  @ gUnknown_0203E75C
_08082028:
	ldr r2, _08082048  @ 0x05000022
	ldr r1, [r0]
	lsls r0, r3, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	strh r1, [r2]
	ldr r1, _0808204C  @ 0x05000042
	movs r2, #0xa0
	lsls r2, r2, #1
	adds r0, r0, r2
	ldrh r0, [r0]
	strh r0, [r1]
	pop {r0}
	bx r0
	.align 2, 0
_08082044: .4byte gUnknown_0203E75C
_08082048: .4byte 0x05000022
_0808204C: .4byte 0x05000042

	THUMB_FUNC_START sub_8082050
sub_8082050: @ 0x08082050
	push {lr}
	ldr r0, _08082068  @ 0x04000006
	ldrh r0, [r0]
	adds r3, r0, #0
	cmp r3, #0x9f
	bls _08082074
	ldr r0, _0808206C  @ gUnknown_0203E75C
	ldr r1, _08082070  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r3, #0
	b _0808207C
	.align 2, 0
_08082068: .4byte 0x04000006
_0808206C: .4byte gUnknown_0203E75C
_08082070: .4byte gUnknown_0203E754
_08082074:
	adds r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	ldr r0, _0808208C  @ gUnknown_0203E75C
_0808207C:
	ldr r2, _08082090  @ 0x04000052
	ldr r1, [r0]
	lsls r0, r3, #1
	adds r0, r0, r1
	ldrh r0, [r0]
	strh r0, [r2]
	pop {r0}
	bx r0
	.align 2, 0
_0808208C: .4byte gUnknown_0203E75C
_08082090: .4byte 0x04000052

	THUMB_FUNC_START sub_8082094
sub_8082094: @ 0x08082094
	push {lr}
	ldr r0, _080820AC  @ 0x04000006
	ldrh r0, [r0]
	adds r3, r0, #0
	cmp r3, #0x9f
	bls _080820B8
	ldr r0, _080820B0  @ gUnknown_0203E75C
	ldr r1, _080820B4  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r3, #0
	b _080820C0
	.align 2, 0
_080820AC: .4byte 0x04000006
_080820B0: .4byte gUnknown_0203E75C
_080820B4: .4byte gUnknown_0203E754
_080820B8:
	adds r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	ldr r0, _080820D0  @ gUnknown_0203E75C
_080820C0:
	ldr r2, _080820D4  @ 0x04000054
	ldr r1, [r0]
	lsls r0, r3, #1
	adds r0, r0, r1
	ldrh r0, [r0]
	strh r0, [r2]
	pop {r0}
	bx r0
	.align 2, 0
_080820D0: .4byte gUnknown_0203E75C
_080820D4: .4byte 0x04000054

	THUMB_FUNC_START sub_80820D8
sub_80820D8: @ 0x080820D8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r9, r0
	mov sl, r1
	adds r4, r2, #0
	adds r5, r3, #0
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	mov r9, r0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	mov sl, r1
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	ldr r7, _08082184  @ gUnknown_0203E754
	ldr r0, [r7, #4]
	adds r2, r4, #0
	movs r3, #0x1f
	ands r2, r3
	lsrs r2, r2, #1
	movs r1, #0xf8
	lsls r1, r1, #2
	mov r8, r1
	adds r1, r4, #0
	mov r3, r8
	ands r1, r3
	lsrs r1, r1, #1
	ands r1, r3
	orrs r2, r1
	movs r6, #0xf8
	lsls r6, r6, #7
	adds r1, r4, #0
	ands r1, r6
	lsrs r1, r1, #1
	ands r1, r6
	orrs r2, r1
	str r2, [sp]
	mov r1, r9
	mov r2, sl
	adds r3, r4, #0
	bl sub_80824C4
	ldr r0, [r7, #4]
	movs r1, #0xa0
	lsls r1, r1, #1
	adds r0, r0, r1
	adds r2, r5, #0
	movs r3, #0x1f
	ands r2, r3
	lsrs r2, r2, #1
	adds r1, r5, #0
	mov r3, r8
	ands r1, r3
	lsrs r1, r1, #1
	ands r1, r3
	orrs r2, r1
	adds r1, r5, #0
	ands r1, r6
	lsrs r1, r1, #1
	ands r1, r6
	orrs r2, r1
	str r2, [sp]
	mov r1, r9
	mov r2, sl
	adds r3, r5, #0
	bl sub_80824C4
	bl sub_8082390
	ldr r0, _08082188  @ sub_8081FFC
	bl SetPrimaryHBlankHandler
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08082184: .4byte gUnknown_0203E754
_08082188: .4byte sub_8081FFC

	THUMB_FUNC_START sub_808218C
sub_808218C: @ 0x0808218C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	adds r4, r1, #0
	mov sl, r2
	mov r9, r3
	ldr r5, [sp, #0x20]
	ldr r0, _080821AC  @ gUnknown_0203E754
	mov r8, r0
	ldr r0, [r0, #4]
	bl sub_80823A0
	b _080821E2
	.align 2, 0
_080821AC: .4byte gUnknown_0203E754
_080821B0:
	ldrb r0, [r5]
	mov r1, sl
	muls r1, r0, r1
	adds r0, r1, #0
	mov r1, r9
	bl Div
	adds r6, r0, #0
	adds r5, #1
	cmp r6, #0
	ble _080821E0
	mov r1, r8
	ldr r0, [r1, #4]
	adds r1, r7, r6
	subs r1, #1
	adds r2, r4, #0
	bl sub_80823DC
	mov r1, r8
	ldr r0, [r1, #4]
	subs r1, r7, r6
	adds r2, r4, #0
	bl sub_80823BC
_080821E0:
	subs r4, #1
_080821E2:
	ldrb r0, [r5]
	cmp r0, #0xff
	beq _080821EC
	cmp r4, #0
	bge _080821B0
_080821EC:
	cmp r6, #0
	ble _08082216
	cmp r4, #0
	blt _08082216
	ldr r5, _08082224  @ gUnknown_0203E754
	adds r0, r7, r6
	mov r8, r0
_080821FA:
	ldr r0, [r5, #4]
	mov r1, r8
	subs r1, #1
	adds r2, r4, #0
	bl sub_80823DC
	ldr r0, [r5, #4]
	subs r1, r7, r6
	adds r2, r4, #0
	bl sub_80823BC
	subs r4, #1
	cmp r4, #0
	bge _080821FA
_08082216:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08082224: .4byte gUnknown_0203E754

	THUMB_FUNC_START sub_8082228
sub_8082228: @ 0x08082228
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r0, _08082264  @ gSinLookup
	mov ip, r0
	lsls r3, r3, #0x10
	asrs r5, r3, #0x10
	lsls r1, r1, #0x10
	lsls r2, r2, #0x10
	asrs r3, r2, #0x10
	asrs r1, r1, #0x10
	movs r6, #0xff
	movs r2, #0x9f
_08082240:
	adds r0, r1, #0
	ands r0, r6
	lsls r0, r0, #1
	add r0, ip
	movs r7, #0
	ldrsh r0, [r0, r7]
	muls r0, r3, r0
	asrs r0, r0, #0xc
	strh r0, [r4]
	adds r4, #2
	adds r1, r1, r5
	subs r2, #1
	cmp r2, #0
	bge _08082240
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08082264: .4byte gSinLookup

	THUMB_FUNC_START sub_8082268
sub_8082268: @ 0x08082268
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r0, _080822A8  @ gSinLookup
	mov ip, r0
	lsls r3, r3, #0x10
	asrs r5, r3, #0x10
	lsls r1, r1, #0x10
	lsls r2, r2, #0x10
	asrs r3, r2, #0x10
	asrs r1, r1, #0x10
	movs r6, #0xff
	movs r2, #0x9f
_08082280:
	adds r0, r1, #0
	ands r0, r6
	lsls r0, r0, #1
	add r0, ip
	movs r7, #0
	ldrsh r0, [r0, r7]
	muls r0, r3, r0
	asrs r0, r0, #0xc
	ldr r7, [sp, #0x14]
	adds r0, r0, r7
	strh r0, [r4]
	adds r4, #2
	adds r1, r1, r5
	subs r2, #1
	cmp r2, #0
	bge _08082280
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080822A8: .4byte gSinLookup

	THUMB_FUNC_START sub_80822AC
sub_80822AC: @ 0x080822AC
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	adds r4, #2
	ldr r0, _080822EC  @ gSinLookup
	mov ip, r0
	lsls r3, r3, #0x10
	asrs r3, r3, #0x10
	lsls r1, r1, #0x10
	asrs r1, r1, #0x10
	lsls r2, r2, #0x10
	asrs r5, r2, #0x10
	adds r1, r3, r1
	movs r6, #0xff
	lsls r3, r3, #1
	movs r2, #0x9e
_080822CA:
	adds r0, r1, #0
	ands r0, r6
	lsls r0, r0, #1
	add r0, ip
	movs r7, #0
	ldrsh r0, [r0, r7]
	muls r0, r5, r0
	asrs r0, r0, #0xc
	strh r0, [r4]
	adds r4, #4
	adds r1, r1, r3
	subs r2, #2
	cmp r2, #0
	bge _080822CA
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080822EC: .4byte gSinLookup

	THUMB_FUNC_START sub_80822F0
sub_80822F0: @ 0x080822F0
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	adds r4, #2
	ldr r0, _08082334  @ gSinLookup
	mov ip, r0
	lsls r3, r3, #0x10
	asrs r3, r3, #0x10
	lsls r1, r1, #0x10
	asrs r1, r1, #0x10
	lsls r2, r2, #0x10
	asrs r5, r2, #0x10
	adds r1, r3, r1
	movs r6, #0xff
	lsls r3, r3, #1
	movs r2, #0x9e
_0808230E:
	adds r0, r1, #0
	ands r0, r6
	lsls r0, r0, #1
	add r0, ip
	movs r7, #0
	ldrsh r0, [r0, r7]
	muls r0, r5, r0
	asrs r0, r0, #0xc
	ldr r7, [sp, #0x14]
	adds r0, r0, r7
	strh r0, [r4]
	adds r4, #4
	adds r1, r1, r3
	subs r2, #2
	cmp r2, #0
	bge _0808230E
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08082334: .4byte gSinLookup

	THUMB_FUNC_START sub_8082338
sub_8082338: @ 0x08082338
	push {r4, r5, r6, r7, lr}
	adds r4, r0, #0
	ldr r7, [sp, #0x18]
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r5, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r0, r3, #0x10
	ldr r2, [sp, #0x14]
	cmp r2, r7
	bge _08082386
	ldr r3, _0808238C  @ gSinLookup
	mov ip, r3
	lsls r0, r0, #0x10
	asrs r3, r0, #0x10
	lsls r1, r1, #0x10
	asrs r1, r1, #0x10
	lsls r0, r5, #0x10
	asrs r5, r0, #0x10
	adds r0, r2, #0
	muls r0, r3, r0
	adds r1, r0, r1
	movs r6, #0xff
	subs r2, r7, r2
_0808236A:
	adds r0, r1, #0
	ands r0, r6
	lsls r0, r0, #1
	add r0, ip
	movs r7, #0
	ldrsh r0, [r0, r7]
	muls r0, r5, r0
	asrs r0, r0, #0xc
	strh r0, [r4]
	adds r4, #2
	adds r1, r1, r3
	subs r2, #1
	cmp r2, #0
	bne _0808236A
_08082386:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0808238C: .4byte gSinLookup

	THUMB_FUNC_START sub_8082390
sub_8082390: @ 0x08082390
	ldr r0, _0808239C  @ gUnknown_0203E754
	ldr r2, [r0]
	ldr r1, [r0, #4]
	str r1, [r0]
	str r2, [r0, #4]
	bx lr
	.align 2, 0
_0808239C: .4byte gUnknown_0203E754

	THUMB_FUNC_START sub_80823A0
sub_80823A0: @ 0x080823A0
	push {lr}
	ldr r1, _080823B8  @ 0x0000F0F0
	adds r2, r1, #0
	movs r1, #0x9f
_080823A8:
	strh r2, [r0]
	adds r0, #2
	subs r1, #1
	cmp r1, #0
	bge _080823A8
	pop {r0}
	bx r0
	.align 2, 0
_080823B8: .4byte 0x0000F0F0

	THUMB_FUNC_START sub_80823BC
sub_80823BC: @ 0x080823BC
	push {lr}
	adds r3, r0, #0
	cmp r2, #0x9f
	bhi _080823D6
	cmp r1, #0
	bge _080823CA
	movs r1, #0
_080823CA:
	cmp r1, #0xf0
	ble _080823D0
	movs r1, #0xf0
_080823D0:
	lsls r0, r2, #1
	adds r0, r3, r0
	strb r1, [r0, #1]
_080823D6:
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_80823DC
sub_80823DC: @ 0x080823DC
	push {lr}
	adds r3, r0, #0
	cmp r2, #0x9f
	bhi _080823F6
	cmp r1, #0
	bge _080823EA
	movs r1, #0
_080823EA:
	cmp r1, #0xf0
	ble _080823F0
	movs r1, #0xf0
_080823F0:
	lsls r0, r2, #1
	adds r0, r3, r0
	strb r1, [r0]
_080823F6:
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_80823FC
sub_80823FC: @ 0x080823FC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	adds r6, r0, #0
	str r1, [sp]
	str r2, [sp, #4]
	adds r7, r3, #0
	str r7, [sp, #8]
	movs r0, #0
	mov sl, r0
	cmp r7, #0
	blt _080824B4
_0808241A:
	ldr r1, [sp]
	adds r4, r1, r7
	ldr r2, [sp, #4]
	add r2, sl
	str r2, [sp, #0xc]
	adds r0, r6, #0
	adds r1, r4, #0
	bl sub_80823DC
	ldr r0, [sp, #4]
	mov r1, sl
	subs r0, r0, r1
	mov r8, r0
	adds r0, r6, #0
	adds r1, r4, #0
	mov r2, r8
	bl sub_80823DC
	ldr r4, [sp]
	add r4, sl
	ldr r2, [sp, #4]
	adds r2, r2, r7
	mov r9, r2
	adds r0, r6, #0
	adds r1, r4, #0
	bl sub_80823DC
	ldr r0, [sp, #4]
	subs r5, r0, r7
	adds r0, r6, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823DC
	ldr r1, [sp]
	subs r4, r1, r7
	adds r0, r6, #0
	adds r1, r4, #0
	ldr r2, [sp, #0xc]
	bl sub_80823BC
	adds r0, r6, #0
	adds r1, r4, #0
	mov r2, r8
	bl sub_80823BC
	ldr r2, [sp]
	mov r0, sl
	subs r4, r2, r0
	adds r0, r6, #0
	adds r1, r4, #0
	mov r2, r9
	bl sub_80823BC
	adds r0, r6, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823BC
	ldr r1, [sp, #8]
	adds r1, #1
	mov r2, sl
	lsls r0, r2, #1
	subs r1, r1, r0
	str r1, [sp, #8]
	cmp r1, #0
	bge _080824AC
	subs r1, r7, #1
	lsls r0, r1, #1
	ldr r2, [sp, #8]
	adds r2, r2, r0
	str r2, [sp, #8]
	adds r7, r1, #0
_080824AC:
	movs r0, #1
	add sl, r0
	cmp r7, sl
	bge _0808241A
_080824B4:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_80824C4
sub_80824C4: @ 0x080824C4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x1c
	adds r7, r0, #0
	ldr r0, [sp, #0x3c]
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	str r1, [sp, #4]
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	str r2, [sp, #8]
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r9, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	mov r8, r0
	subs r0, r2, r1
	str r0, [sp, #0x10]
	movs r1, #0
	str r1, [sp, #0xc]
	mov sl, r1
	movs r1, #0x1f
	mov r0, r8
	ands r0, r1
	lsls r0, r0, #0x10
	str r0, [sp, #0x18]
	mov r0, r9
	ands r0, r1
	lsls r0, r0, #0x10
	str r0, [sp, #0x14]
_08082508:
	ldr r2, [sp, #4]
	cmp sl, r2
	bge _08082516
	mov r3, r9
	strh r3, [r7]
	adds r7, #2
	b _08082596
_08082516:
	ldr r0, [sp, #8]
	cmp sl, r0
	ble _08082524
	mov r1, r8
	strh r1, [r7]
	adds r7, #2
	b _08082596
_08082524:
	ldr r2, [sp, #0x10]
	str r2, [sp]
	movs r0, #0
	ldr r3, [sp, #0x14]
	lsrs r1, r3, #0x10
	ldr r3, [sp, #0x18]
	lsrs r2, r3, #0x10
	ldr r3, [sp, #0xc]
	bl sub_8012DCC
	adds r4, r0, #0
	mov r1, r9
	movs r0, #0xf8
	lsls r0, r0, #2
	ands r1, r0
	lsls r1, r1, #0x10
	mov r2, r8
	ands r2, r0
	lsls r2, r2, #0x10
	ldr r3, [sp, #0x10]
	str r3, [sp]
	movs r0, #0
	lsrs r1, r1, #0x10
	lsrs r2, r2, #0x10
	ldr r3, [sp, #0xc]
	bl sub_8012DCC
	adds r5, r0, #0
	movs r6, #0xf8
	lsls r6, r6, #7
	mov r1, r9
	ands r1, r6
	lsls r1, r1, #0x10
	mov r2, r8
	ands r2, r6
	lsls r2, r2, #0x10
	ldr r0, [sp, #0x10]
	str r0, [sp]
	movs r0, #0
	lsrs r1, r1, #0x10
	lsrs r2, r2, #0x10
	ldr r3, [sp, #0xc]
	bl sub_8012DCC
	movs r1, #0x1f
	ands r4, r1
	movs r2, #0xf8
	lsls r2, r2, #2
	ands r5, r2
	orrs r4, r5
	ands r0, r6
	orrs r4, r0
	strh r4, [r7]
	adds r7, #2
	ldr r3, [sp, #0xc]
	adds r3, #1
	str r3, [sp, #0xc]
_08082596:
	movs r0, #1
	add sl, r0
	mov r1, sl
	cmp r1, #0x9f
	ble _08082508
	add sp, #0x1c
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_80825B0
sub_80825B0: @ 0x080825B0
	adds r0, #0x64
	movs r1, #0
	strh r1, [r0]
	bx lr

	THUMB_FUNC_START sub_80825B8
sub_80825B8: @ 0x080825B8
	push {lr}
	ldr r1, _080825E4  @ gUnknown_0203E754
	ldr r3, [r1, #4]
	movs r1, #0xa0
	lsls r1, r1, #1
	adds r3, r3, r1
	adds r0, #0x64
	ldrh r1, [r0]
	adds r2, r1, #1
	strh r2, [r0]
	lsls r1, r1, #0x10
	asrs r1, r1, #0x10
	adds r0, r3, #0
	movs r2, #0x10
	movs r3, #8
	bl sub_8082228
	bl sub_8082390
	pop {r0}
	bx r0
	.align 2, 0
_080825E4: .4byte gUnknown_0203E754

	THUMB_FUNC_START sub_80825E8
sub_80825E8: @ 0x080825E8
	push {r4, r5, lr}
	ldr r0, _08082640  @ gUnknown_0203E754
	movs r2, #0x80
	lsls r2, r2, #5
	adds r1, r2, #0
	ldr r0, [r0]
	movs r2, #0x9f
_080825F6:
	strh r1, [r0]
	adds r0, #2
	subs r2, #1
	cmp r2, #0
	bge _080825F6
	ldr r0, _08082640  @ gUnknown_0203E754
	ldr r0, [r0]
	movs r1, #0x10
	adds r0, #0x10
	movs r2, #0x8f
_0808260A:
	strh r1, [r0]
	adds r0, #2
	subs r2, #1
	cmp r2, #0
	bge _0808260A
	movs r2, #0
	ldr r0, _08082640  @ gUnknown_0203E754
	ldr r0, [r0]
	movs r5, #0x10
	movs r1, #0x98
	lsls r1, r1, #1
	adds r4, r0, r1
	adds r3, r0, #0
_08082624:
	asrs r1, r2, #1
	subs r0, r5, r1
	lsls r0, r0, #8
	orrs r0, r1
	strh r0, [r3, #0x10]
	strh r0, [r4]
	subs r4, #2
	adds r3, #2
	adds r2, #1
	cmp r2, #0x20
	ble _08082624
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08082640: .4byte gUnknown_0203E754

	THUMB_FUNC_START sub_8082644
sub_8082644: @ 0x08082644
	ldr r2, _08082654  @ gUnknown_0203E754
	lsls r0, r0, #2
	adds r0, r0, r2
	lsls r1, r1, #1
	ldr r0, [r0]
	adds r0, r0, r1
	bx lr
	.align 2, 0
_08082654: .4byte gUnknown_0203E754

	THUMB_FUNC_START sub_8082658
sub_8082658: @ 0x08082658
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov r8, r0
	str r1, [sp]
	str r2, [sp, #4]
	adds r7, r3, #0
	mov sl, r7
	movs r0, #0
	mov r9, r0
	cmp r7, #0
	blt _0808271E
_08082676:
	ldr r6, [sp, #4]
	add r6, r9
	adds r0, r6, #0
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _080826BA
	ldr r2, [sp]
	adds r4, r2, r7
	mov r0, r8
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_80823DC
	ldr r0, [sp, #4]
	mov r1, r9
	subs r5, r0, r1
	mov r0, r8
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823DC
	ldr r2, [sp]
	subs r4, r2, r7
	mov r0, r8
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_80823BC
	mov r0, r8
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823BC
_080826BA:
	ldr r0, [sp, #4]
	adds r6, r0, r7
	adds r0, r6, #0
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _080826FE
	ldr r4, [sp]
	add r4, r9
	mov r0, r8
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_80823DC
	ldr r2, [sp, #4]
	subs r5, r2, r7
	mov r0, r8
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823DC
	ldr r0, [sp]
	mov r1, r9
	subs r4, r0, r1
	mov r0, r8
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_80823BC
	mov r0, r8
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823BC
_080826FE:
	mov r1, sl
	adds r1, #1
	mov r2, r9
	lsls r0, r2, #1
	subs r1, r1, r0
	mov sl, r1
	cmp r1, #0
	bge _08082716
	subs r1, r7, #1
	lsls r0, r1, #1
	add sl, r0
	adds r7, r1, #0
_08082716:
	movs r0, #1
	add r9, r0
	cmp r7, r9
	bge _08082676
_0808271E:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_8082730
sub_8082730: @ 0x08082730
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r5, r0, #0
	adds r6, r1, #0
	mov r8, r2
	ldr r4, _08082760  @ gUnknown_0203E754
	ldr r0, [r4, #4]
	bl sub_80823A0
	ldr r0, [r4, #4]
	adds r1, r5, #0
	adds r2, r6, #0
	mov r3, r8
	bl sub_8082658
	bl sub_8082390
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08082760: .4byte gUnknown_0203E754

	THUMB_FUNC_START sub_8082764
sub_8082764: @ 0x08082764
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r1, r0, #0
	cmp r1, #0x70
	ble _08082776
	movs r1, #0x70
_08082776:
	movs r0, #0x50
	subs r4, r0, r1
	adds r1, #0x50
	mov ip, r1
	cmp r4, #0
	ble _08082798
	ldr r0, _08082858  @ gUnknown_0203E754
	movs r2, #0x80
	lsls r2, r2, #5
	adds r1, r2, #0
	ldr r0, [r0, #4]
	adds r2, r4, #0
_0808278E:
	strh r1, [r0]
	adds r0, #2
	subs r2, #1
	cmp r2, #0
	bne _0808278E
_08082798:
	mov r2, ip
	movs r7, #0x20
	adds r7, r7, r4
	mov sl, r7
	subs r0, r2, #1
	mov r9, r0
	adds r6, r2, #0
	subs r6, #0x20
	cmp r2, #0x9f
	bgt _080827C4
	ldr r0, _08082858  @ gUnknown_0203E754
	ldr r1, [r0, #4]
	movs r7, #0x80
	lsls r7, r7, #5
	adds r3, r7, #0
	lsls r0, r2, #1
	adds r0, r0, r1
_080827BA:
	strh r3, [r0]
	adds r0, #2
	adds r2, #1
	cmp r2, #0x9f
	ble _080827BA
_080827C4:
	adds r2, r4, #0
	cmp r2, #0x4f
	bgt _080827F6
	cmp r2, sl
	bge _080827F6
	ldr r0, _08082858  @ gUnknown_0203E754
	ldr r1, [r0, #4]
	movs r0, #0x10
	mov r8, r0
	mov r5, sl
	lsls r0, r2, #1
	adds r3, r0, r1
_080827DC:
	subs r1, r2, r4
	asrs r1, r1, #1
	mov r7, r8
	subs r0, r7, r1
	lsls r0, r0, #8
	orrs r0, r1
	strh r0, [r3]
	adds r3, #2
	adds r2, #1
	cmp r2, #0x4f
	bgt _080827F6
	cmp r2, r5
	blt _080827DC
_080827F6:
	mov r2, r9
	cmp r2, #0x4f
	ble _08082826
	adds r4, r6, #0
	cmp r2, r6
	blt _08082826
	ldr r0, _08082858  @ gUnknown_0203E754
	ldr r1, [r0, #4]
	movs r5, #0x10
	lsls r0, r2, #1
	adds r3, r0, r1
_0808280C:
	mov r0, ip
	subs r1, r0, r2
	asrs r1, r1, #1
	subs r0, r5, r1
	lsls r0, r0, #8
	orrs r0, r1
	strh r0, [r3]
	subs r3, #2
	subs r2, #1
	cmp r2, #0x4f
	ble _08082826
	cmp r2, r4
	bge _0808280C
_08082826:
	mov r2, sl
	adds r3, r6, #0
	cmp r2, r3
	bge _08082844
	ldr r0, _08082858  @ gUnknown_0203E754
	ldr r1, [r0, #4]
	movs r4, #0x10
	lsls r0, r2, #1
	adds r0, r0, r1
	subs r2, r3, r2
_0808283A:
	strh r4, [r0]
	adds r0, #2
	subs r2, #1
	cmp r2, #0
	bne _0808283A
_08082844:
	bl sub_8082390
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08082858: .4byte gUnknown_0203E754

	THUMB_FUNC_START sub_808285C
sub_808285C: @ 0x0808285C
	push {lr}
	ldr r0, _08082874  @ 0x04000006
	ldrh r0, [r0]
	adds r3, r0, #0
	cmp r3, #0x9e
	bls _08082880
	ldr r0, _08082878  @ gUnknown_0203E75C
	ldr r1, _0808287C  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r3, #0
	b _08082886
	.align 2, 0
_08082874: .4byte 0x04000006
_08082878: .4byte gUnknown_0203E75C
_0808287C: .4byte gUnknown_0203E754
_08082880:
	adds r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
_08082886:
	movs r0, #1
	ands r0, r3
	cmp r0, #0
	bne _0808289C
	ldr r2, _080828A0  @ 0x04000040
	ldr r0, _080828A4  @ gUnknown_0203E75C
	ldr r1, [r0]
	lsls r0, r3, #1
	adds r0, r0, r1
	ldrh r0, [r0]
	strh r0, [r2]
_0808289C:
	pop {r0}
	bx r0
	.align 2, 0
_080828A0: .4byte 0x04000040
_080828A4: .4byte gUnknown_0203E75C

	THUMB_FUNC_START sub_80828A8
sub_80828A8: @ 0x080828A8
	push {lr}
	ldr r0, _080828C0  @ 0x04000006
	ldrh r0, [r0]
	adds r3, r0, #0
	cmp r3, #0x9e
	bls _080828CC
	ldr r0, _080828C4  @ gUnknown_0203E75C
	ldr r1, _080828C8  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r3, #0
	b _080828D4
	.align 2, 0
_080828C0: .4byte 0x04000006
_080828C4: .4byte gUnknown_0203E75C
_080828C8: .4byte gUnknown_0203E754
_080828CC:
	adds r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	ldr r0, _080828E4  @ gUnknown_0203E75C
_080828D4:
	ldr r2, _080828E8  @ 0x04000052
	ldr r1, [r0]
	lsls r0, r3, #1
	adds r0, r0, r1
	ldrh r0, [r0]
	strh r0, [r2]
	pop {r0}
	bx r0
	.align 2, 0
_080828E4: .4byte gUnknown_0203E75C
_080828E8: .4byte 0x04000052

	THUMB_FUNC_START sub_80828EC
sub_80828EC: @ 0x080828EC
	push {lr}
	ldr r0, _08082910  @ 0x04000006
	ldrh r0, [r0]
	adds r0, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #0x9f
	bls _080828FE
	movs r3, #0
_080828FE:
	ldr r2, _08082914  @ 0x0400001A
	ldr r0, _08082918  @ gUnknown_0203E75C
	ldr r1, [r0]
	lsls r0, r3, #1
	adds r0, r0, r1
	ldrh r0, [r0]
	strh r0, [r2]
	pop {r0}
	bx r0
	.align 2, 0
_08082910: .4byte 0x04000006
_08082914: .4byte 0x0400001A
_08082918: .4byte gUnknown_0203E75C

	THUMB_FUNC_START sub_808291C
sub_808291C: @ 0x0808291C
	push {r4, r5, lr}
	movs r3, #0
	ldr r5, _08082948  @ gUnknown_0203E75C
	movs r4, #1
_08082924:
	ldr r0, [r5]
	lsls r2, r3, #1
	adds r2, r2, r0
	adds r0, r3, #0
	ands r0, r4
	asrs r1, r3, #1
	adds r0, r0, r1
	negs r0, r0
	strh r0, [r2]
	adds r3, #1
	cmp r3, #0x9f
	ble _08082924
	bl sub_8082390
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08082948: .4byte gUnknown_0203E75C

	THUMB_FUNC_START sub_808294C
sub_808294C: @ 0x0808294C
	push {lr}
	ldr r0, _08082964  @ 0x04000006
	ldrh r0, [r0]
	adds r1, r0, #0
	cmp r1, #0x9f
	bls _08082970
	ldr r0, _08082968  @ gUnknown_0203E75C
	ldr r1, _0808296C  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r1, #0
	b _08082976
	.align 2, 0
_08082964: .4byte 0x04000006
_08082968: .4byte gUnknown_0203E75C
_0808296C: .4byte gUnknown_0203E754
_08082970:
	adds r0, r1, #1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
_08082976:
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08082998
	ldr r2, _0808299C  @ 0x04000014
	ldr r0, _080829A0  @ gUnknown_0203E75C
	ldr r0, [r0]
	lsls r1, r1, #1
	adds r1, r1, r0
	movs r3, #0xa0
	lsls r3, r3, #1
	adds r0, r1, r3
	ldrh r0, [r0]
	strh r0, [r2]
	adds r2, #2
	ldrh r0, [r1]
	strh r0, [r2]
_08082998:
	pop {r0}
	bx r0
	.align 2, 0
_0808299C: .4byte 0x04000014
_080829A0: .4byte gUnknown_0203E75C

	THUMB_FUNC_START sub_80829A4
sub_80829A4: @ 0x080829A4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r0, #0
	ldr r0, [sp, #0x20]
	mov sl, r0
	ldr r4, [sp, #0x24]
	ldr r0, [sp, #0x28]
	adds r6, #2
	movs r7, #1
	ldr r5, _08082A20  @ gSinLookup
	mov r9, r5
	lsls r3, r3, #0x10
	asrs r3, r3, #0x10
	lsls r1, r1, #0x10
	asrs r1, r1, #0x10
	lsls r2, r2, #0x10
	asrs r2, r2, #0x10
	mov r8, r2
	lsls r0, r0, #0x10
	asrs r0, r0, #0x10
	mov ip, r0
	adds r2, r3, r1
	lsls r4, r4, #0x10
	asrs r4, r4, #0x10
	mov r0, sl
	lsls r5, r0, #0x10
	asrs r5, r5, #0x10
	lsls r3, r3, #1
_080829E2:
	movs r0, #0xff
	ands r0, r2
	lsls r0, r0, #1
	add r0, r9
	movs r1, #0
	ldrsh r0, [r0, r1]
	mov r1, r8
	muls r1, r0, r1
	subs r0, r7, r4
	cmp r0, #0
	bge _080829FA
	subs r0, r4, r7
_080829FA:
	muls r0, r1, r0
	mov r1, ip
	muls r1, r0, r1
	asrs r0, r1, #0x14
	adds r0, r5, r0
	strh r0, [r6]
	adds r6, #4
	adds r2, r2, r3
	adds r7, #2
	cmp r7, #0x9f
	ble _080829E2
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08082A20: .4byte gSinLookup

	THUMB_FUNC_START sub_8082A24
sub_8082A24: @ 0x08082A24
	push {lr}
	ldr r0, _08082A3C  @ 0x04000006
	ldrh r0, [r0]
	adds r1, r0, #0
	cmp r1, #0x9f
	bls _08082A48
	ldr r0, _08082A40  @ gUnknown_0203E75C
	ldr r1, _08082A44  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r1, #0
	b _08082A4E
	.align 2, 0
_08082A3C: .4byte 0x04000006
_08082A40: .4byte gUnknown_0203E75C
_08082A44: .4byte gUnknown_0203E754
_08082A48:
	adds r0, r1, #1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
_08082A4E:
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08082A70
	ldr r2, _08082A74  @ 0x04000010
	ldr r0, _08082A78  @ gUnknown_0203E75C
	ldr r0, [r0]
	lsls r1, r1, #1
	adds r1, r1, r0
	movs r3, #0xa0
	lsls r3, r3, #1
	adds r0, r1, r3
	ldrh r0, [r0]
	strh r0, [r2]
	adds r2, #2
	ldrh r0, [r1]
	strh r0, [r2]
_08082A70:
	pop {r0}
	bx r0
	.align 2, 0
_08082A74: .4byte 0x04000010
_08082A78: .4byte gUnknown_0203E75C

	THUMB_FUNC_START sub_8082A7C
sub_8082A7C: @ 0x08082A7C
	push {lr}
	ldr r0, _08082A94  @ 0x04000006
	ldrh r0, [r0]
	adds r1, r0, #0
	cmp r1, #0x9f
	bls _08082AA0
	ldr r0, _08082A98  @ gUnknown_0203E75C
	ldr r1, _08082A9C  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r1, #0
	b _08082AA6
	.align 2, 0
_08082A94: .4byte 0x04000006
_08082A98: .4byte gUnknown_0203E75C
_08082A9C: .4byte gUnknown_0203E754
_08082AA0:
	adds r0, r1, #1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
_08082AA6:
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08082AC8
	ldr r2, _08082ACC  @ 0x04000014
	ldr r0, _08082AD0  @ gUnknown_0203E75C
	ldr r0, [r0]
	lsls r1, r1, #1
	adds r1, r1, r0
	movs r3, #0xa0
	lsls r3, r3, #1
	adds r0, r1, r3
	ldrh r0, [r0]
	strh r0, [r2]
	adds r2, #2
	ldrh r0, [r1]
	strh r0, [r2]
_08082AC8:
	pop {r0}
	bx r0
	.align 2, 0
_08082ACC: .4byte 0x04000014
_08082AD0: .4byte gUnknown_0203E75C

	THUMB_FUNC_START sub_8082AD4
sub_8082AD4: @ 0x08082AD4
	push {lr}
	ldr r0, _08082AEC  @ 0x04000006
	ldrh r0, [r0]
	adds r1, r0, #0
	cmp r1, #0x9f
	bls _08082AF8
	ldr r0, _08082AF0  @ gUnknown_0203E75C
	ldr r1, _08082AF4  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r1, #0
	b _08082AFE
	.align 2, 0
_08082AEC: .4byte 0x04000006
_08082AF0: .4byte gUnknown_0203E75C
_08082AF4: .4byte gUnknown_0203E754
_08082AF8:
	adds r0, r1, #1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
_08082AFE:
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08082B20
	ldr r2, _08082B24  @ 0x04000018
	ldr r0, _08082B28  @ gUnknown_0203E75C
	ldr r0, [r0]
	lsls r1, r1, #1
	adds r1, r1, r0
	movs r3, #0xa0
	lsls r3, r3, #1
	adds r0, r1, r3
	ldrh r0, [r0]
	strh r0, [r2]
	adds r2, #2
	ldrh r0, [r1]
	strh r0, [r2]
_08082B20:
	pop {r0}
	bx r0
	.align 2, 0
_08082B24: .4byte 0x04000018
_08082B28: .4byte gUnknown_0203E75C

	THUMB_FUNC_START sub_8082B2C
sub_8082B2C: @ 0x08082B2C
	push {lr}
	ldr r0, _08082B44  @ 0x04000006
	ldrh r0, [r0]
	adds r1, r0, #0
	cmp r1, #0x9f
	bls _08082B50
	ldr r0, _08082B48  @ gUnknown_0203E75C
	ldr r1, _08082B4C  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r1, #0
	b _08082B56
	.align 2, 0
_08082B44: .4byte 0x04000006
_08082B48: .4byte gUnknown_0203E75C
_08082B4C: .4byte gUnknown_0203E754
_08082B50:
	adds r0, r1, #1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
_08082B56:
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08082B78
	ldr r2, _08082B7C  @ 0x0400001C
	ldr r0, _08082B80  @ gUnknown_0203E75C
	ldr r0, [r0]
	lsls r1, r1, #1
	adds r1, r1, r0
	movs r3, #0xa0
	lsls r3, r3, #1
	adds r0, r1, r3
	ldrh r0, [r0]
	strh r0, [r2]
	adds r2, #2
	ldrh r0, [r1]
	strh r0, [r2]
_08082B78:
	pop {r0}
	bx r0
	.align 2, 0
_08082B7C: .4byte 0x0400001C
_08082B80: .4byte gUnknown_0203E75C

	THUMB_FUNC_START sub_8082B84
sub_8082B84: @ 0x08082B84
	push {r4, lr}
	ldr r0, _08082B9C  @ 0x04000006
	ldrh r0, [r0]
	adds r2, r0, #0
	cmp r2, #0x9f
	bls _08082BA8
	ldr r0, _08082BA0  @ gUnknown_0203E75C
	ldr r1, _08082BA4  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r2, #0
	b _08082BAE
	.align 2, 0
_08082B9C: .4byte 0x04000006
_08082BA0: .4byte gUnknown_0203E75C
_08082BA4: .4byte gUnknown_0203E754
_08082BA8:
	adds r0, r2, #1
	lsls r0, r0, #0x10
	lsrs r2, r0, #0x10
_08082BAE:
	movs r0, #1
	ands r0, r2
	cmp r0, #0
	beq _08082BDA
	ldr r3, _08082BE0  @ 0x04000018
	ldr r0, _08082BE4  @ gUnknown_0203E75C
	ldr r0, [r0]
	lsls r2, r2, #1
	adds r2, r2, r0
	movs r1, #0xa0
	lsls r1, r1, #1
	adds r0, r2, r1
	ldr r4, _08082BE8  @ gLCDControlBuffer
	ldrh r0, [r0]
	ldrh r1, [r4, #0x24]
	adds r0, r0, r1
	strh r0, [r3]
	adds r3, #2
	ldrh r0, [r2]
	ldrh r1, [r4, #0x26]
	adds r0, r0, r1
	strh r0, [r3]
_08082BDA:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08082BE0: .4byte 0x04000018
_08082BE4: .4byte gUnknown_0203E75C
_08082BE8: .4byte gLCDControlBuffer

	THUMB_FUNC_START sub_8082BEC
sub_8082BEC: @ 0x08082BEC
	push {lr}
	ldr r0, _08082C04  @ 0x04000006
	ldrh r0, [r0]
	adds r1, r0, #0
	cmp r1, #0x9f
	bls _08082C10
	ldr r0, _08082C08  @ gUnknown_0203E75C
	ldr r1, _08082C0C  @ gUnknown_0203E754
	ldr r1, [r1]
	str r1, [r0]
	movs r1, #0
	b _08082C16
	.align 2, 0
_08082C04: .4byte 0x04000006
_08082C08: .4byte gUnknown_0203E75C
_08082C0C: .4byte gUnknown_0203E754
_08082C10:
	adds r0, r1, #1
	lsls r0, r0, #0x10
	lsrs r1, r0, #0x10
_08082C16:
	movs r0, #1
	ands r0, r1
	cmp r0, #0
	beq _08082C44
	ldr r2, _08082C48  @ 0x04000014
	ldr r0, _08082C4C  @ gUnknown_0203E75C
	ldr r0, [r0]
	lsls r1, r1, #1
	adds r1, r1, r0
	movs r0, #0xa0
	lsls r0, r0, #1
	adds r3, r1, r0
	ldrh r0, [r3]
	strh r0, [r2]
	adds r2, #2
	ldrh r0, [r1]
	strh r0, [r2]
	adds r2, #2
	ldrh r0, [r3]
	strh r0, [r2]
	adds r2, #2
	ldrh r0, [r1]
	strh r0, [r2]
_08082C44:
	pop {r0}
	bx r0
	.align 2, 0
_08082C48: .4byte 0x04000014
_08082C4C: .4byte gUnknown_0203E75C

	THUMB_FUNC_START sub_8082C50
sub_8082C50: @ 0x08082C50
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x24
	adds r7, r0, #0
	str r1, [sp]
	str r2, [sp, #4]
	str r3, [sp, #8]
	ldr r0, [sp, #0x44]
	cmp r3, r0
	ble _08082D4E
	mov r9, r3
	mov r1, r9
	str r1, [sp, #0x10]
	movs r2, #0
	str r2, [sp, #0xc]
	cmp r1, #0
	bge _08082C7A
	b _08082E30
_08082C7A:
	movs r0, #0
	str r0, [sp, #0x1c]
	ldr r2, [sp, #0x44]
	mov r1, r9
	muls r1, r2, r1
	str r1, [sp, #0x20]
_08082C86:
	ldr r0, [sp, #0x20]
	ldr r1, [sp, #8]
	bl __divsi3
	mov sl, r0
	ldr r0, [sp, #0x1c]
	ldr r1, [sp, #8]
	bl __divsi3
	mov r8, r0
	ldr r4, [sp]
	add r4, r9
	ldr r6, [sp, #4]
	add r6, r8
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_80823DC
	ldr r0, [sp, #4]
	mov r1, r8
	subs r5, r0, r1
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823DC
	ldr r2, [sp]
	mov r0, r9
	subs r4, r2, r0
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_80823BC
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823BC
	ldr r1, [sp]
	ldr r2, [sp, #0xc]
	adds r4, r1, r2
	ldr r6, [sp, #4]
	add r6, sl
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_80823DC
	ldr r0, [sp, #4]
	mov r1, sl
	subs r5, r0, r1
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823DC
	ldr r2, [sp]
	ldr r0, [sp, #0xc]
	subs r4, r2, r0
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_80823BC
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823BC
	ldr r1, [sp, #0x10]
	adds r1, #1
	ldr r2, [sp, #0xc]
	lsls r0, r2, #1
	subs r1, r1, r0
	str r1, [sp, #0x10]
	cmp r1, #0
	bge _08082D3A
	mov r1, r9
	subs r1, #1
	lsls r0, r1, #1
	ldr r2, [sp, #0x10]
	adds r2, r2, r0
	str r2, [sp, #0x10]
	ldr r0, [sp, #0x20]
	ldr r2, [sp, #0x44]
	subs r0, r0, r2
	str r0, [sp, #0x20]
	mov r9, r1
_08082D3A:
	ldr r0, [sp, #0x1c]
	ldr r1, [sp, #0x44]
	adds r0, r0, r1
	str r0, [sp, #0x1c]
	ldr r2, [sp, #0xc]
	adds r2, #1
	str r2, [sp, #0xc]
	cmp r9, r2
	bge _08082C86
	b _08082E30
_08082D4E:
	ldr r0, [sp, #0x44]
	mov r9, r0
	mov r1, r9
	str r1, [sp, #0x10]
	movs r2, #0
	str r2, [sp, #0xc]
	cmp r1, #0
	blt _08082E30
	movs r0, #0
	str r0, [sp, #0x14]
	ldr r2, [sp, #8]
	mov r1, r9
	muls r1, r2, r1
	str r1, [sp, #0x18]
_08082D6A:
	ldr r0, [sp, #0x18]
	ldr r1, [sp, #0x44]
	bl __divsi3
	mov sl, r0
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x44]
	bl __divsi3
	mov r8, r0
	ldr r4, [sp]
	add r4, sl
	ldr r0, [sp, #4]
	ldr r1, [sp, #0xc]
	adds r6, r0, r1
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_80823DC
	ldr r2, [sp, #4]
	ldr r0, [sp, #0xc]
	subs r5, r2, r0
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823DC
	ldr r1, [sp]
	mov r2, sl
	subs r4, r1, r2
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_80823BC
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823BC
	ldr r4, [sp]
	add r4, r8
	ldr r6, [sp, #4]
	add r6, r9
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_80823DC
	ldr r0, [sp, #4]
	mov r1, r9
	subs r5, r0, r1
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823DC
	ldr r2, [sp]
	mov r0, r8
	subs r4, r2, r0
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r6, #0
	bl sub_80823BC
	adds r0, r7, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_80823BC
	ldr r1, [sp, #0x10]
	adds r1, #1
	ldr r2, [sp, #0xc]
	lsls r0, r2, #1
	subs r1, r1, r0
	str r1, [sp, #0x10]
	cmp r1, #0
	bge _08082E1E
	mov r1, r9
	subs r1, #1
	lsls r0, r1, #1
	ldr r2, [sp, #0x10]
	adds r2, r2, r0
	str r2, [sp, #0x10]
	ldr r0, [sp, #0x18]
	ldr r2, [sp, #8]
	subs r0, r0, r2
	str r0, [sp, #0x18]
	mov r9, r1
_08082E1E:
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #8]
	adds r0, r0, r1
	str r0, [sp, #0x14]
	ldr r2, [sp, #0xc]
	adds r2, #1
	str r2, [sp, #0xc]
	cmp r9, r2
	bge _08082D6A
_08082E30:
	add sp, #0x24
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

	THUMB_FUNC_START sub_8082E40
sub_8082E40: @ 0x08082E40
	push {r4, r5, r6, lr}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	sub sp, #4
	adds r6, r0, #0
	mov r8, r1
	mov r9, r2
	adds r5, r3, #0
	ldr r4, _08082E7C  @ gUnknown_0203E754
	ldr r0, [r4, #4]
	bl sub_80823A0
	ldr r0, [r4, #4]
	str r5, [sp]
	adds r1, r6, #0
	mov r2, r8
	mov r3, r9
	bl sub_8082C50
	bl sub_8082390
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08082E7C: .4byte gUnknown_0203E754

.align 2, 0
