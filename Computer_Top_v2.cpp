#include "Computer_Top_v2.hpp"
#include "Pipeline/PipelineTypes.hpp"
#include <iostream>
#include <cstring>   // memset
#include <atomic>

// Teclado no-bloqueante: actualizado por un thread externo en main_v2.cpp
std::atomic<uint8_t> g_current_key{0};

// =====================================================
//  Constructor: Instanciación y Conexión de Módulos
// =====================================================
SC_HAS_PROCESS(Computer_Top_v2);

Computer_Top_v2::Computer_Top_v2(sc_module_name name) : sc_module(name) {

    // ─── RAM Dual-Port ───────────────────────────
    ram = new DualPortRAM("RAM");
    ram->clk(clk);
    // Puerto IF
    ram->if_addr(s_pc);
    ram->if_data(s_if_instr);
    // Puerto DATA (MEM stage)
    ram->data_write_en(s_exmem_mem_write);
    ram->data_read_en(s_exmem_mem_read);
    ram->data_addr(s_exmem_alu_result);
    ram->data_in(s_exmem_rs2_val);
    ram->data_out(s_ram_data_out);

    // ─── RegisterFile (3 puertos) ────────────────
    rf = new RegisterFile_v2("RF");
    rf->clk(clk);
    rf->reset(reset);
    rf->rs1_addr(s_id_rs1);
    rf->rs1_data(s_id_rs1_val);
    rf->rs2_addr(s_id_rs2);
    rf->rs2_data(s_id_rs2_val);
    rf->reg_write(s_memwb_reg_write);
    rf->rd_addr(s_memwb_rd);
    rf->rd_data(s_wb_data);   // Mux WB: stage_WB selecciona alu vs mem

    // ─── Decoder (combinacional) ──────────────────
    decoder = new Decoder("DEC");
    decoder->opcode(s_id_opcode);
    decoder->ctrl_reg_write (s_dec_reg_write);
    decoder->ctrl_mem_read  (s_dec_mem_read);
    decoder->ctrl_mem_write (s_dec_mem_write);
    decoder->ctrl_alu_src   (s_dec_alu_src);
    decoder->ctrl_branch    (s_dec_branch);
    decoder->ctrl_branch_nz (s_dec_branch_nz);
    decoder->ctrl_jump      (s_dec_jump);
    decoder->ctrl_mem_to_reg(s_dec_mem_to_reg);
    decoder->ctrl_out_en    (s_dec_out_en);
    decoder->ctrl_halt      (s_dec_halt);
    decoder->ctrl_indirect  (s_dec_indirect);
    decoder->ctrl_alu_op    (s_dec_alu_op);

    // ─── ALU v2 ───────────────────────────────────
    alu = new ALU_v2("ALU");
    alu->a(s_ex_alu_a);
    alu->b(s_ex_alu_b);
    alu->alu_op(s_idex_alu_op);
    alu->result(s_ex_alu_result);
    alu->zero(s_ex_zero);

    // ─── IF/ID Register ──────────────────────────
    ifid_reg = new IF_ID_Reg("IFID");
    ifid_reg->clk(clk);
    ifid_reg->stall(s_stall);
    ifid_reg->flush(s_ifid_flush);
    ifid_reg->in_instr   (s_if_instr);
    ifid_reg->in_pc_plus2(s_pc_plus2);
    ifid_reg->out_instr   (s_ifid_instr);
    ifid_reg->out_pc_plus2(s_ifid_pc2);

    // ─── ID/EX Register ──────────────────────────
    idex_reg = new ID_EX_Reg("IDEX");
    idex_reg->clk(clk);
    idex_reg->flush(s_idex_flush);
    idex_reg->in_reg_write (s_dec_reg_write);
    idex_reg->in_mem_read  (s_dec_mem_read);
    idex_reg->in_mem_write (s_dec_mem_write);
    idex_reg->in_alu_src   (s_dec_alu_src);
    idex_reg->in_branch    (s_dec_branch);
    idex_reg->in_branch_nz (s_dec_branch_nz);
    idex_reg->in_jump      (s_dec_jump);
    idex_reg->in_mem_to_reg(s_dec_mem_to_reg);
    idex_reg->in_out_en    (s_dec_out_en);
    idex_reg->in_halt      (s_dec_halt);
    idex_reg->in_alu_op    (s_dec_alu_op);
    idex_reg->in_rs1_val   (s_id_rs1_val);
    idex_reg->in_rs2_val   (s_id_rs2_val);
    idex_reg->in_imm8      (s_id_imm8);
    idex_reg->in_rd        (s_id_rd);
    idex_reg->in_rs1_addr  (s_id_rs1);
    idex_reg->in_rs2_addr  (s_id_rs2);
    idex_reg->in_pc_plus2  (s_ifid_pc2);
    idex_reg->out_reg_write (s_idex_reg_write);
    idex_reg->out_mem_read  (s_idex_mem_read);
    idex_reg->out_mem_write (s_idex_mem_write);
    idex_reg->out_alu_src   (s_idex_alu_src);
    idex_reg->out_branch    (s_idex_branch);
    idex_reg->out_branch_nz (s_idex_branch_nz);
    idex_reg->out_jump      (s_idex_jump);
    idex_reg->out_mem_to_reg(s_idex_mem_to_reg);
    idex_reg->out_out_en    (s_idex_out_en);
    idex_reg->out_halt      (s_idex_halt);
    idex_reg->out_alu_op    (s_idex_alu_op);
    idex_reg->out_rs1_val   (s_idex_rs1_val);
    idex_reg->out_rs2_val   (s_idex_rs2_val);
    idex_reg->out_imm8      (s_idex_imm8);
    idex_reg->out_rd        (s_idex_rd);
    idex_reg->out_rs1_addr  (s_idex_rs1_addr);
    idex_reg->out_rs2_addr  (s_idex_rs2_addr);
    idex_reg->out_pc_plus2  (s_idex_pc2);

    // ─── EX/MEM Register ─────────────────────────
    exmem_reg = new EX_MEM_Reg("EXMEM");
    exmem_reg->clk(clk);
    exmem_reg->flush(s_ifid_flush); // Reutilizamos flush de branch
    exmem_reg->in_reg_write (s_idex_reg_write);
    exmem_reg->in_mem_read  (s_idex_mem_read);
    exmem_reg->in_mem_write (s_idex_mem_write);
    exmem_reg->in_mem_to_reg(s_idex_mem_to_reg);
    exmem_reg->in_out_en    (s_idex_out_en);
    exmem_reg->in_halt      (s_idex_halt);
    exmem_reg->in_alu_result(s_ex_alu_result);
    exmem_reg->in_rs2_val   (s_ex_rs2_fwd);
    exmem_reg->in_rd        (s_idex_rd);
    exmem_reg->in_zero      (s_ex_zero);
    exmem_reg->out_reg_write (s_exmem_reg_write);
    exmem_reg->out_mem_read  (s_exmem_mem_read);
    exmem_reg->out_mem_write (s_exmem_mem_write);
    exmem_reg->out_mem_to_reg(s_exmem_mem_to_reg);
    exmem_reg->out_out_en    (s_exmem_out_en);
    exmem_reg->out_halt      (s_exmem_halt);
    exmem_reg->out_alu_result(s_exmem_alu_result);
    exmem_reg->out_rs2_val   (s_exmem_rs2_val);
    exmem_reg->out_rd        (s_exmem_rd);
    exmem_reg->out_zero      (s_exmem_zero);

    // ─── MEM/WB Register ─────────────────────────
    memwb_reg = new MEM_WB_Reg("MEMWB");
    memwb_reg->clk(clk);
    memwb_reg->in_reg_write (s_exmem_reg_write);
    memwb_reg->in_mem_to_reg(s_exmem_mem_to_reg);
    memwb_reg->in_out_en    (s_exmem_out_en);
    memwb_reg->in_halt      (s_exmem_halt);
    memwb_reg->in_alu_result(s_exmem_alu_result);
    memwb_reg->in_mem_data  (s_mem_data_out);
    memwb_reg->in_rd        (s_exmem_rd);
    memwb_reg->out_reg_write (s_memwb_reg_write);
    memwb_reg->out_mem_to_reg(s_memwb_mem_to_reg);
    memwb_reg->out_out_en    (s_memwb_out_en);
    memwb_reg->out_halt      (s_memwb_halt);
    memwb_reg->out_alu_result(s_memwb_alu_result);
    memwb_reg->out_mem_data  (s_memwb_mem_data);
    memwb_reg->out_rd        (s_memwb_rd);

    // ─── Forwarding Unit ─────────────────────────
    fwd_unit = new ForwardingUnit("FWD");
    fwd_unit->id_ex_rs1      (s_idex_rs1_addr);
    fwd_unit->id_ex_rs2      (s_idex_rs2_addr);
    fwd_unit->ex_mem_rd      (s_exmem_rd);
    fwd_unit->ex_mem_reg_write(s_exmem_reg_write);
    fwd_unit->mem_wb_rd      (s_memwb_rd);
    fwd_unit->mem_wb_reg_write(s_memwb_reg_write);
    fwd_unit->forward_a(s_fwd_a);
    fwd_unit->forward_b(s_fwd_b);

    // ─── Hazard Detection Unit ────────────────────
    hazard_unit = new HazardDetectionUnit("HDU");
    hazard_unit->id_ex_mem_read(s_idex_mem_read);
    hazard_unit->id_ex_rd      (s_idex_rd);
    hazard_unit->if_id_rs1     (s_id_rs1);
    hazard_unit->if_id_rs2     (s_id_rs2);
    hazard_unit->stall         (s_stall);
    hazard_unit->id_ex_flush   (s_idex_bubble);

    // Inicializar framebuffer a 0
    std::memset(fb, 0, sizeof(fb));

    SC_METHOD(stage_IF);
    sensitive << clk.pos() << reset;

    SC_METHOD(stage_ID_decode);
    sensitive << s_ifid_instr;

    SC_METHOD(stage_EX);
    sensitive << s_idex_rs1_val << s_idex_rs2_val << s_idex_imm8
              << s_idex_branch  << s_idex_branch_nz << s_idex_jump
              << s_idex_alu_src << s_idex_alu_op << s_idex_pc2
              << s_fwd_a << s_fwd_b
              << s_exmem_alu_result << s_exmem_mem_to_reg
              << s_memwb_alu_result << s_memwb_mem_data << s_memwb_mem_to_reg
              << s_idex_bubble;

    // MEM stage: combinacional para mux de datos
    SC_METHOD(stage_MEM);
    sensitive << s_ram_data_out << s_exmem_alu_result << s_exmem_mem_write << s_exmem_mem_read << s_exmem_rs2_val;

    SC_METHOD(stage_WB);
    sensitive << s_memwb_reg_write << s_memwb_mem_to_reg
              << s_memwb_alu_result << s_memwb_mem_data
              << s_memwb_out_en << s_memwb_halt << s_memwb_rd;
}

