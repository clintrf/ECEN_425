; Modify AsmFunction to perform the calculation gvar+(a*(b+c))/(d-e).
; Keep in mind the C declaration:
; int AsmFunction(int a, char b, char c, int d, int e);

;a+4
;b+6
;c+8
;d+10
;e+12

	CPU	8086
	align	2

AsmFunction:
	push bp
	mov bp,sp
	
	push bx 		;save bx
	push dx 		;save dx
	
	mov dx,0		;dx=0
	
	mov al, byte[bp+8] 	;al=c
	cbw			;signEx c
	mov bx, ax		;ax=c
	
	mov al, byte [bp+6]	;al=b
	cbw			;signEx b
	add ax, bx		;b=b+c
	imul word [bp+4]	;b=b*a
	mov bx, word [bp+10]	;bx=d
	sub bx, word [bp+12]	;d=d-e
	idiv bx			;ax=dx:ax/bx
	
	add ax, [gvar]
	pop dx			;unsave
	pop bx			;unsave
	pop bp			;unsave

	;mov	ax, 0
	ret

