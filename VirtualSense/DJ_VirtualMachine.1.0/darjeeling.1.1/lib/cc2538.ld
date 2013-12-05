















 
 
 
 
 
 
 
 
 
 


























 
 
 
   
   
 







 
 
 



 
 
 
 
 
 
 
 
 











       






 
 
 
 
 
 
 
 
 
       
 
        
 
 
 
 
 
 
 
 
 
 
 
 
 
 




 
 
 


 
 
 



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
   
     
       
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
         
  
       
     
       
         
         
       
   
 
 
 
 
 




































































MEMORY
{
    FLASH (rx) : ORIGIN = 0x200000, LENGTH = 0x0007AFD4
    FLASH_APP (rx) : ORIGIN = 0x0027AFD4, LENGTH = 0x00004800
    FLASH_APP_TBL (rx) : ORIGIN = 0x0027F7D4, LENGTH = 0x00000800
    FLASH_CCA (RX) : ORIGIN = 0x0027FFD4, LENGTH = 12
    SRAM (RWX) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}
SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.vectors))
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > FLASH= 0
    .data :
    {
        _data = .;
        *(vtable)
        *(.data*)
        _edata = .;
    } > SRAM AT > FLASH
    .ARM.exidx :
    {
        *(.ARM.exidx*)
    } > FLASH
    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM
    .flashcca :
    {
        KEEP(*(.flashcca))
    } > FLASH_CCA
}
