#include "morse_sim.h"

#include <string.h>

#include <assert.h>

#define TURN_LIGHT_ON digitalWrite(13, HIGH);
#define TURN_LIGHT_OFF digitalWrite(13, LOW);

#define IS_A_LETTER(c) ('a'<=c && c<='z')
#define GET_INDEX_AS_A_LETTER(c) (c-'a')

#define IS_A_DIGIT_OR_CLOSE(c) ('&'<=c && c<='9')
#define GET_INDEX_AS_A_DIGIT_OR_CLOSE(c) (c-'&')

#define IS_UPPER_CASE(c) ('A'<=c && c<='Z')
#define AS_LOWER_CASE(c) (c+='a'-'A')

const char *message = "HOLA MUNDO Y 3 TRISTES TIGRES COMEN TRIGO EN UN TRIGAL";

int point_length=400;
double token_separator_ratio=0.5;   // How many points between tokens
double char_separator_ratio=0.8;    // How many points between chars
double word_separator_ratio=4;      // How many points between words
double line_point_ratio=2.5;        // How many points a line are.

int line_length;
int token_separator_length;
int char_separator_length;
int word_separator_length;

void setup() {
    pinMode(13, OUTPUT);
    line_length = (int)(point_length*line_point_ratio);
    token_separator_length = (int)(point_length*token_separator_ratio);
    char_separator_length = (int)(point_length*char_separator_ratio);
    word_separator_length = (int)(point_length*word_separator_ratio);
}

void printPoint(){
    putchar('.');
}
void printLine(){
    putchar('-');
}
void printNewChar(){
    putchar(' ');
    //delay(char_separator_length);
}
void printNewWord(){
    putchar('\n');
    //delay(word_separator_length);
}
void endOfTransmission(){
    putchar('\n');
    fflush(stdout);
    printf(" ## END OF TRANSMISSION ##\n");
}

unsigned char printMorsebin(unsigned char c){
    if(c==0) printNewWord();
    else{
        while(c>1){
            if(c%2) printLine();
            else printPoint();
            c>>=1; 
        }
        printNewChar();
    }
}

unsigned char morsebin(unsigned char c){
    assert(c!='%'); // Procesado a parte (0/0)
    // Morsebin:
    //  - Punto: 0, Línea: 1. Se lee de derecha a izquierda, y se añade un 1 al final
    // El '0' deberá interpretarse como espacio entre palabras
    static unsigned char lowerCaseAlphabet[] = {
        0b110,      // A
        0b10001,    // B
        0b10101,    // C
        0b1001,     // D
        0b10,       // E
        0b10100,    // F
        0b1011,     // G
        0b10000,    // H
        0b100,      // I
        0b11110,    // J
        0b1101,     // K
        0b10010,    // L
        0b111,      // M
        0b101,      // N
        0b1111,     // O
        0b10110,    // P
        0b11011,    // Q
        0b1010,     // R
        0b1000,     // S
        0b11,       // T
        0b1100,     // U
        0b11000,    // V
        0b1110,     // W
        0b11001,    // X
        0b11101,    // Y
        0b11100,    // Z
    };
    static unsigned char digitsAndCloseToDigits[] = {
        0b100010,   // Ampersand        &
        0b1011110,  // Apostrophe       '
        0b110010,   // Open brackets    (
        0b1101101,  // Close brackets   )
        0b11001,    // Asterisk,        *, readen as 'x'
        0b101010,   // Plus             +
        0b1110011,  // Comma            ,
        0b1100001,  // Hyphen           -
        0b1101010,  // Point            .
        0b101001,   // Slash            /
        0b111111,   // 0
        0b111110,   // 1
        0b111100,   // 2
        0b111000,   // 3
        0b110000,   // 4
        0b100000,   // 5
        0b100001,   // 6
        0b100011,   // 7
        0b100111,   // 8
        0b101111,   // 9
    };

    if(IS_A_LETTER(c)) return lowerCaseAlphabet[GET_INDEX_AS_A_LETTER(c)];
    else if(IS_A_DIGIT_OR_CLOSE(c)) return digitsAndCloseToDigits[GET_INDEX_AS_A_DIGIT_OR_CLOSE(c)];
    else{
        if(c==' ') return 0;
        else if(c=='!') return 0b1010110;
        else if(c=='\"') return 0b1010110;
        else if(c=='@') return 0b1010110;
        else if(c==':') return 0b1111000;
        else if(c=='=') return 0b110001;
        else if(c=='?') return 0b1001100;
        else assert(false);
    }
}

void display_point(){
    digitalWrite(13, HIGH);
    delay(point_length);
    digitalWrite(13, LOW);
    delay(token_separator_length);
}
void display_line(){
    digitalWrite(13, HIGH);
    delay(line_length);
    digitalWrite(13, LOW);
    delay(token_separator_length);
}
void waitForever(){
    while(true);
}

void print(unsigned char c){
    printf("MorseBin: %x, y en ascii %x (%c)\n", morsebin(c), c, c);
    //printMorsebin(morsebin(c));
}

void processChar(char c){
    if(IS_UPPER_CASE(c)) print(AS_LOWER_CASE(c));
    else if(c=='%'){
        print('0');
        print('/');
        print('0');
    }
    else print(c);
}
void processString(const char *c){
    for(; *c; ++c) processChar(*c);
    endOfTransmission();
}

void loop(){
    //processString("los numeros del 0 al 9 son 0 1 2 3 4 5 6 7 8 & 9");
    //processString("el 0 % de 100 es 0");
    processString("!\":=?@");
    waitForever();
}