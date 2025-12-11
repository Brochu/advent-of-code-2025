struct box {
    int32 x, y, z;
};
struct link {
    int32 f, t;
};

export uniform int32 solvep1(uniform box boxes[], uniform int32 num_boxes, uniform int32 num_iter, uniform link to_connect[]) {

    uniform int32 min_dist = INT32_MAX;
    uniform link selected_link;
    for(uniform int32 b0 = 0; b0 < num_boxes; b0++) {
        foreach (b1 = 0 ... num_boxes) {
            if (b0 != b1) {
                const box f = boxes[b0];
                const box t = boxes[b1];

                const int32 dx = t.x - f.x;
                const int32 dy = t.y - f.y;
                const int32 dz = t.z - f.z;

                const int32 dist = (dx*dx) + (dy*dy) + (dz*dz);
                if (dist < min_dist) {
                    foreach_active(idx) {
                        uniform int32 curr = extract(dist, idx);
                        if (curr < min_dist) {
                            min_dist = curr;
                            uniform int32 s_idx = extract(b1, idx);
                            selected_link.f = b0;
                            selected_link.t = s_idx;
                        }
                    }
                }
            }
        }
    }

    to_connect[0] = selected_link;
    return reduce_min(min_dist);
}
