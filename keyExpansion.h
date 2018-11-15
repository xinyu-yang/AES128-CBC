#include "AES.h"

#ifndef __keyExpansion__

uint32 firstWord(uint32 word, uchar j);
void keyExpansion();
const static uint32 key[4]={0x00,0x00,0x00,0x00};
static uint32 expKey[44]={key[0],key[1],key[2],key[3]};

#endif

#define __keyExpansion__