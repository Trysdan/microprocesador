#include "ComparatorUnit8Bit.hpp"

void ComparatorUnit8Bit::compute() {
    uint8_t val_a = 0;
    uint8_t val_b = 0;
    
    for (int i = 0; i < 8; i++) {
        if (a[i].read()) val_a |= (1 << i);
        if (b[i].read()) val_b |= (1 << i);
    }

    bool eq = (val_a == val_b);
    bool gt = (val_a > val_b);
    bool lt = (val_a < val_b);

    for (int i = 0; i < 8; i++) {
        out_equal[i].write(eq);
        out_greater[i].write(gt);
        out_less[i].write(lt);
    }
}
