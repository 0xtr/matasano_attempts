#define _GNU_SOURCE
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "../encrypt_repeating_xor.h"
#include "../calc_hamming_distance.h"
#include "../base64_to_hex.h"
#include "../decipher_hex_xor.h"
#include "../score_result.h"

#define MIN_KEYSIZE 2
#define MAX_KEYSIZE 40
#define SCORE_RANKS 10

size_t filelen;
uint8_t *get_c6 (void);
struct Score {
    int32_t rankings_by_ks[SCORE_RANKS];
    uint8_t best_guess_at_keys[MAX_KEYSIZE][MAX_KEYSIZE];
};
struct Score get_scores (void);
struct Score bigscore;
uint8_t *assemble_block_from_subchars (const size_t keysize, const size_t byte_of_block);
struct Score get_plaintext_results (struct Score score, const int32_t which_ks);
struct Score reshuffle_rankings (struct Score score);
uint8_t *file;
void set_file();
uint8_t *get_file();
int32_t compar (const void *x, const void *y);
int32_t keysize_exists (const int32_t keysize);
double get_individual_score (const size_t keysize);
struct Score get_xor_decipher_results (const uint8_t *ks_hex, const size_t bytes_into, const size_t keysize, struct Score score);

/*
  File 6.txt:
  It's been base64'd after being encrypted with repeating-key XOR.
  Decrypt it.
  Here's how:

    Let KEYSIZE be the guessed length of the key; try values from 2 to (say) 40.
    Write a function to compute the edit distance/Hamming distance
    between two strings. The Hamming distance is just the number of
    differing bits. The distance between:
    this is a test
    and
    wokka wokka!!!
    is
    37.
    --------- DONE

    For each KEYSIZE, take the first KEYSIZE worth of bytes, and the
    second KEYSIZE worth of bytes, and find the edit distance between
    them. Normalize this result by dividing by KEYSIZE.

    The KEYSIZE with the smallest normalized edit distance is probably
    the key. You could proceed perhaps with the smallest 2-3 KEYSIZE
    values. Or take 4 KEYSIZE blocks instead of 2 and average the
    distances.

    Now that you probably know the KEYSIZE: break the ciphertext into
    blocks of KEYSIZE length.

    Now transpose the blocks: make a block that is the first byte of
    every block, and a block that is the second byte of every block, and
    so on.

    Solve each block as if it was single-character XOR. You already
    have code to do this.

    For each block, the single-byte XOR key that produces the best
    looking histogram is the repeating-key XOR key byte for that
    block. Put them together and you have the key.
*/
int32_t main (void) {
    set_file();

    const uint8_t *buf1 = (uint8_t*)"this is a test";
    const uint8_t *buf2 = (uint8_t*)"wokka wokka!!!";
    assert(calc_hamming_distance(buf1, buf2, strlen((char*)buf1)) == 37);
    assert(calc_hamming_distance((uint8_t*)"hello123", (uint8_t*)"hello123", 8) == 0);
    printf("--- passed basic hamming check.\n");

    struct Score score = get_scores();
    bigscore = score;
    for (size_t i = 0; i < SCORE_RANKS; ++i) {
        printf("rank %lu: keysize %d (that's %lu blocks) with score %.2f\n", 
                 i+1, score.rankings_by_ks[i], (filelen/score.rankings_by_ks[i]),
                 get_individual_score(score.rankings_by_ks[i]));
    }

    for (size_t i = 0; i < SCORE_RANKS; ++i) {
        score = get_plaintext_results(score, i);
    }
    printf("\n");
    for (size_t i = MIN_KEYSIZE; i < (MAX_KEYSIZE+1); ++i) {
        if (keysize_exists(i)) {
            printf("repeating sequence for key %lu is: ", i);
            for (size_t j = 0; j < i+1; ++j) {
                printf("%c", score.best_guess_at_keys[i][j]);
            }
            printf("\n");
        }
    }
    printf("\n");

    return 0;
}

void set_file() {
    file = get_c6();
}

uint8_t *get_file () {
    return file;
}

struct Score get_plaintext_results (struct Score score, const int32_t which_ks) {
    #define THRESHOLD 80
    uint8_t *ks_block, *ks_hex;
    int32_t ks = 0;
    uint8_t ascii_ranks[128];
    memset(ascii_ranks, 0, sizeof(ascii_ranks));
    ks = score.rankings_by_ks[which_ks];
    for (int32_t i = 0; i < ks; ++i) {
        ks_block = assemble_block_from_subchars(ks, i); 
        assert(ks_block != NULL);
        ks_hex = base64_to_hex(ks_block); 
        assert(ks_hex != NULL);
        score = get_xor_decipher_results(ks_hex, i, ks, score);
        free(ks_block);
        free(ks_hex);
    }
    return score;
}

