uint8_t *base64_to_hex (const uint8_t *ptr) {
    #define B64TH_HEX_BITS 3
    #define B64TH_B64_BITS 5
    size_t len = strlen((char*)ptr);
    const uint8_t hexvals[] = "0123456789abcdef";
    uint8_t *store = calloc(len*2, sizeof(uint8_t));
    int32_t val, shift, pos, power, newstr_len = 0;
    shift = B64TH_B64_BITS, power = B64TH_HEX_BITS, val = pos = 0;
    /*
    int32_t *realvals = base64_to_realvals(len, ptr);
    for (size_t i = 0, pos = 0; i < len; ++i) {
        for (size_t j = 0; j < B64TH_B64_BITS+1; ++j) {
            (is_bit_set(realvals[pos], shift--) == 1) ? val += pow(2, power--) : power--;
            (shift < 0) ? shift = B64TH_B64_BITS, ++pos : 0;
            (power < 0) ? power = B64TH_HEX_BITS : 0;
            (power == B64TH_HEX_BITS) ? memcpy(&store[newstr_len++], &hexvals[val], sizeof(uint8_t)), val = 0 : 0;
        }
    }
    */
    for (size_t i = 0, pos = 0; i < len; ++i) {
        for (size_t j = 0; j <= B64TH_B64_BITS; ++j) {
            (is_bit_set(ptr[pos], shift--) == 1) ? val += pow(2, power--) : power--;
            (shift < 0) ? shift = B64TH_B64_BITS, ++pos : 0;
            (power < 0) ? power = B64TH_HEX_BITS : 0;
            (power == B64TH_HEX_BITS) ? memcpy(&store[newstr_len++], &hexvals[val], sizeof(uint8_t)), val = 0 : 0;
        }
    }
    store[newstr_len] = 0;
    //free(realvals);
    return store;
}
