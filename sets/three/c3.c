#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../is_bit_set.h"
#include "../decipher_hex_xor.h"

int main (void) {
    #define START 48
    #define MAX 123
    const char *buf1 = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
    char *store = NULL;

    // aaaaaaand it's X aka 88:
    // Cooking MC's like a pound of bacon
    for (int32_t i = START; i < MAX; ++i) {
        if (!isalpha(i))
            continue;
        store = decipher_hex_xor(buf1, i); 
        assert (store != NULL);
        printf("%c %d: %s\n", i, i, store);
        free(store);
        store = NULL;
    }
    return 0;
}
