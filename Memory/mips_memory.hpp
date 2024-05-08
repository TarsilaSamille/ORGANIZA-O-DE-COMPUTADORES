// mips_memory.h
#ifndef MIPS_MEMORY_H
#define MIPS_MEMORY_H

#include <systemc.h>

SC_MODULE(MIPS_Memory)
{

    // Inputs
    sc_in<bool> clk;
    sc_in<bool> reset;
    // Entradas
    sc_in<sc_uint<32>> address;
    sc_in<sc_uint<32>> data_in;
    sc_in<bool> write_enable;
    sc_in<bool> read_enable;

    // Saídas
    sc_out<sc_uint<32>> data_out;

    sc_signal<bool> done;

    // Construtor
    SC_CTOR(MIPS_Memory)
    {
        SC_METHOD(memory_access);
        sensitive << address << data_in << write_enable << read_enable;
    }

    // Método para acesso à memória
    void memory_access()
    {
  if (reset.read())
    {
        // Initialize memory to 0
        for (int i = 0; i < 1024; i++)
        {
            memory[i] = 0;
        }
    }
    else
    {
        // Write to memory
        if (write_enable.read())
        {
            memory[address.read()] = data_in.read();
        }

        // Read from memory
        if (read_enable.read())
        {
            data_out.write(memory[address.read()]);
        done.write(true);
        }
    }
    }

private:
    sc_uint<32> memory[1024]; // Memória de 1024 palavras de 32 bits
};

#endif // MIPS_MEMORY_H