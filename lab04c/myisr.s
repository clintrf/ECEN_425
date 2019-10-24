; ISR file
;1 Save the context of whatever was running by pushing all registers onto the stack, except SP, SS, CS, IP, and the flags.
;2 Enable interrupts to allow higher priority IRQs to interrupt.
;3 Run the interrupt handler, which is usually a C function that does the work for this interrupt.
;4 Disable interrupts.
;5 Send the EOI command to the PIC, informing it that the handler is finished.
;6 Restore the context of what was running before the interrupt occurred by popping the registers saved in step 1 off the stack.
;7 Execute the iret instruction. This restores in one atomic operation the values for IP, CS, and the flags, which were automatically -
;	; - saved on the stack by the processor when the interrupt occurred. This effectively restores execution to the point where the interrupt occurred.


RESET:
	call YKEnterISR
	call c_reset_handler
	pop word[bp+20]
	call YKExitISR ; TODO: not sure if this should be called before or after the next instruction

KEY:

	; save reg
	push ax
	push bx
	push cx
	push dx
	
	push si
	push di
	push bp
	push es 
	push ds
	
	; Inform OS that ISR has begun execution
	call YKEnterISR

	; enable interrupts for higher priority IRQs ? 
	sti
	
	; run inte
	call c_key_handler
	
	; disable interrupts ?
	cli
	
	; send eoi to pic
	mov al, 0x20
	out 0x20, al
	
	; Inform OS that ISR has finished execution
	
	
	call YKExitISR

	;restore reg
	pop ds
	pop es
	pop bp
	pop di
	pop si
	pop dx
	pop cx
	pop bx
	pop ax
	
	sti    ; I think we have to enable interuts before we exit
	; execute iret
	iret 

TICK:
	; save reg
	push ax
	push bx
	push cx
	push dx
	
	push si
	push di
	push bp
	push es 
	push ds
	
	; Inform OS that ISR has started execution
	call YKEnterISR

	; enable interrupts for higher priority IRQs ? 
	sti
	
	; run interrupt handler ? I think this is the c file
	call c_tick_handler
	
	; disable interrupts ?
	cli
	
	; send eoi to pic
	mov al, 0x20
	out 0x20, al
	
	; Inform OS that ISR has finished execution
	call YKExitISR

	;restore reg
	pop ds
	pop es
	pop bp
	pop di	
	pop si
	pop dx
	pop cx
	pop bx
	pop ax
	
	; execute iret
	iret 
