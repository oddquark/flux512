/*
 * utils.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: Michael Harper
 */

#include "flux.h"
#include "sha512/sha512.h"
#include <algorithm>
#include <iostream>
#include <ncurses.h>
#include <string.h>

using std::cout;
using std::endl;
using std::string;

// invert round key array
void invert_key_array(KeyArray &keyArray)
{
	 std::reverse(keyArray.begin(),keyArray.end());
}

// the f function
void f(char * in, char * out, vector<Sbox> &sArray, vector<Pbox> &pArray, string &key)
{
	char tmp[32];
	sBlock(in, tmp, key, sArray);
	pArray[key[0]].pBlock(tmp, out);
}

// xor a block of chars
void xor_block(char * a,char * b,char * c, int sz)
{
	for(int i = 0; i < sz; i++)
	{
		unsigned char t = a[i] ^ b[i];
		c[i] = t;
	}
}

// password mismatch screen
void mismatch()
{
	clear();
	mvprintw(0,0, "Error - Passwords must match!");
	refresh();
	getch();
	clear();
}

// usage info
void usage()
{
	cout << endl;
	cout << "Flux-512" << endl;
	cout << "Syntax: flux-512 [options] [infile] [outfile]" << endl << endl;
	cout << "Options: [-e] [-d]" << endl;
	cout << "-e : encrypt" << endl;
	cout << "-d : decrypt" << endl << endl;
}

// init the round key array
void init_key_array(KeyArray &keyArray, int rounds)
{
	for(int i = 0; i <  4; i++)
	{
		string s = genKey("");
		keyArray.push_back(s.substr(0, 32));
		keyArray.push_back(s.substr(32,32));
		keyArray.push_back(s.substr(64,32));
		keyArray.push_back(s.substr(96,32));
	}
}

// get password from user
void getPass(string &out, bool isRepeat)
{
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	string pad;

	bool ex = false;
	while(ex == false)
	{
		if(isRepeat)
		{
			mvprintw(0,0, "Repeat:");
		}
		else
		{
			mvprintw(0,0, "Password:");
		}
		if(isRepeat)
		{
			mvprintw(0,7, pad.c_str());
		}
		else
		{
			mvprintw(0,9, pad.c_str());
		}
		int ch = getch();

		if(ch == '\n')
		{
			clear();
			endwin();
			return;
		}
		else
		{
			out += ch;
			pad += '*';
			refresh();
		}
	}
}

// get initialization vector from user..
// the string from the user is hashed with sha512..
// the halves of the hash are xored to produce the iv (64)
void modIv(char * iv)
{
	char tmp[64];
	memcpy(tmp, iv, sizeof tmp);

	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	string random_pool;
	bool ex = false;
	while (ex == false)
	{
		clear();
		mvprintw(1,0,"Type randomly on the keyboard to seed the iv.");
		mvprintw(5,0,"Press Enter when finished.");
		mvprintw(3,0,"iv:");

		string s = sha512(random_pool);
		string a = s.substr(0, 8);
		string b = s.substr(8,8);
		string c = s.substr(16,8);
		string d = s.substr(32,8);

		mvprintw(3,5,a.c_str());
		mvprintw(3,15,b.c_str());
		mvprintw(3,25,c.c_str());
		mvprintw(3,35,d.c_str());

		char ch = getch();

		if (int(ch) == '\n')
		{
			char a[64];
			char b[64];

			for(int i = 0; i < 64; i++)
			{
				a[i] = s[i];
				b[i] = s[i+64];
			}
			xor_block(a, b,iv, 64);
			ex = false;
			break;
		}
		random_pool += ch;
		refresh();
	}
	endwin();
}
