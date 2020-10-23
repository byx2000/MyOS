#ifndef IDT_H
#define IDT_H

#include "types.h"
#include "gdt.h"

typedef struct
{
	int64 offset_15_0 : 16;
	Selector selector;
	int64 : 8; // unuse and must be zero
	int64 TYPE : 4;
	int64 S : 1;
	int64 DPL : 2;
	int64 P : 1;
	int64 offset_31_16 : 16;
	
} __attribute__ ((packed)) IntGate;

typedef struct
{
 	int16 limit;
 	IntGate* addr;
} __attribute__ ((packed)) IDTRegister;

void SetIntGate(int index, void* proc);

void SetIDT(void* addr); // implemented by assembly
void EnableInt(void); // implemented by assembly
void DisableInt(void); // implemented by assembly
void DefIntProc(void); // implemented by assembly
void KeyboardIntEntry(void); // implemented by assembly
void TimerIntEntry(void); // implemented by assembly


extern IntGate g_IDT[];
extern int16 g_IDTLimit;
extern IDTRegister g_IDTR;

#define INT_TIMER    0x20
#define INT_KEYBOARD 0x21

#endif
