int dotLength;                      // The time for send a dot, in milli-seconds
const int dashPointRatio=3;         // How many dots a dash are.
const int tokenSeparatorRatio=1;    // How many dots between tokens
const int charSeparatorRatio=3;     // How many dots between chars
const int wordSeparatorRatio=7;     // How many dots between words

#define IS_A_LETTER(c) ('a'<=c && c<='z')
#define GET_INDEX_AS_A_LETTER(c) (c-'a')

#define IS_A_DIGIT_OR_CLOSE(c) ('&'<=c && c<='9')
#define GET_INDEX_AS_A_DIGIT_OR_CLOSE(c) (c-'&')

#define IS_A_LETTER_WITH_ACCENT(c) ('ü'<=c && c<='ì')
#define GET_INDEX_AS_LETTER_WITH_ACCENT(c) (c-'ü')

#define IS_UPPER_CASE(c) ('A'<=c && c<='Z')
#define AS_LOWER_CASE(c) (c+='a'-'A')

#define TURN_LIGHT_ON digitalWrite(13, HIGH);
#define TURN_LIGHT_OFF digitalWrite(13, LOW);

#define dashLength (dotLength*dashPointRatio)
#define tokenSeparatorLength (dotLength*tokenSeparatorRatio)
#define charSeparatorLength (dotLength*charSeparatorRatio)
#define wordSeparatorLength (dotLength*wordSeparatorRatio)

void displayDot(){
    TURN_LIGHT_ON;
    delay(dotLength);
    TURN_LIGHT_OFF;
    delay(tokenSeparatorLength);
}
void displayDash(){
    TURN_LIGHT_ON;
    delay(dashLength);
    TURN_LIGHT_OFF;
    delay(tokenSeparatorLength);
}
void displayNewChar() { delay(charSeparatorLength); }
void displayNewWord() { delay(wordSeparatorLength); }

unsigned char displayMorsebin(unsigned char c){
    if(c==0) displayNewWord();
    else{
        while(c>1){
            if(c%2) displayDash();
            else displayDot();
            c>>=1; 
        }
        displayNewChar();
    }
}

unsigned char morsebin(unsigned char c){
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
        else if(c=='\n') return 0b11010;
        else if(c=='!') return 0b1010110;
        else if(c=='\"') return 0b1010110;
        else if(c=='@') return 0b1010110;
        else if(c==':') return 0b1111000;
        else if(c=='=') return 0b110001;
        else if(c=='?') return 0b1001100;
        else if(c=='>') return 0b101000;    //  Here, and only here, "UNDERSTOOD"
        else if(c=='$') return 0b1101;      //  Here, and only here, "INVITATION"
    }
}
unsigned char morsebinSpecial(unsigned char prefix, unsigned char c){ // With prefix 0xc3 (195) or 0xc2 (194)
    if(prefix==194){
        if(c==161) return morsebin('?');                    // ¿
        else if(c==191) return morsebin('!');               // ¡
    }
    else{
        if(160<=c && c<=165) return morsebin('a');          // à á â ã ä å
        else if(c==166) return 0b11010;                     // æ
        else if(c==167) return 0b100101;                    // ç
        else if(168<=c && c<=171) return morsebin('e');     // è é ê ë
        else if(172<=c && c<=175) return morsebin('i');     // ì í î ï
        else if(c==176) return 0b101100;                    // ð
        else if(c==177) return 0b111011;                    // ñ
        else if(178<=c && c<=182) return morsebin('o');     // ò ó ô õ ö
        else if(c==184) return 0b10111;                     // ø
        else if(185<=c && c<=188) return morsebin('u');     // ù ú û ü
        else if(c==189 || c==191) return morsebin('y');     // ý ÿ
        else if(c==190) return 0b100110;                    // þ
    }
}

void displaySpecial(unsigned char prefix, unsigned char c) { displayMorsebin(morsebinSpecial(prefix, c)); }
void display(unsigned char c) { displayMorsebin(morsebin(c)); }

void processSpecialChar(unsigned char prefix, unsigned char c){
    if(prefix==195 && c==159){
        display('s');
        display('s');
    }
    else displaySpecial(prefix, c);
}
void processChar(unsigned char c){
    if(IS_UPPER_CASE(c)) display(AS_LOWER_CASE(c));
    else if(c=='%'){
        display('0');
        display('/');
        display('0');
    }
    else display(c);
}

void startOfTransmission() { displayMorsebin(0b110101); }
void endOfTransmission() { displayMorsebin(0b1101000); }
void displayErrorCode() { for(int i=0; i<8; ++i) displayDot(); displayNewChar(); }  // Not in use, yet

void processString(const char *input_ptr){
    startOfTransmission();
    for(const unsigned char *c=(const unsigned char *)input_ptr; *c; ++c){
        if(*c==195 || *c==194){
            processSpecialChar(*c, *(c+1));
            ++c;
        }
        else processChar(*c);
    }
    endOfTransmission();
}

void setup(){
    pinMode(13, OUTPUT);
    dotLength = 400;
}
void loop(){
    processString("En un lugar de la Mancha, de cuyo nombre no quiero acordarme, no ha mucho tiempo que vivía un hidalgo\n");
    processString("los números del 0 al 9 son 0, 1, 2, 3, 4, 5, 6, 7, 8 & 9");
    processString("el 0 % de 100 es 0");
}
