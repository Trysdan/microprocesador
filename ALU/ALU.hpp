#ifndef ALU_H
#define ALU_H

#include "AdderSub8Bit.hpp"
#include "ComparatorUnit8Bit.hpp"
#include "LogicUnit8Bit.hpp"
#include "Mux16to1.hpp"
#include <systemc.h>

SC_MODULE(ALU) {
  sc_in<bool> a[8], b[8], opcode[4];
  sc_out<bool> result[8];
  sc_out<bool> cout;
  sc_out<bool> zero; 
  
  sc_in<bool> alu_out_en;
  sc_out<sc_lv<8>> data_out;

  // Senales internas
  sc_signal<bool> s_arith[8], s_and[8], s_or[8], s_xor[8], s_not[8];
  sc_signal<bool> s_eq[8], s_gt[8], s_lt[8], s_ctrl_arith;
  sc_signal<bool> s_ground; 

  // Instancias
  AdderSub8Bit *arithUnit;
  LogicUnit8Bit *logicUnit;
  ComparatorUnit8Bit *compUnit;
  Mux16to1 *mux;

  void process_control();
  void drive_bus();
  void drive_zero(); 

  SC_CTOR(ALU) {
    arithUnit = new AdderSub8Bit("ArithUnit");
    logicUnit = new LogicUnit8Bit("LogicUnit");
    compUnit = new ComparatorUnit8Bit("CompUnit");
    mux = new Mux16to1("Mux");

    s_ground.write(false);

    // Conexion de entradas y salidas de sub-modulos
    for (int i = 0; i < 8; i++) {
      arithUnit->a[i](a[i]);
      arithUnit->b[i](b[i]);
      arithUnit->s[i](s_arith[i]);

      logicUnit->a[i](a[i]);
      logicUnit->b[i](b[i]);
      logicUnit->out_and[i](s_and[i]);
      logicUnit->out_or[i](s_or[i]);
      logicUnit->out_xor[i](s_xor[i]);
      logicUnit->out_not_a[i](s_not[i]);

      compUnit->a[i](a[i]);
      compUnit->b[i](b[i]);
      compUnit->out_equal[i](s_eq[i]);
      compUnit->out_greater[i](s_gt[i]);
      compUnit->out_less[i](s_lt[i]);

      mux->out[i](result[i]);
      
      // Mapeo del Mux
      mux->in[2][i](s_arith[i]); // ADD
      mux->in[5][i](s_arith[i]); // SUB
      mux->in[6][i](s_and[i]);   // AND
      mux->in[7][i](s_or[i]);    // OR
      mux->in[8][i](s_xor[i]);   // XOR
      mux->in[9][i](s_not[i]);   // NOT
      mux->in[10][i](s_eq[i]);   // EQL
      mux->in[11][i](s_gt[i]);   // GRT
      mux->in[12][i](s_lt[i]);   // LSS

      mux->in[0][i](s_ground);
      mux->in[1][i](s_ground);
      mux->in[3][i](s_ground);
      mux->in[4][i](s_ground);
      mux->in[13][i](s_ground);
      mux->in[14][i](s_ground);
      mux->in[15][i](s_ground);
    }
    
    arithUnit->ctrl(s_ctrl_arith);
    arithUnit->cout(cout);

    for (int i = 0; i < 4; i++) {
      mux->sel[i](opcode[i]);
    }

    SC_METHOD(process_control);
    for (int i = 0; i < 4; i++) sensitive << opcode[i];

    SC_METHOD(drive_bus);
    sensitive << alu_out_en;
    for (int i = 0; i < 8; i++) sensitive << result[i];

    SC_METHOD(drive_zero);
    for (int i = 0; i < 8; i++) sensitive << result[i];
  }
};

#endif