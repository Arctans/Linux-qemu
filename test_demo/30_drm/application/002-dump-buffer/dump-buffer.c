/*************************************************************************
	> File Name: dump-buffer.c
	> Author: Arctan
	> Created Time: Sat 16 Nov 2024 02:57:41 PM CST
 ************************************************************************/
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>


int main(int argc, char **argv)
{
        int fd;
        char *vaddr;
        struct drm_mode_create_dumb create_req = {};
        struct drm_mode_destroy_dumb destroy_req = {};
        struct drm_mode_map_dumb map_req = {};
		int  card_num = 0;
		char card_name[20] = {1};

		if(argc == 2)
		{
			card_num = atoi(argv[1]);
		}

		sprintf(card_name, "/dev/dri/card%d", card_num);
		
        fd = open((char*)card_name, O_RDWR);
		if(fd < 0)
		{
			printf("open %s failed, %s\n", card_name, strerror(fd));
			exit(-1);
		}

        create_req.bpp = 32;
        create_req.width = 240;
        create_req.height = 320;
        drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &create_req);
        printf("create dumb: handle = %u, pitch = %u, size = %llu\n",
                create_req.handle, create_req.pitch, create_req.size);

        map_req.handle = create_req.handle;
        drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &map_req);
        printf("get mmap offset 0x%llx\n", map_req.offset);

        vaddr = mmap(0, create_req.size, PROT_WRITE, MAP_SHARED, fd, map_req.offset);
        strcpy(vaddr, "This is a dumb buffer!");
        munmap(vaddr, create_req.size);

        vaddr = mmap(0, create_req.size, PROT_READ, MAP_SHARED, fd, map_req.offset);
        printf("read from mmap: %s\n", vaddr);
        munmap(vaddr, create_req.size);

        getchar();

        destroy_req.handle = create_req.handle;
        drmIoctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &destroy_req);
        close(fd);

        return 0;
}
