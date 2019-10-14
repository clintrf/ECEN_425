; All kernel routines that are written in assembly are here 

; Disables interrupts 
YKEnterMutex:
	cli	; clear interrupt flag to disable interrupts 
	ret

; Enables Interrupts 
YKExitMutex:
	sti	; set interrupt flag to enable interrupts 
	ret

YKDispatcher:
	; So if we are following andy's design
	; we will save context here (if the boolean
	; passed to us tells us to)
	
	; Hmm. Should the proper SS and SP be passed to us as an argument?
	; Or shall we fetch them ourselves?


	; I am TRYING to move what YKRdyList is pointing at into the stack pointer
	; Because what YKRdyList points at is the stack pointer of the most ready task
	; Incorrect. The value from the dereferenced YKRdyList needs to be 
	; dereferenced again to get the proper stack pointer
	;mov SP, [YKRdyList]
	mov BX, [YKRdyList]
	mov SP, word [BX]

	; but in any case we need to restore context
	pop ES
	pop DS
	pop DI
	pop SI
	pop BP
	pop DX
	pop CX
	pop BX
	pop AX
	; After this- done by reti
	; IP
	; CS
	; flags
	iret

; This dispatcher has a bool parameter for whether it needs to save context
; @ param: int should_save_context
; @ param: int * save_sp_location
; @ param: int * save_ss_location
; @ param: int * restore_sp_location
; @ param: int * restore_ss_location
YKDispatcher_save_context:
cli
	; Here is where we will deal with our parameters
	push bp
	mov bp, sp
; NOTE: WE DO NOT NEED TO SAVE AX. BECAUSE IT IS THE RETURN REGISTER.

;	push ax				; gotta save ax
	mov ax, word [bp+4]			; getting the bool
	test ax, ax					; if (ax == 0)
;	pop ax				; shouldn't mess up flags
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

;;;ending_dispatcher:
	; do all the ending crap of the function
;;;	mov sp, bp
;;;	pop bp
;;;	ret				; Takes us back to the scheduler, and context is restored!

;
; POSSIBLE SOLUITION
; Use assembly macro to
; in-line all of this
; so it doesn't have to 
; be a function call
;
;
; (Start stack)
;	IP of task
;	return address
;	[SP points here]
; 
; (End stack)
;	flags
;	CS
;	IP
;	return address
;	[SP points here]
;
;YKSaveFlagsAndCS:
;	sub SP, 4
;	push DX			; Save DX
;	push CX			; Save CX
;	mov DX, SP+12	; DX = IP (to original task)
;	mov CX, SP+10	; CX = return address (to whomever called me)
;	mov SP+8, DX
;	mov SP+6, CX
;	pop CX			; Restore CX
;	pop DX			; Restore DX
;
;	add SP, 8
;	pushf			; push flags into their proper spot
;	push CS			; push CS into its proper spot
;	sub SP, 4
;	
;	jmp YKSaveContext	; Now we are ready to save the rest of context

; This saves the context on the current stack frame 
; We are assuming this is called from an interrupt
;	(if it is not, call YKSaveFlagsAndCS instead)
; So we already have flags, CS, and IP saved properly
;YKSaveContext:
;	;SP + 2 (I think) is return address.
;	sub SP, 18	; Get ready to save 9 words onto the stack
;	push DX
;	mov DX, SP + 22	; mov the return address onto SP 
;	mov SP+4, DX	; put it at the end of the stack, ready to be popped
;	pop DX
;	
;	mov [SP+20], AX
;	mov SP+18, BX
;	mov SP+16, CX
;	mov SP+14, DX
;	mov SP+12, BP
;	mov SP+10, SI
;	mov SP+8, DI
;	mov SP+6, DS
;	mov SP+4, ES
;;	; Current stack: return address, empty 
;;	push AX	; Each 
;	push BX	; one  
;	push CX	; is  
;	push DX	; sp-2 
;	; Instruction Pointer - In interrupt, saved already
;	; Stack pointer - Need to be saved seperately
;	push BP ; Base pointer 
;	push SI ; Source index 
;	push DI ; Destination index    
;	; Code segment - Saved with IP?
;	; Stack segment - saved with SP?
;	push DS ; Data segment 
;	push ES ; Extra segment 
;	; Flags - interrupts already save it
;	ret

; This restores the context FROM the current stack frame 
;YKRestoreContext:
;	mov AX, SP+20
;	mov BX, SP+18
;	mov CX, SP+16
;	mov DX, SP+14
;	mov BP, SP+12
;	mov SI, SP+10
;	mov DI, SP+8
;	mov DS, SP+6
;	mov ES, SP+4
;
;	push DX
;	mov DX, SP+4
;	mov SP+22 , DX
;	pop DX
;	add SP, 18
;	ret

;	add SP, 2	; This deletes the return address saved when we call'ed YKRestoreContext.
;	pop ES
;	pop DS
;	SS
;	CS
;	pop DI
;	pop SI
;	pop BP
;	SP
;	IP

;	pop DX
;	pop CX
;	pop BX
;	pop AX
;	reti		; This pops IP, CS, and the flags, and goes to the return address.

