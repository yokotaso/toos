#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "io.h"

int tests_run = 0; 

static char* test_dec_to_ascii_zero() {
    struct Padding_Info padding_info;
    padding_info.length = 3;
    padding_info.padding = ' ';
    char str[30];
    int ret = dec_to_ascii(str, 0, padding_info);

    mu_assert("method should return length of read characters", ret == 3);
    
    char err[30];
    printf("expect str filled with `   0` , actual:`%s`\n", str);
    mu_assert("str format error", strcmp(str, "  0") == 0);
    return 0;
}

static char* test_dec_to_ascii_minus() {
    struct Padding_Info padding_info;
    padding_info.length = 3;
    padding_info.padding = ' ';
    char str[30];
    int ret = dec_to_ascii(str, -10, padding_info);

    mu_assert("method should return length of read characters", ret == 3);
    
    char err[30];
    printf("expect str filled with `-10` , actual:`%s`\n", str);
    mu_assert("str format error", strcmp(str, "-10") == 0);
    return 0;
}


static char * all_tests() {
     mu_run_test(test_dec_to_ascii_zero);
     mu_run_test(test_dec_to_ascii_minus);
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
