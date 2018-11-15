#include "AES.h"

#ifndef __keyExpansion__

uint32 firstWord(uint32 word, uchar j);
uint32* keyExpansion(uint32 key[4]);

#endif

#define __keyExpansion__