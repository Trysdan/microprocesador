#ifndef ALU_H
#define ALU_H

#include "AdderSub4Bit.hpp"
#include "ComparatorUnit4Bit.hpp"
#include "LogicUnit4Bit.hpp"
#include "Mux16to1.hpp"
#include <systemc.h>

SC_MODULE(ALU) {
  sc_in<bool> a[4], b[4], opcode[4];
  sc_out<bool> result[4];
  sc_out<bool> cout;
  
  // Puertos para Integracion de Bus
  sc_in<bool> alu_out_en;
  sc_out<sc_lv<8>> data_out;

  // Senales internas (los cables)
  sc_signal<bool> s_arith[4], s_and[4], s_or[4], s_xor[4], s_not[4];
  sc_signal<bool> s_eq[4], s_gt[4], s_lt[4], s_ctrl_arith;
  sc_signal<bool> s_ground; // Para entradas no usadas del Mux

  // Instancias (punteros a objetos, pero puertos conectados por senales)
  AdderSub4Bit *arithUnit;
  LogicUnit4Bit *logicUnit;
  ComparatorUnit4Bit *compUnit;
  Mux16to1 *mux;

  void process_control();
  void drive_bus();

  SC_CTOR(ALU) {
    arithUnit = new AdderSub4Bit("ArithUnit");
    logicUnit = new LogicUnit4Bit("LogicUnit");
    compUnit = new ComparatorUnit4Bit("CompUnit");
    mux = new Mux16to1("Mux");

    s_ground.write(false);

    // Conexion de entradas
    for (int i = 0; i < 4; i++) {
      arithUnit->a[i](a[i]);
      arithUnit->b[i](b[i]);
      logicUnit->a[i](a[i]);
      logicUnit->b[i](b[i]);
      compUnit->a[i](a[i]);
      compUnit->b[i](b[i]);

      arithUnit->s[i](s_arith[i]);
      logicUnit->out_and[i](s_and[i]);
      logicUnit->out_or[i](s_or[i]);
      logicUnit->out_xor[i](s_xor[i]);
      logicUnit->out_not_a[i](s_not[i]);
      compUnit->out_equal[i](s_eq[i]);
      compUnit->out_greater[i](s_gt[i]);
      compUnit->out_less[i](s_lt[i]);
    }
    arithUnit->ctrl(s_ctrl_arith);
    arithUnit->cout(cout);

    // Mapeo del Mux (Soldando cables)
    for (int i = 0; i < 4; i++) {
      mux->sel[i](opcode[i]);
      mux->out[i](result[i]);

      mux->in[0][i](s_arith[i]); // Suma
      mux->in[1][i](s_arith[i]); // Resta
      mux->in[4][i](s_and[i]);
      mux->in[5][i](s_or[i]);
      mux->in[6][i](s_xor[i]);
      mux->in[7][i](s_not[i]);
      mux->in[10][i](s_eq[i]);
      mux->in[11][i](s_gt[i]);
      mux->in[12][i](s_lt[i]);

      // IMPORTANTE: Conectar entradas no usadas para evitar warnings
      mux->in[2][i](s_ground);
      mux->in[3][i](s_ground);
      mux->in[8][i](s_ground);
      mux->in[9][i](s_ground);
      mux->in[13][i](s_ground);
      mux->in[14][i](s_ground);
      mux->in[15][i](s_ground);
    }

    SC_METHOD(process_control);
    for (int i = 0; i < 4; i++)
      sensitive << opcode[i];

    SC_METHOD(drive_bus);
    sensitive << alu_out_en;
    for (int i = 0; i < 4; i++) sensitive << result[i];
  }
};

#endif