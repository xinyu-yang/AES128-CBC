#include "AES.h"

#ifndef __keyExpansion__

uint32 firstWord(uint32 word, uchar j);
void keyExpansion();
extern const uint32 key[4];
extern uint32 expKey[44];

#endif

#define __keyExpansion__