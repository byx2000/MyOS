#ifndef GDT_H
#define GDT_H

#include "types.h"

typedef struct
{
	int64 limit_15_0 : 16;
	int64 base_15_0 : 16;
	int64 base_23_16 : 8;
	int64 TYPE : 4;
	int64 S : 1;
	int64 DPL : 2;
	int64 P : 1;
	int64 limit_19_16 : 4;
	int64 AVL : 1;
	int64 L : 1;
	int64 DB : 1;
	int64 G : 1;
	int64 base_31_24 : 8;
} __attribute__ ((packed)) GlobalDescriptor;

typedef struct
{
	int16 limit;
	GlobalDescriptor* addr;
} __attribute__ ((packed)) GDTRegister;

typedef struct
{
	int16 RPL : 2;
	int16 TI : 1;
	int16 index : 13;
} __attribute__ ((packed)) Selector;

// Global Descriptor Table Data
extern GlobalDescriptor g_GDT[];
extern uint16 g_GDTLimit;
extern GDTRegister g_GDTR;
extern Selector g_KernelCodeSel, g_KernelDataSel, g_UserDataSel, g_UserCodeSel;

void SetEmptyDescriptor(GlobalDescriptor* pDesc);
void SetDataSegDescriptor(GlobalDescriptor* pDesc, uint32 base, uint32 limit, uint8 is4k, uint8 DPL);
void SetCodeSegDescriptor(GlobalDescriptor* pDesc, uint32 base, uint32 limit, uint8 is4k, uint8 DPL);
void SetStackSegDescriptor(GlobalDescriptor* pDesc, uint32 base, uint32 limit, uint8 is4k);
void SetTSSDescriptor(GlobalDescriptor* pDesc, uint32 base);

void SetSelector(Selector* pSel, uint16 index, uint8 RPL, uint8 TI);

void SetGDT(void* addr); // implemented by assembly

#endif
