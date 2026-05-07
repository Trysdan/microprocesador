#include <systemc.h>
#include "Computer_Top.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

// Funcion Loader: Carga un archivo .asm en la memoria RAM
void loadProgram(RAM256x8* ram, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] No se pudo abrir el archivo: " << filename << std::endl;
        return;
    }

    std::map<std::string, uint8_t> opcodes = {
        {"LDA", 0x1}, {"ADD", 0x2}, {"JMP", 0x3},
        {"JZ",  0x4}, {"SUB", 0x5}, {"AND", 0x6},
        {"OR",  0x7}, {"XOR", 0x8}, {"NOT", 0x9},
        {"EQL", 0xA}, {"GRT", 0xB}, {"IF",  0xC},
        {"STA", 0xD}, {"OUT", 0xE}, {"HLT", 0xF},
        {"LDR", 0x10}, {"STR", 0x18}, {"ADDR", 0x20}
    };

    // Mapa de registros para facilitar el parsing
    std::map<std::string, uint8_t> registers = {
        {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 3},
        {"R4", 4}, {"R5", 5}, {"R6", 6}, {"R7", 7}
    };



    std::string line;
    int address = 0;
    std::cout << "\n--- Cargando Programa: " << filename << " ---" << std::endl;

    while (std::getline(file, line)) {
        // Eliminar comentarios y espacios al inicio/final
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) line = line.substr(0, commentPos);
        
        // Limpiar espacios en blanco al inicio y final
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string word;
        ss >> word;

        // Caso 1: Cambio de direccion (@100)
        if (word[0] == '@') {
            try {
                address = std::stoi(word.substr(1));
                std::cout << "[LOADER] Saltando a Direccion: " << address << std::endl;
                continue;
            } catch (...) {
                std::cerr << "[ERROR] Direccion invalida: " << word << std::endl;
                continue;
            }
        }

        // Caso 2: Mnemónico
        if (opcodes.find(word) != opcodes.end()) {
            uint8_t op = opcodes[word];
            int operand = 0;
            
            // Manejo de instrucciones con registros (Fase 2)
            if (word == "LDR" || word == "STR" || word == "ADDR") {
                std::string regName;
                if (ss >> regName) {
                    // Limpiar coma y espacios
                    size_t commaPos = regName.find(',');
                    if (commaPos != std::string::npos) regName.erase(commaPos);
                    
                    if (registers.count(regName)) {
                        op += registers[regName];
                        std::cout << "   -> Rx Detectado: " << regName << " (Op Final: 0x" << std::hex << (int)op << std::dec << ")" << std::endl;
                    }
                }
            }

            // Para instrucciones que NO son de 1 byte (HLT, OUT, NOT, ADDR)
            if (word != "OUT" && word != "HLT" && word != "NOT" && word != "ADDR") {
                std::string operandStr;
                if (ss >> operandStr) {
                    // Si el operando tenia la coma pegada (ej: R1,100)
                    size_t commaPos = operandStr.find(',');
                    if (commaPos != std::string::npos) operandStr = operandStr.substr(commaPos + 1);
                    
                    try {
                        operand = std::stoi(operandStr);
                    } catch (...) { operand = 0; }
                }
            }

            ram->mem[address].write(op);
            std::cout << "ADDR " << std::setw(3) << address << ": " 
                      << word << " (0x" << std::hex << (int)op << std::dec << ")" << std::endl;
            
            if (word != "OUT" && word != "HLT" && word != "NOT" && word != "ADDR") {
                address++;
                ram->mem[address].write(operand & 0xFF);
                std::cout << "ADDR " << std::setw(3) << address << ":   -> Operando: " << operand << std::endl;
            }
            address++;
        } else {
            // Caso 3: Dato directo
            try {
                int value = std::stoi(word);
                ram->mem[address].write(value & 0xFF);
                std::cout << "ADDR " << std::setw(3) << address << ": DATA " << value << std::endl;
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
    // 1. Manejo de Argumentos de Consola
    std::string programFile = "programa.asm";
    if (argc > 1) {
        programFile = argv[1];
    } else {
        std::cout << "[INFO] No se especifico archivo. Usando defecto: " << programFile << std::endl;
        std::cout << "Uso: ./microprocessor_final <archivo.asm>\n" << std::endl;
    }

    sc_clock clk("clk", 10, SC_NS, 0.5);
    sc_signal<bool> reset;

    Computer_Top computer("MyComputer");
    computer.clk(clk);
    computer.reset(reset);

    // Carga del programa especificado
    loadProgram(computer.ram, programFile);

    reset.write(true);
    sc_start(15, SC_NS);
    reset.write(false);

    std::cout << "Iniciando ejecucion de: " << programFile << "\n";
    std::cout << "----------------------------------------------------------------------\n";
    std::cout << " Tiempo | Est | PC |   Bus    |  ACC  | RegB  | MAR | OUT \n";
    std::cout << "----------------------------------------------------------------------\n";

    for (int i = 0; i < 2000; i++) { 
        sc_start(10, SC_NS);
        
        std::cout << std::setw(6) << sc_time_stamp() << " | "
                  << " T" << (computer.cu->current_state.read() + 1) << " | "
                  << std::setw(2) << computer.pc->current_value.read() << " | "
                  << computer.central_bus.read() << " | "
                  << std::setw(5) << computer.regA->internal_data.read() << " | "
                  << std::setw(5) << computer.regB->internal_data.read() << " | "
                  << std::setw(3) << computer.mar->address.read() << " | "
                  << std::setw(3) << computer.out_reg->internal_value.read() << "\n";
        
        // Deteccion de HLT (OpCode 0xF) al final del ciclo T8
        if (computer.opcode_ir.read() == 0xF && computer.cu->current_state.read() == 7) {
            std::cout << "\n[INFO] HLT Detectado. Programa Finalizado con exito.\n";
            break;
        }
    }

    std::cout << "----------------------------------------------------------------------\n";
    std::cout << "RESULTADO FINAL (OUT REGISTER): " << computer.out_reg->internal_value.read() << "\n";
    std::cout << "======================================================================\n";

    return 0;
}
