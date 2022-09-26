#include <stdlib.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define ALL_LEDS_ON   0x07
#define CAPS_ON 0x04
#define NUM_ON 0x02
#define SCR_ON 0x01
#define RESTORE_LEDS  0xFF

int main() {
	int f = open("/dev/test_led",  O_WRONLY);
	
	if (f == -1) {
		printf("%s\n", strerror(errno));
		exit(1);
	}
	
	char buf;
	
	for (int i = 0; i < 3; i++) {
		buf = CAPS_ON;
		write(f, &buf, 1);
		
		sleep(1);
		
		buf = NUM_ON;
		write(f, &buf, 1);
		
		sleep(1);
		
		buf = SCR_ON;
		write(f, &buf, 1);
		
		sleep(1);
	}
	
	buf = RESTORE_LEDS;
	write(f, &buf, 1);
	
	close(f);
	
	return 0;
}
