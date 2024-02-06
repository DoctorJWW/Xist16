#include <cbm.h>
#include "xist_gfx.h"
#include "xist_mem.h"

void xist_load_file_to_highram(const char *filename, unsigned char bank) {  
  BANK_NUM = bank;
  cbm_k_setnam(filename);
  cbm_k_setlfs(0, 8, 2);
  cbm_k_load(0, (unsigned short)BANK_RAM);
}


void xist_load_file_to_vram(const char *filename,
                            BOOL to_upper,
                            unsigned long vram_address)
{  
  cbm_k_setnam(filename);
  cbm_k_setlfs(0, 8, 2);
  if (to_upper) {
    cbm_k_load(3, vram_address);
  }
  else {
    cbm_k_load(2, vram_address);
  }
}


void xist_copy_highram_to_vram(
  unsigned long vram_address,
  unsigned char bank,
  unsigned short mem_offset,
  unsigned short num_bytes)
{
  VERA.address = vram_address;
  VERA.address_hi = vram_address>>16;
  VERA.address_hi |= 0b10000; // Increment mode

  BANK_NUM = bank;

  R0 = (unsigned short)BANK_RAM + mem_offset;
  R1 = 0x9F23;
  R2 = num_bytes;

  __asm__("jsr $FEE7");
}


void xist_copy_highram_to_vram_partial(
  unsigned long vram_address,
  unsigned char highram_bank,
  unsigned char x_offset,
  unsigned char y_offset,
  unsigned char gfx_width,
  unsigned char gfx_height,
  unsigned char h_width)
{
  unsigned short bytes_copied = 0;
  unsigned short bytes_to_copy = gfx_width * gfx_height;
  unsigned short read_offset = (y_offset * h_width) + x_offset;
  
  VERA.address = vram_address;
  VERA.address_hi = vram_address>>16;
  VERA.address_hi |= 0b10000; // increment mode

  while (bytes_copied < bytes_to_copy) {
    BANK_NUM = highram_bank;
    R0 = (unsigned short)BANK_RAM + read_offset;
    R1 = 0x9F23;
    R2 = gfx_width; // bytes to copy
    __asm__("jsr $FEE7");
    bytes_copied += gfx_width;
    read_offset += gfx_width + (h_width - gfx_width);
  }
}


void xist_copy_highram_to_vram_using_metadata(unsigned long vram_address,
                                              unsigned char highram_bank,
                                              unsigned short metadata_index)
{
  unsigned short *gfx_metadata_address;
  struct XistGraphicsMetadata *gfx_metadata;
  unsigned short address_offset = 0;
  unsigned char current_x;
  unsigned char current_y;
  unsigned char n;
  unsigned char metadata_size = sizeof(struct XistGraphicsMetadata);
  
  // data we need to copy locally since the high-RAM bank will get overwritten during the graphical copy
  unsigned char gfx_frames, gfx_width, gfx_height, gfx_total_width;
  
  BANK_NUM = XIST_SPRITE_METADATA_BANK;
  gfx_metadata_address = (unsigned short *)(XIST_BANKED_RAM_ADDRESS + (metadata_size * metadata_index));
  gfx_metadata = (struct XistGraphicsMetadata *)gfx_metadata_address;
  
  gfx_frames = gfx_metadata->num_frames;
  gfx_width = gfx_metadata->width;
  gfx_height = gfx_metadata->height;
  gfx_total_width = gfx_metadata->total_width;
  current_x = gfx_metadata->x_offset;
  current_y = gfx_metadata->y_offset;
    
  // copy the sprite's frames into contiguous memory in VRAM
  for (n = 0; n < gfx_frames; ++n) {
    xist_copy_highram_to_vram_partial(vram_address + address_offset,
                                      highram_bank,
                                      current_x,
                                      current_y,
                                      gfx_width,
                                      gfx_height,
                                      gfx_total_width);
    address_offset += (gfx_width * gfx_height);
    
    current_x += gfx_width;
    if (current_x >= gfx_total_width) {
      current_x = 0;
      current_y += gfx_height;
    }
  }
}
