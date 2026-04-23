#include <systemc.h>
#include "../ALU/ALU.hpp"
#include "Register4Bit.hpp"
#include <iostream>

// Módulo Adaptador para convertir bool[4] de la ALU estructural a sc_uint<4> del Registro
SC_MODULE(BusAdapter) {
    sc_in<bool> in_bits[4];
    sc_out<sc_lv<4>> out_bus;

    void convert() {
        sc_lv<4> temp = "0000";
        for (int i = 0; i < 4; i++) {
            temp[i] = in_bits[i].read() ? '1' : '0';
        }
        out_bus.write(temp);
    }

    SC_CTOR(BusAdapter) {
        SC_METHOD(convert);
        for (int i = 0; i < 4; i++) {
            sensitive << in_bits[i];
        }
    }
};

int sc_main(int argc, char* argv[]) {
    // Reloj
    sc_clock clk("clk", 10, SC_NS, 0.5);

    // Señales de la ALU (Estructural)
    sc_signal<bool> alu_a[4], alu_b[4], opcode[4], alu_out_bits[4];
    sc_signal<bool> alu_cout;

    // Señales del Registro y Control
    sc_signal<bool> reset, load_enable, out_enable;
    sc_signal<sc_lv<4>> reg_in_bus; // Cable intermedio (Salida Adaptador -> Entrada Registro)
    sc_signal<sc_lv<4>> reg_out_bus;

    // 1. Instancia de la ALU Estructural
    ALU ula("MIP_ALU");
    for (int i = 0; i < 4; i++) {
        ula.a[i](alu_a[i]);
        ula.b[i](alu_b[i]);
        ula.opcode[i](opcode[i]);
        ula.result[i](alu_out_bits[i]);
    }
    ula.cout(alu_cout);

    // 2. Instancia del Adaptador de Bus (bool[4] -> sc_uint<4>)
    BusAdapter adapter("Adapter");
    for (int i = 0; i < 4; i++) {
        adapter.in_bits[i](alu_out_bits[i]);
    }
    adapter.out_bus(reg_in_bus);

    // 3. Instancia del Registro (El Acumulador)
    Register4Bit acc("Acumulador");
    acc.clk(clk);
    acc.reset(reset);
    acc.load_enable(load_enable);
    acc.out_enable(out_enable);
    acc.data_in(reg_in_bus); // Conectado a la salida del adaptador
    acc.data_out(reg_out_bus);

    // Trazado de ondas
    sc_trace_file *wf = sc_create_vcd_trace_file("acumulador_waves");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, load_enable, "load_enable");
    for (int i=0; i<4; i++) {
        sc_trace(wf, alu_out_bits[i], "ALU_OUT_bit_" + std::to_string(i));
    }
    sc_trace(wf, reg_in_bus, "ALU_OUT_BUS");
    sc_trace(wf, reg_out_bus, "ACC_OUT_BUS");

    // Función auxiliar para inyectar datos a los puertos bool[4]
    auto set_inputs = [&](int va, int vb, int op) {
        for(int i=0; i<4; i++) {
            alu_a[i].write((va >> i) & 1);
            alu_b[i].write((vb >> i) & 1);
            opcode[i].write((op >> i) & 1);
        }
    };

    // Inicializacion
    reset.write(true);
    load_enable.write(false);
    out_enable.write(true); // Siempre habilitado en el testbench original
    set_inputs(0, 0, 0);
    
    std::cout << "\n--- TESTBENCH ACUMULADOR (ALU ESTRUCTURAL + REGISTRO) ---\n";

    sc_start(5, SC_NS); // Avanzar al flanco de bajada (evitar condiciones de carrera)
    reset.write(false);

    // Prueba 1: Cargar el resultado de una suma
    set_inputs(5, 3, 0); // 0 = SUMA
    load_enable.write(true);
    std::cout << "T=" << sc_time_stamp() << " | ALU_A=5, ALU_B=3 | SUMA (Deberia ser 8). Esperando flanco..." << std::endl;
    sc_start(10, SC_NS); 
    std::cout << "T=" << sc_time_stamp() << " | ALU_OUT = " << reg_in_bus.read() << " | Registro ACC = " << reg_out_bus.read() << "\n\n";

    // Prueba 2: Cambiar operacion pero NO habilitar carga
    set_inputs(10, 1, 1); // 1 = RESTA (10 - 1 = 9)
    load_enable.write(false); // Deshabilitamos la carga!
    std::cout << "T=" << sc_time_stamp() << " | ALU_A=10, ALU_B=1 | RESTA. (Salida ALU debe ser 9), PERO Load_Enable=0." << std::endl;
    sc_start(10, SC_NS);
    std::cout << "T=" << sc_time_stamp() << " | ALU_OUT_BUS = " << reg_in_bus.read() << " | Registro ACC = " << reg_out_bus.read() << " (Se mantuvo el 8!)" << "\n\n";

    // Prueba 3: Habilitar carga del nuevo valor
    load_enable.write(true);
    std::cout << "T=" << sc_time_stamp() << " | Habilitando Load_Enable=1..." << std::endl;
    sc_start(10, SC_NS);
    std::cout << "T=" << sc_time_stamp() << " | Registro ACC = " << reg_out_bus.read() << " (Se actualizo a 9!)" << "\n";

    sc_close_vcd_trace_file(wf);
    return 0;
}
