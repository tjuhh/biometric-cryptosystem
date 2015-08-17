/*
 * hash.c
 *
 *  Created on: Oct 12, 2014
 *      Author: Charlie
 */

/*
 * Hashes an array of characters by adding them up
 */

 #define MODULUS 37


unsigned int hash_char_array(unsigned char data[], int size){

	int hash=0;
	int index=0;
	int temp;

	for(index=0; index<size; ++index){
		temp = ((int) data[index]) % MODULUS;
		hash += temp;
	}

	return hash;
}
