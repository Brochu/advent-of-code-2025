export uniform int64 solve(uniform int32 xs[], uniform int32 ys[], uniform int32 len) {
    uniform int64 max_area = 0;

    for (uniform int i = 0; i < len; i++) {
        uniform int32 x0 = xs[i];
        uniform int32 y0 = ys[i];

        uniform int64 local_max = 0;
        foreach (j = i+1 ... len) {
            int32 dx = abs(xs[j] - x0);
            int32 dy = abs(ys[j] - y0);

            int64 area = (int64)(dx+1) * (int64)(dy+1);
            local_max = max(local_max, reduce_max(area));
        }
        max_area = max(max_area, local_max);
    }
    return max_area;
}

export uniform int64 part2(uniform int32 xs[], uniform int32 ys[], uniform uint64 set[], uniform int32 len) {
    uniform int64 max_area = 0;

    for (uniform int i = 0; i < len; i++) {
        uniform int32 x0 = xs[i];
        uniform int32 y0 = ys[i];

        uniform int64 local_max = 0;
        foreach (j = i+1 ... len) {
            int32 x1 = xs[j];
            int32 y1 = ys[j];

            bool skip = false;
            for (uniform int ii = 0; ii < len; ii++) {
                uniform int32 xo = xs[ii];
                uniform int32 yo = ys[ii];

                int32 xmin = min(x0, x1);
                int32 ymin = min(y0, y1);
                int32 xmax = max(x0, x1);
                int32 ymax = max(y0, y1);

                if (xo >= xmin && xo <= xmax &&
                    yo >= ymin && yo <= ymax)
                {
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                int32 dx = abs(x1 - x0);
                int32 dy = abs(y1 - y0);
                int64 area = (int64)(dx+1) * (int64)(dy+1);
                local_max = max(local_max, reduce_max(area));
            }
        }
        max_area = max(max_area, local_max);
    }
    return max_area;
}
