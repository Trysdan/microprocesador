#include "OutputRegister.hpp"

void OutputRegister::process_load() {
    if (reset.read() == true) {
        internal_value.write(0);
    } else if (clk.event() && clk.read() == true) {
        if (out_load.read() == true) {
            internal_value.write(data_in.read().to_uint());
        }
    }
}

void OutputRegister::process_display() {
    std::cout << "\n[DISPLAY] >>> Valor en Registro de Salida: " 
              << internal_value.read() << " <<<\n" << std::endl;
}
