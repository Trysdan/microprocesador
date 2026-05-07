#include "RegisterFile.hpp"

void RegisterFile::process_write() {
    if (reset.read()) {
        for (int i = 0; i < 8; i++) {
            regs[i].write(0);
        }
    } else if (reg_write.read()) {
        int idx = reg_sel.read().to_uint();
        regs[idx].write(bus_data.read());
    }
}

void RegisterFile::process_read() {
    if (reg_out.read()) {
        int idx = reg_sel.read().to_uint();
        bus_data.write(regs[idx].read());
    } else {
        bus_data.write("ZZZZZZZZ");
    }
}
