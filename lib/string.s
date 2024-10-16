	.file	"string.c"
	.text
	.align	4
	.global	strlen
	.type	strlen, @function
strlen:
	sub		sp, sp, $4
	str		[sp], fp
	add		sp, sp, $-8
	mov		fp, sp
	str		[fp, $4], r2
	mov		r0, $0
	str		[fp], r0
	jmp		.L2
.L3:
	ldr		r0, [fp]
	add		r0, r0, $1
	str		[fp], r0
.L2:
	ldr		r1, [fp, $4]
	ldr		r0, [fp]
	add		r0, r1, r0
	ldrb	r1, [r0]
	mov		r0, $0
	bne		r1, r0, .L3
	ldr		r0, [fp]
	mov		sp, fp
	add		sp, sp, $8
	ldr		fp, [sp]
	add		sp, sp, $4
	jmp		lr
.ltorg
	.size	strlen, .-strlen
	.align	4
	.global	strcpy
	.type	strcpy, @function
strcpy:
	sub		sp, sp, $8
	str		[sp, $4], lr
	str		[sp], fp
	add		sp, sp, $-12
	mov		fp, sp
	str		[fp, $4], r2
	str		[fp, $8], r3
	mov		r0, $0
	str		[fp], r0
	jmp		.L6
.L7:
	ldr		r0, [fp]
	ldr		r1, [fp, $8]
	add		r1, r1, r0
	ldr		r0, [fp]
	ldr		r2, [fp, $4]
	add		r0, r2, r0
	ldrb	r1, [r1]
	strb	[r0], r1
	ldr		r0, [fp]
	add		r0, r0, $1
	str		[fp], r0
.L6:
	ldr		r0, [fp, $8]
	mov		r2, r0
	jal		strlen
	mov		r1, r0
	ldr		r0, [fp]
	bltu	r0, r1, .L7
	ldr		r0, [fp, $4]
	mov		sp, fp
	add		sp, sp, $12
	ldr		fp, [sp]
	ldr		lr, [sp, $4]
	add		sp, sp, $8
	jmp		lr
.ltorg
	.size	strcpy, .-strcpy
	.align	4
	.global	strncpy
	.type	strncpy, @function
strncpy:
	sub		sp, sp, $4
	str		[sp], fp
	add		sp, sp, $-16
	mov		fp, sp
	str		[fp, $4], r2
	str		[fp, $8], r3
	str		[fp, $12], r4
	mov		r0, $0
	str		[fp], r0
	jmp		.L10
.L11:
	ldr		r0, [fp]
	ldr		r1, [fp, $8]
	add		r1, r1, r0
	ldr		r0, [fp]
	ldr		r2, [fp, $4]
	add		r0, r2, r0
	ldrb	r1, [r1]
	strb	[r0], r1
	ldr		r0, [fp]
	add		r0, r0, $1
	str		[fp], r0
.L10:
	ldr		r0, [fp]
	ldr		r1, [fp, $12]
	bltu	r0, r1, .L11
	ldr		r0, [fp, $4]
	mov		sp, fp
	add		sp, sp, $16
	ldr		fp, [sp]
	add		sp, sp, $4
	jmp		lr
.ltorg
	.size	strncpy, .-strncpy
	.align	4
	.global	strcmp
	.type	strcmp, @function
strcmp:
	sub		sp, sp, $4
	str		[sp], fp
	add		sp, sp, $-8
	mov		fp, sp
	str		[fp], r2
	str		[fp, $4], r3
	jmp		.L14
.L16:
	ldr		r0, [fp]
	add		r0, r0, $1
	str		[fp], r0
	ldr		r0, [fp, $4]
	add		r0, r0, $1
	str		[fp, $4], r0
.L14:
	ldr		r0, [fp]
	ldrb	r1, [r0]
	mov		r0, $0
	beq		r1, r0, .L15
	ldr		r0, [fp, $4]
	ldrb	r1, [r0]
	mov		r0, $0
	beq		r1, r0, .L15
	ldr		r0, [fp]
	ldrb	r1, [r0]
	ldr		r0, [fp, $4]
	ldrb	r0, [r0]
	beq		r1, r0, .L16
