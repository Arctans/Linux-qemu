/*************************************************************************
	> File Name: vmalloc.c
	> Author: Arctan
	> Created Time: 2023年07月03日 星期四 15时43分09秒
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/mm.h>

unsigned int *p = NULL;
unsigned int *q = NULL;
#define BUFFER_SIZE (150)

static int __init kmemleak_test_init(void)
{
	int i;
	q = kmalloc(BUFFER_SIZE,GFP_KERNEL);
	q = kmalloc(BUFFER_SIZE,GFP_KERNEL);
	q = kmalloc(BUFFER_SIZE,GFP_KERNEL);
	if(!q){
		printk("kmalloc failed\n");
		return -ENOMEM;
	}
	return 0;
}
static void __exit kmemleak_test_exit(void)
{
	kfree(q);
}
module_init(kmemleak_test_init);
module_exit(kmemleak_test_exit);

MODULE_AUTHOR("Arctan");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("kmalloc/kfree demo");
