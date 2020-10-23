#include "include/gdt.h"
#include "include/common.h"
#include "include/task.h"

// Global Descriptor Table Data
GlobalDescriptor g_GDT[6];
uint16 g_GDTLimit = sizeof(g_GDT);
GDTRegister g_GDTR;
Selector g_KernelCodeSel, g_KernelDataSel, g_UserDataSel, g_UserCodeSel;

void SetEmptyDescriptor(GlobalDescriptor* pDesc)
{
	Memset(pDesc, sizeof(GlobalDescriptor), 0);
}

void SetDataSegDescriptor(GlobalDescriptor* pDesc, uint32 base, uint32 limit, uint8 is4k, uint8 DPL)
{	
	Memset(pDesc, sizeof(GlobalDescriptor), 0);
	
	pDesc->base_15_0 = base & 0x0000ffff;
	pDesc->base_23_16 = (base & 0x00ff0000) >> 16;
	pDesc->base_31_24 = (base & 0xff000000) >> 24;
	
	pDesc->limit_15_0 = limit & 0x0000ffff;
	pDesc->limit_19_16 = (limit & 0x000f0000) >> 16;
	
	pDesc->G = is4k ? 1 : 0;
	pDesc->S = 1;
	pDesc->TYPE = 0x2;
	pDesc->DPL = DPL;
	pDesc->P = 1;
	pDesc->L = 0;
	pDesc->DB = 1;
}

void SetCodeSegDescriptor(GlobalDescriptor* pDesc, uint32 base, uint32 limit, uint8 is4k, uint8 DPL)
{
	Memset(pDesc, sizeof(GlobalDescriptor), 0);
	
	pDesc->base_15_0 = base & 0x0000ffff;
	pDesc->base_23_16 = (base & 0x00ff0000) >> 16;
	pDesc->base_31_24 = (base & 0xff000000) >> 24;
	
	pDesc->limit_15_0 = limit & 0x0000ffff;
	pDesc->limit_19_16 = (limit & 0x000f0000) >> 16;
	
	pDesc->G = is4k ? 1 : 0;
	pDesc->S = 1;
	pDesc->TYPE = 0x8;
	pDesc->DPL = DPL;
	pDesc->P = 1;
	pDesc->L = 0;
	pDesc->DB = 1;
}

void SetStackSegDescriptor(GlobalDescriptor* pDesc, uint32 base, uint32 limit, uint8 is4k)
{
	Memset(pDesc, sizeof(GlobalDescriptor), 0);
	
	pDesc->base_15_0 = base & 0x0000ffff;
	pDesc->base_23_16 = (base & 0x00ff0000) >> 16;
	pDesc->base_31_24 = (base & 0xff000000) >> 24;
	
	pDesc->limit_15_0 = limit & 0x0000ffff;
	pDesc->limit_19_16 = (limit & 0x000f0000) >> 16;
	
	pDesc->G = is4k ? 1 : 0;
	pDesc->S = 1;
	pDesc->TYPE = 0x6;
	pDesc->DPL = 0;
	pDesc->P = 1;
	pDesc->L = 0;
	pDesc->DB = 1;
}

void SetTSSDescriptor(GlobalDescriptor* pDesc, uint32 base)
{
	Memset(pDesc, sizeof(GlobalDescriptor), 0);
	
	pDesc->base_15_0 = base & 0x0000ffff;
	pDesc->base_23_16 = (base & 0x00ff0000) >> 16;
	pDesc->base_31_24 = (base & 0xff000000) >> 24;
	
	uint32 limit = sizeof(TSS);
	pDesc->limit_15_0 = limit & 0x0000ffff;
	pDesc->limit_19_16 = (limit & 0x000f0000) >> 16;
	
	pDesc->G = 0;
	pDesc->S = 0;
	pDesc->TYPE = 0x9;
	pDesc->DPL = 0;
	pDesc->P = 1;
	pDesc->L = 0;
	pDesc->DB = 1;
}

void SetSelector(Selector* pSel, uint16 index, uint8 RPL, uint8 TI)
{
	pSel->index = index;
	pSel->RPL = RPL;
	pSel->TI = TI ? 1 : 0;
}
