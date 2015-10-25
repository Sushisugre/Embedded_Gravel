/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Shi Su <shis@andrew.cmu.edu>
 *
 * Date:   The current time & date
 */

#define WORD = 4;
#define LDR_BASE = 0xe59ff000;
#define LDR_PC_MINUS_4 = 0xe51ff004; 
#define LDR_MASK = 0xfffff000;
#define E_BADCODE = 0X0badc0de;

// swi handler in assembly
// get the swi num then transfer the control to c_swi_handler
extern void swi_handler(unsigned swi_num);
// TODO
extern unsigned setup_user();


unsigned *swi_vector= (unsigned *)0x08;

void install_handler(unsigned *old_handler, unsigned *new_handler);
void restore_handler(unsigned *old_handler, unsigned *old_instruction);
unsigned* get_old_handler(unsigned *vector);

int main(int argc, char *argv[]) {

    unsigned *addr_old_hander;
    unsigned old_inst[2];

    addr_old_hander = get_old_handler(swi_vector);
    // store the first 2 instructions of old swi handler
    old_inst[0] = *(addr_old_hander);
    old_inst[1] = *(addr_old_hander + WORD);

    // install new swi handler by modifying old one
    install_handler(addr_old_hander, (unsigned*)&swi_handler);

    // setup for usermode & call user program
    unsigned statu = setup_user();

    // restore native swi handler 
    restore_handler(addr_old_hander, old_inst);
	return statu;
}

unsigned* get_old_handler(unsigned *vector){
    unsigned offset, address;
    offset = (*vector) ^ LDR_BASE;

    // if swi vector doesn't contains ldr pc, [pc,#1mm12]
    if((*vector)& LDR_MASK != LDR_BASE){
        return E_BADCODE;
    }
    
    // calculate the address of jumptable,
    // dereference it to get the address of swi handler
    address = *(swi_vector + 2 * WORD + offset);   

    return (unsigned*)address;
}

void install_handler(unsigned *old_handler, unsigned *new_handler){
    // load next instruction to pc when executing this line
    *old_handler = LDR_PC_MINUS_4;
    //  address of the new swi handler
    *(old_handler + WORD) = (unsigned)new_handler;
}


void restore_handler(unsigned *old_handler, unsigned[] old_inst){
    // put the old swi handler instruction back to whare they were
    *old_handler = old_inst[0];
    *(old_handler + WORD) = old_inst[1];
}

