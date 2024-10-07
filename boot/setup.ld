OUTPUT_FORMAT(elf32-amo)
OUTPUT_ARCH(amo)
ENTRY(start)

SECTIONS
{
	. = 0x0;

	.text :
	{
		*(.inittext)
		*(.text)
	}
	
	.rodata :
	{
		*(.rodata)
	}

	.data :
	{
		*(.data)
	}

	.bss :
	{
		*(COMMON)
		*(.bss)
	}
}
