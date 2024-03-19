; #include "idt.c"
; #include "idt.h"

; divide_by_zero_:
;     pushal
;     //push flags
;     call divide_by_zero
;     popal
;     iret

; Debug_:
;     pushal
;     //push flags
;     call Debug
;     popal
;     iret
    
; Non_maskable_interrupt_:
;     pushal
;     //push flags
;     call Non_maskable_interrupt
;     popal
;     iret

; Breakpoint_:
;     pushal
;     //push flags
;     call Breakpoint
;     popal
;     iret

; Overflow_:
;     pushal
;     //push flags
;     call Overflow
;     popal
;     iret

; Bound_Range_Exceeded_:
;     pushal
;     //push flags
;     call Bound_Range_Exceeded
;     popal
;     iret

; Invalid_Opcode_:
;     pushal
;     //push flags
;     call Invalid_Opcode
;     popal
;     iret

; device_not_available_:
;     pushal
;     //push flags
;     call device_not_available
;     popal
;     iret
    

; Double_Fault_:
;     pushal
;     //push flags
;     call Double_Fault
;     popal
;     iret

; Coprocessor_Segment_Overrun_:
;     pushal
;     //push flags
;     call Coprocessor_Segment_Overrun
;     popal
;     iret

; Invalid_Task_State_Segment_:
;     pushal
;     //push flags
;     call Invalid_Task_State_Segment
;     popal
;     iret

; Segment_not_present_:
;     pushal
;     //push flags
;     call Segment_not_present
;     popal
;     iret

; General_Protection_Fault_:
;     pushal
;     //push flags
;     call General_Protection_Fault
;     popal
;     iret

; Page_Fault_:
;     pushal
;     //push flags
;     call Page_Fault
;     popal
;     iret

; reserved_:
;     pushal
;     //push flags
;     call reserved
;     popal
;     iret

; x87_Floating_Point_Exception_:
;     pushal
;     //push flags
;     call x87_Floating_Point_Exception
;     popal
;     iret

; Alignment_Check_:
;     pushal
;     //push flags
;     call Alignment_Check
;     popal
;     iret

; Machine_Check_:
;     pushal
;     //push flags
;     call Machine_Check
;     popal
;     iret

; SIMD_FloatingPoint_Exception_:
;     pushal
;     //push flags
;     call SIMD_FloatingPoint_Exception
;     popal
;     iret

; Virtualization_Exception_:
;     pushal
;     //push flags
;     call Virtualization_Exception
;     popal
;     iret

; Control_Protection_Exception_:
;     pushal
;     //push flags
;     call Control_Protection_Exception
;     popal
;     iret


; keyboard_interrupt:
;     cli
;     pushal
;     //push flags
;     call keyboard_interrupt_ //temp name
;     popal
;     sti
;     iret

; rtc_interrupt:
;     cli
;     pushal
;     //push flags
;     call rtc_interrupt_     //temp rtc name
;     popal
;     sti
;     iret