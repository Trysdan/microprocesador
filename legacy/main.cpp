#include <systemc.h>
#include "Adder4Bit.hpp"
#include "Substractor4Bit.hpp"

int sc_main(int argc, char* argv[]) {
    // Señales para el Sumador
    sc_signal<bool> as[4], bs[4], cin_s, sum[4], cout_s;
    // Señales para el Restador
    sc_signal<bool> ar[4], br[4], res[4], bout_r;

    // Instancias
    Adder4Bit adder("Sumador4Bits");
    Substractor4Bit sub("Restador4Bits");

    // Mapeo de puertos: Sumador
    for (int i = 0; i < 4; i++) {
        adder.a[i](as[i]);
        adder.b[i](bs[i]);
        adder.s[i](sum[i]);
    }
    adder.cin(cin_s);
    adder.cout(cout_s);

    // Mapeo de puertos: Restador
    for (int i = 0; i < 4; i++) {
        sub.a[i](ar[i]);
        sub.b[i](br[i]);
        sub.s[i](res[i]);
    }
    sub.bout(bout_r);

    // --- CASO 1: SUMA (9 + 3) ---
    cin_s = 0;
    as[3]=1; as[2]=0; as[1]=0; as[0]=1; // 9
    bs[3]=0; bs[2]=0; bs[1]=1; bs[0]=1; // 3

    // --- CASO 2: RESTA (10 - 4) ---
    ar[3]=0; ar[2]=0; ar[1]=1; ar[0]=0; // 10
    br[3]=0; br[2]=1; br[1]=0; br[0]=0; // 4

    // Ejecutar simulación
    sc_start(1, SC_NS);

    // Impresión de resultados
    std::cout << "========================================" << std::endl;
    std::cout << "PRUEBA DEL SUMADOR (4 BITS)" << std::endl;
    std::cout << "A: 1001 (9) | B: 0011 (3)" << std::endl;
    std::cout << "Resultado Binario: " << cout_s.read() << " ";
    for (int i = 3; i >= 0; i--) std::cout << sum[i].read();
    std::cout << " (Decimal: 12)" << std::endl;

    std::cout << "----------------------------------------" << std::endl;

    std::cout << "PRUEBA DEL RESTADOR (4 BITS)" << std::endl;
    std::cout << "A: 0010 (2) | B: 0100 (4)" << std::endl;
    std::cout << "Resultado Binario: ";
    for (int i = 3; i >= 0; i--) std::cout << res[i].read();
    std::cout << " (Decimal: -2)" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}