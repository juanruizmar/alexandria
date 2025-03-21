#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// The following functions take a double and return a double:
double square(double x) { return x * x; }
double cube(double x) { return x * x * x; }
double cubic_root(double x) { return cbrt(x); } 
double sigmoid(double x) { return 1 / (1 + exp(-x)); }
double exponential(double x) { return exp(x); }
double logarithm(double x) { return log(x); }

// This function returns a pointer to a function
double (*select_function(char function))(double) {
    switch (function) {
        case 's': return square;
        case 'c': return cube;
        case 'r': return cubic_root;
        case 'g': return sigmoid;
        case 'e': return exponential;
        case 'l': return logarithm;
        default: return NULL;
    }
}

// This function takes an array of function pointers and a double, and acts as a composition of functions:
double compose(double (*functions[])(double), int n, double x) {
    for (int i = 0; i < n; i++) x = functions[i](x);
    return x;
}

// This function takes an array of characters identifying functions and returns an array of function pointers
double (**composition_functions(char functions[], int n))(double) {
    double (**composed_functions)(double) = (double (**)(double))malloc(n * sizeof(double (*)(double)));
        for (int i = 0; i < n; i++) composed_functions[i] = select_function(functions[i]);
            return composed_functions;
}

int main() {
    // Here, a string of operands and a string of operators are used to calculate a composition of functions
    char functions[] = {'s', 'g', 'e', 'l'};
    double (**result_functions)(double) = composition_functions(functions, sizeof(functions) / sizeof(functions[0]));
    printf(
        "The result of the composition is: %f\n", 
        compose(result_functions, sizeof(functions) / sizeof(functions[0]), 2.0)
    );
    free(result_functions);
    return 0;
}
