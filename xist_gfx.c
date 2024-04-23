#include <cx16.h>
#include "xist_gfx.h"
#include "xist_mem.h"
#include "xist_utils.h"

unsigned char xist_proc_sprite_start_idx, xist_proc_sprite_end_idx, xist_curr_sprite_idx;
signed short xist_curr_sprite_x_pos, xist_curr_sprite_y_pos;
unsigned char xist_curr_sprite_byte_6;
struct XistAnimation xist_sprite_bank;

void xist_process_sprites() {
  unsigned long sprite_address;
  unsigned char n;
  
  for (n = xist_proc_sprite_start_idx; n <= xist_proc_sprite_end_idx; ++n) {
    if (! (xist_sprite_bank.bitmask[n] & XIST_SPRITE_ACTIVE)) return;
    
    // frame processing
    if (xist_sprite_bank.num_frames[n] > 1) {
      ++xist_sprite_bank.frame_timer[n];
      if (xist_sprite_bank.frame_timer[n] > xist_sprite_bank.frame_delay[n]) {
        xist_sprite_bank.frame_timer[n] = 0;
        ++xist_sprite_bank.current_frame[n];
        
        if (xist_sprite_bank.current_frame[n] >= xist_sprite_bank.num_frames[n]) {
          if (xist_sprite_bank.bitmask[n] & XIST_SPRITE_REPEATS) {
            xist_sprite_bank.current_frame[n] = 0;
          }
          else {
            xist_sprite_bank.bitmask[n] |= XIST_SPRITE_FINISHED;
            xist_sprite_bank.current_frame[n] = xist_sprite_bank.num_frames[n] - 1;
          }
        }
        
        // if the frame has changed, update its address in VRAM
        sprite_address = XIST_VRAM_SPRITE_0_ADDRESS + (n * 8);
        VERA.address = sprite_address;
        VERA.address_hi = sprite_address>>16;
        VERA.address_hi |= 0b10000;
        // set sprite parameters
        sprite_address = xist_sprite_bank.first_frame_address[n] +
          (xist_sprite_bank.current_frame[n] * xist_sprite_bank.address_offset[n]);
        VERA.data0 = sprite_address>>5;
        VERA.data0 = 0b10000000 | sprite_address>>13; // first part sets 256-color mode
      }
    }
  }
}


// This function loads a sprite's graphical data into high-RAM or VRAM based on its metadata
void xist_load_graphic(unsigned short metadata_index,
                       BOOL to_highram,
                       unsigned char to_highram_bank,
                       BOOL to_upper_vram,
                       unsigned long vram_address)
{
  unsigned char metadata_size = sizeof(struct XistGraphicsMetadata);
  unsigned short *gfx_data_address;
  struct XistGraphicsMetadata *gfx_metadata;
  char file_extension[] = { 46, 105, 109, 103 };
  char combined_filename[9]; // 4 for filename, 4 for extension, 1 for null terminator
  unsigned char n;
  
  BANK_NUM = XIST_SPRITE_METADATA_BANK;
  gfx_data_address = (unsigned short *)(XIST_BANKED_RAM_ADDRESS + (metadata_size * metadata_index));
  
  // load requested sprite metadata
  gfx_metadata = (struct XistGraphicsMetadata *)gfx_data_address;
  
  // create the filename of the graphic file to load the sprite from
  for (n = 0; n < 4; ++n) { // 4 = filename length
    combined_filename[n] = gfx_metadata->filename[n];
  }
  for (n = 0; n < sizeof(file_extension); ++n) {
    combined_filename[4 + n] = file_extension[n]; // 4 = filename length
  }
  combined_filename[8] = 0;
  
  // load the graphic file into RAM
  if (to_highram) xist_load_file_to_highram(combined_filename, to_highram_bank);
  else xist_load_file_to_vram(combined_filename, to_upper_vram, vram_address);
}


