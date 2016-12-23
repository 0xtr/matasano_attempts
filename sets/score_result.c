int32_t score_result (const uint8_t *ptr, size_t len) {
    float score = 0;
    for (size_t i = 0; i < len; ++i) {
        (isalnum(ptr[i]) || isblank(ptr[i]) || ispunct(ptr[i])) ? ++score : 0;
    }
    return ((score / len) * 100);
}
