	.INCLUDE "macro.inc"

	.SYNTAX UNIFIED

	THUMB_FUNC_START Event2E_CheckAt
Event2E_CheckAt: @ 0x0800FD28
	push {r4, r5, lr}
	ldr r0, [r0, #0x38]
	ldrb r1, [r0]
	movs r2, #0xf
	ands r2, r1
	ldrh r1, [r0, #2]
	ldrb r5, [r0, #2]
	lsrs r3, r1, #8
	cmp r2, #0
	beq _0800FD42
	cmp r2, #1
	beq _0800FD60
	b _0800FD64
_0800FD42:
	ldr r0, _0800FD5C  @ gBmMapUnit
	ldr r1, [r0]
	lsls r0, r3, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r1, r0, r5
	ldrb r0, [r1]
	cmp r0, #0
	beq _0800FD68
	bl GetUnit
	adds r4, r0, #0
	b _0800FD64
	.align 2, 0
_0800FD5C: .4byte gBmMapUnit
_0800FD60:
	ldr r0, _0800FD70  @ gActiveUnit
	ldr r4, [r0]
_0800FD64:
	cmp r4, #0
	bne _0800FD78
_0800FD68:
	ldr r1, _0800FD74  @ gEventSlots
	movs r0, #0
	str r0, [r1, #0x30]
	b _0800FD80
	.align 2, 0
_0800FD70: .4byte gActiveUnit
_0800FD74: .4byte gEventSlots
_0800FD78:
	ldr r0, _0800FD88  @ gEventSlots
	ldr r1, [r4]
	ldrb r1, [r1, #4]
	str r1, [r0, #0x30]
_0800FD80:
	movs r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0800FD88: .4byte gEventSlots

	THUMB_FUNC_START Event2F_MoveUnit
Event2F_MoveUnit: @ 0x0800FD8C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	adds r7, r0, #0
	ldr r1, [r7, #0x38]
	ldrh r2, [r1]
	movs r4, #7
	ands r4, r2
	movs r0, #0xf
	ands r0, r2
	lsrs r0, r0, #3
	str r0, [sp, #4]
	ldrh r0, [r1, #2]
	str r0, [sp, #8]
	movs r2, #4
	ldrsh r0, [r1, r2]
	bl GetUnitStructFromEventParameter
	mov r8, r0
	cmp r0, #0
	bne _0800FDBE
	b _0800FF0C
_0800FDBE:
	mov r0, r8
	ldrb r0, [r0, #0x10]
	mov r9, r0
	mov r1, r8
	ldrb r1, [r1, #0x11]
	mov sl, r1
	mov r2, r8
	ldr r1, [r2, #0xc]
	movs r0, #0x80
	lsls r0, r0, #0xf
	ands r0, r1
	cmp r0, #0
	bne _0800FDE2
	movs r0, #4
	ands r1, r0
	cmp r1, #0
	beq _0800FDE2
	b _0800FF0C
_0800FDE2:
	cmp r4, #1
	beq _0800FE04
	cmp r4, #1
	bgt _0800FDF0
	cmp r4, #0
	beq _0800FDFA
	b _0800FE7C
_0800FDF0:
	cmp r4, #2
	beq _0800FE28
	cmp r4, #3
	beq _0800FE78
	b _0800FE7C
_0800FDFA:
	ldr r0, [r7, #0x38]
	ldrh r1, [r0, #6]
	ldrb r6, [r0, #6]
	lsrs r5, r1, #8
	b _0800FE72
_0800FE04:
	ldr r0, [r7, #0x38]
	ldrh r1, [r0, #6]
	movs r2, #6
	ldrsh r0, [r0, r2]
	cmp r0, #0
	bge _0800FE12
	movs r1, #0
_0800FE12:
	lsls r0, r1, #0x10
	asrs r0, r0, #0x10
	bl GetUnitStructFromEventParameter
	cmp r0, #0
	bne _0800FE22
	movs r0, #6
	b _0800FF0E
_0800FE22:
	ldrb r6, [r0, #0x10]
	ldrb r5, [r0, #0x11]
	b _0800FE72
_0800FE28:
	ldr r0, [r7, #0x38]
	ldrh r0, [r0, #6]
	mov r6, r9
	mov r5, sl
	cmp r0, #1
	beq _0800FE68
	cmp r0, #1
	bgt _0800FE3E
	cmp r0, #0
	beq _0800FE5E
	b _0800FE72
_0800FE3E:
	cmp r0, #2
	beq _0800FE52
	cmp r0, #3
	bne _0800FE72
	lsls r0, r5, #0x18
	movs r1, #0xff
	lsls r1, r1, #0x18
	adds r0, r0, r1
	lsrs r5, r0, #0x18
	b _0800FE72
_0800FE52:
	lsls r0, r5, #0x18
	movs r2, #0x80
	lsls r2, r2, #0x11
	adds r0, r0, r2
	lsrs r5, r0, #0x18
	b _0800FE72
_0800FE5E:
	lsls r0, r6, #0x18
	movs r1, #0xff
	lsls r1, r1, #0x18
	adds r0, r0, r1
	b _0800FE70
_0800FE68:
	lsls r0, r6, #0x18
	movs r2, #0x80
	lsls r2, r2, #0x11
	adds r0, r0, r2
_0800FE70:
	lsrs r6, r0, #0x18
_0800FE72:
	movs r0, #0
	str r0, [sp, #0xc]
	b _0800FE7C
_0800FE78:
	ldr r1, _0800FEB8  @ gEventSlotQueue
	str r1, [sp, #0xc]
_0800FE7C:
	ldr r1, [sp, #4]
	adds r0, r7, #0
	bl GetSomeEventEngineMoveRelatedBitfield
	lsls r0, r0, #0x10
	lsrs r4, r0, #0x10
	ldr r0, _0800FEBC  @ gBmMapUnk
	ldr r0, [r0]
	movs r1, #0
	bl BmMapFill
	ldrh r0, [r7, #0x3c]
	lsrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _0800FEA6
	ldr r2, [sp, #8]
	lsls r0, r2, #0x10
	cmp r0, #0
	bge _0800FEC0
_0800FEA6:
	lsls r1, r6, #0x18
	asrs r1, r1, #0x18
	lsls r2, r5, #0x18
	asrs r2, r2, #0x18
	mov r0, r8
	adds r3, r4, #0
	bl MoveUnit_
	b _0800FF0C
	.align 2, 0
_0800FEB8: .4byte gEventSlotQueue
_0800FEBC: .4byte gBmMapUnk
_0800FEC0:
	mov r0, r9
	lsls r1, r0, #0x18
	asrs r1, r1, #0x18
	mov r0, sl
	lsls r2, r0, #0x18
	asrs r2, r2, #0x18
	adds r0, r7, #0
	bl TryPrepareEventUnitMovement
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0800FEDC
	movs r0, #3
	b _0800FF0E
_0800FEDC:
	ldr r1, [sp, #0xc]
	cmp r1, #0
	bne _0800FEFA
	lsls r1, r6, #0x18
	asrs r1, r1, #0x18
	lsls r2, r5, #0x18
	asrs r2, r2, #0x18
	ldr r0, [sp, #8]
	lsls r3, r0, #0x18
	lsrs r3, r3, #0x18
	str r4, [sp]
	mov r0, r8
	bl sub_8079DDC
	b _0800FF0C
_0800FEFA:
	ldr r0, _0800FF20  @ gEventSlots
	ldr r2, [r0, #0x34]
	lsls r2, r2, #0xf
	asrs r2, r2, #0x10
	mov r0, r8
	ldr r1, [sp, #0xc]
	adds r3, r4, #0
	bl sub_8079D74
_0800FF0C:
	movs r0, #0
_0800FF0E:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0800FF20: .4byte gEventSlots

	THUMB_FUNC_START Event30_ENUN
Event30_ENUN: @ 0x0800FF24
	push {lr}
	ldrh r0, [r0, #0x3c]
	lsrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0800FF36
	bl SetAllMOVEUNITField44To1_
_0800FF36:
	bl MuCtrExists
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	cmp r0, #1
	beq _0800FF60
	bl RefreshEntityBmMaps
	bl SMS_UpdateFromGameData
	bl RenderBmMap
	ldr r0, _0800FF5C  @ gBmMapUnk
	ldr r0, [r0]
	movs r1, #0
	bl BmMapFill
	movs r0, #2
	b _0800FF62
	.align 2, 0
_0800FF5C: .4byte gBmMapUnk
_0800FF60:
	movs r0, #3
_0800FF62:
	pop {r1}
	bx r1

	THUMB_FUNC_START Event31_DisplayEffectRange
Event31_DisplayEffectRange: @ 0x0800FF68
	push {r4, lr}
	adds r1, r0, #0
	ldrh r0, [r1, #0x3c]
	lsrs r4, r0, #2
	movs r0, #1
	ands r4, r0
	cmp r4, #0
	beq _0800FF7C
	movs r0, #0
	b _0800FFE8
_0800FF7C:
	ldr r2, [r1, #0x38]
	ldrb r0, [r2]
	movs r1, #0xf
	ands r1, r0
	cmp r1, #0
	beq _0800FF8E
	cmp r1, #1
	beq _0800FFD4
	b _0800FFE6
_0800FF8E:
	movs r1, #2
	ldrsh r0, [r2, r1]
	bl GetUnitStructFromEventParameter
	adds r4, r0, #0
	cmp r4, #0
	bne _0800FFA0
	movs r0, #6
	b _0800FFE8
_0800FFA0:
	ldr r0, _0800FFC8  @ gUnknown_0202BCF0
	adds r0, #0x41
	ldrb r0, [r0]
	lsls r0, r0, #0x1e
	cmp r0, #0
	blt _0800FFB2
	movs r0, #0x68
	bl m4aSongNumStart
_0800FFB2:
	ldr r2, _0800FFCC  @ gUnknown_03000434
	ldr r1, _0800FFD0  @ gActiveUnit
	ldr r0, [r1]
	str r0, [r2]
	str r4, [r1]
	bl HideMoveRangeGraphics
	adds r0, r4, #0
	bl DisplayUnitEffectRange
	b _0800FFE6
	.align 2, 0
_0800FFC8: .4byte gUnknown_0202BCF0
_0800FFCC: .4byte gUnknown_03000434
_0800FFD0: .4byte gActiveUnit
_0800FFD4:
	bl HideMoveRangeGraphics
	ldr r2, _0800FFF0  @ gUnknown_03000434
	ldr r1, [r2]
	cmp r1, #0
	beq _0800FFE6
	ldr r0, _0800FFF4  @ gActiveUnit
	str r1, [r0]
	str r4, [r2]
_0800FFE6:
	movs r0, #2
_0800FFE8:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0800FFF0: .4byte gUnknown_03000434
_0800FFF4: .4byte gActiveUnit

	THUMB_FUNC_START Event32_SpawnSingleUnit
Event32_SpawnSingleUnit: @ 0x0800FFF8
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	ldr r0, [r0, #0x38]
	ldrb r1, [r0]
	movs r2, #0xf
	mov ip, r2
	ands r2, r1
	mov ip, r2
	ldrh r1, [r0, #4]
	ldrb r6, [r0, #4]
	lsrs r7, r1, #8
	ldrh r4, [r0, #2]
	movs r2, #2
	ldrsh r1, [r0, r2]
	movs r0, #3
	negs r0, r0
	cmp r1, r0
	bne _08010020
	ldr r0, _08010100  @ gEventSlots
	ldrh r4, [r0, #8]
_08010020:
	lsls r0, r6, #0x18
	cmp r0, #0
	blt _0801002C
	lsls r0, r7, #0x18
	cmp r0, #0
	bge _08010032
_0801002C:
	ldr r0, _08010104  @ gUnknown_030004E4
	ldrb r6, [r0]
	ldrb r7, [r0, #2]
_08010032:
	mov r0, sp
	movs r5, #0
	strb r4, [r0]
	mov r3, sp
	ldr r2, _08010108  @ gCharacterData
	lsls r0, r4, #0x10
	asrs r0, r0, #0x10
	subs r0, #1
	movs r1, #0x34
	muls r0, r1, r0
	adds r4, r0, r2
	ldrb r0, [r4, #5]
	strb r0, [r3, #1]
	mov r0, sp
	strb r5, [r0, #2]
	mov r1, sp
	ldrb r0, [r1, #3]
	movs r2, #2
	negs r2, r2
	ands r2, r0
	strb r2, [r1, #3]
	movs r0, #0
	mov r1, ip
	cmp r1, #0xf
	beq _08010066
	mov r0, ip
_08010066:
	movs r1, #3
	ands r1, r0
	lsls r1, r1, #1
	movs r0, #7
	negs r0, r0
	ands r0, r2
	orrs r0, r1
	strb r0, [r3, #3]
	mov r3, sp
	ldrb r1, [r4, #0xb]
	lsls r1, r1, #3
	movs r2, #7
	ands r0, r2
	orrs r0, r1
	strb r0, [r3, #3]
	mov r2, sp
	movs r0, #0x3f
	ands r6, r0
	ldrb r1, [r2, #4]
	movs r0, #0x40
	negs r0, r0
	ands r0, r1
	orrs r0, r6
	strb r0, [r2, #4]
	lsls r1, r7, #0x18
	asrs r1, r1, #0x18
	movs r0, #0x3f
	ands r1, r0
	lsls r1, r1, #6
	ldrh r2, [r3, #4]
	ldr r0, _0801010C  @ 0xFFFFF03F
	ands r0, r2
	orrs r0, r1
	strh r0, [r3, #4]
	mov r2, sp
	ldrb r1, [r2, #5]
	movs r0, #0x11
	negs r0, r0
	ands r0, r1
	strb r0, [r2, #5]
	movs r1, #0x20
	orrs r0, r1
	strb r0, [r2, #5]
	movs r1, #0x41
	negs r1, r1
	ands r1, r0
	strb r1, [r2, #5]
	movs r0, #0x7f
	ands r0, r1
	strb r0, [r2, #5]
	mov r0, sp
	strb r5, [r0, #6]
	strb r5, [r0, #7]
	str r5, [sp, #8]
	strb r5, [r0, #0xc]
	strb r5, [r0, #0xd]
	strb r5, [r0, #0xe]
	strb r5, [r0, #0xf]
	strb r5, [r0, #0x10]
	strb r5, [r0, #0x11]
	strb r5, [r0, #0x12]
	strb r5, [r0, #0x13]
	movs r3, #0
	mov r2, ip
	cmp r2, #0xf
	bne _080100EC
	movs r3, #1
_080100EC:
	mov r0, sp
	movs r1, #0
	movs r2, #0
	bl LoadUnit_800F704
	movs r0, #0
	add sp, #0x14
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08010100: .4byte gEventSlots
_08010104: .4byte gUnknown_030004E4
_08010108: .4byte gCharacterData
_0801010C: .4byte 0xFFFFF03F

	THUMB_FUNC_START Event33_CheckUnitVarious
Event33_CheckUnitVarious: @ 0x08010110
	push {r4, r5, lr}
	ldr r0, [r0, #0x38]
	ldrb r1, [r0]
	movs r4, #0xf
	ands r4, r1
	ldrh r5, [r0, #2]
	movs r1, #2
	ldrsh r0, [r0, r1]
	bl GetUnitStructFromEventParameter
	adds r2, r0, #0
	cmp r4, #8
	bls _0801012C
	b _0801028A
_0801012C:
	lsls r0, r4, #2
	ldr r1, _08010138  @ _0801013C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08010138: .4byte _0801013C
_0801013C: @ jump table
	.4byte _08010160 @ case 0
	.4byte _08010170 @ case 1
	.4byte _08010188 @ case 2
	.4byte _080101B8 @ case 3
	.4byte _080101E4 @ case 4
	.4byte _08010214 @ case 5
	.4byte _0801024C @ case 6
	.4byte _08010264 @ case 7
	.4byte _08010278 @ case 8
_08010160:
	cmp r2, #0
	beq _0801018C
	ldr r1, _0801016C  @ gEventSlots
	movs r0, #1
	b _08010288
	.align 2, 0
_0801016C: .4byte gEventSlots
_08010170:
	cmp r2, #0
	bne _08010176
	b _0801027C
_08010176:
	ldr r0, _08010184  @ gEventSlots
	ldr r1, [r2]
	adds r1, #0x30
	ldrb r1, [r1]
	str r1, [r0, #0x30]
	b _0801028A
	.align 2, 0
_08010184: .4byte gEventSlots
_08010188:
	cmp r2, #0
	bne _08010198
_0801018C:
	ldr r0, _08010194  @ gEventSlots
	str r2, [r0, #0x30]
	b _0801028A
	.align 2, 0
_08010194: .4byte gEventSlots
_08010198:
	ldr r0, [r2, #0xc]
	movs r1, #4
	ands r0, r1
	cmp r0, #0
	beq _080101AC
	ldr r1, _080101A8  @ gEventSlots
	movs r0, #0
	b _08010288
	.align 2, 0
_080101A8: .4byte gEventSlots
_080101AC:
	ldr r1, _080101B4  @ gEventSlots
	movs r0, #1
	b _08010288
	.align 2, 0
_080101B4: .4byte gEventSlots
_080101B8:
	cmp r2, #0
	beq _0801027C
	ldr r1, [r2, #0xc]
	movs r0, #8
	ands r1, r0
	cmp r1, #0
	beq _080101D0
	ldr r1, _080101CC  @ gEventSlots
	movs r0, #0
	b _08010288
	.align 2, 0
_080101CC: .4byte gEventSlots
_080101D0:
	movs r0, #0x10
	ldrsb r0, [r2, r0]
	cmp r0, #0
	blt _08010234
	ldr r1, _080101E0  @ gEventSlots
	movs r0, #1
	b _08010288
	.align 2, 0
_080101E0: .4byte gEventSlots
_080101E4:
	cmp r2, #0
	beq _0801027C
	ldr r0, _08010200  @ gActiveUnit
	ldr r0, [r0]
	ldr r0, [r0]
	ldrb r1, [r0, #4]
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	cmp r1, r0
	beq _08010208
	ldr r1, _08010204  @ gEventSlots
	movs r0, #0
	b _08010288
	.align 2, 0
_08010200: .4byte gActiveUnit
_08010204: .4byte gEventSlots
_08010208:
	ldr r1, _08010210  @ gEventSlots
	movs r0, #1
	b _08010288
	.align 2, 0
_08010210: .4byte gEventSlots
_08010214:
	cmp r2, #0
	beq _0801027C
	movs r1, #0xb
	ldrsb r1, [r2, r1]
	movs r0, #0xc0
	ands r1, r0
	cmp r1, #0
	beq _08010234
	cmp r1, #0x80
	beq _08010240
	ldr r1, _08010230  @ gEventSlots
	movs r0, #1
	b _08010288
	.align 2, 0
_08010230: .4byte gEventSlots
_08010234:
	ldr r0, _0801023C  @ gEventSlots
	str r1, [r0, #0x30]
	b _0801028A
	.align 2, 0
_0801023C: .4byte gEventSlots
_08010240:
	ldr r1, _08010248  @ gEventSlots
	movs r0, #2
	b _08010288
	.align 2, 0
_08010248: .4byte gEventSlots
_0801024C:
	cmp r2, #0
	beq _0801027C
	ldr r0, _08010260  @ gUnknown_030004E8
	movs r1, #0x10
	ldrsb r1, [r2, r1]
	strh r1, [r0]
	movs r1, #0x11
	ldrsb r1, [r2, r1]
	strh r1, [r0, #2]
	b _0801028A
	.align 2, 0
_08010260: .4byte gUnknown_030004E8
_08010264:
	cmp r2, #0
	beq _0801027C
	ldr r0, _08010274  @ gEventSlots
	ldr r1, [r2, #4]
	ldrb r1, [r1, #4]
	str r1, [r0, #0x30]
	b _0801028A
	.align 2, 0
_08010274: .4byte gEventSlots
_08010278:
	cmp r2, #0
	bne _08010280
_0801027C:
	movs r0, #6
	b _0801028C
_08010280:
	adds r0, r2, #0
	bl GetUnitLuck
	ldr r1, _08010294  @ gEventSlots
_08010288:
	str r0, [r1, #0x30]
_0801028A:
	movs r0, #0
_0801028C:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08010294: .4byte gEventSlots

	THUMB_FUNC_START Event34_MessWithUnitState
Event34_MessWithUnitState: @ 0x08010298
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r6, r0, #0
	ldr r1, _080102CC  @ gUnknown_080D793C
	mov r0, sp
	movs r2, #3
	bl memcpy
	ldr r1, [r6, #0x38]
	ldrb r0, [r1]
	movs r4, #0xf
	ands r4, r0
	movs r2, #2
	ldrsh r0, [r1, r2]
	bl GetUnitStructFromEventParameter
	adds r5, r0, #0
	cmp r4, #0xa
	blt _080102D0
	cmp r4, #0xc
	ble _080102D8
	cmp r4, #0xf
	bgt _080102D0
	cmp r5, #0
	bne _080102D8
	b _080104A6
	.align 2, 0
_080102CC: .4byte gUnknown_080D793C
_080102D0:
	cmp r5, #0
	bne _080102D8
	movs r0, #6
	b _080104A8
_080102D8:
	cmp r4, #0xf
	bls _080102DE
	b _0801049A
_080102DE:
	lsls r0, r4, #2
	ldr r1, _080102E8  @ _080102EC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080102E8: .4byte _080102EC
_080102EC: @ jump table
	.4byte _0801032C @ case 0
	.4byte _08010338 @ case 1
	.4byte _08010344 @ case 2
	.4byte _08010344 @ case 3
	.4byte _08010344 @ case 4
	.4byte _08010352 @ case 5
	.4byte _08010370 @ case 6
	.4byte _08010376 @ case 7
	.4byte _0801037C @ case 8
	.4byte _0801049A @ case 9
	.4byte _080103CC @ case 10
	.4byte _08010400 @ case 11
	.4byte _08010426 @ case 12
	.4byte _0801044C @ case 13
	.4byte _0801047C @ case 14
	.4byte _08010494 @ case 15
_0801032C:
	ldr r0, [r5, #0xc]
	ldr r1, _08010334  @ 0x04010001
	b _080103A8
	.align 2, 0
_08010334: .4byte 0x04010001
_08010338:
	ldr r0, [r5, #0xc]
	ldr r1, _08010340  @ 0xFBFEFFFE
	b _0801039E
	.align 2, 0
_08010340: .4byte 0xFBFEFFFE
_08010344:
	subs r0, r4, #2
	add r0, sp
	ldrb r1, [r0]
	adds r0, r5, #0
	bl UnitChangeFaction
	b _0801049A
_08010352:
	ldr r4, _0801036C  @ gEventSlots
	ldr r1, [r4, #4]
	adds r0, r5, #0
	bl SetUnitHp
	ldr r0, [r4, #4]
	cmp r0, #0
	beq _08010364
	b _0801049A
_08010364:
	ldr r0, [r5, #0xc]
	movs r1, #4
	b _080103A8
	.align 2, 0
_0801036C: .4byte gEventSlots
_08010370:
	ldr r0, [r5, #0xc]
	movs r1, #2
	b _080103A8
_08010376:
	ldr r0, [r5, #0xc]
	movs r1, #0x40
	b _080103A8
_0801037C:
	ldr r0, _08010394  @ gEventSlots
	ldr r1, [r0, #4]
	cmp r1, #1
	beq _08010398
	cmp r1, #1
	bcc _080103A4
	movs r0, #1
	negs r0, r0
	cmp r1, r0
	beq _080103AE
	b _0801049A
	.align 2, 0
_08010394: .4byte gEventSlots
_08010398:
	ldr r0, [r5, #0xc]
	movs r1, #9
	negs r1, r1
_0801039E:
	ands r0, r1
	str r0, [r5, #0xc]
	b _0801049A
_080103A4:
	ldr r0, [r5, #0xc]
	movs r1, #8
_080103A8:
	orrs r0, r1
	str r0, [r5, #0xc]
	b _0801049A
_080103AE:
	ldr r1, [r5, #0xc]
	movs r0, #0x80
	lsls r0, r0, #0xe
	ands r0, r1
	cmp r0, #0
	beq _080103C2
	movs r0, #8
	orrs r1, r0
	str r1, [r5, #0xc]
	b _0801049A
_080103C2:
	movs r0, #9
	negs r0, r0
	ands r1, r0
	str r1, [r5, #0xc]
	b _0801049A
_080103CC:
	bl MU_EndAll
	movs r4, #1
_080103D2:
	adds r0, r4, #0
	bl GetUnit
	adds r2, r0, #0
	cmp r2, #0
	beq _080103F4
	ldr r0, [r2]
	cmp r0, #0
	beq _080103F4
	ldr r0, [r2, #0xc]
	movs r1, #1
	orrs r0, r1
	subs r1, #4
	ands r0, r1
	subs r1, #0x2e
	ands r0, r1
	str r0, [r2, #0xc]
_080103F4:
	adds r4, #1
	cmp r4, #0x3f
	ble _080103D2
	bl ClearCutsceneUnits
	b _0801049A
_08010400:
	bl MU_EndAll
	movs r4, #0x41
_08010406:
	adds r0, r4, #0
	bl GetUnit
	adds r1, r0, #0
	cmp r1, #0
	beq _0801041E
	ldr r0, [r1]
	cmp r0, #0
	beq _0801041E
	adds r0, r1, #0
	bl ClearUnit
_0801041E:
	adds r4, #1
	cmp r4, #0x7f
	ble _08010406
	b _0801049A
_08010426:
	bl MU_EndAll
	movs r4, #0x81
_0801042C:
	adds r0, r4, #0
	bl GetUnit
	adds r1, r0, #0
	cmp r1, #0
	beq _08010444
	ldr r0, [r1]
	cmp r0, #0
	beq _08010444
	adds r0, r1, #0
	bl ClearUnit
_08010444:
	adds r4, #1
	cmp r4, #0xbf
	ble _0801042C
	b _0801049A
_0801044C:
	ldrh r0, [r6, #0x3c]
	lsrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _080104A6
	adds r0, r5, #0
	bl HideUnitSMS
	ldr r0, [r5, #0xc]
	movs r1, #1
	orrs r0, r1
	str r0, [r5, #0xc]
	adds r0, r5, #0
	bl MU_Create
	adds r4, r0, #0
	bl MU_SetDefaultFacing_Auto
	adds r0, r4, #0
	bl MU_StartDeathFade
	movs r0, #2
	b _080104A8
_0801047C:
	ldr r0, _08010490  @ gProcScr_MUDeathFade
	bl Proc_Find
	negs r1, r0
	orrs r1, r0
	cmp r1, #0
	bge _08010494
	movs r0, #3
	b _080104A8
	.align 2, 0
_08010490: .4byte gProcScr_MUDeathFade
_08010494:
	adds r0, r5, #0
	bl ClearUnit
_0801049A:
	bl RefreshEntityBmMaps
	bl SMS_UpdateFromGameData
	bl RenderBmMap
_080104A6:
	movs r0, #0
_080104A8:
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	THUMB_FUNC_START Event35_UnitClassChanging
Event35_UnitClassChanging: @ 0x080104B0
	push {r4, r5, r6, r7, lr}
	ldr r0, [r0, #0x38]
	ldrb r1, [r0]
	movs r4, #0xf
	ands r4, r1
	ldrh r6, [r0, #4]
	ldrh r7, [r0, #6]
	movs r1, #2
	ldrsh r0, [r0, r1]
	bl GetUnitStructFromEventParameter
	adds r5, r0, #0
	cmp r5, #0
	beq _0801052E
	cmp r4, #0
	beq _080104D6
	cmp r4, #1
	beq _080104F0
	b _08010522
_080104D6:
	cmp r6, #0
	bne _08010518
	ldr r2, _080104EC  @ gCharacterData
	lsls r0, r7, #0x10
	asrs r0, r0, #0x10
	subs r0, #1
	movs r1, #0x34
	muls r0, r1, r0
	adds r0, r0, r2
	ldrb r6, [r0, #5]
	b _08010518
	.align 2, 0
_080104EC: .4byte gCharacterData
_080104F0:
	adds r4, r6, #0
	ldr r0, [r5, #4]
	ldrb r6, [r0, #4]
	ldr r2, _08010538  @ gCharacterData
	lsls r4, r4, #0x10
	asrs r4, r4, #0x10
	subs r1, r4, #1
	movs r0, #0x34
	muls r0, r1, r0
	adds r0, r0, r2
	ldrb r0, [r0, #5]
	bl GetClassData
	str r0, [r5, #4]
	adds r0, r4, #0
	bl GetUnitFromCharId
	adds r5, r0, #0
	cmp r5, #0
	beq _0801052E
_08010518:
	lsls r0, r6, #0x10
	asrs r0, r0, #0x10
	bl GetClassData
	str r0, [r5, #4]
_08010522:
	bl RefreshEntityBmMaps
	bl SMS_UpdateFromGameData
	bl RenderBmMap
_0801052E:
	movs r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08010538: .4byte gCharacterData

	THUMB_FUNC_START Event36_CheckInArea
Event36_CheckInArea: @ 0x0801053C
	push {r4, r5, r6, r7, lr}
	ldr r2, [r0, #0x38]
	ldrh r0, [r2, #4]
	ldrb r4, [r2, #4]
	lsrs r0, r0, #8
	adds r6, r0, #0
	ldrh r1, [r2, #6]
	adds r0, r1, r4
	subs r0, #1
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	lsrs r1, r1, #8
	adds r1, r1, r6
	subs r1, #1
	lsls r1, r1, #0x18
	lsrs r7, r1, #0x18
	movs r1, #2
	ldrsh r0, [r2, r1]
	bl GetUnitStructFromEventParameter
	adds r2, r0, #0
	ldr r3, _080105A0  @ gEventSlots
	movs r0, #0
	str r0, [r3, #0x30]
	movs r1, #0x10
	ldrsb r1, [r2, r1]
	lsls r4, r4, #0x18
	asrs r4, r4, #0x18
	cmp r1, r4
	blt _0801059A
	lsls r0, r5, #0x18
	asrs r0, r0, #0x18
	cmp r0, r1
	blt _08010598
	movs r1, #0x11
	ldrsb r1, [r2, r1]
	lsls r0, r6, #0x18
	asrs r0, r0, #0x18
	cmp r1, r0
	blt _08010598
	lsls r0, r7, #0x18
	asrs r0, r0, #0x18
	cmp r0, r1
	blt _08010598
	movs r0, #1
	str r0, [r3, #0x30]
_08010598:
	movs r0, #0
_0801059A:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080105A0: .4byte gEventSlots

	THUMB_FUNC_START Event37_GiveItem
Event37_GiveItem: @ 0x080105A4
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r1, [r5, #0x38]
	ldrb r0, [r1]
	movs r4, #0xf
	ands r4, r0
	adds r6, r4, #0
	movs r2, #2
	ldrsh r0, [r1, r2]
	bl GetUnitStructFromEventParameter
	adds r1, r0, #0
	cmp r1, #0
	bne _080105C4
	movs r0, #6
	b _0801060E
_080105C4:
	cmp r4, #1
	beq _080105E8
	cmp r4, #1
	bgt _080105D2
	cmp r4, #0
	beq _080105D8
	b _0801060C
_080105D2:
	cmp r6, #2
	beq _080105F8
	b _0801060C
_080105D8:
	ldr r0, _080105E4  @ gEventSlots
	ldrh r2, [r0, #0xc]
	adds r0, r5, #0
	bl NewItemGot
	b _0801060C
	.align 2, 0
_080105E4: .4byte gEventSlots
_080105E8:
	ldr r0, _080105F4  @ gEventSlots
	ldr r2, [r0, #0xc]
	adds r0, r5, #0
	bl NewGoldGotPopup
	b _0801060C
	.align 2, 0
_080105F4: .4byte gEventSlots
_080105F8:
	bl GetPartyGoldAmount
	ldr r1, _08010614  @ gEventSlots
	ldr r1, [r1, #0xc]
	subs r0, r0, r1
	cmp r0, #0
	bge _08010608
	movs r0, #0
_08010608:
	bl SetPartyGoldAmount
_0801060C:
	movs r0, #2
_0801060E:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08010614: .4byte gEventSlots

	THUMB_FUNC_START Event38_ChangeActiveUnit
Event38_ChangeActiveUnit: @ 0x08010618
	push {r4, lr}
	ldr r0, [r0, #0x38]
	movs r1, #2
	ldrsh r0, [r0, r1]
	bl GetUnitStructFromEventParameter
	adds r4, r0, #0
	cmp r4, #0
	beq _0801063C
	bl ClearActiveUnit
	ldr r0, _08010638  @ gActiveUnit
	str r4, [r0]
	movs r0, #0
	b _0801063E
	.align 2, 0
_08010638: .4byte gActiveUnit
_0801063C:
	movs r0, #6
_0801063E:
	pop {r4}
	pop {r1}
	bx r1

	THUMB_FUNC_START Event39_
Event39_: @ 0x08010644
	push {r4, r5, r6, r7, lr}
	ldr r2, [r0, #0x38]
	ldrb r0, [r2]
	movs r3, #0xf
	ands r3, r0
	ldr r4, _0801066C  @ gEventSlots
	ldr r1, [r4, #4]
	ldrb r7, [r4, #4]
	lsrs r0, r1, #8
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	lsrs r1, r1, #0x10
	lsls r1, r1, #0x18
	lsrs r5, r1, #0x18
	cmp r3, #0
	beq _08010670
	cmp r3, #1
	beq _0801068C
	b _080106DC
	.align 2, 0
_0801066C: .4byte gEventSlots
_08010670:
	ldrh r1, [r2, #2]
	movs r3, #2
	ldrsh r0, [r2, r3]
	cmp r0, #0
	bge _0801067C
	ldrh r1, [r4, #8]
_0801067C:
	lsls r0, r1, #0x18
	lsrs r0, r0, #0x18
	adds r1, r7, #0
	adds r2, r6, #0
	adds r3, r5, #0
	bl sub_8011D10
	b _080106DC
_0801068C:
	ldrh r0, [r2, #2]
	lsrs r1, r0, #8
	ldrb r3, [r2, #2]
	movs r0, #2
	ldrsb r0, [r2, r0]
	cmp r0, #0
	blt _080106A0
	lsls r0, r1, #0x18
	cmp r0, #0
	bge _080106AC
_080106A0:
	ldrh r0, [r4, #0x2c]
	lsls r0, r0, #0x18
	lsrs r3, r0, #0x18
	ldrh r0, [r4, #0x2e]
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
_080106AC:
	lsls r1, r1, #0x18
	ldr r0, _080106CC  @ gBmMapUnit
	ldr r0, [r0]
	asrs r1, r1, #0x16
	adds r1, r1, r0
	lsls r0, r3, #0x18
	asrs r0, r0, #0x18
	ldr r1, [r1]
	adds r1, r1, r0
	ldrb r0, [r1]
	cmp r0, #0
	beq _080106D0
	bl GetUnit
	b _080106D2
	.align 2, 0
_080106CC: .4byte gBmMapUnit
_080106D0:
	movs r0, #0
_080106D2:
	adds r1, r7, #0
	adds r2, r6, #0
	adds r3, r5, #0
	bl sub_8011CCC
_080106DC:
	movs r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	THUMB_FUNC_START Event3A_
Event3A_: @ 0x080106E4
	push {r4, r5, lr}
	adds r4, r0, #0
	ldrh r0, [r4, #0x3c]
	lsrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _080106F8
	movs r0, #0
	b _08010742
_080106F8:
	ldr r1, [r4, #0x38]
	ldrb r0, [r1]
	movs r2, #0xf
	ands r2, r0
	ldrh r3, [r1, #2]
	movs r5, #2
	ldrsh r0, [r1, r5]
	cmp r0, #0
	bge _0801070E
	ldr r0, _08010718  @ gEventSlots
	ldrh r3, [r0, #8]
_0801070E:
	cmp r2, #0
	beq _0801071C
	cmp r2, #1
	beq _0801072A
	b _08010740
	.align 2, 0
_08010718: .4byte gEventSlots
_0801071C:
	ldrh r1, [r1, #4]
	lsls r0, r3, #0x10
	asrs r0, r0, #0x10
	adds r2, r4, #0
	bl sub_8011750
	b _08010740
_0801072A:
	ldrh r2, [r1, #6]
	lsls r0, r3, #0x10
	asrs r0, r0, #0x10
	ldrb r1, [r1, #6]
	lsls r1, r1, #0x18
	asrs r1, r1, #0x18
	lsls r2, r2, #0x10
	asrs r2, r2, #0x18
	adds r3, r4, #0
	bl sub_8011C94
_08010740:
	movs r0, #2
_08010742:
	pop {r4, r5}
	pop {r1}
	bx r1

	THUMB_FUNC_START sub_8010748
sub_8010748: @ 0x08010748
	push {r4, lr}
	adds r2, r0, #0
	adds r0, #0x68
	ldrh r0, [r0]
	movs r1, #0xc
	ands r1, r0
	cmp r1, #0
	beq _0801075E
	cmp r1, #4
	beq _0801077A
	b _08010796
_0801075E:
	adds r0, r2, #0
	adds r0, #0x64
	movs r1, #0
	ldrsh r0, [r0, r1]
	lsls r0, r0, #4
	adds r1, r2, #0
	adds r1, #0x66
	movs r2, #0
	ldrsh r1, [r1, r2]
	lsls r1, r1, #4
	movs r2, #0
	bl DisplayCursor
	b _08010796
_0801077A:
	adds r0, r2, #0
	adds r0, #0x64
	movs r3, #0
	ldrsh r0, [r0, r3]
	adds r1, r2, #0
	adds r1, #0x66
	movs r3, #0
	ldrsh r1, [r1, r3]
	adds r4, r2, #0
	adds r4, #0x6a
	ldrh r2, [r4]
	bl sub_8010E6C
	strh r0, [r4]
_08010796:
	pop {r4}
	pop {r0}
	bx r0

	THUMB_FUNC_START Event3B_
Event3B_: @ 0x0801079C
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	ldrh r0, [r6, #0x3c]
	lsrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _080107B8
	ldr r0, _080107B4  @ gUnknown_08591F08
	bl Proc_DeleteAllWithScript
	b _08010844
	.align 2, 0
_080107B4: .4byte gUnknown_08591F08
_080107B8:
	ldr r1, [r6, #0x38]
	ldrb r0, [r1]
	movs r7, #0xf
	ands r7, r0
	movs r0, #3
	ands r0, r7
	cmp r0, #1
	beq _080107F8
	cmp r0, #1
	bgt _080107D2
	cmp r0, #0
	beq _080107D8
	b _0801081C
_080107D2:
	cmp r0, #2
	beq _0801080E
	b _0801081C
_080107D8:
	ldrh r0, [r1, #2]
	lsrs r4, r0, #8
	ldrb r5, [r1, #2]
	movs r0, #2
	ldrsb r0, [r1, r0]
	cmp r0, #0
	blt _080107EC
	lsls r0, r4, #0x18
	cmp r0, #0
	bge _0801081C
_080107EC:
	ldr r0, _080107F4  @ gUnknown_030004E4
	ldrb r5, [r0]
	ldrb r4, [r0, #2]
	b _0801081C
	.align 2, 0
_080107F4: .4byte gUnknown_030004E4
_080107F8:
	movs r2, #2
	ldrsh r0, [r1, r2]
	bl GetUnitStructFromEventParameter
	cmp r0, #0
	bne _08010808
	movs r0, #6
	b _08010846
_08010808:
	ldrb r5, [r0, #0x10]
	ldrb r4, [r0, #0x11]
	b _0801081C
_0801080E:
	ldr r0, _08010818  @ gUnknown_08591F08
	bl Proc_DeleteAllWithScript
	movs r0, #2
	b _08010846
	.align 2, 0
_08010818: .4byte gUnknown_08591F08
_0801081C:
	ldr r0, _0801084C  @ gUnknown_08591F08
	adds r1, r6, #0
	bl Proc_Create
	adds r3, r0, #0
	lsls r0, r5, #0x18
	asrs r0, r0, #0x18
	adds r1, r3, #0
	adds r1, #0x64
	movs r2, #0
	strh r0, [r1]
	lsls r0, r4, #0x18
	asrs r0, r0, #0x18
	adds r1, #2
	strh r0, [r1]
	adds r0, r3, #0
	adds r0, #0x68
	strh r7, [r0]
	adds r0, #2
	strh r2, [r0]
_08010844:
	movs r0, #0
_08010846:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801084C: .4byte gUnknown_08591F08

	THUMB_FUNC_START Event3C_
Event3C_: @ 0x08010850
	push {lr}
	ldr r2, [r0, #0x38]
	ldrb r0, [r2]
	movs r1, #0xf
	ands r1, r0
	cmp r1, #0
	beq _08010864
	cmp r1, #1
	beq _0801087C
	b _080108A2
_08010864:
	ldr r2, _08010874  @ gUnknown_030004E8
	ldr r1, _08010878  @ gUnknown_0202BCB0
	ldrh r0, [r1, #0x14]
	strh r0, [r2]
	ldrh r0, [r1, #0x16]
	strh r0, [r2, #2]
	b _080108A2
	.align 2, 0
_08010874: .4byte gUnknown_030004E8
_08010878: .4byte gUnknown_0202BCB0
_0801087C:
	ldrh r0, [r2, #2]
	lsrs r1, r0, #8
	ldrb r3, [r2, #2]
	movs r0, #2
	ldrsb r0, [r2, r0]
	cmp r0, #0
	blt _08010890
	lsls r0, r1, #0x18
	cmp r0, #0
	bge _08010896
_08010890:
	ldr r0, _080108A8  @ gUnknown_030004E4
	ldrb r3, [r0]
	ldrb r1, [r0, #2]
_08010896:
	lsls r0, r3, #0x18
	asrs r0, r0, #0x18
	lsls r1, r1, #0x18
	asrs r1, r1, #0x18
	bl SetCursorMapPosition
_080108A2:
	movs r0, #0
	pop {r1}
	bx r1
	.align 2, 0
_080108A8: .4byte gUnknown_030004E4

	THUMB_FUNC_START Event3D_
Event3D_: @ 0x080108AC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x18
	adds r5, r0, #0
	ldr r1, _08010914  @ gUnknown_080D793F
	mov r0, sp
	movs r2, #0xf
	bl memcpy
	add r4, sp, #0x10
	ldr r1, _08010918  @ gUnknown_080D794E
	adds r0, r4, #0
	movs r2, #5
	bl memcpy
	ldr r1, [r5, #0x38]
	ldrb r0, [r1]
	movs r5, #0xf
	ands r5, r0
	ldrh r7, [r1, #2]
	bl ResetMenuOverrides
	movs r6, #1
	mov r8, r4
	cmp r5, #1
	beq _08010920
	cmp r5, #1
	bgt _08010950
	cmp r5, #0
	bne _08010950
	movs r5, #0
_080108EC:
	adds r0, r7, #0
	ands r0, r6
	cmp r0, #0
	beq _08010902
	mov r1, sp
	adds r0, r1, r5
	ldrb r0, [r0]
	movs r1, #1
	ldr r2, _0801091C  @ MenuAlwaysNotShown
	bl AddMenuOverride
_08010902:
	lsls r0, r6, #0x11
	lsrs r6, r0, #0x10
	adds r0, r5, #1
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #0xe
	bls _080108EC
	b _08010950
	.align 2, 0
_08010914: .4byte gUnknown_080D793F
_08010918: .4byte gUnknown_080D794E
_0801091C: .4byte MenuAlwaysNotShown
_08010920:
	movs r5, #0
_08010922:
	adds r0, r7, #0
	ands r0, r6
	cmp r0, #0
	beq _08010942
	mov r0, r8
	adds r4, r0, r5
	ldrb r0, [r4]
	movs r1, #1
	ldr r2, _08010960  @ MenuAlwaysDisabled
	bl AddMenuOverride
	ldrb r0, [r4]
	movs r1, #2
	ldr r2, _08010964  @ Get8
	bl AddMenuOverride
_08010942:
	lsls r0, r6, #0x11
	lsrs r6, r0, #0x10
	adds r0, r5, #1
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	cmp r5, #4
	bls _08010922
_08010950:
	movs r0, #0
	add sp, #0x18
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08010960: .4byte MenuAlwaysDisabled
_08010964: .4byte Get8

	THUMB_FUNC_START Event3E_PrepScreenCall
Event3E_PrepScreenCall: @ 0x08010968
	push {r4, lr}
	adds r4, r0, #0
	bl HideAllUnits
	movs r0, #0x84
	bl UnsetEventId
	ldr r0, _08010988  @ gUnknown_0859DBBC
	adds r1, r4, #0
	bl Proc_CreateBlockingChild
	movs r0, #2
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08010988: .4byte gUnknown_0859DBBC

	THUMB_FUNC_START sub_801098C
sub_801098C: @ 0x0801098C
	push {r4, r5, r6, r7, lr}
	ldr r4, _08010A18  @ gEventSlotQueue
	ldr r3, _08010A1C  @ gUnknown_0203A974
	movs r5, #0
	ldr r0, _08010A20  @ gEventSlots
	ldr r1, [r0, #0x34]
	adds r7, r0, #0
	cmp r5, r1
	bcs _08010A04
	ldrb r0, [r4]
	cmp r0, #0xff
	beq _08010A04
	movs r6, #7
_080109A6:
	ldrh r2, [r4, #2]
	ldr r0, [r3]
	ldr r1, _08010A24  @ 0xFFF80000
	ands r0, r1
	orrs r0, r2
	str r0, [r3]
	ldrb r0, [r3, #2]
	adds r2, r6, #0
	ands r2, r0
	strb r2, [r3, #2]
	cmp r5, #0
	bne _080109CE
	ldr r0, [r3]
	lsls r0, r0, #8
	lsrs r0, r0, #0x1b
	movs r1, #1
	orrs r0, r1
	lsls r0, r0, #3
	orrs r0, r2
	strb r0, [r3, #2]
_080109CE:
	ldrb r0, [r4]
	cmp r0, #1
	bne _080109EA
	ldr r1, [r3]
	lsls r1, r1, #8
	lsrs r1, r1, #0x1b
	movs r0, #8
	orrs r1, r0
	lsls r1, r1, #3
	ldrb r2, [r3, #2]
	adds r0, r6, #0
	ands r0, r2
	orrs r0, r1
	strb r0, [r3, #2]
_080109EA:
	ldrb r0, [r4, #1]
	strb r0, [r3, #3]
	adds r3, #4
	adds r4, #4
	adds r0, r5, #1
	lsls r0, r0, #0x18
	lsrs r5, r0, #0x18
	ldr r0, [r7, #0x34]
	cmp r5, r0
	bcs _08010A04
	ldrb r0, [r4]
	cmp r0, #0xff
	bne _080109A6
_08010A04:
	ldrb r0, [r3, #2]
	movs r1, #7
	ands r1, r0
	movs r0, #0x80
	orrs r1, r0
	strb r1, [r3, #2]
	ldr r0, _08010A1C  @ gUnknown_0203A974
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08010A18: .4byte gEventSlotQueue
_08010A1C: .4byte gUnknown_0203A974
_08010A20: .4byte gEventSlots
_08010A24: .4byte 0xFFF80000

	THUMB_FUNC_START sub_8010A28
sub_8010A28: @ 0x08010A28
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r0, #0x64
	movs r1, #0
	ldrsh r4, [r0, r1]
	bl GetThread2SkipStack
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r4, r0
	bne _08010A50
	bl sub_80121D4
	ldr r0, [r5, #0x58]
	movs r1, #6
	bl Proc_SetMark
	adds r0, r5, #0
	bl Proc_ClearNativeCallback
_08010A50:
	pop {r4, r5}
	pop {r0}
	bx r0

	THUMB_FUNC_START Event3F_
Event3F_: @ 0x08010A58
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	adds r6, r0, #0
	ldr r0, [r6, #0x38]
	ldrb r1, [r0]
	movs r7, #0xf
	ands r7, r1
	ldrh r4, [r0, #2]
	ldrh r5, [r0, #4]
	ldrh r1, [r0, #6]
	ldrb r0, [r0, #6]
	str r0, [sp, #0xc]
	lsrs r1, r1, #8
	str r1, [sp, #0x10]
	ldr r0, _08010A88  @ gEventSlots
	ldr r0, [r0, #0x34]
	cmp r0, #0
	bne _08010A8C
	movs r0, #0
	b _08010A90
	.align 2, 0
_08010A88: .4byte gEventSlots
_08010A8C:
	bl sub_801098C
_08010A90:
	mov r8, r0
	cmp r7, #0
	blt _08010B36
	cmp r7, #1
	ble _08010AA0
	cmp r7, #2
	beq _08010B2C
	b _08010B36
_08010AA0:
	lsls r0, r4, #0x10
	cmp r0, #0
	bge _08010AA8
	ldr r4, _08010AE0  @ 0x0000FFFE
_08010AA8:
	lsls r0, r5, #0x10
	cmp r0, #0
	bge _08010AB0
	ldr r5, _08010AE0  @ 0x0000FFFE
_08010AB0:
	lsls r0, r4, #0x10
	asrs r0, r0, #0x10
	bl GetUnitStructFromEventParameter
	mov sl, r0
	lsls r0, r5, #0x10
	asrs r0, r0, #0x10
	bl GetUnitStructFromEventParameter
	mov r9, r0
	ldrh r2, [r6, #0x3c]
	lsrs r0, r2, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _08010ADA
	movs r0, #0x80
	lsls r0, r0, #1
	ands r0, r2
	cmp r0, #0
	beq _08010AE4
_08010ADA:
	movs r5, #0
	b _08010B06
	.align 2, 0
_08010AE0: .4byte 0x0000FFFE
_08010AE4:
	movs r5, #1
	ldr r0, _08010B28  @ gUnknown_08591F18
	adds r1, r6, #0
	bl Proc_CreateBlockingChild
	adds r4, r0, #0
	str r6, [r4, #0x58]
	bl GetThread2SkipStack
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r4, #0x64
	strh r0, [r4]
	adds r0, r6, #0
	movs r1, #7
	bl Proc_SetMark
_08010B06:
	adds r3, r5, #0
	ldr r0, [sp, #0xc]
	str r0, [sp]
	mov r0, r8
	str r0, [sp, #4]
	negs r0, r7
	orrs r0, r7
	lsrs r0, r0, #0x1f
	str r0, [sp, #8]
	mov r0, sl
	mov r1, r9
	ldr r2, [sp, #0x10]
	bl sub_8011F5C
	movs r0, #2
	b _08010B38
	.align 2, 0
_08010B28: .4byte gUnknown_08591F18
_08010B2C:
	mov r0, r8
	bl SetScriptedBattle
	movs r0, #0
	b _08010B38
_08010B36:
	movs r0, #6
_08010B38:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	THUMB_FUNC_START sub_8010B48
sub_8010B48: @ 0x08010B48
	push {r4, r5, lr}
	adds r5, r0, #0
	adds r0, #0x64
	movs r1, #0
	ldrsh r4, [r0, r1]
	bl GetThread2SkipStack
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r4, r0
	bne _08010B70
	bl sub_8012324
	ldr r0, [r5, #0x58]
	movs r1, #6
	bl Proc_SetMark
	adds r0, r5, #0
	bl Proc_ClearNativeCallback
_08010B70:
	pop {r4, r5}
	pop {r0}
	bx r0

	THUMB_FUNC_START Event40_
Event40_: @ 0x08010B78
	push {r4, r5, r6, lr}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	adds r6, r0, #0
	ldr r0, [r6, #0x38]
	ldrh r5, [r0, #2]
	ldrh r1, [r0, #4]
	mov r8, r1
	ldrh r0, [r0, #6]
	mov r9, r0
	ldr r0, _08010BE8  @ gUnknown_08591F28
	adds r1, r6, #0
	bl Proc_CreateBlockingChild
	adds r4, r0, #0
	str r6, [r4, #0x58]
	bl GetThread2SkipStack
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r4, #0x64
	strh r0, [r4]
	adds r0, r6, #0
	movs r1, #7
	bl Proc_SetMark
	lsls r5, r5, #0x10
	asrs r5, r5, #0x10
	adds r0, r5, #0
	bl GetUnitStructFromEventParameter
	adds r4, r0, #0
	movs r1, #0
	bl SetUnitStatus
	mov r0, r8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov r8, r0
	mov r1, r9
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	mov r9, r1
	adds r0, r4, #0
	mov r1, r8
	mov r2, r9
	bl sub_8012270
	movs r0, #2
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08010BE8: .4byte gUnknown_08591F28

	THUMB_FUNC_START Event41_
Event41_: @ 0x08010BEC
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r5, r0, #0
	ldrh r6, [r5, #0x3c]
	lsrs r4, r6, #2
	movs r0, #1
	ands r4, r0
	cmp r4, #0
	beq _08010C02
	movs r0, #0
	b _08010C68
_08010C02:
	ldr r2, [r5, #0x38]
	ldrb r0, [r2]
	movs r3, #0xf
	ands r3, r0
	cmp r3, #0
	blt _08010C66
	cmp r3, #1
	ble _08010C18
	cmp r3, #0xf
	beq _08010C56
	b _08010C66
_08010C18:
	ldrb r2, [r2, #2]
	adds r1, r2, #0
	lsls r0, r2, #0x18
	cmp r0, #0
	bge _08010C28
	ldr r0, _08010C40  @ gUnknown_030004E4
	ldrb r1, [r0]
	ldrb r2, [r0, #2]
_08010C28:
	movs r0, #0x40
	ands r0, r6
	cmp r0, #0
	bne _08010C44
	lsls r1, r1, #0x18
	asrs r1, r1, #0x18
	lsls r2, r2, #0x18
	asrs r2, r2, #0x18
	movs r0, #1
	str r0, [sp]
	b _08010C4E
	.align 2, 0
_08010C40: .4byte gUnknown_030004E4
_08010C44:
	lsls r1, r1, #0x18
	asrs r1, r1, #0x18
	lsls r2, r2, #0x18
	asrs r2, r2, #0x18
	str r4, [sp]
_08010C4E:
	adds r0, r5, #0
	bl sub_8012950
	b _08010C66
_08010C56:
	bl sub_8012974
	lsls r0, r0, #0x18
	asrs r0, r0, #0x18
	cmp r0, #1
	bne _08010C66
	movs r0, #3
	b _08010C68
_08010C66:
	movs r0, #2
_08010C68:
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	THUMB_FUNC_START Event42_
Event42_: @ 0x08010C70
	push {r4, r5, lr}
	adds r2, r0, #0
	ldr r0, [r2, #0x38]
	ldrb r1, [r0]
	movs r3, #0xf
	ands r3, r1
	ldrh r1, [r0, #2]
	ldrb r5, [r0, #2]
	lsrs r4, r1, #8
	cmp r3, #0
	beq _08010C8C
	cmp r3, #0xf
	beq _08010CE2
	b _08010CE6
_08010C8C:
	ldrh r0, [r2, #0x3c]
	lsrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _08010CE6
	adds r0, r2, #0
	adds r0, #0x42
	ldrb r0, [r0]
	cmp r0, #5
	bhi _08010CD4
	lsls r0, r0, #2
	ldr r1, _08010CAC  @ _08010CB0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08010CAC: .4byte _08010CB0
_08010CB0: @ jump table
	.4byte _08010CC8 @ case 0
	.4byte _08010CCC @ case 1
	.4byte _08010CD0 @ case 2
	.4byte _08010CC8 @ case 3
	.4byte _08010CC8 @ case 4
	.4byte _08010CD0 @ case 5
_08010CC8:
	movs r3, #0
	b _08010CD4
_08010CCC:
	movs r3, #1
	b _08010CD4
_08010CD0:
	movs r0, #6
	b _08010CE8
_08010CD4:
	lsls r2, r4, #0x18
	asrs r2, r2, #0x18
	adds r0, r3, #0
	adds r1, r5, #0
	bl sub_8012C34
	b _08010CE6
_08010CE2:
	bl sub_8012C88
_08010CE6:
	movs r0, #0
_08010CE8:
	pop {r4, r5}
	pop {r1}
	bx r1

	THUMB_FUNC_START Event43_
Event43_: @ 0x08010CF0
	push {r4, lr}
	adds r4, r0, #0
	ldrh r0, [r4, #0x3c]
	lsrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _08010D04
	movs r0, #0
	b _08010D20
_08010D04:
	ldr r0, [r4, #0x38]
	movs r1, #2
	ldrsh r0, [r0, r1]
	bl GetUnitStructFromEventParameter
	adds r1, r0, #0
	cmp r1, #0
	beq _08010D1E
	adds r0, r4, #0
	bl sub_8080E9C
	movs r0, #2
	b _08010D20
_08010D1E:
	movs r0, #6
_08010D20:
	pop {r4}
	pop {r1}
	bx r1

	THUMB_FUNC_START Event44_
Event44_: @ 0x08010D28
	push {r4, lr}
	adds r4, r0, #0
	ldrh r0, [r4, #0x3c]
	lsrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _08010D3C
	movs r0, #0
	b _08010D56
_08010D3C:
	ldr r0, [r4, #0x38]
	movs r1, #2
	ldrsh r0, [r0, r1]
	bl GetUnitStructFromEventParameter
	cmp r0, #0
	beq _08010D54
	adds r1, r4, #0
	bl sub_8012CFC
	movs r0, #2
	b _08010D56
_08010D54:
	movs r0, #6
_08010D56:
	pop {r4}
	pop {r1}
	bx r1

	THUMB_FUNC_START Event45_
Event45_: @ 0x08010D5C
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r1, [r5, #0x38]
	ldrb r0, [r1]
	movs r4, #0xf
	ands r4, r0
	movs r2, #2
	ldrsh r0, [r1, r2]
	bl GetUnitStructFromEventParameter
	adds r2, r0, #0
	cmp r2, #0
	bne _08010D7A
	movs r0, #6
	b _08010DB8
_08010D7A:
	cmp r4, #0
	beq _08010D84
	cmp r4, #0xf
	beq _08010D9A
	b _08010DB6
_08010D84:
	ldrh r0, [r5, #0x3c]
	lsrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _08010DB6
	adds r0, r5, #0
	adds r1, r2, #0
	bl sub_8081020
	b _08010DB6
_08010D9A:
	ldrh r0, [r5, #0x3c]
	lsrs r0, r0, #2
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	bne _08010DB2
	adds r0, r5, #0
	movs r1, #0x78
	bl sub_80811D0
	movs r0, #2
	b _08010DB8
_08010DB2:
	bl sub_8081068
_08010DB6:
	movs r0, #0
_08010DB8:
	pop {r4, r5}
	pop {r1}
	bx r1

	.align 2, 0 @ Don't pad with nop.
