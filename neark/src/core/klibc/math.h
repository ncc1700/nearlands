#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED






#include <stdint.h>


uint64_t pow_64(uint64_t number, uint64_t power);
#define pow pow_64
int abs(int number);
int isdigit(int number);
double log10(double x);
double fmod(double divisor, double dividend);
#endif