/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Shi Su <shis@andrew.cmu.edu>
 *
 * Date:   The current time & date
 */

unsigned *swi_vector= (unsigned *)0x08;

unsigned* install_handler(unsigned* vector, unsigned* new_handler);
void restore_handler();

int main(int argc, char *argv[]) {
	// install custom swi handler


    // restore native swi handler 
	return -255;
}
