#define ASM     1

#define INTERRUPT_HANDLER(name, handler)\
    .global	name            ;\
    name:                   ;\
        pushal              ;\
        pushfl              ;\
        call handler        ;\
        popfl               ;\
        popal               ;\
        iret         

INTERRUPT_HANDLER(divide_by_zero,dividebyzero);
INTERRUPT_HANDLER(Debug_1,Debug);
INTERRUPT_HANDLER(Non_maskable_interrupt,Nonmaskableinterrupt);
INTERRUPT_HANDLER(Breakpoint_1,Breakpoint);
INTERRUPT_HANDLER(Overflow_1,Overflow);
INTERRUPT_HANDLER(Bound_Range_Exceeded,BoundRangeExceeded);
INTERRUPT_HANDLER(Invalid_Opcode,InvalidOpcode);
INTERRUPT_HANDLER(device_not_available,devicenotavailable);
INTERRUPT_HANDLER(Double_Fault,DoubleFault);
INTERRUPT_HANDLER(Coprocessor_Segment_Overrun,CoprocessorSegmentOverrun);
INTERRUPT_HANDLER(Invalid_Task_State_Segment,InvalidTaskStateSegment);
INTERRUPT_HANDLER(Segment_not_present,Segmentnotpresent);
INTERRUPT_HANDLER(Stack_Segment_Fault,StackSegmentFault);
INTERRUPT_HANDLER(General_Protection_Fault,GeneralProtectionFault);
INTERRUPT_HANDLER(Page_Fault,PageFault);
INTERRUPT_HANDLER(reserved_1,reserved);
INTERRUPT_HANDLER(x87_Floating_Point_Exception,x87FloatingPointException);
INTERRUPT_HANDLER(Alignment_Check,AlignmentCheck);
INTERRUPT_HANDLER(Machine_Check,MachineCheck);
INTERRUPT_HANDLER(SIMD_FloatingPoint_Exception,SIMDFloatingPointException);
INTERRUPT_HANDLER(Virtualization_Exception,VirtualizationException);
INTERRUPT_HANDLER(Control_Protection_Exception,ControlProtectionException);

INTERRUPT_HANDLER(keyboard_interrupt_1,keyboard_interrupt);
INTERRUPT_HANDLER(rtc_interrupt_1,rtc_interrupt);

