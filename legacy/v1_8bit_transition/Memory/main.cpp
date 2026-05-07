#include <systemc.h>
#include "RAM16x8.hpp"
#include <iostream>

int sc_main(int argc, char* argv[]) {
    // Reloj
    sc_clock clk("clk", 10, SC_NS, 0.5);

    // Senales
    sc_signal<bool> ram_we;
    sc_signal<bool> ram_oe;
    sc_signal<sc_uint<4>> ram_addr;
    sc_signal<sc_lv<8>> ram_data_in;
    sc_signal<sc_lv<8>> ram_data_out;

    // Instancia de la RAM
    RAM16x8 ram("RAM");
    ram.clk(clk);
    ram.write_enable(ram_we);
    ram.out_enable(ram_oe);
    ram.address(ram_addr);
    ram.data_in(ram_data_in);
    ram.data_out(ram_data_out);

    // Trazado de ondas
    sc_trace_file *wf = sc_create_vcd_trace_file("ram_waves");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, ram_we, "WE");
    sc_trace(wf, ram_oe, "OE");
    sc_trace(wf, ram_addr, "ADDR");
    sc_trace(wf, ram_data_in, "DATA_IN");
    sc_trace(wf, ram_data_out, "DATA_OUT");

    // Inicializacion
    ram_we.write(false);
    ram_oe.write(false);
    ram_addr.write(0);
    ram_data_in.write("ZZZZZZZZ");

    std::cout << "\n--- TESTBENCH MEMORIA RAM 16x8 ---\n";

    sc_start(5, SC_NS); // Ir a flanco negativo

    // a) Escribir el valor 1010 (10) en la direccion 0
    std::cout << "Escribiendo 10 en la direccion 0...\n";
    ram_addr.write(0);
    ram_data_in.write("00001010");
    ram_we.write(true);
    sc_start(10, SC_NS); // Flanco de subida, guarda dato
    ram_we.write(false);
    ram_data_in.write("ZZZZZZZZ");

    // b) Escribir el valor 0101 (5) en la direccion 15
    std::cout << "Escribiendo 5 en la direccion 15...\n";
    ram_addr.write(15);
    ram_data_in.write("00000101");
    ram_we.write(true);
    sc_start(10, SC_NS);
    ram_we.write(false);
    ram_data_in.write("ZZZZZZZZ");

    // --- LECTURA ---
    std::cout << "\nIniciando lecturas...\n";
    
    // Leer direccion 0 (deberia seguir siendo 1010)
    ram_addr.write(0);
    ram_oe.write(true);
    sc_start(2, SC_NS); // Pequeno avance para dejar que la salida combinacional se asiente
    std::cout << "Lectura Dir 0: " << ram_data_out.read() << "\n";
    
    // Leer direccion 15 (deberia ser 0101)
    ram_addr.write(15);
    sc_start(10, SC_NS);
    std::cout << "Lectura Dir 15: " << ram_data_out.read() << "\n";

    // Deshabilitar salida y verificar Tri-State (Z)
    ram_oe.write(false);
    sc_start(10, SC_NS);
    std::cout << "Lectura con OE=0: " << ram_data_out.read() << " (Esperado ZZZZZZZZ)\n";

    std::cout << "\nSimulacion de RAM finalizada. Revisa ram_waves.vcd\n";
    sc_close_vcd_trace_file(wf);
    return 0;
}
