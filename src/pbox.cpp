/*
 * pbox.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: Michael Harper
 */

#include "flux.h"
#include <algorithm>

// init a permutation box
void Pbox::init(char key)
{
	for(unsigned short int i = 0; i < 32; i++)
	{
		pbox[i] = i;
	}
	srand(key);
	std::random_shuffle(&pbox[0], &pbox[31]);
}

// permute a block (32)
void Pbox::pBlock(char * input, char * output)
{
	for(int i = 0; i < 32; i++)
	{
		output[i] = input[pbox[i]];
	}
}

