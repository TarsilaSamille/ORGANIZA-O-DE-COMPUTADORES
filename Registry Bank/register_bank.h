#ifndef REGISTER_BANK_H
#define REGISTER_BANK_H
#include "systemc.h"

#include <vector>

SC_MODULE(registers_bank)
{
    sc_in<sc_uint<32>> LoadAddress1, LoadAddress2;
    sc_in<sc_uint<5>> WriteAddress;
    sc_in<sc_uint<32>> WriteData;
    sc_in<bool> RegWrite, MemWrite;

    sc_out<sc_uint<32>> DataOut1, DataOut2;

    sc_uint<32> RegistersBank[32];

    sc_in_clk clock;

    // methods
    void control();
    void memory_load();
    void memory_write();
    void print();

    SC_CTOR(registers_bank)
    {
        SC_METHOD(control);
        sensitive << LoadAddress1 << LoadAddress2 << WriteAddress << WriteData << RegWrite << MemWrite;
    }
};

void registers_bank::control()
{
    if (RegWrite.read())
    {
        memory_load();
    }
    if (MemWrite.read())
    {
        memory_write();
    }
}

void registers_bank::memory_load()
{
    RegistersBank[WriteAddress.read()] = WriteData.read();
}

void registers_bank::memory_write()
{
    DataOut1.write(RegistersBank[LoadAddress1.read().range(25, 21)]);
}

void registers_bank::print()
{
    std::cout << "=======Registradores=======" << std::endl;
    for (size_t i{0}; i < 32; i++)
    {
        std::cout << "[" << i << "]"
                  << "- (" << RegistersBank[i] << ")" << std::endl;
    }
    std::cout << std::endl;
}

#endif