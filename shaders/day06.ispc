export uniform int64 part1(uniform int64 nums[], uniform int8 ops[], uniform uint64 num_ops, uniform uint64 op_len) {

    int64 sub_total = 0;
    foreach(op_idx = 0 ... num_ops) {
        int8 op = ops[op_idx];
        int64 op_res = (op == 0) ? 1 : 0;

        for (uniform uint64 i = 0; i < op_len; i++) {
            if (op == 0) { // MULT
                op_res *= nums[(i * num_ops) + op_idx];
            }
            else if (op == 1) { // ADD
                op_res += nums[(i * num_ops) + op_idx];
            }
        }
        sub_total += op_res;
    }

    return reduce_add(sub_total);
}
