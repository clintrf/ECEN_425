;Assemble funtcions

YKEnterMutex:                   ; Disables interrupts 
	cli	                          ; clear interrupt flag
	ret


YKExitMutex:                    ; Enables Interrupts 
	sti	                          ; set interrupt flag 
	ret

YKDispatcherNSave:   		; Dispatcher that saves to mem/stack
	cli
	push bp
	mov bp, sp

	mov sp, word[bp+4]			; getting first arg
	mov ss, word[bp+6]			; getting second arg	
	
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
	call print_debug
	cli
	push bp
	mov bp, sp
	
	mov AX, [bp+2]	; The return address.
	pop bp
	add sp, 2
	
	pushf
	push CS	
	push AX

	push AX
	push BX
	push CX
	push DX
	push BP						
	push SI
	push DI
	push DS
	push ES

	mov bp, sp
	add bp, 20

	mov si, word [bp+4] 			; getting first arg (sp)
	mov word [si], sp
	
	mov si, word [bp+6]			; getting second arg (ss)
	mov word [si], ss

	mov sp, word[bp+8]			; getting third arg
	mov ss, word[bp+10]			; getting forth arg	
	
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
	
