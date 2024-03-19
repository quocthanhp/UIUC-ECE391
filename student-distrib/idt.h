#ifndef IDT_H
#define IDT_H

extern void idt_init(void);
extern void divide_by_zero(void);

extern void Debug(void);

extern void Non_maskable_interrupt(void);
extern void Breakpoint(void);

extern void Overflow(void);
extern void Bound_Range_Exceeded(void);
extern void Invalid_Opcode(void);

extern void Coprocessor_not_available(void);
	
extern void Double_Fault(void);
    
extern void Coprocessor_Segment_Overrun(void);
extern void Invalid_Task_State_Segment(void);

extern void Segment_not_present(void);

extern void Stack_Segment_Fault(void);
    
extern void General_Protection_Fault(void);

extern void Page_Fault(void);
    
extern void reserved(void);
    
extern void x87_Floating_Point_Exception(void);
    
extern void Alignment_Check(void);
    
extern void Machine_Check(void);
    
extern void SIMD_FloatingPoint_Exception(void);
    
extern void Virtualization_Exception(void);

extern void Control_Protection_Exception(void);
#endif
