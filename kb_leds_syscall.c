#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/fs.h>

void (*kbleds_all_on)(void) = NULL;
void (*kbleds_all_off)(void) = NULL;

EXPORT_SYMBOL(kbleds_all_on);
EXPORT_SYMBOL(kbleds_all_off);


SYSCALL_DEFINE0(kb_leds_all_on) {
	(*kbleds_all_on)();
	
	return 0;
}

SYSCALL_DEFINE0(kb_leds_all_off) {
	(*kbleds_all_off)();
	
	return 0;
}
