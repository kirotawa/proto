#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/uaccess.h>

#define BUFFER_LEN 100

static char msg[BUFFER_LEN] = {0};

static ssize_t proto_cdev_read(struct file *, char *, ssize_t, loff_t *);
static ssize_t proto_cdev_write(struct file *, const char *, ssize_t, loff_t *);

static const struct file_operations proto_cdev_fops = {
	.owner =  THIS_MODULE,
	.read  = proto_cdev_read,
	.write = proto_cdev_write,
};

static struct miscdevice  proto_cdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "proto_cdev",
	.fops = &proto_cdev_fops,
};

static ssize_t proto_cdev_read(struct file *f, char *buff, ssize_t count, loff_t
	*loff)
{
	int len = strlen(msg);

	/* read from kernel space to userspace */
	return simple_read_from_buffer(buff, count, loff, msg, len);
}

static ssize_t proto_cdev_write(struct file *f, const char *buff, ssize_t count,
		loff_t *loff)
{

	/* write from user space to kernel space */
	simple_write_to_buffer(msg, BUFFER_LEN, loff, buff, count);

	printk("count=%ld\n", count);
	return count;
}

static int __init proto_cdev_init(void)
{
	int rc = 0;

	pr_debug("Registering device...!\n");
	rc = misc_register(&proto_cdev);
	if (rc)
		pr_debug("Not able to register misc device\n");

	return rc;
}

static void proto_cdev_cleanup(void)
{
	misc_deregister(&proto_cdev);
	pr_debug("Deregistering device...\n");
}

module_init(proto_cdev_init);
module_exit(proto_cdev_cleanup);
MODULE_LICENSE("GPL");
