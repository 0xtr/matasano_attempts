int32_t is_bit_set (uint8_t c, int32_t shift) {
    return (c & (1 << shift)) != 0;
}
