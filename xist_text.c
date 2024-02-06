#include <cx16.h>
#include "xist_mem.h"
#include "xist_text.h"
#include "xist_tiles.h"

#define TEXT_TABLE_CAP_A 0x0000UL
#define TEXT_TABLE_CAP_B 0x0040UL
#define TEXT_TABLE_CAP_C 0x0080UL
#define TEXT_TABLE_CAP_D 0x00C0UL
#define TEXT_TABLE_CAP_E 0x0100UL
#define TEXT_TABLE_CAP_F 0x0140UL
#define TEXT_TABLE_CAP_G 0x0180UL
#define TEXT_TABLE_CAP_H 0x01C0UL
#define TEXT_TABLE_CAP_I 0x0200UL
#define TEXT_TABLE_CAP_J 0x0240UL
#define TEXT_TABLE_CAP_K 0x0280UL
#define TEXT_TABLE_CAP_L 0x02C0UL
#define TEXT_TABLE_CAP_M 0x0300UL
#define TEXT_TABLE_CAP_N 0x0340UL
#define TEXT_TABLE_CAP_O 0x0380UL
#define TEXT_TABLE_CAP_P 0x03C0UL
#define TEXT_TABLE_CAP_Q 0x0400UL
#define TEXT_TABLE_CAP_R 0x0440UL
#define TEXT_TABLE_CAP_S 0x0480UL
#define TEXT_TABLE_CAP_T 0x04C0UL
#define TEXT_TABLE_CAP_U 0x0500UL
#define TEXT_TABLE_CAP_V 0x0540UL
#define TEXT_TABLE_CAP_W 0x0580UL
#define TEXT_TABLE_CAP_X 0x05C0UL
#define TEXT_TABLE_CAP_Y 0x0600UL
#define TEXT_TABLE_CAP_Z 0x0640UL

#define TEXT_TABLE_SMALL_A 0x0680UL
#define TEXT_TABLE_SMALL_B 0x06C0UL
#define TEXT_TABLE_SMALL_C 0x0700UL
#define TEXT_TABLE_SMALL_D 0x0740UL
#define TEXT_TABLE_SMALL_E 0x0780UL
#define TEXT_TABLE_SMALL_F 0x07C0UL
#define TEXT_TABLE_SMALL_G 0x0800UL
#define TEXT_TABLE_SMALL_H 0x0840UL
#define TEXT_TABLE_SMALL_I 0x0880UL
#define TEXT_TABLE_SMALL_J 0x08C0UL
#define TEXT_TABLE_SMALL_K 0x0900UL
#define TEXT_TABLE_SMALL_L 0x0940UL
#define TEXT_TABLE_SMALL_M 0x0980UL
#define TEXT_TABLE_SMALL_N 0x09C0UL
#define TEXT_TABLE_SMALL_O 0x0A00UL
#define TEXT_TABLE_SMALL_P 0x0A40UL
#define TEXT_TABLE_SMALL_Q 0x0A80UL
#define TEXT_TABLE_SMALL_R 0x0AC0UL
#define TEXT_TABLE_SMALL_S 0x0B00UL
#define TEXT_TABLE_SMALL_T 0x0B40UL
#define TEXT_TABLE_SMALL_U 0x0B80UL
#define TEXT_TABLE_SMALL_V 0x0BC0UL
#define TEXT_TABLE_SMALL_W 0x0C00UL
#define TEXT_TABLE_SMALL_X 0x0C40UL
#define TEXT_TABLE_SMALL_Y 0x0C80UL
#define TEXT_TABLE_SMALL_Z 0x0CC0UL

#define TEXT_TABLE_NUM_ZERO  0x0D00UL
#define TEXT_TABLE_NUM_ONE   0x0D40UL
#define TEXT_TABLE_NUM_TWO   0x0D80UL
#define TEXT_TABLE_NUM_THREE 0x0DC0UL
#define TEXT_TABLE_NUM_FOUR  0x0E00UL
#define TEXT_TABLE_NUM_FIVE  0x0E40UL
#define TEXT_TABLE_NUM_SIX   0x0E80UL
#define TEXT_TABLE_NUM_SEVEN 0x0EC0UL
#define TEXT_TABLE_NUM_EIGHT 0x0F00UL
#define TEXT_TABLE_NUM_NINE  0x0F40UL

