#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include <systemc.h>

SC_MODULE(ControlUnit) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<sc_uint<8>> opcode;
    sc_in<sc_uint<8>> acc_val; // Nueva entrada para leer el acumulador directamente
    sc_in<bool> zero_flag;    // Nueva entrada de bandera

    sc_out<bool> pc_inc, pc_out, pc_load;
    sc_out<bool> mar_load;
    sc_out<bool> ir_load;
    sc_out<bool> ram_out;     // RAM Out Enable
    sc_out<bool> ram_write;   // RAM Write Enable
    sc_out<bool> acc_load;
    sc_out<bool> acc_out;
    sc_out<bool> regB_load;   // Nueva senal para el Registro B
    sc_out<bool> alu_out;     // ALU Out Enable
    sc_out<bool> out_load;    // Nueva senal para el Registro de Salida (Week 11)
    
    // Senales para el Banco de Registros (Fase 2)
    sc_out<bool> reg_file_write;
    sc_out<bool> reg_file_out;
    sc_out<sc_uint<3>> reg_file_sel;

    enum State { T1, T2, T3, T4, T5, T6, T7, T8 };
    sc_signal<State> current_state;

    void process_state_transition();
    void process_output_logic();

    SC_CTOR(ControlUnit) {
        SC_METHOD(process_state_transition);
        sensitive << clk.pos() << reset;

        SC_METHOD(process_output_logic);
        sensitive << current_state << opcode << acc_val;
    }
};

#endif
