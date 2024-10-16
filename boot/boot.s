.global start

.section .inittext
start:
	; vector table
	jmp reset
	jmp interrupt

; far away jump
reset:
	; initialize MEMORY

	; clear Video RAM
	mov r1, $4096
	mov r2, $8896
	mov r3, $0
.CL0:
	str [r1], r3
	add r1, r1, $4
	bne r1, r2, .CL0

	; clear RAM
	mov r1, $9000
	mov r2, $17192
	mov r3, $0
.CL1:
	str [r1], r3
	add r1, r1, $4
	bne r1, r2, .CL1

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
