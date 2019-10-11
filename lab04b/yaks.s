;Assemble funtcions

YKEnterMutex:                   ; Disables interrupts 
	cli	                          ; clear interrupt flag
	ret


YKExitMutex:                    ; Enables Interrupts 
	sti	                          ; set interrupt flag 
	ret


;YKDispatcher:   		; Original Dispatcher?

YKDispatcherSave:   		; Dispatcher that saves to mem/stack
	YKEnterMutex:		; Disable interrupts			
	
	push	bp
	mov	bp,sp		; move to stack to save to
	
	pop	
	
	push	bp
	mov	bp,sp		; mov to the new stack 
	
	push 			; 
	
	

YKDispatcherNonSave:		; Dispatcher that  doesn't saves to mem/stack?

