#include "Computer_Top.hpp"

void Computer_Top::alu_glue_logic() {
    sc_uint<8> a_val = regA->internal_data.read();
    sc_uint<8> b_val = regB_val.read();
    sc_uint<8> op = opcode_ir.read();
    
    for(int i=0; i<8; i++) {
        alu_a[i].write(a_val[i]);
        alu_b[i].write(b_val[i]);
    }
    for(int i=0; i<4; i++) {
        alu_op[i].write(op[i]);
    }
}
