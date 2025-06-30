#include "galois_field2.h"

long gf2_degree_lsb(const unsigned char* in, unsigned long bytelen) {
    long i;
    for (i = (long)bytelen - 1; i >= 0; --i) {
        if (in[i]) {
            int bit;
            for (bit = 7; bit >= 0; --bit) {
                if (in[i] & (1 << bit))
                    return (long)(i * 8 + bit);
            }
        }
    }
    return -1;
}

void gf2_multiply_lsb(const unsigned char* in1, const unsigned char* in2, unsigned char* out, unsigned long bytelen) {
    unsigned long total_bits = bytelen * 8;
    unsigned long out_bytelen = bytelen * 2;
    unsigned long i, j;

    for (i = 0; i < out_bytelen; ++i)
        out[i] = 0;

    for (i = 0; i < total_bits; ++i) {
        unsigned char bit1 = (in1[i >> 3] >> (i & 7)) & 1;
        if (!bit1) continue;

        for (j = 0; j < total_bits; ++j) {
            unsigned char bit2 = (in2[j >> 3] >> (j & 7)) & 1;
            if (!bit2) continue;

            unsigned long k = i + j;
            out[k >> 3] ^= (1U << (k & 7));
        }
    }
}

void gf2_reduce_lsb(unsigned char* inout_reducible, unsigned long reducible_bytelen, const unsigned char* in_reducer, unsigned long reducer_bytelen) {
    long deg_r = gf2_degree_lsb(inout_reducible, reducible_bytelen);
    long deg_d = gf2_degree_lsb(in_reducer, reducer_bytelen);

    if (deg_r < 0 || deg_d < 0)
        return;

    while (deg_r >= deg_d) {
        unsigned long shift = (unsigned long)(deg_r - deg_d);
        unsigned long byte_shift = shift >> 3;
        unsigned long bit_shift = shift & 7;
        unsigned long i;

        for (i = 0; i < reducer_bytelen; ++i) {
            if (i + byte_shift < reducible_bytelen) {
                unsigned char lo = in_reducer[i] << bit_shift;
                inout_reducible[i + byte_shift] ^= lo;
            }
            if (bit_shift && (i + byte_shift + 1) < reducible_bytelen) {
                unsigned char hi = in_reducer[i] >> (8 - bit_shift);
                inout_reducible[i + byte_shift + 1] ^= hi;
            }
        }

        deg_r = gf2_degree_lsb(inout_reducible, reducible_bytelen);
    }
}
