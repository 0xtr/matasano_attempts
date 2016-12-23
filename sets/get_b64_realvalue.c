int32_t get_b64_realvalue (const uint8_t to_find) {
    const uint8_t base64vals[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int32_t i, found = 0;
    for (i = 0; i < 63; ++i) {
        if (base64vals[i] == to_find) {
            found = 1;
            break;
        }
    }
    return (found == 1) ? i : -1;
}
