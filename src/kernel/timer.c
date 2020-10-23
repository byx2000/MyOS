#include "include/timer.h"
#include "include/display.h"
#include "include/common.h"
#include "include/task.h"

void* TimerIntProc(uint32 eip, uint32 eflags, uint32 eax, uint32 ebx, uint32 ecx, uint32 edx, uint32 esp, uint32 ebp, uint32 esi, uint32 edi)
{
    /*static int i = 0;
    static int cnt = 1000 / TIMER_DURATION;
    if (i % cnt == 0)
    {
    	ClearScreen();
    	SetCursorPos(0);
    	PrintString("Current Time: ");
		Time time;
		GetCurrentTime(&time);
		PrintString("20");
		PrintInt(time.year);
		PrintChar('/');
		PrintInt(time.month);
		PrintChar('/');
		PrintInt(time.day);
		PrintString("  ");
		PrintInt(time.hour);
		PrintChar(':');
		PrintInt(time.minute);
		PrintChar(':');
		PrintInt(time.second);
		PrintChar('\n');
    }
	i++;*/
	
	if (g_TaskInfo[g_CurTask].state == TaskState_Create)
	{
		g_TaskInfo[g_CurTask].state = TaskState_Running;
		return &g_TaskInfo[g_CurTask];
	}
	
	// save current task context
	g_TaskInfo[g_CurTask].state = TaskState_Ready;
	g_TaskInfo[g_CurTask].eip = eip;
	g_TaskInfo[g_CurTask].esp = esp;	
	g_TaskInfo[g_CurTask].eflags = eflags;
	g_TaskInfo[g_CurTask].eax = eax;
	g_TaskInfo[g_CurTask].ebx = ebx;
	g_TaskInfo[g_CurTask].ecx = ecx;
	g_TaskInfo[g_CurTask].edx = edx;
	g_TaskInfo[g_CurTask].ebp = ebp;
	g_TaskInfo[g_CurTask].esi = esi;
	g_TaskInfo[g_CurTask].edi = edi;
	
	// choose another task to run
	uint32 old = g_CurTask;
	g_CurTask = (g_CurTask + 1) % MAX_TASK_CNT;
	while (g_CurTask != old)
	{
		if (g_TaskInfo[g_CurTask].state == TaskState_Ready || g_TaskInfo[g_CurTask].state == TaskState_Create)
		{
			break;
		}
		g_CurTask = (g_CurTask + 1) % MAX_TASK_CNT;
	}
	
	g_TaskInfo[g_CurTask].state = TaskState_Running;
	
	return &g_TaskInfo[g_CurTask];
	
	//return 0;
	
}
