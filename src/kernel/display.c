#include "include/display.h"
#include "include/hardware.h"
#include "include/idt.h"

char* VIDEO_ADDR = (char*)0xb8000;

uint16 g_CurPos = 0;

void ClearScreen(void)
{
    char* p = VIDEO_ADDR;
    for (int i = 0; i < 80 * 25; ++i)
    {
        *p++ = ' ';
        *p++ = 0x07;
    }
}

void SetCursorPos(uint16 pos)
{
    pos %= (80 * 25);
    int8 low = pos;
    int8 high = pos >> 8;
    SetPort8(0x03d4, 0x0e);
    SetPort8(0x03d5, high);
    SetPort8(0x03d4, 0x0f);
    SetPort8(0x03d5, low);
    g_CurPos = pos;
}

void PrintChar(char c)
{
	DisableInt();
	if (c == '\n')
    {   
        g_CurPos += (80 - (g_CurPos % 80));
    }
    else
    {
        *(VIDEO_ADDR + g_CurPos * 2) = c;
        g_CurPos++;
    }
    
    SetCursorPos(g_CurPos);
    EnableInt();
}

void PrintInt(int num)
{
	if (num == 0)
	{
		PrintChar('0');
		return;
	}
	else if (num < 0)
	{
		PrintChar('-');
		num = -num;
	}
	
	char digits[32];
	int i = 0;
	
	while (num)
	{
		digits[i++] = num % 10 + '0';
		num /= 10;
	}
	
	i--;
	while (i >= 0)
	{
		PrintChar(digits[i--]);
	}
}

void PrintUInt(unsigned int num)
{
	if (num == 0)
	{
		PrintChar('0');
		return;
	}
	else if (num < 0)
	{
		PrintChar('-');
		num = -num;
	}
	
	char digits[32];
	int i = 0;
	
	while (num)
	{
		digits[i++] = num % 10 + '0';
		num /= 10;
	}
	
	i--;
	while (i >= 0)
	{
		PrintChar(digits[i--]);
	}
}

void PrintString(const char* str)
{
    while (*str)
    {
        PrintChar(*str++);
    }
}
