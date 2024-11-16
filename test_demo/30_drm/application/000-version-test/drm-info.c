/*************************************************************************
	> File Name: 001_singleb-uffer.c
	> Author: Arctan
	> Created Time: Sat 16 Nov 2024 02:57:41 PM CST
 ************************************************************************/
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>


int main(int argc, char **argv)
{
	int fd;
	drmVersionPtr retval;
	drm_version_t *version;
	
	version = drmMalloc(sizeof(*version));

	fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);

	drmIoctl(fd, DRM_IOCTL_VERSION, version);

	printf("the verson %s\n", version->name);
	close(fd);
	return 0;
}