Computer_Top_v2::~Computer_Top_v2() {
    delete ram; delete rf; delete decoder; delete alu;
    delete ifid_reg; delete idex_reg; delete exmem_reg; delete memwb_reg;
    delete fwd_unit; delete hazard_unit;
}

// =====================================================
//  Stage IF — Actualización Síncrona del PC
//  Único driver de s_pc y s_pc_plus2.
//  Branch taken y stall vienen de señales combinacionales.
// =====================================================
void Computer_Top_v2::stage_IF() {
    if (reset.read()) {
        s_pc.write(0);
        s_pc_plus2.write(2);
        return;
    }
    // Prioridad: branch taken > stall > normal
    if (s_ex_branch_taken.read()) {
        s_pc.write(s_ex_branch_target.read());
    } else if (!s_stall.read()) {
        s_pc.write(s_pc.read() + 2);
    }
    // stall=true y branch=false → PC se queda igual
    s_pc_plus2.write(s_pc.read() + 2);
}

// =====================================================
//  Stage ID — Decodificación de campos de instrucción
// =====================================================
void Computer_Top_v2::stage_ID_decode() {
    sc_uint<16> instr = s_ifid_instr.read();
    sc_uint<4> op  = instr.range(15, 12);
    sc_uint<3> rd  = instr.range(11,  9);
    sc_uint<3> rs1 = instr.range( 8,  6);
    sc_uint<3> rs2 = instr.range( 5,  3);
    sc_uint<8> imm = instr.range( 8,  1);

    // Para BEZ/BNZ: el registro condición está en Rd [11:9], no en Rs1 [8:6]
    if (op == ISA::BEZ || op == ISA::BNZ) {
        rs1 = rd;  // Leer el registro condicional desde el campo Rd
    }

    s_id_opcode.write(op);
    s_id_rd    .write(rd);
    s_id_rs1   .write(rs1);
    s_id_rs2   .write(rs2);
    s_id_imm8  .write(imm);
}

