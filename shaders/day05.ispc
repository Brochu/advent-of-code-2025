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
    //TODO: This is way to slow, will need to find a good way to merge ranges
    int64 min_lo = INT64_MAX;
    int64 max_hi = INT64_MIN;

    foreach (range_idx = 0 ... num_ranges) {
        min_lo = min(min_lo, ls[range_idx]);
        max_hi = max(max_hi, hs[range_idx]);
    }

    uniform int64 start = reduce_min(min_lo);
    uniform int64 end = reduce_max(max_hi);

    int64 num_fresh = 0;
    foreach (id = start ... end+1) {
        for(uniform int64 i = 0; i < num_ranges; i++) {
            int64 lo = ls[i];
            int64 hi = hs[i];

            if (id >= lo && id <= hi) {
                num_fresh++;
                break;
            }
        }
    }
    return reduce_add(num_fresh);
}
