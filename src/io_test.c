#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "io.h"

int tests_run = 0; 

struct dec_to_ascii_expected {
    char* string;
    int length;
};

struct dec_to_ascii_input {
    int dec;
    int padding_length;
    char padding_char;
};

static char* test_dec_to_ascii(struct dec_to_ascii_input input, struct dec_to_ascii_expected expected) {
    struct Padding_Info padding_info;
    padding_info.length = input.padding_length;
    padding_info.padding = input.padding_char;

    char str[256];
    int ret = dec_to_ascii(str, input.dec, padding_info);

    mu_assert("method should return length of read characters", ret == expected.length);
    
    printf("expect str filled with `%s` , actual:`%s`\n", expected.string, str);
    mu_assert("str format error", strcmp(str, expected.string) == 0);
    return 0;
}

#define DEC_TO_ASCII_TEST_CASE 7
static char* test_dec_to_ascii_parameterized() {
    struct dec_to_ascii_input test_input[DEC_TO_ASCII_TEST_CASE] = {
        {10, 3, ' '},
        {-10, 3, ' '},
        {1, 3, ' '},
        {-1, 3, ' '},
        {0, 3, ' '},
        {1234, 3, ' '},
        {-1234, 3, ' '},
    };

    struct dec_to_ascii_expected test_expected[DEC_TO_ASCII_TEST_CASE] = {
        {" 10", 3},
        {"-10", 3},
        {"  1", 3},
        {" -1", 3},
        {"  0", 3},
        {"1234", 4},
        {"-1234", 5},
    };

    for(int i = 0; i < DEC_TO_ASCII_TEST_CASE; i++) {
         struct dec_to_ascii_input input = test_input[i];
         struct dec_to_ascii_expected expected = test_expected[i];
         test_dec_to_ascii(input, expected);
    }
    return 0;
}


static char * all_tests() {
     mu_run_test(test_dec_to_ascii_parameterized);
     return 0;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
