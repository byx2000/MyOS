#include "include/common.h"
#include "include/hardware.h"

void Memset(void* addr, uint32 size, int8 val)
{
	char* p = addr;
	for (int i = 0; i < size; ++i)
	{
		*p++ = val;
	}
}

static int8 BCDToInt8(int8 bcd)
{
	static int d[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	return d[bcd >> 4] * 10 + d[bcd & 0x0f];
}

void GetCurrentTime(Time* pTime)
{
	SetPort8(0x70, 0x09);
	pTime->year = BCDToInt8(GetPort8(0x71));
	SetPort8(0x70, 0x08);
	pTime->month = BCDToInt8(GetPort8(0x71));
	SetPort8(0x70, 0x07);
	pTime->day = BCDToInt8(GetPort8(0x71));
	SetPort8(0x70, 0x04);
	pTime->hour = BCDToInt8(GetPort8(0x71));
	SetPort8(0x70, 0x02);
	pTime->minute = BCDToInt8(GetPort8(0x71));
	SetPort8(0x70, 0x00);
	pTime->second = BCDToInt8(GetPort8(0x71));
}
