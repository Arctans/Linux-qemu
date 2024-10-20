/*************************************************************************
	> File Name: rcu_test.c
	> Author: Arctan
	> Created Time: 2024年10月20日 星期一 15时16分00秒
 ************************************************************************/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kthread.h>
#include <linux/rcupdate.h>
#include <linux/delay.h>

struct rcu_device{
	struct device dev;
	struct module *owner;
	struct cdev char_dev;
	int a; 
	struct rcu_head rcu; 
};
static struct rcu_device *g_ptr;

static void rcu_del(struct rcu_head *rh)
{
	struct rcu_device *ptr = container_of(rh, struct rcu_device, rcu);
	printk("%s: a=%d\n", __func__, ptr->a);
}

static int rcu_reader_thread(void *data)
{
	struct rcu_device *ptr = NULL;
	while(1)
	{
		msleep(200);
		rcu_read_lock();
		ptr = rcu_dereference(g_ptr);
		if(ptr)
			printk("%s: read a=%d\n", __func__, ptr->a);
		rcu_read_unlock();
	}
	return 0;
}

static int rcu_writer_thread(void *data)
{
	struct rcu_device *new;
	struct rcu_device *old;
	int value = (unsigned long)data;
	while(1)
	{
		msleep(400);
		struct rcu_device *new = (struct rcu_device*)kmalloc(sizeof(struct rcu_device), GFP_KERNEL);
		old = g_ptr;
		printk("%s: write to new %d\n", __func__, value);
		/* *new = *old; */
		new->a = value;
		rcu_assign_pointer(g_ptr, new);
		call_rcu(&old->rcu, rcu_del);
		value++;
	}
	return 0;
}

static int __init rcu_dev_init(void)
{
	struct task_struct *reader_thread = NULL;
	struct task_struct *writer_thread = NULL;
	int value = 5;

	printk("%s %d\n", __func__, __LINE__);
	g_ptr = (struct rcu_device*)kmalloc(sizeof(struct rcu_device), GFP_KERNEL);
	writer_thread = kthread_run(rcu_writer_thread, (void *)(unsigned long )value, "rcu_writer");
	reader_thread = kthread_run(rcu_reader_thread, NULL, "rcu_reader");

	return 0;
}

static void __exit rcu_dev_exit(void)
{
	kfree(g_ptr);	
}

module_init(rcu_dev_init);
module_exit(rcu_dev_exit);

MODULE_AUTHOR("Arctan");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("rcu dev demo");
