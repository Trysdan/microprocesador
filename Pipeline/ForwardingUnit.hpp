#ifndef FORWARDING_UNIT_HPP
#define FORWARDING_UNIT_HPP

#include <systemc.h>

// =====================================================
//  ForwardingUnit — Resuelve RAW Hazards de Datos
//
//  Detecta dependencias entre instrucciones en vuelo
//  y genera selecciones para los MUXes de forwarding
//  antes de la ALU en la etapa EX.
//
//  forward_a / forward_b:
//    0b00 (0) → Leer de RegisterFile (sin hazard)
//    0b01 (1) → Forward desde MEM/WB (resultado de hace 2 ciclos)
//    0b10 (2) → Forward desde EX/MEM (resultado de hace 1 ciclo)
// =====================================================
SC_MODULE(ForwardingUnit) {
    // Registros fuente de la instrucción en EX
    sc_in<sc_uint<3>> id_ex_rs1;
    sc_in<sc_uint<3>> id_ex_rs2;

    // Destino de la instrucción en MEM
    sc_in<sc_uint<3>> ex_mem_rd;
    sc_in<bool>       ex_mem_reg_write;

    // Destino de la instrucción en WB
    sc_in<sc_uint<3>> mem_wb_rd;
    sc_in<bool>       mem_wb_reg_write;

    // Selección de MUX (2 bits cada una)
    sc_out<sc_uint<2>> forward_a;
    sc_out<sc_uint<2>> forward_b;

    void compute();

    SC_CTOR(ForwardingUnit) {
        SC_METHOD(compute);
        sensitive << id_ex_rs1 << id_ex_rs2
                  << ex_mem_rd << ex_mem_reg_write
                  << mem_wb_rd << mem_wb_reg_write;
    }
};

#endif // FORWARDING_UNIT_HPP
