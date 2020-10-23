global SetPort8           ; void SetPort8(uint16 port, int8 data);
global GetPort8           ; int8 GetPort8(uint16 port);
global Pause              ; void Pause(void);
global SetGDT             ; void SetGDT(void* addr);
global SetIDT             ; void SetIDT(void* addr);
global SetTR              ; void SetTR(void* addr);
global SetDS              ; void SetDS(Selector* pSel);
global SetES              ; void SetES(Selector* pSel);
global SetFS              ; void SetFS(Selector* pSel);
global SetGS              ; void SetGS(Selector* pSel);
global SetSS              ; void SetSS(Selector* pSel);
global EnableInt          ; void EnableInt(void);
global DisableInt         ; void DisableInt(void);
global DefIntProc         ; void DefIntProc(void);
global KeyboardIntEntry   ; void KeyboardIntEntry(void);
global TimerIntEntry      ; void TimerIntEntry(void);


[bits 32]
section .text

SetPort8:
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

GetPort8:
	xor eax, eax
	mov dx, [esp + 4]
	in al, dx
	ret

Pause:
	hlt
	ret
	
SetGDT:
	mov eax, [esp + 4]
	lgdt [eax]
	ret
	
SetIDT:
	mov eax, [esp + 4]
	lidt [eax]
	ret
	
SetTR:
	ltr [esp + 4]
	ret
	
SetDS:
	mov eax, [esp + 4];
	mov ax, [eax]
	mov ds, ax
	ret

SetES:
	mov eax, [esp + 4];
	mov ax, [eax]
	mov es, ax
	ret
	
SetFS:
	mov eax, [esp + 4];
	mov ax, [eax]
	mov fs, ax
	ret
	
SetGS:
	mov eax, [esp + 4];
	mov ax, [eax]
	mov gs, ax
	ret
	
SetSS:
	mov eax, [esp + 4];
	mov ax, [eax]
	mov ss, ax
	ret
	
EnableInt:
	sti
	ret
	
DisableInt:
	cli
	ret
	
DefIntProc:
	mov al, 0x20
	out 0x20, al
	out 0xa0, al
	iret
	
extern KeyboardIntProc
KeyboardIntEntry:
	call KeyboardIntProc
	mov al, 0x20
	out 0x20, al
	out 0xa0, al
	iret
	
extern Task1
extern g_UserCodeSel
extern g_UserDataSel
extern g_KernelDataSel
extern g_TSS
extern TimerIntProc
TimerIntEntry:
	push edi
	push esi
	push ebp
	push dword [esp + 24] ; store ESP
	push edx
	push ecx
	push ebx
	push eax
	push dword [esp + 40] ; store EFLAGS
	push dword [esp + 36] ; store EIP
	call TimerIntProc
	add esp, 40
	
	; test eax
	cmp eax, 0
	jne .L0
	mov al, 0x20
	out 0x20, al
	out 0xa0, al
	iret
	
	.L0
	; test cs privilage
	mov ebx, [esp + 4]
	and ebx, 0x00000003
	jne .L
	push dword 0
	push dword 0
	
	.L:
	; eax points to task info
	
	; set TSS
	mov ebx, esp
	add ebx, 20
	mov [g_TSS + 4], ebx
	mov ebx, [g_KernelDataSel]
	mov [g_TSS + 8], ebx
	
	; set EIP
	mov ebx, [eax]
	mov [esp], ebx
	
	; set CS
	mov ebx, [g_UserCodeSel]
	mov [esp + 4], ebx
	
	; set EFLAGS
	mov ebx, [eax + 4]
	mov [esp + 8], ebx
	
	; set ESP
	mov ebx, [eax + 24]
	mov [esp + 12], ebx
	
	; set ss
	mov ebx, [g_UserDataSel]
	mov [esp + 16], ebx
	
	; set ds, es, gs, fs
	mov bx, [g_UserDataSel]
	mov ds, bx
	mov es, bx
	mov gs, bx
	mov fs, bx
	
	; set ecx, edx, ebp, esi, edi
	mov ecx, [eax + 16]
	mov edx, [eax + 20]
	mov ebp, [eax + 28]
	mov esi, [eax + 32]
	mov edi, [eax + 36]
	
	; set ebx
	mov ebx, [eax + 12]
	
	;mov eax, Task1
	;mov [esp], eax
	;mov eax, [g_UserCodeSel]
	;mov [esp + 4], eax
	;mov ax, [g_UserDataSel]
	;mov ds, ax
	;mov es, ax
	;mov gs, ax
	;mov fs, ax
	;mov dword [esp + 12], 0x30000
	;mov [esp + 16], ax
	
	mov al, 0x20
	out 0x20, al
	out 0xa0, al
	
	; set eax
	mov eax, [eax + 8]
	
	iret