// This function copies a sprite's graphical data from high-RAM into VRAM based on its metadata,
// and then prepares the sprite data in VRAM for use.
// add_bitmask turns on additional flags in addition to any that are on by default from the metadata.
// The most common use case would be SPRITE_ACTIVE (0b00000001) to make it immediately active for use.
// Give vram_address a value of 0 to skip copying the graphics from high-RAM to VRAM
void xist_prepare_sprite(unsigned short metadata_index,
                         BOOL copy_to_vram,
                         unsigned long vram_address,
                         unsigned char highram_bank,
                         unsigned char sprite_index,
                         signed short x_pos,
                         signed short y_pos,
                         unsigned char add_bitmask)
{
  unsigned char n = 0;
  unsigned char metadata_size = sizeof(struct XistGraphicsMetadata);
  unsigned short *gfx_metadata_address;
  unsigned short address_offset = 0;
  unsigned char current_x;
  unsigned char current_y;
  struct XistGraphicsMetadata *gfx_metadata;
  unsigned long sprite_vram_index_address;
  
  // data we need to copy locally since the high-RAM bank will get overwritten during the graphical copy
  unsigned char sprite_frames, sprite_width, sprite_height, sprite_total_width;
  
  BANK_NUM = XIST_SPRITE_METADATA_BANK;
  gfx_metadata_address = (unsigned short *)(XIST_BANKED_RAM_ADDRESS + (metadata_size * metadata_index));
  
  // load requested sprite metadata (assume sprite metadata already loaded into local bank)
  gfx_metadata = (struct XistGraphicsMetadata *)gfx_metadata_address;
  
  sprite_frames = gfx_metadata->num_frames;
  sprite_width = gfx_metadata->width;
  sprite_height = gfx_metadata->height;
  sprite_total_width = gfx_metadata->total_width;
  current_x = gfx_metadata->x_offset;
  current_y = gfx_metadata->y_offset;
  
  // set up the sprite data in the sprite bank (in low-RAM)
  xist_sprite_bank.first_frame_address[sprite_index] = vram_address;
  xist_sprite_bank.address_offset[sprite_index] = sprite_width * sprite_height;
  xist_sprite_bank.num_frames[sprite_index] = sprite_frames;
  xist_sprite_bank.current_frame[sprite_index] = 0;
  xist_sprite_bank.frame_delay[sprite_index] = gfx_metadata->frame_delay;
  xist_sprite_bank.frame_timer[sprite_index] = 0;
  xist_sprite_bank.bitmask[sprite_index] = gfx_metadata->bitmask;
  xist_sprite_bank.bitmask[sprite_index] |= add_bitmask;
  xist_sprite_bank.x_pos[sprite_index] = x_pos;
  xist_sprite_bank.y_pos[sprite_index] = y_pos;
  
  // set up the sprite data in VRAM
  sprite_vram_index_address = XIST_VRAM_SPRITE_0_ADDRESS + ((unsigned long)sprite_index * 8UL);
  VERA.address = sprite_vram_index_address;
  VERA.address_hi = sprite_vram_index_address>>16;
  VERA.address_hi |= 0b10000;
  // set sprite parameters
  VERA.data0 = vram_address>>5;
  VERA.data0 = 0b10000000 | vram_address>>13; // first part sets 256-color mode
  VERA.data0 = x_pos;
  VERA.data0 = x_pos >>8;
  VERA.data0 = y_pos;
  VERA.data0 = y_pos >>8;
  VERA.data0 = gfx_metadata->byte_6;
  VERA.data0 = gfx_metadata->byte_7;
  
  // copy the sprite's frames into contiguous memory in VRAM
  if (copy_to_vram) {
    for (n = 0; n < sprite_frames; ++n) {
      xist_copy_highram_to_vram_partial(vram_address + address_offset,
                                        highram_bank,
                                        current_x,
                                        current_y,
                                        sprite_width,
                                        sprite_height,
                                        sprite_total_width);
      address_offset += (sprite_width * sprite_height);
      
      current_x += sprite_width;
      if (current_x >= sprite_total_width) {
        current_x = 0;
        current_y += sprite_height;
      }
    }
  }
}


void xist_update_sprite_position() {
  unsigned long sprite_address = XIST_VRAM_SPRITE_0_ADDRESS + (xist_curr_sprite_idx * 8) + 2;
  VERA.address = (sprite_address);
  VERA.address_hi = (sprite_address)>>16;
  VERA.address_hi |= 0b10000;
  VERA.data0 = xist_curr_sprite_x_pos;
  VERA.data0 = xist_curr_sprite_x_pos>>8;
  VERA.data0 = xist_curr_sprite_y_pos;
  VERA.data0 = xist_curr_sprite_y_pos>>8;
}


void xist_update_sprite_parameters() {
  unsigned long sprite_address = XIST_VRAM_SPRITE_0_ADDRESS + (xist_curr_sprite_idx * 8) + 6;
  VERA.address = (sprite_address);
  VERA.address_hi = (sprite_address)>>16;
  VERA.address_hi |= 0b10000;
  VERA.data0 = xist_curr_sprite_byte_6;
}
