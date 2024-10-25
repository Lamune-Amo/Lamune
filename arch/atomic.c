int atomic_cas_addr (void * volatile *ptr, void *cmp_val, void *swap_val)
{
	/* lock */
	asm volatile (
		"lock\n\t"
	);

	if (*ptr == cmp_val)
	{
		*ptr = swap_val;
		/* unlock */
		asm volatile (
			"lock\n\t"
		);
		return 1;
	}

	/* unlock */
	asm volatile (
		"lock\n\t"
	);
	return 0;
}
