#include "include/task.h"
#include "include/common.h"
#include "include/hardware.h"
#include "include/gdt.h"

Task g_TaskInfo[MAX_TASK_CNT];
uint32 g_CurTask = 0;

static uint32 ps = 0x30000;

TSS g_TSS;
Selector g_TSSSel;

int32 CreateTask(void* entry)
{	
	//DisableInt();
	for (int i = 0; i < MAX_TASK_CNT; ++i)
	{
		if (g_TaskInfo[i].state == TaskState_Empty)
		{
			Memset(&g_TaskInfo[i], sizeof(Task), 0);
			g_TaskInfo[i].state = TaskState_Create;
			g_TaskInfo[i].eip = (uint32)entry;
			g_TaskInfo[i].esp = ps;
			g_TaskInfo[i].eflags = 0x00003202;
			ps += 0x100;
			return i;
		}
	}
	//EnableInt();
	
	return -1;
}
