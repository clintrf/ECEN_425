; Generated by c86 (BYU-NASM) 5.1 (beta) from myinth.i
	CPU	8086
	ALIGN	2
	jmp	main	; Jump to program start
	ALIGN	2
c_reset_handler:
	; >>>>> Line:	16
	; >>>>> void c_reset_handler(){ 
	jmp	L_myinth_1
L_myinth_2:
	; >>>>> Line:	17
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
L_myinth_4:
	DW	0
L_myinth_5:
	DW	0
L_myinth_6:
	DB	"  TickISR: queue overflow! ",0xA,0
	ALIGN	2
c_tick_handler:
	; >>>>> Line:	20
	; >>>>> void c_tick_handler(){ 
	jmp	L_myinth_7
L_myinth_8:
	; >>>>> Line:	27
	; >>>>> MsgArray[next].tick = YKTickNum; 
	mov	ax, word [L_myinth_4]
	shl	ax, 1
	shl	ax, 1
	mov	si, ax
	add	si, MsgArray
	mov	ax, word [YKTickNum]
	mov	word [si], ax
	; >>>>> Line:	28
	; >>>>> data = (data + 89) % 100; 
	mov	ax, word [L_myinth_5]
	add	ax, 89
	cwd
	mov	cx, 100
	idiv	cx
	mov	ax, dx
	mov	word [L_myinth_5], ax
	; >>>>> Line:	29
	; >>>>> MsgArray[next].data = data; 
	mov	ax, word [L_myinth_4]
	shl	ax, 1
	shl	ax, 1
	add	ax, MsgArray
	mov	si, ax
	add	si, 2
	mov	ax, word [L_myinth_5]
	mov	word [si], ax
	; >>>>> Line:	31
	; >>>>> if (YKQPost(MsgQPtr, (void *) &(MsgArray[next])) == 0){ 
	mov	ax, word [L_myinth_4]
	shl	ax, 1
	shl	ax, 1
	add	ax, MsgArray
	push	ax
	push	word [MsgQPtr]
	call	YKQPost
	add	sp, 4
	test	ax, ax
	jne	L_myinth_9
	; >>>>> Line:	32
	; >>>>> printString("  TickISR: queue overflow! \n"); 
	mov	ax, L_myinth_6
	push	ax
	call	printString
	add	sp, 2
	jmp	L_myinth_10
L_myinth_9:
	; >>>>> Line:	34
	; >>>>> else if (++next >= 20){ 
	mov	ax, word [L_myinth_4]
	inc	ax
	mov	word [L_myinth_4], ax
	cmp	ax, 20
	jl	L_myinth_11
	; >>>>> Line:	35
	; >>>>> next = 0; 
	mov	word [L_myinth_4], 0
L_myinth_11:
L_myinth_10:
	mov	sp, bp
	pop	bp
	ret
L_myinth_7:
	push	bp
	mov	bp, sp
	jmp	L_myinth_8
	ALIGN	2
c_key_handler:
	; >>>>> Line:	45
	; >>>>> void c_key_handler(){ 
	jmp	L_myinth_13
L_myinth_14:
	; >>>>> Line:	46
	; >>>>> GlobalFlag = 1; 
	mov	word [GlobalFlag], 1
	mov	sp, bp
	pop	bp
	ret
L_myinth_13:
	push	bp
	mov	bp, sp
	jmp	L_myinth_14
