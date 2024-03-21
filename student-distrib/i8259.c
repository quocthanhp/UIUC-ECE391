/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask = 0xFF; /* IRQs 0-7  */
uint8_t slave_mask  = 0xFF;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {

    uint32_t a1, a2;
    a1 = inb(MASTER_8259_DATA);
    a2 = inb(SLAVE_8259_DATA);

    /* all interrupts masked */
    outb(master_mask, MASTER_8259_DATA);
    outb(slave_mask, SLAVE_8259_DATA);
    /* send icws */
    outb(ICW1, MASTER_8259_CMD);
    outb(ICW1, SLAVE_8259_CMD);
    outb(ICW2_MASTER, MASTER_8259_DATA);
    outb(ICW2_SLAVE, SLAVE_8259_DATA);
    outb(ICW3_MASTER, MASTER_8259_DATA);
    outb(ICW3_SLAVE, SLAVE_8259_DATA);

    outb(ICW4, MASTER_8259_DATA);
    outb(ICW4, SLAVE_8259_DATA);

    outb(a1, MASTER_8259_DATA);
    outb(a2, SLAVE_8259_DATA);

    // enable_irq( 2 ); // not sure what goes here. i think pin 2
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    if( irq_num >= 15){return;}

    if (irq_num >= 8){
        slave_mask &= !(1 << (irq_num + 8));
        outb(slave_mask, SLAVE_8259_DATA);
    }

    else{
        master_mask &= !(1 << irq_num);
        outb(master_mask, MASTER_8259_DATA);
    }
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    if( irq_num >= 15){return;}
    
    if (irq_num >= 8){
        slave_mask |= !(1 << (irq_num + 8));
        outb(slave_mask, SLAVE_8259_DATA);
    }

    else{
        master_mask |= !(1 << irq_num);
        outb(master_mask, MASTER_8259_DATA);
    }
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    if(irq_num >= 8){
		outb(EOI + irq_num, SLAVE_8259_CMD);
        outb(EOI + irq_num, MASTER_8259_CMD);
    }
    else
    outb(EOI + irq_num, MASTER_8259_CMD);
}
