/*
 * fourier_transform.c
 *
 *  Created on: Jul 28, 2014
 *      Author: Charlie
 */


#include "fourier_transform.h"

/*
FFT(A, m, w)
{
  if (m==1) return vector (a_0)
  else {
    A_even = (a_0, a_2, ..., a_{m-2})
    A_odd  = (a_1, a_3, ..., a_{m-1})
    F_even = FFT(A_even, m/2, w^2)    //w^2 is a primitive m/2-th root of unity
    F_odd = FFT(A_odd, m/2, w^2)
    F = new vector of length m
    x = 1
    for (j=0; j < m/2; ++j) {
      F[j] = F_even[j] + x*F_odd[j]
      F[j+m/2] = F_even[j] - x*F_odd[j]
      x = x * w
  }
  return F
}*/


  void fft_recursive(int target[], int size, char unity_root, int destination[]){
	  int even[size>>1], odd[size>>1], f_even[size>>1], f_odd[size>>1];
	  int count, value;
	  if(size<1){
		  return;
	  }
	  if(size==1){
		  destination[0] = target[0];
		  return;
	  }
	  for(count=0; count<(size>>1); ++count){
		  even[count] = target[2*count];
		  odd[count] = target[2*count + 1];
	  }
	  fft_recursive(even, size>>1, unity_root * unity_root, f_even);
	  fft_recursive(odd, size>>1, unity_root * unity_root, f_odd);
	  value = 1;
	  for(count=0; count<(size>>1); ++count){
		  destination[count] = even[count] + value*odd[count];
		  destination[count + (size>>1)] = even[count] - value*odd[count];
		  value*=unity_root;
	  }
  }
