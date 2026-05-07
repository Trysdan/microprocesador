#include "Decoder.hpp"
#include "../Pipeline/PipelineTypes.hpp"

void Decoder::decode() {
    uint8_t op = opcode.read().to_uint();

    ctrl_reg_write .write(false);
    ctrl_mem_read  .write(false);
    ctrl_mem_write .write(false);
    ctrl_alu_src   .write(false);
    ctrl_branch    .write(false);
    ctrl_branch_nz .write(false);
    ctrl_jump      .write(false);
    ctrl_mem_to_reg.write(false);
    ctrl_out_en    .write(false);
    ctrl_halt      .write(false);
    ctrl_indirect  .write(false);
    ctrl_alu_op    .write(op);   // La ALU siempre recibe el opcode

    switch (op) {
        case ISA::LOAD:
            ctrl_reg_write .write(true);   // El dato cargado va a RF
            ctrl_mem_read  .write(true);   // Leer de RAM
            ctrl_mem_to_reg.write(true);   // WB: usar dato de memoria
            ctrl_alu_src   .write(true);   // Usar imm8 como dirección
            break;

        case ISA::STORE:
            ctrl_mem_write .write(true);   // Escribir en RAM
            ctrl_alu_src   .write(true);   // imm8 = dirección destino
            break;

        case ISA::MOV:
            ctrl_reg_write.write(true);    // Rd <- Rs1
            ctrl_alu_op   .write(ISA::MOV);
            break;

        case ISA::MVI:
            ctrl_reg_write.write(true);    // Rd <- imm8
            ctrl_alu_src  .write(true);    // Fuente B = imm8
            ctrl_alu_op   .write(ISA::MVI); 
            break;

        case ISA::ADD:
        case ISA::SUB:
        case ISA::AND:
        case ISA::OR:
        case ISA::XOR:
            ctrl_reg_write.write(true);
            break;

        // LOADI: Rd <- MEM[Rs1]  (acceso indirecto)
        case ISA::LOADI:
            ctrl_reg_write .write(true);
            ctrl_mem_read  .write(true);
            ctrl_mem_to_reg.write(true);
            ctrl_indirect  .write(true);  // la dirección viene de Rs1, no de imm8
            break;

        // STORI: MEM[Rs2] <- Rs1  (store indirecto)
        case ISA::STORI:
            ctrl_mem_write.write(true);
            ctrl_indirect .write(true);  // la dirección viene de Rs2, no de imm8
            break;

        case ISA::JMP:
            ctrl_jump  .write(true);
            ctrl_alu_src.write(true);      // imm8 es el destino
            break;

        case ISA::BEZ:
            ctrl_branch   .write(true);
            ctrl_branch_nz.write(false);   // Branch if equal zero
            ctrl_alu_src  .write(true);
            break;

        case ISA::BNZ:
            ctrl_branch   .write(true);
            ctrl_branch_nz.write(true);    // Branch if NOT zero
            ctrl_alu_src  .write(true);
            break;

        case ISA::OUT:
            ctrl_out_en.write(true);       // Rs1 → OUTPUT register
            break;

        case ISA::HLT:
            ctrl_halt.write(true);
            break;

        default:
            break; // NOP / instrucción desconocida
    }
}
