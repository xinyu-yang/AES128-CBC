#include<stdio.h>
#include<stdlib.h>

#include "AES.h"
#include "PolyOptn.h"

#define DEBUG

#ifdef DEBUG
void main()
{
	printf("hello world!");
}
#endif

void mixColumns(uchar m[4][4])
{
	for(int i=0;i<4;i++)
	{
		mixColumn(m[i]);
	}
}

void mixColumn(uchar col[4])
{
	uchar list[4] = {0x02,0x03,0x01,0x01};
	uchar res[4];
	for(int i=0;i<4;i++)
	{
		res[i] = 0x00;
		for(int j=0;j<4;j++)
		{
			res[i] =res[i] ^ multiply(list[(4+j-i)%4], col[j]);
		}
	}
	col = res;
}