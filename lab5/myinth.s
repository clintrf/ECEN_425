; Generated by c86 (BYU-NASM) 5.1 (beta) from myinth.i
	CPU	8086
	ALIGN	2
	jmp	main	; Jump to program start
L_myinth_1:
	DB	0xA,"RESET PROGRAM",0xA,0
	ALIGN	2
c_reset_handler:
	; >>>>> Line:	13
	; >>>>> void c_reset_handler(){ 
	jmp	L_myinth_2
L_myinth_3:
	; >>>>> Line:	14
	; >>>>> printString("\nRESET PROGRAM\n"); 
	mov	ax, L_myinth_1
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	15
	; >>>>> exit(0); 
	xor	al, al
	push	ax
	call	exit
	add	sp, 2
	mov	sp, bp
	pop	bp
	ret
L_myinth_2:
	push	bp
	mov	bp, sp
	jmp	L_myinth_3
	ALIGN	2
L_myinth_5:
	DW	0
L_myinth_6:
	DB	0xA,"TICK ",0
	ALIGN	2
c_tick_handler:
	; >>>>> Line:	18
	; >>>>> void c_tick_handler(){ 
	jmp	L_myinth_7
L_myinth_8:
	; >>>>> Line:	20
	; >>>>> printString("\nTICK "); 
	mov	ax, L_myinth_6
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	21
	; >>>>> printInt(tick++); 
	mov	ax, word [L_myinth_5]
	inc	word [L_myinth_5]
	push	ax
	call	printInt
	add	sp, 2
	; >>>>> Line:	22
	; >>>>> printNewLine(); 
	call	printNewLine
	; >>>>> Line:	24
	; >>>>> YKTickHandler(); 
	call	YKTickHandler
	mov	sp, bp
	pop	bp
	ret
L_myinth_7:
	push	bp
	mov	bp, sp
	jmp	L_myinth_8
L_myinth_14:
	DB	") IGNORED*********************************************",0xD,0xA,0
L_myinth_13:
	DB	0xD,0xA,"KEYPRESS (",0
L_myinth_12:
	DB	0xD,0xA," P KEY PRESSED",0xD,0xA,0
L_myinth_11:
	DB	0xD,0xA,"DELAY COMPLETE$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",0xD,0xA,0
L_myinth_10:
	DB	0xD,0xA,"DELAY KEY PRESSED",0xD,0xA,0
	ALIGN	2
c_key_handler:
	; >>>>> Line:	27
	; >>>>> void c_key_handler(){ 
	jmp	L_myinth_15
L_myinth_16:
	; >>>>> Line:	29
	; >>>>> if (c == 'd'){ 
	mov	al, byte [KeyBuffer]
	mov	byte [bp-1], al
	; >>>>> Line:	29
	; >>>>> if (c == 'd'){ 
	cmp	byte [bp-1], 100
	jne	L_myinth_17
	; >>>>> Line:	30
	; >>>>> printString("\r\nDELAY KEY PRESSED\r\n"); 
	mov	ax, L_myinth_10
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	31
	; >>>>> delay 
	call	delay
	; >>>>> Line:	32
	; >>>>> printString("\r\nDELAY COMPLETE$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\r\n"); 
	mov	ax, L_myinth_11
	push	ax
	call	printString
	add	sp, 2
	jmp	L_myinth_18
L_myinth_17:
	; >>>>> Line:	34
	; >>>>> else if(c == 'p'){ 
	cmp	byte [bp-1], 112
	jne	L_myinth_19
	; >>>>> Line:	35
	; >>>>> printString("\r\n P KEY PRESSED\r\n"); 
	mov	ax, L_myinth_12
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	36
	; >>>>> YKSemPost(NSemPtr); 
	push	word [NSemPtr]
	call	YKSemPost
	add	sp, 2
	jmp	L_myinth_20
L_myinth_19:
	; >>>>> Line:	39
	; >>>>> printString("\r\nKEYPRESS ("); 
	mov	ax, L_myinth_13
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	40
	; >>>>> printChar(c); 
	push	word [bp-1]
	call	printChar
	add	sp, 2
	; >>>>> Line:	41
	; >>>>> printString(") IGNORED*********************************************\r\n"); 
	mov	ax, L_myinth_14
	push	ax
	call	printString
	add	sp, 2
L_myinth_20:
L_myinth_18:
	mov	sp, bp
	pop	bp
	ret
L_myinth_15:
	push	bp
	mov	bp, sp
	push	cx
	jmp	L_myinth_16
	ALIGN	2
delay:
	; >>>>> Line:	45
	; >>>>> void delay(){ 
	jmp	L_myinth_22
L_myinth_23:
	; >>>>> Line:	47
	; >>>>> for(i = 0; i < 5000; i++){} 
	mov	word [bp-2], 0
	; >>>>> Line:	47
	; >>>>> for(i = 0; i < 5000; i++){} 
	mov	word [bp-2], 0
	jmp	L_myinth_25
L_myinth_24:
L_myinth_27:
	; >>>>> Line:	47
	; >>>>> for(i = 0; i < 5000; i++){} 
	inc	word [bp-2]
L_myinth_25:
	cmp	word [bp-2], 5000
	jl	L_myinth_24
L_myinth_26:
	mov	sp, bp
	pop	bp
	ret
L_myinth_22:
	push	bp
	mov	bp, sp
	push	cx
	jmp	L_myinth_23
L_myinth_29:
	DB	0xD,0xA,"DEBUGGER",0xD,0xA,0
	ALIGN	2
print_debug:
	; >>>>> Line:	50
	; >>>>> void print_debug(){ 
	jmp	L_myinth_30
L_myinth_31:
	; >>>>> Line:	51
	; >>>>> printString("\r\nDEBUGGER\r\n"); 
	mov	ax, L_myinth_29
	push	ax
	call	printString
	add	sp, 2
	mov	sp, bp
	pop	bp
	ret
L_myinth_30:
	push	bp
	mov	bp, sp
	jmp	L_myinth_31