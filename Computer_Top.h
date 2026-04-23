#ifndef COMPUTER_TOP_H
#define COMPUTER_TOP_H

#include <systemc.h>
#include "ALU/ALU.hpp"
#include "Registers/RegisterA.hpp"
#include "Registers/RegisterB.hpp"
#include "Memory/RAM16x8.hpp"
#include "Memory/MAR.hpp"
#include "Control/InstructionRegister.hpp"
#include "Control/ProgramCounter.hpp"
#include "Control/ControlUnit.hpp"

SC_MODULE(Computer_Top) {
    sc_in<bool> clk;
    sc_in<bool> reset;

    sc_signal_rv<8> central_bus;

    // Senales de Control
    sc_signal<bool> pc_inc, pc_out, pc_load;
    sc_signal<bool> mar_load;
    sc_signal<bool> ir_load, ir_out;
    sc_signal<bool> ram_out, ram_we;
    sc_signal<bool> acc_load, acc_out;
    sc_signal<bool> regB_load;
    sc_signal<bool> alu_out;

    // Senales de Datos Internas
    sc_signal<sc_uint<4>> ram_addr;
    sc_signal<sc_uint<4>> opcode_ir;
    sc_signal<sc_uint<4>> operand_ir;
    sc_signal<sc_uint<4>> regB_val;
    
    sc_signal<bool> alu_a[4], alu_b[4], alu_op[4], alu_res[4];
    sc_signal<bool> alu_cout;

    ProgramCounter *pc;
    MAR *mar;
    RAM16x8 *ram;
    InstructionRegister *ir;
    ControlUnit *cu;
    ALU *alu;
    RegisterA *regA;
    RegisterB *regB;

    void alu_glue_logic();

    SC_CTOR(Computer_Top) {
        pc = new ProgramCounter("PC");
        pc->clk(clk); pc->reset(reset); pc->pc_inc(pc_inc);
        pc->pc_load(pc_load); pc->pc_out_enable(pc_out);
        pc->data_in(central_bus); pc->data_out(central_bus);

        mar = new MAR("MAR");
        mar->clk(clk); mar->reset(reset); mar->mar_load(mar_load);
        mar->data_in(central_bus); mar->address(ram_addr);

        ram = new RAM16x8("RAM");
        ram->clk(clk); ram->write_enable(ram_we); ram->out_enable(ram_out);
        ram->address(ram_addr); ram->data_in(central_bus); ram->data_out(central_bus);

        ir = new InstructionRegister("IR");
        ir->clk(clk); ir->reset(reset); ir->ir_load(ir_load);
        ir->data_in(central_bus); ir->opcode(opcode_ir); ir->operand(operand_ir);
        ir->ir_out_en(ir_out); ir->bus_out(central_bus);

        cu = new ControlUnit("CU");
        cu->clk(clk); cu->reset(reset); cu->opcode(opcode_ir);
        cu->pc_inc(pc_inc); cu->pc_out(pc_out); cu->pc_load(pc_load); 
        cu->mar_load(mar_load); cu->ir_load(ir_load); cu->ir_out(ir_out); 
        cu->ram_out(ram_out); cu->ram_write(ram_we); cu->acc_load(acc_load); 
        cu->acc_out(acc_out); cu->regB_load(regB_load); cu->alu_out(alu_out);
        
        pc_load.write(false);

        regA = new RegisterA("RegA");
        regA->clk(clk); regA->reset(reset); regA->load_enable(acc_load);
        regA->out_enable(acc_out); regA->data_in(central_bus); regA->data_out(central_bus);

        regB = new RegisterB("RegB");
        regB->clk(clk); regB->reset(reset); regB->load_enable(regB_load);
        regB->data_in(central_bus); regB->data_out(regB_val);

        alu = new ALU("ALU");
        for(int i=0; i<4; i++) {
            alu->a[i](alu_a[i]); alu->b[i](alu_b[i]);
            alu->opcode[i](alu_op[i]); alu->result[i](alu_res[i]);
        }
        alu->cout(alu_cout); alu->alu_out_en(alu_out); alu->data_out(central_bus);

        SC_METHOD(alu_glue_logic);
        sensitive << regA->internal_data << regB_val << opcode_ir;
    }
};

void Computer_Top::alu_glue_logic() {
    sc_uint<4> a_val = regA->internal_data.read();
    sc_uint<4> b_val = regB_val.read();
    
    for(int i=0; i<4; i++) {
        alu_a[i].write(a_val[i]);
        alu_b[i].write(b_val[i]);
        alu_op[i].write(false); 
    }
}

#endif
