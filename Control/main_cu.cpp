#include <systemc.h>
#include "ControlUnit.hpp"
#include <iostream>

int sc_main(int argc, char* argv[]) {
    sc_clock clk("clk", 10, SC_NS, 0.5);
    sc_signal<bool> reset;
    sc_signal<sc_uint<4>> opcode;

    // Senales de salida de la CU
    sc_signal<bool> pc_inc, pc_out, mar_load, ir_load, ir_out;
    sc_signal<bool> ram_out, ram_write, acc_load, acc_out, alu_out;

    // Instancia de la Unidad de Control
    ControlUnit cu("CU");
    cu.clk(clk);
    cu.reset(reset);
    cu.opcode(opcode);
    
    cu.pc_inc(pc_inc);
    cu.pc_out(pc_out);
    cu.mar_load(mar_load);
    cu.ir_load(ir_load);
    cu.ir_out(ir_out);
    cu.ram_out(ram_out);
    cu.ram_write(ram_write);
    cu.acc_load(acc_load);
    cu.acc_out(acc_out);
    cu.alu_out(alu_out);

    // Trazado
    sc_trace_file *wf = sc_create_vcd_trace_file("cu_waves");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, cu.current_state, "state"); // sc_signal de enum se puede trazar
    sc_trace(wf, pc_out, "pc_out");
    sc_trace(wf, mar_load, "mar_load");
    sc_trace(wf, ram_out, "ram_out");
    sc_trace(wf, acc_load, "acc_load");

    std::cout << "\n--- TESTBENCH UNIDAD DE CONTROL (CU) ---\n";

    // 1. Reset
    reset.write(true);
    opcode.write(0);
    sc_start(15, SC_NS);
    reset.write(false);

    // 2. Probar Ciclo FETCH (T1-T3)
    std::cout << "T=" << sc_time_stamp() << " | [Fetch T1] pc_out: " << pc_out.read() << " mar_load: " << mar_load.read() << "\n";
    sc_start(10, SC_NS);
    std::cout << "T=" << sc_time_stamp() << " | [Fetch T2] pc_inc: " << pc_inc.read() << "\n";
    sc_start(10, SC_NS);
    std::cout << "T=" << sc_time_stamp() << " | [Fetch T3] ram_out: " << ram_out.read() << " ir_load: " << ir_load.read() << "\n";

    // 3. Probar Execute LDA (OpCode 0001)
    std::cout << "\n[Prueba] Ejecutando LDA (OpCode 0001)...\n";
    opcode.write(0x1);
    sc_start(10, SC_NS);
    std::cout << "T=" << sc_time_stamp() << " | [LDA T4] ir_out: " << ir_out.read() << " mar_load: " << mar_load.read() << "\n";
    sc_start(10, SC_NS);
    std::cout << "T=" << sc_time_stamp() << " | [LDA T5] ram_out: " << ram_out.read() << " acc_load: " << acc_load.read() << "\n";
    sc_start(10, SC_NS);
    std::cout << "T=" << sc_time_stamp() << " | [LDA T6] (NOP)\n";

    // 4. Probar ciclo siguiente (vuelve a T1)
    sc_start(10, SC_NS);
    std::cout << "\nT=" << sc_time_stamp() << " | [Fetch T1 de nuevo] pc_out: " << pc_out.read() << "\n";

    std::cout << "\nSimulacion de CU finalizada.\n";
    sc_close_vcd_trace_file(wf);
    return 0;
}
