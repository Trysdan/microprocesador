#ifndef COMPUTER_TOP_V2_HPP
#define COMPUTER_TOP_V2_HPP

#include <systemc.h>
#include "Memory/DualPortRAM.hpp"
#include "Registers/RegisterFile_v2.hpp"
#include "ALU/ALU_v2.hpp"
#include "Control/Decoder.hpp"
#include "Pipeline/PipelineRegisters.hpp"
#include "Pipeline/ForwardingUnit.hpp"
#include "Pipeline/HazardDetectionUnit.hpp"

// =====================================================
//  Computer_Top_v2 — Procesador Pipeline RISC 5 Etapas
//
//  IF → ID → EX → MEM → WB
//
//  Buses dedicados por etapa (sin tri-state compartido).
//  Forwarding Unit resuelve RAW hazards sin stalls.
//  Hazard Detection Unit inserta burbuja en load-use.
//  Branch resolution en EX: flush de 2 ciclos si taken.
// =====================================================
SC_MODULE(Computer_Top_v2) {
    sc_in<bool> clk;
    sc_in<bool> reset;

    // Puerto de salida del procesador (instrucción OUT)
    sc_signal<sc_uint<8>> output_reg;
    sc_signal<bool>       halt_flag;

    // ─── Señales IF ───────────────────────────────
    sc_signal<sc_uint<8>>  s_pc;          // Program Counter
    sc_signal<sc_uint<16>> s_if_instr;    // Instrucción 16-bit de RAM
    sc_signal<sc_uint<8>>  s_pc_plus2;    // PC + 2

    // ─── Señales IF/ID → ID ───────────────────────
    sc_signal<sc_uint<16>> s_ifid_instr;
    sc_signal<sc_uint<8>>  s_ifid_pc2;

    // ─── Señales ID (decodificadas) ───────────────
    sc_signal<sc_uint<4>>  s_id_opcode;
    sc_signal<sc_uint<3>>  s_id_rd, s_id_rs1, s_id_rs2;
    sc_signal<sc_uint<8>>  s_id_imm8;
    sc_signal<sc_uint<8>>  s_id_rs1_val, s_id_rs2_val;
    // Control signals del Decoder
    sc_signal<bool>        s_dec_reg_write, s_dec_mem_read, s_dec_mem_write;
    sc_signal<bool>        s_dec_alu_src, s_dec_branch, s_dec_branch_nz;
    sc_signal<bool>        s_dec_jump, s_dec_mem_to_reg;
    sc_signal<bool>        s_dec_out_en, s_dec_halt, s_dec_indirect;
    sc_signal<sc_uint<4>>  s_dec_alu_op;

    // ─── Señales ID/EX → EX ───────────────────────
    sc_signal<bool>        s_idex_reg_write, s_idex_mem_read, s_idex_mem_write;
    sc_signal<bool>        s_idex_alu_src, s_idex_branch, s_idex_branch_nz;
    sc_signal<bool>        s_idex_jump, s_idex_mem_to_reg;
    sc_signal<bool>        s_idex_out_en, s_idex_halt, s_idex_indirect;
    sc_signal<sc_uint<4>>  s_idex_alu_op;
    sc_signal<sc_uint<8>>  s_idex_rs1_val, s_idex_rs2_val, s_idex_imm8;
    sc_signal<sc_uint<3>>  s_idex_rd, s_idex_rs1_addr, s_idex_rs2_addr;
    sc_signal<sc_uint<8>>  s_idex_pc2;

    // ─── MMIO: Periféricos ────────────────────────
    sc_signal<sc_uint<8>>  s_key_state;   // tecla actual (0=ninguna)
    sc_signal<bool>        s_refresh_req; // pulso: refresco del framebuffer
    uint8_t                fb[48];        // framebuffer 8x6 (0xC0-0xEF)

    // ─── Señales EX ───────────────────────────────
    sc_signal<sc_uint<2>>  s_fwd_a, s_fwd_b;       // Forwarding mux selects
    sc_signal<sc_uint<8>>  s_ex_alu_a, s_ex_alu_b; // Entradas a ALU (tras mux)
    sc_signal<sc_uint<8>>  s_ex_alu_result;
    sc_signal<bool>        s_ex_zero;
    sc_signal<bool>        s_ex_branch_taken;
    sc_signal<sc_uint<8>>  s_ex_branch_target;
    sc_signal<sc_uint<8>>  s_ex_rs2_fwd;     // Valor Rs2 tras forwarding (para STORE)

    // ─── Señales EX/MEM → MEM ─────────────────────
    sc_signal<bool>        s_exmem_reg_write, s_exmem_mem_read, s_exmem_mem_write;
    sc_signal<bool>        s_exmem_mem_to_reg, s_exmem_out_en, s_exmem_halt;
    sc_signal<sc_uint<8>>  s_exmem_alu_result, s_exmem_rs2_val;
    sc_signal<sc_uint<3>>  s_exmem_rd;
    sc_signal<bool>        s_exmem_zero;

    // ─── Señales MEM ──────────────────────────────
    sc_signal<sc_uint<8>>  s_mem_data_out; // Mux entre RAM y MMIO
    sc_signal<sc_uint<8>>  s_ram_data_out; // Salida cruda de la RAM

    // ─── Señales MEM/WB → WB ──────────────────────
    sc_signal<bool>        s_memwb_reg_write, s_memwb_mem_to_reg;
    sc_signal<bool>        s_memwb_out_en, s_memwb_halt;
    sc_signal<sc_uint<8>>  s_memwb_alu_result, s_memwb_mem_data;
    sc_signal<sc_uint<3>>  s_memwb_rd;
    sc_signal<sc_uint<8>>  s_wb_data;  // Mux WB: alu_result vs mem_data -> RF

    // ─── Control de Hazards ───────────────────────
    sc_signal<bool>        s_stall;
    sc_signal<bool>        s_idex_flush;
    sc_signal<bool>        s_ifid_flush;   // flush por branch taken (EX stage)
    sc_signal<bool>        s_idex_bubble;  // burbuja interna de la HDU

    // ─── Instancias de Módulos ────────────────────
    DualPortRAM*         ram;
    RegisterFile_v2*     rf;
    Decoder*             decoder;
    ALU_v2*              alu;
    IF_ID_Reg*           ifid_reg;
    ID_EX_Reg*           idex_reg;
    EX_MEM_Reg*          exmem_reg;
    MEM_WB_Reg*          memwb_reg;
    ForwardingUnit*      fwd_unit;
    HazardDetectionUnit* hazard_unit;

    // ─── Procesos combinacionales ─────────────────
    void stage_IF();        // PC + fetch
    void stage_ID_decode(); // Extrae campos de la instrucción IF/ID
    void stage_EX();        // MUXes de forwarding + ALU + branch
    void stage_MEM();       // MMIO decode + framebuffer + keyboard
    void stage_WB();        // Selección WB data + output + halt

    SC_CTOR(Computer_Top_v2);
    ~Computer_Top_v2();
};

#endif // COMPUTER_TOP_V2_HPP
