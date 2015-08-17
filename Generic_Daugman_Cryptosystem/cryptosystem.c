/*
 * cryptosystem.c
 *
 *  Created on: May 23, 2014
 *  Author: Charles McGuffey
 *  Attempt to reproduce Daugman's Cryptosystem algorithm
 */

// Uses Reed-Solomon Library found at http://sourceforge.net/projects/rscode/

#include <stdlib.h>
#include <stdio.h>
#include "cryptosystem.h"
#include "number_printing.h"
#include "testing.h"
#include "ecc.h"
#include "hadamard.h"
#include "data.h"


/*
 * Cryptosystem uses 140 bit randomly generated key.
 * Key is broken into 20 blocks of 7 bits each.
 * For this system, each block is stored in a byte.
 * Reed-Solomon encoding provides 32 blocks of 7 bits each.
 * Hadamard encoding converts 32 7 bit blocks into 32 64 bit blocks
 * XORed with template size 256 bytes
 */

/*****************************************************************************/
/**
*
* Main function to call the appropriate code.
*
* @param	None
*
* @return
*		- XST_FAILURE if the Test Failed .
*		- A non-negative number indicating the number of characters
*		  sent.
*
* @note		None
*
******************************************************************************/
int main(void)
{
	int status=0;
	unsigned char result = 0;
	unsigned char enroll = 1;
	unsigned char verify = 1;

	unsigned int index;

	printf("\n");

	if(enroll==1){
		run_Enrollment(key, enrollment_template, locked_template, &key_hashed);
/*		for(index=0; index<template_size; ++index){
			printf("%d\t",(int)locked_template[index]);
			if(index%16==15){
				printf("\n");
			}
		}*/
		printf("Enrollment Completed\n");
		printf("Hashed key value is: %d\n",key_hashed);
	}
	if(verify==1){
		result = run_Verification(locked_template, key_hashed, sample_template);

		if(result==1){
			printf("Verification was successful\n");
		}else{
			printf("Verification failed\n");
		}
	}

	return status;
}

