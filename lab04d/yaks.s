;Assemble funtcions

YKEnterMutex:                   ; Disables interrupts 
	cli	                          ; clear interrupt flag
	ret


YKExitMutex:                    ; Enables Interrupts 
	sti	                          ; set interrupt flag 
	ret

YKDispatcherNSave:   		; Dispatcher that saves to mem/stack
	push bp			; Push the base pointer onto the stack
	mov bp, sp		; set the current base pointer to be the stack pointer

	; Get the sp values to restore and save them to sp
	mov sp, word[bp+4]			; getting first arg (highest priority task sp)
	
	mov	al, 0x20
	out 	0x20, al
	
	; Pop register values off the stack
	pop ES
	pop DS
	pop DI
	pop SI
	pop BP
	pop DX
	pop CX
	pop BX
	pop AX
	iret		

YKDispatcherSave:   		; Dispatcher that saves to mem/stack
	push bp
	mov bp, sp
	
	pop bp
	add sp, 2
	
	; Push flags, CS, and IP to the stack
	pushf

	; This is something about the flags that Ian told me to do
	push BX
	add sp, 2
	pop BX
	or BX, 0x200
	push BX
	sub sp, 2
	pop BX
	
	push CS	
	push word[bp+2]
	
	; Push all register values to the stack
	push AX
	push BX
	push CX
	push DX
	push BP						
	push SI
	push DI
	push DS
	push ES

	; Move bp back to its original value
	mov bp, sp
	add bp, 20
	
	; Store the save sp to si
	mov si, word [bp+4] 			; getting first arg (sp)
	mov word [si], sp


	; Save the sp and ss values to restore to sp and ss
	mov sp, word[bp+6]			; getting third arg
	
	mov	al, 0x20
	out 	0x20, al

	; Pop register values back off the stack
	pop ES
	pop DS
	pop DI
	pop SI
	pop BP
	pop DX
	pop CX
	pop BX
	pop AX
	iret			
	
