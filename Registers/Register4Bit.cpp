#include "Register4Bit.hpp"

void Register4Bit::write_data() {
    // 1. Verificamos primero el reset asíncrono
    if (reset.read() == true) {
        internal_data.write(0); // Limpiar el registro internamente
    } 
    // 2. Si no hay reset, y estamos en un flanco de reloj
    else if (clk.event() && clk.read() == true) {
        if (load_enable.read() == true) {
            internal_data.write(data_in.read().to_uint()); // Guardar el nuevo dato (convirtiendo sc_lv a sc_uint)
        }
    }
}

void Register4Bit::read_data() {
    // Lógica Tri-State Combinacional
    if (out_enable.read() == true) {
        // Enviar el dato al bus
        // SystemC convierte sc_uint a sc_lv automáticamente
        data_out.write(internal_data.read());
    } else {
        // Alta impedancia
        data_out.write("ZZZZ");
    }
}
