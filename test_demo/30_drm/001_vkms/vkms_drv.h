/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef _VKMS_DRV_H_
#define _VKMS_DRV_H_

#include <linux/hrtimer.h>

#include <drm/drm.h>
#include <drm/drm_gem.h>
#include <drm/drm_encoder.h>
#include <drm/drm_writeback.h>

#define XRES_MIN    20
#define YRES_MIN    20

#define XRES_DEF  1024
#define YRES_DEF   768

#define XRES_MAX  8192
#define YRES_MAX  8192

extern bool enable_cursor;

struct vkms_device {
	struct drm_device drm;
	struct platform_device *platform;
};

#endif /* _VKMS_DRV_H_ */
