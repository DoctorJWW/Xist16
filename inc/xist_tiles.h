/**
 * \file     xist_tiles.h
 * \brief    Functionality for rendering tilemaps on layer 0.
 *
 * Contains macros, global variables, and function signatures to control tile dimensions
 * and manipulate the tiles on layer 0.
 */

#ifndef XIST_TILES_H
#define XIST_TILES_H

#define XIST_TILE_PALETTE_OFFSET_1  0b00010000
#define XIST_TILE_PALETTE_OFFSET_2  0b00100000
#define XIST_TILE_PALETTE_OFFSET_3  0b01000000
#define XIST_TILE_PALETTE_OFFSET_4  0b10000000

// Important terminology to remember:
// MapBase = The space dedicated to your tile metadata (which graphic each tile is pointing to).
//           There are 2 separate MapBases, for layer 0 (drawn behind) and layer 1 (drawn in front).
//           The default Xist16 assumption is that layer 1 is dedicated to text display.
//           The amount of space required for the MapBase depends on the dimensions of each layer.
// TileBase = The graphical data for your tile layers.
//            The amount of space required for the TileBase depends on how many tile graphics you have.

// Set the dimensions for your tile layer 0 below and the xist_initialize_tiles() function
// will take care of configuring all the other values needed by the VERA.
// Then, in xist_mem.h, make sure to supply enough space for your tile layers. Remember that
// each tile needs 2 bytes!


// Edit these to values appropriate to your game's background tiles:

/**
 * \def LAYER_0_MAPBASE_WIDTH
 * \brief The width in tiles of tile layer 0 for your game. Valid values are 32, 62, 128, 256.
 */
#define LAYER_0_MAPBASE_WIDTH 64

/**
 * \def LAYER_0_MAPBASE_HEIGHT
 * \brief The height in tiles of tile layer 0 for your game. Valid values are 32, 62, 128, 256.
 */
#define LAYER_0_MAPBASE_HEIGHT 64

/**
 * \def LAYER_0_MAPBASE_TOTAL
 * \brief The total number of tiles in your layer 0 (equal to width * height).
 * \see xist_map_tiles
 * Set this to be equal to LAYER_0_MAPBASE_WIDTH * LAYER_0_MAPBASE_HEIGHT. The reason this is a macro
 * instead of being auto-calculated by `xist_initialize_tiles` is because it's used to define the size
 * of the `xist_map_tiles` array.
 */
#define LAYER_0_MAPBASE_TOTAL 4096

/**
 * \def LAYER_0_TILE_DIMENSIONS
 * \brief The width/height of your individual tiles, in pixels.
 * Valid values are 8 or 16. In Xist16, your tiles must be the same width and height.
 */
#define LAYER_0_TILE_DIMENSIONS 16

// Don't edit these if you want to use the text drawing functions:
#define LAYER_1_MAPBASE_WIDTH 64
#define LAYER_1_MAPBASE_HEIGHT 32
#define LAYER_1_MAPBASE_TOTAL 2048
#define LAYER_1_WIDTH_BITMASK  0b00010000
#define LAYER_1_HEIGHT_BITMASK 0b00000000
#define LAYER_1_SCREEN_TILE_WIDTH  40
#define LAYER_1_SCREEN_TILE_HEIGHT 30
#define LAYER_1_TILE_DIMENSIONS 8
#define LAYER_1_BYTE4_BITMASK  0b00000000

/**
 * \def XIST_MAX_MAP_TILE_GRAPHICS
 * \brief The maximum number of concurrent tile graphics in VRAM.
 *
 * Set this to the maximum number of concurrent tile graphics in VRAM you wish for your game to
 * support. The highest value that works with Xist16's functionality is 256 (even though the
 * Commander X16 can technically support more). Make sure the amount of space you allocate to
 * your XIST_LAYER_0_TILE_BASE defined in `xist_mem.h` comports with the value defined here.
 */
#define XIST_MAX_MAP_TILE_GRAPHICS 64

/** \var unsigned char xist_map_tiles[]
 *  \brief Array containing the indices of tile graphics for each tile on layer 0
 *  \see xist_animate_map_tiles
 *
 * This array contains the indices of each of the tile graphics used by every tile in VRAM for the
 * current tilemap. I have read that multidimensional arrays are slow when compiled by cc65, so
 * as a performance optimization, this is a one-dimensional array. It would map to an equivalent
 * two-dimensional array if that array were read from left-to-right and top-to-bottom (i.e.,
 * one row at a time).
 */
extern unsigned char xist_map_tiles[LAYER_0_MAPBASE_TOTAL];

/** \var unsigned char xist_tile_animations[]
 *  \brief Lookup table that defines the animation sequences for tile graphics
 *  \see xist_animate_map_tiles
 *
 * Tile animations are defined by having this array be equal to the maximum number of concurrent tile
 * graphics supported by your game, where each index in this table is equal to the index of that tile
 * in VRAM, and the value corresponding to that index is the index of the next frame of animation.
 * If a tile is not animated, just set the value equal to the same value as that index, essentially
 * defining a 1-frame animation. For example, here is how you would define a 3-frame animation that
 * cycles through tile graphics 0, 1, and 3:
 * \code
 * xist_tile_animations[0] = 1;
 * xist_tile_animations[1] = 2;
 * xist_tile_animations[2] = 0;
 * \endcode
 */
extern unsigned char xist_tile_animations[XIST_MAX_MAP_TILE_GRAPHICS];

