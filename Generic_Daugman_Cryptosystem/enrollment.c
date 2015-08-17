/*
 * enrollment.c
 *
 *  Created on: Oct 12, 2014
 *      Author: Charlie
 */


// Uses Reed-Solomon Library found at http://sourceforge.net/projects/rscode/

#include "cryptosystem.h"
#include "ecc.h"
#include "hadamard.h"
#include "number_printing.h"
#include "fix.h"
#include "hash.h"

/*
 * Cryptosystem uses 140 bit randomly generated key.
 * Key is broken into 20 blocks of 7 bits each.
 * For this system, each block is stored in a byte.
 * Reed-Solomon encoding provides 32 blocks of 7 bits each.
 * Hadamard encoding converts 32 7 bit blocks into 32 64 bit blocks
 * XORed with template size 256 bytes
 */


void run_Enrollment(unsigned char key[], unsigned char enrollment_template[], unsigned char* locked_template, unsigned int* hash){

	unsigned char reed_solomon_codeword[RS_encoded_blocks];
	unsigned char hadamard_codeword[Had_encoded_bytes];
	unsigned char display[6];

	int index;

//------- Hashing --------

	*hash = hash_char_array(key, key_blocks);

//------- Reed-Solomon Encoding --------

	// ts = 6 (20 blocks)

	// Initialize the ECC library
	initialize_ecc ();

	// Encode the data
	encode_data(key, key_blocks, reed_solomon_codeword);

	printf("\nReed-Solomon Encode Completed:\n");
	printf("Results:\n");
	for(index=0; index<RS_encoded_blocks; ++index){
        printf("%d\t", (int)reed_solomon_codeword[index]);
	}
	printf("\n\n");

//------- Hadamard Encoding --------

	// k = 6 (32 blocks of 64 bits)

	// Initialize the Hadamard Code
	initialize_hadamard_code();

	// Encode the data
	hadamard_encode(reed_solomon_codeword, RS_encoded_blocks, hadamard_codeword);

//------- Forward the data required to fix the 7 bit vs 8 bit Reed-Solomon issue

	for(index=0; index<(RS_encoded_blocks - key_blocks); ++index){
		rs_fix[index] = reed_solomon_codeword[index + key_blocks];
/*		intToCharArray5((int) rs_fix[index], display);
		display[5] = 0;
		print(display);
		print(", ");*/
	}

//------- Enrollment Template XOR --------

	for (index=0; index<template_size; ++index){
		locked_template[index] = hadamard_codeword[index] ^ enrollment_template[index];
	}

}



