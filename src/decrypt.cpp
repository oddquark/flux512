/*
 * decrypt.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: Michael Harper
 */

#include "flux.h"
#include "sha512/sha512.h"
#include <string.h>
#include <fstream>

using std::string;

union {
    char myByte[4];
    long mylong;
} checksum;

void decrypt_block(char *in, char *out, vector<Sbox> &sArray, vector<Pbox> &pArray, KeyArray &kArray, int rounds, char *iv)
{
	char holder[64];

	char b0[32];
	char b1[32];

	memcpy(b0, in, 32*sizeof(char));
	memcpy(b1, in + 32, 32*sizeof(char));

	for(int i = 0; i != rounds; i++)
	{
		if(i != 0)
		{
			memcpy(b0, holder, 32*sizeof(char));
			memcpy(b1, holder + 32, 32*sizeof(char));
		}

		// b0 is used as input to the f function
		char f1_out[32];
		f(b0, f1_out, sArray, pArray, kArray[i]);

		// output from f is xored with b1
		char x1_out[32];
		xor_block(b1, f1_out, x1_out, 32);

		memcpy(holder, x1_out, 32*sizeof(char));
		memcpy(holder + 32, b0, 32*sizeof(char));
	}
	// after rounds, block is xored with iv
	xor_block(holder, iv, out, 64);
}

void decrypt(string iFile, string oFile, string password)
{
	// init s-boxes
	string sKey;
	sKey += genKey(password);
	sKey += genKey("");

	vector<Sbox> sboxArray;
	for(int i = 0; i < 256; i++)
	{
		Sbox tmp;
		tmp.init(sKey[i]);
		sboxArray.push_back(tmp);
	}

	// init p-boxes
	string pKey;
	pKey += genKey("");
	pKey += genKey("");
	vector<Pbox> pboxArray;
	for(int i = 0; i < 256; i++)
	{
		Pbox tmp;
		tmp.init(pKey[i]);
		pboxArray.push_back(tmp);
	}

	// init round keys
	KeyArray keyArray;
	init_key_array(keyArray, 16);

	// invert
	invert_key_array(keyArray);

	// open in/out files
	std::ifstream infile(iFile.c_str(), std::ios::binary);
	std::ofstream outfile(oFile.c_str(), std::ios::binary | std::ios::app);

	// get size of infile
	infile.seekg(0,infile.end);
	unsigned long int size = infile.tellg();

	infile.seekg(0 ,infile.beg);

	// get total block count
	int block_count = size / 64;
	char ibuf[64];
	char obuf[64];

	char iv[64];

	// loop through blocks - 1
	for(int i = 0; i < block_count - 1; i++)
	{
		if (i == 0)
		{
			infile.read(iv, sizeof iv);
		}
		else
		{
			// read, encrypt, write
			infile.read(ibuf, sizeof ibuf);
			decrypt_block(ibuf, obuf, sboxArray, pboxArray, keyArray, 16, iv);
			outfile.write(obuf, sizeof obuf);
		}
	}

	// read final block
	char rbuf[64];
	infile.read(rbuf, sizeof rbuf);

	// decrypt/write final block
	char fbuf[64];
	decrypt_block(rbuf, fbuf, sboxArray, pboxArray, keyArray, 16, iv);

	checksum.myByte[0] = fbuf[60];
	checksum.myByte[1] = fbuf[61];
	checksum.myByte[2] = fbuf[62];
	checksum.myByte[3] = fbuf[63];

	outfile.write(fbuf, checksum.mylong % 64);

	// close files
	infile.close();
	outfile.close();
}
