#include <systemc.h>
#include "Computer_Top.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

// Funcion Loader: Carga un archivo .asm en la memoria RAM
void loadProgram(RAM16x8* ram, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] No se pudo abrir el archivo: " << filename << std::endl;
        return;
    }

    std::map<std::string, uint8_t> opcodes = {
        {"LDA", 0x1}, {"ADD", 0x2}, {"JMP", 0x3},
        {"JZ",  0x4}, {"SUB", 0x5}, {"AND", 0x6},
        {"OR",  0x7}, {"XOR", 0x8}, {"NOT", 0x9},
        {"EQL", 0xA}, {"GRT", 0xB}, {"OUT", 0xE}, {"HLT", 0xF}
    };


    std::string line;
    int address = 0;
    std::cout << "\n--- Cargando Programa: " << filename << " ---" << std::endl;

    while (std::getline(file, line) && address < 16) {
        if (line.empty() || line[0] == ';') continue; // Saltar lineas vacias o comentarios

        std::stringstream ss(line);
        std::string mnemonic;
        int operand = 0;

        ss >> mnemonic;
        if (opcodes.find(mnemonic) != opcodes.end()) {
            uint8_t op = opcodes[mnemonic];
            if (mnemonic != "OUT" && mnemonic != "HLT" && mnemonic != "NOT") {
                ss >> operand;
            }

            
            uint8_t instruction = (op << 4) | (operand & 0x0F);
            ram->mem[address].write(instruction);
            
            std::cout << "ADDR " << std::setw(2) << address << ": " 
                      << mnemonic << " " << operand 
                      << " (0x" << std::hex << (int)instruction << std::dec << ")" << std::endl;
            address++;
        } else {
            // Si no es un mnemónico, podria ser un dato directo (ej: para ADDR 14)
            try {
                int value = std::stoi(mnemonic);
                ram->mem[address].write(value & 0xFF);
                std::cout << "ADDR " << std::setw(2) << address << ": DATA " << value << std::endl;
                address++;
            } catch (...) {
                std::cerr << "[WARN] Linea ignorada: " << line << std::endl;
            }
        }
    }
    std::cout << "--- Carga Finalizada ---\n" << std::endl;
    file.close();
}

int sc_main(int argc, char* argv[]) {
    // Reloj Global
    sc_clock clk("clk", 10, SC_NS, 0.5);
    sc_signal<bool> reset;

    // Instancia del Sistema Completo
    Computer_Top computer("MyComputer");
    computer.clk(clk);
    computer.reset(reset);

    // Trazado de Ondas
    sc_trace_file *wf = sc_create_vcd_trace_file("microprocessor_loader");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, computer.central_bus, "BUS");
    sc_trace(wf, computer.cu->current_state, "STATE");
    sc_trace(wf, computer.pc->current_value, "PC");
    sc_trace(wf, computer.regA->internal_data, "ACC");

    // CARGA DEL PROGRAMA USANDO EL LOADER
    // El archivo programa.asm debe existir en el directorio de ejecucion
    loadProgram(computer.ram, "programa.asm");

    // Inicializacion
    reset.write(true);
    sc_start(15, SC_NS);
    reset.write(false);

    std::cout << "Iniciando ejecucion...\n";
    std::cout << "---------------------------------------------\n";
    std::cout << " Tiempo |  Estado  | PC |  Bus  |  ACC  | MAR \n";
    std::cout << "---------------------------------------------\n";

    for (int i = 0; i < 100; i++) {
        sc_start(10, SC_NS);
        
        std::cout << std::setw(6) << sc_time_stamp() << " | "
                  << "   T" << (computer.cu->current_state.read() + 1) << "    | "
                  << std::setw(2) << computer.pc->current_value.read() << " | "
                  << computer.central_bus.read() << " | "
                  << std::setw(3) << computer.regA->internal_data.read() << " | "
                  << std::setw(2) << computer.ram_addr.read() << "\n";
        
        if (computer.opcode_ir.read() == 0xF && computer.cu->current_state.read() == 3) {
            std::cout << "\n[INFO] HLT Detectado. Programa Finalizado.\n";
            break;
        }
    }

    std::cout << "---------------------------------------------\n";
    std::cout << "RESULTADO FINAL EN ACUMULADOR: " << computer.regA->internal_data.read() << "\n";
    std::cout << "=============================================\n";

    sc_close_vcd_trace_file(wf);
    return 0;
}
