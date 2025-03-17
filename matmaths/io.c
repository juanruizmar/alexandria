#include "io.h"

const char* scientific_double_to_str(double value, int precision){
    int size = snprintf(NULL, 0, "%.*e", precision, value);
    if(size<0) return NULL;

    char* buffer = (char*)malloc(size+1);
    if(!buffer) return NULL;

    snprintf(buffer, size + 1, "%.*e", precision, value);

    return buffer;
}
