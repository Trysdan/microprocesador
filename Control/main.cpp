#include <systemc.h>
#include "InstructionRegister.hpp"
#include <iostream>

int sc_main(int argc, char* argv[]) {
    // Señales de prueba
    sc_clock clk("clk", 10, SC_NS, 0.5);
    sc_signal<bool> reset, ir_load;
    sc_signal<sc_lv<8>> data_in;
    sc_signal<sc_uint<4>> opcode, operand;

    // Instancia del IR
    InstructionRegister ir("IR");
    ir.clk(clk);
    ir.reset(reset);
    ir.ir_load(ir_load);
    ir.data_in(data_in);
    ir.opcode(opcode);
    ir.operand(operand);

    // Trazado de ondas
    sc_trace_file *wf = sc_create_vcd_trace_file("ir_waves");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, ir_load, "ir_load");
    sc_trace(wf, data_in, "data_in");
    sc_trace(wf, opcode, "opcode");
    sc_trace(wf, operand, "operand");

    std::cout << "\n--- TESTBENCH REGISTRO DE INSTRUCCION (IR) ---\n";

    // Inicializacion
    reset.write(true);
    ir_load.write(false);
    data_in.write("00000000");
    sc_start(15, SC_NS); // Flanco de subida ocurre en 10ns
    reset.write(false);

    // Prueba 1: Cargar LDA 10 (0001 1010)
    // 0001 = LDA
    // 1010 = Direccion 10 (A en hexa)
    std::cout << "T=" << sc_time_stamp() << " | [Prueba 1] Cargando LDA 10 (0001 1010)\n";
    data_in.write("00011010");
    ir_load.write(true);
    sc_start(10, SC_NS); // Flanco de subida captura el dato
    ir_load.write(false);
    sc_start(5, SC_NS);
    std::cout << "T=" << sc_time_stamp() << " | OpCode: " << opcode.read() << " (Esperado: 1)\n";
    std::cout << "T=" << sc_time_stamp() << " | Operand: " << operand.read() << " (Esperado: 10)\n";

    // Prueba 2: Cargar ADD 5 (0010 0101)
    std::cout << "\nT=" << sc_time_stamp() << " | [Prueba 2] Cargando ADD 5 (0010 0101)\n";
    data_in.write("00100101");
    ir_load.write(true);
    sc_start(10, SC_NS);
    ir_load.write(false);
    sc_start(5, SC_NS);
    std::cout << "T=" << sc_time_stamp() << " | OpCode: " << opcode.read() << " (Esperado: 2)\n";
    std::cout << "T=" << sc_time_stamp() << " | Operand: " << operand.read() << " (Esperado: 5)\n";

    // Prueba 3: Verificar persistencia (apagar ir_load y cambiar data_in)
    std::cout << "\nT=" << sc_time_stamp() << " | [Prueba 3] Verificando persistencia (ir_load=0)\n";
    data_in.write("11111111"); // Intentar cargar HLT
    sc_start(10, SC_NS);
    std::cout << "T=" << sc_time_stamp() << " | OpCode: " << opcode.read() << " (Esperado: 2 - No cambio)\n";

    std::cout << "\nSimulacion de IR finalizada exitosamente.\n";
    sc_close_vcd_trace_file(wf);
    return 0;
}
