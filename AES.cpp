#include<stdio.h>
#include<stdlib.h>

#include "AES.h"
#include "Sbox.h"
#include "keyExpansion.h"
#include "shiftRows.h"
#include "mixColumns.h"
#include "keyEnpy.h"

//#define DEBUG

static uint32 enCount = 0x00;
static uint32 deCount = 0x00;
//encryption and decryption function
void encrypt(FILE *inputData, FILE *outputData);
void unitEncrypt(uchar list[16]);
void decrypt(FILE *inputData, FILE *outputData);
void unitDecrypt(uchar list[16]);




#ifndef DEBUG
void main()
{
	
	//generate Sbox and invSbox
	genSbox(Sbox);
	geninvSbox(invSbox);
	//encrypt key expansion,key:expKey
	//key expansion must after generate Sbox invxbox!!!!!!
	keyExpansion();
	
	printf("*************************************************\n");
	//input data will be encrypted
	FILE *inputData = fopen("testData.txt", "rb");
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
	getchar();

}

#endif


void encrypt(FILE *inputData, FILE *outputData)
{
	//input list of 16 bytes
	uchar list[16];
	int suffix=0;
	uchar temp=0x00;
	while(fread(list,16,1,inputData)==1)
	{
		unitEncrypt(list);
		fwrite(list,16,1,outputData);
	}

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
		unitEncrypt(list);
		fwrite(list,16,1,outputData);
	}

	fclose(inputData);
	fclose(outputData);
}

void unitEncrypt(uchar list[16])
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

	printf("The %-3d encryption process complted!\n", enCount++);
}


void decrypt(FILE *inputData, FILE *outputData)
{
	//input list of 16 bytes
	uchar list[16];
	int suffix=0;
	while(fread(list,16,1,inputData)==1)
	{
		unitDecrypt(list);
		fwrite(list,16,1,outputData);
	}

	fclose(inputData);
	fclose(outputData);
}

void unitDecrypt(uchar list[16])
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
		}
	}

	printf("The %-3d decryption process complted!\n", deCount++);

}