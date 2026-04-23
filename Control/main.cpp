#include <systemc.h>
#include "InstructionRegister.hpp"
#include <iostream>

int sc_main(int argc, char* argv[]) {
    // Reloj
    sc_clock clk("clk", 10, SC_NS, 0.5);

    // Senales de prueba
    sc_signal<bool> reset;
    sc_signal<bool> ir_load;
    sc_signal<sc_lv<8>> data_in;
    sc_signal<sc_uint<4>> opcode;
    sc_signal<sc_uint<4>> operand;
    sc_signal<bool> ir_out_dummy;
    sc_signal<sc_lv<8>> bus_out_dummy;

    // Instancia del IR
    InstructionRegister ir("IR");
    ir.clk(clk);
    ir.reset(reset);
    ir.ir_load(ir_load);
    ir.data_in(data_in);
    ir.opcode(opcode);
    ir.operand(operand);
    ir.ir_out_en(ir_out_dummy);
    ir.bus_out(bus_out_dummy);

    // Trazado de ondas
    sc_trace_file *wf = sc_create_vcd_trace_file("ir_waves");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, ir_load, "ir_load");
    sc_trace(wf, data_in, "data_in");
    sc_trace(wf, opcode, "opcode");
    sc_trace(wf, operand, "operand");

    std::cout << "\n--- TESTBENCH REGISTRO DE INSTRUCCION (IR) ---\n";

    // 1. Reset
    reset.write(true);
    ir_load.write(false);
    data_in.write("00000000");
    sc_start(15, SC_NS);
    reset.write(false);

    // 2. Cargar Instruccion LDA (0001) con Operando 10 (1010)
    // Instruccion completa: 0001 1010
    std::cout << "Cargando LDA 10 (0001 1010)...\n";
    data_in.write("00011010");
    ir_load.write(true);
    sc_start(10, SC_NS);
    ir_load.write(false);
    
    std::cout << "OpCode decodificado: " << opcode.read() << " (Esperado 1)\n";
    std::cout << "Operando decodificado: " << operand.read() << " (Esperado 10)\n";

    // 3. Cargar Instruccion ADD (0010) con Operando 15 (1111)
    std::cout << "\nCargando ADD 15 (0010 1111)...\n";
    data_in.write("00101111");
    ir_load.write(true);
    sc_start(10, SC_NS);
    ir_load.write(false);

    std::cout << "OpCode decodificado: " << opcode.read() << " (Esperado 2)\n";
    std::cout << "Operando decodificado: " << operand.read() << " (Esperado 15)\n";

    std::cout << "\nSimulacion de IR finalizada. Revisa ir_waves.vcd\n";
    sc_close_vcd_trace_file(wf);
    return 0;
}
