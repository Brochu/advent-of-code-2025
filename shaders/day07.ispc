struct coord {
    int32 x;
    int32 y;
};

export void step(
    uniform coord splitters[],
    uniform int32 num_splitters,
    uniform int32 max_height,
    uniform coord active[],
    uniform int32 num_active,
    uniform coord next[],
    uniform int32 *uniform num_next,
    uniform int32 *uniform num_splits) {

    foreach(active_idx = 0 ... num_active) {
        coord curr = active[active_idx];
        curr.y += 1;

        if (curr.y <= max_height) {
            int32 idx = atomic_add_global(num_next, 1);
            next[idx] = curr;
        }
    }

    *num_splits = max_height;
}
