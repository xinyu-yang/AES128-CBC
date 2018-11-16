#include<stdio.h>
#include<stdlib.h>

#include "AES.h"
#include "PolyOptn.h"
#include "mixColumns.h"

//#define DEBUG

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
	uchar temp[4];
	for(int i=0;i<4;i++)
		temp[i] = col[i];
	for(int i=0;i<4;i++)
	{
		col[i] = 0x00;
		for(int j=0;j<4;j++)
		{
			col[i] =col[i] ^ multiply(list[(4+j-i)%4], temp[j]);
		}
	}
}

void invmixColumn(uchar col[4])
{
	uchar list[4] = {0x0E,0x0B,0x0D,0x09};
	uchar temp[4];
	for(int i=0;i<4;i++)
		temp[i] = col[i];
	for(int i=0;i<4;i++)
	{
		col[i] = 0x00;
		for(int j=0;j<4;j++)
		{
			col[i] =col[i] ^ multiply(list[(4+j-i)%4], temp[j]);
		}
	}
}

void invmixColumns(uchar m[4][4])
{
	for(int i=0;i<4;i++)
	{
		invmixColumn(m[i]);
	}
}