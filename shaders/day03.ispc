/*
export uniform int64 solve(uniform int8 batteries[], uniform uint64 banks[], uniform uint64 num_banks, uniform uint64 bank_len, uniform int8 k) {

    int64 max_jolt = 0;
    foreach (bank_idx = 0 ... num_banks) {
        int64 base = banks[bank_idx];
        int64 hi = batteries[base];
        int64 lo = 0;

        for (uniform int64 i = 1; i < bank_len; i++) {
            //TODO: use k to select biggest value selecting k values; keep order
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
*/

export uniform int64 solve(uniform int8 batteries[], uniform uint64 banks[], uniform uint64 num_banks, uniform uint64 bank_len, uniform int8 k) {

    int64 max_jolt = 0;

    foreach (bank_idx = 0 ... num_banks) {

        int64 base = banks[bank_idx];
        int8 chosen[20];
        int64 c = 0;

        for (uniform int64 i = 0; i < bank_len; i++) {

            int8 b = batteries[base + i];

            while (c > 0 && chosen[c - 1] < b && (bank_len - i + c - 1) >= k) {
                c--;
            }
            if (c < k) {
                chosen[c++] = b;
            }
        }

        int64 value = 0;
        for (uniform int64 i = 0; i < k; i++) {
            value *= 10;
            value += chosen[i];
        }

        max_jolt += value;
    }
    return reduce_add(max_jolt);

}
