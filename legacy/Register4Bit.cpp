#include "Register4Bit.hpp"

void Register4Bit::write_data() {
    if (reset.read() == true) {
        internal_data.write(0);
    } else if (clk.event() && clk.read() == true) {
        if (load_enable.read() == true) {
            internal_data.write(data_in.read().range(3, 0).to_uint());
        }
    }
}

void Register4Bit::read_data() {
    if (out_enable.read() == true) {
        sc_lv<8> temp;
        sc_uint<4> val = internal_data.read();
        for (int i = 0; i < 4; i++) temp[i] = val[i] ? '1' : '0';
        for (int i = 4; i < 8; i++) temp[i] = '0';
        data_out.write(temp);
    } else {
        data_out.write("ZZZZZZZZ");
    }
}
