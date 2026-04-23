#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include <systemc.h>

// Modulo ControlUnit: La FSM que orquesta el microprocesador
SC_MODULE(ControlUnit) {
    // Entradas
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<sc_uint<4>> opcode; // Viene del Instruction Register

    // Senales de Control (Salidas)
    sc_out<bool> pc_inc;
    sc_out<bool> pc_out;
    sc_out<bool> mar_load;
    sc_out<bool> ir_load;
    sc_out<bool> ir_out;      // Habilitar operando del IR al bus
    sc_out<bool> ram_out;     // RAM Out Enable
    sc_out<bool> ram_write;   // RAM Write Enable
    sc_out<bool> acc_load;
    sc_out<bool> acc_out;
    sc_out<bool> alu_out;     // ALU Out Enable

    // Definicion de estados de la FSM
    enum State { T1, T2, T3, T4, T5, T6 };
    sc_signal<State> current_state;

    // Metodos
    void process_fsm();
    void reset_signals();

    SC_CTOR(ControlUnit) {
        SC_METHOD(process_fsm);
        sensitive << clk.pos() << reset;
    }
};

#endif
