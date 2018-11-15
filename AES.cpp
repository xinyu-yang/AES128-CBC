#include<stdio.h>
#include<stdlib.h>

#include "AES.h"
#include "Sbox.h"
#include "keyExpansion.h"
#include "shiftRows.h"
#include "mixColumns.h"
#include "keyEnpy.h"

//#define DEBUG


//matrix from encrypt 128 bits 
static uchar array[4][4];

#ifndef DEBUG
void main()
{
	//loop num
	uchar loopNum = 9;
	//encrypt key expansion,key:expKey
	keyExpansion();
	//generate Sbox and invSbox
	genSbox(Sbox);
	geninvSbox(invSbox);
	//input data will be encrypted
	FILE *inputData = fopen("testData.txt", "rb");
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			fread(&array[j][i],1,1,inputData);
			//printf("0x%x\t", array[j][i]);
		}
		//printf("\n");
		//printf("0x%x\n", *(uint32 *)array[i]);
	}
	fclose(inputData);
	//first encrypt by key
	keyEnpy(array, 0);
	//9 cycles encrypt
	for(int i=0; i<loopNum; i++)
	{
		//Sbox substitude
		for(int j=0;j<4;j++)
		{
			for(int k=0;k<4;k++)
			{
				array[k][j] = SboxSubt(array[k][j], Sbox);
			}
		
		}
		//shift rows
		shiftRows(array);
		mixColumns(array);
		//encrypt by key
		keyEnpy(array, (i+1)*4);
	}//for loopNum

	//the last time substitude
	for(int j=0;j<4;j++)
	{
		for(int k=0;k<4;k++)
		{
			array[k][j] = SboxSubt(array[k][j], Sbox);
		}
		
	}
	//the last time shift rows
	shiftRows(array);
	//the last time key encrypt
	keyEnpy(array, 40);

	//save data
	FILE *outputData = fopen("outputData.txt", "wb+");
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			fwrite(&array[j][i],1,1,outputData);
			//printf("0x%x\t", array[j][i]);
		}
		//printf("\n");
		//printf("0x%x\n", *(uint32 *)array[i]);
	}
	fclose(outputData);

	printf("encryption process complted!");
	getchar();

}

#endif