struct Score get_xor_decipher_results (const uint8_t *ks_hex, const size_t bytes_into, const size_t keysize, struct Score score) {
    int32_t stringscore, best = 0;
    uint8_t *ks_pt;
    for (size_t j = 0; j < 128; ++j) {
        ks_pt = decipher_hex_xor(ks_hex, j);
        stringscore = score_result(ks_pt, strlen((char*)ks_hex)/2);
        best = (stringscore > best) ? stringscore : best;
        free(ks_pt);
    }
    score.best_guess_at_keys[keysize][bytes_into] = best;
    return score;
}

uint8_t *get_c6 (void) {
    size_t alloc_size = 0;
    uint8_t *contents = NULL;
    char *line = NULL;
    FILE *fp = fopen("6.txt", "r");
    assert(fp != NULL);
    ssize_t read = 0;
    size_t len = 0;
    while ((read = getline(&line, &len, fp) != -1)) {
        alloc_size += strlen(line);
    }
    rewind(fp);
    contents = calloc(alloc_size, sizeof(uint8_t));
    while ((read = getline(&line, &len, fp) != -1)) {
        strcat((char*)contents, line);
    }
    free(line);
    assert(fclose(fp) == 0);
    filelen = strlen((char*)contents);
    return contents;
}

struct Score get_scores () {
    struct Score score;
    memset(&score, 0, sizeof(score));
    for (size_t i = MIN_KEYSIZE; i < MAX_KEYSIZE+1; ++i) {
        double h0 = calc_hamming_distance(file, &file[i], i)/i;
        double h1 = calc_hamming_distance(&file[i], &file[(2*i)], i)/i;
        double h2 = calc_hamming_distance(&file[(2*i)], &file[(3*i)], i)/i;
        double h3 = calc_hamming_distance(&file[(3*i)], &file[(4*i)], i)/i;
        double holder = ((h0 + h1 + h2 + h3)/4);
        printf("%lu | %.2f + %.2f + %.2f + %.2f = %.2f\n", i, h0, h1, h2, h3, holder);
        int32_t greater_than_existing = 0;
        for (size_t j = 0; j < SCORE_RANKS; ++j) {
            if (get_individual_score(score.rankings_by_ks[j]) > holder) {
                greater_than_existing = j;
            }
            if (score.rankings_by_ks[j] == 0 && greater_than_existing == 0) {
                score.rankings_by_ks[j] = i;
                break;
            }
        }
        (greater_than_existing != 0) ? score.rankings_by_ks[greater_than_existing] = i: 0;
        score = reshuffle_rankings(score);
    }
    return score;
}

double get_individual_score (const size_t keysize) {
    return ((calc_hamming_distance(file, &file[keysize], keysize)/keysize) + 
            (calc_hamming_distance(&file[keysize], &file[(2*keysize)], keysize)/keysize) +
            (calc_hamming_distance(&file[(2*keysize)], &file[(3*keysize)], keysize)/keysize) +
            (calc_hamming_distance(&file[(3*keysize)], &file[(4*keysize)], keysize)/keysize))/4;
}

struct Score reshuffle_rankings (struct Score score) {
    qsort(&score.rankings_by_ks, SCORE_RANKS, sizeof(score.rankings_by_ks[0]), &compar);
    return score;
}

int32_t compar (const void *x, const void *y) {
    int32_t xi = *((int*)x);
    int32_t yi = *((int*)y);
    double x_compar = get_individual_score(xi);
    double y_compar = get_individual_score(yi);
    if (keysize_exists(xi)) {
        x_compar = get_individual_score(bigscore.rankings_by_ks[xi]);
    }
    if (keysize_exists(yi)) {
        x_compar = get_individual_score(bigscore.rankings_by_ks[yi]);
    } 
    return (x_compar > y_compar) ? 1 : (x_compar < y_compar) ? -1 : 0;
}

int32_t keysize_exists (const int32_t keysize) {
    int32_t exists = 0;
    for (size_t i = 0; i < SCORE_RANKS; ++i) {
        if (bigscore.rankings_by_ks[i] == keysize) {
            exists = 1;
        }
    } 
    return exists;
}

uint8_t *assemble_block_from_subchars (const size_t keysize, const size_t byte_of_block) {
    assert (byte_of_block <= (keysize-1));
    const size_t blocks = strlen((char*)get_file())/keysize;
    uint8_t *chars = calloc(blocks, sizeof(uint8_t));
    int32_t bytes_into_file = 0;

    for (size_t i = 0; i < blocks; ++i) {
        memcpy(&chars[i], &get_file()[bytes_into_file+byte_of_block], sizeof(uint8_t));
        bytes_into_file += keysize;
    }
    return chars;
}
