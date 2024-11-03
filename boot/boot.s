.global start

.section .inittext
; entry for kernel
start:
	; stack
	mov sp, kernel_stack
	add sp, sp, $8192
	mov fp, sp

	sub sp, sp, $4
	str [sp], lr

	; jump to c-based kernel
	jal kernel_init

	ldr lr, [sp]
	add sp, sp, $0x4

	; jump to bios section
	jmp lr

.ltorg
.comm	kernel_stack,8192,4
