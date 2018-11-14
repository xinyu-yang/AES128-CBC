#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "AES.h"
#include "PolyOptn.h"

#define uchar unsigned char
//#define DEBUG

#ifdef DEBUG

void main()
{
	//uchar test = multiply(0x3a, 0x24);
	uint16 r=0x00;
	uchar test = divide(0x20E3,0x11B,r);
	printf("result: 0x%x,\t reserved:0x%x \n", test,r);
	//printf("Hello World!");
	getchar();
}

#endif

//多项式加减

uint16 addDel(uint16 a, uint16 b)
{
	uint16 res=0x00;
	res=a ^ b;
	return res;
}


//polyminal multiply

//divide modeule 0x1B when overflow
uchar mod(uchar a)
{
	return (a << 1) ^ ((a & 0x80) ? 0x1B : 0);
}

//multiply
uchar multiply(uchar a, uchar b)
{
	uchar res = 0x00;
	uchar bitAnd = 0x01;
	uchar addList[8] = {a};
	for(int i=0;i<7;i++)
	{
		addList[i+1] = mod(addList[i]);
	}

	for(int i=0;i<8;i++)
	{
		if(b & bitAnd)
		{
			res = addDel(res, addList[i]);
		}
		bitAnd = bitAnd << 1;
	}
	return res;
}


//多项式除法

//查找第一个非0值的位置
uchar findHigh(uint16 a)
{
	for(int i=15;i>=0;i--)
	{
		if(a & (0x01 << i))
			return i+1;
	}
}

//a divide b, return result, r is reservation.
uint16 divide(uint16 a, uint16 b, uint16 &r)
{
	uint16 res=0x00;
	uint16 tempBit=0x01;
	while(findHigh(a) >= findHigh(b))
	{
		uchar diff = findHigh(a) - findHigh(b);
		res = addDel(res, tempBit << diff);
		a = addDel(a, b << diff);
	}
	r = a;
	return res;
}

