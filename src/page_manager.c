#include "page_manager.h"
#include <fcntl.h> /* open(), O_RDWR, O_CREAT, O_RDONLY */
#include <unistd.h> /* pread(), pwrite(), close(), fsync() */
#include <string.h> /* memset(), memcpy() */
#include <errno.h> /* errno, ENOENT */
#include <stdint.h> /* uint8_t for the raw byte buffer */

PMStatus page_open(const char *path, PageManager *pm) {

    int fd = open(path, O_RDWR);

    if (fd == -1) {
        if (errno != ENOENT) {
            return PM_ERR_OPEN;
        }

        fd = open(path, O_RDWR | O_CREAT, 0644);
        if (fd == -1) return PM_ERR_OPEN;

        uint8_t buf[PAGE_SIZE];
        memset(buf, 0, PAGE_SIZE);

        uint32_t magic      = PM_MAGIC;
        uint16_t page_size  = PAGE_SIZE;
        uint32_t page_count = 0;              

        memcpy(buf + 0, &magic,      4);
        memcpy(buf + 4, &page_size,  2);
        memcpy(buf + 6, &page_count, 4);

        ssize_t written = pwrite(fd, buf, PAGE_SIZE, 0);
        if (written != PAGE_SIZE) { close(fd); return PM_ERR_IO; }

        pm->fd         = fd;
        pm->page_count = 0;                 
        return PM_OK;
    }

    uint8_t buf[PAGE_SIZE];
    ssize_t n = pread(fd, buf, PAGE_SIZE, 0);
    if (n != PAGE_SIZE) { close(fd); return PM_ERR_IO; }

    uint32_t magic;
    uint16_t page_size;

    memcpy(&magic,     buf + 0, 4);
    memcpy(&page_size, buf + 4, 2);

    if (magic != PM_MAGIC)      { close(fd); return PM_ERR_CORRUPT; }
    if (page_size != PAGE_SIZE) { close(fd); return PM_ERR_CORRUPT; }

    off_t file_size    = lseek(fd, 0, SEEK_END);
    pm->page_count     = file_size / PAGE_SIZE;

    pm->fd = fd;
    return PM_OK;
}
