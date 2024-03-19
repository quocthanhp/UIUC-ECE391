// #include "idt.h"
// #include "lib.h"
// #include "x86_desc.h"

// void divide_by_zero(){
//     printf("EXCEPTION: DIVIDE BY ZERO\n");
//     while(1);
// }

// void Debug(){
//     printf("EXCEPTION: Debug\n");
//     while(1);
// }

// void Non_maskable_interrupt(){
//     printf("EXCEPTION: non-maskable interrupt\n");
//     while(1);
// }
// void Breakpoint(){
//     printf("EXCEPETION: BREAKPOINT interrupt\n");
//     while(1);
// }

// void Overflow(){
//     printf("EXCEPTION: OVERFLOW interrupt\n");
//     while(1);
// }
// void Bound_Range_Exceeded(){
//     printf("EXCEPTION: Bound Range Exceeded\n");
//     while(1);
// }
// void Invalid_Opcode(){
//     printf("EXCEPTION: Invalid Opcode\n");
//     while(1);
// }

// void device_not_available(){
//     printf("EXCEPTION: device not available\n");
//     while(1);
// }
	
// void Double_Fault(){
//     printf("EXCEPTION: Double Fault\n");
//     while(1);
// }

// /* never called */    
// void Coprocessor_Segment_Overrun(){
//     printf("EXCEPTION: Coprocessor_Segment_Overrun\n");
//     while(1);
// }
// void Invalid_Task_State_Segment(){
//     printf("EXCEPTION: Invalid Task State Segment\n");
//     while(1);
// }

// void Segment_not_present(){
//     printf("EXCEPTION: Segment not present\n");
//     while(1);
// }

// void Stack_Segment_Fault(){
//     printf("EXCEPTION: Stack Segment Fault\n");
//     while(1);
// }
    
// void General_Protection_Fault(){
//     printf("EXCEPTION: General Protection Fault\n");
//     while(1);
// }

// void Page_Fault(){
//     printf("EXCEPTION: Page Fault\n");
//     while(1);
// }
    
// void reserved(){
//     printf("EXCEPTION: reserved\n");
//     while(1);
// }
    
// void x87_Floating_Point_Exception(){
//     printf("EXCEPTION:x87 Floating Point Exception\n");
//     while(1);
// }
    
// void Alignment_Check(){
//     printf("EXCEPTION:Alignment Check\n");
//     while(1);
// }
    
// void Machine_Check(){
//     printf("Machine Check\n");
//     while(1);
// }
    
// void SIMD_FloatingPoint_Exception(){
//     printf("EXCEPTION: SIMD Floating-Point Exception\n");
//     while(1);
// }
    
// void Virtualization_Exception(){
//     printf("EXCEPTION: Virtualization Exception\n");
//     while(1);
// }

// void Control_Protection_Exception(){
//     printf("EXCEPTION: Control Protection Exception\n");
//     while(1);
// }
    
// void idt_init(void){
//     int i;
//     for(i = 0; i < NUM_VEC; i++){
//         idt[i].seg_selector = KERNEL_CS;
//         if(i < 32){
//             idt[i].dpl = 0;
//             idt[i].reserved3 = 1;
//         }else{
//             idt[i].dpl = 3;
//             idt[i].reserved3 = 0;
//         }
//         idt[i].reserved0 = 0;
//         idt[i].reserved1 = 1;
//         idt[i].reserved2 = 1;
//         idt[i].reserved4 = 0;
//         idt[i].present = 1;
//         idt[i].size = 1;


//     }

//     //exception
//     SET_IDT_ENTRY(idt[0x00],divide_by_zero);
//     SET_IDT_ENTRY(idt[0x01],Debug);
//     SET_IDT_ENTRY(idt[0x02],Non_maskable_interrupt);
//     SET_IDT_ENTRY(idt[0x03],Breakpoint);
//     SET_IDT_ENTRY(idt[0x04],Overflow);
//     SET_IDT_ENTRY(idt[0x05],Bound_Range_Exceeded);
//     SET_IDT_ENTRY(idt[0x06],Invalid_Opcode);
//     SET_IDT_ENTRY(idt[0x07],device_not_available);
//     SET_IDT_ENTRY(idt[0x08],Double_Fault);
//     SET_IDT_ENTRY(idt[0x09],Coprocessor_Segment_Overrun);
//     SET_IDT_ENTRY(idt[0x0A],Invalid_Task_State_Segment);
//     SET_IDT_ENTRY(idt[0x0B],Segment_not_present);
//     SET_IDT_ENTRY(idt[0x0C],Stack_Segment_Fault);
//     SET_IDT_ENTRY(idt[0x0D],General_Protection_Fault);
//     SET_IDT_ENTRY(idt[0x0E],Page_Fault);
//     SET_IDT_ENTRY(idt[0x0F],reserved);
//     SET_IDT_ENTRY(idt[0x10],x87_Floating_Point_Exception);
//     SET_IDT_ENTRY(idt[0x11],Alignment_Check);
//     SET_IDT_ENTRY(idt[0x12],Machine_Check);
//     SET_IDT_ENTRY(idt[0x13],SIMD_FloatingPoint_Exception);
//     SET_IDT_ENTRY(idt[0x14],Virtualization_Exception);
//     SET_IDT_ENTRY(idt[0x15],Control_Protection_Exception);

//     //interrupts for the keyboard and the RTC
//     SET_IDT_ENTRY(idt[0x21],keyboard_interrupt);
//     SET_IDT_ENTRY(idt[0x28],rtc_interrupt);
//     //system calls
//     SET_IDT_ENTRY(idt[0x80],syscall);
// }