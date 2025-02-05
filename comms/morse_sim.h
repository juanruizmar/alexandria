#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

enum{HIGH, LOW, OUTPUT};

const char *content(int value){
    switch(value){
        case HIGH: return "HIGH";
        case LOW: return "LOW";
        case OUTPUT: return "OUTPUT";
        default: return "UNKNOWN";
    }
}

void pinMode(int pin, int value){
    printf("pinMode(%i, %s)\n", pin, content(value));
}
void digitalWrite(int pin, int value){
    printf("digitalWrite(%i, %s)\n", pin, content(value));
}
void delay(int t){
    printf("wait during %i milliseconds..\n", t);
    sleep((int)((double)t/1000));
}