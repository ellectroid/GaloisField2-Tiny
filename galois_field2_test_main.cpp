#include <iostream>
#include <iomanip>

#include "galois_field2.h"
#include "galois_field2_test.h"

int main()
{
    std::cout << "\n=== GF(2) Multiplication Test Vectors ===" << std::endl;

    struct MultiplyTestCase {
        const char* name;
        unsigned char in1[8];
        unsigned char in2[8];
        size_t bytelen;
        unsigned char expected[16];
    };

    MultiplyTestCase mult_tests[] = {
        {
            // Classic multiplication example from Wikipedia
            "Wikipedia 8×8 (0xA2 × 0x96)",
            { 0xA2 }, { 0x96 }, 1,
            { 0xEC, 0x58 }
        },
        {
            // Identity test: a × 1 = a (64-bit)
            "Identity 64-bit",
            { 0xF0, 0xDE, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12 },
            { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
            8,
            { 0xF0, 0xDE, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
        },
        {
            // Multiply highest set bit: x^63 × x^63 = x^126
            "x^63 × x^63 = x^126",
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80 },
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80 },
            8,
            { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40 }
        }
    };

    for (const auto& test : mult_tests) {
        unsigned char actual[16] = {0};

        gf2_multiply_lsb(test.in1, test.in2, actual, test.bytelen);

        bool pass = compare_results(actual, test.expected, test.bytelen * 2);

        std::cout << "\n=== " << test.name << " ===" << std::endl;
        std::cout << "in1      (MSB): "; print_bits_msb_first(test.in1, test.bytelen);
        std::cout << "in2      (MSB): "; print_bits_msb_first(test.in2, test.bytelen);
        std::cout << "Expected (LSB): "; print_bits_lsb_first(test.expected, test.bytelen * 2);
        std::cout << "Actual   (LSB): "; print_bits_lsb_first(actual, test.bytelen * 2);
        std::cout << "Result:  " << (pass ? "✅ PASS" : "❌ FAIL") << std::endl;
    }

    std::cout << "\n=== GF(2^8) Reduction Test ===" << std::endl;

    unsigned char reducer_8bit[2] = { 0x1B, 0x01 }; // x^8 + x^4 + x^3 + x + 1
    unsigned char mult_a[1] = { 0x57 };
    unsigned char mult_b[1] = { 0x83 };
    unsigned char product[2] = { 0 };

    gf2_multiply_lsb(mult_a, mult_b, product, 1);

    std::cout << "a        (MSB): "; print_bits_msb_first(mult_a, 1);
    std::cout << "b        (MSB): "; print_bits_msb_first(mult_b, 1);
    std::cout << "Product  (LSB): "; print_bits_lsb_first(product, 2);

    gf2_reduce_lsb(product, 2, reducer_8bit, 2);

    std::cout << "Reduced  (LSB): "; print_bits_lsb_first(product, 2);
    std::cout << "Reduced  (MSB): "; print_bits_msb_first(product, 2);

    if (product[0] == 0xC1 && product[1] == 0x00)
        std::cout << "Result: ✅ PASS (0x57 × 0x83 mod 0x11B == 0xC1)\n";
    else
        std::cout << "Result: ❌ FAIL (expected 0xC1, got 0x"
                  << std::hex << (int)product[0] << std::dec << ")\n";


    std::cout << "\n=== AES GF(2^8) Reduction Test ===" << std::endl;

    unsigned char aes_input[2] = { 0x01, 0x01 };     // x^8 + 1
    unsigned char aes_reducer[2] = { 0x1B, 0x01 };   // AES: x^8 + x^4 + x^3 + x + 1
    unsigned char aes_expected[2] = { 0x1A, 0x00 };  // Result: x^4 + x^3 + x

    std::cout << "Input    (LSB): "; print_bits_lsb_first(aes_input, 2);
    std::cout << "Reducer  (LSB): "; print_bits_lsb_first(aes_reducer, 2);

    gf2_reduce_lsb(aes_input, 2, aes_reducer, 2);

    std::cout << "Reduced  (LSB): "; print_bits_lsb_first(aes_input, 2);
    std::cout << "Reduced  (MSB): "; print_bits_msb_first(aes_input, 2);

    bool aes_pass = compare_results(aes_input, aes_expected, 2);
    std::cout << "Result:  " << (aes_pass ? "✅ PASS" : "❌ FAIL") << " (AES poly reduction)\n";


    std::cout << "\n=== Mid-Sized GF(2) Reduction Test ===" << std::endl;

    unsigned char input_64b[9] = {
        0x19, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01
    };

    unsigned char reducer_36b[5] = {
        0x29, 0x00, 0x00, 0x00, 0x10
    };

    unsigned char expected_64b[9] = {
        0x19, 0x00, 0x00, 0x90, 0x02,
        0x00, 0x00, 0x00, 0x00
    };

    std::cout << "Input    (LSB): "; print_bits_lsb_first(input_64b, 9);
    std::cout << "Reducer  (LSB): "; print_bits_lsb_first(reducer_36b, 5);

    gf2_reduce_lsb(input_64b, 9, reducer_36b, 5);

    std::cout << "Reduced  (LSB): "; print_bits_lsb_first(input_64b, 9);
    std::cout << "Reduced  (MSB): "; print_bits_msb_first(input_64b, 9);

    bool pass_64b = compare_results(input_64b, expected_64b, 9);
    std::cout << "Result:  " << (pass_64b ? "✅ PASS" : "❌ FAIL") << " (x^64 mod x^36)\n";


    std::cout << "\n=== GF(2^130 mod GCM) Test ===\n";

    unsigned char input_130b[17] = {
        0x25, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x04
    };

    unsigned char reducer_gcm[17] = {
        0x87, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01
    };

    unsigned char expected_130b[17] = {
        0x39, 0x02, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    std::cout << "Input    (LSB): "; print_bits_lsb_first(input_130b, 17);
    std::cout << "Reducer  (LSB): "; print_bits_lsb_first(reducer_gcm, 17);

    gf2_reduce_lsb(input_130b, 17, reducer_gcm, 17);

    std::cout << "Reduced  (LSB): "; print_bits_lsb_first(input_130b, 17);
    std::cout << "Reduced  (MSB): "; print_bits_msb_first(input_130b, 17);

    bool pass_130b = compare_results(input_130b, expected_130b, 17);
    std::cout << "Result:  " << (pass_130b ? "✅ PASS" : "❌ FAIL") << " (x^130 mod GCM)\n";

    return 0;
}