#define TEXT_TABLE_EXCLAMATION  0x0F80UL
#define TEXT_TABLE_AT           0x0FC0UL
#define TEXT_TABLE_HASH         0x1000UL
#define TEXT_TABLE_DOLLAR       0x1040UL
#define TEXT_TABLE_PERCENT      0x1080UL
#define TEXT_TABLE_CARET        0x10C0UL
#define TEXT_TABLE_AMPERSAND    0x1100UL
#define TEXT_TABLE_ASTERISK     0x1140UL
#define TEXT_TABLE_OPEN_PAREN   0x1180UL
#define TEXT_TABLE_CLOSE_PAREN  0x11C0UL
#define TEXT_TABLE_OPEN_SQUARE  0x1200UL
#define TEXT_TABLE_CLOSE_SQUARE 0x1240UL
#define TEXT_TABLE_COMMA        0x1280UL
#define TEXT_TABLE_PERIOD       0x12C0UL
#define TEXT_TABLE_SLASH        0x1300UL
#define TEXT_TABLE_OPEN_ANGLE   0x1340UL
#define TEXT_TABLE_CLOSE_ANGLE  0x1380UL
#define TEXT_TABLE_QUESTION     0x13C0UL
#define TEXT_TABLE_TILDE        0x1400UL
#define TEXT_TABLE_ARROW        0x1440UL
#define TEXT_TABLE_DASH         0x1480UL
#define TEXT_TABLE_UNDERSCORE   0x14C0UL
#define TEXT_TABLE_EQUALS       0x1500UL
#define TEXT_TABLE_PLUS         0x1540UL
#define TEXT_TABLE_PIPE         0x1580UL
#define TEXT_TABLE_BACKSLASH    0x15C0UL
#define TEXT_TABLE_SEMICOLON    0x1600UL
#define TEXT_TABLE_COLON        0x1640UL
#define TEXT_TABLE_QUOTE        0x1680UL
#define TEXT_TABLE_DQUOTE       0x16C0UL
#define TEXT_TABLE_SPACE        0x1700UL

char xist_typewriter_text[XIST_TYPEWRITER_TEXT_ROWS][XIST_TYPEWRITER_TEXT_COLUMNS];
char *xist_text;
char typewriter_text_row;
char typewriter_text_column;
unsigned char xist_text_tile_x, xist_text_tile_y;
unsigned char xist_tw_text_tile_x, xist_tw_text_tile_y;
unsigned char xist_palette_offset;

