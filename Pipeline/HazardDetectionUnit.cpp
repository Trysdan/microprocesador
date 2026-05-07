#include "HazardDetectionUnit.hpp"

void HazardDetectionUnit::detect() {
    bool hazard = false;

    if (id_ex_mem_read.read()) {
        sc_uint<3> rd = id_ex_rd.read();
        if (rd != 0) {  // R0 es siempre 0, no crea hazard
            if (rd == if_id_rs1.read() || rd == if_id_rs2.read()) {
                hazard = true;
            }
        }
    }

    stall      .write(hazard);
    id_ex_flush.write(hazard);
}
