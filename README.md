# Flux-512

Flux512 is a 16 round <a href="https://en.wikipedia.org/wiki/Feistel_cipher">Feistel Network</a> cipher with a 512-bit block size. 

#Warning

This block cipher is most likely insecure and should not be used to encrypt anything important.

# About

Flux512 uses 256 substitution boxes, and 256 permutation boxes in order to achieve <a href="https://en.wikipedia.org/wiki/Confusion_and_diffusion">Shannons Confusion/Diffusion.</a> The substitution and permutation boxes are key dependent.

The SHA512 hash algorithm is used to generate key material from the password. This key material is used to initialize the substitution and permutation boxes, and the round key array. Each round key is 256-bits.

# Build

Requires: cmake, ncurses

#####Debian/Ubuntu:
```
user@system:~/ sudo apt-get install libncurses5-dev cmake
user@system:~/ git clone https://github.com/oddquark/flux512
user@system:~/ cd flux512
user@system:~/ mkdir build
user@system:~/ cd build
user@system:~/ cmake ../
user@system:~/ make
```

# Run

Encrypt
```
user@system:~/ ./flux512 -e <infile> <outfile>
```
Decrypt
```
user@system:~/ ./flux512 -d <infile> <outfile>

```