unsigned short character_addresses[] = {
  TEXT_TABLE_CAP_A, TEXT_TABLE_CAP_B, TEXT_TABLE_CAP_C,
  TEXT_TABLE_CAP_D, TEXT_TABLE_CAP_E, TEXT_TABLE_CAP_F, TEXT_TABLE_CAP_G, TEXT_TABLE_CAP_H, TEXT_TABLE_CAP_I, TEXT_TABLE_CAP_J,
  TEXT_TABLE_CAP_K, TEXT_TABLE_CAP_L, TEXT_TABLE_CAP_M, TEXT_TABLE_CAP_N, TEXT_TABLE_CAP_O, TEXT_TABLE_CAP_P, TEXT_TABLE_CAP_Q,
  TEXT_TABLE_CAP_R, TEXT_TABLE_CAP_S, TEXT_TABLE_CAP_T, TEXT_TABLE_CAP_U, TEXT_TABLE_CAP_V, TEXT_TABLE_CAP_W, TEXT_TABLE_CAP_X,
  TEXT_TABLE_CAP_Y, TEXT_TABLE_CAP_Z,
  TEXT_TABLE_SMALL_A, TEXT_TABLE_SMALL_B, TEXT_TABLE_SMALL_C, TEXT_TABLE_SMALL_D,
  TEXT_TABLE_SMALL_E, TEXT_TABLE_SMALL_F, TEXT_TABLE_SMALL_G, TEXT_TABLE_SMALL_H, TEXT_TABLE_SMALL_I, TEXT_TABLE_SMALL_J,
  TEXT_TABLE_SMALL_K, TEXT_TABLE_SMALL_L, TEXT_TABLE_SMALL_M, TEXT_TABLE_SMALL_N, TEXT_TABLE_SMALL_O, TEXT_TABLE_SMALL_P,
  TEXT_TABLE_SMALL_Q, TEXT_TABLE_SMALL_R, TEXT_TABLE_SMALL_S, TEXT_TABLE_SMALL_T, TEXT_TABLE_SMALL_U, TEXT_TABLE_SMALL_V,
  TEXT_TABLE_SMALL_W, TEXT_TABLE_SMALL_X, TEXT_TABLE_SMALL_Y, TEXT_TABLE_SMALL_Z,
  TEXT_TABLE_NUM_ZERO, TEXT_TABLE_NUM_ONE, TEXT_TABLE_NUM_TWO, TEXT_TABLE_NUM_THREE, TEXT_TABLE_NUM_FOUR, TEXT_TABLE_NUM_FIVE,
  TEXT_TABLE_NUM_SIX, TEXT_TABLE_NUM_SEVEN, TEXT_TABLE_NUM_EIGHT, TEXT_TABLE_NUM_NINE,
  TEXT_TABLE_EXCLAMATION, TEXT_TABLE_AT, TEXT_TABLE_HASH, TEXT_TABLE_DOLLAR, TEXT_TABLE_PERCENT, TEXT_TABLE_CARET,
  TEXT_TABLE_AMPERSAND, TEXT_TABLE_ASTERISK, TEXT_TABLE_OPEN_PAREN, TEXT_TABLE_CLOSE_PAREN, TEXT_TABLE_OPEN_SQUARE,
  TEXT_TABLE_CLOSE_SQUARE,
  TEXT_TABLE_COMMA, TEXT_TABLE_PERIOD, TEXT_TABLE_SLASH, TEXT_TABLE_OPEN_ANGLE, TEXT_TABLE_CLOSE_ANGLE, TEXT_TABLE_QUESTION,
  TEXT_TABLE_TILDE, TEXT_TABLE_ARROW, TEXT_TABLE_DASH, TEXT_TABLE_UNDERSCORE, TEXT_TABLE_EQUALS, TEXT_TABLE_PLUS,
  TEXT_TABLE_PIPE, TEXT_TABLE_BACKSLASH, TEXT_TABLE_SEMICOLON, TEXT_TABLE_COLON, TEXT_TABLE_QUOTE, TEXT_TABLE_DQUOTE,
  TEXT_TABLE_SPACE
};

unsigned short character_x_offsets[] = {
  0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128, 136, 144, 152, 160, 168, 176, 184, 192, 200,
  0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128, 136, 144, 152, 160, 168, 176, 184, 192, 200,
  0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128, 136, 144, 152, 160, 168,
  0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128, 136, 144
};

// This maps the ASCII value of each character to the corresponding address of that character in VRAM
// The first actual meaningful character here starts at index 32, so all indexing into this table should
// be offset by that amount.
unsigned short text_table[96] = {
  92,  // space -- corresponding to ASCII code 32
  62,  // !
  91,  // "
  64,  // #
  65,  // $
  66,  // %
  68,  // &
  90,  // '
  70,  // (
  71,  // )
  69,  // *    [10]
  85,  // +
  74,  // ,
  82,  // -
  75,  // .
  76,  // /
  52,  // 0
  53,  // 1
  54,  // 2
  55,  // 3
  56,  // 4    [20]
  57,  // 5
  58,  // 6
  59,  // 7
  60,  // 8
  61,  // 9
  89,  // :
  88,  // ;
  77,  // <
  84,  // =
  78,  // >    [30]
  79,  // ?
  63,  // @
  0,   // A
  1,   // B
  2,   // C
  3,   // D
  4,   // E
  5,   // F
  6,   // G
  7,   // H    [40]
  8,   // I
  9,   // J
  10,  // K
  11,  // L
  12,  // M
  13,  // N
  14,  // O
  15,  // P
  16,  // Q
  17,  // R    [50]
  18,  // S
  19,  // T
  20,  // U
  21,  // V
  22,  // W
  23,  // X
  24,  // Y
  25,  // Z
  72,  // [
  87,  // \    [60]
  73,  // ]
  67,  // ^
  83,  // _
  81,  // actually a backtick, but it's an arrow in my character set
  26,  // a
  27,  // b
  28,  // c
  29,  // d
  30,  // e
  31,  // f    [70]
  32,  // g
  33,  // h
  34,  // i
  35,  // j
  36,  // k
  37,  // l
  38,  // m
  39,  // n
  40,  // o
  41,  // p    [80]
  42,  // q
  43,  // r
  44,  // s
  45,  // t
  46,  // u
  47,  // v
  48,  // w
  49,  // x
  50,  // y
  51,  // z    [90]
  70,  // {
  86,  // |
  71,  // }
  80,  // ~
  92 // invalid whitespace character
};


