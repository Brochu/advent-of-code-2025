export uniform int64 part1(uniform int64 ls[], uniform int64 hs[], uniform int64 num_ranges, uniform int64 ids[], uniform int64 num_ids) {
    int64 num_fresh = 0;

    foreach (id = 0 ... num_ids) {
        int64 value = ids[id];

        for(uniform int64 i = 0; i < num_ranges; i++) {
            int64 lo = ls[i];
            int64 hi = hs[i];

            if (value >= lo && value <= hi) {
                num_fresh++;
                break;
            }
        }
    }
    return reduce_add(num_fresh);
}

export uniform int64 part2(uniform int64 ls[], uniform int64 hs[], uniform int64 num_ranges) {
    for(uniform int64 i = num_ranges - 1; i >= 0; i--) {
        for(uniform int64 j = 0; j < i; j++) {
            if (ls[j] > ls[j+1]) {
                uniform int64 temp = ls[j];
                ls[j] = ls[j+1];
                ls[j+1] = temp;

                temp = hs[j];
                hs[j] = hs[j+1];
                hs[i+1] = temp;
            }
        }
    }

    //TODO: Implement range merging

    return 0;
}
