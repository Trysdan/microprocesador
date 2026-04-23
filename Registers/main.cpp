#include <systemc.h>
#include "RegisterA.hpp"
#include "../ALU/ALU.hpp"
#include <iostream>

// Modulo Adaptador para convertir bool[4] de la ALU estructural a sc_uint<4> del Registro
SC_MODULE(ALU_to_Reg_Adapter) {
    sc_in<bool> alu_res[4];
    sc_out<sc_lv<8>> reg_data_in;

    void convert() {
        sc_lv<8> temp;
        for (int i = 0; i < 4; i++) {
            temp[i] = alu_res[i].read() ? '1' : '0';
        }
        for (int i = 4; i < 8; i++) {
            temp[i] = '0';
        }
        reg_data_in.write(temp);
    }

    SC_CTOR(ALU_to_Reg_Adapter) {
        SC_METHOD(convert);
        for (int i = 0; i < 4; i++) sensitive << alu_res[i];
    }
};

int sc_main(int argc, char* argv[]) {
    // Reloj
    sc_clock clk("clk", 10, SC_NS, 0.5);

    // Senales de la ALU (Estructural)
    sc_signal<bool> alu_a[4], alu_b[4], alu_op[4], alu_res[4];
    sc_signal<bool> alu_cout;

    // Senales del Registro y Control
    sc_signal<bool> reg_rst, reg_le, reg_oe;
    sc_signal_rv<8> bus_signal; // Simulando el bus de datos

    // Instancia de la ALU
    ALU alu("ALU");
    for (int i = 0; i < 4; i++) {
        alu.a[i](alu_a[i]);
        alu.b[i](alu_b[i]);
        alu.opcode[i](alu_op[i]);
        alu.result[i](alu_res[i]);
    }
    alu.cout(alu_cout);
    // Para este testbench antiguo, no usamos el puerto data_out de la ALU integrada
    sc_signal<bool> alu_oe_dummy;
    sc_signal<sc_lv<8>> alu_bus_dummy;
    alu.alu_out_en(alu_oe_dummy);
    alu.data_out(alu_bus_dummy);

    // Instancia del Adaptador
    ALU_to_Reg_Adapter adapter("Adapter");
    for (int i = 0; i < 4; i++) adapter.alu_res[i](alu_res[i]);
    adapter.reg_data_in(bus_signal);

    // Instancia del Registro (Acumulador)
    RegisterA acc("Accumulator");
    acc.clk(clk);
    acc.reset(reg_rst);
    acc.load_enable(reg_le);
    acc.out_enable(reg_oe);
    acc.data_in(bus_signal);
    acc.data_out(bus_signal);

    // Trazado de ondas
    sc_trace_file *wf = sc_create_vcd_trace_file("register_waves");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, reg_rst, "reset");
    sc_trace(wf, reg_le, "load_enable");
    sc_trace(wf, bus_signal, "bus_data");

    // Funcion auxiliar para inyectar datos a los puertos bool[4]
    auto set_alu_val = [&](sc_signal<bool>* port, int val) {
        for (int i = 0; i < 4; i++) port[i].write((val >> i) & 1);
    };

    // --- SECUENCIA DE PRUEBA ---
    reg_rst.write(true);
    reg_le.write(false);
    reg_oe.write(false);
    alu_oe_dummy.write(false);
    set_alu_val(alu_op, 0); // Suma
    set_alu_val(alu_a, 0);
    set_alu_val(alu_b, 0);

    sc_start(15, SC_NS);
    reg_rst.write(false);

    // 1. Calcular 5 + 3 en la ALU
    std::cout << "Calculando 5 + 3 en la ALU...\n";
    set_alu_val(alu_a, 5); // 0101
    set_alu_val(alu_b, 3); // 0011
    sc_start(10, SC_NS);

    // 2. Cargar el resultado (8) en el RegistroA
    std::cout << "Cargando resultado en el acumulador (RegisterA)...\n";
    reg_le.write(true);
    sc_start(10, SC_NS);
    reg_le.write(false);

    // 3. Cambiar ALU para verificar que el RegistroA mantiene el valor
    std::cout << "Cambiando entradas de la ALU (el registro debe mantener el valor)...\n";
    set_alu_val(alu_a, 0);
    sc_start(10, SC_NS);

    std::cout << "Simulacion de Registro finalizada. Revisa register_waves.vcd\n";
    sc_close_vcd_trace_file(wf);
    return 0;
}
