#include "RegisterFile_v2.hpp"

void RegisterFile_v2::process_read() {
    sc_uint<3> a1 = rs1_addr.read();
    sc_uint<3> a2 = rs2_addr.read();
    sc_uint<3> wa = rd_addr.read();
    bool       we = reg_write.read() && (wa != 0);

    // WB→ID bypass: si estamos escribiendo el mismo registro que leemos,
    // retornamos el dato de escritura directamente (evita hazard de 1 ciclo).
    sc_uint<8> d1 = (we && wa == a1) ? rd_data.read() : regs[a1.to_uint()].read();
    sc_uint<8> d2 = (we && wa == a2) ? rd_data.read() : regs[a2.to_uint()].read();

    // R0 hardwired zero
    rs1_data.write(a1 == 0 ? sc_uint<8>(0) : d1);
    rs2_data.write(a2 == 0 ? sc_uint<8>(0) : d2);
}

void RegisterFile_v2::process_write() {
    if (reset.read()) {
        for (int i = 0; i < 8; i++) regs[i].write(0);
    } else if (reg_write.read()) {
        int idx = rd_addr.read().to_uint();
        // R0 es siempre 0 (convención RISC hardwired zero)
        if (idx != 0) {
            regs[idx].write(rd_data.read());
        }
    }
}
