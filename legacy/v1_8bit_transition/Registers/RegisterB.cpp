#include "RegisterB.hpp"

void RegisterB::process_load() {
    if (reset.read() == true) {
        internal_data.write(0);
    } else if (clk.event() && clk.read() == true) {
        if (load_enable.read() == true) {
            internal_data.write(data_in.read().range(3, 0).to_uint());
        }
    }
}

void RegisterB::drive_output() {
    data_out.write(internal_data.read());
}
