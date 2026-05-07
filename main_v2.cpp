#include <systemc.h>
#include "Computer_Top_v2.hpp"
#include "Pipeline/PipelineTypes.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <thread>
#include <atomic>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <termios.h>
#include <unistd.h>

// Declarada en Computer_Top_v2.cpp
extern std::atomic<uint8_t> g_current_key;

// =====================================================
//  Loader: Codifica ASM de 16 bits y carga en DualPortRAM
//
//  Formato por instrucción:
//    Tipo-R: MNEMO Rd, Rs1, Rs2  → [op|Rd|Rs1|Rs2|000]
//    Tipo-I: MNEMO Rd, imm8      → [op|Rd|imm8|0]
//    OUT   : OUT  Rd             → [0xE|Rd|000|000|000]
//    HLT   :                     → [0xF|000|000|000|000]
// =====================================================
static const std::map<std::string, uint8_t> OPCODES = {
    {"STORI",0x0},{"LOAD",0x1},{"STORE",0x2},{"MOV",0x3},{"MVI",0x4},
    {"ADD", 0x5},{"SUB", 0x6},{"AND",0x7},{"OR", 0x8},
    {"XOR", 0x9},{"LOADI",0xA},{"JMP",0xB},{"BEZ",0xC},
    {"BNZ", 0xD},{"OUT", 0xE},{"HLT",0xF}
};
static const std::map<std::string, uint8_t> REGS = {
    {"R0",0},{"R1",1},{"R2",2},{"R3",3},
    {"R4",4},{"R5",5},{"R6",6},{"R7",7}
};

static const std::map<std::string, bool> IS_TYPE_R = {
    {"STORI",true},{"MOV",true},{"ADD",true},{"SUB",true},{"AND",true},
    {"OR",true},{"XOR",true},{"LOADI",true},{"OUT",true}
};
// Instrucciones sin operandos
static const std::map<std::string, bool> IS_NOARG = {
    {"HLT",true}
};
// Instrucciones de salto incondicionado (solo imm8, sin registro)
static const std::map<std::string, bool> IS_JUMP_ONLY = {
    {"JMP",true}
};

std::string trim(std::string s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    size_t b = s.find_last_not_of(" \t\r\n");
    return (a == std::string::npos) ? "" : s.substr(a, b - a + 1);
}

uint8_t parseReg(const std::string& s) {
    auto it = REGS.find(s);
    return (it != REGS.end()) ? it->second : 0;
}

void loadProgram(DualPortRAM* ram, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] No se pudo abrir: " << filename << "\n";
        return;
    }

    std::vector<std::string> lines;
    std::string raw_line;
    while (std::getline(file, raw_line)) {
        auto cp = raw_line.find(';');
        if (cp != std::string::npos) raw_line = raw_line.substr(0, cp);
        raw_line = trim(raw_line);
        if (!raw_line.empty()) lines.push_back(raw_line);
    }

    // Pass 1: Collect Labels and determine addresses
    std::map<std::string, uint8_t> labels;
    uint8_t addr_ptr = 0;
    for (auto& line : lines) {
        if (line[0] == '@') {
            addr_ptr = std::stoi(line.substr(1));
            continue;
        }
        if (line.back() == ':') {
            labels[line.substr(0, line.size()-1)] = addr_ptr;
            continue;
        }
        addr_ptr += 2;
    }

    // Pass 2: Encode and Load
    addr_ptr = 0;
    std::cout << "\n--- Cargando (16-bit ISA): " << filename << " ---\n";
    for (auto& line : lines) {
        if (line[0] == '@') {
            addr_ptr = std::stoi(line.substr(1));
            continue;
        }
        if (line.back() == ':') continue;

        std::istringstream ss(line);
        std::string mnem;
        ss >> mnem;

        auto oit = OPCODES.find(mnem);
        if (oit == OPCODES.end()) {
            std::cerr << "[WARN] Mnemónico desconocido: " << mnem << "\n";
            continue;
        }
        uint8_t op = oit->second;

        std::string rest;
        std::getline(ss, rest);
        for (char& c : rest) if (c == ',') c = ' ';
        std::istringstream ops(rest);
        std::vector<std::string> tokens;
        std::string tok;
        while (ops >> tok) tokens.push_back(tok);

        uint16_t instr = 0;
        instr |= (uint16_t)(op & 0xF) << 12;

        if (IS_NOARG.count(mnem)) {
            // HLT
        } else if (IS_JUMP_ONLY.count(mnem)) {
            // JMP label/imm8
            int imm = 0;
            if (labels.count(tokens[0])) imm = labels[tokens[0]];
            else imm = std::stoi(tokens[0]);
            instr |= (uint16_t)(imm & 0xFF) << 1;
        } else if (IS_TYPE_R.count(mnem)) {
            uint8_t rd, rs1, rs2;
            if (mnem == "LOADI") {
                rd  = tokens.size() > 0 ? parseReg(tokens[0]) : 0;
                rs1 = tokens.size() > 1 ? parseReg(tokens[1]) : 0;
                rs2 = 0;
            } else if (mnem == "STORI") {
                rs1 = tokens.size() > 0 ? parseReg(tokens[0]) : 0;
                rs2 = tokens.size() > 1 ? parseReg(tokens[1]) : 0;
                rd  = 0;
            } else if (mnem == "OUT") {
                rd  = 0;
                rs1 = tokens.size() > 0 ? parseReg(tokens[0]) : 0;
                rs2 = 0;
            } else {
                rd  = tokens.size() > 0 ? parseReg(tokens[0]) : 0;
                rs1 = tokens.size() > 1 ? parseReg(tokens[1]) : rd;
                rs2 = tokens.size() > 2 ? parseReg(tokens[2]) : 0;
            }
            instr |= (uint16_t)(rd  & 0x7) << 9;
            instr |= (uint16_t)(rs1 & 0x7) << 6;
            instr |= (uint16_t)(rs2 & 0x7) << 3;
        } else {
            // Tipo-I: Rd imm8/label
            uint8_t rd  = tokens.size() > 0 ? parseReg(tokens[0]) : 0;
            int imm = 0;
            if (tokens.size() > 1) {
                if (labels.count(tokens[1])) imm = labels[tokens[1]];
                else imm = std::stoi(tokens[1]);
            }
            instr |= (uint16_t)(rd       & 0x7) << 9;
            instr |= (uint16_t)(imm      & 0xFF) << 1;
        }

        uint8_t hi = (instr >> 8) & 0xFF;
        uint8_t lo =  instr       & 0xFF;
        ram->mem[addr_ptr  ].write(hi);
        ram->mem[addr_ptr+1].write(lo);

        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)addr_ptr
                  << ": " << std::setw(4) << instr << std::dec
                  << "  (" << mnem << ")\n";
        addr_ptr += 2;
    }
    std::cout << "--- Carga Finalizada ---\n\n";
}

