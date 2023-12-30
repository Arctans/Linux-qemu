/*************************************************************************
	> File Name: slab_test.c
	> Author: Arctan
	> Created Time: 2023年12月30日 星期六 09时25分21秒
 ************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

struct student{
	int id;
	int age;
	float score;
	void (*print_score)(int id);
	void (*print_age)(int id);

};
/* }__attribute__((packed)); */
struct kmem_cache *stu_cache;
struct student *p;

void stu_ctor(void *p)
{
	;
}

static int __init slab_test_init(void)
{
	printk("sizeof student = %d score %d\n", sizeof(struct student), sizeof(float));
	stu_cache = kmem_cache_create("student", sizeof(struct student), 0, \
									SLAB_PANIC | SLAB_ACCOUNT, stu_ctor);
	BUG_ON( stu_cache == NULL);
	printk("stu_cache = %x\n", (unsigned int)&stu_cache);
	
	p = kmem_cache_alloc(stu_cache, GFP_KERNEL);
	if(p){
		printk("p object size = %lx\n", sizeof(*p));
		printk("p object size = %ld\n", sizeof(struct student));
	}
	return 0;
}
static void __exit slab_test_exit(void)
{
	kmem_cache_free(stu_cache, p);
	kmem_cache_destroy(stu_cache);
}
module_init(slab_test_init);
module_exit(slab_test_exit);

MODULE_AUTHOR("Arctan");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("slab demo");
