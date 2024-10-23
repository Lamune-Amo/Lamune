	.text
	.align	4
	.global IDT
	.extern irq
IDT:
	; vector table
	; 0x0
	jmp _idt_reset
	; 0x4
	jmp _idt_undefined
	; 0x8
	jmp _idt_interrupt
	; 0xC
	jmp _idt_irq

; far away jump
_idt_reset:
	mov r0, $0x0
	jmp r0

_idt_undefined:
	mov r0, $0x4

_idt_interrupt:
	jmp lr

_idt_irq:
	; switch to kerenl stack
	sub sp, sp, $4
	str [sp], r0
	mov r0, sp
	; kerenl sp should be moved here
	; mov sp, sp
	
	; save r0 and lr to kernel stack
	sub sp, sp, $8
	str [sp, $4], r0
	str [sp], lr

	; interrupt controller
	mov r2, $8896
	ldr r2, [r2]
	jal irq
	
	ldr lr, [sp]
	ldr r0, [sp, $4]
	add sp, sp, $8

	jmp lr