// =====================================================
//  Stage EX — Forwarding MUXes + ALU + Branch Decision
// =====================================================
void Computer_Top_v2::stage_EX() {
    sc_uint<8> rs1_val = s_idex_rs1_val.read();
    sc_uint<8> rs2_val = s_idex_rs2_val.read();

    // MUX de forwarding para entrada A (Rs1)
    sc_uint<8> alu_a;
    switch (s_fwd_a.read().to_uint()) {
        case 2:  alu_a = s_exmem_alu_result.read(); break; // EX hazard
        case 1:  alu_a = s_exmem_mem_to_reg.read()  // MEM hazard
                         ? s_memwb_mem_data.read()
                         : s_memwb_alu_result.read(); break;
        default: alu_a = rs1_val; break;
    }

    // MUX de forwarding para entrada B (Rs2 o Immediate)
    sc_uint<8> alu_b_reg;
    switch (s_fwd_b.read().to_uint()) {
        case 2:  alu_b_reg = s_exmem_alu_result.read(); break;
        case 1:  alu_b_reg = s_exmem_mem_to_reg.read()
                             ? s_memwb_mem_data.read()
                             : s_memwb_alu_result.read(); break;
        default: alu_b_reg = rs2_val; break;
    }

    // MUX ALU src: 0=Rs2, 1=Imm
    sc_uint<8> alu_b = s_idex_alu_src.read() ? s_idex_imm8.read() : alu_b_reg;

    s_ex_alu_a.write(alu_a);
    s_ex_alu_b.write(alu_b);
    s_ex_rs2_fwd.write(alu_b_reg); // Rs2 con forwarding

    // Branch decision (tomada en EX) — único driver de estas señales
    bool taken = false;
    sc_uint<8> branch_target = s_idex_imm8.read();

    if (s_idex_jump.read()) {
        taken = true;
    } else if (s_idex_branch.read()) {
        bool zero = (alu_a == 0); // condición basada en Rs1
        taken = s_idex_branch_nz.read() ? !zero : zero;
    }

    s_ex_branch_taken .write(taken);
    s_ex_branch_target.write(branch_target);
    
    // flush: señal de branch propagada al IF/ID y ID/EX registers
    s_ifid_flush.write(taken);
    // El flush de ID/EX es (bubble de HDU || branch taken en EX)
    s_idex_flush.write(s_idex_bubble.read() || taken);
}

