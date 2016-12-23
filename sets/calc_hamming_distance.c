double calc_hamming_distance (const uint8_t *buf1, const uint8_t *buf2, const size_t len) {
    #define MAX_POWER 7
    int32_t base_power = MAX_POWER;
    double ham = 0;
    for (size_t i = 0; i < len; ++i) {
        if (!isascii(buf1[i])) continue;
        for (size_t j = 0; j < (MAX_POWER+1); ++j) {
            ((is_bit_set(buf1[i], base_power) ^ is_bit_set(buf2[i], base_power)) == 1) ? ham += 1 : 0;
            (--base_power < 0) ? base_power = MAX_POWER : 0;
        }
    }
    return ham;
}
