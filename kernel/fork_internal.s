	.text
	.align	4
	.global _fork_copy
_fork_copy:
	; r0 return (int)
	; r2 task->regs
	; r3 old_stack
	; r4 new_stack
	str [r2], r0
	str [r2, $4], r1
	str [r2, $8], r2
	str [r2, $12], r3
	str [r2, $16], r4
	str [r2, $20], r5
	str [r2, $24], r6
	str [r2, $28], r7
	str [r2, $32], r8
	str [r2, $36], r9

	str [r2, $40], r10
	str [r2, $44], r11
	str [r2, $48], r12
	str [r2, $52], r13
	str [r2, $56], r14
	str [r2, $60], r15
	str [r2, $64], r16
	str [r2, $68], r17
	str [r2, $72], r18
	str [r2, $76], r19

	str [r2, $80], r20
	str [r2, $84], r21
	str [r2, $88], r22
	str [r2, $92], r23
	str [r2, $96], r24
	str [r2, $100], r25
	str [r2, $104], r28
	str [r2, $108], fp
	str [r2, $116], lr

	; stack pointer
	sub r0, sp, r3
	add r0, r0, r4
	str [r2, $112], r0

	; pc
	mov r0, child
	str [r2, $120], r0

	; parent
	mov r0, $0
	jmp lr

child:
	mov r0, $1
	jmp lr

