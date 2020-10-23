#ifndef HARDWARE_H
#define HARDWARE_H

#include "types.h"
#include "gdt.h"

void SetPort8(uint16 port, int8 data);
int8 GetPort8(uint16 port);
void Pause(void);
void SetDS(Selector* pSel);
void SetES(Selector* pSel);
void SetFS(Selector* pSel);
void SetGS(Selector* pSel);
void SetSS(Selector* pSel);

#endif
