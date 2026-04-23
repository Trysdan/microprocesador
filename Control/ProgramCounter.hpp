#ifndef PROGRAMCOUNTER_H
#define PROGRAMCOUNTER_H

#include <systemc.h>

// Modulo ProgramCounter: Gestiona la direccion de la proxima instruccion
SC_MODULE(ProgramCounter) {
    // Puertos de Control
    sc_in<bool> clk;
    sc_in<bool> reset;        // Reset asincrono
    sc_in<bool> pc_inc;       // Incrementar contador
    sc_in<bool> pc_load;      // Cargar desde bus
    sc_in<bool> pc_out_enable; // Habilitar salida al bus (Tri-State)

    // Puertos de Datos (4 bits)
    sc_in<sc_lv<4>> data_in;   // Para pc_load desde el bus
    sc_out<sc_lv<4>> data_out; // Salida al bus

    // Senal interna
    sc_signal<sc_uint<4>> current_value;

    // Procesos
    void update_pc();
    void drive_bus();

    SC_CTOR(ProgramCounter) {
        SC_METHOD(update_pc);
        sensitive << clk.pos() << reset;

        SC_METHOD(drive_bus);
        sensitive << current_value << pc_out_enable;
    }
};

#endif
