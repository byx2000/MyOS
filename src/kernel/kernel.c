#include "include/types.h"
#include "include/common.h"
#include "include/init.h"
#include "include/hardware.h"
#include "include/display.h"
#include "include/gdt.h"
#include "include/idt.h"
#include "include/task.h"

// IMPORTANT: You CANNOT add any function definition before KernelEntry function !!!
// However, you can add data herein whatever you want.
// kernel entry
// The kernel code will be loaded in memory at address 0

typedef struct 
{
    unsigned int  baseAddrLow ;  //内存基地址的低32位
    unsigned int  baseAddrHigh;  //内存基地址的高32位
    unsigned int  lengthLow;     //内存块长度的低32位
    unsigned int  lengthHigh;    //内存块长度的高32位
    unsigned int  type;          //描述内存块的类型
} AddrRangeDesc;

void Task1(void);
void Task2(void);
void Task3(void);

void KernelEntry(AddrRangeDesc* memChkInfo, uint32 menChkCnt)
{
	Init();
	CreateTask(Task1);
	CreateTask(Task2);
	CreateTask(Task3);
	
	EnableInt();
    
    while (1)
    {
    	Pause();
    }
}

int g = 0;

void Task1(void)
{
	while (1)
	{
		PrintChar('A');
	}
}

void Task2(void)
{
	while (1)
	{
		PrintChar('B');
	}
}

void Task3(void)
{
	while (1)
	{
		PrintChar('C');
	}
}
