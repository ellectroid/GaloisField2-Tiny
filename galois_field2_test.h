#ifndef GALOIS_FIELD2_TEST_H
#define GALOIS_FIELD2_TEST_H

void print_bits_lsb_first(const unsigned char* in, unsigned long bytelen);
void print_bits_msb_first(const unsigned char* in, unsigned long bytelen);

int compare_results(const unsigned char* in1,
                    const unsigned char* in2,
                    unsigned long bytelen);

void gf2_reverse_bytes(unsigned char* out,
                       const unsigned char* in,
                       unsigned long bytelen);

#endif // GALOIS_FIELD2_TEST_H
