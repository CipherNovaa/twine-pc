/* The Windows build's partial link of a module (port/Makefile LD_R): the common symbols -d allocates go after the BSS
   (without a .bss statement here, the PE linker puts them at its start, over the objects' own variables). */
SECTIONS
{
  .bss : { *(.bss) *(COMMON) }
}
