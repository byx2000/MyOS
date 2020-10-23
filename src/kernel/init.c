#include "include/init.h"
#include "include/common.h"
#include "include/display.h"
#include "include/gdt.h"
#include "include/hardware.h"
#include "include/idt.h"
#include "include/task.h"

void Init(void)
{
	InitBSS();
	InitGDT();
	
	ClearScreen();
	SetCursorPos(0);
	
	InitTask();
	
	InitTimer();
	Init8259A();
	InitIDT();
}

void InitBSS(void)
{
	extern int bss_start;
	extern int bss_end;
	int size = (char*)(&bss_end) - (char*)(&bss_start);
	Memset(&bss_start, size, 0x00);
}

void InitGDT(void)
{
	SetEmptyDescriptor(&g_GDT[0]); // empty segmemnt (unuse)
	SetDataSegDescriptor(&g_GDT[1], 0, 0xfffff, 1, 0); // kernel data segment
	SetCodeSegDescriptor(&g_GDT[2], 0, 0xfffff, 1, 0); // kernel code segment
	SetDataSegDescriptor(&g_GDT[3], 0, 0xfffff, 1, 3); // user data segment
	SetCodeSegDescriptor(&g_GDT[4], 0, 0xfffff, 1, 3); // user code segment
	g_GDTR.limit = g_GDTLimit;
	g_GDTR.addr = g_GDT;
	SetGDT(&g_GDTR);
	
	SetSelector(&g_KernelDataSel, 1, 0, 0); // kernel data selector
	SetSelector(&g_KernelCodeSel, 2, 0, 0); // kernel code selector
	SetSelector(&g_UserDataSel, 3, 3, 0); // user data selector
	SetSelector(&g_UserCodeSel, 4, 3, 0); // user code selector
	
	SetDS(&g_KernelDataSel);
	SetES(&g_KernelDataSel);
	SetFS(&g_KernelDataSel);
	SetGS(&g_KernelDataSel);
	SetSS(&g_KernelDataSel);
}

void Init8259A(void)
{	
	SetPort8(0x20, 0x11);
	SetPort8(0x21, 0x20);
	SetPort8(0x21, 0x04);
	SetPort8(0x21, 0x01);
	
	SetPort8(0xa0, 0x11);
	SetPort8(0xa1, 0x28);
	SetPort8(0xa1, 0x02);
	SetPort8(0xa1, 0x01);
	
	SetPort8(0x21, 0xfc);
	SetPort8(0xa1, 0xff);
}

void InitIDT(void)
{
	g_IDTR.limit = g_IDTLimit;
	g_IDTR.addr = g_IDT;
	SetIDT(&g_IDTR);
	
	for (int i = 0; i < g_IDTLimit / sizeof(IntGate); ++i)
	{
		SetIntGate(i, DefIntProc);
	}
	
	SetIntGate(INT_TIMER, TimerIntEntry);
	SetIntGate(INT_KEYBOARD, KeyboardIntEntry);
	
	//EnableInt();
}

void InitTimer(void)
{
	// 100HZ
	SetPort8(0x43, 0x34);
	SetPort8(0x40, 0x9b);
	SetPort8(0x40, 0x2e);
}

void InitTask(void)
{
	for (int i = 0; i < MAX_TASK_CNT; ++i)
	{
		g_TaskInfo[i].state = TaskState_Empty;
	}
	
	Memset(&g_TSS, sizeof(TSS), 0);
	//g_TSS.iomap = 0x40000000;
	SetTSSDescriptor(&g_GDT[5], (uint32)(&g_TSS));
	SetSelector(&g_TSSSel, 5, 0, 0);
	SetTR(g_TSSSel);
}
