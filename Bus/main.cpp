#include <systemc.h>
#include "../Memory/RAM16x8.hpp"
#include "../Registers/RegisterA.hpp"
#include <iostream>

int sc_main(int argc, char* argv[]) {
    // Reloj
    sc_clock clk("clk", 10, SC_NS, 0.5);

    // ==========================================
    // BUS CENTRAL RESOLVED (Soporta multiples escritores y Z)
    // ==========================================
    sc_signal_rv<8> central_bus; 

    // Senales de Control RAM
    sc_signal<bool> ram_we;
    sc_signal<bool> ram_oe;
    sc_signal<sc_uint<4>> ram_addr;
    sc_signal<sc_lv<8>> ram_data_in;

    // Senales de Control Registro
    sc_signal<bool> reg_rst;
    sc_signal<bool> reg_le;
    sc_signal<bool> reg_oe;

    // Instancia de la RAM
    RAM16x8 ram("RAM");
    ram.clk(clk);
    ram.write_enable(ram_we);
    ram.out_enable(ram_oe);
    ram.address(ram_addr);
    ram.data_in(ram_data_in);
    ram.data_out(central_bus);

    // Instancia del Registro A
    RegisterA regA("RegA");
    regA.clk(clk);
    regA.reset(reg_rst);
    regA.load_enable(reg_le);
    regA.out_enable(reg_oe);
    regA.data_in(central_bus);
    regA.data_out(central_bus);

    // Trazado de ondas
    sc_trace_file *wf = sc_create_vcd_trace_file("bus_waves");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, central_bus, "CENTRAL_BUS");

    // Inicializacion
    ram_we.write(false);
    ram_oe.write(false);
    ram_addr.write(0);
    ram_data_in.write("ZZZZZZZZ");
    
    reg_rst.write(true);
    reg_le.write(false);
    reg_oe.write(false);

    std::cout << "\n--- TESTBENCH BUS CENTRAL Y LOGICA TRI-STATE (8-BIT) ---\n";

    sc_start(5, SC_NS);
    reg_rst.write(false);

    // a) Cargar un valor en la RAM silenciosamente
    std::cout << "[Paso A] Escribiendo '00001100' en RAM addr 0.\n";
    ram_addr.write(0);
    ram_data_in.write("00001100");
    ram_we.write(true);
    sc_start(10, SC_NS);
    ram_we.write(false); 
    ram_data_in.write("ZZZZZZZZ");
    
    // b) Habilita RAM_OE y Reg_LE
    std::cout << "[Paso B] RAM_OE=1, REG_LE=1. RAM empuja al bus, RegA captura.\n";
    ram_oe.write(true);
    reg_le.write(true);
    sc_start(10, SC_NS);
    reg_le.write(false);
    ram_oe.write(false);

    // c) Registro A empuja al bus
    std::cout << "[Paso C] REG_OE=1. RegA expone su valor al bus.\n";
    reg_oe.write(true);
    sc_start(10, SC_NS);
    std::cout << "Bus Central: " << central_bus.read() << "\n";

    std::cout << "\nSimulacion de Bus finalizada. Revisa bus_waves.vcd\n";
    sc_close_vcd_trace_file(wf);
    return 0;
}
