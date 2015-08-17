/*
 * testing.c
 *
 *  Created on: Oct 13, 2014
 *      Author: Charlie
 */

#include <stdio.h>
#include <stdlib.h>
#include "cryptosystem.h"
#include "ecc.h"
#include "hadamard.h"

#define iterations 1

unsigned char testReedSolomon(){
	unsigned char message[key_blocks] = {115,53,73,92,88,80,65,94,19,70,119,66,62,61,12,1,91,121,99,94};
	unsigned char codeword[RS_encoded_blocks];
	unsigned char error[iterations][RS_encoded_blocks];
	unsigned char decodeword[iterations][RS_encoded_blocks];
	unsigned char result[iterations][key_blocks];

	int index, iteration;
	unsigned char fail = 0;

	for(iteration=0; iteration<iterations; ++iteration){
		for(index=0; index<RS_encoded_blocks; ++index){
			error[iteration][index]=0;
		}
	}

	error[0][0] = 0xff;
	error[0][1] = 0xff;
	error[0][2] = 0xff;
	error[0][3] = 0xff;
	error[0][4] = 0xff;
	error[0][5] = 0xff;

//------- Reed-Solomon Encoding --------

	// ts = 6 (20 blocks)

	// Initialize the ECC library
	initialize_ecc ();

	// Encode the data
	encode_data(message, key_blocks, codeword);

//------- Error XOR --------

	for (iteration=0; iteration<iterations; ++iteration){
		for (index=0; index<RS_encoded_blocks; ++index){
			decodeword[iteration][index] = codeword[index] ^ error[iteration][index];
		}
	}

//------- Reed-Solomon Decoding --------

	for (iteration=0; iteration<iterations; ++iteration){
		// perform decoding and analysis
		decode_data(decodeword[iteration],RS_encoded_blocks);

		// transfer results over to next variable
		for (index=0; index<RS_encoded_blocks; ++index){
			result[iteration][index] = decodeword[iteration][index];
		}

		// check for errors in computation
		if (check_syndrome () != 0) {
			// correct errors and put results in Hadamard decode
			// 0 and null mean that the code does not check for erasure errors
			correct_errors_erasures (result[iteration], RS_encoded_blocks, 0, 0);
		}
	}

//------- Final Test --------

	for (iteration=0; iteration<iterations; ++iteration){
		for (index=0; index<key_blocks; ++index){
			if (result[iteration][index] != message[index]){
				fail = fail * 10 + iteration + 1;
				break;
			}
		}
	}

	return fail;
}

unsigned char testHadamard(){
	unsigned char message[RS_encoded_blocks]
	                    = {73,125,61,120,5,120,67,67,78,105,38,5,58,38,91,
	                    		  121,91,92,92,25,9,98,41,43,56,81,87,113,103,78,37,83};
	unsigned char codeword[Had_encoded_bytes];
	unsigned char error[iterations][Had_encoded_bytes];
	unsigned char decodeword[iterations][Had_encoded_bytes];
	unsigned char result[iterations][RS_encoded_blocks];

	int index, iteration;
	unsigned char fail = 0;

	for (iteration=0; iteration<iterations; ++iteration){
		for (index=0; index<Had_encoded_bytes; ++index){
			if(index%8==0){
				error[iteration][index] = 0x80;
			}else{
				error[iteration][index] = 0x14;
			}
		}
	}

//------- Hadamard Encoding --------

	// k = 6 (32 blocks of 64 bits)

	// Initialize the Hadamard Code
	initialize_hadamard_code();

	// Encode the data
	hadamard_encode(message, RS_encoded_blocks, codeword);

//------- Error XOR --------

	for (iteration=0; iteration<iterations; ++iteration){
		for (index=0; index<template_size; ++index){
			decodeword[iteration][index] = codeword[index] ^ error[iteration][index];
		}
	}

//------- Hadamard Decoding --------

	for (iteration=0; iteration<iterations; ++iteration){
		// Decode the data
		hadamard_decode(decodeword[iteration], RS_encoded_blocks, result[iteration]);
	}

//------- Final Test --------

	for (iteration=0; iteration<iterations; ++iteration){
		for (index=0; index<RS_encoded_blocks; ++index){
			if (result[iteration][index] != message[index]){
				fail = fail * 10 + iteration + 1;
				break;
			}
		}
	}

	return fail;
}

void printTest(){
	printf("Hello World\n");
}

void countTest(){
	char data = 0x79;

	if(count_ones(data)==5){
		printf("count_ones worked!\n");
	}else{
		printf("count_ones failed!\n");
	}
}
/*
void mult_test(){
	int a, b, c;
	char string[6];
	a = 1;
	b = 1;
	initialize_ecc();
	c = gmult(a, b);
	intToCharArray5(a,string);
	string[5] = 0;
	print(string);
	print(" times ");
	intToCharArray5(b,string);
	string[5] = 0;
	print(string);
	print(" equals: \n");
	intToCharArray5(c,string);
	string[5] = 0;
	print(string);
}
*/

