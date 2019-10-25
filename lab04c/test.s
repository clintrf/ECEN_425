push bp
	mov bp, sp
	
	;;push AX
	;;mov BX, [bp+2]	; The return address.
	;pop bp
	;add sp, 2
	
	;; Push flags, CS, and IP to the stack
	pushf

	;; This is something about the flags that Ian told me to do
	;push BX
	;add sp, 2
	;pop BX
	;or BX, 0x200
	;push BX
	;sub sp, 2
	;pop BX
	
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

	;; Move bp back to its original value
	;mov bp, sp
	;add bp, 20
	
	; Store the save sp to si
	mov si, word [bp+4] 			; getting first arg (sp)
	mov word [si], sp

	; Store the save ss to si
	mov si, word [bp+6]			; getting second arg (ss)
	mov word [si], ss

	; Save the sp and ss values to restore to sp and ss
	mov sp, word[bp+8]			; getting third arg
	mov ss, word[bp+10]			; getting fourth arg	

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
	