.L15:
	ldr		r0, [fp]
	ldrb	r0, [r0]
	ext		r1, r0, $1
	ldr		r0, [fp, $4]
	ldrb	r0, [r0]
	ext		r0, r0, $1
	sub		r0, r1, r0
	mov		sp, fp
	add		sp, sp, $8
	ldr		fp, [sp]
	add		sp, sp, $4
	jmp		lr
.ltorg
	.size	strcmp, .-strcmp
	.align	4
	.global	strncmp
	.type	strncmp, @function
strncmp:
	sub		sp, sp, $4
	str		[sp], fp
	add		sp, sp, $-16
	mov		fp, sp
	str		[fp, $4], r2
	str		[fp, $8], r3
	str		[fp, $12], r4
	mov		r0, $0
	str		[fp], r0
	jmp		.L19
.L21:
	ldr		r0, [fp, $4]
	add		r0, r0, $1
	str		[fp, $4], r0
	ldr		r0, [fp, $8]
	add		r0, r0, $1
	str		[fp, $8], r0
	ldr		r0, [fp]
	add		r0, r0, $1
	str		[fp], r0
.L19:
	ldr		r0, [fp, $4]
	ldrb	r1, [r0]
	mov		r0, $0
	beq		r1, r0, .L20
	ldr		r0, [fp, $8]
	ldrb	r1, [r0]
	mov		r0, $0
	beq		r1, r0, .L20
	ldr		r0, [fp, $4]
	ldrb	r1, [r0]
	ldr		r0, [fp, $8]
	ldrb	r0, [r0]
	bne		r1, r0, .L20
	ldr		r0, [fp, $12]
	add		r0, r0, $-1
	ldr		r1, [fp]
	bltu	r1, r0, .L21
.L20:
	ldr		r0, [fp, $4]
	ldrb	r0, [r0]
	ext		r1, r0, $1
	ldr		r0, [fp, $8]
	ldrb	r0, [r0]
	ext		r0, r0, $1
	sub		r0, r1, r0
	mov		sp, fp
	add		sp, sp, $16
	ldr		fp, [sp]
	add		sp, sp, $4
	jmp		lr
.ltorg
	.size	strncmp, .-strncmp
	.align	4
	.global	strchr
	.type	strchr, @function
strchr:
	sub		sp, sp, $4
	str		[sp], fp
	add		sp, sp, $-8
	mov		fp, sp
	str		[fp], r2
	str		[fp, $4], r3
	jmp		.L24
.L27:
	ldr		r0, [fp]
	ldrb	r0, [r0]
	ext		r0, r0, $1
	ldr		r1, [fp, $4]
	bne		r1, r0, .L25
	ldr		r0, [fp]
	jmp		.L26
.L25:
	ldr		r0, [fp]
	add		r0, r0, $1
	str		[fp], r0
.L24:
	ldr		r0, [fp]
	ldrb	r1, [r0]
	mov		r0, $0
	bne		r1, r0, .L27
	mov		r0, $0
.L26:
	mov		sp, fp
	add		sp, sp, $8
	ldr		fp, [sp]
	add		sp, sp, $4
	jmp		lr
.ltorg
	.size	strchr, .-strchr
	.section	.rodata
	.align	4
.LC0:
	.string	"0123456789abcdef"
	.align	4
.LC1:
	.string	"0123456789ABCDEF"
	.text
	.align	4
	.type	atoi_hex, @function
atoi_hex:
	sub		sp, sp, $8
	str		[sp, $4], lr
	str		[sp], fp
	add		sp, sp, $-28
	mov		fp, sp
	str		[fp, $24], r2
	mov		r0, .LC0
	str		[fp, $16], r0
	mov		r0, .LC1
	str		[fp, $20], r0
	mov		r0, $0
	str		[fp, $8], r0
	mov		r0, $0
	str		[fp, $12], r0
	jmp		.L29
