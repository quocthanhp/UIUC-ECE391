#ifndef _ASM_H
#define _ASM_H

void divide_by_zero_link();
void Debug_link();
void Non_maskable_interrupt_link();
void Breakpoint_link();
void Overflow_link();
void Bound_Range_Exceeded_link();
void Invalid_Opcode_link();
void device_not_available_link();
void Double_Fault_link();
void Coprocessor_Segment_Overrun_link();
void Invalid_Task_State_Segment_link();
void Segment_not_present_link();
void Stack_Segment_Fault_link();
void General_Protection_Fault_link();
void Page_Fault_link();
void reserved_link();
void x87_Floating_Point_Exception_link();
void Alignment_Check_link();
void Machine_Check_link();
void SIMD_FloatingPoint_Exception_link();
void Virtualization_Exception_link();
void Control_Protection_Exception_link();
void keyboard_interrupt_link();
void rtc_interrupt_link();

#endif