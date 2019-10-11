;Assemble funtcions

YKEnterMutex:                   ; Disables interrupts 
	cli	                          ; clear interrupt flag
	ret


YKExitMutex:                    ; Enables Interrupts 
	sti	                          ; set interrupt flag 
	ret


;YKDispatcher:   		; Original Dispatcher?
	
YKDispatcherNSave:		; Dispatcher that  doesn't saves to mem/stack?
	mov BX, [YKRdyList]
	mov SP, word [BX]

	;restore context
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
	cli
	push bp
	mov bp, sp

	mov ax, word [bp+4]			; getting the bool
	test ax, ax					; if (ax == 0)
	
	jz	restoring_context		; If zero, we do NOT store contex
	
	
restoring_context:
;	sp is still pointing at the return address.

	; Now we just need to restore SS and SP from the proper TCB. (parameters)
	; 5th argument, int * restore_ss = [bp+12]
	; 4th argument, int * restore_sp = [bp+10]
	mov ss, word[bp+12]
	mov sp, word[bp+10]
	
