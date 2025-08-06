#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"

void lfsr_calculate(uint16_t *reg) {
    /* YOUR CODE HERE */
    uint16_t x0 = (*reg)&1;
    uint16_t x2 = (*reg >> 2)&1;
    uint16_t x3 = (*reg >> 3)&1;
    uint16_t x5 = (*reg >> 5)&1;
    uint16_t x = (x0^x2^x3^x5);
    (*reg) = ((*reg) >> 1) ^ (x << 15);
}

