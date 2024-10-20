/*************************************************************************
	> File Name: hrtimer_dev.c
	> Author: Arctan
	> Created Time: 2024年10月20日 星期一 10时41分00秒
 ************************************************************************/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/hrtimer.h>

struct hrtimer_dev{
	struct device dev;
	struct module *owner;
	struct cdev char_dev;
	struct hrtimer rcu_timer;
};

struct hrtimer_dev * hrtimer;
enum hrtimer_restart rcu_hrtimer_test(struct hrtimer *timer)
{

	printk("----%s %d\n", __func__, __LINE__);
	while(1);	
	return HRTIMER_RESTART;
}
static int __init hrtimer_dev_init(void)
{
	printk("%s %d\n", __func__, __LINE__);
	hrtimer = (struct hrtimer_dev *)kmalloc(sizeof(struct hrtimer_dev), GFP_KERNEL);
	hrtimer_init(&hrtimer->rcu_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hrtimer->rcu_timer.function = rcu_hrtimer_test;
	hrtimer_start(&hrtimer->rcu_timer, 1, HRTIMER_MODE_REL);

	return 0;
}

static void __exit hrtimer_dev_exit(void)
{
	
}

module_init(hrtimer_dev_init);
module_exit(hrtimer_dev_exit);

MODULE_AUTHOR("Arctan");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("hrtimer dev demo");
