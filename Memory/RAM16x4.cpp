#include "RAM16x4.hpp"

void RAM16x4::write_data() {
    // La lógica de escritura solo se ejecuta en el flanco de subida del reloj
    if (write_enable.read() == true) {
        unsigned int addr = address.read();
        if (addr < 16) {
            mem[addr].write(data_in.read());
        }
    }
}

void RAM16x4::read_data() {
    // Lógica de Lectura Combinacional y Tri-State
    if (out_enable.read() == true) {
        unsigned int addr = address.read();
        if (addr < 16) {
            data_out.write(mem[addr].read());
        } else {
            data_out.write(0); // Opcional: Podría ser ZZZZ para direcciones inválidas
        }
    } else {
        // Alta impedancia
        data_out.write("ZZZZ");
    }
}
