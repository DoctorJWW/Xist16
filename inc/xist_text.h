/**
 * \file     xist_text.h
 * \brief    Text rendering functionality
 *
 * Contains macros, global variables, and function signatures
 * for rendering and handling text.
 */

#ifndef XIST_TEXT_H
#define XIST_TEXT_H

#define XIST_TYPEWRITER_TEXT_ROWS 10
#define XIST_TYPEWRITER_TEXT_COLUMNS 36

#define XIST_TEXT_COLOR_WHITE     0b00000000
#define XIST_TEXT_COLOR_BLACK     0b00010000
#define XIST_TEXT_COLOR_BURGUNDY  0b00100000
#define XIST_TEXT_COLOR_RED       0b00110000
#define XIST_TEXT_COLOR_YELLOW    0b01000000
#define XIST_TEXT_COLOR_UMBER     0b01010000
#define XIST_TEXT_COLOR_PINE      0b01100000
#define XIST_TEXT_COLOR_GREEN     0b01110000
#define XIST_TEXT_COLOR_MINT      0b10000000
#define XIST_TEXT_COLOR_SLATE     0b10010000
#define XIST_TEXT_COLOR_CYAN      0b10100000
#define XIST_TEXT_COLOR_GRAY      0b10110000
#define XIST_TEXT_COLOR_MIDNIGHT  0b11000000
#define XIST_TEXT_COLOR_INDIGO    0b11010000
#define XIST_TEXT_COLOR_PURPLE    0b11100000
#define XIST_TEXT_COLOR_PINK      0b11110000


/** \var char xist_typewriter_text[][]
 *  \brief Multidimensional array to hold strings to be rendered as typewriter text
 *
 * Populate this array with strings you wish to render one character at a time to create
 * a "typewriter text" effect. Then call `xist_draw_typewriter_text` for each game tick you
 * want to render the next character in these strings.
 * IMPORTANT NOTE: Your strings must be arrays of raw ASCII codes, not hard-coded strings,
 * because cc65 encodes hard-coded strings using non-standard encoding that doesn't work with
 * Xist16's text drawing routines.
 * IMPORTANT NOTE 2: Always remember to null-terminate your strings (the last value in the
 * string should be 0) or the behavior of this function is UNDEFINED.
 *
 * \see xist_draw_typewriter_text
 */
extern char xist_typewriter_text[XIST_TYPEWRITER_TEXT_ROWS][XIST_TYPEWRITER_TEXT_COLUMNS];

/** \var char* xist_text
 *  \brief Acts as a virtual parameter for the function `xist_draw_text`.
 *  \see xist_draw_text
 *
 * Sets the text to be rendered when `xist_draw_text` is called.
 * IMPORTANT NOTE: Your strings must be arrays of raw ASCII codes, not hard-coded strings,
 * because cc65 encodes hard-coded strings using non-standard encoding that doesn't work with
 * Xist16's text drawing routines.
 * IMPORTANT NOTE 2: Always remember to null-terminate your strings (the last value in the
 * string should be 0) or the behavior of this function is UNDEFINED.
 */
extern char *xist_text;

/** \var unsigned char xist_text_tile_x
 *  \brief Acts as a virtual parameter for the function `xist_draw_text`.
 *  \see xist_draw_text
 *
 * Sets the X position (in TILE coordinates) for the leftmost character of the string
 * to be rendered by `xist_draw_text`.
 */
extern unsigned char xist_text_tile_x;

/** \var unsigned char xist_text_tile_y
 *  \brief Acts as a virtual parameter for the function `xist_draw_text`.
 *  \see xist_draw_text
 *
 * Sets the Y position (in TILE coordinates) for the row where you wish to render
 * the string with `xist_draw_text`.
 */
extern unsigned char xist_text_tile_y;

/** \var unsigned char xist_tw_text_tile_x
 *  \brief Acts as a virtual parameter for the function `xist_draw_typewriter_text`.
 *  \see xist_draw_typewriter_text
 *
 * Sets the X position (in TILE coordinates) for the upper-left corner of the strings
 * to be rendered by `xist_draw_typewriter_text`.
 */
extern unsigned char xist_tw_text_tile_x;

/** \var unsigned char xist_tw_text_tile_y
 *  \brief Acts as a virtual parameter for the function `xist_draw_typewriter_text`.
 *  \see xist_draw_typewriter_text
 *
 * Sets the Y position (in TILE coordinates) for the upper-left corner of the strings
 * to be rendered by `xist_draw_typewriter_text`.
 */
extern unsigned char xist_tw_text_tile_y;

/** \var unsigned char xist_palette_offset
 *  \brief Acts as a virtual parameter for the functions `xist_draw_text` and `xist_draw_typewriter_text`.
 *  \see xist_draw_text
 *  \see xist_draw_typewriter_text
 *
 * Set to a non-zero value to palette-shift the text to render it in different colors.
 * Use the macros defined in this file for easy access to the available colors.
 */
