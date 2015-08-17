Charles McGuffey
2015-02-25
Work done with Dr. Chen Liu
Clarkson University
Department of Electrical and Computer Engineering

This is the readme file for our implementation of the Cambridge Biometric Cryptosystem.
Details of this cryptosystem algorithm can be found at: http://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-640.pdf
In the event that this link becomes unavailable, the article can be found using the following information:

Title: Combining cryptography with biometrics effectively
Authors: Feng Hao, Ross Anderson, John Daugman

Disclaimer: There are no guarantees made about the accuracy or safety of this source code.
This project was created for research purposes. The authors have attempted to produce code that is both 
accurate, efficient, and safe, but only limited testing has been performed. The authors of this code shall
not be held accountable for any damage caused by using this code or any derivative works. 

The project has been written using the C programming language. It was compiled using the GCC compiler that 
comes with the Code::Blocks development software. Project build settings indicate that no special flags are
needed for compilation. The application should run effectively in both Windows and GNU/Linux environments,
although this has not been explicitly tested. 

Usage:
There are two major functions used for this code: run_Enrollment and run_Verification. run_Enrollment takes
a twenty byte key, a 256 byte enrollment template, a pointer to a 256 byte locked template, and a pointer to
an integer representing the hash of the key. The first two parameters are inputs to the function, and the 
last two are outputs. run_Verification takes a 256 byte locked template, a hashed key integer, and a 256 
byte sample template. The function returns a 1 if the verification result is positive and a 0 if the result
is negative. 

Implementation Details:
Many of the blocks in the Cambridge Biometric Cryptosystem consist of 7 bits. These values are stored in 
single bytes, typically as characters. 

The algorithm used for the Reed-Solomon encoding and decoding was taken from a free software library rather
than implemented by the authors. See the documentation at the beginning of the ecc.h file for more details.
This implementation is based on 8 byte blocks. This has led to issues where the result of the Reed-Solomon
encoding contains a 1 in the 8th bit position. This bit is ignored by the 7 bit Hadamard encoding and 
decoding. The way that this isssue has been fixed in this implementation is to provide an array of the 
parity bytes generated. During the verification step, the 8th bit of this fix array is ORed with the 
result of the Hadamard decode. This results in sending the correct data to the Reed-Solomon decoding. 
This repair array is declared in fix.h. run_Enrollment puts data into this variable. run_Verification reads
data from this array. If you intend to use this code for verification separate from enrollment, this 
variable will need to be saved after enrollment and edited prior to verification. 

The hash algorithm implemented in this project is rather weak and simplistic. If you would like to change
the hash algorithm used, go to hash.c and edit the hash function. Alternatively, calls to the 
hash_char_array function and the associated include statement can be replaced in the run_Enrollment and 
run_Verification functions. 