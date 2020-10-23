%include "addr.inc"

org LOADER_START_ADDR

jmp loader_start

; Temp GDT, I will rebuild it when enter protect mode

GDT_REG: 	dw GDT_END - GDT_START 
			dd GDT_START

GDT_START:	dd 0x00000000, 0x00000000
GDT_DATA:	dd 0x0000ffff, 0x00cf9200
GDT_CODE:	dd 0x0000ffff, 0x00cf9800
GDT_END:

SEL_DATA    equ     GDT_DATA - GDT_START
SEL_CODE    equ     GDT_CODE - GDT_START

MEM_CHK_BUF: times 256 db 0
MEM_CHK_CNT: dd 0


loader_start:
	
	; get memory info
	mov ebx, 0
	mov es, bx
    mov di, MEM_CHK_BUF
    .loop:
    	mov eax, 0xE820
     	mov ecx, 20
     	mov edx, 0x534D4150
     	int 0x15
     	jc .faild
     	add di, 20   
     	inc dword [MEM_CHK_CNT]
     	cmp ebx, 0
     	jne .loop
     	jmp .init_GDTR
     	
     .faild:
     	mov dword [MEM_CHK_CNT], 0
     
	.init_GDTR:
		mov ax, 0
		mov ds, ax
		lgdt [ds:GDT_REG]
	
	; open A20
	in al,0x92
	or al,0000_0010B
	out 0x92,al
	
	; open protected mode
	cli
	mov eax,cr0
	or eax,1
	mov cr0,eax

	; now we jump to protected mode!
	jmp dword SEL_CODE:flush

[bits 32]
flush:    

    ; init segment register
    mov ax, SEL_DATA
    mov ds, ax
    mov gs, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov esp, 0x200000

    mov eax, 5
    mov ebx, 0
    mov ecx, 20 ; read block count
    read_loop:
        call read_hard_disk_0
        inc eax
        dec ecx
        jne read_loop

    ; jump to kernel code
    ; jmp KERNEL_START_ADDR
    push dword [MEM_CHK_CNT]
    push MEM_CHK_BUF
    call KERNEL_START_ADDR
	

; function: read disk
read_hard_disk_0:                        ;从硬盘读取一个逻辑扇区
                                         ;EAX=逻辑扇区号
                                         ;DS:EBX=目标缓冲区地址
                                         ;返回：EBX=EBX+512 
         push eax 
         push ecx
         push edx
      
         push eax
         
         mov dx,0x1f2
         mov al,1
         out dx,al                       ;读取的扇区数

         inc dx                          ;0x1f3
         pop eax
         out dx,al                       ;LBA地址7~0

         inc dx                          ;0x1f4
         mov cl,8
         shr eax,cl
         out dx,al                       ;LBA地址15~8

         inc dx                          ;0x1f5
         shr eax,cl
         out dx,al                       ;LBA地址23~16

         inc dx                          ;0x1f6
         shr eax,cl
         or al,0xe0                      ;第一硬盘  LBA地址27~24
         out dx,al

         inc dx                          ;0x1f7
         mov al,0x20                     ;读命令
         out dx,al

  .waits:
         in al,dx
         and al,0x88
         cmp al,0x08
         jnz .waits                      ;不忙，且硬盘已准备好数据传输 

         mov ecx,256                     ;总共要读取的字数
         mov dx,0x1f0
  .readw:
         in ax,dx
         mov [ebx],ax
         add ebx,2
         loop .readw

         pop edx
         pop ecx
         pop eax
      
         ret
