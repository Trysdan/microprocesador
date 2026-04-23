#include <systemc.h>
#include "ProgramCounter.hpp"
#include <iostream>

int sc_main(int argc, char* argv[]) {
    // Reloj
    sc_clock clk("clk", 10, SC_NS, 0.5);

    // Senales de control
    sc_signal<bool> reset, pc_inc, pc_load, pc_oe;
    sc_signal<sc_lv<4>> data_in, data_out;

    // Instancia del PC
    ProgramCounter pc("PC");
    pc.clk(clk);
    pc.reset(reset);
    pc.pc_inc(pc_inc);
    pc.pc_load(pc_load);
    pc.pc_out_enable(pc_oe);
    pc.data_in(data_in);
    pc.data_out(data_out);

    // Trazado de ondas
    sc_trace_file *wf = sc_create_vcd_trace_file("pc_waves");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, pc_inc, "pc_inc");
    sc_trace(wf, pc_load, "pc_load");
    sc_trace(wf, pc_oe, "pc_oe");
    sc_trace(wf, data_out, "BUS_OUT");

    std::cout << "\n--- TESTBENCH PROGRAM COUNTER (PC) ---\n";

    // 1. Inicializacion y Reset
    reset.write(true);
    pc_inc.write(false);
    pc_load.write(false);
    pc_oe.write(true); // Ver el valor inicial
    data_in.write("ZZZZ");
    
    sc_start(15, SC_NS);
    reset.write(false);
    std::cout << "T=" << sc_time_stamp() << " | Despues de Reset: " << data_out.read() << " (Esperado: 0)\n";

    // 2. Incrementar 3 veces
    std::cout << "\n[Prueba] Incrementando 3 veces...\n";
    pc_inc.write(true);
    sc_start(30, SC_NS); // 3 ciclos
    pc_inc.write(false);
    std::cout << "T=" << sc_time_stamp() << " | Valor actual: " << data_out.read() << " (Esperado: 3)\n";

    // 3. Cargar valor desde el bus (1010)
    std::cout << "\n[Prueba] Cargando valor 1010 desde el bus...\n";
    data_in.write("1010");
    pc_load.write(true);
    sc_start(10, SC_NS); // Capturar en flanco de subida
    pc_load.write(false);
    data_in.write("ZZZZ");
    std::cout << "T=" << sc_time_stamp() << " | Valor despues de carga: " << data_out.read() << " (Esperado: 10)\n";

    // 4. Probar Tri-State (pc_out_enable = 0)
    std::cout << "\n[Prueba] Apagando pc_out_enable (Tri-State)...\n";
    pc_oe.write(false);
    sc_start(5, SC_NS); // Cambio combinacional
    std::cout << "T=" << sc_time_stamp() << " | Valor en bus: " << data_out.read() << " (Esperado: ZZZZ)\n";

    pc_oe.write(true);
    sc_start(5, SC_NS);
    std::cout << "T=" << sc_time_stamp() << " | Restaurando OE, valor en bus: " << data_out.read() << " (Esperado: 10)\n";

    std::cout << "\nSimulacion de PC finalizada exitosamente.\n";
    sc_close_vcd_trace_file(wf);
    return 0;
}
