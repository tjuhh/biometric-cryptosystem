/*
 * data.h
 *
 *  Created on: Jan 23, 2015
 *      Author: Charlie
 */

#ifndef DATA_H_
#define DATA_H_

#include "cryptosystem.h"

// 140 bit random key broken into 20 blocks of 7 bits where each is stored in a byte
unsigned char key[key_blocks];
// 256 byte enrollment template
unsigned char enrollment_template[template_size];
// 256 byte sample template
unsigned char sample_template[template_size];
// 256 byte locked template
unsigned char locked_template[template_size];

unsigned int key_hashed;


#endif /* DATA_H_ */
