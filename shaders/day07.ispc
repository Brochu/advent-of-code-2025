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

uniform int64 count_time(uniform coord splits[], uniform int32 num_splits, uniform int32 width, uniform int32 max_height, uniform coord curr, uniform int64 *uniform cache) {
    if (curr.x < 0 || curr.x >= width || curr.y < 0) {
        return 0;
    }
    if (curr.y >= max_height) {
        return 1;
    }

    uniform int32 idx = (curr.y * width) + curr.x;
    if (cache[idx] >= 0) {
        return cache[idx];
    }

    uniform bool is_split = false;
    for(uniform int32 i = 0; i < num_splits; i++) {
        if (splits[i].x == curr.x && splits[i].y == curr.y) {
            is_split = true;
            break;
        }
    }

    uniform int64 res = 0;
    if (is_split) {
        uniform coord l;
        l.x = curr.x - 1;
        l.y = curr.y;

        uniform coord r;
        r.x = curr.x + 1;
        r.y = curr.y;

        res += count_time(splits, num_splits, width, max_height, l, cache);
        res += count_time(splits, num_splits, width, max_height, r, cache);
    }
    else {
        uniform coord next;
        next.x = curr.x;
        next.y = curr.y + 1;
        res += count_time(splits, num_splits, width, max_height, next, cache);
    }

    cache[idx] = res;
    return res;
}

#define SIZE 142*142

export void time(uniform coord splits[], uniform int32 num_splits, uniform int32 width, uniform int32 max_height, uniform coord *uniform start, uniform int64 *uniform num_timelines) {
    uniform int64 *uniform cache = uniform new int64[SIZE];
    for (uniform int32 i = 0; i < SIZE; i++) {
        cache[i] = -1;
    }

    *num_timelines = count_time(splits, num_splits, width, max_height, *start, cache);
    delete cache;
}
