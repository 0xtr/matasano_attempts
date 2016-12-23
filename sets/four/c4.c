#define _GNU_SOURCE
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../is_bit_set.h"
#include "../decipher_hex_xor.h"
char* decipher_hex_xor (const char *buf1, int32_t c);
void process_chars (char *line);
int32_t score_string (const char *ptr);

int main (void) {
    #define ASCII_START 0
    #define ASCII_END 127
    char *store = NULL;
    char *line = NULL;
    int rownum_for_top = 0;
    int top_score = 0;
    ssize_t read;
    size_t len;
    typedef struct {
        int32_t best_cipher;
        int32_t best_score;
        char *best_result;
        char *original;
        int fail:1;
    } line_entry;
    
    FILE *fp = fopen("file4.txt", "r");
    if (fp == NULL) {
        printf("Failed to open file4.txt.\n");
        return -1;
    }
    int32_t rows = 0;
    int32_t maxlen = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        if ((int32_t)strlen(line) > maxlen) 
            maxlen = (int32_t)strlen(line);
        ++rows;
    }
    line_entry le[rows];
    for (int32_t i = 0; i < rows; ++i) {
        le[i].best_cipher = 0;
        le[i].best_score = 0;
        le[i].best_result = calloc(maxlen, 1);
        le[i].original = calloc(maxlen, 1);
    }
    int32_t current = 0;
    int32_t score = 0;
    rewind(fp);

    while ((read = getline(&line, &len, fp)) != -1) {
        if (read <= 0) {
            store = NULL;
            break;
        }
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';
        strncpy(le[current].original, line, strlen(line));

        for (int32_t i = ASCII_START; i < ASCII_END; ++i) {
            store = decipher_hex_xor(le[current].original, i); 
            store[strlen(store)-1] = '\0';
            process_chars(store);
            assert (store != NULL);
            score = score_string(store);
            if (score < 90) {
                le[current].fail = 1;
            } else {
                le[current].fail = 0;
                if (score >= le[current].best_score) {
                    le[current].best_cipher = i;
                    le[current].best_score = score;
                    memset(le[current].best_result, 0, maxlen);
                    strncpy(le[current].best_result, store, strlen(line));
                    if (score > top_score) {
                        rownum_for_top = current;
                        top_score = score;
                    }
                }
            }
            free(store);
            store = NULL;
        }
        ++current;
    }
    free(line);
    line = NULL;

    for (int32_t i = 0; i < rows; ++i) {
        if (le[i].fail == 0) {
            printf("Highest-scoring result for %s was %s, with cipher %d (%c), score %d\n", le[i].original, le[i].best_result, le[i].best_cipher, 
                    le[i].best_cipher, le[i].best_score);
        }
    }
    // why am i missing spaces here
    // it's NAK aka 21 for Now that the party is jumping
    printf("Highest-scoring result: row %d, which was original: [%s]\n--> the result was [%s] from cipher [%c](asc %d) aka score of %d\n", 
            top_score, le[rownum_for_top].original, le[rownum_for_top].best_result, le[rownum_for_top].best_cipher, le[rownum_for_top].best_cipher,
            le[rownum_for_top].best_score);
    return 0;
}

void process_chars (char *line) {
    for (int32_t i = 0; i < (int32_t)strlen(line); ++i) {
        line[i] = (i == 0) ? toupper(line[i]) : tolower(line[i]);
    }
}

int32_t score_string (const char *ptr) {
    assert (ptr != NULL);
    float score = 0;
    for (int32_t i = 0; i < (int32_t)strlen(ptr); ++i) {
        if (isalnum(ptr[i]) || isblank(ptr[i]) || ptr[i] == 39)
            ++score;
    }
    return (score / (int32_t)strlen(ptr)) * 100;
}
