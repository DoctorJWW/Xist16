#include "xist_input.h"

unsigned char xist_get_joypad_state_high;
unsigned char xist_get_joypad_state_low;

unsigned char xist_get_mouse_btn_state;

unsigned short *xist_mouse_x = (unsigned short *)2;
unsigned short *xist_mouse_y = (unsigned short *)4;

// Gets the two joypad status bytes and returns them as a combined short.
// Call this and use the #defined bitmasks to check which buttons are pressed.
// The return is inverted so 1 means pressed and 0 means not pressed.
unsigned short xist_get_joypad() {
  asm("lda #0");
  asm("jsr $FF56");
  asm("STA %v", xist_get_joypad_state_high);
  asm("STX %v", xist_get_joypad_state_low);
  return ~((unsigned short)xist_get_joypad_state_high << 8 | xist_get_joypad_state_low);
}


// "falling edge" means a button was pressed last frame and has just been released
unsigned short xist_joypad_get_falling_edges(unsigned short last_state) {
  unsigned short current_state, buttons_changed;
  current_state = xist_get_joypad();
  buttons_changed = current_state ^ last_state; // buttons that have changed state
  return buttons_changed & last_state; // buttons that were just released specifically
}


void xist_update_mouse_position() {
  asm("ldx #2");
  asm("jsr $FF6B");
}


unsigned char xist_get_mouse_buttons() {
  asm("ldx #2");
  asm("jsr $FF6B");
  asm("sta %v", xist_get_mouse_btn_state);
  return xist_get_mouse_btn_state;
}


unsigned char xist_get_mouse_buttons_get_falling_edges(unsigned char last_state) {
  unsigned char current_state, buttons_changed;
  current_state = xist_get_mouse_buttons();
  buttons_changed = current_state ^ last_state; // buttons that have changed state
  return buttons_changed & last_state; // buttons that were just released specifically
}
