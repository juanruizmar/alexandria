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

unsigned char ascii_to_binmorse(unsigned char c){
    static unsigned char codes[] = {1,2,3};
    return codes[c];
}

void turnLightOn(){
    digitalWrite(13, HIGH);
}
void turnLightOff(){
    digitalWrite(13, LOW);
}

void display_point(){
    turnLightOn()
    delay(point_length);
    turnLightOff()
    delay(token_separator_length);
}
void display_line(){
    turnLightOn()
    delay(line_length);
    turnLightOff()
    delay(token_separator_length);
}
void new_char(){
    delay(char_separator_length);
}
void new_word(){
    delay(word_separator_length);
}
void wait_forever(){
    turnLightOff()
    delay(8000);
    while(true){
    turnLightOn()
    delay(200);
    turnLightOff()
    delay(200);
    }
}

void print(unsigned char c){
    printf("%c\n")

}
void print_S(){
    display_point();
    display_point();
    display_point();
    new_char();
}

void print_O(){
    display_line();
    display_line();
    display_line();
    new_char();
}

void print(const char *message){
    delay(point_length); 
    //set_point_length(point_length*2);
}

void loop() {
    print('S');
    print('O');
    print('S');
}
