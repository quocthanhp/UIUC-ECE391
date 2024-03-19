#define ASM     1
#include "idt.c"
#include "idt.h"

#define INTERRUPT_HANDLER(name, handler, idt_vector):
    .global	name        ;\
    name:               ;\
        pushal          ;\
        pushfl          ;\
        call handler   ;\
        popfl           ;\
        popal           ;\
        iret         

INTERRUPT_HANDLER(divide_by_zero_link,divide_by_zero,0x00);
INTERRUPT_HANDLER(Debug_link,Debug,0x01);
INTERRUPT_HANDLER(Non_maskable_interrupt_link,Non_maskable_interrupt,0x02);
INTERRUPT_HANDLER(Breakpoint_link,Breakpoint,0x03);
INTERRUPT_HANDLER(Overflow_link,Overflow,0x04);
INTERRUPT_HANDLER(Bound_Range_Exceeded_link,Bound_Range_Exceeded,0x05);
INTERRUPT_HANDLER(Invalid_Opcode_link,Invalid_Opcode,0x06);
INTERRUPT_HANDLER(device_not_available_link,device_not_available,0x07);
INTERRUPT_HANDLER(Double_Fault_link,Double_Fault,0x08);
INTERRUPT_HANDLER(Coprocessor_Segment_Overrun_link,Coprocessor_Segment_Overrun,0x09);
INTERRUPT_HANDLER(Invalid_Task_State_Segment_link,Invalid_Task_State_Segment,0x0A);
INTERRUPT_HANDLER(Segment_not_present_link,Segment_not_present,0x0B);
INTERRUPT_HANDLER(Stack_Segment_Fault_link,Stack_Segment_Fault,0x0C);
INTERRUPT_HANDLER(General_Protection_Fault_link,General_Protection_Fault,0x0D);
INTERRUPT_HANDLER(Page_Fault_link,Page_Fault,0x0E);
INTERRUPT_HANDLER(reserved_link,reserved,0x0F);
INTERRUPT_HANDLER(x87_Floating_Point_Exception_link,x87_Floating_Point_Exception,0x10);
INTERRUPT_HANDLER(Alignment_Check_link,Alignment_Check,0x11);
INTERRUPT_HANDLER(Machine_Check_link,Machine_Check,0x12);
INTERRUPT_HANDLER(SIMD_FloatingPoint_Exception_link,SIMD_FloatingPoint_Exception,0x13);
INTERRUPT_HANDLER(Virtualization_Exception_link,Virtualization_Exception,0x14);
INTERRUPT_HANDLER(Control_Protection_Exception_link,Control_Protection_Exception,0x15);
INTERRUPT_HANDLER(keyboard_interrupt_link,keyboard_interrupt,0x21);
INTERRUPT_HANDLER(rtc_interrupt_link,rtc_interrupt,0x28);

