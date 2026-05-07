#include "ForwardingUnit.hpp"

void ForwardingUnit::compute() {
    sc_uint<2> fwd_a = 0;
    sc_uint<2> fwd_b = 0;

    sc_uint<3> rs1 = id_ex_rs1.read();
    sc_uint<3> rs2 = id_ex_rs2.read();

    bool ex_wr  = ex_mem_reg_write.read();
    bool mem_wr = mem_wb_reg_write.read();
    sc_uint<3> ex_rd  = ex_mem_rd.read();
    sc_uint<3> mem_rd = mem_wb_rd.read();

    // ── Forward A ──────────────────────────────────
    // EX hazard tiene PRIORIDAD sobre MEM hazard
    if (ex_wr && ex_rd != 0 && ex_rd == rs1) {
        fwd_a = 2;  // Forward desde EX/MEM
    } else if (mem_wr && mem_rd != 0 && mem_rd == rs1) {
        fwd_a = 1;  // Forward desde MEM/WB
    }

    // ── Forward B ──────────────────────────────────
    if (ex_wr && ex_rd != 0 && ex_rd == rs2) {
        fwd_b = 2;  // Forward desde EX/MEM
    } else if (mem_wr && mem_rd != 0 && mem_rd == rs2) {
        fwd_b = 1;  // Forward desde MEM/WB
    }

    forward_a.write(fwd_a);
    forward_b.write(fwd_b);
}
