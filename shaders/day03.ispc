export uniform int64 part1(uniform int8 batteries[], uniform uint64 banks[], uniform uint64 num_banks, uniform uint64 bank_len) {

    int64 max_jolt = 0;
    foreach (bank_idx = 0 ... num_banks) {
        int64 base = banks[bank_idx];
        int64 hi = batteries[base];
        int64 lo = 0;

        for (uniform int64 i = 1; i < bank_len; i++) {
            int8 b = batteries[base+i];

            if (b > lo) {
                lo = b;
            }

            if (b > hi && i != bank_len-1) {
                hi = b;
                lo = batteries[base+i+1];
            }
        }
        max_jolt += (hi * 10) + lo;
    }
    return reduce_add(max_jolt);

}
