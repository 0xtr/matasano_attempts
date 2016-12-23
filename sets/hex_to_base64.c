uint8_t* hex_to_base64 (const uint8_t *ptr) {
    #define HTB64_HEX_BITS 3
    #define HTB64_B64_BITS 5
    const uint8_t base64vals[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    uint8_t *store = malloc(strlen(ptr));
    int32_t val, shift, power;
    shift = HTB64_HEX_BITS, power = HTB64_B64_BITS, val = 0;

    for (size_t i = 0; i < (char*)strlen(ptr); ++i) {
        for (size_t j = 0; j < 4; ++j) {
            int32_t calc = (isalpha(ptr[i])) ? (strtol(&ptr[i], NULL, 16) & (1 << shift--)) != 0
                                             : (ptr[i] & (1 << shift--)) != 0;
            val += (calc == 1) ? pow(2, power--) : 0;
            (shift < 0) ? shift = HTB64_HEX_BITS : 0;
            (power < 0) ? power = HTB64_B64_BITS : 0;
            (power == HTB64_B64_BITS) ? strncat(store, &base64vals[val], 1), val = 0 : 0;
        }
    }
    strncat(store, 0, 1);
    return store;
}
