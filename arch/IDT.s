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
	add sp, r27, $72 ; 12 + 64 - 4(margin)
	
	; save regs
	sub sp, sp, $4
	str [sp], r0

	mov fp, sp

	; get irq device number from interrupt controller
	mov r27, $8896
	ldr r27, [r27]

	lsl r27, r27, $2
	mov r0, interrupt_irq
	add r0, r0, r27
	ldr r0, [r0]

	jal r0
	
	ldr r0, [sp]
	add sp, sp, $4

	; restore stack frame
	mov r27, interrupt_stack
	ldr lr, [r27, $8]
	ldr sp, [r27, $4]
	ldr fp, [r27]

	ret lr

