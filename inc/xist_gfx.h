/**
 * \file     xist_gfx.h
 * \brief    Graphics functionality.
 *
 * Contains macros, structs, extern variables, and function signatures
 * for Xist graphics functionality generally and sprites in particular.
 */

#ifndef XIST_GFX_H
#define XIST_GFX_H

#include "xist_utils.h"

/**
 * \def XIST_MAX_SPRITES
 * \brief The maximum number of sprites supported.
 *
 * Defines the maximum number of sprites your application will support and
 * controls the size of `xist_sprite_bank`. The maximum possible value for
 * this macro is 128 because that is the maximum number of sprites supported
 * by the Commander X16. You should not set this value higher than necessary,
 * because the higher you set this value, the more memory will be consumed,
 * and processing more sprites will entail a greater processor strain.
 */
#define XIST_MAX_SPRITES 64

/**
 * \def XIST_SPRITE_ACTIVE
 * \brief Bit in the sprite bitmask that determines if it is currently active
 */
#define XIST_SPRITE_ACTIVE   0b00000001

/**
 * \def XIST_SPRITE_REPEATS
 * \brief Bit in the sprite bitmask that determines if it is a repeating animation
 */
#define XIST_SPRITE_REPEATS  0b00000010

/**
 * \def XIST_SPRITE_FINISHED
 * \brief Bit in the sprite bitmask that determines if it the animation is finished playing
 *
 * This value is only relevant to sprites that do not repeat, and will be set to 1
 * once the non-repeating sprite is finished animating.
 */
#define XIST_SPRITE_FINISHED 0b00000100

#define XIST_SPRITE_PALETTE_OFFSET_1  0b00000001
#define XIST_SPRITE_PALETTE_OFFSET_2  0b00000010
#define XIST_SPRITE_PALETTE_OFFSET_3  0b00000100
#define XIST_SPRITE_PALETTE_OFFSET_4  0b00001000

/** \struct XistAnimation
 *  \brief Contains data needed for Xist16 to animate the sprites.
 *
 * This is defined as a struct of arrays rather than an array of structs
 * as an important performance optimization due to the 6502's architecture.
 */
struct XistAnimation {
  unsigned short first_frame_address[XIST_MAX_SPRITES]; /**< VRAM address of the first frame of graphical data. */
  unsigned short address_offset[XIST_MAX_SPRITES]; /**< Size in bytes of each frame of animation (every frame must be the same size). */
  unsigned char num_frames[XIST_MAX_SPRITES]; /**< Total number of frames of animation in this sprite. */
  unsigned char current_frame[XIST_MAX_SPRITES]; /**< The current frame of animation being displayed. */
  unsigned char frame_delay[XIST_MAX_SPRITES]; /**< The number of game ticks to display each frame. */
  unsigned char frame_timer[XIST_MAX_SPRITES]; /**< Current value of the sprite's frame timer. */
  unsigned char bitmask[XIST_MAX_SPRITES]; /**< bytes from low (right) to high (left): Active, Repeats, Finished  animating -----FRA */
  signed short x_pos[XIST_MAX_SPRITES]; /**< The sprite's current X position onscreen, in pixels. */
  signed short y_pos[XIST_MAX_SPRITES]; /**< The sprite's current Y position onscreen, in pixels. */
};

/** \struct XistGraphicsMetadata
 *  \brief Contains metadata needed for Xist16 to load and parse graphical data.
 */
struct XistGraphicsMetadata {
  char filename[4]; /**< Name of the file that contains the graphical data, minus the .IMG file extension. */
  unsigned char x_offset; /**< Number of pixels from the left side of the image where the subimage begins. */
  unsigned char y_offset; /**< Number of pixels from the top of the image where the subimage begins. */
  unsigned char width; /**< The width in pixels of each frame of animation for this graphic (must all be the same). */
  unsigned char height; /**< The height in pixels of each frame of animation for this graphic (must all be the same). */
  unsigned char total_width; /**< The total width in pixels of the entire image that contains this graphic. */
  unsigned char num_frames; /**< This graphic's total frames of animation. */
  unsigned char frame_delay; /**< The number of game ticks to show each frame of animation for. */
  unsigned char bitmask; /**< bytes from low (right) to high (left): Active, Repeats, Finished  animating -----FRA */
  unsigned char byte_6; /**< Byte 6 of the Commander X16 sprite metadata */
  unsigned char byte_7; /**< Byte 7 of the Commander X16 sprite metadata */
};

