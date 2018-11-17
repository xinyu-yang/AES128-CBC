#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include "AES.h"
#include "Sbox.h"
#include "keyExpansion.h"
#include "shiftRows.h"
#include "mixColumns.h"
#include "keyEnpy.h"

static uchar IV[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};

//#define DEBUG

static uint32 enCount = 0x00;
static uint32 deCount = 0x00;
//encryption and decryption function
void encrypt(FILE *inputData, FILE *outputData);
void unitEncrypt(uchar list[16], uchar *preC);
void decrypt(FILE *inputData, FILE *outputData);
void unitDecrypt(uchar list[16], uchar *preC);




#ifndef DEBUG
void main()
{
	//initiate time variabls, and record start time
	clock_t startTime,endTime;
	double runTime=0;

	//generate Sbox and invSbox
	genSbox(Sbox);
	geninvSbox(invSbox);
	//encrypt key expansion,key:expKey
	//key expansion must after generate Sbox invxbox!!!!!!
	keyExpansion();
	
	startTime = clock();
	printf("Author: xinyu yang\n");
	printf("*************************************************\n");
	//input data will be encrypted
	FILE *inputData = fopen("The_old_man_and_the_sea.txt", "rb");
	//save data
	FILE *cryptedData = fopen("encryptedData.txt", "wb+");
	encrypt(inputData, cryptedData);
	printf("\nOutput in encryptedData.txt");
	printf("\n*************************************************\n\n");
	
	cryptedData = fopen("encryptedData.txt", "rb+");
	FILE *decryptData = fopen("decryptedData.txt", "wb+");
	decrypt(cryptedData, decryptData);
	printf("\nOutput in decryptedData.txt");
	printf("\n*************************************************\n");
	endTime = clock();
	runTime = endTime - startTime;
	printf("Run time is %.3f ms\n",runTime);
	getchar();

}

#endif


void encrypt(FILE *inputData, FILE *outputData)
{
	//input list of 16 bytes
	uchar list[16];
	int suffix=0;
	uchar temp=0x00;
	uchar *preC = IV;
	fwrite(preC,16,1,outputData);
	while(fread(list,16,1,inputData)==1)
	{
		unitEncrypt(list, preC);
		for(int i=0;i<16;i++)
			preC[i] = list[i];
		fwrite(list,16,1,outputData);
	}

	//encrypt least data, whose length < 128
	while((temp = fgetc(inputData)!=EOF))
	{
		list[suffix++] = temp;
	}

	if(suffix > 0)
	{
		for(int i=suffix; i<16; i++)
		{
			list[i] = 0x00;
		}
		unitEncrypt(list, preC);
		fwrite(list,16,1,outputData);
	}

	fclose(inputData);
	fclose(outputData);
}

void unitEncrypt(uchar list[16], uchar *preC)
{
	//the suffix of list
	int suffix=0;
	//loop num
	uchar loopNum = 9;
	//matrix from encrypt 128 bits 
	uchar array[4][4];
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			array[j][i] = list[suffix++];
			array[j][i] ^= preC[suffix - 1]; 
		}
	}
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

	suffix = 0;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			list[suffix++] = array[j][i];
		}
	}
	if(enCount%1000==0)
		printf("The %-5d encryption process complted!\n", enCount);
	enCount++;
}


void decrypt(FILE *inputData, FILE *outputData)
{
	//input list of 16 bytes
	uchar list[16];
	int suffix=0;
	uchar IV[16] = {0x00};
	uchar *preC = IV;
	uchar thisC[16] = {0x00};
	fread(preC,16,1,inputData);
	while(fread(list,16,1,inputData)==1)
	{
		for(int i=0;i<16;i++)
			thisC[i] = list[i];
		unitDecrypt(list, preC);
		for(int i=0;i<16;i++)
			preC[i] = thisC[i];
		fwrite(list,16,1,outputData);
	}

	fclose(inputData);
	fclose(outputData);
}

void unitDecrypt(uchar list[16], uchar *preC)
{
	//the suffix of list
	int suffix=0;
	//loop num
	uchar loopNum = 9;
	//matrix from encrypt 128 bits 
	uchar array[4][4];
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			array[j][i] = list[suffix++];
		}
	}
	//first encrypt by key
	keyEnpy(array, 40);

	//9 cycles encrypt
	for(int i=0; i<loopNum; i++)
	{
		//shift rows
		invshiftRows(array);
		//Sbox substitude
		for(int j=0;j<4;j++)
		{
			for(int k=0;k<4;k++)
			{
				array[k][j] = SboxSubt(array[k][j], invSbox);
			}
		
		}
		//encrypt by key
		keyEnpy(array, 40 - (i+1)*4);
		invmixColumns(array);
	}//for loopNum

	//last shift rows
	invshiftRows(array);

	//Sbox substitude
	for(int j=0;j<4;j++)
	{
		for(int k=0;k<4;k++)
		{
			array[k][j] = SboxSubt(array[k][j], invSbox);
		}
		
	}

	//last encrypt by key
	keyEnpy(array, 0);

	suffix = 0;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			list[suffix++] = array[j][i];
			list[suffix - 1] ^= preC[suffix - 1];
		}
	}
	if(deCount%1000 ==0)
		printf("The %-5d decryption process complted!\n", deCount);
	deCount++;

}