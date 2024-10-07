.global start

.section .inittext
start:
	; vector table
	jmp reset
	jmp interrupt

; far away jump
reset:
	; stack
	mov sp, $17176
	mov fp, sp
	; jump
	mov r0, kernel_init 
	jal r0

	; halt
.hlt:
	jmp .hlt

interrupt:
	mov r0, $42 ; temporary
	jmp r0

.ltorg
