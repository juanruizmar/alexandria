// Una función que muestra los 256 valores posibles en FP8

#include <stdio.h>
#include <iostream>
#include <math.h>
// fp8 es coma flotante de 8 bits
double fp8(unsigned char i) {
    int sign = (i >> 7) & 1; // Bit 7: Sign
    int exponent = ((i >> 3) & 0x0F) - 8; // Bits 3-6: Exponent, adjusted to range from -8 to 7
    int mantissa = i & 0x07; // Bits 0-2: Mantissa

    return (1 - 2 * sign) * pow(2, exponent) * (1 + mantissa / 8.0);
}

const char *exp_str(int n){
    switch(n){
        case -8: return "0b1000";
        case -7: return "0b1001";
        case -6: return "0b1010";
        case -5: return "0b1011";
        case -4: return "0b1100";
        case -3: return "0b1101";
        case -2: return "0b1110";
        case -1: return "0b1111";
        case 0: return "0b0000";
        case 1: return "0b0001";
        case 2: return "0b0010";
        case 3: return "0b0011";
        case 4: return "0b0100";
        case 5: return "0b0101";
        case 6: return "0b0110";
        case 7: return "0b0111";
        default: return "0b????";
    }
}

int main() {
    double values[256];
    for (int i = 0; i < 256; i++) values[i] = fp8(i);
    
    for (int i = 0; i < 256; i++) {
        int sign = (i >> 7) & 1; // Bit 7: Sign
        int exponent = ((i >> 3) & 0x0F) - 8; // Bits 3-6: Exponent, adjusted to range from -8 to 7
        int mantissa = i & 0x07; // Bits 0-2: Mantissa

        //obtener cadenas de caracteres con la codificación en binario de cada cosa
        char sign_str[2] = {sign + '0', '\0'};
        const char *exponent_str = exp_str(exponent);
        char mantissa_str[4] = {(mantissa >> 2) + '0', ((mantissa >> 1) & 1) + '0', (mantissa & 1) + '0', '\0'};

        printf("%s1.%s^%s = %f\n", sign_str, mantissa_str, exponent_str, values[i]);
    }

    return 0;
}