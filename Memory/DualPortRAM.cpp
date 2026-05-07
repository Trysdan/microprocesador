#include "DualPortRAM.hpp"

// Puerto IF: lectura combinacional de 16 bits (2 bytes consecutivos)
void DualPortRAM::read_instruction() {
    sc_uint<8> addr = if_addr.read();
    sc_uint<8> byte_hi = mem[addr.to_uint()].read();
    sc_uint<8> byte_lo = (addr.to_uint() < 255) ? mem[addr.to_uint() + 1].read() : sc_uint<8>(0);
    sc_uint<16> instr;
    instr.range(15, 8) = byte_hi;
    instr.range( 7, 0) = byte_lo;
    if_data.write(instr);
}

// Puerto DATA: lectura combinacional de 8 bits para LOAD
void DualPortRAM::read_data() {
    if (data_read_en.read()) {
        data_out.write(mem[data_addr.read().to_uint()].read());
    } else {
        data_out.write(0);
    }
}

// Puerto DATA: escritura síncrona de 8 bits para STORE
void DualPortRAM::write_data() {
    if (data_write_en.read()) {
        mem[data_addr.read().to_uint()].write(data_in.read());
    }
}
