#include "idt.h"
#include "lib.h"
#include "x86_desc.h"
#include "./devices/keyboard.h"
// #include "./devices/keyboard.c"
#include "./devices/RTC.h"
 // #include "./devices/RTC.c"
 #include "handler.h"




// void exc_handler(uint32_t interrupt_vector){
//     switch(interrupt_vector){
//         case x0: 
//             printf("EXCEPTION: DIVIDE BY ZERO\n");
//             while(1);
//             break;
//         case x1:
//             printf("EXCEPTION: Debug\n");
//             while(1);
//             break;
//         case x2:
//             printf("EXCEPTION: non-maskable interrupt\n");
//             while(1);
//             break;
//         case x3:
//             printf("EXCEPETION: BREAKPOINT interrupt\n");
//             while(1);
//             break;
//         case x4:
//             printf("EXCEPTION: OVERFLOW interrupt\n");
//             while(1);
//             break;
//         case x5:
//             printf("EXCEPTION: Bound Range Exceeded\n");
//             while(1);
//             break;
//         case x6:
//             printf("EXCEPTION: Invalid Opcode\n");
//             while(1);
//             break;
//         case x7:
//             printf("EXCEPTION: device not available\n");
//             while(1); 
//             break;
//         case x8:
//             printf("EXCEPTION: Double Fault\n");
//             while(1);
//             break;
//         /* never called */    
//         case x9:
//             printf("EXCEPTION: Coprocessor_Segment_Overrun\n");
//             while(1);
//             break;
//         case xA:
//             printf("EXCEPTION: Invalid Task State Segment\n");
//             while(1);
//             break;
//         case xB:
//             printf("EXCEPTION: Segment not present\n");
//             while(1);
//             break;
//         case xC:
//             printf("EXCEPTION: Stack Segment Fault\n");
//             while(1);  
//             break;
//         case xD:
//             printf("EXCEPTION: General Protection Fault\n");
//             while(1);
//             break;
//         case xE:
//             printf("EXCEPTION: Page Fault\n");
//             while(1);  
//             break;
//         case xF:
//             printf("EXCEPTION: reserved\n");
//             while(1); 
//             break;
//         case x10:
//             printf("EXCEPTION:x87 Floating Point Exception\n");
//             while(1); 
//             break;
//         case x11:
//             printf("EXCEPTION:Alignment Check\n");
//             while(1);
//             break;
//         case x12:
//             printf("Machine Check\n");
//             while(1);
//             break;
//         case x13:
//             printf("EXCEPTION: SIMD Floating-Point Exception\n");
//             while(1);
//             break;
//         case x14:
//             printf("EXCEPTION: Virtualization Exception\n");
//             while(1);
//             break;
//         case x15:
//             printf("EXCEPTION: Control Protection Exception\n");
//             while(1);
//             break;
//         default:
//             break; 
//     }
//     return;
// }

void dividebyzero(){
    printf("EXCEPTION: DIVIDE BY ZERO\n");
    while(1);
}

void Debug(){
    printf("EXCEPTION: Debug\n");
    while(1);
}

void Nonmaskableinterrupt(){
    printf("EXCEPTION: non-maskable interrupt\n");
    while(1);
}
void Breakpoint(){
    printf("EXCEPETION: BREAKPOINT interrupt\n");
    while(1);
}

void Overflow(){
    printf("EXCEPTION: OVERFLOW interrupt\n");
    while(1);
}
void BoundRangeExceeded(){
    printf("EXCEPTION: Bound Range Exceeded\n");
    while(1);
}
void InvalidOpcode(){
    printf("EXCEPTION: Invalid Opcode\n");
    while(1);
}

void devicenotavailable(){
    printf("EXCEPTION: device not available\n");
    while(1);
}
	
void DoubleFault(){
    printf("EXCEPTION: Double Fault\n");
    while(1);
}

/* never called */    
void CoprocessorSegmentOverrun(){
    printf("EXCEPTION: Coprocessor_Segment_Overrun\n");
    while(1);
}
void InvalidTaskStateSegment(){
    printf("EXCEPTION: Invalid Task State Segment\n");
    while(1);
}

void Segmentnotpresent(){
    printf("EXCEPTION: Segment not present\n");
    while(1);
}

void StackSegmentFault(){
    printf("EXCEPTION: Stack Segment Fault\n");
    while(1);
}
    
void GeneralProtectionFault(){
    printf("EXCEPTION: General Protection Fault\n");
    while(1);
}

void PageFault(){
    printf("EXCEPTION: Page Fault\n");
    while(1);
}
    
void reserved(){
    printf("EXCEPTION: reserved\n");
    while(1);
}
    
void x87FloatingPointException(){
    printf("EXCEPTION:x87 Floating Point Exception\n");
    while(1);
}
    
void AlignmentCheck(){
    printf("EXCEPTION:Alignment Check\n");
    while(1);
}
    
void MachineCheck(){
    printf("Machine Check\n");
    while(1);
}
    
void SIMDFloatingPointException(){
    printf("EXCEPTION: SIMD Floating-Point Exception\n");
    while(1);
}
    
void VirtualizationException(){
    printf("EXCEPTION: Virtualization Exception\n");
    while(1);
}

void ControlProtectionException(){
    printf("EXCEPTION: Control Protection Exception\n");
    while(1);
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
