/*
 * number_printing.c
 *
 *  Created on: May 28, 2014
 *      Author: Charlie
 */


unsigned char numbers[] = "0123456789";

void intToCharArray5(int src, unsigned char dest[]){
	int position, temp;
	temp = src;
	for(position=4; position >=0; --position){
		dest[position] = numbers[temp % 10];
		temp = temp / 10;
	}

}
