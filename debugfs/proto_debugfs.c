#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h>

struct dentry *file = NULL;
struct dentry *proto_debugfs = NULL;

static u8 count = 0;

int __init proto_debugfs_init(void)
{
	struct dentry *a; 

	proto_debugfs = debugfs_create_dir("proto_debugfs", NULL);
	if (!proto_debugfs)
		return -ENOENT;

	a = debugfs_create_u8("count", 0644, proto_debugfs, &count);
	if (!a)
		goto err;
	 	
	return 0;
err:
	debugfs_remove_recursive(proto_debugfs);
	proto_debugfs = NULL;
	return -ENOENT;
}

void __exit proto_debugfs_exit(void)
{
	debugfs_remove_recursive(proto_debugfs);
	printk("Leaving...\n");	
}

module_init(proto_debugfs_init);
module_exit(proto_debugfs_exit);
MODULE_LICENSE("GPL");
