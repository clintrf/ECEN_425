;Assemble funtcions

YKEnterMutex:                   ; Disables interrupts 
	cli	                          ; clear interrupt flag
	ret


YKExitMutex:                    ; Enables Interrupts 
	sti	                          ; set interrupt flag 
	ret

YKDispatcher:                   ; Standard dispatcher
