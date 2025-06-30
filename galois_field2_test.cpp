#include "galois_field2_test.h"
#include <stdio.h>

void print_bits_lsb_first(const unsigned char* in, unsigned long bytelen) {
    unsigned long i;
    for (i = 0; i < bytelen; ++i) {
        int b;
        for (b = 0; b < 8; ++b)
            printf("%d", (in[i] >> b) & 1);
        printf(" ");
    }
    printf("\n");
}

void print_bits_msb_first(const unsigned char* in, unsigned long bytelen) {
    unsigned long i;
    for (i = 0; i < bytelen; ++i) {
        int b;
        for (b = 7; b >= 0; --b)
            printf("%d", (in[i] >> b) & 1);
        printf(" ");
    }
    printf("\n");
}

int compare_results(const unsigned char* in1, const unsigned char* in2, unsigned long bytelen) {
    unsigned long i;
    for (i = 0; i < bytelen; ++i)
        if (in1[i] != in2[i])
            return 0;
    return 1;
}

void gf2_reverse_bytes(unsigned char* out, const unsigned char* in, unsigned long bytelen) {
    unsigned long i;
    for (i = 0; i < bytelen; ++i)
        out[i] = in[bytelen - 1 - i];
}
