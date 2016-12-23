#define _GNU_SOURCE
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../encrypt_repeating_xor.h"

/*
  Here is the opening stanza of an important work of the English language:
        Burning 'em, if you ain't quick and nimble
        I go crazy when I hear a cymbal
  Encrypt it, under the key "ICE", using repeating-key XOR.
  In repeating-key XOR, you'll sequentially apply each byte of the key; the first byte of plaintext
  will be XOR'd against I, the next C, the next E, then I again for the 4th byte, and so on.
  It should come out to:
        0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272
        a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f
*/
int32_t main (void) {
    const char *line = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    const char *expected = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";
    char *store = NULL;
    int32_t seq[] = {73, 67, 69}; // I C E

    printf("ENCRYPTING:\n%s\nEXPECTING:\n%s\n", line, expected);
    store = encrypt_repeating_xor(line, seq, ((sizeof(seq)/sizeof(seq[0]))-1));
    assert(store != NULL);
    if (strcmp(store, expected)) {
        printf("Mismatch! %s vs %s expected\n", store, expected);
    } else {
        printf("...\nSuccess!!\n...\n");
    }
    free(store);
    store = NULL;
    return 0;
}
