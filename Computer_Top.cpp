#include "Computer_Top.hpp"

void Computer_Top::alu_glue_logic() {
    sc_uint<8> a_val = regA->internal_data.read();
    sc_uint<8> b_val = regB_val.read();
    sc_uint<8> op = opcode_ir.read();
    
    // Si es una instruccion ADDR Rx (0x20-0x27), mandamos el opcode de SUMA (0x02) a la ALU
    sc_uint<4> alu_opcode_val = op.range(3, 0);
    if (op >= 0x20 && op <= 0x27) {
        alu_opcode_val = 0x02; // Codigo de SUMA en la ALU
    }
    
    for(int i=0; i<8; i++) {
        alu_a[i].write(a_val[i]);
        alu_b[i].write(b_val[i]);
    }
    for(int i=0; i<4; i++) {
        alu_op[i].write(alu_opcode_val[i]);
    }
}
