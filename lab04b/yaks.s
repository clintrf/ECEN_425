;Assemble funtcions

YKEnterMutex:                   ; Disables interrupts 
	cli	                          ; clear interrupt flag
	ret


YKExitMutex:                    ; Enables Interrupts 
	sti	                          ; set interrupt flag 
	ret


YKDispatcher:   		; Dispatcher that saves to mem/stack
	cli
	push bp
	mov bp, sp
	
						
	mov ax, word [bp+4]			; getting first arg(the save flag)
	test ax, ax				; if (flag == 0)
	jz	restore				; If zero, skip the save
	
	
store:
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

	mov si, word [bp+6] 			; getting second arg (sp)
	mov word [si], sp
	
	mov si, word [bp+8]			; getting third arg (ss)
	mov word [si], ss

	mov sp, word[bp+10]			; getting forth arg
	mov ss, word[bp+12]			; getting fifth arg	
	
	

restore:
	mov sp, word[bp+10]			; getting forth arg
	mov ss, word[bp+12]			; getting fifth arg	
	
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
	
