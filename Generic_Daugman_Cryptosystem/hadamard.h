/*
 * hadamard.h
 *
 *  Created on: Jun 2, 2014
 *      Author: Charlie
 */

#ifndef HADAMARD_H_
#define HADAMARD_H_

void initialize_hadamard_code();
void hadamard_encode(unsigned char[], int, unsigned char[]);
void hadamard_decode(unsigned char[], int, unsigned char[]);
char count_ones(char);
void print_had_code();


#endif /* HADAMARD_H_ */
