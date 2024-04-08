#include "idt.h"
#include "lib.h"
#include "x86_desc.h"
#include "./devices/keyboard.h"
// #include "./devices/keyboard.c"
#include "./devices/RTC.h"
 // #include "./devices/RTC.c"
 #include "handler.h"
#include "syscall.h"


void dividebyzero(){
    printf("EXCEPTION: DIVIDE BY ZERO\n");
    halt(255);
}

void Debug(){
    printf("EXCEPTION: Debug\n");
    halt(255);
    
}

void Nonmaskableinterrupt(){
    printf("EXCEPTION: non-maskable interrupt\n");
    halt(255);
    
}
void Breakpoint(){
    printf("EXCEPETION: BREAKPOINT interrupt\n");
    halt(255);
    
}

void Overflow(){
    printf("EXCEPTION: OVERFLOW interrupt\n");
    halt(255);
    
}
void BoundRangeExceeded(){
    printf("EXCEPTION: Bound Range Exceeded\n");
    halt(255);
    
}
void InvalidOpcode(){
    printf("EXCEPTION: Invalid Opcode\n");
    halt(255);
    
}

void devicenotavailable(){
    printf("EXCEPTION: device not available\n");
    halt(255);
    
}
	
void DoubleFault(){
    printf("EXCEPTION: Double Fault\n");
    halt(255);
    
}

/* never called */    
void CoprocessorSegmentOverrun(){
    printf("EXCEPTION: Coprocessor_Segment_Overrun\n");
    halt(255);
    
}
void InvalidTaskStateSegment(){
    printf("EXCEPTION: Invalid Task State Segment\n");
    halt(255);
    
}

void Segmentnotpresent(){
    printf("EXCEPTION: Segment not present\n");
    halt(255);
    
}

void StackSegmentFault(){
    printf("EXCEPTION: Stack Segment Fault\n");
    halt(255);
    
}
    
void GeneralProtectionFault(){
    printf("EXCEPTION: General Protection Fault\n");
    halt(255);
    
}

void PageFault(){
    printf("EXCEPTION: Page Fault\n");
    halt(255);
}
    
void reserved(){
    printf("EXCEPTION: reserved\n");
    halt(255);
    
}
    
void x87FloatingPointException(){
    printf("EXCEPTION:x87 Floating Point Exception\n");
    halt(255);
    
}
    
void AlignmentCheck(){
    printf("EXCEPTION:Alignment Check\n");
    halt(255);
    
}
    
void MachineCheck(){
    printf("Machine Check\n");
    halt(255);
    
}
    
void SIMDFloatingPointException(){
    printf("EXCEPTION: SIMD Floating-Point Exception\n");
    halt(255);
    
}
    
void VirtualizationException(){
    printf("EXCEPTION: Virtualization Exception\n");
    halt(255);
    
}

void ControlProtectionException(){
    printf("EXCEPTION: Control Protection Exception\n");
    halt(255);
    
}


extern void syscall_handler();

/*
idt_init
initializing and mapping for exception handling
-

*/
void idt_init(void){
    int i;                                      
    for(i = 0; i < NUM_VEC; i++){
        idt[i].seg_selector = KERNEL_CS;
        if(i < 32){//check if the vector is the exception 
            idt[i].dpl = 0;
            
        }else{                              
            idt[i].dpl = 3;
        }

        //set signals
        idt[i].reserved0 = 0;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved3 = 0;

        if (i == 0x80) {
            idt[i].reserved3 = 1;
        }
        
        idt[i].reserved4 = 0;
        idt[i].present = 1;
        idt[i].size = 1;
    }

    //exception
    SET_IDT_ENTRY(idt[0x00],dividebyzero);
    SET_IDT_ENTRY(idt[0x01],Debug);
    SET_IDT_ENTRY(idt[0x02],Nonmaskableinterrupt);
    SET_IDT_ENTRY(idt[0x03],Breakpoint);
    SET_IDT_ENTRY(idt[0x04],Overflow);
    SET_IDT_ENTRY(idt[0x05],BoundRangeExceeded);
    SET_IDT_ENTRY(idt[0x06],InvalidOpcode);
    SET_IDT_ENTRY(idt[0x07],devicenotavailable);
    SET_IDT_ENTRY(idt[0x08],DoubleFault);
    SET_IDT_ENTRY(idt[0x09],CoprocessorSegmentOverrun);
    SET_IDT_ENTRY(idt[0x0A],InvalidTaskStateSegment);
    SET_IDT_ENTRY(idt[0x0B],Segmentnotpresent);
    SET_IDT_ENTRY(idt[0x0C],StackSegmentFault);
    SET_IDT_ENTRY(idt[0x0D],GeneralProtectionFault);
    SET_IDT_ENTRY(idt[0x0E],PageFault);
    SET_IDT_ENTRY(idt[0x0F],reserved);
    SET_IDT_ENTRY(idt[0x10],x87FloatingPointException);
    SET_IDT_ENTRY(idt[0x11],AlignmentCheck);
    SET_IDT_ENTRY(idt[0x12],MachineCheck);
    SET_IDT_ENTRY(idt[0x13],SIMDFloatingPointException);
    SET_IDT_ENTRY(idt[0x14],VirtualizationException);
    SET_IDT_ENTRY(idt[0x15],ControlProtectionException);

    //interrupts for the keyboard and the RTC
    SET_IDT_ENTRY(idt[0x21],keyboard_interrupt_1);
    SET_IDT_ENTRY(idt[0x28],rtc_interrupt_1);
    //system calls
    SET_IDT_ENTRY(idt[0x80],syscall_handler);
}
