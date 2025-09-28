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
