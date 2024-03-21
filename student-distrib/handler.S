#define ASM     1
#include "idt.c"
#include "idt.h"

#define INTERRUPT_HANDLER(name, handler, idt_vector):
    .global	name            ;\
    name:                   ;\
        pushal              ;\
        pushfl              ;\
        pushl   $idt_vector ;\
        call    handler        ;\
        addl	$4, %esp 
        popfl               ;\
        popal               ;\
        iret         

INTERRUPT_HANDLER(divide_by_zero_,exc_handler,0x00);
INTERRUPT_HANDLER(Debug_,exc_handler,0x01);
INTERRUPT_HANDLER(Non_maskable_interrupt_,exc_handler,0x02);
INTERRUPT_HANDLER(Breakpoint_,exc_handler,0x03);
INTERRUPT_HANDLER(Overflow_,exc_handler,0x04);
INTERRUPT_HANDLER(Bound_Range_Exceeded_,exc_handler,0x05);
INTERRUPT_HANDLER(Invalid_Opcode_,exc_handler,0x06);
INTERRUPT_HANDLER(device_not_available_,exc_handler,0x07);
INTERRUPT_HANDLER(Double_Fault_,exc_handler,0x08);
INTERRUPT_HANDLER(Coprocessor_Segment_Overrun_,exc_handler,0x09);
INTERRUPT_HANDLER(Invalid_Task_State_Segment_,exc_handler,0x0A);
INTERRUPT_HANDLER(Segment_not_present_,exc_handler,0x0B);
INTERRUPT_HANDLER(Stack_Segment_Fault_,exc_handler,0x0C);
INTERRUPT_HANDLER(General_Protection_Fault_,exc_handler,0x0D);
INTERRUPT_HANDLER(Page_Fault_,exc_handler,0x0E);
INTERRUPT_HANDLER(reserved_,exc_handler,0x0F);
INTERRUPT_HANDLER(x87_Floating_Point_Exception_,exc_handler,0x10);
INTERRUPT_HANDLER(Alignment_Check_,exc_handler,0x11);
INTERRUPT_HANDLER(Machine_Check_,exc_handler,0x12);
INTERRUPT_HANDLER(SIMD_FloatingPoint_Exception_,exc_handler,0x13);
INTERRUPT_HANDLER(Virtualization_Exception_,exc_handler,0x14);
INTERRUPT_HANDLER(Control_Protection_Exception_,exc_handler,0x15);
INTERRUPT_HANDLER(keyboard_interrupt_,exc_handler,0x21);
INTERRUPT_HANDLER(rtc_interrupt_,exc_handler,0x28);

