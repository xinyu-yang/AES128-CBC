#include "AES.h"

#ifndef __mixColumns__

void mixColumns(uchar m[4][4]);
void invmixColumns(uchar m[4][4]);
void mixColumn(uchar col[4]);
void invmixColumn(uchar col[4]);

#endif

#define __mixColumns__