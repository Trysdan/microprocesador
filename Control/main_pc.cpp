#include <systemc.h>
#include "ProgramCounter.hpp"
#include <iostream>

int sc_main(int argc, char* argv[]) {
    // Reloj
    sc_clock clk("clk", 10, SC_NS, 0.5);

    // Senales de prueba
    sc_signal<bool> reset;
    sc_signal<bool> pc_inc;
    sc_signal<bool> pc_load;
    sc_signal<bool> pc_oe;
    sc_signal<sc_lv<8>> bus_signal;

    // Instancia del PC
    ProgramCounter pc("PC");
    pc.clk(clk);
    pc.reset(reset);
    pc.pc_inc(pc_inc);
    pc.pc_load(pc_load);
    pc.pc_out_enable(pc_oe);
    pc.data_in(bus_signal);
    pc.data_out(bus_signal);

    // Trazado de ondas
    sc_trace_file *wf = sc_create_vcd_trace_file("pc_waves");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, pc_inc, "pc_inc");
    sc_trace(wf, pc_oe, "pc_oe");
    sc_trace(wf, bus_signal, "bus_data");

    std::cout << "\n--- TESTBENCH PROGRAM COUNTER (PC) ---\n";

    // 1. Reset
    reset.write(true);
    pc_inc.write(false);
    pc_load.write(false);
    pc_oe.write(false);
    sc_start(15, SC_NS);
    reset.write(false);
    std::cout << "Reset finalizado. PC=" << bus_signal.read() << "\n";

    // 2. Incrementar 3 veces
    pc_oe.write(true); // Habilitar salida al bus
    for(int i=0; i<3; i++) {
        pc_inc.write(true);
        sc_start(10, SC_NS);
        pc_inc.write(false);
        std::cout << "Incremento " << (i+1) << ": " << bus_signal.read() << "\n";
    }

    // 3. Cargar valor desde el bus (ej. 10 -> 1010)
    std::cout << "\nCargando valor 10 desde el bus...\n";
    pc_oe.write(false); // Liberar bus para inyectar dato
    bus_signal.write("00001010");
    pc_load.write(true);
    sc_start(10, SC_NS);
    pc_load.write(false);
    bus_signal.write("ZZZZZZZZ"); // Liberar bus tras carga
    
    pc_oe.write(true);
    sc_start(2, SC_NS);
    std::cout << "Valor cargado en PC: " << bus_signal.read() << " (Esperado 00001010)\n";

    // 4. Verificar Tri-State
    pc_oe.write(false);
    sc_start(10, SC_NS);
    std::cout << "OE=0, Bus=" << bus_signal.read() << " (Esperado ZZZZZZZZ)\n";

    std::cout << "\nSimulacion de PC finalizada. Revisa pc_waves.vcd\n";
    sc_close_vcd_trace_file(wf);
    return 0;
}
