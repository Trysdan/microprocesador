#include "ALU_v2.hpp"
#include "../Pipeline/PipelineTypes.hpp"

void ALU_v2::compute() {
    sc_uint<8> av = a.read();
    sc_uint<8> bv = b.read();
    sc_uint<8> res = 0;

    switch (alu_op.read().to_uint()) {
        case ISA::ADD:  res = av + bv;  break;
        case ISA::SUB:  res = av - bv;  break;
        case ISA::AND:  res = av & bv;  break;
        case ISA::OR:   res = av | bv;  break;
        case ISA::XOR:  res = av ^ bv;  break;
        case ISA::LOAD:
        case ISA::STORE: res = bv;       break;  // Address from Immediate
        case ISA::LOADI:
        case ISA::STORI:
        case ISA::OUT:   res = av;       break;  // Address/Data from Register
        case ISA::MOV:   res = av;       break;  // PASS_A
        case ISA::MVI:   res = bv;       break;  // PASS_B (Immediate)
        default:         res = av;       break;
    }

    result.write(res);
    zero.write(res == 0);
}
