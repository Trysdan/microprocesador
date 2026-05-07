#ifndef HAZARD_DETECTION_UNIT_HPP
#define HAZARD_DETECTION_UNIT_HPP

#include <systemc.h>

// =====================================================
//  HazardDetectionUnit — Detecta Load-Use Hazard
//
//  El único hazard que el forwarding NO puede resolver:
//    LOAD R1, addr          <- necesita ir a MEM antes de tener el dato
//    ADD  R2, R1, R3        <- necesita R1 en EX, pero aún está en MEM
//
//  Solución: insertar 1 burbuja (stall 1 ciclo):
//    - pc_write = 0     (congela el PC)
//    - if_id_write = 0  (congela IF/ID)
//    - id_ex flush = 1  (inserta NOP en ID/EX)
// =====================================================
SC_MODULE(HazardDetectionUnit) {
    // Instrucción en ID/EX (la que acaba de hacer LOAD)
    sc_in<bool>       id_ex_mem_read;  // Es LOAD?
    sc_in<sc_uint<3>> id_ex_rd;        // Registro destino del LOAD

    // Instrucción en IF/ID (la que necesita el dato)
    sc_in<sc_uint<3>> if_id_rs1;       // Rs1 de la instrucción siguiente
    sc_in<sc_uint<3>> if_id_rs2;       // Rs2 de la instrucción siguiente

    // Señales de control de hazard
    sc_out<bool> stall;        // 1 → congelar PC e IF/ID
    sc_out<bool> id_ex_flush;  // 1 → insertar burbuja en ID/EX

    void detect();

    SC_CTOR(HazardDetectionUnit) {
        SC_METHOD(detect);
        sensitive << id_ex_mem_read << id_ex_rd
                  << if_id_rs1 << if_id_rs2;
    }
};

#endif // HAZARD_DETECTION_UNIT_HPP
