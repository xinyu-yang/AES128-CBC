#include<stdio.h>
#include<stdlib.h>

#include "AES.h"

#define DEBUG

#ifndef DEBUG
void main()
{
	FILE *data = fopen("testData.txt", "rb");
}

#endif