/*************************************************************************
	> File Name: kmap.c
	> Author: Arctan
	> Created Time: 2023年01月27日 星期五 11时34分24秒
	>
	> 运行结果如下所示:
	> user/kernel 3:1 
	> 内存分布如下:
	> [    0.000000] Virtual kernel memory layout:
    >              vector  : 0xffff0000 - 0xffff1000   (   4 kB)
    >              fixmap  : 0xffc00000 - 0xfff00000   (3072 kB)
    >              vmalloc : 0xf0800000 - 0xff800000   ( 240 MB)
    >              lowmem  : 0xc0000000 - 0xf0000000   ( 768 MB)
    >              pkmap   : 0xbfe00000 - 0xc0000000   (   2 MB)
    >              modules : 0xbf000000 - 0xbfe00000   (  14 MB)
    >                .text : 0x(ptrval) - 0x(ptrval)   (8160 kB)
    >                .init : 0x(ptrval) - 0x(ptrval)   (1024 kB)
    >                .data : 0x(ptrval) - 0x(ptrval)   ( 586 kB)
    >                 .bss : 0x(ptrval) - 0x(ptrval)   ( 206 kB)
	> ]
	> root@runninglinuxkernel:~# insmod /mnt/kmap.ko
	> kmap: loading out-of-tree module taints kernel.
	> low phys_addr:6174a000  virt_addr:c174a000
	> high phys_addr:9eef8000  virt_addr:bfe77000
	> root@runninglinuxkernel:~#
	> 高端的内存 物理地址9eef8000 并且不是线性映射
	> *** 可以看到如果你申请的物理页是低端内存她直接采用的是线性映射，并没有
	> *** 使用pkmap 的虚拟地址，当你使用的是高端内存页是才会使用pkmap 的虚拟
	> *** 地址进行映射
 ************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/highmem.h>

struct page *page_lowmem,*page_highmem;
void *virt_addr;
unsigned int phys_addr;

static int __init hello_init(void)
{
	page_lowmem =alloc_page(GFP_KERNEL);
	if(!page_lowmem)
	{
		printk("lowmem alloc page failed\n");
	}
	virt_addr = kmap(page_lowmem);
	phys_addr = __page_to_pfn(page_lowmem) << 12;
	printk("low phys_addr:%x  virt_addr:%x\n",phys_addr,(unsigned int)virt_addr);

	page_highmem =alloc_page(__GFP_HIGHMEM);
	if(!page_highmem)
	{
		printk("highmem alloc page failed\n");
	}
	virt_addr = kmap(page_highmem);
	phys_addr = __page_to_pfn(page_highmem) << 12;
	printk("high phys_addr:%x  virt_addr:%x\n",phys_addr,(unsigned int)virt_addr);

	return 0;	
}

static void __exit hello_exit(void)
{
	kunmap(page_lowmem);
	kunmap(page_highmem);
	__free_page(page_lowmem);
	__free_page(page_highmem);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Arctan");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("kmap demo");
