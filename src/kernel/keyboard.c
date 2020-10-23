#include "include/keyboard.h"
#include "include/hardware.h"
#include "include/display.h"

void KeyboardIntProc(void)
{
	int8 code = GetPort8(0x60);
	PrintString("code = ");
	PrintInt(code);
	PrintChar('\n');
}
