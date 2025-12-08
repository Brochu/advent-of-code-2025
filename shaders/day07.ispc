struct coord {
    int32 x;
    int32 y;
};
uniform uint64 coord_pack(uniform coord c) {
    return ((uint64)c.y << 32) | (uint64)c.x;
}
uniform coord coord_unpack(uniform uint64 p) {
    uniform coord c;
    c.x = (int32)p;
    c.y = (int32)(p >> 32);
    return c;
}


export void step(uniform uint64 beams[], uniform uint64 num_beams, uniform uint64 uniform *uniform num_splits) {

    uniform coord c;
    c.x = 69;
    c.y = 420;
    uniform uint64 key = coord_pack(c);

    uint64 sum = 0;
    foreach(beam_idx = 0 ... num_beams) {
        sum++;
    }
    *num_splits = key;
}
