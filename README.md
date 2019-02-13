# ORAM SGX Project

This project is a copy from this [github project](https://github.com/asonnino/hello-enclave) with some modifications for the purpose of learning how this SGX works. 

## How I am understanding this is working

To explain how I understand this works:  
The main method (ie 'int main(...)') is located in App/App.cpp  
The first thing main does is initialize the enclave using some function in sgx_utils  
Once the enclave is intialized I execute ecalls to do things  

* One example is multiplication on line 62 in App.cpp
* From here the ecall takes the 2 values and executes the multiply function that exists in Enclave/MathOperations/Multiply.cpp
* The way this is all linking properly is because of the makefile (somehow?) and the .edl files
	* These edl files define the ocalls and ecalls that we can use and they can be imported as can be seen in Enclave/Enclave.edl <- the main edl file
* Also, while the ecall is doing it's work, we can also execute ocalls.
	* The best example is in Enclave/Enclave.cpp where when we do the ecall of generate_random_number, that function executes an ocall of printing some content.


Also, from the array copying example that I made, (line 107 in App/App.cpp)  

* I create an integer array, send that data into the enclave using ecall_store_int_arr and now I have 2 int arrrays one in main memory and one in enclave memory.
* and then I do some multiplication and copy the data from the enclave copy into another one. (with all the source code for these functions in Enclave/DataMovement/ArrMove.cpp)


This is why I feel that the main method is executing outside the enclave, and then context switches into the enclave when executing ecalls, and then switches context back to the main method at the end of an ecall function.

I think that I will need to make this into a multi-threaded project moving along so that we don't lose data from a data stream, but I am not ready for that part yet.

### Post meeting notes
After talking with Lee today, I understand it as 2 threads and each ecall/ocall is a blocked synchronous call. 

The next objective is to access unprotected memory from the enclave without an ocall/ecall but more directly. Example I want to do, create a string, access it from the enclave, create an encrypted version inside enclave, and then just ocall print it.

### Expected output

```bash
Processing random number generation...
0
Random number: 42
Adding numbers ... 
Doing ocall add to test ocall printing from ecall not
Adding outside
The enclave said to print the following int: 144
Enclave addition success! 42 + 100 = 142
Enclave multiplication success! 34 * 2 = 68
Seal round trip success! Receive back 142


Some Buffer Space

The enclave said to print the following int: 5

Some Buffer Space
The enclave said to print the following int: 0
The enclave said to print the following int: 1
The enclave said to print the following int: 2
The enclave said to print the following int: 3
The enclave said to print the following int: 4
The enclave said to print the following int: 5
The enclave said to print the following int: 6
The enclave said to print the following int: 7
The enclave said to print the following int: 8
The enclave said to print the following int: 9
The enclave said to print the following int: 10
The enclave said to print the following int: 11
The enclave said to print the following int: 12
The enclave said to print the following int: 13
The enclave said to print the following int: 14
The enclave said to print the following int: 15
The enclave said to print the following int: 16
The enclave said to print the following int: 17
The enclave said to print the following int: 18
The enclave said to print the following int: 19

Some Buffer Space
The enclave said to print the following int: 0
The enclave said to print the following int: 7
The enclave said to print the following int: 14
The enclave said to print the following int: 21
The enclave said to print the following int: 28
The enclave said to print the following int: 35
The enclave said to print the following int: 42
The enclave said to print the following int: 49
The enclave said to print the following int: 56
The enclave said to print the following int: 63
The enclave said to print the following int: 70
The enclave said to print the following int: 77
The enclave said to print the following int: 84
The enclave said to print the following int: 91
The enclave said to print the following int: 98
The enclave said to print the following int: 105
The enclave said to print the following int: 112
The enclave said to print the following int: 119
The enclave said to print the following int: 126
The enclave said to print the following int: 133
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0
The enclave said to print the following int: 0

Some Buffer Space
outHold has 0
outHold has 7
outHold has 14
outHold has 21
outHold has 28
outHold has 35
outHold has 42
outHold has 49
outHold has 56
outHold has 63
outHold has 70
outHold has 77
outHold has 84
outHold has 91
outHold has 98
outHold has 105
outHold has 112
outHold has 119
outHold has 126
outHold has 133

```

Below is just the readme from the previous/copied project.
## Intel SGX "Hello World"

This is meant to be a base template for an [Intel SGX](https://github.com/01org/linux-sgx/) application on Linux. Not sure if it is just me, but I feel the documentations on Intel SGX development on Linux is still sorely lacking. This meant to be a stub of a "Getting-started" tutorial.

This template is based on the SampleEnclave app of the sample enclaves provided with the Intel SGX Linux [drivers](https://github.com/01org/linux-sgx-driver) and [SDK](https://github.com/01org/linux-sgx/).

## Features

- Sample code for doing `ECALL`
- Sample code for doing `OCALL`
- Sample code for sealing (can be taken out and patched into your enclave!)

## TODO

- Tutorial explaining what each directory and file is used for.

- Write a getting started tutorial.

- Tutorial on treating `edl`s as static library (with the sealing functions as example)

## Contribute

Any help for the above TODOs or any general feedback will be much appreciated! Go ahead and submit those PRs in!


