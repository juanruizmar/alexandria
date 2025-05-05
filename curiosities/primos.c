#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned int *vectorPrimos = NULL;
unsigned int nPrimos = 0;

void inicializarVectorPrimos() {
    vectorPrimos = (unsigned int *) malloc(sizeof(unsigned int));
    vectorPrimos[0] = 2;
    nPrimos = 1;
}
void imprimirPrimos() {
    for(unsigned int i = 0; i < nPrimos; i++) printf("%u ", vectorPrimos[i]);
    printf("\n");
}

unsigned int mayorPrimoEncontrado() { return vectorPrimos[nPrimos-1]; }

void generarPrimo(){
    unsigned int candidato = mayorPrimoEncontrado();
    bool candidatoEsPrimo;

    do {
        candidato++;
        candidatoEsPrimo = true;
        for(unsigned int i = 0; i < nPrimos && candidatoEsPrimo && vectorPrimos[i] * vectorPrimos[i] <= candidato; i++)
            candidatoEsPrimo = (candidato % vectorPrimos[i] != 0);
    }while(!candidatoEsPrimo);

    vectorPrimos = (unsigned int *) realloc(vectorPrimos, (nPrimos + 1) * sizeof(unsigned int));
    vectorPrimos[nPrimos] = candidato;
    nPrimos++;
}

unsigned int nFactoresPrimos(unsigned int n) {
    unsigned int divisores = 0;
    for(int i=0; n!=1; ++i){
        if(i == nPrimos) generarPrimo();
        while(n % vectorPrimos[i] == 0){
            n /= vectorPrimos[i];
            divisores++;
        }
    }
    return divisores;
}

bool esPrimo(unsigned int n) {
    if(mayorPrimoEncontrado() > n) {
        int left = 0, right = nPrimos - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (vectorPrimos[mid] == n) return true;
            if (vectorPrimos[mid] < n) left = mid + 1;
            else right = mid - 1;
        }
        return false;
    }
    else{
        bool hayDivisores = false;
        for(int i=0; i<n && n!=1 && !hayDivisores; ++i){
            if(i == nPrimos) generarPrimo();
            if(n % vectorPrimos[i] == 0) hayDivisores = true;
        }
        return !hayDivisores;
    }
}

int main() {
    inicializarVectorPrimos();

    imprimirPrimos();
    
    for(int i=1; i<100; ++i) printf("El %d tiene %u divisores primos\n", i, nFactoresPrimos(i));
    
    imprimirPrimos();

    for(int i=1; i<100; ++i) {
        printf("El %d tiene %u divisores primos y %s primo\n", i, nFactoresPrimos(i), esPrimo(i) ? "es" : "no es");
    }

    printf("El 998 %s primo\n", esPrimo(998) ? "es" : "no es"); imprimirPrimos();
    printf("El 997 %s primo\n", esPrimo(997) ? "es" : "no es"); imprimirPrimos();

    free(vectorPrimos);
    return 0;
}