.L31:
	ldr		r0, [fp, $8]
	lsl		r0, r0, $4
	str		[fp, $8], r0
	ldr		r0, [fp, $16]
	str		[fp], r0
	ldr		r0, [fp, $12]
	ldr		r1, [fp, $24]
	add		r0, r1, r0
	ldrb	r0, [r0]
	ext		r0, r0, $1
	mov		r3, r0
	ldr		r2, [fp]
	jal		strchr
	str		[fp, $4], r0
	ldr		r1, [fp, $4]
	mov		r0, $0
	bne		r1, r0, .L30
	ldr		r0, [fp, $20]
	str		[fp], r0
	ldr		r0, [fp, $12]
	ldr		r1, [fp, $24]
	add		r0, r1, r0
	ldrb	r0, [r0]
	ext		r0, r0, $1
	mov		r3, r0
	ldr		r2, [fp]
	jal		strchr
	str		[fp, $4], r0
.L30:
	ldr		r1, [fp, $4]
	ldr		r0, [fp]
	sub		r0, r1, r0
	ldr		r1, [fp, $8]
	add		r0, r1, r0
	str		[fp, $8], r0
	ldr		r0, [fp, $12]
	add		r0, r0, $1
	str		[fp, $12], r0
.L29:
	ldr		r0, [fp, $12]
	ldr		r1, [fp, $24]
	add		r0, r1, r0
	ldrb	r0, [r0]
	ext		r1, r0, $1
	ldr		r0, [fp, $16]
	mov		r3, r1
	mov		r2, r0
	jal		strchr
	mov		r1, r0
	mov		r0, $0
	bne		r1, r0, .L31
	ldr		r0, [fp, $12]
	ldr		r1, [fp, $24]
	add		r0, r1, r0
	ldrb	r0, [r0]
	ext		r1, r0, $1
	ldr		r0, [fp, $20]
	mov		r3, r1
	mov		r2, r0
	jal		strchr
	mov		r1, r0
	mov		r0, $0
	bne		r1, r0, .L31
	ldr		r0, [fp, $8]
	mov		sp, fp
	add		sp, sp, $28
	ldr		fp, [sp]
	ldr		lr, [sp, $4]
	add		sp, sp, $8
	jmp		lr
.ltorg
	.size	atoi_hex, .-atoi_hex
	.section	.rodata
	.align	4
.LC2:
	.string	"0123456789"
	.text
	.align	4
	.type	atoi_dec, @function
atoi_dec:
	sub		sp, sp, $8
	str		[sp, $4], lr
	str		[sp], fp
	add		sp, sp, $-16
	mov		fp, sp
	str		[fp, $12], r2
	mov		r0, $0
	str		[fp, $4], r0
	ldr		r0, [fp, $12]
	ldrb	r1, [r0]
	mov		r0, $45
	bne		r1, r0, .L34
	mov		r0, $1
	str		[fp, $4], r0
	ldr		r0, [fp, $12]
	add		r0, r0, $1
	str		[fp, $12], r0
.L34:
	mov		r0, $0
	str		[fp], r0
	mov		r0, $0
	str		[fp, $8], r0
	jmp		.L35
.L36:
	ldr		r1, [fp]
	mov		r0, r1
	lsl		r0, r0, $2
	add		r0, r0, r1
	add		r0, r0, r0
	str		[fp], r0
	ldr		r0, [fp, $8]
	ldr		r1, [fp, $12]
	add		r0, r1, r0
	ldrb	r0, [r0]
	ext		r0, r0, $1
	add		r0, r0, $-48
	ldr		r1, [fp]
	add		r0, r1, r0
	str		[fp], r0
	ldr		r0, [fp, $8]
	add		r0, r0, $1
	str		[fp, $8], r0
