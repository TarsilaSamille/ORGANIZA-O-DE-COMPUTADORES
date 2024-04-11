#ifndef ULA_H
#define ULA_H

#include <systemc.h>

SC_MODULE(ULA)
{
    sc_in<sc_uint<32>> opA, opB;
    sc_in<sc_uint<4>> aluOp;
    sc_out<sc_uint<32>> result;
    sc_out<bool> zero;

    void compute()
    {
        sc_uint<32> a = opA.read();
        sc_uint<32> b = opB.read();
        sc_uint<32> out;
        bool z;

        // Executar operação de acordo com aluOp
        switch (aluOp.read())
        {
        case 0: // ADD
            out = a + b;
            z = (out == 0);
            break;
        case 1: // SUB
            out = a - b;
            z = (out == 0);
            break;
        case 2: // AND
            out = a & b;
            z = (out == 0);
            break;
        case 3: // OR
            out = a | b;
            z = (out == 0);
            break;
        case 4: // XOR
            out = a ^ b;
            z = (out == 0);
            break;
        case 5: // SLL
            out = a << b;
            z = (out == 0);
            break;
        case 6: // SRL
            out = a >> b;
            z = (out == 0);
            break;
        case 7:           // SRA
            out = a >> b; // Shift right arithmetic
            // Sign extend if the MSB is 1
            if (a[31] == 1)
            {
                for (int i = 31; i >= (int)(32 - b); --i)
                {
                    out[i] = 1;
                }
            }
            z = (out == 0);
            break;
        case 8: // MULT
            out = a * b;
            z = (out == 0);
            break;
        case 9: // DIV
            if (b != 0)
            {
                out = a / b; // Quociente
                z = (out == 0);
            }
            else
            {
                out = 0;
                z = true; // Divisão por zero
            }
            break;
        default:
            out = 0;
            z = true; // Indicar erro se aluOp for inválido
        }

        // Escrever resultados
        result.write(out);
        zero.write(z);
    }

    SC_CTOR(ULA)
    {
        SC_METHOD(compute);
        sensitive << opA << opB << aluOp;
    }
};

#endif