/** \var XistAnimation xist_sprite_bank
 *  \brief Contains data needed for Xist16 to animate the sprites.
 */
extern struct XistAnimation xist_sprite_bank;

/** \var unsigned char xist_proc_sprite_start_idx
 *  \brief Acts as a virtual parameter for the function `xist_process_sprites`.
 *  \see xist_process_sprites
 *
 * Sets the start index (inclusive) for the sprites to process in `xist_sprite_bank`.
 */
extern unsigned char xist_proc_sprite_start_idx;

/** \var unsigned char xist_proc_sprite_end_idx
 *  \brief Acts as a virtual parameter for the function `xist_process_sprites`.
 *  \see xist_process_sprites
 *
 * Sets the end index (inclusive) for the sprites to process in `xist_sprite_bank`.
 */
extern unsigned char xist_proc_sprite_end_idx;

/** \var unsigned char xist_curr_sprite_idx
 *  \brief Acts as a virtual parameter for the function `xist_update_sprite_position`.
 *  \see xist_update_sprite_position
 *
 * Sets the index for the sprite in `xist_sprite_bank` whose position will be updated.
 */
extern unsigned char xist_curr_sprite_idx;

/** \var unsigned char xist_curr_sprite_x_pos
 *  \brief Acts as a virtual parameter for the function `xist_update_sprite_position`.
 *  \see xist_update_sprite_position
 *
 * Sets the new X position (in pixels) for the sprite to be updated.
 */
extern signed short xist_curr_sprite_x_pos;

/** \var unsigned char xist_curr_sprite_y_pos
 *  \brief Acts as a virtual parameter for the function `xist_update_sprite_position`.
 *  \see xist_update_sprite_position
 *
 * Sets the new Y position (in pixels) for the sprite to be updated.
 */
extern signed short xist_curr_sprite_y_pos;

/** \var unsigned char xist_curr_sprite_byte_6
 *  \brief Acts as a virtual parameter for the function `xist_update_sprite_parameters`.
 *  \see xist_update_sprite_parameters
 *
 * Sets the new byte 6 of the CX16 sprite metadata for the sprite to be updated.
 */
extern unsigned char xist_curr_sprite_byte_6;

/**
 * \fn void xist_process_sprites(void)
 * \brief Animates all of the sprites in `xist_sprite_bank` within the supplied indices (inclusive).
 * \see xist_proc_sprite_start_idx
 * \see xist_proc_sprite_end_idx
 *
 * As a performance optimization, this function uses global variables as virtual parameters.
 * It performs all the necessary updates to the sprite metadata in `xist_sprite_bank` and the
 * Commander X16's metadata in VRAM (including updating the address where the graphics for the
 * next frame of animation are stored in VRAM) necessary to animate all the sprites specified
 * by the start and end indices. It ignores sprites that aren't set to active or which have
 * finished animating (the latter does not apply to sprites with repeating animations).
 */
void xist_process_sprites();

/**
 * \fn       void xist_load_graphic(unsigned short metadata_index,
 *                                  BOOL to_highram,
 *                                  unsigned char to_highram_bank,
 *                                  BOOL to_upper_vram,
 *                                  unsigned long vram_address)
 * \brief    Loads the graphic file specified in the given metadata entry into either high-RAM or VRAM.
 *
 * \param    metadata_index   The row in your graphics metadata file that contains the information for this graphic.
 * \param    to_highram       Set to TRUE (1) to load the graphics into high-RAM, or FALSE (0) to load the
 *                            graphics into VRAM.
 * \param    to_highram_bank  The bank of high-RAM to load the image data into, if loading into high-RAM
 *                            (ignored if `to_highram` is set to FALSE).
 * \param    to_upper_vram    If loading to VRAM, set this to TRUE to set the parameter into the Kernal’s LOAD
 *                            function to 3, loading into VRAM starting from 0x10000 + the specified VRAM address.
 *                            Set to FALSE to set the parameter into the Kernal’s LOAD function to 2, loading into
 *                            VRAM starting from 0x00000 + the specified starting address. Ignored if `to_highram`
 *                            is set to TRUE.
 * \param    vram_address     If loading to VRAM, supply the VRAM address to load into with this parameter.
 *                            Ignored if `to_highram` is set to TRUE.
 * \return   void
 * \see XistGraphicsMetadata
 */
