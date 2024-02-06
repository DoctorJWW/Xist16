/**
 * \file     xist_input.h
 * \brief    User input functionality.
 *
 * Contains macros, extern variables, and function signatures
 * for handling user input from the joypad and mouse.
 */

#ifndef XIST_INPUT_H
#define XIST_INPUT_H

#define JOYPAD_B   0b1000000000000000U
#define JOYPAD_Y   0b0100000000000000U
#define JOYPAD_SEL 0b0010000000000000U
#define JOYPAD_STA 0b0001000000000000U
#define JOYPAD_U   0b0000100000000000U
#define JOYPAD_D   0b0000010000000000U
#define JOYPAD_L   0b0000001000000000U
#define JOYPAD_R   0b0000000100000000U
#define JOYPAD_A   0b0000000010000000U
#define JOYPAD_X   0b0000000001000000U
#define JOYPAD_LT  0b0000000000100000U
#define JOYPAD_RT  0b0000000000010000U

#define MOUSE_L    0b00000001U
#define MOUSE_R    0b00000010U
#define MOUSE_M    0b00000100U

/** \var unsigned short* xist_mouse_x
 *  \brief Contains the X position of the mouse cursor (in pixels) after calling `xist_update_mouse_position`
 *  \see xist_update_mouse_position
 */
extern unsigned short *xist_mouse_x;

/** \var unsigned short* xist_mouse_y
 *  \brief Contains the Y position of the mouse cursor (in pixels) after calling `xist_update_mouse_position`
 *  \see xist_update_mouse_position
 */
extern unsigned short *xist_mouse_y;

/**
 * \fn unsigned short xist_get_joypad(void)
 * \brief Returns the current state of the joypad.
 *
 * Returns the current state of the joypad concatenated into a single 16-bit integer and inverted
 * so that 1 = button is pressed, 0 = button is not pressed. See the macro defines in this file
 * which you can AND the returned short with to check for button presses.
 *
 * \return   unsigned short
 */
unsigned short xist_get_joypad();

/**
 * \fn unsigned short xist_joypad_get_falling_edges(unsigned short last_state)
 * \brief Returns which joypad buttons are on a falling edge (have just been pressed and then released)
 *
 * Returns which joypad buttons that were pressed in the passed-in state are now released (i.e.,
 * are on a falling edge).
 * 1 = button is pressed, 0 = button is not pressed. See the macro defines in this file
 * which you can AND the returned short with to check for falling edges.
 *
 * \param    unsigned short last_state The state of the joypad during the last game tick. Required
 *                                     to tell which buttons were previously pressed and are now released.
 * \return   unsigned short
 */
unsigned short xist_joypad_get_falling_edges(unsigned short last_state);

/**
 * \fn void xist_update_mouse_position(void)
 * \brief Copies the current mouse cursor coordinates into `xist_mouse_x` and `xist_mouse_y`.
 * \see xist_mouse_x
 * \see xist_mouse_y
 * \return   void
 */
void xist_update_mouse_position();

/**
 * \fn unsigned char xist_get_mouse_buttons(void)
 * \brief Returns the current state of the mouse buttons.
 *
 * Returns the current state of the mouse buttons, where
 * 1 = button is pressed, 0 = button is not pressed. See the macro defines in this file
 * which you can AND the returned short with to check for button presses.
 *
 * \return   unsigned char
 */
unsigned char xist_get_mouse_buttons();

/**
 * \fn unsigned char xist_get_mouse_buttons_get_falling_edges(unsigned char last_state)
 * \brief Returns which mouse buttons are on a falling edge (have just been pressed and then released)
 *
 * Returns which mouse buttons that were pressed in the passed-in state are now released (i.e.,
 * are on a falling edge).
 * 1 = button is pressed, 0 = button is not pressed. See the macro defines in this file
 * which you can AND the returned short with to check for falling edges.
 *
 * \param    unsigned char  last_state The state of the mouse buttons during the last game tick. Required
 *                                     to tell which buttons were previously pressed and are now released.
 * \return   unsigned char
 */
unsigned char xist_get_mouse_buttons_get_falling_edges(unsigned char last_state);

#endif