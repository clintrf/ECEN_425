; Generated by c86 (BYU-NASM) 5.1 (beta) from lab4d_app.i
	CPU	8086
	ALIGN	2
	jmp	main	; Jump to program start
L_lab4d_app_2:
	DB	"Starting kernel...",0xA,0
L_lab4d_app_1:
	DB	"Creating tasks...",0xA,0
	ALIGN	2
main:
	; >>>>> Line:	26
	; >>>>> { 
	jmp	L_lab4d_app_3
L_lab4d_app_4:
	; >>>>> Line:	27
	; >>>>> YKInitialize(); 
	call	YKInitialize
	; >>>>> Line:	29
	; >>>>> printString("Creating tasks...\n"); 
	mov	ax, L_lab4d_app_1
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	30
	; >>>>> YKNewTask(ATask, (void *) &AStk[256], 3); 
	mov	al, 3
	push	ax
	mov	ax, (AStk+512)
	push	ax
	mov	ax, ATask
	push	ax
	call	YKNewTask
	add	sp, 6
	; >>>>> Line:	31
	; >>>>> YKNewTask(BTask, (void *) &BStk[256], 5); 
	mov	al, 5
	push	ax
	mov	ax, (BStk+512)
	push	ax
	mov	ax, BTask
	push	ax
	call	YKNewTask
	add	sp, 6
	; >>>>> Line:	32
	; >>>>> YKNewTask(CTask, (void *) &CStk[256], 7); 
	mov	al, 7
	push	ax
	mov	ax, (CStk+512)
	push	ax
	mov	ax, CTask
	push	ax
	call	YKNewTask
	add	sp, 6
	; >>>>> Line:	33
	; >>>>> YKNewTask(DTask, (void *) &DStk[256], 8); 
	mov	al, 8
	push	ax
	mov	ax, (DStk+512)
	push	ax
	mov	ax, DTask
	push	ax
	call	YKNewTask
	add	sp, 6
	; >>>>> Line:	35
	; >>>>> printString("Starting kernel...\n"); 
	mov	ax, L_lab4d_app_2
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	36
	; >>>>> YKRun(); 
	call	YKRun
	mov	sp, bp
	pop	bp
	ret
L_lab4d_app_3:
	push	bp
	mov	bp, sp
	jmp	L_lab4d_app_4
L_lab4d_app_7:
	DB	"Task A, delaying 2.",0xA,0
L_lab4d_app_6:
	DB	"Task A started.",0xA,0
	ALIGN	2
ATask:
	; >>>>> Line:	40
	; >>>>> { 
	jmp	L_lab4d_app_8
L_lab4d_app_9:
	; >>>>> Line:	41
	; >>>>> printString("Task A started.\n"); 
	mov	ax, L_lab4d_app_6
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	42
	; >>>>> while (1) 
	jmp	L_lab4d_app_11
L_lab4d_app_10:
	; >>>>> Line:	44
	; >>>>> printString("Task A, delaying 2.\n"); 
	mov	ax, L_lab4d_app_7
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	45
	; >>>>> YKDelayTask(2); 
	mov	ax, 2
	push	ax
	call	YKDelayTask
	add	sp, 2
L_lab4d_app_11:
	jmp	L_lab4d_app_10
L_lab4d_app_12:
	mov	sp, bp
	pop	bp
	ret
L_lab4d_app_8:
	push	bp
	mov	bp, sp
	jmp	L_lab4d_app_9
L_lab4d_app_15:
	DB	"Task B, delaying 3.",0xA,0
L_lab4d_app_14:
	DB	"Task B started.",0xA,0
	ALIGN	2
BTask:
	; >>>>> Line:	50
	; >>>>> { 
	jmp	L_lab4d_app_16
L_lab4d_app_17:
	; >>>>> Line:	51
	; >>>>> printString("Task B started.\n"); 
	mov	ax, L_lab4d_app_14
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	52
	; >>>>> while (1) 
	jmp	L_lab4d_app_19
L_lab4d_app_18:
	; >>>>> Line:	54
	; >>>>> printString("Task B, delaying 3.\n"); 
	mov	ax, L_lab4d_app_15
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	55
	; >>>>> YKDelayTask(3); 
	mov	ax, 3
	push	ax
	call	YKDelayTask
	add	sp, 2
L_lab4d_app_19:
	jmp	L_lab4d_app_18
L_lab4d_app_20:
	mov	sp, bp
	pop	bp
	ret
L_lab4d_app_16:
	push	bp
	mov	bp, sp
	jmp	L_lab4d_app_17
L_lab4d_app_23:
	DB	"Task C, delaying 5.",0xA,0
L_lab4d_app_22:
	DB	"Task C started.",0xA,0
	ALIGN	2
CTask:
	; >>>>> Line:	60
	; >>>>> { 
	jmp	L_lab4d_app_24
L_lab4d_app_25:
	; >>>>> Line:	61
	; >>>>> printString("Task C started.\n"); 
	mov	ax, L_lab4d_app_22
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	62
	; >>>>> while (1) 
	jmp	L_lab4d_app_27
L_lab4d_app_26:
	; >>>>> Line:	64
	; >>>>> printString("Task C, delaying 5.\n"); 
	mov	ax, L_lab4d_app_23
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	65
	; >>>>> YKDelayTask(5); 
	mov	ax, 5
	push	ax
	call	YKDelayTask
	add	sp, 2
L_lab4d_app_27:
	jmp	L_lab4d_app_26
L_lab4d_app_28:
	mov	sp, bp
	pop	bp
	ret
L_lab4d_app_24:
	push	bp
	mov	bp, sp
	jmp	L_lab4d_app_25
L_lab4d_app_31:
	DB	"Task D, delaying 10.",0xA,0
L_lab4d_app_30:
	DB	"Task D started.",0xA,0
	ALIGN	2
DTask:
	; >>>>> Line:	70
	; >>>>> { 
	jmp	L_lab4d_app_32
L_lab4d_app_33:
	; >>>>> Line:	71
	; >>>>> printString("Task D started.\n"); 
	mov	ax, L_lab4d_app_30
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	72
	; >>>>> while (1) 
	jmp	L_lab4d_app_35
L_lab4d_app_34:
	; >>>>> Line:	74
	; >>>>> printString("Task D, delaying 10.\n"); 
	mov	ax, L_lab4d_app_31
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	75
	; >>>>> YKDelayTask(10); 
	mov	ax, 10
	push	ax
	call	YKDelayTask
	add	sp, 2
L_lab4d_app_35:
	jmp	L_lab4d_app_34
L_lab4d_app_36:
	mov	sp, bp
	pop	bp
	ret
L_lab4d_app_32:
	push	bp
	mov	bp, sp
	jmp	L_lab4d_app_33
	ALIGN	2
AStk:
	TIMES	512 db 0
BStk:
	TIMES	512 db 0
CStk:
	TIMES	512 db 0
DStk:
	TIMES	512 db 0
