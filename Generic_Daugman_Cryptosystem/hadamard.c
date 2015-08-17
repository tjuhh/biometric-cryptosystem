/*
 * hadamard.c
 *
 *  Created on: May 28, 2014
 *  Author: Charles McGuffey
 *  Attempt to create a program that can handle Hadamard Matrices and
 *  code for use in the reproduction of Daugman's Biometric Cryptosystem
 *  Algorithm
 *
 *  This program uses Sylvester's method of creating Hadamard Matrices
 *  This means that only matrices of size n = 2 ^ k for some natural
 *  number k are supported. For this particular program, only matrices of
 *  size 8 or greater are supported. In order to change n, change HAD_SIZE
 *  at compile-time
 */

#include "hadamard.h"
#include "number_printing.h"

#define HAD_SIZE 64
#define ROWS (2 * HAD_SIZE)
#define COLUMNS (HAD_SIZE / 8)

unsigned char had_code[ROWS][COLUMNS];
unsigned char masks[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
unsigned char lookuparray[] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3,
		3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3,
		2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4,
		4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4,
		4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6,
		5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3,
		3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5,
		3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6,
		6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5,
		4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5,
		5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8 };

void initialize_hadamard_code(){
	int bits, bytes, row, col;
	had_code[0][0] = 0xFF;
	had_code[1][0] = 0xAA;
	had_code[2][0] = 0xCC;
	had_code[3][0] = 0x99;
	had_code[4][0] = 0xF0;
	had_code[5][0] = 0xA5;
	had_code[6][0] = 0xC3;
	had_code[7][0] = 0x96;

	/*
	print("\ninitial 8\n");
	print_had_code();*/

	for(bits=8; bits<HAD_SIZE; bits*=2){		//Perform recursive hadamard fill-in
		bytes = bits / 8;
		// Make upper right quadrant look like uppper left quadrant
		for(row=0; row<bits; ++row){
			for(col=0; col<bytes; ++col){
				had_code[row][col+bytes] = had_code[row][col];
			}
		}

		// Make lower left quadrant look like uppper left quadrant
		for(row=0; row<bits; ++row){
			for(col=0; col<bytes; ++col){
				had_code[row+bits][col] = had_code[row][col];
			}
		}

		// Make lower right quadrant look like inverse of uppper left quadrant
		for(row=0; row<bits; ++row){
			for(col=0; col<bytes; ++col){
				had_code[row+bits][col+bytes] = had_code[row][col] ^ 0xFF;
			}
		}
		/*print("\niteration end\n");
		print_had_code();*/
	}

	/*print("\npre-inverse\n");
	print_had_code();*/

	// Create inverse of hadamard matrix below original to create hadamard code
	for(row=0; row<HAD_SIZE; ++row){
		for(col=0; col<COLUMNS; ++col){
			had_code[row+HAD_SIZE][col] = had_code[row][col] ^ 0xFF;
		}
	}
}


void hadamard_encode(unsigned char msg[], int num_blocks, unsigned char destination[]){
	int block_num, msg_block, base, col;

	for(block_num=0; block_num<num_blocks; ++block_num){
		msg_block = msg[block_num] % ROWS;
		base = block_num * COLUMNS;
		for(col=0; col<COLUMNS; ++col){
			destination[base+col] = had_code[msg_block][col];
		}
	}
}

void hadamard_decode(unsigned char codeword[], int num_blocks, unsigned char destination[]){
	unsigned char best_index, best_ones, row, col, current_ones, temp;
	int block, base_index;

	for(block=0; block<num_blocks; ++block){
		base_index = COLUMNS * block;
		best_index = -1;
		best_ones = HAD_SIZE;
		for(row=0; row<ROWS; ++row){
			current_ones = 0;
			for(col=0; col<COLUMNS; ++col){
				temp = codeword[base_index + col] ^ had_code[row][col];
				current_ones += count_ones(temp);
			}
			if (current_ones < best_ones){
				best_index = row;
				best_ones = current_ones;
				if (best_ones < 16) {
					break;
				}
			}
		}
		destination[block] = best_index;
	}
}
/*
char count_ones(char data){
	char i, temp, ones;
	temp = data;
	ones = 0;
	for(i=0; i<8; ++i){
		if(temp %2 > 0){
			++ones;
		}
		temp = temp >> 1;
	}
	return ones;
}
*/
char count_ones(char data){
	unsigned char temp;
	temp = (unsigned char) data;
	temp = lookuparray[temp];
	return temp;
}

void print_had_code(){
	int r, c;
	char print_code[5];

	printf("\nHadamard Code\n");

	for(r=0; r<ROWS; ++r){
		for(c=0; c<COLUMNS; ++c){
			intToCharArray5((int) had_code[r][c], print_code);
			printf(print_code);
			printf("  ");
		}
		printf("\n");
	}
}