/** \var unsigned char xist_tile_animation_time_trigger
 *  \brief The number of game ticks that each frame of animation for animation tiles should display
 *  \see xist_animate_map_tiles
 *
 * Sets the amount of time (in game ticks, or more specifically invocations of the `xist_animate_map_tiles`
 * function) that each frame of animation in animated tiles should be displayed before moving on to the
 * next frame. As a performance optimization, all tiles animate on the same timer.
 */
extern unsigned char xist_tile_animation_time_trigger;

/** \var BOOL xist_allow_tile_scroll_wrap
 *  \brief Acts as a virtual parameter for the function `xist_scroll_camera`.
 *  \see xist_scroll_camera
 *
 * Determines whether tile layer 0 is allowed to wrap; i.e., whether the viewport/camera is
 * allowed to scroll past the edges of the tile map
 * (defined as [0, 0] - [xist_tilemap_width_pix, xist_tilemap_height_pix])
 * or whether it is constrained to those boundaries. Set it to FALSE to constrain the
 * camera and TRUE to permit wrapping.
 */
extern BOOL xist_allow_tile_scroll_wrap;

/** \var unsigned short xist_tilemap_width_pix
 *  \brief Acts as a virtual parameter for the function `xist_scroll_camera`.
 *  \see xist_scroll_camera
 *
 * Sets the right boundary for the viewable area of tile layer 0; i.e., the camera/viewport will
 * not scroll past the edge defined by this value. If you want the entire horizontal area of layer 0
 * to be viewable, set the value of this variable to LAYER_0_MAPBASE_WIDTH * LAYER_0_TILE_DIMENSIONS.
 * Its minimum value should be XIST_RESOLUTION_WIDTH.
 */
extern unsigned short xist_tilemap_width_pix; 

/** \var unsigned short xist_tilemap_height_pix
 *  \brief Acts as a virtual parameter for the function `xist_scroll_camera`.
 *  \see xist_scroll_camera
 *
 * Sets the bottom boundary for the viewable area of tile layer 0; i.e., the camera/viewport will
 * not scroll past the edge defined by this value. If you want the entire vertical area of layer 0
 * to be viewable, set the value of this variable to LAYER_0_MAPBASE_HEIGHT * LAYER_0_TILE_DIMENSIONS.
 * Its minimum value should be XIST_RESOLUTION_HEIGHT.
 */
extern unsigned short xist_tilemap_height_pix;

/** \var signed short xist_camera_x
 *  \brief The left side of the current viewport, in pixels.
 *  \see xist_scroll_camera
 */
extern signed short xist_camera_x;

/** \var signed short xist_camera_y
 *  \brief The top of the current viewport, in pixels.
 *  \see xist_scroll_camera
 */
extern signed short  xist_camera_y;

/** \var unsigned char xist_camera_change_x
 *  \brief Acts as a virtual parameter for the function `xist_scroll_camera`.
 *  \see xist_scroll_camera
 *
 * Sets the amount by which to scroll the viewport horizontally when `xist_scroll_camera` is called.
 * A negative value scrolls the 'camera' to the left; a positive value scrolls it to the right.
 */
extern signed char xist_camera_change_x;

/** \var unsigned char xist_camera_change_y
 *  \brief Acts as a virtual parameter for the function `xist_scroll_camera`.
 *  \see xist_scroll_camera
 *
 * Sets the amount by which to scroll the viewport vertically when `xist_scroll_camera` is called.
 * A negative value scrolls the 'camera' up; a positive value scrolls it down.
 */
extern signed char  xist_camera_change_y;

/**
 * \fn void xist_initialize_tiles()
 * \brief Initializes the tile layers for use with Xist16's functions
 *
 * Uses the macro definitions in this file to automatically configure the tile layers, setting VERA values
 * and precomputing addresses as appropriate. You should call this during your program initialization,
 * before any other Xist16 functions.
 */
void xist_initialize_tiles();

/**
 * \fn void xist_scroll_camera()
 * \brief Scrolls tile layer 0 and optionally stops scrolling at the layer's edge
 * \see xist_camera_change_x
 * \see xist_camera_change_y
 * \see xist_tilemap_width_pix
 * \see xist_tilemap_height_pix
 * \see xist_allow_tile_scroll_wrap
 *
 * As a performance optimization, this function uses global variables as virtual parameters.
 * When called, it scrolls tile layer 0 by an amount specified in `xist_camera_change_x` and
 * `xist_camera_change_y`. If `xist_allow_tile_scroll_wrap` is set to FALSE, scrolling stops
 * when you hit the edge of the tile layer; i.e., your viewport is constrained to the boundaries
 * of tile layer 0. These boundaries are defined as [0, 0] - [xist_tilemap_width_pix, xist_tilemap_height_pix].
 */
void xist_scroll_camera();

/**
 * \fn void xist_animate_map_tiles()
 * \brief Animates visible tiles on layer 0
 * \see xist_map_tiles
 * \see xist_tile_animation_time_trigger
 *
 * Animates all tiles on layer 0 that have animations defined in the `xist_map_tiles` lookup table.
 * As a performance opmization, all tiles animate on the same timer. Define the number of game ticks that
 * each frame of animation lasts for using the `xist_tile_animation_time_trigger` global variable.
 * IMPORTANT NOTE: This function assumes that wrapping tile layer 0 is disallowed. If this is not the
 * case, its behavior is undefined.
 */
void xist_animate_map_tiles();

#endif