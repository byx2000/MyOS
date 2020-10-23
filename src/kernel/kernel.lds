ENTRY(KernelEntry)

SECTIONS
{
	. = 0;
	
	/*代码段*/
	.text : 
	{
        * (.text)
    }
    
    /*只读数据段(字符串常量)*/
    .rodata : 
    {
    	* (.rodata)
    }
    
    /*未初始化全局变量和初始化为0的全局变量*/
    bss_start = .; 
    .bss : 
    {
        * (.bss)
    }
    bss_end  = .;    
    
    /*已初始化且不为0的全局变量*/
    .data : 
    {
        * (.data)
    }
    
    .note.gnu.property : { }
    .comment : { }
}
