export uniform int64 solve(uniform int64 nums[], uniform int8 ops[], uniform uint64 num_ops, uniform uint64 op_len) {

    int64 sub_total = 0;
    foreach(op_idx = 0 ... num_ops) {
        int8 op = ops[op_idx];
        int64 op_res = (op == 0) ? 1 : 0;

        for (uniform uint64 i = 0; i < op_len; i++) {
            if (op == 0) {
                int64 val = nums[(op_idx * op_len) + i];
                if (val != 0) { op_res *= val; }
            }
            else if (op == 1) {
                int64 val = nums[(op_idx * op_len) + i];
                if (val != 0) { op_res += val; }
            }
        }
        sub_total += op_res;
    }

    return reduce_add(sub_total);
}
