	.INCLUDE "macro.inc"

	.SYNTAX UNIFIED

	.global ARMCodeToCopy_Start
ARMCodeToCopy_Start:

_08000228: .4byte gPaletteBuffer @ pool
_0800022C: .4byte gUnknown_020222A8 @ pool
_08000230: .4byte gUnknown_02022288 @ pool

	ARM_FUNC_START sub_8000234
sub_8000234: @ 0x08000234
	push {r4, r5, r6, r7}
	mov r7, #0x3e0
_0800023C:
	ldr r0, _08000230
	add r0, r0, r7, lsr #5
	ldrsb r5, [r0]
	tst r5, r5
	beq _080002F4
	ldr r4, _0800022C
	lsr r0, r7, #1
	add r0, r0, r0, lsl #1
	add r0, r0, #0x30
	add r4, r4, r0
	mov r6, #0x1e
_08000268:
	sub r4, r4, #3
	ldrb r0, [r4]
	add r0, r0, r5
	strb r0, [r4]
	subs r0, r0, #0x20
	bpl _08000284
	mov r0, #0
_08000284:
	cmp r0, #0x20
	blo _08000290
	mov r0, #0x1f
_08000290:
	ldrb r1, [r4, #1]
	add r1, r1, r5
	strb r1, [r4, #1]
	subs r1, r1, #0x20
	bpl _080002A8
	mov r1, #0
_080002A8:
	cmp r1, #0x20
	blo _080002B4
	mov r1, #0x1f
_080002B4:
	ldrb r2, [r4, #2]
	add r2, r2, r5
	strb r2, [r4, #2]
	subs r2, r2, #0x20
	bpl _080002CC
	mov r2, #0
_080002CC:
	cmp r2, #0x20
	blo _080002D8
	mov r2, #0x1f
_080002D8:
	add r0, r0, r1, lsl #5
	add r0, r0, r2, lsl #10
	ldr r1, _08000228
	add r1, r1, r6
	strh r0, [r1, r7]
	subs r6, r6, #2
	bpl _08000268
_080002F4:
	subs r7, r7, #0x20
	bpl _0800023C
	pop {r4, r5, r6, r7}
	bx lr

	ARM_FUNC_START Store160To80Structs
Store160To80Structs: @ 0x08000304
	lsr r1, r1, #4
	sub r1, r1, #1
	mov r2, #0xa0
_08000310:
	str r2, [r0]
	str r2, [r0, #8]
	str r2, [r0, #0x10]
	str r2, [r0, #0x18]
	str r2, [r0, #0x20]
	str r2, [r0, #0x28]
	str r2, [r0, #0x30]
	str r2, [r0, #0x38]
	str r2, [r0, #0x40]
	str r2, [r0, #0x48]
	str r2, [r0, #0x50]
	str r2, [r0, #0x58]
	str r2, [r0, #0x60]
	str r2, [r0, #0x68]
	str r2, [r0, #0x70]
	str r2, [r0, #0x78]
	add r0, r0, #0x80
	subs r1, r1, #1
	bpl _08000310
	bx lr

	ARM_FUNC_START sub_8000360
sub_8000360: @ 0x08000360
	push {r4, r5, r6, r7}
	sub r1, r1, #2
	mov r2, #0
	mov r3, #0
_08000370:
	ldrh r4, [r0]
	add r2, r2, r4
	eor r3, r3, r4
	add r0, r0, #2
	subs r1, r1, #2
	bpl _08000370
	mov r0, #0x10000
	sub r0, r0, #1
	and r2, r2, r0
	lsl r3, r3, #0x10
	mov r0, r2
	add r0, r0, r3
	pop {r4, r5, r6, r7}
	bx lr

	ARM_FUNC_START ARM_FillRect
ARM_FillRect: @ 0x080003A8
	@ r0 = destination
	@ r1 = width
	@ r2 = height
	@ r3 = fillValue
	push {r4, r5, r6, r7}
	mov r4, r0
	sub r6, r2, #0
  1:
	sub r5, r1, #0
  2:
	strh r3, [r4]
	add r4, r4, #2
	subs r5, r5, #1		@ decrement width counter
	bpl 2b
	add r0, r0, #0x40
	mov r4, r0
	subs r6, r6, #1		@ decrement height counter
	bpl 1b
	pop {r4, r5, r6, r7}
	bx lr

	ARM_FUNC_START sub_80003E0
sub_80003E0: @ 0x080003E0
	push {r4, r5, r6, r7}
	tst r2, r2
	beq _08000434
	bmi _08000434
	tst r3, r3
	beq _08000434
	bmi _08000434
	mov r4, #0x40
	sub r4, r4, r2, lsl #1
	sub r6, r3, #1
_08000408:
	sub r5, r2, #1
_0800040C:
	ldrh r7, [r0]
	strh r7, [r1]
	add r0, r0, #2
	add r1, r1, #2
	subs r5, r5, #1
	bpl _0800040C
	add r0, r0, r4
	add r1, r1, r4
	subs r6, r6, #1
	bpl _08000408
_08000434:
	pop {r4, r5, r6, r7}
	bx lr

	ARM_FUNC_START sub_800043C
sub_800043C: @ 0x0800043C
	push {r4, r5, r6, r7}
	ldrb r3, [r1]
	ldrb r4, [r1, #1]
	add r1, r1, #2
	lsl r7, r4, #6
	add r0, r0, r7
	mov r6, r4
_08000458:
	mov r5, r3
_0800045C:
	ldrh r7, [r1]
	add r7, r7, r2
	strh r7, [r0]
	add r0, r0, #2
	add r1, r1, #2
	subs r5, r5, #1
	bpl _0800045C
	sub r0, r0, r3, lsl #1
	sub r0, r0, #0x42
	subs r6, r6, #1
	bpl _08000458
	pop {r4, r5, r6, r7}
	bx lr

	.align 2, 0
_08000490: .4byte gUnknown_03003744 @ pool

	ARM_FUNC_START IRAMARM_CopyToSecondaryOAM
IRAMARM_CopyToSecondaryOAM: @ 0x08000494
	push {r4, r5, r6, r7}
	ldr r7, _08000490
_0800049C:
	ldr r5, [r7]
	ldrh r4, [r2]
	tst r4, r4
	beq _08000528
	bmi _08000528
	add r2, r2, #2
	add r6, r5, r4, lsl #3
	str r6, [r7]
	mov r7, #0x10000
	sub r7, r7, #1
	and r0, r0, r7
	and r1, r1, r7
	orr r0, r0, r1, lsl #16
_080004D0:
	ldrh r1, [r2]
	orr r6, r1, r0, lsr #16
	and r6, r6, #0xff00
	add r7, r1, r0, lsr #16
	and r7, r7, #0xff
	orr r6, r6, r7
	strh r6, [r5]
	ldrh r1, [r2, #2]
	orr r6, r1, r0
	and r6, r6, #0xfe00
	add r7, r1, r0
	lsl r7, r7, #0x17
	lsr r7, r7, #0x17
	orr r6, r6, r7
	strh r6, [r5, #2]
	ldrh r1, [r2, #4]
	add r6, r1, r3
	strh r6, [r5, #4]
	add r2, r2, #6
	add r5, r5, #8
	subs r4, r4, #1
	bne _080004D0
_08000528:
	pop {r4, r5, r6, r7}
	bx lr
	.align 2, 0
_08000530: .4byte gUnknown_03003070 @ pool

	ARM_FUNC_START IRAMARM_CopyToPrimaryOAM
IRAMARM_CopyToPrimaryOAM: @ 0x08000534
	push {r4, r5, r6, r7}
	ldr r7, _08000530
	b _0800049C

_08000540:
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000010
	.4byte 0x00000040
	.4byte 0x00000100
	.4byte 0x00000400
	.4byte 0x00001000
	.4byte 0x00004000
_08000560: .4byte _08000540 @ pool

	ARM_FUNC_START IRAMARM_Func3_DrawGlyph
IRAMARM_Func3_DrawGlyph: @ 0x08000564
	push {r4, r5, r6, r7, r8, r9, sl}
	mov r9, #0xf
	mov sl, #0x10000
	sub sl, sl, #1
_08000574:
	ldr r4, _08000560  @ _08000540
	ldr r5, [r4, r3, lsl #2]
	ldr r4, [r2]
	umull r5, r6, r4, r5
	mov r7, r5
	and r7, r7, #0xff
	ldr r7, [r0, r7, lsl #1]
	lsr r8, r5, #8
	and r8, r8, #0xff
	ldr r8, [r0, r8, lsl #1]
	and r7, r7, sl
	orr r7, r7, r8, lsl #16
	ldr r4, [r1]
	orr r4, r4, r7
	str r4, [r1]
	lsr r7, r5, #0x10
	and r7, r7, #0xff
	ldr r7, [r0, r7, lsl #1]
	lsr r8, r5, #0x18
	and r8, r8, #0xff
	ldr r8, [r0, r8, lsl #1]
	and r7, r7, sl
	orr r7, r7, r8, lsl #16
	ldr r4, [r1, #0x40]
	orr r4, r4, r7
	str r4, [r1, #0x40]
	mov r7, r6
	and r7, r7, #0xff
	ldr r7, [r0, r7, lsl #1]
	lsr r8, r6, #8
	and r8, r8, #0xff
	ldr r8, [r0, r8, lsl #1]
	and r7, r7, sl
	orr r7, r7, r8, lsl #16
	ldr r4, [r1, #0x80]
	orr r4, r4, r7
	str r4, [r1, #0x80]
	add r1, r1, #4
	add r2, r2, #4
	subs r9, r9, #1
	bpl _08000574
	pop {r4, r5, r6, r7, r8, r9, sl}
	bx lr

	ARM_FUNC_START sub_8000620
sub_8000620: @ 0x08000620
	push {r4, r5, r6, r7, r8, r9, sl}
	mov r9, #7
	mov sl, #0x10000
	sub sl, sl, #1
_08000630:
	ldr r4, _08000560  @ gUnknown_08000540
	ldr r5, [r4, r3, lsl #2]
	ldr r4, [r2]
	umull r5, r6, r4, r5
	mov r7, r5
	and r7, r7, #0xff
	ldr r7, [r0, r7, lsl #1]
	lsr r8, r5, #8
	and r8, r8, #0xff
	ldr r8, [r0, r8, lsl #1]
	and r7, r7, sl
	orr r7, r7, r8, lsl #16
	ldr r4, [r1]
	orr r4, r4, r7
	str r4, [r1]
	lsr r7, r5, #0x10
	and r7, r7, #0xff
	ldr r7, [r0, r7, lsl #1]
	lsr r8, r5, #0x18
	and r8, r8, #0xff
	ldr r8, [r0, r8, lsl #1]
	and r7, r7, sl
	orr r7, r7, r8, lsl #16
	ldr r4, [r1, #0x40]
	orr r4, r4, r7
	str r4, [r1, #0x20]
	mov r7, r6
	and r7, r7, #0xff
	ldr r7, [r0, r7, lsl #1]
	lsr r8, r6, #8
	and r8, r8, #0xff
	ldr r8, [r0, r8, lsl #1]
	and r7, r7, sl
	orr r7, r7, r8, lsl #16
	ldr r4, [r1, #0x80]
	orr r4, r4, r7
	str r4, [r1, #0x40]
	add r1, r1, #4
	add r2, r2, #4
	subs r9, r9, #1
	bpl _08000630
	pop {r4, r5, r6, r7, r8, r9, sl}
	bx lr

	.align 2, 0
_080006DC: .4byte gMsgHuffmanTableRoot
_080006E0: .4byte gMsgHuffmanTable

	ARM_FUNC_START IRAMARM_DecompText
IRAMARM_DecompText: @ 0x080006E4
	push {r4, r5, r6, r7}
	sub r3, r3, r3
	ldr r5, _080006E0
	ldr r7, _080006DC
	ldr r7, [r7]
_080006F8:
	mov r4, r7
_080006FC:
	subs r3, r3, #1
	bpl _08000710
	ldrb r2, [r0]
	add r0, r0, #1
	mov r3, #7
_08000710:
	tst r2, #1
	beq _08000720
	ldrh r6, [r4, #2]
	b _08000724
_08000720:
	ldrh r6, [r4]
_08000724:
	add r4, r5, r6, lsl #2
	lsr r2, r2, #1
	ldr r6, [r4]
	tst r6, r6
	bpl _080006FC
	tst r6, #0xff00
	beq _08000754
	strb r6, [r1]
	lsr r6, r6, #8
	strb r6, [r1, #1]
	add r1, r1, #2
	b _080006F8
_08000754:
	strb r6, [r1]
	tst r6, #0xff
	beq _08000768
	add r1, r1, #1
	b _080006F8
_08000768:
	pop {r4, r5, r6, r7}
	bx lr
	.align 2, 0
_08000770: .4byte gUnknown_03004BB0 @ pool
_08000774: .4byte gUnknown_03004E60 @ pool
_08000778: .4byte gUnknown_030049A0 @ pool
_0800077C: .4byte gUnknown_0202E4DC @ pool
_08000780: .4byte gUnknown_0202E4D8 @ pool

	ARM_FUNC_START IRAMARM_Func5
IRAMARM_Func5: @ 0x08000784
	push {r4, r5, r6, r7, r8, r9, sl}
	ldr r3, _08000774
	ldr r4, [r3]
	ldrb r5, [r4]
	add r1, r1, r5
	ldrb r6, [r4, #1]
	add r2, r2, r6
	ldr r7, _0800077C
	ldr r7, [r7]
	ldr r7, [r7, r2, lsl #2]
	ldrb r7, [r7, r1]
	ldr r8, _08000770
	ldrb sl, [r8, r7]
	ldr r7, _08000778
	ldr r7, [r7]
	ldr r9, [r7, r6, lsl #2]
	ldrb r9, [r9, r5]
	add sl, sl, r9
	ldr r9, [r7, r2, lsl #2]
	ldrb r9, [r9, r1]
	cmp sl, r9
	bhs _08000848
	ldrb r4, [r3, #8]
	tst r4, r4
	beq _08000810
	ldr r7, _08000780
	ldr r7, [r7]
	ldr r7, [r7, r2, lsl #2]
	ldrb r7, [r7, r1]
	tst r7, r7
	beq _08000810
	ldrb r4, [r3, #0xa]
	eor r4, r4, r7
	ands r4, r4, #0x80
	bne _08000848
_08000810:
	ldrb r4, [r3, #9]
	cmp sl, r4
	bhi _08000848
	ldr r4, [r3, #4]
	strb r1, [r4]
	strb r2, [r4, #1]
	strb r0, [r4, #2]
	strb sl, [r4, #3]
	add r4, r4, #4
	str r4, [r3, #4]
	ldr r7, _08000778
	ldr r7, [r7]
	ldr r7, [r7, r2, lsl #2]
	strb sl, [r7, r1]
_08000848:
	pop {r4, r5, r6, r7, r8, r9, sl}
	bx lr
	.align 2, 0
_08000850: .4byte gUnknown_030049B0 @ pool
_08000854: .4byte gUnknown_03004C50 @ pool

_08000858:
	b _08000994
	b _080009C8
	b _08000960
	b _0800092C
	b _08000A14
	b _080008E8

	.4byte _08000858

	ARM_FUNC_START IRAMARM_FillMovementMap
IRAMARM_FillMovementMap: @ 0x08000874
	push {r4, r5, r6, lr}
	mov r4, #0
	ldr r5, _08000774  @ gUnknown_03004E60
_08000880:
	eors r4, r4, #1
	beq _0800089C
	ldr r0, _08000850  @ gUnknown_030049B0
	str r0, [r5]
	ldr r0, _08000854  @ gUnknown_03004C50
	str r0, [r5, #4]
	b _080008AC
_0800089C:
	ldr r0, _08000854  @ gUnknown_03004C50
	str r0, [r5]
	ldr r0, _08000850  @ gUnknown_030049B0
	str r0, [r5, #4]
_080008AC:
	ldr r6, [r5]
	ldrb r6, [r6, #2]
	cmp r6, #4
	beq _08000A18
_080008BC:
	ldr r6, [r5]
	ldrb r6, [r6, #2]
	mov r0, pc
	add r0, r0, #8
	add r0, r0, r6, lsl #2
	bx r0
	b _08000994
	b _080009C8
	b _08000960
	b _0800092C
	b _08000A14
_080008E8:
	mov r0, #3
	mov r1, #0
	mvn r2, #0
	bl IRAMARM_Func5
	mov r0, #2
	mov r1, #0
	mov r2, #1
	bl IRAMARM_Func5
	mov r0, #0
	mvn r1, #0
	mov r2, #0
	bl IRAMARM_Func5
	mov r0, #1
	mov r1, #1
	mov r2, #0
	bl IRAMARM_Func5
	b _080009F8
_0800092C:
	mov r0, #3
	mov r1, #0
	mvn r2, #0
	bl IRAMARM_Func5
	mov r0, #0
	mvn r1, #0
	mov r2, #0
	bl IRAMARM_Func5
	mov r0, #1
	mov r1, #1
	mov r2, #0
	bl IRAMARM_Func5
	b _080009F8
_08000960:
	mov r0, #2
	mov r1, #0
	mov r2, #1
	bl IRAMARM_Func5
	mov r0, #0
	mvn r1, #0
	mov r2, #0
	bl IRAMARM_Func5
	mov r0, #1
	mov r1, #1
	mov r2, #0
	bl IRAMARM_Func5
	b _080009F8
_08000994:
	mov r0, #3
	mov r1, #0
	mvn r2, #0
	bl IRAMARM_Func5
	mov r0, #2
	mov r1, #0
	mov r2, #1
	bl IRAMARM_Func5
	mov r0, #0
	mvn r1, #0
	mov r2, #0
	bl IRAMARM_Func5
	b _080009F8
_080009C8:
	mov r0, #3
	mov r1, #0
	mvn r2, #0
	bl IRAMARM_Func5
	mov r0, #2
	mov r1, #0
	mov r2, #1
	bl IRAMARM_Func5
	mov r0, #1
	mov r1, #1
	mov r2, #0
	bl IRAMARM_Func5
_080009F8:
	ldr r6, [r5, #4]
	mov r0, #4
	strb r0, [r6, #2]
	ldr r6, [r5]
	add r6, r6, #4
	str r6, [r5]
	b _080008BC
_08000A14:
	b _08000880
_08000A18:
	pop {r4, r5, r6, lr}
	bx lr

	.global ARMCodeToCopy_End
ARMCodeToCopy_End:
