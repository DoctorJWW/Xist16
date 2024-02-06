#include <cx16.h>
#include "xist_gfx.h"
#include "xist_input.h"
#include "xist_mem.h"
#include "xist_text.h"
#include "xist_tiles.h"
#include "xist_utils.h"

#define TEST_SPRITE_BANK  15


unsigned short ship_x = 144;
unsigned short ship_y = 200;
char clear_counter[MAX_SIGNED_INT_CHARS] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 0 };
const unsigned char num_stars = 32;
signed short star_starting_pos[32];

unsigned short current_joypad_state;


void process_input() {
  if (current_joypad_state & JOYPAD_L) ship_x -= 3;
  if (current_joypad_state & JOYPAD_R) ship_x += 3;
  if (current_joypad_state & JOYPAD_U) ship_y -= 3;
  if (current_joypad_state & JOYPAD_D) ship_y += 3;
  
  // Adjust position of ship sprite
  xist_curr_sprite_idx = 1;
  xist_curr_sprite_x_pos = ship_x;
  xist_curr_sprite_y_pos = ship_y;
  xist_update_sprite_position();
}


void move_stars() {
  signed short n;
  
  for (n = 0; n < num_stars; ++n) {
    // let's make the star speeds variable
    if (n % 3 == 0) xist_sprite_bank.y_pos[n+2] += 3;
    else xist_sprite_bank.y_pos[n+2] += 2;
    if (xist_sprite_bank.y_pos[n+2] > 250) xist_sprite_bank.y_pos[n+2] = star_starting_pos[n];
    
    xist_curr_sprite_idx = n + 2;
    xist_curr_sprite_x_pos = xist_sprite_bank.x_pos[n+2];
    xist_curr_sprite_y_pos = xist_sprite_bank.y_pos[n+2];
    xist_update_sprite_position();
  }
}


void main() {
  signed long game_counter = 0;
  unsigned short second_counter = 0;
  unsigned short n;
  char game_counter_str[MAX_SIGNED_INT_CHARS];
  char second_counter_str[MAX_SIGNED_INT_CHARS];
  
  xist_seed_a = 123;
  xist_seed_b = 234;
  
  // Xist initialization
  xist_initialize_tiles();
  xist_initialize_text_tiles();
  xist_load_file_to_highram("gfxmeta.dat", XIST_SPRITE_METADATA_BANK);
  
  // Load image that contains ship and star animations
  xist_load_graphic(0, TRUE, TEST_SPRITE_BANK, FALSE, 0);
  
  // Copy deep blue 'space' tile into VRAM
  xist_copy_highram_to_vram_using_metadata(XIST_LAYER_0_TILE_BASE, TEST_SPRITE_BANK, 2);
  
  // Fill the map with this tile
  for (n = 0; n < LAYER_0_MAPBASE_TOTAL; ++n) xist_map_tiles[n] = 0;
  VERA.address = XIST_LAYER_0_MAPBASE;
  VERA.address_hi = XIST_LAYER_0_MAPBASE>>16;
  VERA.address_hi |= 0b10000;
  for (n = 0; n < LAYER_0_MAPBASE_TOTAL; ++n) {
    VERA.data0 = xist_map_tiles[n];
    VERA.data0 = 0;
  }
  
  // Prepare ship sprite
  xist_prepare_sprite(0, TRUE, XIST_VRAM_SPRITE_IMAGE_DATA, TEST_SPRITE_BANK, 1, ship_x, ship_y, XIST_SPRITE_ACTIVE);
  
  // Initialize star sprites and randomize their Y positions  
  xist_rand_min = 0;
  xist_rand_max = 255;
  for (n = 0; n < num_stars; ++n) {
    xist_sprite_bank.x_pos[n+2] = n * 10;
    xist_sprite_bank.y_pos[n+2] = -(xist_rand());
    star_starting_pos[n] = xist_sprite_bank.y_pos[n+2];
    if (n == 0) {
      xist_prepare_sprite(1, TRUE, XIST_VRAM_SPRITE_IMAGE_DATA + 1024, TEST_SPRITE_BANK, n+2, xist_sprite_bank.x_pos[n+2], xist_sprite_bank.y_pos[n+2], XIST_SPRITE_ACTIVE);
    }
    else {
      xist_prepare_sprite(1, FALSE, XIST_VRAM_SPRITE_IMAGE_DATA + 1024, TEST_SPRITE_BANK, n+2, xist_sprite_bank.x_pos[n+2], xist_sprite_bank.y_pos[n+2], XIST_SPRITE_ACTIVE);
    }
    // every other iteration, set the frame to 2 so they aren't all on the same frame of animation
    if (n % 2 == 0) xist_sprite_bank.current_frame[n+2] = 2;
  }
  
  // main game loop
  for(EVER) {
    xist_wait();
    
    current_joypad_state = xist_get_joypad();
    
    process_input();
    move_stars();
    
    xist_proc_sprite_start_idx = 1;
    xist_proc_sprite_end_idx = num_stars+1;
    xist_process_sprites();
    
    // display game counter
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
}