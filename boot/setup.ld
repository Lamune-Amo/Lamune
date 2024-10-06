OUTPUT_FORMAT(elf32-amo)
OUTPUT_ARCH(amo)
ENTRY(start)

SECTIONS
{
	. = 0x0;

	.inittext			: { *(.inittext) }

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}
	
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
