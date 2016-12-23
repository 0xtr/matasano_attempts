int32_t *base64_to_realvals (size_t len, const uint8_t *ptr) {
    int32_t pos = 0;
    int32_t *realvals = calloc(len, sizeof(int32_t));
    for (size_t i = 0; i < len; ++i) {
        int32_t ret = get_b64_realvalue(ptr[i]);
        (ret > -1) ? realvals[pos++] = ret : 0;
    }
    return realvals;
}
