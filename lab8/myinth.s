; Generated by c86 (BYU-NASM) 5.1 (beta) from myinth.i
	CPU	8086
	ALIGN	2
	jmp	main	; Jump to program start
	ALIGN	2
c_reset_handler:
	; >>>>> Line:	15
	; >>>>> void c_reset_handler(){ 
	jmp	L_myinth_1
L_myinth_2:
	; >>>>> Line:	16
	; >>>>> exit(0); 
	xor	al, al
	push	ax
	call	exit
	add	sp, 2
	mov	sp, bp
	pop	bp
	ret
L_myinth_1:
	push	bp
	mov	bp, sp
	jmp	L_myinth_2
	ALIGN	2
c_tick_handler:
	; >>>>> Line:	19
	; >>>>> void c_tick_handler(){ 
	jmp	L_myinth_4
L_myinth_5:
	; >>>>> Line:	20
	; >>>>> YKTickHandler(); 
	call	YKTickHandler
	mov	sp, bp
	pop	bp
	ret
L_myinth_4:
	push	bp
	mov	bp, sp
	jmp	L_myinth_5
L_myinth_8:
	DB	") IGNORED",0xA,0
L_myinth_7:
	DB	0xA,"KEYPRESS (",0
	ALIGN	2
c_key_handler:
	; >>>>> Line:	23
	; >>>>> void c_key_handler(){ 
	jmp	L_myinth_9
L_myinth_10:
	; >>>>> Line:	25
	; >>>>> c = KeyBuffer; 
	mov	al, byte [KeyBuffer]
	mov	byte [bp-1], al
	; >>>>> Line:	27
	; >>>>> print("\nKEYPRESS (", 11); 
	mov	ax, 11
	push	ax
	mov	ax, L_myinth_7
	push	ax
	call	print
	add	sp, 4
	; >>>>> Line:	28
	; >>>>> printChar(c); 
	push	word [bp-1]
	call	printChar
	add	sp, 2
	; >>>>> Line:	29
	; >>>>> print(") IGNORED\n", 10); 
	mov	ax, 10
	push	ax
	mov	ax, L_myinth_8
	push	ax
	call	print
	add	sp, 4
	mov	sp, bp
	pop	bp
	ret
L_myinth_9:
	push	bp
	mov	bp, sp
	push	cx
	jmp	L_myinth_10
L_myinth_12:
	DB	0xA,"GAME OVER",0xA,0
	ALIGN	2
c_game_over_handler:
	; >>>>> Line:	32
	; >>>>> void c_game_over_handler(void){ 
	jmp	L_myinth_13
L_myinth_14:
	; >>>>> Line:	33
	; >>>>>  
	mov	ax, L_myinth_12
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	34
	; >>>>> exit(0); 
	xor	al, al
	push	ax
	call	exit
	add	sp, 2
	mov	sp, bp
	pop	bp
	ret
L_myinth_13:
	push	bp
	mov	bp, sp
	jmp	L_myinth_14
	ALIGN	2
L_myinth_16:
	DW	0
	ALIGN	2
c_new_piece_handler:
	; >>>>> Line:	37
	; >>>>> void c_new_piece_handler(void){ 
	jmp	L_myinth_17
L_myinth_18:
	; >>>>> Line:	39
	; >>>>> newPieceList[i].id = NewPieceID; 
	mov	ax, word [L_myinth_16]
	mov	cx, 3
	shl	ax, cl
	mov	si, ax
	add	si, newPieceList
	mov	ax, word [NewPieceID]
	mov	word [si], ax
	; >>>>> Line:	40
	; >>>>> newPieceList[i].type = NewPieceType; 
	mov	ax, word [L_myinth_16]
	mov	cx, 3
	shl	ax, cl
	add	ax, newPieceList
	mov	si, ax
	add	si, 2
	mov	ax, word [NewPieceType]
	mov	word [si], ax
	; >>>>> Line:	41
	; >>>>> newPieceList[i].orient = NewPieceOrientation; 
	mov	ax, word [L_myinth_16]
	mov	cx, 3
	shl	ax, cl
	add	ax, newPieceList
	mov	si, ax
	add	si, 4
	mov	ax, word [NewPieceOrientation]
	mov	word [si], ax
	; >>>>> Line:	42
	; >>>>> newPieceList[i].col = NewPieceColumn; 
	mov	ax, word [L_myinth_16]
	mov	cx, 3
	shl	ax, cl
	add	ax, newPieceList
	mov	si, ax
	add	si, 6
	mov	ax, word [NewPieceColumn]
	mov	word [si], ax
	; >>>>> Line:	44
	; >>>>> YKQPost(newPieceQueue, (void *) &(newPieceList[i])); 
	mov	ax, word [L_myinth_16]
	mov	cx, 3
	shl	ax, cl
	add	ax, newPieceList
	push	ax
	push	word [newPieceQueue]
	call	YKQPost
	add	sp, 4
	; >>>>> Line:	45
	; >>>>> i++; 
	inc	word [L_myinth_16]
	; >>>>> Line:	46
	; >>>>> if(i == 50){ 
	cmp	word [L_myinth_16], 50
	jne	L_myinth_19
	; >>>>> Line:	47
	; >>>>> i = 0; 
	mov	word [L_myinth_16], 0
L_myinth_19:
	mov	sp, bp
	pop	bp
	ret
L_myinth_17:
	push	bp
	mov	bp, sp
	jmp	L_myinth_18
	ALIGN	2
c_received_handler:
	; >>>>> Line:	51
	; >>>>> void c_received_handler(void){ 
	jmp	L_myinth_21
L_myinth_22:
	; >>>>> Line:	52
	; >>>>> YKSemPost(movePieceSem); 
	push	word [movePieceSem]
	call	YKSemPost
	add	sp, 2
	mov	sp, bp
	pop	bp
	ret
L_myinth_21:
	push	bp
	mov	bp, sp
	jmp	L_myinth_22
