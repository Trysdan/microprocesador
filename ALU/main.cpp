#include <systemc.h>
#include "ALU.hpp"
#include <iostream>

int sc_main(int argc, char* argv[]) {
    sc_signal<bool> a[4], b[4], opcode[4], result[4], cout;
    
    ALU ula("MIP_ALU");
    for(int i=0; i<4; i++) {
        ula.a[i](a[i]); ula.b[i](b[i]);
        ula.opcode[i](opcode[i]); ula.result[i](result[i]);
    }
    ula.cout(cout);

    auto set_inputs = [&](int va, int vb, int op) {
        for(int i=0; i<4; i++) {
            a[i].write((va >> i) & 1);
            b[i].write((vb >> i) & 1);
            opcode[i].write((op >> i) & 1);
        }
    };

    auto print_alu = [&](std::string msg) {
        sc_start(1, SC_NS);
        std::cout << msg << " -> Resultado: ";
        for(int i=3; i>=0; i--) std::cout << result[i].read();
        std::cout << " (Cout: " << cout.read() << ")" << std::endl;
    };

    std::cout << "--- INICIANDO PRUEBAS DE LA ALU ---" << std::endl;

    set_inputs(5, 3, 0);  print_alu("SUMA (5+3)   "); // Op 0: 1000 (8)
    set_inputs(10, 4, 1); print_alu("RESTA (10-4) "); // Op 1: 0110 (6)
    set_inputs(12, 10, 4);print_alu("AND (1100&1010)"); // Op 4: 1000 (8)
    set_inputs(7, 7, 10); print_alu("EQUAL (7==7) "); // Op 10: 0001 (True)
    set_inputs(5, 9, 11); print_alu("GREATER (5>9)"); // Op 11: 0000 (False)

    return 0;
}