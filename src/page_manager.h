#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include <stdint.h>
#include <unistd.h>

#define PAGE_SIZE 8192
#define PM_MAGIC 0x504D4442

typedef struct {
 int fd;
 uint32_t page_count;
 } PageManager;
 
typedef struct __attribute__((packed)) {
    uint32_t page_number;
    uint16_t free_space_offset;
    uint16_t slot_count;
} PageHeader;
 
typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint16_t page_size;
    uint32_t page_count;
} FileHeader;

typedef struct __attribute__((packed)) {
 uint16_t offset;
 uint16_t length;
 } Slot;
 
typedef enum {
 PM_OK = 0,
 PM_ERR_OPEN = 1,
 PM_ERR_IO = 2,
 PM_ERR_CORRUPT = 3,
 PM_ERR_ARG = 4
 } PMStatus;
 
PMStatus page_open  (const char *path, PageManager *pm);
PMStatus page_read  (PageManager *pm, uint32_t page_num, void *buf);
PMStatus page_write (PageManager *pm, uint32_t page_num, const void *buf);
PMStatus page_alloc (PageManager *pm, uint32_t *page_num);
PMStatus page_flush (PageManager *pm);
void     page_close (PageManager *pm);

#endif
