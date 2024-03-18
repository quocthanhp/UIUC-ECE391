#include "idt.h"
#include "lib.h"
#include "x86_desc.h"

void divide_by_zero(){
    printf("EXCEPTION: DIVIDE BY ZERO\n");
}

void Debug(){
    printf("EXCEPTION: Debug\n");
}

void Non_maskable_interrupt(){
    printf("EXCEPTION: non-maskable interrupt\n");
}
void Breakpoint(){
    printf("EXCEPETION: BREAKPOINT interrupt\n");
}

void Overflow(){
    printf("EXCEPTION: OVERFLOW interrupt\n");
}
void Bound_Range_Exceeded(){
    printf("EXCEPTION: Bound Range Exceeded\n");
}
void Invalid_Opcode(){
    printf("EXCEPTION: Invalid Opcode\n");
}

void Coprocessor_not_available(){
    printf("EXCEPTION: Coprocessor not available\n");
}
	
void Double_Fault(){
    printf("EXCEPTION: Double Fault\n");
}

/* never called */    
void Coprocessor_Segment_Overrun(){
    printf("EXCEPTION: Double Fault\n");
}
void Invalid_Task_State_Segment(){
    printf("EXCEPTION: Invalid Task State Segment\n");
}

void Segment_not_present(){
    printf("EXCEPTION: Segment not present\n");
}

void Stack_Segment_Fault(){
    printf("EXCEPTION: Stack Segment Fault\n");
}
    
void General_Protection_Fault(){
    printf("EXCEPTION: General Protection Fault\n");
}

void Page_Fault(){
    printf("EXCEPTION: Page Fault\n");
}
    
void reserved(){
    printf("EXCEPTION: reserved\n");
}
    
void x87_Floating_Point_Exception(){
    printf("EXCEPTION:x87 Floating Point Exception\n");
}
    
void Alignment_Check(){
    printf("EXCEPTION:Alignment Check\n");
}
    
void Machine_Check(){
    printf("Machine Check\n");
}
    
void SIMD_FloatingPoint_Exception(){
    printf("EXCEPTION: SIMD Floating-Point Exception\n");
}
    
void Virtualization_Exception(){
    printf("EXCEPTION: Virtualization Exception\n");
}

void Control_Protection_Exception(){
    printf("EXCEPTION: Control Protection Exception\n");
}
    
void idt_init(void){
    int i;
    for(i = 0; i < NUM_VEC; i++){
        idt[i].seg_selector = KERNEL_CS;
        if(i < 32){
            idt[i].dpl = 0;
            idt[i].reserved3 = 1;
        }else{
            idt[i].dpl = 3;
            idt[i].reserved3 = 0;
        }
        idt[i].reserved0 = 0;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved4 = 0;
        idt[i].present = 1;
        idt[i].size = 1;


    }
    SET_IDT_ENTRY(idt[0x00],divide_by_zero);
    SET_IDT_ENTRY(idt[0x01],Debug);
    SET_IDT_ENTRY(idt[0x02],Non_maskable_interrupt);
    SET_IDT_ENTRY(idt[0x03],Breakpoint);
    SET_IDT_ENTRY(idt[0x04],Overflow);
    SET_IDT_ENTRY(idt[0x05],Bound_Range_Exceeded);
    SET_IDT_ENTRY(idt[0x06],Invalid_Opcode);
    SET_IDT_ENTRY(idt[0x07],Coprocessor_not_available);
    SET_IDT_ENTRY(idt[0x08],Double_Fault);
    SET_IDT_ENTRY(idt[0x09],Coprocessor_Segment_Overrun);
    SET_IDT_ENTRY(idt[0x0A],Invalid_Task_State_Segment);
    SET_IDT_ENTRY(idt[0x0B],Segment_not_present);
    SET_IDT_ENTRY(idt[0x0C],Stack_Segment_Fault);
    SET_IDT_ENTRY(idt[0x0D],General_Protection_Fault);
    SET_IDT_ENTRY(idt[0x0E],Page_Fault);
    SET_IDT_ENTRY(idt[0x0F],reserved);
    SET_IDT_ENTRY(idt[0x10],x87_Floating_Point_Exception);
    SET_IDT_ENTRY(idt[0x11],Alignment_Check);
    SET_IDT_ENTRY(idt[0x12],Machine_Check);
    SET_IDT_ENTRY(idt[0x13],SIMD_FloatingPoint_Exception);
    SET_IDT_ENTRY(idt[0x14],Virtualization_Exception);
    SET_IDT_ENTRY(idt[0x15],Control_Protection_Exception);

}