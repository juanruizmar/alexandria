#include <stdio.h>

union {
    unsigned char i;
    struct {
        unsigned char mantissa: 3;  // 3 bits for mantissa
        unsigned char exponent: 4;  // 4 bits for exponent
        unsigned char sign: 1;      // 1 bit for sign
    };
} fp8_union;

double two_pow(char exp) {
    double base;
    double result = 1.0;
    if (exp < 0) {
        base = 0.5;
        exp = -exp;
    }
    else base = 2.0;

    for (char i = 0; i < exp; i++)  result *= base;
    return result;
}

double fp8(unsigned char i) {
    fp8_union.i = i;

    double sign = (fp8_union.sign == 0) ? 1.0 : -1.0;
    double exponent = two_pow(fp8_union.exponent - 8);
    double mantissa = 0.0;

    if(fp8_union.exponent == 0){
        if(fp8_union.mantissa == 0) return 0.0;
        else mantissa = (fp8_union.mantissa / 4.0);
    }
    else if(fp8_union.exponent == 15){
        if(fp8_union.mantissa == 0) return sign * 1.0 / 0.0; // Inf
        else return sign * 0.0 / 0.0; // NaN
    }
    else if(fp8_union.exponent < 8) mantissa = (fp8_union.mantissa / 4.0);
    else mantissa = 1.0 + (fp8_union.mantissa / 8.0);

    return sign * mantissa * exponent;
}

int main() {
    double values[256];
    for (int i = 0; i < 256; i++) values[i] = fp8((unsigned char)i);
    
    for (int i = 0; i < 256; i++) {
        printf("fp8(%d) = %f\n", i, values[i]);
    }

    return 0;
}