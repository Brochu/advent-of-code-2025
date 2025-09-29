export void sums(
    uniform int vals[],
    uniform int starts[],
    uniform int lens[],
    uniform int outs[],
    uniform int count
) {

    foreach (group_idx = 0 ... count) {
        int sum = 0;
        for (int i = 0; i < lens[group_idx]; i++) {
            sum += vals[starts[group_idx] + i];
        }
        outs[group_idx] = sum;
    }
}

export void sums_gang_groups(
    uniform int vals[],
    uniform int starts[],
    uniform int lens[],
    uniform int outs[],
    uniform int count
) {
    // Each group is processed sequentially by the gangs
    for (uniform int group_idx = 0; group_idx < count; group_idx++) {
        uniform int base = starts[group_idx];
        uniform int len  = lens[group_idx];

        int partial_sum = 0;

        // Distribute iterations across lanes: each lane works on different i,
        // but all use the same base and same len (uniform).
        foreach (i = 0 ... len) {
            partial_sum += vals[base + i];
        }

        // Reduce across all program instances in this gang
        uniform int group_sum = reduce_add(partial_sum);

        // Store the result once (only in uniform form)
        outs[group_idx] = group_sum;
    }
}
