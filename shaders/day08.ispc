export void dist(uniform int32 xs[], uniform int32 ys[], uniform int32 zs[], uniform int num_boxes, uniform int32 from[], uniform int32 to[], uniform int64 dists[]) {
    uniform int num_combos = 0;

    for (uniform int i = 0; i < num_boxes; i++) {
        foreach (j = i+1 ... num_boxes) {
            int64 dx = xs[j] - xs[i];
            int64 dy = ys[j] - ys[i];
            int64 dz = zs[j] - zs[i];
            int64 dist = dx*dx + dy*dy + dz*dz;

            packed_store_active(&from[num_combos], i);
            packed_store_active(&to[num_combos], j);
            num_combos += packed_store_active(&dists[num_combos], dist);
        }
    }
}

export uniform int64 connect(uniform int32 from[], uniform int32 to[], uniform int32 num_connect, uniform int32 c_ids[], uniform int32 c_sizes[], uniform int32 num_boxes) {
    for (uniform int i = 0; i < num_connect; i++) {
        uniform int64 from_id = from[i];
        uniform int64 to_id   = to[i];

        uniform int from_circ_id = c_ids[from_id];
        uniform int to_circ_id   = c_ids[to_id];
        if (from_circ_id == to_circ_id) {
            continue;
        }

        if (c_sizes[from_circ_id] > c_sizes[to_circ_id]) {
            atomic_add_global(&c_sizes[from_circ_id], c_sizes[to_circ_id]);
            atomic_swap_global(&c_sizes[to_circ_id], 0);
            for (uniform int j = 0; j < num_boxes; j++) {
                if (c_ids[j] == to_circ_id) {
                    atomic_swap_global(&c_ids[j], from_circ_id);
                }
            }
        } else {
            atomic_add_global(&c_sizes[to_circ_id], c_sizes[from_circ_id]);
            atomic_swap_global(&c_sizes[from_circ_id], 0);
            for (uniform int j = 0; j < num_boxes; j++) {
                if (c_ids[j] == from_circ_id) {
                    atomic_swap_global(&c_ids[j], to_circ_id);
                }
            }
        }
    }

    uniform int top[3];
    uniform int selected = 0;

    for (uniform int i = 0; i < num_boxes; i++) {
        uniform int current = i;
        uniform int pos = 0;
        while (pos < selected && c_sizes[top[pos]] > c_sizes[current]) {
            pos++;
        }

        if (pos < num_boxes) {
            for (uniform int j = min(selected, (uniform int)(num_boxes-1)); j > pos; j--) {
                top[j] = top[j-1];
            }

            top[pos] = current;
            if (selected < num_boxes) {
                selected++;
            }
        }
    }

    uniform int64 res = c_sizes[top[0]];
    res *= c_sizes[top[1]];
    res *= c_sizes[top[2]];
    return res;
}
