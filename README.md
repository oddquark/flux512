# Flux-512

Flux512 is a 16 round <a href="https://en.wikipedia.org/wiki/Feistel_cipher">Feistel Network</a> cipher with a 512-bit block size. 

# About

Flux512 uses 256 substitution boxes, and 256 permutation boxes in order to achieve <a href="https://en.wikipedia.org/wiki/Confusion_and_diffusion">Shannons Confusion/Diffusion.</a> The substitution and permutation boxes are key dependent.

The SHA512 hash algorithm is used to generate key material from the password. This key material is used to initialize the substitution and permutation boxes, and the round key array.

The substitution and permutation boxes lie at the heart of the F function. The F function accepts a block of 32 characters and performs substitution on each character with 32 s-boxes, each of which are chosen according to the current character in the round key. After the substitution is done, a single permutation box is chosen. The positions of the characters within the block are reordered according to the p-box.

The F function is the main component of the Feistel network. An input block of 64 characters is xored with the initialization vector before being split in half (B0 and B1).
B1 is fed into the F function. The result is xored with B0. The result from this xor operation becomes the new B1. The old B1 becomes the new B0 (unencrypted). This is repeated for each round.

# Build/Run
```
user@system:~/ git clone https://github.com/oddquark/flux512
user@system:~/ cd flux512
user@system:~/ mkdir build
user@system:~/ cd build
user@system:~/ cmake ../
user@system:~/ make
```

# Usage

Encrypt
```
user@system:~/ ./flux512 -e <infile> <outfile>
```
Decrypt
```
user@system:~/ ./flux512 -d <infile> <outfile>

```

#Warning

This block cipher is experimental and should not be used for your security.

If you find any cryptographic attacks or bugs/exploits please contact me.   

