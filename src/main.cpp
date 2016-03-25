/*
 * main.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: Michael Harper
 */

#include "flux.h"
#include <fstream>
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char * argv[])
{
	if (argc < 4)
	{
		usage();
		return 0;
	}

	for (int i = 1; i < argc; i++)
	{
		if(string(argv[i]) == "-e")
		{
			string pass;
			string repeat;

			bool ex = false;
			while(ex == false)
			{
				getPass(pass, false);
				getPass(repeat, true);
				if(pass == repeat)
				{
					ex = true;
				}
				else
				{
					mismatch();
					pass = "";
					repeat = "";
				}
			}
			encrypt(argv[i+1],argv[i+2],pass);
		}
		else if(string(argv[i]) == "-d")
		{
			string pass;
			getPass(pass, false);
			decrypt(argv[i+1],argv[i+2],pass);
		}
		else if(string(argv[i]) == "-h")
		{
			usage();
			return 0;
		}
	}
	return 0;
}
