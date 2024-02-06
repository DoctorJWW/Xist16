/**
 * \file     xist_mem.h
 * \brief    Functionality for loading and managing memory.
 *
 * Contains macros and function signatures
 * for loading and managing memory and defining important memory addresses in VRAM.
 */
 
#ifndef XIST_MEM_H
#define XIST_MEM_H

#include "xist_utils.h"

#define BANK_NUM (*(unsigned char *)0x00)
#define R0 (*(unsigned short *)0x02)
#define R1 (*(unsigned short *)0x04)
#define R2 (*(unsigned short *)0x06)

#define XIST_BANKED_RAM_ADDRESS     0xA000UL

#define XIST_LAYER_1_TILE_BASE      0x0000UL
#define XIST_LAYER_1_MAPBASE        0x2000UL
#define XIST_LAYER_0_MAPBASE        0x3000UL
#define XIST_LAYER_0_TILE_BASE      0x5000UL

#define XIST_VRAM_SPRITE_IMAGE_DATA 0x9000UL
#define XIST_VRAM_SPRITE_0_ADDRESS  0x1FC00UL
#define XIST_VRAM_SPRITE_1_ADDRESS  0x1FC08UL

#define XIST_SPRITE_METADATA_BANK 63

/**
 * \fn void xist_load_file_to_highram(const char *filename, unsigned char bank)
 * \brief Loads the specified file into the specified high-RAM bank.
 *
 * \param   const char *filename Name of the file to load into high-RAM, including the file extension.
 * \param   unsigned char bank   The bank of high-RAM to load the file into.
 * \return  void
 */
void xist_load_file_to_highram(const char *filename, unsigned char bank);

void xist_load_file_to_vram(const char *filename,
                            BOOL to_upper,
                            unsigned long vram_address);

/**
 * \fn xist_copy_highram_to_vram(unsigned long vram_address,
 *                               unsigned char bank,
 *                               unsigned short mem_offset,
 *                               unsigned short num_bytes)
 * \brief Copies data from the specified high-RAM bank to the specified address in VRAM.
 *
 * \param   unsigned long vram_address      VRAM address to copy the data to
 * \param   unsigned char bank              High-RAM bank to copy the data from
 * \param   unsigned short mem_offset       Number of bytes from the start of the high-RAM bank to start
 *                                          copying from
 * \param   unsigned short num_bytes        Number of bytes to copy
 * \return  void
 */
void xist_copy_highram_to_vram(unsigned long vram_address,
                               unsigned char bank,
                               unsigned short mem_offset,
                               unsigned short num_bytes);

/**
 * \fn void xist_copy_highram_to_vram_partial(unsigned long vram_address,
 *                                            unsigned char highram_bank,
 *                                            unsigned char x_offset,
 *                                            unsigned char y_offset,
 *                                            unsigned char gfx_width,
 *                                            unsigned char gfx_height,
 *                                            unsigned char h_width)
 * \brief Copies a rectangular segment of graphical data from the specified high-RAM bank to the
 *        specified address in VRAM.
 *
 * \param   unsigned long vram_address      VRAM address to copy the data to
 * \param   unsigned char highram_bank      High-RAM bank to copy the data from
 * \param   unsigned char x_offset          Number of pixels from the left side of the image data to start
 *                                          copying from
 * \param   unsigned char y_offset          Number of pixels from the top of the image data to start copying from
 * \param   unsigned char gfx_width         The width in pixels of the area of graphical data to copy
 * \param   unsigned char gfx_height        The height in pixels of the area of graphical data to copy
 * \param   unsigned char h_width           The total width of the graphical data segment being copied from
 * \return  void
 */
void xist_copy_highram_to_vram_partial(unsigned long vram_address,
                                       unsigned char highram_bank,
                                       unsigned char x_offset,
                                       unsigned char y_offset,
                                       unsigned char gfx_width,
                                       unsigned char gfx_height,
                                       unsigned char h_width);

/**
 * \fn void xist_copy_highram_to_vram_using_metadata(unsigned long vram_address,
 *                                                   unsigned char highram_bank,
 *                                                   unsigned short metadata_index)
 * \brief Copies a segment of graphical data from the specified high-RAM bank to the
 *        specified address in VRAM using data from the graphics metadata file.
 *
 * \param   unsigned long vram_address      VRAM address to copy the data to
 * \param   unsigned char highram_bank      High-RAM bank to copy the data from
 * \param   unsigned short metadata_index   The row in your sprite metadata that contains the data for the graphic
 *                                          to be copied
 * \return  void
 * \see     XistGraphicsMetadata
 */
void xist_copy_highram_to_vram_using_metadata(unsigned long vram_address,
                                              unsigned char highram_bank,
                                              unsigned short metadata_index);

#endif