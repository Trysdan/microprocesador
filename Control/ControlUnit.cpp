#include "ControlUnit.hpp"

void ControlUnit::process_state_transition() {
    if (reset.read() == true) {
        current_state.write(T1);
    } else {
        switch (current_state.read()) {
            case T1: current_state.write(T2); break;
            case T2: current_state.write(T3); break;
            case T3: current_state.write(T4); break;
            case T4: current_state.write(T5); break;
            case T5: current_state.write(T6); break;
            case T6: current_state.write(T7); break;
            case T7: current_state.write(T8); break;
            case T8: current_state.write(T1); break;
        }
    }
}

void ControlUnit::process_output_logic() {
    // Reset combinacional
    pc_inc.write(false);
    pc_out.write(false);
    pc_load.write(false);
    mar_load.write(false);
    ir_load.write(false);
    ram_out.write(false);
    ram_write.write(false);
    acc_load.write(false);
    acc_out.write(false);
    regB_load.write(false);
    alu_out.write(false);
    out_load.write(false);

    State state = current_state.read();
    
    // El OpCode ahora es de 8 bits, pero seguimos usando los valores definidos para mantener compatibilidad
    sc_uint<8> op_full = opcode.read();
    sc_uint<4> op = op_full.range(3, 0); // O podemos evaluar el byte completo, pero nuestro ISA original usa 0x1, 0x2, etc.
    // Usaremos el byte completo para evaluar
    uint8_t op_val = op_full.to_uint();

    switch (state) {
        case T1: // Fetch OpCode 1: PC a MAR
            pc_out.write(true);
            mar_load.write(true);
            break;

        case T2: // Fetch OpCode 2: PC++
            pc_inc.write(true);
            break;

        case T3: // Fetch OpCode 3: RAM[MAR] a IR (Guardamos OpCode)
            ram_out.write(true);
            ir_load.write(true);
            break;

        case T4: // Fetch Operando 1
            if (op_val == 0x01 || op_val == 0x02 || op_val == 0x05 || op_val == 0x06 || op_val == 0x07 || op_val == 0x08 || op_val == 0x0A || op_val == 0x0B || op_val == 0x03 || op_val == 0x04 || op_val == 0x0C || op_val == 0x0D) { 
                // Instrucciones que requieren operando de 8 bits (Direccion)
                pc_out.write(true);
                mar_load.write(true);
            } else if (op_val == 0x0E || op_val == 0x0F || op_val == 0x09) {
                // OUT, HLT, NOT (Instrucciones de 1 Byte, no requieren operando)
                // Se ejecutan en ciclos tempranos para no esperar a T8.
                // Podriamos atajar la ejecucion aqui, pero lo mantendremos alineado.
                if (op_val == 0x09) { // NOT (Acc = ~Acc)
                    alu_out.write(true);
                    acc_load.write(true);
                } else if (op_val == 0x0E) { // OUT
                    acc_out.write(true);
                    out_load.write(true);
                }
            }
            break;

        case T5: // Fetch Operando 2
            if (op_val == 0x01 || op_val == 0x02 || op_val == 0x05 || op_val == 0x06 || op_val == 0x07 || op_val == 0x08 || op_val == 0x0A || op_val == 0x0B || op_val == 0x03 || op_val == 0x04 || op_val == 0x0C || op_val == 0x0D) { 
                pc_inc.write(true); // PC apunta a la siguiente instruccion despues del operando
            }
            break;

        case T6: // Fetch Operando 3: Direccionamiento / Carga de PC
            if (op_val == 0x03) { // JMP incondicional
                ram_out.write(true);
                pc_load.write(true);
            } else if (op_val == 0x04) { // JZ (Jump if Zero)
                if (acc_val.read() == 0) {
                    ram_out.write(true);
                    pc_load.write(true);
                }
            } else if (op_val == 0x0C) { // IF (Jump if NOT Zero)
                if (acc_val.read() != 0) {
                    ram_out.write(true);
                    pc_load.write(true);
                }
            } else if (op_val == 0x01 || op_val == 0x02 || op_val == 0x05 || op_val == 0x06 || op_val == 0x07 || op_val == 0x08 || op_val == 0x0A || op_val == 0x0B || op_val == 0x0D) {
                // LDA, ADD, SUB, Lógicas, STA... El operando es una dirección de RAM donde esta el dato real
                // Por lo tanto, movemos el operando (dirección) de la RAM al MAR
                ram_out.write(true);
                mar_load.write(true);
            }
            break;

        case T7: // Execute 1: Leer dato de RAM
            if (op_val == 0x01) { // LDA
                ram_out.write(true);
                acc_load.write(true);
            } else if (op_val == 0x02 || op_val == 0x05 || op_val == 0x06 || op_val == 0x07 || op_val == 0x08 || op_val == 0x0A || op_val == 0x0B) { 
                // Operaciones con ALU requieren el dato en RegB
                ram_out.write(true);
                regB_load.write(true);
            } else if (op_val == 0x0D) { // STA
                acc_out.write(true);
                ram_write.write(true);
            }
            break;

        case T8: // Execute 2: Operacion ALU
            if (op_val == 0x02 || op_val == 0x05 || op_val == 0x06 || op_val == 0x07 || op_val == 0x08 || op_val == 0x0A || op_val == 0x0B) { 
                alu_out.write(true);
                acc_load.write(true);
            }
            break;
    }
}
