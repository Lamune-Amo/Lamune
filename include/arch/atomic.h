#ifndef _ARCH_ATOMIC_
#define _ARCH_ATOMIC_

int atomic_cas_addr (void * volatile *ptr, void *cmp_val, void *swap_val);

#endif
