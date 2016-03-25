/*
 * encrypt.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: Michael Harper
 */

#include "flux.h"
#include "sha512/sha512.h"
#include <string.h>
#include <fstream>
#include <vector>

using std::vector;
using std::string;

union {
    char myByte[4];
    long myLong;
} checksum;

void encrypt_block(char *in, char *out, vector<Sbox> &sArray, vector<Pbox> &pArray, KeyArray &kArray, int rounds, char *iv)
{
	// before rounds, block is xored with iv
	char holder[64];
	xor_block(in, iv, holder, 64);

	char b0[32];
	char b1[32];

	for(int i = 0; i != rounds; i++)
	{
		memcpy(b0, holder, 32*sizeof(char));
		memcpy(b1, holder + 32, 32*sizeof(char));

		// send b1 to the f function
		char f1_out[32];
		f(b1, f1_out, sArray, pArray, kArray[i]);

		// xor output from f with b0
		char x1_out[32];
		xor_block(b0, f1_out, x1_out, 32);

		// b1 is the new b0 (without modification)
		memcpy(holder, b1, 32*sizeof(char));
		// output from xor is the new b0
		memcpy(holder + 32, x1_out, 32*sizeof(char));
	}
	memcpy(out, holder, 64*sizeof(char));
}

void encrypt(string iFile, string oFile, string password)
{
	// init iv
	char iv[64];
	modIv(iv);

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

	std::ifstream infile(iFile.c_str(), std::ios::binary);
	std::ofstream outfile(oFile.c_str(), std::ios::binary | std::ios::app);

	// write iv to outfile
	outfile.write(iv, sizeof iv);

	// get size of infile
	infile.seekg(0,infile.end);
	unsigned long int size = infile.tellg();
	infile.seekg(0,infile.beg);

	// add padding to make infile % 64
	vector<char> padding;
	string::size_type sz = padding.size() + size + 4;
	int c = 0;
	string::size_type psz;
	while(sz % 64 != 0)
	{
		padding.push_back(0);
		psz = padding.size();
		sz = psz + size + 4;
		c++;
	}
	const int remainder = 64 - (psz + 4);

	char cpad[psz];
	for(string::size_type i = 0; i < psz; i++)
	{
		cpad[i] = padding[i];
	}

	// get total block count
	int block_count = sz / 64;
	char ibuf[64];
	char obuf[64];

	// loop through blocks - 1
	for(int i = 0; i < block_count - 1; i++)
	{
		// read, encrypt, write
		infile.read(ibuf, sizeof ibuf);
		encrypt_block(ibuf, obuf, sboxArray, pboxArray, keyArray, 16, iv);
		outfile.write(obuf, sizeof obuf);
	}

	// read final block
	char rbuf[remainder];
	infile.read(rbuf, remainder);

	char fbuf[64];

	// convert checksum to char array
	char cs[4];
	checksum.myLong = size;

	cs[0] = checksum.myByte[0];
	cs[1] = checksum.myByte[1];
	cs[2] = checksum.myByte[2];
	cs[3] = checksum.myByte[3];

	// copy final block to fbuf
	memcpy(fbuf, rbuf, remainder* sizeof(char));
	memcpy(fbuf + remainder, cpad, psz*sizeof(char));
	memcpy(fbuf + remainder + psz, cs, sizeof(char)*4);

	// encrypt/write final block
	char febuf[64];
	encrypt_block(fbuf, febuf, sboxArray, pboxArray, keyArray, 16, iv);
	outfile.write(febuf, sizeof febuf);

	// close files
	infile.close();
	outfile.close();
}
