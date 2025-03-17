#include "io.h"

const char* doubleToStringWithScientific(double value, int precision){
    int size;
    char* buffer;

    if(1<=value && value<10){
        size = snprintf(NULL, 0, "%.*f", precision, value);
        if(size<0) return NULL;

        char* buffer = (char*)malloc(size+1);
        if(!buffer) return NULL;

        snprintf(buffer, size+1, "%.*f", precision, value);
    }else{
        size = snprintf(NULL, 0, "%.*e", precision, value);
        if(size<0) return NULL;

        char* buffer = (char*)malloc(size+1);
        if(!buffer) return NULL;

        snprintf(buffer, size+1, "%.*e", precision, value);

        char* exp_ptr = strchr(buffer, 'e');
        if(exp_ptr && *(exp_ptr+1)=='+') memmove(exp_ptr+1, exp_ptr+2, strlen(exp_ptr+2)+1);

    }
    return buffer;
}
