struct box {
    int32 x, y, z;
};
struct link {
    int32 from, to;
};

uniform int32 calc_shortest(uniform box boxes[], uniform int32 num_boxes, uniform int32 ids[], uniform int32 * uniform box0, uniform int32 * uniform box1) {
    uniform int32 global_min = INT32_MAX;

    for(uniform int32 b0 = 0; b0 < num_boxes; ++b0) {
        uniform box from = boxes[b0];

        foreach (b1 = 0 ... num_boxes) {
            uniform int32 b1_u = extract(b1, 0);
            if (b0 == b1_u) continue;

            uniform box to = boxes[b1_u];
            int32 dx = to.x - from.x;
            int32 dy = to.y - from.y;
            int32 dz = to.z - from.z;
            int32 dist = (dx*dx) + (dy*dy) + (dz*dz);
            bool valid = ids[b0] != ids[b1_u];
            int32 potential = select(dist < global_min && valid, dist, INT32_MAX);

            uniform int32 new_p = reduce_min(potential);
            if (new_p < global_min) {
                global_min = new_p;

                uniform int32 temp_mask = packmask(new_p == potential);
                uniform int32 win_idx = count_leading_zeros(temp_mask);
                *box0 = b0;
                *box1 = extract(b1_u, win_idx);
            }
        }
    }
    return global_min;
}

export uniform int32 solvep1(uniform box boxes[], uniform int32 num_boxes, uniform int32 num_iter) {

    int32* uniform circuit_ids = uniform new int32[num_boxes];
    int32* uniform circuit_sizes = uniform new int32[num_boxes];

    for (uniform int32 box_idx = 0; box_idx < num_boxes; box_idx++) {
        circuit_ids[box_idx] = box_idx;
        circuit_sizes[box_idx] = 1;
    }

    uniform int32 f;
    uniform int32 t;
    uniform int32 dist = calc_shortest(boxes, num_boxes, circuit_ids, &f, &t);

    uniform int32 test = circuit_ids[6];
    delete[] circuit_ids;
    delete[] circuit_sizes;
    return dist;
}
