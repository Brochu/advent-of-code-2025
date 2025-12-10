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

    //foreach(active_idx = 0 ... num_active) {
    for(uniform int32 active_idx = 0; active_idx < num_active; active_idx++) {
        coord curr = active[active_idx];
        curr.y += 1;

        if (curr.y <= max_height) {
            bool should_split = false;

            for(uniform int32 i = 0; i < num_splitters; i++) {
                if (splitters[i].x == curr.x && splitters[i].y == curr.y) {
                    should_split = true;
                    break;
                }
            }

            if (!should_split) {
                int32 idx = atomic_add_local(num_next, 1);
                next[idx] = curr;
            } else {
                int32 left = atomic_add_local(num_next, 2);
                next[left + 0].x = curr.x - 1;
                next[left + 0].y = curr.y;

                next[left + 1].x = curr.x + 1;
                next[left + 1].y = curr.y;

                atomic_add_local(num_splits, 1);
            }
        }
    }
}
