#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//#define DEBUG

#ifdef DEBUG
void main()
{
	FILE* data=fopen("testData.txt", "wb+");
	char ch='A';
	for(int i=0;i<128;i++)
	{
		fputc(ch,data);
		ch = (ch + 1 - 'A')%26 + 'A';
	}
	fclose(data);
}

#endif