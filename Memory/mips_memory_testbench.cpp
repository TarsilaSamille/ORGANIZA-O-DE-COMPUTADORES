// mips_memory_testbench.cpp
#include "mips_memory.h"
#include <systemc.h>
#include <iostream>

int sc_main(int argc, char *argv[])
{
    // Criando o módulo de memória
    MIPS_Memory memory("Memory");

    // Criando sinais
    sc_signal<sc_uint<32>> address;
    sc_signal<sc_uint<32>> data_in;
    sc_signal<bool> write_enable;
    sc_signal<bool> read_enable;
    sc_signal<sc_uint<32>> data_out;

    // Conectando sinais ao módulo de memória
    memory.address(address);
    memory.data_in(data_in);
    memory.write_enable(write_enable);
    memory.read_enable(read_enable);
    memory.data_out(data_out);

    // Caso de teste 1: Escrever e ler um valor válido
    write_enable = true;
    read_enable = false;
    address = 100;
    data_in = 123456789;
    sc_start(1, SC_NS);

    write_enable = false;
    read_enable = true;
    address = 100;
    sc_start(1, SC_NS);
    cout << "Valor lido da memória: " << data_out.read() << endl;

    // Caso de teste 2: Tentar escrever em um endereço inválido
    write_enable = true;
    read_enable = false;
    address = 1024; // Endereço inválido
    data_in = 987654321;
    sc_start(1, SC_NS);

    // Caso de teste 3: Tentar ler de um endereço inválido
    write_enable = false;
    read_enable = true;
    address = 1024; // Endereço inválido
    sc_start(1, SC_NS);
    cout << "Tentando ler de um endereço inválido: " << data_out.read() << endl;

    // Caso de teste 4: Leitura e escrita simultâneas
    write_enable = true;
    read_enable = true;
    address = 200;
    data_in = 11223344;
    sc_start(1, SC_NS);
    cout << "Valor lido durante a escrita: " << data_out.read() << endl;

    // Caso de teste 5: Escrever e ler um valor em um endereço de borda
    write_enable = true;
    read_enable = false;
    address = 1023; // Endereço de borda
    data_in = 99999999;
    sc_start(1, SC_NS);

    write_enable = false;
    read_enable = true;
    address = 1023; // Endereço de borda
    sc_start(1, SC_NS);
    cout << "Valor lido do endereço de borda: " << data_out.read() << endl;

    return 0;
}