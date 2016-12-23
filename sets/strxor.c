char* strxor (const char *buf1, const char *buf2) {
    if ((strlen(buf1) != strlen(buf2)) || (sizeof(buf1[0]) != sizeof(buf2[0])))
        return NULL;

    char *hexvals = "0123456789abcdef";
    int32_t j, len = strlen(buf1);
    int32_t val, i, power;
    char *store = malloc(len);
    power = 3, val = 0;

    for (i = 0; i < len; ++i) {
        if (!isalnum(buf1[i])) 
            break;
        for (j = 0; j < 4; ++j) {
            int8_t xor = is_bit_set(buf1[i], power) ^ is_bit_set(buf2[i], power);
            val += (xor == 1) ? pow(2, power) : 0;
            (--power < 0) ? power = 3 : 0;
            (power == 3) ? strncat(store, &hexvals[val], 1), 
                           val = 0
                         : 0;
        }
    }

    return store;
}
