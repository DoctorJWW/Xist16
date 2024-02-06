#include <cx16.h>
#include "xist_mem.h"
#include "xist_tiles.h"
#include "xist_utils.h"

unsigned char xist_map_tiles[LAYER_0_MAPBASE_TOTAL];

// lookup table for each row's address (for performance efficiency)
unsigned short xist_mapbase_addresses[LAYER_0_MAPBASE_HEIGHT];

unsigned char xist_tile_animations[XIST_MAX_MAP_TILE_GRAPHICS];
unsigned char xist_tile_animation_timer;
unsigned char xist_tile_animation_time_trigger;

unsigned short xist_tilemap_width_pix, xist_tilemap_height_pix;

signed short xist_camera_x = 0, xist_camera_y = 0;
signed char xist_camera_change_x, xist_camera_change_y;

BOOL xist_allow_tile_scroll_wrap;

// auto-configured variables
unsigned short layer_0_mapbase_width_px, layer_0_mapbase_height_px;
unsigned char layer_0_width_bitmask, layer_0_height_bitmask, layer_0_byte4_bitmask;
unsigned char layer_0_screen_tile_width, layer_0_screen_tile_height;

unsigned char layer_0_width_bitmask_lookup[8] = {
  0b00000000, 0b00010000, 0, 0b00100000,
  0,          0,          0, 0b00110000
};

unsigned char layer_0_height_bitmask_lookup[8] = {
  0b00000000, 0b01000000, 0, 0b10000000,
  0,          0,          0, 0b11000000
};

unsigned char layer_0_tile_dimensions_lookup[2] = { 0, 0b00000011 };

void xist_initialize_tiles() {
  unsigned char n;
  unsigned short tilemap_address = XIST_LAYER_0_MAPBASE;
  
  // turn on 320x240 mode
  VERA.display.hscale = 64;
  VERA.display.vscale = 64;
  
  // set auto-configured variables
  layer_0_mapbase_width_px = LAYER_0_MAPBASE_WIDTH * LAYER_0_TILE_DIMENSIONS;
  layer_0_mapbase_height_px = LAYER_0_MAPBASE_HEIGHT * LAYER_0_TILE_DIMENSIONS;
  
  layer_0_width_bitmask = layer_0_width_bitmask_lookup[(LAYER_0_MAPBASE_WIDTH>>5)-1];
  layer_0_height_bitmask = layer_0_height_bitmask_lookup[(LAYER_0_MAPBASE_HEIGHT>>5)-1];
  
  layer_0_byte4_bitmask = ((XIST_LAYER_0_TILE_BASE & 0x1FFFF) >> 9) & 0xFC;
  layer_0_byte4_bitmask = 0b00101011;
  layer_0_byte4_bitmask |= layer_0_tile_dimensions_lookup[(LAYER_0_TILE_DIMENSIONS>>3)-1];
  
  layer_0_screen_tile_width = XIST_RESOLUTION_WIDTH / LAYER_0_TILE_DIMENSIONS;
  layer_0_screen_tile_height = XIST_RESOLUTION_HEIGHT / LAYER_0_TILE_DIMENSIONS;
  
  // set VERA properties
  VERA.display.video  |= 0b01110000; // Enable sprites and both layers
  
  VERA.layer0.config   = layer_0_height_bitmask | layer_0_width_bitmask | 0b00000011; // 256-color mode
  VERA.layer0.mapbase  = XIST_LAYER_0_MAPBASE>>9;
  VERA.layer0.tilebase = layer_0_byte4_bitmask;
  
  VERA.layer1.config   = LAYER_1_HEIGHT_BITMASK | LAYER_1_WIDTH_BITMASK | 0b00000011; // 256-color mode
  VERA.layer1.mapbase  = XIST_LAYER_1_MAPBASE>>9;
  VERA.layer1.tilebase = LAYER_1_BYTE4_BITMASK;
  
  // precompute the addresses of each row of our layer-0 tilemap
  for (n = 0; n < LAYER_0_MAPBASE_HEIGHT; ++n) {
    xist_mapbase_addresses[n] = tilemap_address;
    tilemap_address += (LAYER_1_MAPBASE_WIDTH * 2); // because each tile is 2 bytes
  }
}


void xist_scroll_camera() {
  xist_camera_x += xist_camera_change_x;
  xist_camera_y += xist_camera_change_y;
  
  if (!xist_allow_tile_scroll_wrap) {
    if (xist_camera_x < 0) xist_camera_x = 0;
    else if (xist_camera_x > xist_tilemap_width_pix - XIST_RESOLUTION_WIDTH)
      xist_camera_x = xist_tilemap_width_pix - XIST_RESOLUTION_WIDTH;
    if (xist_camera_y < 0) xist_camera_y = 0;
    else if (xist_camera_y > xist_tilemap_height_pix - XIST_RESOLUTION_HEIGHT)
      xist_camera_y = xist_tilemap_height_pix - XIST_RESOLUTION_HEIGHT;
  }
    
  VERA.layer0.hscroll = xist_camera_x;
  VERA.layer0.vscroll = xist_camera_y;
}


void xist_animate_map_tiles() {
  unsigned char start_tile_x, end_tile_x;
  unsigned char start_tile_y, end_tile_y;
  unsigned char y, x;
  unsigned short mapbase_index;
  unsigned long mapbase_address;
  unsigned char current_tile_index;
  
  ++xist_tile_animation_timer;
  if (xist_tile_animation_timer == xist_tile_animation_time_trigger) {
    xist_tile_animation_timer = 0;
    
    start_tile_x = xist_camera_x / LAYER_0_TILE_DIMENSIONS;
    end_tile_x = start_tile_x + layer_0_screen_tile_width;
    
    start_tile_y = xist_camera_y / LAYER_0_TILE_DIMENSIONS;
    end_tile_y = start_tile_y + layer_0_screen_tile_height;
    
    mapbase_index = start_tile_y * LAYER_0_MAPBASE_WIDTH + start_tile_x;
    
    for (y = start_tile_y; y < end_tile_y; ++y) {
      mapbase_address = xist_mapbase_addresses[y] + (start_tile_x*2);
      VERA.address = mapbase_address;
      VERA.address_hi = mapbase_address>>16;
      VERA.address_hi |= 0b10000;
      
      for (x = start_tile_x; x < end_tile_x; ++x) {
        current_tile_index = xist_map_tiles[mapbase_index];
        xist_map_tiles[mapbase_index] = xist_tile_animations[current_tile_index];
        VERA.data0 = xist_map_tiles[mapbase_index];
        VERA.data0 = 0;
        ++mapbase_index;
      }
      mapbase_index += (LAYER_0_MAPBASE_WIDTH - end_tile_x) + start_tile_x;
    }
  }
}
