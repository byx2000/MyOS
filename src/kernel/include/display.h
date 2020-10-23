#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

void ClearScreen(void);
void SetCursorPos(uint16 pos);
void PrintChar(char c);
void PrintInt(int num);
void PrintUInt(unsigned int num);
void PrintString(const char* str);

#endif
