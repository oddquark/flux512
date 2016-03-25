/*
 * sbox.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: Michael Harper
 */

#include "flux.h"
#include <algorithm>

// init a key dependent sbox.
void Sbox::init(char key)
{
	for(int i = 0; i < 256; i++)
	{
		sbox[i] = char(i);
	}
	srand(key);
	std::random_shuffle(&sbox[0], &sbox[255]);
}

// substitute a single char
char Sbox::lookup(unsigned char l)
{
	unsigned char x =  sbox[int(l)];
	return reinterpret_cast<char&>(x);
}

// substitute a block of char
// each char uses an sbox based on the key (32)
void sBlock(char * in, char * out, string &key, vector<Sbox> &sArray)
{
	for(int i = 0; i < 32; i++)
	{
		unsigned char x =  sArray[int(key[i])].lookup(in[i]);
		out[i] =  reinterpret_cast<unsigned char&>(x);
	}
}
