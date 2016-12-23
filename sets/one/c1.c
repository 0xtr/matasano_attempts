#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../hex_to_base64.h"

int main (void) {
    const char *ptr = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    const char *final = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

    char *store = hex_to_base64(ptr);
    printf("processing: %s\n", ptr);
    printf("expecting : %s\n", final);
    if (strcmp(store,final) == 0) {
        printf("Got       : %s\n", final);
        printf("!!! Success !!!\n");
    } else {
        printf("You failed. Go try again.\n");
        printf("got: %lu, expected: %lu\n", strlen(store), strlen(final));
    }

    free(store);
    return 0;
}
