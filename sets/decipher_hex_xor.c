uint8_t *decipher_hex_xor (const uint8_t *buf1, const uint8_t c) {
    #define HEX_POWER 3
    #define MAX_POWER 7
    size_t len = strlen((char*)buf1);
    uint8_t *store = calloc(len, sizeof(uint8_t));
    int32_t val __attribute__ ((unused)), count __attribute__ ((unused)), newstr_len = 0;
    int32_t hex_power = HEX_POWER;
    int32_t power = MAX_POWER;
    for (size_t i = 0, count = 0, val = 0; i < len; ++i, power--) {
        for (size_t j = 0; j <= HEX_POWER; ++j) {
            ((is_bit_set(buf1[count], hex_power) ^ is_bit_set(c, power)) == 1) ? val += pow(2, power--) : 0;
            (--hex_power < 0) ? hex_power = HEX_POWER, ++count : 0;
            (--power < 0) ? power = MAX_POWER : 0;
            (power == MAX_POWER) ? memcpy(&store[newstr_len++], &val, sizeof(uint8_t)), val = 0 : 0;
        }
    }
    store[newstr_len] = 0;
    return store;
}
