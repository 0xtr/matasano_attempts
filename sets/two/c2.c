#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../strxor.h"

int main (void) {
    /*
    If your function works properly, then when you feed it the string:
    1c0111001f010100061a024b53535009181c
      ... after hex decoding, and when XOR'd against:
    686974207468652062756c6c277320657965
      ... should produce:
    746865206b696420646f6e277420706c6179
    */

    // hex
    const char *buf1 = "1c0111001f010100061a024b53535009181c";
    // hex
    const char *buf2 = "686974207468652062756c6c277320657965";
    // hex
    const char *final = "746865206b696420646f6e277420706c6179";
    // so need to decode both hex, xor, then re-hex

    char *store = strxor(buf1, buf2); 
    printf("expecting : %s\n", final);
    if (strcmp(store,final) == 0) {
        printf("Got       : %s\n", final);
        printf("!!! Success !!!\n");
    } else {
        printf("You failed. Go try again.\n");
        printf("got: %lu, expected: %lu (result: %s)\n", strlen(store), strlen(final), store);
    }

    free(store);
    return 0;
}
