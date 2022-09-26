#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define ALL_ON 449
#define ALL_OFF 450

int main(int argc, char *argv[]) {
    for (int i = 0; i < 10; i++) {
	    syscall(ALL_ON);
	    sleep(1);
	    syscall(ALL_OFF);
	    sleep(1);
    }

    return 0;
}
