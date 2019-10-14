;Assemble funtcions

YKEnterMutex:                   ; Disables interrupts 
	cli	                          ; clear interrupt flag
	ret


YKExitMutex:                    ; Enables Interrupts 
	sti	                          ; set interrupt flag 
	ret


;YKDispatcher:   		; Original Dispatcher?
	
YKDispatcherNSave:		; Dispatcher that  doesn't saves to mem/stack?
	cli
	; Here is where we will deal with our parameters
	push bp
	mov bp, sp

	mov ax, word [bp+4]			; getting the bool
	test ax, ax					; if (ax == 0)

	jz	restoring_context		; If zero, we do NOT store context
	
	storing_context:
	mov AX, [bp+2]	; The return address.
	pop bp
	add sp, 2
	
	pushf
	push CS

;	mov AX, ending_dispatcher
;	push AX
;;	mov AX, [SP+4]	; return address now stored at sp+4
	push AX

	push AX
	push BX
	push CX
	push DX
	push BP						; Maybe not?
	push SI
	push DI
	push DS
	push ES

	mov bp, sp
	add bp, 20

	; Now we just need to store SS and SP in the proper TCB. (these are parameters)
	; 2nd argument, int * save_sp = [bp+6]
	mov si, word [bp+6]
	mov word [si], sp
;	mov word [bp+6], SP
	; 3rd argument, int * save_ss = [bp+8]
	mov si, word [bp+8]
	mov word [si], ss
;	mov word [bp+8], SS


	mov ss, word[bp+12]
	mov sp, word[bp+10]
	jmp real_restoring_context
	
restoring_context:
;	sp is still pointing at the return address.

	; Now we just need to restore SS and SP from the proper TCB. (parameters)
	; 5th argument, int * restore_ss = [bp+12]
	; 4th argument, int * restore_sp = [bp+10]
	mov ss, word[bp+12]
	mov sp, word[bp+10]

real_restoring_context:
	pop ES
	pop DS
	pop DI
	pop SI
	pop BP
	pop DX
	pop CX
	pop BX
	pop AX
	iret			; restores CS, IP, and flags. Starts execution at ENDING_IP
	
YKDispatcherSave:   		; Dispatcher that saves to mem/stack
	cli
	push bp
	mov bp, sp

	mov ax, word [bp+4]			; getting the bool
	test ax, ax					; if (ax == 0)
	
	jz	restoring_context		; If zero, we do NOT store contex
	
	
storing_context:
	mov AX, [bp+2]	; The return address.
	pop bp
	add sp, 2
	
	pushf
	push CS

;	mov AX, ending_dispatcher
;	push AX
;;	mov AX, [SP+4]	; return address now stored at sp+4
	push AX

	push AX
	push BX
	push CX
	push DX
	push BP						; Maybe not?
	push SI
	push DI
	push DS
	push ES

	mov bp, sp
	add bp, 20

	; Now we just need to store SS and SP in the proper TCB. (these are parameters)
	; 2nd argument, int * save_sp = [bp+6]
	mov si, word [bp+6]
	mov word [si], sp
;	mov word [bp+6], SP
	; 3rd argument, int * save_ss = [bp+8]
	mov si, word [bp+8]
	mov word [si], ss
;	mov word [bp+8], SS


	mov ss, word[bp+12]
	mov sp, word[bp+10]
	jmp real_restoring_context
	
restoring_context:
;	sp is still pointing at the return address.

	; Now we just need to restore SS and SP from the proper TCB. (parameters)
	; 5th argument, int * restore_ss = [bp+12]
	; 4th argument, int * restore_sp = [bp+10]
	mov ss, word[bp+12]
	mov sp, word[bp+10]

real_restoring_context:
	pop ES
	pop DS
	pop DI
	pop SI
	pop BP
	pop DX
	pop CX
	pop BX
	pop AX
	iret			; restores CS, IP, and flags. Starts execution at ENDING_IP
	
