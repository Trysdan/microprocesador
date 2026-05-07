#include "PipelineRegisters.hpp"

// ─────────────────────────────────────────
//  IF/ID
// ─────────────────────────────────────────
void IF_ID_Reg::process() {
    if (flush.read()) {
        out_instr   .write(0);
        out_pc_plus2.write(0);
    } else if (!stall.read()) {
        out_instr   .write(in_instr.read());
        out_pc_plus2.write(in_pc_plus2.read());
    }
    // stall=true y flush=false: mantiene valores actuales
}

// ─────────────────────────────────────────
//  ID/EX
// ─────────────────────────────────────────
void ID_EX_Reg::process() {
    if (flush.read()) {
        // Insertar burbuja NOP: poner todo a 0
        out_reg_write .write(false); out_mem_read .write(false);
        out_mem_write .write(false); out_alu_src  .write(false);
        out_branch    .write(false); out_branch_nz.write(false);
        out_jump      .write(false); out_mem_to_reg.write(false);
        out_out_en    .write(false); out_halt     .write(false);
        out_alu_op    .write(0);
        out_rs1_val   .write(0);    out_rs2_val  .write(0);
        out_imm8      .write(0);    out_rd       .write(0);
        out_rs1_addr  .write(0);    out_rs2_addr .write(0);
        out_pc_plus2  .write(0);
    } else {
        out_reg_write .write(in_reg_write .read());
        out_mem_read  .write(in_mem_read  .read());
        out_mem_write .write(in_mem_write .read());
        out_alu_src   .write(in_alu_src   .read());
        out_branch    .write(in_branch    .read());
        out_branch_nz .write(in_branch_nz .read());
        out_jump      .write(in_jump      .read());
        out_mem_to_reg.write(in_mem_to_reg.read());
        out_out_en    .write(in_out_en    .read());
        out_halt      .write(in_halt      .read());
        out_alu_op    .write(in_alu_op    .read());
        out_rs1_val   .write(in_rs1_val   .read());
        out_rs2_val   .write(in_rs2_val   .read());
        out_imm8      .write(in_imm8      .read());
        out_rd        .write(in_rd        .read());
        out_rs1_addr  .write(in_rs1_addr  .read());
        out_rs2_addr  .write(in_rs2_addr  .read());
        out_pc_plus2  .write(in_pc_plus2  .read());
    }
}

// ─────────────────────────────────────────
//  EX/MEM
// ─────────────────────────────────────────
void EX_MEM_Reg::process() {
    if (flush.read()) {
        out_reg_write .write(false); out_mem_read .write(false);
        out_mem_write .write(false); out_mem_to_reg.write(false);
        out_out_en    .write(false); out_halt     .write(false);
        out_alu_result.write(0);     out_rs2_val  .write(0);
        out_rd        .write(0);     out_zero     .write(false);
    } else {
        out_reg_write .write(in_reg_write .read());
        out_mem_read  .write(in_mem_read  .read());
        out_mem_write .write(in_mem_write .read());
        out_mem_to_reg.write(in_mem_to_reg.read());
        out_out_en    .write(in_out_en    .read());
        out_halt      .write(in_halt      .read());
        out_alu_result.write(in_alu_result.read());
        out_rs2_val   .write(in_rs2_val   .read());
        out_rd        .write(in_rd        .read());
        out_zero      .write(in_zero      .read());
    }
}

// ─────────────────────────────────────────
//  MEM/WB
// ─────────────────────────────────────────
void MEM_WB_Reg::process() {
    out_reg_write .write(in_reg_write .read());
    out_mem_to_reg.write(in_mem_to_reg.read());
    out_out_en    .write(in_out_en    .read());
    out_halt      .write(in_halt      .read());
    out_alu_result.write(in_alu_result.read());
    out_mem_data  .write(in_mem_data  .read());
    out_rd        .write(in_rd        .read());
}
