/*
 *  chardev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for put_user */
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/string.h>

/*  
 *  Prototypes - this would normally go in a .h file
 */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev"	/* Dev name as it appears in /proc/devices   */
#define BUF_LEN 80		/* Max length of the message from the device */

/* 
 * Global variables are declared as static, so are global within the file. 
 */

static int Major;		/* Major number assigned to our device driver */
static int Device_Open = 0;	/* Is device open?  
				 * Used to prevent multiple access to device */
static char msg[BUF_LEN];	/* The msg the device will give when asked */
static char *msg_Ptr;

static struct gpio leds[] = {
			//{  19, GPIOF_OUT_INIT_HIGH, "LED" },
			{  29, GPIOF_OUT_INIT_HIGH, "LED" },
	};

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
        Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
	  printk(KERN_ALERT "Registering char device failed with %d\n", Major);
	  return Major;
	}

	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");

	return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
	/* 
	 * Unregister the device 
	 */
  unregister_chrdev(Major, DEVICE_NAME);
	/*
  int ret = unregister_chrdev(Major, DEVICE_NAME);
	if (ret < 0)
		printk(KERN_ALERT "Error in unregister_chrdev: %d\n", ret);
  */
}

/*
 * Methods
 */

/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file)
{
	//static int counter = 1;

	if (Device_Open)
		return -EBUSY;

	Device_Open++;
	
	//if((counter%2)==1)
        sprintf(msg, "Created by: Resha Dwika Hefni Al-Fahsi, 16/394959/TK/44251");
	//\n");
	//else if((counter%2)==0){
	//sprintf(msg, "16/394959/TK/44251\n");
	//}
	//counter++;
	msg_Ptr = msg;
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;		/* We're now ready for our next caller */

	/* 
	 * Decrement the usage count, or else once you opened the file, you'll
	 * never get get rid of the module. 
	 */
	module_put(THIS_MODULE);

	return 0;
}

/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset)
{
	/*
	 * Number of bytes actually written to the buffer 
	 */
	int bytes_read = 0;

	/*
	 * If we're at the end of the message, 
	 * return 0 signifying end of file 
	 */
	if (*msg_Ptr == 0)
		return 0;

	/* 
	 * Actually put the data into the buffer 
	 */
	while (length && *msg_Ptr) {

		/* 
		 * The buffer is in the user data segment, not the kernel 
		 * segment so "*" assignment won't work.  We have to use 
		 * put_user which copies data from the kernel data segment to
		 * the user data segment. 
		 */
		put_user(*(msg_Ptr++), buffer++);

		length--;
		bytes_read++;
	}

	/* 
	 * Most read functions return the number of bytes put into the buffer
	 */
	return bytes_read;
}

/*  
 * Called when a process writes to dev file: echo "hi" > /dev/hello 
 */
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	int bytes_write = 0;

	if(!strcmp(buff,"LED ON")){
		gpio_set_value(leds[0].gpio, 1);
	}
	else if(!strcmp(buff,"LED OFF")){
		gpio_set_value(leds[0].gpio, 0);
	}
	else if(!strcmp(buff,"DELAY")){
		usleep_range(0,500000);
	}

	return bytes_write;
	//printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
	//return -EINVAL;
}

