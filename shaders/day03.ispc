export void part1(uniform int8 batteries[], uniform uint64 banks[], uniform uint64 num_banks, uniform uint64 bank_len, uniform int64 *uniform result) {

    /*
    foreach (group_idx = 0 ... count) {
        int sum = 0;
        for (int i = 0; i < lens[group_idx]; i++) {
            sum += vals[starts[group_idx] + i];
        }
        outs[group_idx] = sum;
    }
    */

    varying int64 sum = 0;
    foreach (bank_idx = 0 ... num_banks) {
        sum = bank_idx;
    }
    *result = reduce_add(sum);

}
