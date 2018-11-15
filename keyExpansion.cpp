#include<stdio.h>
#include<stdlib.h>

#include "AES.h"
#include "Sbox.h"

#define DEBUG
const uint32 ron[] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

#ifdef DEBUG
void main()
{
	genSbox(Sbox);
	geninvSbox(invBox);
	printf("Hello world");
	getchar();
}
#endif

//keyExpansion
uint32* keyExpansion(uint32 key[4])
{
	uint32 expKey[44]={key[0],key[1],key[2],key[3]};
	for(int i=0; i<44; i++)
	{
		if(i%4==0)
		{
			expKey[i] = expKey[i-4] ^ firstWord(expKey[i-1], i);
		}
		else
		{
			expKey[i] = expKey[i-4] ^ expKey[i-1];
		}
	}
	return expKey;
}



//first word process function
uint32 firstWord(uint32 word, uchar j)
{
	uint32 cycleMove=0x00;
	uint32 subt=0x00;
	uint32 add=0x00;
	cycleMove ^= (word & 0xff000000) >> 24;
	cycleMove ^= (word << 8) & 0xffffff00;
	for(i=0;i<4;i++)
	{
		subt ^= SboxSubt((cycleMove >> (4*i)), Sbox) << (4*i);
	}
	add = subt ^ (ron[j/4] << 24);
	return add;
}






