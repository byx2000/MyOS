#include "include/idt.h"
#include "include/hardware.h"
#include "include/common.h"
#include "include/display.h"

IntGate g_IDT[256];
int16 g_IDTLimit = sizeof(g_IDT);
IDTRegister g_IDTR;

void SetIntGate(int index, void* proc)
{
	IntGate* pGate = g_IDT + index;
	Memset(pGate, sizeof(IntGate), 0);
	pGate->TYPE = 0xe;
	pGate->DPL = 0;
	pGate->P = 1;
	pGate->S = 0;
	pGate->selector = g_KernelCodeSel;
	pGate->offset_15_0 = (int32)proc & 0x0000ffff;
	pGate->offset_31_16 = ((int32)proc & 0xffff0000) >> 16;
}
