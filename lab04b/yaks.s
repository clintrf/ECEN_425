;Assemble funtcions

YKEnterMutex:                   ; Disables interrupts 
	cli	                          ; clear interrupt flag
	ret


YKExitMutex:                    ; Enables Interrupts 
	sti	                          ; set interrupt flag 
	ret


;YKDispatcher:   		; Original Dispatcher?

YKDispatcherSave:   		; Dispatcher that saves to mem/stack

YKDispatcherNonSave:		; Dispatcher that  doesn't saves to mem/stack?

