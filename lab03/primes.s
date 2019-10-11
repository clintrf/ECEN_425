; Generated by c86 (BYU-NASM) 5.1 (beta) from primes.i
	CPU	8086
	ALIGN	2
	jmp	main	; Jump to program start
L_primes_3:
	DB	":",0xA,0
L_primes_2:
	DB	" and ",0
L_primes_1:
	DB	"Primes between ",0
	ALIGN	2
main:
	; >>>>> Line:	7
	; >>>>> { 
	jmp	L_primes_4
L_primes_5:
	; >>>>> Line:	11
	; >>>>> printString("Primes between "); 
	mov	ax, L_primes_1
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	12
	; >>>>> printUInt(1001U); 
	mov	ax, 1001
	push	ax
	call	printUInt
	add	sp, 2
	; >>>>> Line:	13
	; >>>>> printString(" and "); 
	mov	ax, L_primes_2
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	14
	; >>>>> printUInt(65000U); 
	mov	ax, 65000
	push	ax
	call	printUInt
	add	sp, 2
	; >>>>> Line:	15
	; >>>>> printString(":\n"); 
	mov	ax, L_primes_3
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	16
	; >>>>> lncnt = 0; 
	mov	word [bp-8], 0
	; >>>>> Line:	18
	; >>>>> for (i = 1001U; i < 65000U; i += 2) { 
	mov	word [bp-2], 1001
	jmp	L_primes_7
L_primes_6:
	; >>>>> Line:	19
	; >>>>> flag = 0; 
	mov	word [bp-6], 0
	; >>>>> Line:	20
	; >>>>> for (j = 3; (j*j) < i; j += 2) { 
	mov	word [bp-4], 3
	jmp	L_primes_11
L_primes_10:
	; >>>>> Line:	21
	; >>>>> if (i % j == 0) { 
	mov	ax, word [bp-2]
	xor	dx, dx
	div	word [bp-4]
	mov	ax, dx
	test	ax, ax
	jne	L_primes_14
	; >>>>> Line:	22
	; >>>>> flag = 1; 
	mov	word [bp-6], 1
	; >>>>> Line:	23
	; >>>>> break; 
	jmp	L_primes_12
L_primes_14:
L_primes_13:
	add	word [bp-4], 2
L_primes_11:
	mov	ax, word [bp-4]
	imul	word [bp-4]
	mov	dx, word [bp-2]
	cmp	dx, ax
	ja	L_primes_10
L_primes_12:
	; >>>>> Line:	26
	; >>>>> if (!flag) { 
	mov	ax, word [bp-6]
	test	ax, ax
	jne	L_primes_15
	; >>>>> Line:	27
	; >>>>> printChar(' '); 
	mov	al, 32
	push	ax
	call	printChar
	add	sp, 2
	; >>>>> Line:	28
	; >>>>> printUInt(i 
	push	word [bp-2]
	call	printUInt
	add	sp, 2
	; >>>>> Line:	29
	; >>>>> lncnt++; 
	inc	word [bp-8]
	; >>>>> Line:	30
	; >>>>> if (lncnt > 9) { 
	cmp	word [bp-8], 9
	jle	L_primes_16
	; >>>>> Line:	31
	; >>>>> printNewLine(); 
	call	printNewLine
	; >>>>> Line:	32
	; >>>>> lncnt = 0; 
	mov	word [bp-8], 0
L_primes_16:
L_primes_15:
L_primes_9:
	add	word [bp-2], 2
L_primes_7:
	cmp	word [bp-2], 65000
	jb	L_primes_6
L_primes_8:
	; >>>>> Line:	37
	; >>>>> printNewLine(); 
	call	printNewLine
	mov	sp, bp
	pop	bp
	ret
L_primes_4:
	push	bp
	mov	bp, sp
	sub	sp, 8
	jmp	L_primes_5