/* 

	#include <linux/module.h>
	#include <linux/kernel.h>
	#include <linux/init.h>
	#include <linux/gpio.h>
	#include <linux/interrupt.h>
	#include <linux/delay.h>
	
	MODULE_LICENSE("GPL"); 
	
	#define DRIVER_AUTHOR "Resha Dwika Hefni Al-Fahsi <16/394959/TK/44251>"
	#define DRIVER_DESC   "Loadable Kernel Module to Access Raspberry Pi I/O"
	
	static char *nama = "Resha Dwika Hefni Al-Fahsi\n";
	static char *nim = "<16/394959/TK/44251>\n";
	
	int enable = 0;
	module_param(enable, int, 0644);
	
	static struct gpio leds[] = {
			{  19, GPIOF_OUT_INIT_HIGH, "LED" },
	};
	
	// Define GPIOs for BUTTONS
	static struct gpio buttons[] = {
			{ 20, GPIOF_IN, "BUTTON 1" },
			{ 21, GPIOF_IN, "BUTTON 2" }
	};
	
	static int button_irqs[] = { -1,-1 };
	
	static irqreturn_t button_isr(int irq, void *data)
	{
		if(enable){
			if(irq == button_irqs[0] //&& !gpio_get_value(leds[0].gpio)
			) {
				
					gpio_set_value(leds[0].gpio, 1);
					//usleep_range(0,500000);
					//gpio_set_value(leds[0].gpio, 0);
					//usleep_range(0,500000);
			}
		
			else if(irq == button_irqs[1] //&& gpio_get_value(leds[0].gpio)
			) {
				
					gpio_set_value(leds[0].gpio, 0);
			}
		}
		
		return IRQ_HANDLED;
	}
	
	static int __init init_tugas(void)
	{
		printk(KERN_INFO "Blink LED Module installed\n");
		printk(KERN_INFO "Author: %s\n",nama);
		printk(KERN_INFO "Author's Student Number: %s\n",nim);
		int ret = 0;
	
		printk(KERN_INFO "%s\n", __func__);
		ret = gpio_request_array(leds, ARRAY_SIZE(leds));
	
		if (ret) {
			printk(KERN_ERR "Unable to request GPIOs for LEDs: %d\n", ret);
			return ret;
		}
	
		ret = gpio_request_array(buttons, ARRAY_SIZE(buttons));
	
		if (ret) {
			printk(KERN_ERR "Unable to request GPIOs for BUTTONs: %d\n", ret);
			goto fail1;
		}
	
		printk(KERN_INFO "Current button1 value: %d\n", gpio_get_value(buttons[0].gpio));
	
		ret = gpio_to_irq(buttons[0].gpio);
	
		if(ret < 0) {
			printk(KERN_ERR "Unable to request IRQ: %d\n", ret);
			goto fail2;
		}
		button_irqs[0] = ret;
	
		printk(KERN_INFO "Successfully requested BUTTON1 IRQ # %d\n", button_irqs[0]);
	
		ret = request_irq(button_irqs[0], button_isr, IRQF_TRIGGER_RISING // | IRQF_DISABLED 
			, "gpiomod#button1", NULL);
	
		if(ret) {
			printk(KERN_ERR "Unable to request IRQ: %d\n", ret);
			goto fail2;
		}
		ret = gpio_to_irq(buttons[1].gpio);
	
		if(ret < 0) {
			printk(KERN_ERR "Unable to request IRQ: %d\n", ret);
			goto fail2;
		}
	
		button_irqs[1] = ret;
	
		printk(KERN_INFO "Successfully requested BUTTON2 IRQ # %d\n", button_irqs[1]);
	
		ret = request_irq(button_irqs[1], button_isr, IRQF_TRIGGER_RISING // | IRQF_DISABLED
			, "gpiomod#button2", NULL);
	
		if(ret) {
			printk(KERN_ERR "Unable to request IRQ: %d\n", ret);
			goto fail3;
		}	
		return 0;
	
	fail3:
		free_irq(button_irqs[0], NULL);
	
	fail2:
		gpio_free_array(buttons, ARRAY_SIZE(leds));
	
	fail1:
		gpio_free_array(leds, ARRAY_SIZE(leds));
	
		return ret;
		
	}
	
	static void __exit cleanup_tugas(void)
	{
		printk(KERN_INFO "Blink LED Module has been removed\n");
	
		int i;
	
		printk(KERN_INFO "%s\n", __func__);
		free_irq(button_irqs[0], NULL);
		free_irq(button_irqs[1], NULL);
		for(i = 0; i < ARRAY_SIZE(leds); i++) {
			gpio_set_value(leds[i].gpio, 0);
		}
	
		// unregister
		gpio_free_array(leds, ARRAY_SIZE(leds));
		gpio_free_array(buttons, ARRAY_SIZE(buttons));
	}
	
	
	MODULE_LICENSE("GPL");
	MODULE_AUTHOR(DRIVER_AUTHOR);
	MODULE_DESCRIPTION(DRIVER_DESC);
	module_init(init_tugas);
	module_exit(cleanup_tugas);

*/
