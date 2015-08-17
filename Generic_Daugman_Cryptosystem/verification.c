/*
 * verification.c
 *
 *  Created on: Oct 12, 2014
 *      Author: Charlie
 */


// Uses Reed-Solomon Library found at http://sourceforge.net/projects/rscode/

#include <stdio.h>
#include <stdlib.h>
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

unsigned char rs_fix[] ={
		150, 205, 37, 252, 87, 219, 51, 47, 247, 112, 78, 232
};


unsigned char run_Verification(unsigned char locked_template[], unsigned int key_hash, unsigned char sample_template[]){

	unsigned char hadamard_to_decode[Had_encoded_bytes];
	unsigned char reed_solomon_to_decode[RS_encoded_blocks];
	unsigned char result[key_blocks];
	unsigned char danger_bit = 0x80;
	unsigned int hash;

	int index;
	unsigned char success = 1;

//------- Sample Template XOR --------

	for (index=0; index<template_size; ++index){
		hadamard_to_decode[index] = locked_template[index] ^ sample_template[index];
	}

//------- Hadamard Decoding --------

	// k = 6 (32 blocks of 64 bits)

	// Initialize the Hadamard Code
	initialize_hadamard_code();

	// Decode the data
	hadamard_decode(hadamard_to_decode, RS_encoded_blocks, reed_solomon_to_decode);

	/*
	 * There could be an issue here where the reed-solomon encoding could put
	 * the first bit of any particular byte as a 1. This would be ignored when
	 * hadamard encoding took place, and therefore all results from hadamard
	 * decoding will have a 0 as the first bit. This could cause issues.
	 *
	 * Currently, I have thought of two ways to fix this. The first way is to
	 * fix these bits between hadamard decoding and reed-solomon decoding by
	 * masking the first bits with the original reed-solomon encoded result.
	 * The second is to change the hadamard encoding to use 8 bit size instead of seven
	 *
	 * -- Added 6/3/2014 by Charles McGuffey
	 */

//------- Fixing the first bit via masking --------

	for(index=0; index<(RS_encoded_blocks - key_blocks); ++index){
		reed_solomon_to_decode[index + key_blocks] |= (rs_fix[index] & danger_bit);
	}

    printf("\nFix Completed:\n");
	printf("Results:\n");
	for(index=0; index<RS_encoded_blocks; ++index){
        printf("%d\t", (int)reed_solomon_to_decode[index]);
	}
	printf("\n\n");
//------- Reed-Solomon Decoding --------

	// ts = 6 (20 blocks)

	// Initialize the ECC library
	initialize_ecc ();

	// perform decoding and analysis
	decode_data(reed_solomon_to_decode,RS_encoded_blocks);

    printf("\nReed-Solomon Decode Completed:\n");
	printf("Results:\n");
	for(index=0; index<RS_encoded_blocks; ++index){
        printf("%d\t", (int)reed_solomon_to_decode[index]);
	}
	printf("\n\n");

	// transfer results over to next variable
	for (index=0; index<RS_encoded_blocks; ++index){
		result[index] = reed_solomon_to_decode[index];
	}

	// check for errors in computation
	if (check_syndrome () != 0) {
		// correct errors and put results in Hadamard decode
			// 0 and null mean that the code does not check for erasure errors
        printf("correcting errors in RS\n");
		correct_errors_erasures (result, RS_encoded_blocks, 0, NULL);
	}

	printf("\nReed-Solomon Correction Completed:\n");
	printf("Results:\n");
	for(index=0; index<RS_encoded_blocks; ++index){
        printf("%d\t", (int)result[index]);
	}
	printf("\n\n");

//------- Hash --------

	hash = hash_char_array(result, key_blocks);

//------- Final Test --------

	if(hash == key_hash){
		success = 1;
	}else{
		success = 0;
	}

	return success;
}

