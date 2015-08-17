/*
 * cryptosystem.h
 *
 *  Created on: May 28, 2014
 *      Author: Charlie
 */

#ifndef CRYPTOSYSTEM_H_
#define CRYPTOSYSTEM_H_

int main(void);
//						key			enrollment template	locked template
void run_Enrollment(unsigned char[] , unsigned char[] , unsigned char* , unsigned int* );
//								locked template		key			sample template
unsigned char run_Verification(unsigned char[] , unsigned int , unsigned char[] );

/*
 * Cryptosystem uses 140 bit randomly generated key.
 * Key is broken into 20 blocks of 7 bits each.
 * For this system, each block is stored in a byte.
 * Reed-Solomon encoding provides 32 blocks of 7 bits each.
 * Hadamard encoding converts 32 7 bit blocks into 32 64 bit blocks
 * XORed with template size 256 bytes
 */
#define key_blocks 20
#define RS_encoded_blocks 32
#define Had_encoded_bytes 256
#define template_size 256

#endif /* CRYPTOSYSTEM_H_ */
