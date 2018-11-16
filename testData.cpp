#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//#define DEBUG
const int size = 0x1000;

#ifdef DEBUG
void main()
{
	FILE* data=fopen("testData.txt", "wb+");
	char ch='A';
	for(int i=0;i < size;i++)
	{
		fputc(ch,data);
		ch = (ch + 1 - 'A')%26 + 'A';
	}
	fclose(data);
}

#endif