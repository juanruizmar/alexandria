#include <stdio.h>
#include <unistd.h>

#include "arduino_simulator.h"

const char *toString(int value){
    switch(value){
        case HIGH: return "HIGH";
        case LOW: return "LOW";
        case OUTPUT: return "OUTPUT";
        default: return "UNKNOWN";
    }
}

void pinMode(int pin, int value) { printf("pinMode(%i, %s)\n", pin, toString(value)); }
void digitalWrite(int pin, int value) { printf("digitalWrite(%i, %s)\n", pin, toString(value)); }
void delay(int t){
    printf("wait during %i milliseconds..\n", t);
    sleep((int)((double)t/1000));
}