// =====================================================
//  Keyboard Thread: Lectura no-bloqueante
// =====================================
void kb_thread_func(std::atomic<bool>& running) {
    try {
        struct termios oldt, newt;
        if (tcgetattr(STDIN_FILENO, &oldt) != 0) return;
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

        while (running) {
            char ch;
            if (read(STDIN_FILENO, &ch, 1) > 0) {
                g_current_key.store((uint8_t)ch);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }

        fcntl(STDIN_FILENO, F_SETFL, flags);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    } catch (...) {
        // Silently exit
    }
}

// =====================================================
//  sc_main — Testbench Principal
// =====================================================
int sc_main(int argc, char* argv[]) {
    std::string progFile = "test_pipeline.asm";
    if (argc > 1) progFile = argv[1];

    sc_clock clk("clk", 10, SC_NS, 0.5);
    sc_signal<bool> reset;

    Computer_Top_v2 cpu("CPU");
    cpu.clk(clk);
    cpu.reset(reset);

    // Cargar programa antes de la simulación
    loadProgram(cpu.ram, progFile);

    // Reset inicial
    reset.write(true);
    sc_start(15, SC_NS);
    reset.write(false);

    // Iniciar thread de teclado
    std::atomic<bool> kb_running(true);
    std::thread kb_thread(kb_thread_func, std::ref(kb_running));

    bool is_snake = (progFile.find("snake.asm") != std::string::npos);

    // Cabecera de la tabla (solo si no es snake)
    if (!is_snake) {
        std::cout << "\n" << std::string(110, '=') << "\n";
        std::printf("%-10s | %-4s | %-8s | %-12s | %-8s | %-8s | %-6s | %-4s\n", 
                    "Tiempo", "PC", "IF(Hex)", "ID/EX", "EX_Res", "MEM_D", "WB_Rd", "OUT");
        std::cout << std::string(110, '-') << "\n";
    }

    bool halted = false;
    for (int cycle = 0; cycle < 100000000; cycle++) {
        sc_start(10, SC_NS);
        
        // Ralentizar un poco para que sea jugable (aprox 100kHz efectivos)
        if (cycle % 100 == 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }

        uint16_t instr = cpu.s_ifid_instr.read().to_uint();
        uint8_t  op    = cpu.s_idex_alu_op.read().to_uint();
        uint8_t  rd    = cpu.s_idex_rd.read().to_uint();
        
        if (!is_snake) {
            std::printf("%-10s | 0x%02X | 0x%04X   | Op:%-2d Rd:%-2d | %-8d | %-8d | R%-5d | %-4d\n",
                        sc_time_stamp().to_string().c_str(),
                        cpu.s_pc.read().to_uint(),
                        instr,
                        op, rd,
                        cpu.s_exmem_alu_result.read().to_int(),
                        cpu.s_memwb_mem_data.read().to_int(),
                        cpu.s_memwb_rd.read().to_uint(),
                        cpu.output_reg.read().to_uint());
        }

        if (cpu.halt_flag.read()) {
            halted = true;
            break;
        }

    }
    std::cout << std::string(110, '=') << "\n";

    if (halted) {
        std::cout << "\n[HLT] Ejecución detenida por software.\n";
    }

    std::cout << "\n--- Estado Final del RegisterFile ---\n";
    for (int r = 0; r < 8; r++) {
        std::printf("  R%d = %3d\n", r, cpu.rf->regs[r].read().to_uint());
    }
    std::cout << "-------------------------------------\n";
    std::printf("RESULTADO FINAL (OUT): %d\n\n", cpu.output_reg.read().to_uint());

    // Detener thread de teclado
    kb_running = false;
    if (kb_thread.joinable()) kb_thread.join();

    return 0;
}
