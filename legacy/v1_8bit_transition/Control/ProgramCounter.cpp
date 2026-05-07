#include "ProgramCounter.hpp"

void ProgramCounter::update_pc() {
    if (reset.read() == true) {
        current_value.write(0);
    } else if (clk.event() && clk.read() == true) {
        if (pc_load.read() == true) {
            current_value.write(data_in.read().range(3, 0).to_uint());
        } else if (pc_inc.read() == true) {
            current_value.write(current_value.read() + 1);
        }
    }
}

void ProgramCounter::drive_bus() {
    if (pc_out_enable.read() == true) {
        sc_lv<8> temp;
        sc_uint<4> val = current_value.read();
        for (int i = 0; i < 4; i++) temp[i] = val[i] ? '1' : '0';
        for (int i = 4; i < 8; i++) temp[i] = '0';
        data_out.write(temp);
    } else {
        data_out.write("ZZZZZZZZ");
    }
}