// =====================================================
//  Stage WB — Escritura a Registros / OUT / HLT
// =====================================================
void Computer_Top_v2::stage_WB() {
    // Mux WB: ¿resultado ALU o dato de memoria?
    sc_uint<8> wb = s_memwb_mem_to_reg.read()
                    ? s_memwb_mem_data .read()
                    : s_memwb_alu_result.read();
    s_wb_data.write(wb);  // único driver de s_wb_data y por tanto de rf->rd_data

    // OUT instruction
    if (s_memwb_out_en.read()) {
        output_reg.write(wb);
        std::cout << "[OUT] " << wb.to_uint()
                  << " (R" << s_memwb_rd.read().to_uint() << ")"
                  << " @ " << sc_time_stamp() << std::endl;
    }

    // HLT
    halt_flag.write(s_memwb_halt.read());
}

// =====================================================
//  Render Framebuffer — Imprime cuadrícula ASCII 8×6
// =====================================================
static void render_fb(const uint8_t* fb) {
    std::cout << "\033[2J\033[H";   // clear terminal
    std::cout << "+--------+\n";
    for (int row = 0; row < 6; ++row) {
        std::cout << "|";
        for (int col = 0; col < 8; ++col) {
            std::cout << (fb[row * 8 + col] ? '#' : ' ');
        }
        std::cout << "|\n";
    }
    std::cout << "+--------+\n";
    std::cout.flush();
}

// =====================================================
//  Stage MEM — MMIO Decode + RAM access
//
//  Memory Map:
//    0x00-0xBF  → RAM normal (gestionada por DualPortRAM)
//    0xC0-0xEF  → Framebuffer (8×6 píxeles, 1 byte/píxel)
//    0xF0       → Keyboard register (read-only)
//    0xF1       → Refresh trigger  (write-only)
// =====================================================
void Computer_Top_v2::stage_MEM() {
    uint8_t addr  = s_exmem_alu_result.read().to_uint();
    uint8_t wdata = s_exmem_rs2_val   .read().to_uint();
    bool    mw    = s_exmem_mem_write  .read();
    bool    mr    = s_exmem_mem_read   .read();

    sc_uint<8> final_data = s_ram_data_out.read();

    if (addr >= 0xC0) {
        if (addr <= 0xEF) {
            if (mw) {
                fb[addr - 0xC0] = wdata;
            } else if (mr) {
                final_data = fb[addr - 0xC0];
            }
        } else if (addr == 0xF0 && mr) {
            final_data = g_current_key.exchange(0); 
        } else if (addr == 0xF1 && mw) {
            render_fb(fb);
        }
    }
    
    s_mem_data_out.write(final_data);
}