extern unsigned char xist_palette_offset;

/**
 * \fn void xist_clear_text()
 * \brief Clears all text glyphs from the screen (on tile layer 1).
 */
void xist_clear_text();

/**
 * \fn void xist_initialize_text_tiles()
 * \brief Initializes the Commander X16 state for use with Xist16's text drawing functionality
 *
 * Loads the sysf.img file which contains the font glyphs into VRAM and initializes all the tiles
 * on layer 1 to display no text (technically a space). Must be called before you use any other
 * text drawing functions. Typically you would call this early in your program's initialization
 * after calling `xist_initialize_tiles`.
 */
void xist_initialize_text_tiles();

/**
 * \fn void xist_draw_text()
 * \brief Renders the provided text string at the provided coordinates.
 * \see xist_text
 * \see xist_text_tile_x
 * \see xist_text_tile_y
 * \see xist_palette_offset
 *
 * As a performance optimization, this function uses global variables as virtual parameters.
 * When called, it draws the glyphs for the characters in the provided string left-aligned on
 * the provided coordinates.
 * Set `xist_text_tile_x` to the X position (in TILE coordinates) of the first character of
 * the string. Set `xist_text_tile_y` to the Y position (in TILE coordinates) of the row where
 * you want to draw the string. Point `xist_text` to the string you want rendered.
 * You can also set `xist_palette_offset` to a non-zero value to change the color of the text.
 * Use the macros defined in this file as an easy reference for the available colors.
 * IMPORTANT NOTE: Your strings must be arrays of raw ASCII codes, not hard-coded strings,
 * because cc65 encodes hard-coded strings using non-standard encoding that doesn't work with
 * Xist16's text drawing routines.
 * IMPORTANT NOTE 2: Always remember to null-terminate your strings (the last value in the
 * string should be 0) or the behavior of this function is UNDEFINED.
 */
void xist_draw_text();

/**
 * \fn void xist_reset_typewriter_counters()
 * \brief Resets the variables used to render typewriter text. Call before rendering typewriter text.
 */
void xist_reset_typewriter_counters();

/**
 * \fn void xist_draw_typewriter_text()
 * \brief Renders the next character in the multdimensional `xist_typewriter_text` array.
 * \see xist_typewriter_text
 * \see xist_tw_text_tile_x
 * \see xist_tw_text_tile_y
 * \see xist_palette_offset
 *
 * As a performance optimization, this function uses global variables as virtual parameters.
 * To use this function, fill the `xist_typewriter_text` multidimensional array with the strings
 * you wish to render one character at a time.
 * Then, call this function each time you want to render the next character in that array.
 * You can control the speed at which the typewriter text is rendered by how many game ticks
 * (i.e., iterations of your game loop) you wait in between the next call to this function.
 * I.e., if your game loop runs at 60 Hz, call this function once per iteration to render
 * 60 characters per second. Call it every other iteration to render 30 characters per
 * second, etc.
 * Set `xist_tw_text_tile_x` to the X position (in TILE coordinates) of the left column of
 * the text to be rendered.
 * Set `xist_text_tile_y` to the Y position (in TILE coordinates) of the top row of the text
 * to be rendered.
 * You can also set `xist_palette_offset` to a non-zero value to change the color of the text.
 * Use the macros defined in this file as an easy reference for the available colors.
 * IMPORTANT NOTE: Your strings must be arrays of raw ASCII codes, not hard-coded strings,
 * because cc65 encodes hard-coded strings using non-standard encoding that doesn't work with
 * Xist16's text drawing routines.
 * IMPORTANT NOTE 2: Always remember to null-terminate your strings (the last value in the
 * string should be 0) or the behavior of this function is UNDEFINED.
 */
void xist_draw_typewriter_text();

/**
 * \fn void xist_replace_substring(char *str, const char *placeholder, const char *replacement)
 * \brief Replaces the first occurrence of `placeholder` in `str` with `replacement`.
 * \param char* str The string containing the placeholder substring
 * \param char* placeholder The placeholder substring to be replaced; must be of equal or greater length than
 *                          the string that will replace it
 * \param char* replacement The string to replace the placeholder substring with
 *
 * Replaces the first occurrence of `placeholder` in `str` with `replacement`. `placeholder` must
 * be of equal or greater length to `replacement`.
 * IMPORTANT NOTE: Your strings must be arrays of raw ASCII codes, not hard-coded strings,
 * because cc65 encodes hard-coded strings using non-standard encoding that doesn't work with
 * Xist16's text drawing routines.
 * IMPORTANT NOTE 2: Always remember to null-terminate your strings (the last value in the
 * string should be 0) or the behavior of this function is UNDEFINED.
 */
void xist_replace_substring(char *str, const char *placeholder, const char *replacement);

#endif