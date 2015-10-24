/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Shi Su <shis@andrew.cmu.edu>
 *
 * Date:   The current time & date
 */

#define LDR_MASK = 0xe59ff000;
#define E_BADCODE = 0X0badc0de;

// swi handler in assembly
// get the swi num then transfer the control to c_swi_handler
extern unsigned swi_handler();

unsigned *swi_vector= (unsigned *)0x08;

void install_handler(unsigned* old_handler, unsigned* new_handler);
void restore_handler(unsigned* old_handler, unsigned* old_instruction);
unsigned* get_old_handler(unsigned* vector);

int main(int argc, char *argv[]) {

    unsigned* addr_old_hander;
    unsigned old_inst[2];

    addr_old_hander = get_old_handler(swi_vector);
    // store the first 2 instructions of old swi handler
    old_inst[0] = *(addr_old_hander);
    old_inst[1] = *(addr_old_hander+4);

    // install new swi handler by modifying old one
    install_handler(addr_old_hander, &swi_handler);

    // restore native swi handler 
    restore_handler()
	return -255;
}

unsigned* get_old_handler(unsigned* vector){
    unsigned offset, address;
    offset = (*vector) ^ LDR_MASK;

    // if swi vector doesn't contains ldr pc, [pc,#1mm12]
    if(offset){
        return
    }
    
    // calculate the address of jumptable,
    // dereference it to get the address of swi handler
    address = *(swi_vector + 8 + offset);   

    return (unsigned*)address;
}

restore_handler(unsigned* old_handler, unsigned* old_instruction){

}