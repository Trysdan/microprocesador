#ifndef RAM16X4_H
#define RAM16X4_H

#include <systemc.h>

SC_MODULE(RAM16x4) {
    // Puertos
    sc_in<bool> clk;
    sc_in<bool> write_enable;
    sc_in<sc_uint<4>> address;
    sc_in<sc_uint<4>> data_in;
    
    sc_out<sc_uint<4>> data_out;

    // Almacenamiento interno (16 registros de 4 bits)
    // Usamos sc_signal para que la lectura combinacional pueda ser sensible a cambios internos
    sc_signal<sc_uint<4>> mem[16];

    // Procesos
    void write_data();
    void read_data();

    SC_CTOR(RAM16x4) {
        // Inicializar memoria en 0
        for (int i = 0; i < 16; i++) {
            mem[i].write(0);
        }

        // Lógica Síncrona: Escritura
        SC_METHOD(write_data);
        sensitive << clk.pos();

        // Lógica Combinacional: Lectura
        SC_METHOD(read_data);
        sensitive << address;
        for (int i = 0; i < 16; i++) {
            sensitive << mem[i];
        }
    }
};

#endif
