
//there'll be a 0 at the end of string
asm(" .string \"WTK20070612VER06\"");
asm(" .byte 0x56, 0x45, 0x52, 2+0x30");
asm(" .byte 7+0x30,8+0x30,0+0x30,0+0x30");
asm(" .byte 2+0x30,7+0x30,7+0x30,1+0x30");
asm(" .word 6 ");
asm(" .word 32 ");                 //dwIR_Type
//asm(" .word  ");
asm(" .word 21 ");  //dwLoader_Name
asm(" .word 0 ");                 //dwData_Reserved

