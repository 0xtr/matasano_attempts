char* encrypt_repeating_xor (const char *buf1, int32_t seq[], const int32_t SEQ_RANGE_MAX) {
    #define HEX_POWER 3
    #define MAX_POWER 7
    #define HEX_BITS  4
    const char *hex = "0123456789abcdef";
    size_t len = strlen(buf1);
    char *store = calloc(len*2, 1);
    int32_t val __attribute__((unused)), count __attribute__((unused)) = 0;

    int32_t seq_current = 0;
    int32_t hex_power = HEX_POWER;
    int32_t base_power = MAX_POWER;
    for (size_t i = 0, count = 0, val = 0; i < (len*2); ++i) {
        if (count >= len || !isascii(buf1[count])) break;
        for (size_t j = 0; j < HEX_BITS; ++j) {
            int8_t xor = is_bit_set(buf1[count], base_power) ^ is_bit_set(seq[seq_current], base_power);
            (xor == 1) ? val += pow(2, hex_power) : 0;
            (--hex_power < 0) ? hex_power = HEX_POWER : 0;
            (--base_power < 0) ? base_power = MAX_POWER, ++count, ++seq_current : 0;
            (seq_current > SEQ_RANGE_MAX) ? seq_current = 0 : 0;
            (hex_power >= HEX_POWER) ? strncat(store, &hex[val], 1), val = 0
                                      : 0;
        }
    }
    strcat(store, "\0");
    return store;
}
