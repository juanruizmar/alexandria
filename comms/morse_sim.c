#include <stdio.h>

#include "morse.c"

int main(){
    printf("Setup: \n");
    printf("========================= \n");
    setup();
    printf("Loop: \n");
    while(true){
        printf("========================= \n");
        loop();
    }
}