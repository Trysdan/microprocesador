#include "RAM16x4.hpp"

void RAM16x4::write_data() {
    // La lógica de escritura solo se ejecuta en el flanco de subida del reloj
    // (Asegurado por la sensibilidad a clk.pos() en el constructor)
    
    if (write_enable.read() == true) {
        // Obtenemos la dirección entera para usarla como índice del arreglo
        unsigned int addr = address.read();
        
        // Protegemos contra desbordamientos, aunque un sc_uint<4> siempre estará entre 0 y 15
        if (addr < 16) {
            mem[addr].write(data_in.read());
        }
    }
}

void RAM16x4::read_data() {
    // La lectura es puramente combinacional (asíncrona)
    unsigned int addr = address.read();
    
    if (addr < 16) {
        data_out.write(mem[addr].read());
    } else {
        data_out.write(0);
    }
}
