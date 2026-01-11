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

        //TODO: Revise this algo, it doesn't really use the power of ISPC properly
        // Are atmoics here really helping anything at all?
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

    int top[3] = { 0, 0, 0 };
    foreach (i = 0 ... num_boxes) {
        int current = c_sizes[i];

        if (top[0] < current) {
            top[2] = top[1];
            top[1] = top[0];
            top[0] = current;
        }
        else if (top[1] < current) {
            top[2] = top[1];
            top[1] = current;
        } else if (top[2] < current) {
            top[2] = current;
        }
    }

    uniform int flat_maxes[programCount * 3];
    uniform int write_pos = 0;
    for (uniform int i = 0; i < 3; i++) {
        write_pos += packed_store_active(&flat_maxes[write_pos], top[i]);
    }

    uniform int global_top[3] = { 0, 0, 0 };
    for (uniform int i = 0; i < programCount * 3; i++) {
        uniform int current = flat_maxes[i];

        if (global_top[0] < current) {
            global_top[2] = global_top[1];
            global_top[1] = global_top[0];
            global_top[0] = current;
        }
        else if (global_top[1] < current) {
            global_top[2] = global_top[1];
            global_top[1] = current;
        } else if (global_top[2] < current) {
            global_top[2] = current;
        }
    }

    uniform int64 res = 1;
    res *= global_top[0];
    res *= global_top[1];
    res *= global_top[2];
    return res;
}

uniform bool check_ids(uniform int32 c_ids[], uniform int32 num_boxes) {
    uniform int first_id = c_ids[0];
    varying bool matches = true;

    foreach (i = 0 ... num_boxes) {
        matches = matches && (c_ids[i] == first_id);
    }

    return all(matches);
}

export uniform int64 complete(uniform int32 from[], uniform int32 to[], uniform int32 num_connect, uniform int32 c_ids[], uniform int32 c_sizes[], uniform int32 num_boxes) {
    uniform int i = num_connect;
    while (!check_ids(c_ids, num_boxes)) {
        uniform int64 from_id = from[i];
        uniform int64 to_id   = to[i];

        uniform int from_circ_id = c_ids[from_id];
        uniform int to_circ_id   = c_ids[to_id];
        if (from_circ_id == to_circ_id) {
            i++;
            continue;
        }

        //TODO: Revise this algo, it doesn't really use the power of ISPC properly
        // Are atmoics here really helping anything at all?
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
        i++;
    }

    return i - 1;
}
