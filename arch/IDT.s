	.text
	.align	4
	.global IDT
	.extern current_task
	.extern interrupt_stack
	.extern interrupt_irq
IDT:
	; vector table
	; 0x0
	jmp idt_reset
	; 0x4
	jmp idt_undefined
	; 0x8
	jmp idt_trap
	; 0xC
	jmp idt_irq

; far away jump
idt_reset:
	mov r0, $0x0
	ret r0

idt_undefined:
	mov r0, $0x4

idt_trap:
	ret lr

idt_irq:
	; save stack frame
	; r27 (interrupt only)
	mov r27, current_task
	ldr r27, [r27]

	; offset
	add r27, r27, $8

	; save regs
	str [r27], r0
	str [r27, $4], r1
	str [r27, $8], r2
	str [r27, $12], r3
	str [r27, $16], r4
	str [r27, $20], r5
	str [r27, $24], r6
	str [r27, $28], r7
	str [r27, $32], r8
	str [r27, $36], r9
	str [r27, $40], r10
	str [r27, $44], r11
	str [r27, $48], r12
	str [r27, $52], r13
	str [r27, $56], r14
	str [r27, $60], r15
	str [r27, $64], r16
	str [r27, $68], r17
	str [r27, $72], r18
	str [r27, $76], r19
	str [r27, $80], r20
	str [r27, $84], r21
	str [r27, $88], r22
	str [r27, $92], r23
	str [r27, $96], r24
	str [r27, $100], r25
	str [r27, $104], r28
	str [r27, $108], fp
	str [r27, $112], sp
	str [r27, $116], lr

	; switch to kerenl stack
	mov sp, interrupt_stack
	add sp, sp, $508

	; get irq device number from interrupt controller
	mov r0, $8896
	ldr r0, [r0]
	lsl r0, r0, $2

	mov r1, interrupt_irq
	add r1, r1, r0
	ldr r1, [r1]

	jal r1

	; load current task
	mov r27, current_task
	ldr r27, [r27]

	; offset
	add r27, r27, $8

	; restore regs
	ldr r0, [r27]
	ldr r1, [r27, $4]
	ldr r2, [r27, $8]
	ldr r3, [r27, $12]
	ldr r4, [r27, $16]
	ldr r5, [r27, $20]
	ldr r6, [r27, $24]
	ldr r7, [r27, $28]
	ldr r8, [r27, $32]
	ldr r9, [r27, $36]
	ldr r10, [r27, $40]
	ldr r11, [r27, $44]
	ldr r12, [r27, $48]
	ldr r13, [r27, $52]
	ldr r14, [r27, $56]
	ldr r15, [r27, $60]
	ldr r16, [r27, $64]
	ldr r17, [r27, $68]
	ldr r18, [r27, $72]
	ldr r19, [r27, $76]
	ldr r20, [r27, $80]
	ldr r21, [r27, $84]
	ldr r22, [r27, $88]
	ldr r23, [r27, $92]
	ldr r24, [r27, $96]
	ldr r25, [r27, $100]
	ldr r28, [r27, $104]

	; switch to stack
	ldr fp, [r27, $108]
	ldr sp, [r27, $112]
	ldr lr, [r27, $116]

	ret r26
