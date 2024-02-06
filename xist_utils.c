#include "xist_utils.h"

unsigned char xist_random_table[XIST_RANDOM_TABLE_LENGTH] = {
  10, 60, 245, 164, 132, 46, 253, 214, 16, 200, 79, 172, 57, 117, 27, 64, 230, 160, 35, 106, 37, 191, 194, 52, 44, 24, 151, 227, 47, 212, 182, 195, 61, 218, 189, 211, 8, 162, 187, 21, 124, 168, 70, 137, 76, 180, 247, 254, 206, 63, 139, 252, 4, 126, 179, 240, 207, 15, 38, 85, 25, 183, 87, 157, 107, 19, 232, 6, 186, 102, 74, 121, 32, 77, 197, 242, 219, 131, 112, 125, 114, 45, 91, 176, 205, 150, 193, 220, 72, 246, 171, 59, 143, 101, 177, 223, 118, 43, 82, 80, 108, 142, 226, 17, 34, 134, 123, 228, 135, 116, 159, 122, 14, 78, 169, 233, 18, 109, 250, 65, 103, 170, 222, 213, 36, 31, 146, 221, 167, 231, 2, 174, 216, 93, 161, 95, 188, 86, 165, 42, 111, 54, 68, 199, 140, 149, 98, 203, 90, 75, 145, 155, 55, 50, 152, 156, 39, 147, 1, 136, 113, 88, 92, 251, 243, 185, 184, 153, 26, 244, 67, 115, 234, 9, 255, 209, 133, 22, 73, 173, 163, 249, 204, 0, 154, 235, 97, 198, 56, 49, 130, 119, 236, 127, 239, 158, 238, 99, 62, 210, 66, 141, 33, 104, 148, 229, 13, 20, 40, 201, 225, 30, 178, 5, 217, 94, 23, 175, 166, 84, 51, 48, 81, 58, 208, 224, 105, 190, 71, 29, 69, 241, 96, 192, 100, 28, 83, 129, 181, 120, 202, 196, 110, 128, 12, 89, 248, 138, 215, 144, 237, 41, 3, 7, 53, 11
};

unsigned long start, next;
unsigned char xist_seed_a, xist_seed_b, xist_rand_min, xist_rand_max;
unsigned char random_counter = 0;

void xist_wait() {
    asm("jsr $FFDE");
    asm("sta %v", start);
    do {
        asm("jsr $FFDE");
        asm("sta %v", next);
    }
    while (start == next);
}


unsigned char xist_convert_long_to_ascii_array(signed long num, char* result) {
  signed char length = 0;
  signed char i, j, temp;
  BOOL negative = FALSE;
  
  if (num == 0) {
    result[length++] = 48; // ascii code for 0
  } else {
    if (num < 0) {
      num = -num;
      negative = TRUE;
    }

    while (num > 0 && length < MAX_SIGNED_INT_CHARS) {
      result[length++] = (48 + (num % 10));
      num /= 10;
    }
  }
  
  // It's backwards, so reverse it
  for (i = 0, j = length - 1; i < j; ++i, --j) {
    temp = result[i];
    result[i] = result[j];
    result[j] = temp;
  }
  
  // Add a - to the front if it was a negative number
  if (negative) {
    for (j = length - 1; j >= 0; --j) result[j+1] = result[j];
    result[0] = 45; // ascii code for -
  }

  if (negative) ++length;
  result[length] = 0; // null terminator

  return length;
}


unsigned char xist_rand() {
  unsigned char range = xist_rand_max - xist_rand_min + 1;
  unsigned char feedback = ((xist_seed_a >> 1) ^ (xist_seed_a >> 2) ^ (xist_seed_a >> 3) ^ (xist_seed_a >> 4)) & 1;
  xist_seed_a = (xist_seed_a >> 1) | (feedback << 7);
  xist_seed_a ^= xist_random_table[xist_seed_b];
  
  ++random_counter;
  if (random_counter == 255) ++xist_seed_b;

  return xist_rand_min + (xist_seed_a % range);
}
