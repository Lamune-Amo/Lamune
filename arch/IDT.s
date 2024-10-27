	.text
	.align	4
	.global IDT
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
	mov r27, interrupt_stack
	str [r27], fp
	str [r27, $4], sp
	str [r27, $8], lr

	; switch to kerenl stack
	add sp, r27, $1032 ; 12 + 1024 - 4(margin)
	
	; save regs
	sub sp, sp, $108
	str [sp], r0
	str [sp, $4], r1
	str [sp, $8], r2
	str [sp, $12], r3
	str [sp, $16], r4
	str [sp, $20], r5
	str [sp, $24], r6
	str [sp, $28], r7
	str [sp, $32], r8
	str [sp, $36], r9
	str [sp, $40], r10
	str [sp, $44], r11
	str [sp, $48], r12
	str [sp, $52], r13
	str [sp, $56], r14
	str [sp, $60], r15
	str [sp, $64], r16
	str [sp, $68], r17
	str [sp, $72], r18
	str [sp, $76], r19
	str [sp, $80], r20
	str [sp, $84], r21
	str [sp, $88], r22
	str [sp, $92], r23
	str [sp, $96], r24
	str [sp, $100], r25
	str [sp, $104], r28

	; get irq device number from interrupt controller
	mov r0, $8896
	ldr r0, [r0]
	lsl r0, r0, $2

	mov r1, interrupt_irq
	add r1, r1, r0
	ldr r1, [r1]

	jal r1
	
	; restore regs
	ldr r0, [sp]
	ldr r1, [sp, $4]
	ldr r2, [sp, $8]
	ldr r3, [sp, $12]
	ldr r4, [sp, $16]
	ldr r5, [sp, $20]
	ldr r6, [sp, $24]
	ldr r7, [sp, $28]
	ldr r8, [sp, $32]
	ldr r9, [sp, $36]
	ldr r10, [sp, $40]
	ldr r11, [sp, $44]
	ldr r12, [sp, $48]
	ldr r13, [sp, $52]
	ldr r14, [sp, $56]
	ldr r15, [sp, $60]
	ldr r16, [sp, $64]
	ldr r17, [sp, $68]
	ldr r18, [sp, $72]
	ldr r19, [sp, $76]
	ldr r20, [sp, $80]
	ldr r21, [sp, $84]
	ldr r22, [sp, $88]
	ldr r23, [sp, $92]
	ldr r24, [sp, $96]
	ldr r25, [sp, $100]
	ldr r28, [sp, $104]
	add sp, sp, $108

	; switch to stack
	ldr lr, [r27, $8]
	ldr sp, [r27, $4]
	ldr fp, [r27]

	ret r26

