#include "ProgramCounter.hpp"

void ProgramCounter::update_pc() {
    // 1. Reset Asincrono
    if (reset.read() == true) {
        current_value.write(0);
    } 
    // 2. Logica sincrona
    else if (clk.event() && clk.read() == true) {
        if (pc_load.read() == true) {
            // Cargar valor desde el bus (data_in)
            current_value.write(data_in.read().to_uint());
        } 
        else if (pc_inc.read() == true) {
            // Incrementar contador
            current_value.write(current_value.read() + 1);
        }
    }
}

void ProgramCounter::drive_bus() {
    // Logica Tri-State
    if (pc_out_enable.read() == true) {
        data_out.write(current_value.read());
    } else {
        data_out.write("ZZZZ");
    }
}
