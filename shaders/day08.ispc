void swap(uniform int64 from[], uniform int64 to[], uniform int64 dists[], uniform int i, uniform int j) {
    uniform int64 temp = dists[i];
    dists[i] = dists[j];
    dists[j] = temp;

    temp = from[i];
    from[i] = from[j];
    from[j] = temp;

    temp = to[i];
    to[i] = to[j];
    to[j] = temp;
}

export uniform int64 solvep1(
    uniform int64 xs[],
    uniform int64 ys[],
    uniform int64 zs[],
    uniform int64 num_boxes,
    uniform int64 num_iter,
    uniform int64 from[],
    uniform int64 to[],
    uniform int64 dists[],
    uniform int circ_ids[],
    uniform int circ_sizes[])
{
    for (uniform int i = 0; i < num_boxes; i++) {
        foreach (j = i+1 ... num_boxes) {
            int idx =
                i * num_boxes
                - (i * (i + 1))/2
                + (j - i - 1);

            int64 dx = xs[j] - xs[i];
            int64 dy = ys[j] - ys[i];
            int64 dz = zs[j] - zs[i];
            int64 dist = dx*dx + dy*dy + dz*dz;

            from[idx] = i;
            to[idx] = j;
            dists[idx] = dist;
        }
    }

    const int K = 10;
    uniform int top[10];
    uniform int selected = 0;

    for (uniform int i = 0; i < (num_boxes * (num_boxes - 1)) / 2; i++) {
        uniform int current = i;
        uniform int pos = 0;
        while (pos < selected && dists[top[pos]] < dists[current]) {
            pos++;
        }

        if (pos < K) {
            for (uniform int j = min(selected, 9); j > pos; j--) {
                top[j] = top[j-1];
            }

            top[pos] = current;
            if (selected < K) {
                selected++;
            }
        }
    }

    for (uniform int i = 0; i < K; i++) {
        swap(from, to, dists, i, top[i]);
    }

    return 1337;
}
