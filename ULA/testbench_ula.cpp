#include <systemc.h>
#include "ula.h"

int sc_main(int argc, char *argv[])
{
    sc_signal<sc_uint<32>> opA, opB, result;
    sc_signal<sc_uint<4>> aluOp;
    sc_signal<bool> zero;

    ULA ula("ULA");
    ula.opA(opA);
    ula.opB(opB);
    ula.aluOp(aluOp);
    ula.result(result);
    ula.zero(zero);

    // Teste de operações básicas
    opA = 10;
    opB = 20;
    aluOp = 0; // ADD
    sc_start(1, SC_NS);
    cout << "Resultado da soma (ADD): " << result.read() << ", Zero: " << zero.read() << endl;

    // Adicionando mais testes para outras operações
    // SUB
    opA = 30;
    opB = 15;
    aluOp = 1; // SUB
    sc_start(1, SC_NS);
    cout << "Resultado da subtração (SUB): " << result.read() << ", Zero: " << zero.read() << endl;

    // AND
    opA = 0x0F;
    opB = 0xF0;
    aluOp = 2; // AND
    sc_start(1, SC_NS);
    cout << "Resultado do AND: " << result.read() << ", Zero: " << zero.read() << endl;

    // OR
    opA = 0x0F;
    opB = 0xF0;
    aluOp = 3; // OR
    sc_start(1, SC_NS);
    cout << "Resultado do OR: " << result.read() << ", Zero: " << zero.read() << endl;

    // XOR
    opA = 0x0F;
    opB = 0xF0;
    aluOp = 4; // XOR
    sc_start(1, SC_NS);
    cout << "Resultado do XOR: " << result.read() << ", Zero: " << zero.read() << endl;

    // SLL
    opA = 2;
    opB = 3;
    aluOp = 5; // SLL
    sc_start(1, SC_NS);
    cout << "Resultado do SLL: " << result.read() << ", Zero: " << zero.read() << endl;

    // SRL
    opA = 16;
    opB = 2;
    aluOp = 6; // SRL
    sc_start(1, SC_NS);
    cout << "Resultado do SRL: " << result.read() << ", Zero: " << zero.read() << endl;

    // SRA
    opA = -16;
    opB = 2;
    aluOp = 7; // SRA
    sc_start(1, SC_NS);
    cout << "Resultado do SRA: " << result.read() << ", Zero: " << zero.read() << endl;

    // MULT
    opA = 10;
    opB = 5;
    aluOp = 8; // MULT
    sc_start(1, SC_NS);
    cout << "Resultado do MULT: " << result.read() << ", Zero: " << zero.read() << endl;

    // DIV
    opA = 20;
    opB = 4;
    aluOp = 9; // DIV
    sc_start(1, SC_NS);
    cout << "Resultado do DIV: " << result.read() << ", Zero: " << zero.read() << endl;

    return 0;
}
