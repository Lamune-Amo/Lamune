unsigned int sleep (unsigned int seconds)
{
	volatile int i;

	for (; seconds > 0; seconds--)
		for (i = 0; i < 1004000; i++);	

	return 0;
}
