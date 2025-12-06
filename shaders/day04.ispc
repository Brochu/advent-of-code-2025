int8 scan_cell(uniform int8 cells[], uniform int64 w, uniform int64 h, uniform int8 roll_val, int64 x, int64 y) {
    int8 adj = 0;

    for (uniform int64 dy = -1; dy <= 1; dy++) {
        for (uniform int64 dx = -1; dx <= 1; dx++) {
            if (dy == 0 && dx == 0) { continue; }

            int64 ty = y + dy;
            int64 tx = x + dx;
            if (ty < 0 || ty >= h || tx < 0 || tx >= w) { continue; }

            int64 t_idx = (ty * w) + tx;
            if (cells[t_idx] == roll_val) { adj++; }
        }
    }

    return adj;
}

export uniform int64 solve(uniform int8 cells[], uniform int64 w, uniform int64 h, uniform int8 roll_val, uniform int8 removed[]) {
    int64 count = 0;

    foreach (y = 0 ... h) {
        for (uniform int64 x = 0; x < w; x++) {
            int64 idx = (y * w) + x;
            if (cells[idx] == roll_val) {
                int8 adj = scan_cell(cells, w, h, roll_val, x, y);

                if (adj < 4) {
                    count++;
                    removed[idx] = 1;
                }
            }
        }
    }
    return reduce_add(count);
}
