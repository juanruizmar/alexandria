#include <assert.h>

#include "4B5B.h"

unsigned char code(unsigned char input){
    assert(input<16);

    switch(input){
        case 0b0000: return 0b11110;
        case 0b0001: return 0b01001;
        case 0b0010: return 0b10100;
        case 0b0011: return 0b10101;
        case 0b0100: return 0b01010;
        case 0b0101: return 0b01011;
        case 0b0110: return 0b01110;
        case 0b0111: return 0b01111;
        case 0b1000: return 0b10010;
        case 0b1001: return 0b10011;
        case 0b1010: return 0b10110;
        case 0b1011: return 0b10111;
        case 0b1100: return 0b11010;
        case 0b1101: return 0b11011;
        case 0b1110: return 0b11100;
        case 0b1111: return 0b11101;
    }
}
unsigned char decode(unsigned char input){
    assert(input<32);

    switch(input){
        case 0b11110: return 0b0000;
        case 0b01001: return 0b0001;
        case 0b10100: return 0b0010;
        case 0b10101: return 0b0011;
        case 0b01010: return 0b0100;
        case 0b01011: return 0b0101;
        case 0b01110: return 0b0110;
        case 0b01111: return 0b0111;
        case 0b10010: return 0b1000;
        case 0b10011: return 0b1001;
        case 0b10110: return 0b1010;
        case 0b10111: return 0b1011;
        case 0b11010: return 0b1100;
        case 0b11011: return 0b1101;
        case 0b11100: return 0b1110;
        case 0b11101: return 0b1111;
        default: return 16;
    }
}

void inc_index(int *quotient, int *modulus){
    *modulus += 5;
    if(*modulus>=8){
        *modulus -= 8;
        *quotient += 1;
    }
}
