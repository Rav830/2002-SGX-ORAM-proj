# ORAM SGX Project

This project is a copy from this [github project](https://github.com/asonnino/hello-enclave) with some modifications for the purpose of learning how this SGX works. 


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


