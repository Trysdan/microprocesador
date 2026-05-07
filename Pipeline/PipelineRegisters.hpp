#ifndef PIPELINE_REGISTERS_HPP
#define PIPELINE_REGISTERS_HPP

#include <systemc.h>

// =====================================================
//  Registros de Pipeline IF/ID → ID/EX → EX/MEM → MEM/WB
//
//  Cada registro captura las señales de su etapa entrada
//  en el flanco positivo del reloj.
//  stall: congela el registro (no actualiza)
//  flush: inserta burbuja NOP (pone señales de control a 0)
// =====================================================

// ─────────────────────────────────────────
//  IF/ID Register
// ─────────────────────────────────────────
SC_MODULE(IF_ID_Reg) {
    sc_in<bool> clk;
    sc_in<bool> stall;   // PC y este registro se congelan
    sc_in<bool> flush;   // Burbuja por branch taken

    sc_in<sc_uint<16>> in_instr;
    sc_in<sc_uint<8>>  in_pc_plus2;

    sc_out<sc_uint<16>> out_instr;
    sc_out<sc_uint<8>>  out_pc_plus2;

    void process();
    SC_CTOR(IF_ID_Reg) {
        SC_METHOD(process);
        sensitive << clk.pos();
    }
};

// ─────────────────────────────────────────
//  ID/EX Register
// ─────────────────────────────────────────
SC_MODULE(ID_EX_Reg) {
    sc_in<bool> clk;
    sc_in<bool> flush;   // Burbuja por stall o branch

    // Control
    sc_in<bool> in_reg_write, in_mem_read, in_mem_write;
    sc_in<bool> in_alu_src, in_branch, in_branch_nz;
    sc_in<bool> in_jump, in_mem_to_reg, in_out_en, in_halt;
    sc_in<sc_uint<4>> in_alu_op;

    // Datos
    sc_in<sc_uint<8>>  in_rs1_val, in_rs2_val, in_imm8;
    sc_in<sc_uint<3>>  in_rd, in_rs1_addr, in_rs2_addr;
    sc_in<sc_uint<8>>  in_pc_plus2;

    // Outputs
    sc_out<bool> out_reg_write, out_mem_read, out_mem_write;
    sc_out<bool> out_alu_src, out_branch, out_branch_nz;
    sc_out<bool> out_jump, out_mem_to_reg, out_out_en, out_halt;
    sc_out<sc_uint<4>> out_alu_op;

    sc_out<sc_uint<8>>  out_rs1_val, out_rs2_val, out_imm8;
    sc_out<sc_uint<3>>  out_rd, out_rs1_addr, out_rs2_addr;
    sc_out<sc_uint<8>>  out_pc_plus2;

    void process();
    SC_CTOR(ID_EX_Reg) {
        SC_METHOD(process);
        sensitive << clk.pos();
    }
};

// ─────────────────────────────────────────
//  EX/MEM Register
// ─────────────────────────────────────────
SC_MODULE(EX_MEM_Reg) {
    sc_in<bool> clk;
    sc_in<bool> flush;

    // Control
    sc_in<bool> in_reg_write, in_mem_read, in_mem_write;
    sc_in<bool> in_mem_to_reg, in_out_en, in_halt;

    // Datos
    sc_in<sc_uint<8>> in_alu_result;
    sc_in<sc_uint<8>> in_rs2_val;   // Para STORE
    sc_in<sc_uint<3>> in_rd;
    sc_in<bool>       in_zero;      // Flag zero de la ALU

    // Outputs
    sc_out<bool> out_reg_write, out_mem_read, out_mem_write;
    sc_out<bool> out_mem_to_reg, out_out_en, out_halt;

    sc_out<sc_uint<8>> out_alu_result;
    sc_out<sc_uint<8>> out_rs2_val;
    sc_out<sc_uint<3>> out_rd;
    sc_out<bool>       out_zero;

    void process();
    SC_CTOR(EX_MEM_Reg) {
        SC_METHOD(process);
        sensitive << clk.pos();
    }
};

// ─────────────────────────────────────────
//  MEM/WB Register
// ─────────────────────────────────────────
SC_MODULE(MEM_WB_Reg) {
    sc_in<bool> clk;

    // Control
    sc_in<bool> in_reg_write, in_mem_to_reg, in_out_en, in_halt;

    // Datos
    sc_in<sc_uint<8>> in_alu_result;
    sc_in<sc_uint<8>> in_mem_data;
    sc_in<sc_uint<3>> in_rd;

    // Outputs
    sc_out<bool> out_reg_write, out_mem_to_reg, out_out_en, out_halt;

    sc_out<sc_uint<8>> out_alu_result;
    sc_out<sc_uint<8>> out_mem_data;
    sc_out<sc_uint<3>> out_rd;

    void process();
    SC_CTOR(MEM_WB_Reg) {
        SC_METHOD(process);
        sensitive << clk.pos();
    }
};

#endif // PIPELINE_REGISTERS_HPP
