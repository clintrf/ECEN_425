; ISR file
;1 Save the context of whatever was running by pushing all registers onto the stack, except SP, SS, CS, IP, and the flags.
;2 Enable interrupts to allow higher priority IRQs to interrupt.
;3 Run the interrupt handler, which is usually a C function that does the work for this interrupt.
;4 Disable interrupts.
;5 Send the EOI command to the PIC, informing it that the handler is finished.
;6 Restore the context of what was running before the interrupt occurred by popping the registers saved in step 1 off the stack.
;7 Execute the iret instruction. This restores in one atomic operation the values for IP, CS, and the flags, which were automatically -
;	; - saved on the stack by the processor when the interrupt occurred. This effectively restores execution to the point where the interrupt occurred.

ISR_RESET:

ISR_KEY:

ISR_TICK:
