#include <systemc.h>
#include "../Computer_Top.hpp"
#include <iostream>
#include <iomanip>

int sc_main(int argc, char* argv[]) {
    sc_clock clk("clk", 10, SC_NS, 0.5);
    sc_signal<bool> reset;

    Computer_Top computer("OutputComputer");
    computer.clk(clk);
    computer.reset(reset);

    std::cout << "\n=============================================\n";
    std::cout << "      TESTBENCH: OUTPUT REGISTER (WEEK 11)   \n";
    std::cout << "=============================================\n\n";

    // PROGRAMA "HELLO WORLD" (Contador Infinito - 2 Bytes):
    // ADDR 0: 0x01 (LDA)
    // ADDR 1: 14   (Operando) -> Carga valor inicial 0
    // ADDR 2: 0x02 (ADD)
    // ADDR 3: 15   (Operando) -> Suma constante 1
    // ADDR 4: 0x0E (OUT)    -> Enviar al Registro de Salida
    // ADDR 5: 0x03 (JMP)
    // ADDR 6: 2    (Operando) -> Saltar de regreso al ADD (Dirección 2)
    // ADDR 14: 0x00 (Valor inicial)
    // ADDR 15: 0x01 (Constante 1)
    
    computer.ram->mem[0].write(0x01);
    computer.ram->mem[1].write(14);
    computer.ram->mem[2].write(0x02);
    computer.ram->mem[3].write(15);
    computer.ram->mem[4].write(0x0E);
    computer.ram->mem[5].write(0x03);
    computer.ram->mem[6].write(2);
    computer.ram->mem[14].write(0x00);
    computer.ram->mem[15].write(0x01);

    reset.write(true);
    sc_start(15, SC_NS);
    reset.write(false);

    std::cout << " Tiempo |  Estado  | PC |  Bus  |  ACC  |  OUT \n";
    std::cout << "----------------------------------------------\n";

    // Ejecutamos suficientes ciclos para ver varios incrementos
    for (int i = 0; i < 200; i++) {
        sc_start(10, SC_NS);
        
        // El OutputRegister ya imprime por consola cuando cambia, 
        // pero aqui mostramos el estado general.
        if (computer.cu->current_state.read() == 0) { // Mostrar solo al inicio de ciclo T1
            std::cout << std::setw(6) << sc_time_stamp() << " | "
                      << "   T" << (computer.cu->current_state.read() + 1) << "    | "
                      << std::setw(2) << computer.pc->current_value.read() << " | "
                      << computer.central_bus.read() << " | "
                      << std::setw(3) << computer.regA->internal_data.read() << " | "
                      << computer.out_reg->internal_value.read() << "\n";
        }

        // Si el valor llega a 10, podemos detener la prueba para el reporte
        if (computer.out_reg->internal_value.read() == 10) {
            std::cout << "\n[INFO] El contador llego a 10. Prueba exitosa.\n";
            break;
        }
    }

    std::cout << "=============================================\n";
    return 0;
}
