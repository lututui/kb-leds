#include <linux/module.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <linux/console_struct.h>
#include <linux/vt_kern.h>

#define DEVICE_NAME "kb_leds_device"

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

// Para syscall
extern void (*kbleds_all_on);
extern void (*kbleds_all_off);

void all_on(void);
void all_off(void);


// Controle interno
static int device_open_count = 0;
static int major_num;

static char *msg_ptr;
static char msg_buffer;

struct tty_driver *my_driver;

#define RESTORE_LEDS  0xFF

// Operações
static struct file_operations file_ops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

// Device read
static ssize_t device_read(struct file *flip, char *buffer, size_t len, loff_t *offset) {
	printk(KERN_ALERT "This operation is not supported.\n");
	return -EINVAL;
}

// Device write
static ssize_t device_write(struct file *flip, const char *buffer, size_t len, loff_t *offset) {
	get_user(msg_buffer, buffer);
	
	(my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, *msg_ptr);
	
	return 1;
}

// Device open
static int device_open(struct inode *inode, struct file *file) {
	if (device_open_count) {
		return -EBUSY;
	}
	
	device_open_count++;
	try_module_get(THIS_MODULE);
	return 0;
}

// Device close
static int device_release(struct inode *inode, struct file *file) {
	device_open_count--;
	module_put(THIS_MODULE);
	return 0;
}

/* Usado na syscall */

void all_on(void) {
	msg_buffer = 0x07;
	
	(my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, *msg_ptr);
}

void all_off(void) {
	msg_buffer = 0x00;
	
	(my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, *msg_ptr);
}


int kbleds_init(void) {
    /* Usado na syscall */ 
	kbleds_all_on = &all_on;
	kbleds_all_off = &all_off;
	
	// tty driver
	my_driver = vc_cons[fg_console].d->port.tty->driver;
	
    // Shorthand para o endereço do buffer
	msg_ptr = &msg_buffer;
	
    // Número do device
	major_num = register_chrdev(0, DEVICE_NAME, &file_ops);
	
	if (major_num < 0) {
		printk(KERN_ALERT "Could not register device: %d\n", major_num);
		return major_num;
	} else {
		printk(KERN_INFO "kb_leds_example module loaded with device major number %d\n", major_num);
	}
	

	return 0;
}

void kbleds_exit(void) {
	printk(KERN_ALERT "kb_leds: unloading...\n");
    
	(my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, RESTORE_LEDS);
	unregister_chrdev(major_num, DEVICE_NAME);
}

module_init(kbleds_init);
module_exit(kbleds_exit);


MODULE_DESCRIPTION("Control keyboard LEDS");
MODULE_AUTHOR("ARTHUR JUAN EDUARDO");
MODULE_LICENSE("GPL");
