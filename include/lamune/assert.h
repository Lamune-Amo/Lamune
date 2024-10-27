#ifndef _LAMUNE_ASSERT_
#define _LAMUNE_ASSERT_

void assert_panic (const char *msg, const char *file, int line);

# define panic(x) do { assert_panic (x, __FILE__, __LINE__); } while (0);
# define assert(x) do { if (!(x)) panic("assertion failed") } while (0);

#endif
