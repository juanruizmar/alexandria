#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

#include "arduino_simulator.h"

#define IS_A_LETTER(c) ('a'<=c && c<='z')
#define GET_INDEX_AS_A_LETTER(c) (c-'a')

#define IS_A_DIGIT_OR_CLOSE(c) ('&'<=c && c<='9')
#define GET_INDEX_AS_A_DIGIT_OR_CLOSE(c) (c-'&')

#define IS_A_LETTER_WITH_ACCENT(c) ('ü'<=c && c<='ì')
#define GET_INDEX_AS_LETTER_WITH_ACCENT(c) (c-'ü')

#define IS_UPPER_CASE(c) ('A'<=c && c<='Z')
#define AS_LOWER_CASE(c) (c+='a'-'A')
