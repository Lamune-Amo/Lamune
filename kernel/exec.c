#include "kernel/task.h"
#include "lamune/string.h"
#include "lamune/types.h"

void exef (const char *name, uint32_t pc)
{
	strcpy (CURRENT_TASK->name, name);

	asm volatile (
		"jmp\t\tr0\n\t"
		: 
		: "r"(pc)
		: "memory"
	);
}
