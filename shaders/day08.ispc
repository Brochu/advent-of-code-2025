struct box {
    int32 x, y, z;
};
struct link {
    int32 from, to;
};

uniform int32 calc_shortest(uniform int32 xs[], uniform int32 ys[], uniform int32 zs[], uniform int32 num_boxes, uniform int32 ids[], uniform int32 * uniform from, uniform int32 * uniform to) {

    // best distance seen so far (uniform because we need one value for the gang)
    uniform float bestDist = 1e30f;
    uniform int   bestI    = -1;
    uniform int   bestJ    = -1;

    // iterate over i in blocks of programCount
    for (uniform int i = 0; i < 20; ++i)
    {
        float xi = xs[i];
        float yi = ys[i];
        float zi = zs[i];

        // j always starts at i+1 to avoid double work and self-comparison
        for (uniform int j = i + 1; j < 20; ++j)
        {
            float xj = xs[j];
            float yj = ys[j];
            float zj = zs[j];

            float dx = xi - xj;
            float dy = yi - yj;
            float dz = zi - zj;
            float d2 = dx*dx + dy*dy + dz*dz;   // squared distance

            // mask of lanes that beat the current best
            varying bool closer = (d2 < bestDist) && (ids[i] != ids[j]);

            if (any(closer))
            {
                // 1. reduce the minimum distance inside this gang
                varying float newBest = select(bestDist, d2, closer);
                uniform float gangMin = reduce_min(newBest);

                // 2. which lane produced that minimum?
                varying bool  isWinner = (newBest == gangMin);
                uniform int   winnerLane = count_leading_zeros(packmask(isWinner));

                // 3. update uniform best and indices
                bestDist = gangMin;
                bestI    = i;      // i is varying base + lane
                bestJ    = j;
            }
        }
    }

    *from = bestI;
    *to = bestJ;

    return 0;
}

export uniform int32 solvep1(uniform int32 xs[], uniform int32 ys[], uniform int32 zs[], uniform int32 num_boxes, uniform int32 num_iter, uniform int32 * uniform from, uniform int32 * uniform to) {
    uniform int32 * uniform circ_ids = uniform new int32[num_boxes];
    uniform int32 * uniform circ_sizes = uniform new int32[num_boxes];

    for (uniform int32 i = 0; i < num_boxes; i++) {
        circ_ids[i] = i;
        circ_sizes[i] = 1;
    }

    uniform int32 f;
    uniform int32 t;
    uniform int32 dist = calc_shortest(xs, ys, zs, num_boxes, circ_ids, &f, &t);
    //TODO: THIS NEEDS TO GROUP BASED ON SIZE
    circ_ids[f] = circ_ids[t];
    circ_sizes[f]--;
    circ_sizes[t]++;

    dist = calc_shortest(xs, ys, zs, num_boxes, circ_ids, &f, &t);
    circ_ids[f] = circ_ids[t];
    circ_sizes[f]--;
    circ_sizes[t]++;

    dist = calc_shortest(xs, ys, zs, num_boxes, circ_ids, &f, &t);
    circ_ids[f] = circ_ids[t];
    circ_sizes[f]--;
    circ_sizes[t]++;

    *from = f;
    *to = t;

    delete[] circ_ids;
    delete[] circ_sizes;
    return dist;
}
