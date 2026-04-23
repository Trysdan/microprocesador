#include <systemc.h>
#include "RAM16x4.hpp"
#include <iostream>

int sc_main(int argc, char* argv[]) {
    // Reloj
    sc_clock clk("clk", 10, SC_NS, 0.5);

    // Señales
    sc_signal<bool> write_enable;
    sc_signal<sc_uint<4>> address;
    sc_signal<sc_uint<4>> data_in;
    sc_signal<sc_uint<4>> data_out;

    // Instancia de la Memoria RAM
    RAM16x4 ram("RAM_16x4");
    ram.clk(clk);
    ram.write_enable(write_enable);
    ram.address(address);
    ram.data_in(data_in);
    ram.data_out(data_out);

    // Archivo de trazas (.vcd)
    sc_trace_file *wf = sc_create_vcd_trace_file("ram_waves");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, write_enable, "write_enable");
    sc_trace(wf, address, "address");
    sc_trace(wf, data_in, "data_in");
    sc_trace(wf, data_out, "data_out");

    // Inicialización
    write_enable.write(false);
    address.write(0);
    data_in.write(0);

    std::cout << "\n--- TESTBENCH MEMORIA RAM 16x4 ---\n";

    sc_start(5, SC_NS); // Offset para no sincronizar los cambios exactamente con el flanco de subida

    // a) Escribir el valor 1010 (10) en la dirección 0
    std::cout << "T=" << sc_time_stamp() << " | [Paso A] Escribiendo 1010 (10) en la direccion 0..." << std::endl;
    address.write(0);
    data_in.write(10);
    write_enable.write(true);
    sc_start(10, SC_NS); // Cruza el flanco de reloj (T=10ns)
    
    // Verificamos lectura inmediatamente gracias a que la lectura es combinacional
    std::cout << "T=" << sc_time_stamp() << " | Lectura combinacional en addr 0 = " << data_out.read().to_string(SC_BIN) << "\n\n";

    // b) Escribir el valor 0101 (5) en la dirección 15
    std::cout << "T=" << sc_time_stamp() << " | [Paso B] Escribiendo 0101 (5) en la direccion 15..." << std::endl;
    address.write(15);
    data_in.write(5);
    write_enable.write(true);
    sc_start(10, SC_NS); 
    std::cout << "T=" << sc_time_stamp() << " | Lectura combinacional en addr 15 = " << data_out.read().to_string(SC_BIN) << "\n\n";

    // c) Desactivar write_enable e intentar sobreescribir la direccion 0
    std::cout << "T=" << sc_time_stamp() << " | [Paso C] Intentando sobreescribir la direccion 0 con 1111 (15) PERO con WE=0..." << std::endl;
    address.write(0);
    data_in.write(15);
    write_enable.write(false); // PROTEGER!
    sc_start(10, SC_NS); 

    // d) Leer ambas direcciones y verificar
    std::cout << "T=" << sc_time_stamp() << " | [Paso D] Verificacion Final:" << std::endl;
    
    // Leer dirección 0 (debería seguir siendo 1010)
    address.write(0);
    sc_start(2, SC_NS); // Pequeño avance para dejar que la salida combinacional se asiente
    std::cout << " -> Direccion  0 contiene: " << data_out.read().to_string(SC_BIN) << " (Esperado: 1010)" << std::endl;

    // Leer dirección 15 (debería ser 0101)
    address.write(15);
    sc_start(2, SC_NS);
    std::cout << " -> Direccion 15 contiene: " << data_out.read().to_string(SC_BIN) << " (Esperado: 0101)" << std::endl;

    std::cout << "\nSimulacion completada. Revisa ram_waves.vcd para ver las ondas.\n";

    sc_close_vcd_trace_file(wf);
    return 0;
}
