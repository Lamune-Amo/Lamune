#ifndef _ARCH_KEYBOARD_
#define _ARCH_KEYBOARD_

typedef void (*keyboard_hook_t) (unsigned char code);

void keyboard_irq (void);

ssize_t keyboard_init (void);
ssize_t keyboard_insert_hook (keyboard_hook_t func);
ssize_t keyboard_remove_hook (ssize_t idx);

#endif
