%include "addr.inc"

org MBR_START_ADDR

mbr_entry:
	mov ax, 0
	mov ss, ax
	mov sp, 0x7c00
	mov ds, ax
	mov es, ax
	mov si, ax

load_loader:
	mov ah, 0x2 ; 读扇区功能号
	mov al, 4 ; 读取的扇区数目
	mov ch, 0 ; 磁道号
	mov cl, 2 ; 起始扇区号
	mov dh, 0 ; 磁头号
	mov dl, 0x80 ; 驱动器号
	mov bx, LOADER_START_ADDR
	int 0x13
	jc pause ; 如果出错，则进入死循环
	
jmp LOADER_START_ADDR

pause:
	jmp pause

times 510-($-$$) db 0
db 0x55, 0xaa 
