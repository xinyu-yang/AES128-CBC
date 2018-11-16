#include<stdio.h>

#include "AES.h"

//#define DEBUG

#ifdef DEBUG
void main()
{
	printf("this is shiftrow");
}
#endif

//circle left move
void shiftRows(uchar m[4][4])
{
	uchar mm[4][4];
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			mm[i][j] = m[i][j];
		}
	}
	//mm = m;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			m[i][j] = mm[i][(j+i)%4];
		}
	}
	//return mm;
}


//circle right move
void invshiftRows(uchar m[4][4])
{
	uchar mm[4][4];
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			mm[i][j] = m[i][j];
		}
	}
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			m[i][j] = mm[i][(j-i+4)%4];
		}
	}
	//m = mm;
}