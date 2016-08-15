STARTUP(vectors.o)
ENTRY(reset_vector)
INPUT(extras.o)
GROUP(libtarget.a libtarget_ram.a libgcc.a)

MEMORY
{
    rom     : ORIGIN = 0x00000000, LENGTH = 32M
    ram     : ORIGIN = 0x40000000, LENGTH = 0x12F000
    ram2    : ORIGIN = 0x4012F000, LENGTH = 0x3000
}

SECTIONS
{
    .osdstr         0x4012F000   :   
    {
        ./OBJS/stringdec.o (.data*)
    } >  ram2

    .rom_vectors    0x40000000   :
    { 
        . = . ; KEEP (*(.vectors)) 
    } >  ram
    
    .text           0x40032000   :
    { 
    __dwCoPoStartAddr = ABSOLUTE(.);
        _stext = ABSOLUTE(.); *(.text*) *(.gnu.warning) *(.gnu.linkonce*) *(.init)
    } >  ram
    _etext = .; PROVIDE (etext = .);
    
    .rodata         ALIGN (0x8)  :
    {
        . = . ; *(.rodata*)
        ./OBJS/notransl.o (.data*)
    } >  ram  
    
    .rodata1        ALIGN (0x1)  :
    {
        . = . ; *(.rodata1) 
    } >  ram  
    __dwCoPoEndAddr = ABSOLUTE(.);
    
    .data           ALIGN (0x8)  :
    {
        __ram_data_start = ABSOLUTE (.); *(.data*) *(.data1) _GOT1_START_ = ABSOLUTE (.); *(.got1) _GOT1_END_ = ABSOLUTE (.); _GOT2_START_ = ABSOLUTE (.); *(.got2) _GOT2_END_ = ABSOLUTE (.); . = ALIGN (4); KEEP(*( SORT (.ecos.table.*))); __CTOR_LIST__ = ABSOLUTE (.); KEEP(*(SORT(.ctors*))) __CTOR_END__ = ABSOLUTE (.); __DTOR_LIST__ = ABSOLUTE (.); KEEP(*(SORT(.dtors*))) __DTOR_END__ = ABSOLUTE (.); _GOT_START = ABSOLUTE (.); _GLOBAL_OFFSET_TABLE_ = ABSOLUTE (.) + 32768; _SDA_BASE_ = ABSOLUTE (.); *(.got.plt) *(.got) _GOT_END_ = ABSOLUTE (.); *(.dynamic) *(.sdata*) *(.sbss*) *(.eh_frame) 
    } >  ram  
    __rom_data_start = LOADADDR (.data); . = ALIGN (8);__ram_data_end = .; PROVIDE (__ram_data_end = .); _edata = .; PROVIDE (edata = .); 
    
    .bss            ALIGN (0x8)  :
    {
        __bss_start = ABSOLUTE (.);
        *(.scommon) *(.dynbss) *(.bss) *(COMMON) 
        __bss_end = ABSOLUTE (.); 
    } >  ram  

    .bss1           ALIGN (0x1)  :
    {
        . = . ; *(.bss1) 
    } >  ram    
    __heap1   = ALIGN (0x8); . = ALIGN(8); _end = .; PROVIDE (end = .); .debug          0 : { *(.debug) } .line           0 : { *(.line) } .debug_srcinfo  0 : { *(.debug_srcinfo) } .debug_sfnames  0 : { *(.debug_sfnames) } .debug_aranges  0 : { *(.debug_aranges) } .debug_pubnames 0 : { *(.debug_pubnames) } .debug_info     0 : { *(.debug_info) } .debug_abbrev   0 : { *(.debug_abbrev) } .debug_line     0 : { *(.debug_line) } .debug_frame    0 : { *(.debug_frame) } .debug_str      0 : { *(.debug_str) } .debug_loc      0 : { *(.debug_loc) } .debug_macinfo  0 : { *(.debug_macinfo) } .debug_weaknames 0 : { *(.debug_weaknames) } .debug_funcnames 0 : { *(.debug_funcnames) } .debug_typenames 0 : { *(.debug_typenames) } .debug_varnames  0 : { *(.debug_varnames) } 
}