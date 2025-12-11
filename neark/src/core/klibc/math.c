#include "math.h"





uint64_t pow_64(uint64_t number, uint64_t power){
    uint64_t result = 1;
    for(int i = 0; i < power; i++){
        result *= number;
    }
    return result;
}

int abs(int number){
    if(number < 0) return number * -1;
    else return number;
}

int isdigit(int number){
    if(number > 0 && number < 10) return 1;
    return 0;
}

double fmod(double divisor, double dividend){
    return 0.0;
}