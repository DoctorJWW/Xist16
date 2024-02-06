/**
 * \file     xist_utils.h
 * \brief    Miscellaneous utility functionality
 */

#ifndef XIST_UTILS_H
#define XIST_UTILS_H

#define XIST_RESOLUTION_WIDTH  320
#define XIST_RESOLUTION_HEIGHT 240

#define BOOL unsigned char
#define TRUE    1
#define FALSE   0
#define EVER    ;;

#define XIST_RANDOM_TABLE_LENGTH

/**
 * \def MAX_SIGNED_INT_CHARS
 * \brief Maximum number of characters (including negative sign and null terminator)
 *        for numerical data to be converted into text
 */
#define MAX_SIGNED_INT_CHARS 12

/** \var unsigned char xist_seed_a
 *  \brief Current index into one of the two tables that `xist_rand` uses to generate pseudorandom numbers.
 *
 * Either this variable, or `xist_seed_b`, or both, should be initialized to a random value at the start
 * of your program in order to generate a different sequence of pseudorandom numbers on every program run.
 *
 * \see xist_rand
 */
extern unsigned char xist_seed_a;

/** \var unsigned char xist_seed_b
 *  \brief Current index into one of the two tables that `xist_rand` uses to generate pseudorandom numbers.
 *
 * Either this variable, or `xist_seed_a`, or both, should be initialized to a random value at the start
 * of your program in order to generate a different sequence of pseudorandom numbers on every program run.
 *
 * \see xist_rand
 */
extern unsigned char xist_seed_b;

/** \var unsigned char xist_rand_min
 *  \brief Acts as a virtual parameter for the function `xist_rand`.
 *  \see xist_rand
 *
 * The minimum randomized value (inclusive) that you want returned by `xist_rand` the next time it is called.
 */
extern unsigned char xist_rand_min;

/** \var unsigned char xist_rand_max
 *  \brief Acts as a virtual parameter for the function `xist_rand`.
 *  \see xist_rand
 *
 * The maximum randomized value (inclusive) that you want returned by `xist_rand` the next time it is called.
 */
extern unsigned char xist_rand_max;

/**
 * \fn void xist_wait()
 * \brief Call this once per iteration of your main game loop to synchronize your game to 60 frames per second.
 */
void xist_wait();

/**
 * \fn unsigned char xist_convert_long_to_ascii_array(signed long num, char* result)
 * \brief Converts provided integer to its string representation
 *
 * \param   signed long num      Integer to be converted to a string representation
 * \param   char* result         Pointer to string for the number to be converted. Make sure the string is
 *                               long enough to contain the maximum possible number of digits, plus a null
 *                               terminator, plus (if applicable) a minus sign.
 * \return  unsigned char        The length of the resultant string
 */
unsigned char xist_convert_long_to_ascii_array(signed long num, char* result);

/**
 * \fn unsigned char xist_rand(void)
 * \brief Generates a random number between the values indicated by `xist_rand_min` and `xist_rand_max` (inclusive)
 * \see xist_rand_min
 * \see xist_rand_max
 *
 * As a performance optimization, this function uses global variables as virtual parameters.
 * When called, it returns a random number between the values set in `xist_rand_min` and `xist_rand_max`
 * (inclusive). The minimum and maximum possible values for this range are 0 and 255, because
 * this pseudorandom number generator is optimized for 8-bit numbers. If you need a bigger number,
 * you can just call this function twice and then concatenate the two returned numbers into a single
 * 16-bit integer with bit shifting. This function has a period of 65,536 which should be more than sufficient
 * for Commander X16 games.
 */
unsigned char xist_rand();

#endif