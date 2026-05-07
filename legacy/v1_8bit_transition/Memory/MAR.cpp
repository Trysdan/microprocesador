#include "MAR.hpp"

void MAR::process_mar() {
    if (reset.read() == true) {
        address.write(0);
    } else if (clk.event() && clk.read() == true) {
        if (mar_load.read() == true) {
            address.write(data_in.read().range(3, 0).to_uint());
        }
    }
}