void xist_clear_text() {
  unsigned short n;
  
  // initialize all the tiles to black
  VERA.address = XIST_LAYER_1_MAPBASE;
  VERA.address_hi = XIST_LAYER_1_MAPBASE>>16;
  VERA.address_hi |= 0b10000; // Turn on increment mode of 1

  for (n=0; n < LAYER_1_MAPBASE_TOTAL; ++n) {
    VERA.data0 = 92;
    VERA.data0 = 0;
  }
}


void xist_initialize_text_tiles() {
  unsigned short n = 0;
  unsigned char x_offset = 0;
  unsigned char y_offset = 0;
  unsigned short character_address = TEXT_TABLE_CAP_A;
  
  xist_load_file_to_highram("sysf.img", 15);
  
  for (n = 0; n < 93; ++n) {
    character_address = character_addresses[n];
    x_offset = character_x_offsets[n];
    if (n < 26) y_offset = 0;
    else if (n < 52) y_offset = 8;
    else if (n < 74) y_offset = 16;
    else y_offset = 24;
    xist_copy_highram_to_vram_partial(character_address, 15, x_offset, y_offset, 8, 8, 208);
  }
  
  xist_clear_text();
}


void xist_draw_text() {
  unsigned long mapbase_address;
  unsigned short mapbase_address_width = LAYER_1_MAPBASE_WIDTH * 2; // x2 because each mapbase 'node' is 2 bytes
  char *xist_text_ptr = xist_text;
  
  if (*xist_text_ptr == 0) return;
  
  mapbase_address = XIST_LAYER_1_MAPBASE + (mapbase_address_width * xist_text_tile_y) + (xist_text_tile_x * 2);
  
  VERA.address = mapbase_address;
  VERA.address_hi = mapbase_address>>16;
  VERA.address_hi |= 0b10000; // Turn on increment mode

  while (*xist_text_ptr != 0) {
    VERA.data0 = text_table[*xist_text_ptr - 32]; // tile index
    VERA.data0 = xist_palette_offset;
    ++xist_text_ptr;
  }
}


void xist_reset_typewriter_counters() {
  typewriter_text_row = 0;
  typewriter_text_column = 0;
}


void xist_draw_typewriter_text() {
  unsigned long mapbase_address;
  unsigned short mapbase_address_width = LAYER_1_MAPBASE_WIDTH * 2;
  char character = xist_typewriter_text[typewriter_text_row][typewriter_text_column];
  
  xist_tw_text_tile_x += typewriter_text_column;
  xist_tw_text_tile_y += typewriter_text_row;

  if (typewriter_text_row >= XIST_TYPEWRITER_TEXT_ROWS) return;
  
  if (character == 0) {
    if (typewriter_text_row < XIST_TYPEWRITER_TEXT_ROWS) {
      typewriter_text_column = 0;
      ++typewriter_text_row;
    }
    return;
  }
  
  // draw the next character
  mapbase_address = XIST_LAYER_1_MAPBASE + (mapbase_address_width * xist_tw_text_tile_y) + (xist_tw_text_tile_x * 2);
  VERA.address = mapbase_address;
  VERA.address_hi = mapbase_address>>16;
  VERA.address_hi |= 0b10000; // Turn on increment mode
  VERA.data0 = text_table[character - 32]; // tile index
  VERA.data0 = xist_palette_offset;
  
  ++typewriter_text_column;
  if (typewriter_text_column >= XIST_TYPEWRITER_TEXT_COLUMNS) {
    ++typewriter_text_row;
    typewriter_text_column = 0;
  }
}


void xist_replace_substring(char *str, const char *placeholder, const char *replacement) {
  unsigned char n = 0, i = 0;
  char *found = str;

  while (*found) {
    n = 0;
    while (found[n] == placeholder[n] && placeholder[n]) ++n;

    if (placeholder[n] == 0) {
      i = 0;
      while (replacement[i]) {
          found[i] = replacement[i];
          ++i;
      }
      while (found[n] != 0) found[i++] = found[n++];
      found[i] = 0;
      break;
    }

    ++found;
  }
}

