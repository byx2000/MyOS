#ifndef TIMER_H
#define TIMER_H

#include "types.h"

const int TIMER_DURATION = 10; // ms

void* TimerIntProc(uint32 eip, uint32 eflags, uint32 eax, uint32 ebx, uint32 ecx, uint32 edx, uint32 esp, uint32 ebp, uint32 esi, uint32 edi);

#endif
