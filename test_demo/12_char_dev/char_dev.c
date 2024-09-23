/*************************************************************************
	> File Name: char_dev.c
	> Author: Arctan
	> Created Time: 2024年09月23日 星期一 10时41分00秒
 ************************************************************************/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define NUM_CHR_DEVICES	1

struct char_dev {
	struct class *chr_class;
	struct cdev cdev;
} *chr_dev;

static dev_t chr_major;

static ssize_t chrdev_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    printk(KERN_WARNING "%s %d buf %p\n", __func__, __LINE__, buf);
    return 0;
}

static ssize_t chrdev_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    printk(KERN_WARNING "%s %d buf %p %s\n", __func__, __LINE__, buf, buf);
    return count;
}

static int chrdev_open(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "%s %d\n", __func__, __LINE__);
	return 0;
}
static const struct file_operations chrdev_fops = {
	.owner		= THIS_MODULE,
	.read		= chrdev_read,
	.write		= chrdev_write,
	.open		= chrdev_open,
};


static int __init char_dev_init(void)
{
	int ret = 0;

	ret = alloc_chrdev_region(&chr_major, 0, 1, "chr_dev");
	if(ret)
	{
		goto out;
	}
	chr_major = MAJOR(chr_major);
	chr_dev = (struct char_dev *)kmalloc(sizeof(struct char_dev), GFP_KERNEL);
	if(chr_dev == NULL)
	{
		ret = -ENOMEM;
		goto out_mem;
	}

	cdev_init(&chr_dev->cdev, &chrdev_fops);
	chr_dev->cdev.owner = THIS_MODULE;	
	chr_dev->cdev.dev = MKDEV(chr_major, 0);
	ret = cdev_add(&chr_dev->cdev, chr_dev->cdev.dev, 1);

	chr_dev->chr_class = class_create(THIS_MODULE, "chr-dev");
	device_create(chr_dev->chr_class, NULL, chr_dev->cdev.dev, NULL,"chr-dev0");

	return 0;
out:
	printk(KERN_ERR "%s: Driver Initialisation failed\n", __FILE__);
out_mem:
    unregister_chrdev_region(MKDEV(chr_major, 0), 1);
	printk(KERN_ERR "%s: Driver Initialisation failed\n", __FILE__);
	return ret;
}

static void __exit char_dev_exit(void)
{
	dev_t devno =  MKDEV(chr_major, 0);

    device_destroy(chr_dev->chr_class, devno);
    class_destroy(chr_dev->chr_class);

    cdev_del(&chr_dev->cdev);
    unregister_chrdev_region(devno, 1);
	kfree(chr_dev);
	printk("char /dev exit driver\n");
	
}

module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_AUTHOR("Arctan");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("char dev demo");
