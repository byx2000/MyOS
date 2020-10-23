#ifndef COMMON_H
#define COMMON_H

#include "types.h"

void Memset(void* addr, uint32 size, int8 val);

typedef struct
{
	int8 year, month, day;
	int8 hour, minute, second;
} Time;

void GetCurrentTime(Time* pTime);

#endif
