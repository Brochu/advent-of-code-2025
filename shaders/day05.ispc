export uniform int64 solve(uniform int64 ls[], uniform int64 hs[], uniform int64 num_ranges, uniform int64 ids[], uniform int64 num_ids) {
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
