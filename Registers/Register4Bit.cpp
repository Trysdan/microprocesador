#include "Register4Bit.hpp"

void Register4Bit::write_data() {
    // 1. Verificamos primero el reset asíncrono
    if (reset.read() == true) {
        data_out.write(0); // Limpiar el registro
    } 
    // 2. Si no hay reset, y estamos en un flanco de reloj
    else if (clk.event() && clk.read() == true) {
        if (load_enable.read() == true) {
            data_out.write(data_in.read()); // Guardar el nuevo dato
        }
        // Si load_enable es falso, la señal mantiene su valor anterior por defecto
    }
}