void xist_load_graphic(unsigned short metadata_index,
                       BOOL to_highram,
                       unsigned char to_highram_bank,
                       BOOL to_upper_vram,
                       unsigned long vram_address);
                       
/**
 * \fn       void xist_prepare_sprite(unsigned short metadata_index,
 *                                    BOOL copy_to_vram,
 *                                    unsigned long vram_address,
 *                                    unsigned char highram_bank,
 *                                    unsigned char sprite_index,
 *                                    signed short x_pos,
 *                                    signed short y_pos,
 *                                    unsigned char add_bitmask)
 * \brief    Readies a sprite which has been loaded into high-RAM for display and animation.
 * 
 * Prepares a sprite whose graphical data has already been loaded into high-RAM for display
 * and animation. It sets up all the necessary parameters for the sprite using the given index
 * into `xist_sprite_bank` (which is also the same as the sprite's index in the CX16's sprite
 * metadata). Can also be directed to copy the sprite's graphical data from high-RAM into VRAM.
 *
 * \param    metadata_index   The row in your graphics metadata file that contains the information for this sprite.
 * \param    copy_to_vram     Set to TRUE if you want to copy the image data from high-RAM into VRAM. The use
 *                            case for setting this to FALSE is if you have many sprites that use the same image
 *                            data, in which case you could set this to TRUE when preparing the first sprite and
 *                            FALSE for all subsequent calls to save a lot of unnecessary copying.
 * \param    vram_address     If loading to VRAM, supply the VRAM address to load into with this parameter.
 *                            Ignored if to_ vram is set to FALSE.
 * \param    highram_bank     The high-RAM bank that contains the image data to be copied.
 * \param    sprite_index     The index of the sprite in both the xist_sprite_bank and the Commander X16’s sprite
 *                            metadata.
 * \param    x_pos            The onscreen X position (in pixels) to display the sprite at.
 * \param    y_pos            The onscreen Y position (in pixels) to display the sprite at.
 * \param    add_bitmask      A bitmask with any flags you want to turn on for this sprite. By far the most
 *                            common use case would be to provide `XIST_SPRITE_ACTIVE` to make the sprite
 *                            immediately active for use.
 *
 * \return   void
 * \see XistAnimation
 * \see XistGraphicsMetadata
 * \see xist_sprite_bank
 */
void xist_prepare_sprite(unsigned short metadata_index,
                         BOOL copy_to_vram,
                         unsigned long vram_address,
                         unsigned char highram_bank,
                         unsigned char sprite_index,
                         signed short x_pos,
                         signed short y_pos,
                         unsigned char add_bitmask);
                         
/**
 * \fn void xist_update_sprite_position(void)
 * \brief Updates the position of the provided sprite.
 * \see xist_curr_sprite_idx
 * \see xist_curr_sprite_x_pos
 * \see xist_curr_sprite_y_pos
 *
 * As a performance optimization, this function uses global variables as virtual parameters.
 * It updates the position of the given sprite in both `xist_sprite_bank` and the Commander X16's
 * sprite metadata in VRAM. `xist_curr_sprite_idx` indicates the index of the sprite to be updated,
 * while `xist_curr_sprite_x_pos` and `xist_curr_sprite_y_pos` indicate the screen X and Y position
 * (in pixels) to set the sprite to.
 *
 * \return   void
 */
void xist_update_sprite_position();

/**
 * \fn void xist_update_sprite_parameters(void)
 * \brief Updates byte 6 of the provided sprite.
 * \see xist_curr_sprite_idx
 * \see xist_curr_sprite_byte_6
 *
 * As a performance optimization, this function uses global variables as virtual parameters.
 * Updates the 6th byte of the CX16 sprite metadata in VRAM (collision mask, z-depth, v-flip, and h-flip).
 *
 * \return   void
 */
void xist_update_sprite_parameters();

#endif
