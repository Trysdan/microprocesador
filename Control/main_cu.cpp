#include <systemc.h>
#include "ControlUnit.hpp"
#include <iostream>

int sc_main(int argc, char* argv[]) {
    // Reloj
    sc_clock clk("clk", 10, SC_NS, 0.5);

    // Senales de entrada
    sc_signal<bool> reset;
    sc_signal<sc_uint<4>> opcode;

    // Senales de salida (Control)
    sc_signal<bool> pc_inc, pc_out, pc_load;
    sc_signal<bool> mar_load;
    sc_signal<bool> ir_load, ir_out;
    sc_signal<bool> ram_out, ram_write;
    sc_signal<bool> acc_load, acc_out;
    sc_signal<bool> regB_load;
    sc_signal<bool> alu_out;

    // Instancia de la CU
    ControlUnit cu("CU");
    cu.clk(clk);
    cu.reset(reset);
    cu.opcode(opcode);
    cu.pc_inc(pc_inc);
    cu.pc_out(pc_out);
    cu.pc_load(pc_load);
    cu.mar_load(mar_load);
    cu.ir_load(ir_load);
    cu.ir_out(ir_out);
    cu.ram_out(ram_out);
    cu.ram_write(ram_write);
    cu.acc_load(acc_load);
    cu.acc_out(acc_out);
    cu.regB_load(regB_load);
    cu.alu_out(alu_out);

    // Trazado de ondas
    sc_trace_file *wf = sc_create_vcd_trace_file("cu_waves");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, cu.current_state, "state");
    sc_trace(wf, pc_out, "pc_out");
    sc_trace(wf, mar_load, "mar_load");
    sc_trace(wf, ram_out, "ram_out");
    sc_trace(wf, ir_load, "ir_load");
    sc_trace(wf, ir_out, "ir_out");
    sc_trace(wf, acc_load, "acc_load");

    std::cout << "\n--- TESTBENCH UNIDAD DE CONTROL (CU) ---\n";

    // 1. Reset
    reset.write(true);
    opcode.write(0);
    sc_start(15, SC_NS);
    reset.write(false);

    // 2. Simular ciclo de FETCH (T1-T3)
    std::cout << "Simulando Ciclo de Fetch (T1-T3)...\n";
    sc_start(30, SC_NS); // Deberia pasar por T1, T2, T3

    // 3. Simular Ejecucion de LDA (OpCode 0001)
    std::cout << "\nSimulando Ejecucion de LDA (OpCode 1)...\n";
    opcode.write(1);
    sc_start(30, SC_NS); // T4, T5, T6

    std::cout << "\nSimulacion de CU finalizada. Revisa cu_waves.vcd\n";
    sc_close_vcd_trace_file(wf);
    return 0;
}
