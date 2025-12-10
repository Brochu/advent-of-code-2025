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
    uniform int32 *uniform num_splits,
    uniform int32 *uniform num_done) {

    foreach(active_idx = 0 ... num_active) {
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
                uniform int32 len = reduce_add(1);
                uniform int32 base = atomic_add_local(num_next, len);
                int32 offset = exclusive_scan_add(1);

                next[base+offset] = curr;
            } else {
                uniform int32 len = reduce_add(2);
                uniform int32 base = atomic_add_local(num_next, len);
                int32 offset = exclusive_scan_add(2);

                next[base+offset+0].x = curr.x - 1;
                next[base+offset+0].y = curr.y;

                next[base+offset+1].x = curr.x + 1;
                next[base+offset+1].y = curr.y;

                atomic_add_local(num_splits, popcnt(lanemask()));
            }
        }
        else {
            atomic_add_local(num_done, popcnt(lanemask()));
        }
    }
}

/*
export void test(uniform int32 num_active, uniform int32 out[], uniform int32 *uniform num_out) {
    int32 sum = 0;
    uniform int32 next_idx = 0;

    foreach(act_idx = 0 ... num_active) {
        if (act_idx % 2 == 0) {
            uniform int32 count = reduce_add(1);
            uniform int32 base = atomic_add_local(&next_idx, count);
            int32 lane_offset = exclusive_scan_add(1);
            out[base + lane_offset] = act_idx;
        }
    }
    *num_out = next_idx;
}
*/