.L35:
	ldr		r0, [fp, $8]
	ldr		r1, [fp, $12]
	add		r0, r1, r0
	ldrb	r0, [r0]
	ext		r0, r0, $1
	mov		r3, r0
	mov		r2, .LC2
	jal		strchr
	mov		r1, r0
	mov		r0, $0
	bne		r1, r0, .L36
	ldr		r1, [fp, $4]
	mov		r0, $0
	beq		r1, r0, .L37
	mov		r1, $0
	ldr		r0, [fp]
	sub		r0, r1, r0
	str		[fp], r0
.L37:
	ldr		r0, [fp]
	mov		sp, fp
	add		sp, sp, $16
	ldr		fp, [sp]
	ldr		lr, [sp, $4]
	add		sp, sp, $8
	jmp		lr
.ltorg
	.size	atoi_dec, .-atoi_dec
	.section	.rodata
	.align	4
.LC3:
	.string	"0x"
	.align	4
.LC4:
	.string	"0"
	.text
	.align	4
	.global	atoi
	.type	atoi, @function
atoi:
	sub		sp, sp, $8
	str		[sp, $4], lr
	str		[sp], fp
	add		sp, sp, $-4
	mov		fp, sp
	str		[fp], r2
	mov		r4, $2
	mov		r3, .LC3
	ldr		r2, [fp]
	jal		strncmp
	mov		r1, r0
	mov		r0, $0
	bne		r1, r0, .L40
	ldr		r0, [fp]
	add		r0, r0, $2
	mov		r2, r0
	jal		atoi_hex
	jmp		.L41
.L40:
	mov		r3, .LC4
	ldr		r2, [fp]
	jal		strcmp
	mov		r1, r0
	mov		r0, $0
	beq		r1, r0, .L42
	ldr		r2, [fp]
	jal		atoi_dec
	jmp		.L41
.L42:
	mov		r0, $0
.L41:
	mov		sp, fp
	add		sp, sp, $4
	ldr		fp, [sp]
	ldr		lr, [sp, $4]
	add		sp, sp, $8
	jmp		lr
.ltorg
	.size	atoi, .-atoi
	.align	4
	.global	memset
	.type	memset, @function
memset:
	sub		sp, sp, $4
	str		[sp], fp
	add		sp, sp, $-20
	mov		fp, sp
	str		[fp, $8], r2
	str		[fp, $12], r3
	str		[fp, $16], r4
	ldr		r0, [fp, $16]
	lsr		r0, r0, $2
	str		[fp, $4], r0
	mov		r0, $0
	str		[fp], r0
	jmp		.L44
.L45:
	ldr		r0, [fp]
	lsl		r0, r0, $2
	ldr		r1, [fp, $8]
	add		r0, r1, r0
	ldr		r1, [fp, $12]
	str		[r0], r1
	ldr		r0, [fp]
	add		r0, r0, $1
	str		[fp], r0
.L44:
	ldr		r1, [fp]
	ldr		r0, [fp, $4]
	blt		r1, r0, .L45
	ldr		r0, [fp, $4]
	str		[fp], r0
	jmp		.L46
.L47:
	ldr		r0, [fp]
	ldr		r1, [fp, $8]
	add		r0, r1, r0
	ldr		r1, [fp, $12]
	strb	[r0], r1
	ldr		r0, [fp, $12]
	asr		r0, r0, $8
	str		[fp, $12], r0
	ldr		r0, [fp]
	add		r0, r0, $1
	str		[fp], r0
.L46:
	ldr		r0, [fp, $4]
	lsl		r0, r0, $2
	mov		r1, r0
	ldr		r0, [fp, $16]
	sub		r1, r0, r1
	ldr		r0, [fp]
	bltu	r0, r1, .L47
	ldr		r0, [fp, $8]
	mov		sp, fp
	add		sp, sp, $20
	ldr		fp, [sp]
	add		sp, sp, $4
	jmp		lr
.ltorg
	.size	memset, .-memset
	.ident	"GCC: (GNU) 8.4.0"
