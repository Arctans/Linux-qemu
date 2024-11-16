// SPDX-License-Identifier: GPL-2.0+

/**
 * DOC: vkms (Virtual Kernel Modesetting)
 *
 * VKMS is a software-only model of a KMS driver that is useful for testing
 * and for running X (or similar) on headless machines. VKMS aims to enable
 * a virtual display with no need of a hardware display capability, releasing
 * the GPU in DRM API tests.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

#include <drm/drm_gem.h>
#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_file.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_ioctl.h>
#include <drm/drm_managed.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_vblank.h>

#include "vkms_drv.h"

#define DRIVER_NAME	"vkms"
#define DRIVER_DESC	"Virtual Kernel Mode Setting"
#define DRIVER_DATE	"20241114"
#define DRIVER_MAJOR	1
#define DRIVER_MINOR	0

static struct vkms_device *vkms_device;

static const struct file_operations vkms_driver_fops = {
	.open			= drm_open,
};
static struct drm_driver vkms_driver = {
	.name			= "vkms",
	.desc			= "Virtual Kernel Mode Setting",
	.date			= "20241114",
	.major			= 1,
	.minor			= 1,
	.fops			= &vkms_driver_fops,

};

static int __init vkms_init(void)
{
	int ret;
	struct platform_device *pdev;

	printk("%s %d\n", __func__, __LINE__);
	pdev = platform_device_register_simple(DRIVER_NAME, -1, NULL, 0);
	vkms_device = devm_drm_dev_alloc(&pdev->dev, &vkms_driver,
					 struct vkms_device, drm);
	vkms_device->platform = pdev;
	ret = drm_dev_register(&vkms_device->drm, 0);
	if (ret)
		return -1;
	return 0;
}

static void __exit vkms_exit(void)
{
	struct platform_device *pdev;
	if(!vkms_device){
		DRM_INFO("vkms_device is NULL.\n");
		return;
	}
	pdev = vkms_device->platform;
	drm_dev_unregister(&vkms_device->drm);
	platform_device_unregister(pdev);
}
module_init(vkms_init);
module_exit(vkms_exit);

MODULE_AUTHOR("Arctan");
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
