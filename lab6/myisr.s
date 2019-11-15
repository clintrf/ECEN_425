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
	push ax
	push bx
	push cx
	push dx
	push bp
	push si
	push di
	push ds
	push es

	call YKEnterISR

	sti
	call c_reset_handler
	cli

	mov al, 0x20        ; send eoi to pic
	out 0x20, al        ; send eoi to pic

	call YKExitISR

	pop es
	pop ds
	pop di
	pop si
	pop bp
	pop dx
	pop cx
	pop bx
	pop ax

	iret


KEY:

	push ax
	push bx
	push cx
	push dx
	push bp
	push si
	push di
	push ds
	push es

  mov ax, [YKISRDepth]          ;save the depth counter
	cmp ax, 0                     ; check if the depth is zero, if it is save the SP of the task we are interupting
	jnz key_not_lowest_interrupt  ; skip saving the stack because we are not interupting that task

	mov bx, [YKRdyList] ; save the sp
	mov [bx], sp

key_not_lowest_interrupt:
	call YKEnterISR

	sti                 ; enable interrupts for higher priority IRQs ?
	call c_key_handler  ; run
	cli                 ; disable interrupts ?

	mov al, 0x20        ; send eoi to pic
	out 0x20, al        ; send eoi to pic

	call YKExitISR

	;restore reg
	pop es
	pop ds
	pop di
	pop si
	pop bp
	pop dx
	pop cx
	pop bx
	pop ax

	iret

TICK:
	; save reg
	push ax
	push bx
	push cx
	push dx
	push bp
	push si
	push di
	push ds
	push es

  mov ax, [YKISRDepth] ;save the depth counter
	cmp ax, 0            ; check if the depth is zero, if it is save the SP of the task we are interupting
	jnz not_lowest_interrupt


	mov bx, [YKRdyList] ; save SP of interupted task
	mov [bx], sp

not_lowest_interrupt:
	call YKEnterISR

	sti                 ; enable interrupts for higher priority IRQs ?

	call YKTickHandler;
	call c_tick_handler ; run interrupt handler ? I think this is the c file

	cli                 ; disable interrupts

	mov al, 0x20        ; send eoi to pic
	out 0x20, al        ; send eoi to pic

	call YKExitISR

	;restore
	pop es
	pop ds
	pop di
	pop si
	pop bp
	pop dx
	pop cx
	pop bx
	pop ax

	iret
