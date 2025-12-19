export uniform int solvep1(
    uniform int xs[],
    uniform int ys[],
    uniform int zs[],
    uniform int num_boxes,
    uniform int num_iter,
    uniform int from[],
    uniform int to[])
{
    foreach (i = 0 ... num_boxes) {
        for (uniform int j = 0; j < num_boxes; j++) {
            uniform int base = j*num_boxes;
            from[base+i] = i;
            to[base+i] = j;
        }
    }

    return 1337;
}
