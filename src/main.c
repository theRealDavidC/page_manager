#include <stdio.h>
#include "page_manager.h"
#include <string.h>


int main() {
  PageManager pm;
  PMStatus s;
  uint8_t buf[PAGE_SIZE];
  
  s = page_open("test.db", &pm);
  if (s != PM_OK) {
   printf("open failed: %d\n", s); 
   return 1; 
   }
   
  printf("opened. page_count = %u\n", pm.page_count);
  
  uint32_t page_num;
  s = page_alloc(&pm, &page_num);
  if(s != PM_OK) {
    printf("alloc failed: %d\n", s);
    return 1;
    }
   
  printf("allocated page %u\n", page_num);
  
  memset(buf, 0, PAGE_SIZE);
  
  PageHeader ph;
  ph.page_number = page_num;
  ph.free_space_offset = 8;
  ph.slot_count = 0;
  
  memcpy(buf, &hd, sizeof(PageHeader));
  
  s = page_write(&pm, page_num, buf);
  if(s != PM_OK) {
	  printf("failed to write page: %d\n", s);
	  return 1;
  }
  
  printf("wrote page %u\n", page_num);
  
  memset(buf, 0, 0);
  
  s = page_read(&pm, page_num, buf);
  if(s != PM_OK) {
	  print("Read failed: %d\n" s);
	  return 1;
  }
  
  printf("Read page %u\n", page_num);
  printf("Page number: %u\n", ph.page_number);
  printf("Free space offset on the read is: %u\n", ph.free_space_offset);
  printf("Slot count based on our read is: %u\n", ph.slot_count);
  
  void closed = page_close(&pm);
  printf("Page manager closed");
  
  s = page_open("test.db", &pm);
  if (s != PM_OK) {
   printf("open failed: %d\n", s); 
   return 1; 
   }
   
  printf("%d recovered", pm.page_count);
  
  page_close(&pm);
}
  
  
  
