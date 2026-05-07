#ifndef PIPELINE_TYPES_HPP
#define PIPELINE_TYPES_HPP

#include <systemc.h>

// =====================================================
//  ISA de 16 bits — Constantes de OpCode
// =====================================================
namespace ISA {
    constexpr uint8_t STORI = 0x0;  // R-type: MEM[Rs2] <- Rs1  (store indirecto)
    constexpr uint8_t LOAD  = 0x1;  // I-type: Rd <- MEM[imm8]
    constexpr uint8_t STORE = 0x2;  // I-type: MEM[imm8] <- Rd
    constexpr uint8_t MOV   = 0x3;  // R-type: Rd <- Rs1
    constexpr uint8_t MVI   = 0x4;  // I-type: Rd <- imm8
    constexpr uint8_t ADD   = 0x5;  // R-type: Rd <- Rs1 + Rs2
    constexpr uint8_t SUB   = 0x6;  // R-type: Rd <- Rs1 - Rs2
    constexpr uint8_t AND   = 0x7;  // R-type: Rd <- Rs1 & Rs2
    constexpr uint8_t OR    = 0x8;  // R-type: Rd <- Rs1 | Rs2
    constexpr uint8_t XOR   = 0x9;  // R-type: Rd <- Rs1 ^ Rs2
    constexpr uint8_t LOADI = 0xA;  // R-type: Rd <- MEM[Rs1] (load indirecto)
    constexpr uint8_t JMP   = 0xB;  // I-type: PC <- imm8
    constexpr uint8_t BEZ   = 0xC;  // I-type: if(Rd==0) PC <- imm8
    constexpr uint8_t BNZ   = 0xD;  // I-type: if(Rd!=0) PC <- imm8
    constexpr uint8_t OUT   = 0xE;  // R-type: OUTPUT <- Rs1
    constexpr uint8_t HLT   = 0xF;  // SYS:   detener
}

// =====================================================
//  Formato de Instrucción de 16 bits
//
//  Tipo-R: [15:12]=OpCode [11:9]=Rd [8:6]=Rs1 [5:3]=Rs2 [2:0]=xxx
//  Tipo-I: [15:12]=OpCode [11:9]=Rd [8:1]=imm8            [0]=x
// =====================================================
struct InstrFields {
    sc_uint<4> opcode;
    sc_uint<3> rd;
    sc_uint<3> rs1;
    sc_uint<3> rs2;
    sc_uint<8> imm8;

    void decode(sc_uint<16> instr) {
        opcode = instr.range(15, 12);
        rd     = instr.range(11,  9);
        rs1    = instr.range( 8,  6);
        rs2    = instr.range( 5,  3);
        imm8   = instr.range( 8,  1);
    }
};

// =====================================================
//  Señales de Control que fluyen a través del pipeline
//  Generadas en ID (Decoder) y propagadas hasta WB
// =====================================================
struct ControlSignals {
    bool reg_write;    // WB:  escribir resultado en RegisterFile
    bool mem_read;     // MEM: leer de RAM (LOAD)
    bool mem_write;    // MEM: escribir en RAM (STORE)
    bool alu_src;      // EX:  0=Rs2, 1=Inmediato
    bool branch;       // EX:  salto condicional (BEZ/BNZ)
    bool branch_nz;    // EX:  1=BNZ, 0=BEZ
    bool jump;         // EX:  salto incondicional (JMP)
    bool mem_to_reg;   // WB:  0=resultado ALU, 1=dato de memoria
    bool out_en;       // SYS: OUT instruction
    bool halt;         // SYS: HLT
    sc_uint<4> alu_op; // EX:  operación de la ALU (= opcode)

    // NOP por defecto
    ControlSignals() :
        reg_write(false), mem_read(false), mem_write(false),
        alu_src(false), branch(false), branch_nz(false),
        jump(false), mem_to_reg(false), out_en(false),
        halt(false), alu_op(0) {}
};

#endif // PIPELINE_TYPES_HPP
