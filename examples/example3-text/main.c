#include <cx16.h>
#include "xist_gfx.h"
#include "xist_mem.h"
#include "xist_text.h"
#include "xist_tiles.h"
#include "xist_utils.h"

#include <string.h>

#define GRAPHICS_BANK 15

const char row0[XIST_TYPEWRITER_TEXT_COLUMNS] = { 87, 101, 32, 97, 114, 101, 32, 99, 108, 111, 115, 101, 32, 116, 111, 32, 104, 97, 118, 105, 110, 103, 32, 97, 108, 108, 32, 116, 104, 101, 0 };
const char row1[XIST_TYPEWRITER_TEXT_COLUMNS] = { 102, 117, 110, 100, 97, 109, 101, 110, 116, 97, 108, 32, 102, 117, 110, 99, 116, 105, 111, 110, 97, 108, 105, 116, 121, 32, 119, 101, 32, 110, 101, 101, 100, 0 };
const char row2[XIST_TYPEWRITER_TEXT_COLUMNS] = { 102, 111, 114, 32, 97, 32, 117, 115, 97, 98, 108, 101, 32, 67, 111, 109, 109, 97, 110, 100, 101, 114, 32, 88, 49, 54, 32, 103, 114, 97, 112, 104, 105, 99, 115, 0 };
const char row3[XIST_TYPEWRITER_TEXT_COLUMNS] = { 108, 105, 98, 114, 97, 114, 121, 33, 0 };
const char row4[XIST_TYPEWRITER_TEXT_COLUMNS] = { 76, 101, 116, 39, 115, 32, 109, 97, 107, 101, 32, 99, 114, 101, 97, 116, 105, 110, 103, 32, 103, 97, 109, 101, 115, 32, 102, 111, 114, 32, 97, 0 };
const char row5[XIST_TYPEWRITER_TEXT_COLUMNS] = { 109, 111, 100, 101, 114, 110, 45, 114, 101, 116, 114, 111, 32, 99, 111, 109, 112, 117, 116, 101, 114, 47, 103, 97, 109, 101, 32, 99, 111, 110, 115, 111, 108, 101, 0 };
const char row6[XIST_TYPEWRITER_TEXT_COLUMNS] = { 98, 111, 116, 104, 32, 102, 117, 110, 32, 97, 110, 100, 32, 97, 99, 99, 101, 115, 115, 105, 98, 108, 101, 33, 0 };
const char row7[XIST_TYPEWRITER_TEXT_COLUMNS] = { 66, 101, 104, 111, 108, 100, 32, 116, 104, 101, 32, 112, 111, 119, 101, 114, 32, 111, 102, 32, 88, 105, 115, 116, 49, 54, 33, 0 };
const char row8[XIST_TYPEWRITER_TEXT_COLUMNS] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 40, 94, 95, 94, 41, 86, 0 };
const char row9[XIST_TYPEWRITER_TEXT_COLUMNS] = { 40, 126, 95, 94, 41, 32, 32, 61, 95, 61, 32, 32, 62, 46, 62, 32, 32, 60, 46, 60, 32, 32, 58, 80, 0 };

char fixed_str[19] = { 66, 111, 114, 105, 110, 103, 32, 116, 101, 115, 116, 32, 115, 116, 114, 105, 110, 103, 0 };
char fixed_str2[20] = { 73, 39, 118, 101, 32, 98, 101, 101, 110, 32, 114, 101, 112, 108, 97, 99, 101, 100, 33, 0 };
char clear_counter[MAX_SIGNED_INT_CHARS] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 0 };

void main() {
  unsigned short n;
  signed long game_counter = 0;
  char game_counter_str[MAX_SIGNED_INT_CHARS];
  
  // Xist initialization
  xist_initialize_tiles();
  xist_initialize_text_tiles();
  xist_load_file_to_highram("gfxmeta.dat", XIST_SPRITE_METADATA_BANK);
  
  // Load image that contains the background tile we want
  xist_load_graphic(0, TRUE, GRAPHICS_BANK, FALSE, 0);
  
  // Copy deep blue tile into VRAM
  xist_copy_highram_to_vram_using_metadata(XIST_LAYER_0_TILE_BASE, GRAPHICS_BANK, 2);
  
  // Fill the background with this tile
  for (n = 0; n < LAYER_0_MAPBASE_TOTAL; ++n) xist_map_tiles[n] = 0;
  VERA.address = XIST_LAYER_0_MAPBASE;
  VERA.address_hi = XIST_LAYER_0_MAPBASE>>16;
  VERA.address_hi |= 0b10000;
  for (n = 0; n < LAYER_0_MAPBASE_TOTAL; ++n) {
    VERA.data0 = xist_map_tiles[n];
    VERA.data0 = 0;
  }
  
  // Text display test
  memcpy(xist_typewriter_text[0], row0, sizeof(row0));
  memcpy(xist_typewriter_text[1], row1, sizeof(row1));
  memcpy(xist_typewriter_text[2], row2, sizeof(row2));
  memcpy(xist_typewriter_text[3], row3, sizeof(row3));
  memcpy(xist_typewriter_text[4], row4, sizeof(row4));
  memcpy(xist_typewriter_text[5], row5, sizeof(row5));
  memcpy(xist_typewriter_text[6], row6, sizeof(row6));
  memcpy(xist_typewriter_text[7], row7, sizeof(row7));
  memcpy(xist_typewriter_text[8], row8, sizeof(row8));
  memcpy(xist_typewriter_text[9], row9, sizeof(row9));
  xist_reset_typewriter_counters();

  xist_text = fixed_str;
  xist_text_tile_x = 3;
  xist_text_tile_y = 20;
  xist_draw_text();
  
  for(EVER) {
    xist_wait();
    
    if (game_counter > 180) {
      xist_tw_text_tile_x = 3;
      xist_tw_text_tile_y = 3;
      xist_draw_typewriter_text();
    }
    if (game_counter == 900) xist_clear_text();
    if (game_counter == 1080) {
      xist_text = fixed_str2;
      xist_text_tile_x = 3;
      xist_text_tile_y = 20;
      xist_draw_text();
    }
    
    xist_convert_long_to_ascii_array(game_counter, game_counter_str);
    
    xist_text = clear_counter;
    xist_text_tile_x = 34;
    xist_text_tile_y = 28;
    xist_draw_text();
    
    xist_text = game_counter_str;
    xist_draw_text(game_counter_str);
    
    ++game_counter;
  }
}