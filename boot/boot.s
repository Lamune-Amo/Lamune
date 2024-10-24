.global start

.section .inittext
; entry for kernel
start:
	; stack
	mov sp, $71920
	mov fp, sp

	sub sp, sp, $0x4
	str [sp], lr

	; jump to c-based kernel
	jal kernel_init

	ldr lr, [sp]
	add sp, sp, $0x4

	; jump to bios section
	jmp lr

.ltorg
