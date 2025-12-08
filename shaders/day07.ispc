export void step(uniform uint64 beams[], uniform uint64 num_beams, uniform uint64 uniform *uniform num_splits) {

    uint64 sum = 0;
    foreach(beam_idx = 0 ... num_beams) {
        sum++;
    }
    *num_splits += reduce_add(sum);
}
