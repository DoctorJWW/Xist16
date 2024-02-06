#include <cx16.h>
#include <cbm.h>
#include "xist_gfx.h"
#include "xist_input.h"
#include "xist_mem.h"
#include "xist_text.h"
#include "xist_tiles.h"
#include "xist_utils.h"

signed long game_counter = 0;
unsigned short second_counter = 0;
char game_counter_str[MAX_SIGNED_INT_CHARS];
char second_counter_str[MAX_SIGNED_INT_CHARS];
char clear_counter[MAX_SIGNED_INT_CHARS] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 0 };

char camera_x_str[MAX_SIGNED_INT_CHARS];
char camera_y_str[MAX_SIGNED_INT_CHARS];

unsigned short current_joypad_state;


void scroll_map() {
  // mouse version
  /*
  xist_update_mouse_position();
  if (*xist_mouse_x > XIST_RESOLUTION_WIDTH - 10) xist_camera_change_x = 10;
  else if (*xist_mouse_x < 10) xist_camera_change_x = -10;
  if (*xist_mouse_y > XIST_RESOLUTION_HEIGHT - 10) xist_camera_change_y = 10;
  else if (*xist_mouse_y < 10) xist_camera_change_y = -10;
  xist_scroll_camera();
  */
  
  if (current_joypad_state & JOYPAD_L) xist_camera_change_x = -10;
  if (current_joypad_state & JOYPAD_R) xist_camera_change_x = 10;
  if (current_joypad_state & JOYPAD_U) xist_camera_change_y = -10;
  if (current_joypad_state & JOYPAD_D) xist_camera_change_y = 10;
  
  xist_scroll_camera();
  
  xist_camera_change_x = 0;
  xist_camera_change_y = 0;
}


void display_game_counter() {
  xist_palette_offset = XIST_TEXT_COLOR_BLACK;
  
  xist_convert_long_to_ascii_array(game_counter, game_counter_str);
  xist_convert_long_to_ascii_array(second_counter, second_counter_str);
  
  xist_text = clear_counter;
  xist_text_tile_x = 31;
  xist_text_tile_y = 28;
  xist_draw_text();
  xist_text = second_counter_str;
  xist_draw_text();
  
  xist_text = clear_counter;
  xist_text_tile_x = 34;
  xist_draw_text();
  xist_text = game_counter_str;
  xist_draw_text();
  
  ++game_counter;
  if (game_counter % 60 == 0) ++second_counter;
}


void display_camera_coordinates() {
  xist_palette_offset = XIST_TEXT_COLOR_RED;
  
  xist_convert_long_to_ascii_array(xist_camera_x, camera_x_str);
  xist_convert_long_to_ascii_array(xist_camera_y, camera_y_str);
  
  xist_text = clear_counter;
  xist_text_tile_x = 2;
  xist_text_tile_y = 27;
  xist_draw_text();
  xist_text = camera_x_str;
  xist_draw_text();
  
  xist_text = clear_counter;
  xist_text_tile_x = 2;
  xist_text_tile_y = 26;
  xist_draw_text();
  xist_text = camera_y_str;
  xist_draw_text();
}


void main() {
  unsigned short n;
  
  // Xist initialization
  xist_initialize_tiles();
  xist_initialize_text_tiles();
  xist_load_file_to_highram("gfxmeta.dat", XIST_SPRITE_METADATA_BANK);
  
  // Set up tile graphics
  xist_load_graphic(0, TRUE, 15, FALSE, 0);
  
  xist_copy_highram_to_vram_using_metadata(XIST_LAYER_0_TILE_BASE,     15, 0);
  xist_copy_highram_to_vram_using_metadata(XIST_LAYER_0_TILE_BASE+256, 15, 1);
  xist_copy_highram_to_vram_using_metadata(XIST_LAYER_0_TILE_BASE+512, 15, 2);
  xist_copy_highram_to_vram_using_metadata(XIST_LAYER_0_TILE_BASE+768, 15, 3);
      
  // Define tile animations
  xist_tile_animation_time_trigger = 45; // 3/4-second intervals
  xist_tile_animations[0] = 0; // 0 = not animated
  xist_tile_animations[1] = 2; // 1 & 2 cycle back and forth between each other
  xist_tile_animations[2] = 1;
  xist_tile_animations[3] = 3; // 3 = not animated
  
  // Fill the map with random tiles
  xist_seed_a = 123;
  xist_seed_b = 234;
  xist_rand_min = 0;
  xist_rand_max = 3;
  
  for (n = 0; n < LAYER_0_MAPBASE_TOTAL; ++n) {
    xist_map_tiles[n] = xist_rand();
  }
    
  VERA.address = XIST_LAYER_0_MAPBASE;
  VERA.address_hi = XIST_LAYER_0_MAPBASE>>16;
  VERA.address_hi |= 0b10000;
  
  for (n = 0; n < LAYER_0_MAPBASE_TOTAL; ++n) {
    VERA.data0 = xist_map_tiles[n];
    VERA.data0 = 0;
  }
  
  xist_camera_x = 0;
  xist_camera_y = 0;
  
  xist_tilemap_width_pix = 1024;
  xist_tilemap_height_pix = 1024;
  
  // main game loop
  for(EVER) {
    xist_wait();
    
    current_joypad_state = xist_get_joypad();
    
    scroll_map();
    xist_animate_map_tiles();
    
    display_game_counter();
    display_camera_coordinates();
  }
}