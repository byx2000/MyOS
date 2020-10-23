#ifndef TASK_H
#define TASK_H

#include "types.h"
#include "gdt.h"

typedef struct TSS
{
	uint32 last;
	uint32 esp0;
	uint32 ss0;
	uint32 esp1;
	uint32 ss1;
	uint32 esp2;
	uint32 ss2;
	uint32 cr3;
	uint32 eip;
	uint32 eflags;
	uint32 eax;
	uint32 ecx;
	uint32 edx;
	uint32 ebx;
	uint32 esp;
	uint32 ebp;
	uint32 esi;
	uint32 edi;
	uint32 es;
	uint32 cs;
	uint32 ss;
	uint32 ds;
	uint32 fs;
	uint32 gs;
	uint32 ldtSel;
	uint32 iomap;
} TSS;

typedef enum
{
	TaskState_Empty,
	TaskState_Create,
	TaskState_Running,
	TaskState_Ready,
	TaskState_Block,
} TaskState;

typedef struct
{
	uint32 eip, eflags, eax, ebx, ecx, edx, esp, ebp, esi, edi;
	TaskState state;
} Task;

#define MAX_TASK_CNT 10

extern Task g_TaskInfo[];
extern uint32 g_CurTask;
extern TSS g_TSS;
extern Selector g_TSSSel;

int32 CreateTask(void* entry);
void SwitchTask(uint32 taskId);
void SaveRegisters(Task* pTask);
void LoadRegister(Task* pTask);
void Exit();

void SetTR(Selector sel); // implemented by assembly

#endif
