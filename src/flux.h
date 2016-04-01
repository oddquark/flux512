/*
 * flux.h
 *
 *  Created on: Feb 16, 2016
 *      Author: Michael Harper
 */

#ifndef FLUX_H_
#define FLUX_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

typedef vector<string> KeyArray;

class Sbox
{
	 unsigned char sbox[256];

public:

	void init(char);
	char lookup(unsigned char);
};

class Pbox
{
	unsigned short int pbox[32];

public:

	void init(char);
	void pBlock(char *, char *);
};

void usage();
void init_key_array(KeyArray &, int);
void invert_key_array(KeyArray &);
void sBlock(char *, char *, string &, vector<Sbox> &);
void f(char *, char *, vector<Sbox> &, vector<Pbox> &, string &);
void genIv(char *);
void encrypt(string, string, string);
void decrypt(string, string, string);
void xor_block(char *,char *,char *, int);
void getPass(string &, bool);
void modIv(char * iv);
void mismatch();

#endif /* FLUX_H_ */
