#include "RAM16x8.hpp"

void RAM16x8::write_data() {
    if (write_enable.read() == true) {
        mem[address.read()].write(data_in.read().to_uint());
    }
}

void RAM16x8::read_data() {
    if (out_enable.read() == true) {
        data_out.write(mem[address.read()].read());
    } else {
        data_out.write("ZZZZZZZZ");
    }
}
