#include <systemc.h>
#include "../Memory/RAM16x4.hpp"
#include "../Registers/Register4Bit.hpp"
#include <iostream>

int sc_main(int argc, char* argv[]) {
    // Reloj
    sc_clock clk("clk", 10, SC_NS, 0.5);

    // ==========================================
    // BUS CENTRAL RESOLVED (Soporta múltiples escritores y Z)
    // ==========================================
    sc_signal_rv<4> central_bus; 

    // Señales de Control RAM
    sc_signal<bool> ram_we;
    sc_signal<bool> ram_oe;
    sc_signal<sc_uint<4>> ram_addr;
    sc_signal<sc_uint<4>> ram_data_in; // Datos inyectados desde el testbench

    // Señales de Control Registro
    sc_signal<bool> reg_rst;
    sc_signal<bool> reg_le;
    sc_signal<bool> reg_oe;

    // Instancia de la RAM
    RAM16x4 ram("RAM");
    ram.clk(clk);
    ram.write_enable(ram_we);
    ram.out_enable(ram_oe);
    ram.address(ram_addr);
    ram.data_in(ram_data_in);
    ram.data_out(central_bus); // RAM Escribe al Bus Central

    // Instancia del Registro A
    Register4Bit regA("RegA");
    regA.clk(clk);
    regA.reset(reg_rst);
    regA.load_enable(reg_le);
    regA.out_enable(reg_oe);
    regA.data_in(central_bus);   // RegA Lee del Bus Central
    regA.data_out(central_bus);  // RegA Escribe al Bus Central (¡Mismo Bus!)

    // Trazado de ondas
    sc_trace_file *wf = sc_create_vcd_trace_file("bus_waves");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, central_bus, "CENTRAL_BUS");
    sc_trace(wf, ram_oe, "RAM_OE");
    sc_trace(wf, reg_le, "REG_LE");
    sc_trace(wf, reg_oe, "REG_OE");

    // Inicialización de señales
    ram_we.write(false);
    ram_oe.write(false);
    ram_addr.write(0);
    ram_data_in.write(0);
    
    reg_rst.write(true);
    reg_le.write(false);
    reg_oe.write(false);

    std::cout << "\n--- TESTBENCH BUS CENTRAL Y LOGICA TRI-STATE ---\n";
    std::cout << "T=" << sc_time_stamp() << " | Bus: " << central_bus.read() << " (Inicial)\n\n";

    sc_start(5, SC_NS); // Bajar a flanco negativo
    reg_rst.write(false);

    // a) Cargar un valor en la RAM silenciosamente (desde el testbench, sin usar el bus)
    std::cout << "T=" << sc_time_stamp() << " | [Paso A] Escribiendo '1100' en RAM addr 0.\n";
    ram_addr.write(0);
    ram_data_in.write(12); // 12 en binario es 1100
    ram_we.write(true);
    sc_start(10, SC_NS); // Pasa el flanco de subida, guarda el dato
    ram_we.write(false); 
    
    std::cout << "T=" << sc_time_stamp() << " | Bus Central (ambos OE=0): " << central_bus.read() << "\n\n";

    // b) Habilita RAM_out_enable y Reg_load_enable (el dato viaja por el bus)
    std::cout << "T=" << sc_time_stamp() << " | [Paso B] RAM_OE=1, REG_LE=1. RAM empuja al bus, RegA captura.\n";
    ram_oe.write(true);   // RAM se adueña del bus
    reg_le.write(true);   // RegA se prepara para leer del bus
    sc_start(5, SC_NS);   // Esperamos a que la señal combinacional se propague
    std::cout << "T=" << sc_time_stamp() << " | Bus Central: " << central_bus.read() << " (La RAM domina el bus)\n";
    
    sc_start(5, SC_NS);   // Flanco de subida, RegA guarda el dato
    reg_le.write(false);  // RegA deja de escuchar

    // c) Deshabilita ambos y verifica que el bus quede en alta impedancia
    std::cout << "\n" << "T=" << sc_time_stamp() << " | [Paso C] Apagando RAM_OE. Nadie escribe en el bus.\n";
    ram_oe.write(false);
    sc_start(10, SC_NS);
    std::cout << "T=" << sc_time_stamp() << " | Bus Central: " << central_bus.read() << " (Alta Impedancia)\n\n";

    // Extra: Que el Registro A empuje lo que memorizó al bus
    std::cout << "T=" << sc_time_stamp() << " | [Extra] REG_OE=1. RegA expone su valor almacenado al bus.\n";
    reg_oe.write(true);
    sc_start(10, SC_NS);
    std::cout << "T=" << sc_time_stamp() << " | Bus Central: " << central_bus.read() << " (RegA domina el bus)\n";

    std::cout << "\nSimulacion completada exitosamente. Revisa bus_waves.vcd\n";
    sc_close_vcd_trace_file(wf);
    return 0;
}
