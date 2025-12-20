export uniform int64 solve(uniform int64 xs[], uniform int64 ys[], uniform int len) {
    uniform int64 max_area = 0;

    for (uniform int i = 0; i < len; i++) {
        uniform int64 x0 = xs[i];
        uniform int64 y0 = ys[i];

        uniform int64 local_max = 0;
        foreach (j = i+1 ... len) {
            int64 dx = abs(xs[j] - x0);
            int64 dy = abs(ys[j] - y0);

            int64 area = (dx+1) * (dy+1);
            local_max = max(local_max, reduce_max(area));
        }
        max_area = max(max_area, local_max);
    }
    return max_area;
}
