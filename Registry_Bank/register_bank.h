#ifndef REGISTER_BANK_H
#define REGISTER_BANK_H

#include "systemc.h"
#include <iostream>

SC_MODULE(Registers_Bank)
{
    // Inputs
    sc_in<sc_uint<32>> LoadAddress1, LoadAddress2;
    sc_in<sc_uint<5>> WriteAddress;
    sc_in<sc_uint<32>> WriteData;
    sc_in<bool> RegWrite, MemWrite;
    sc_in_clk clock;

    // Outputs
    sc_out<sc_uint<32>> DataOut1, DataOut2;

    // Internal registers
    sc_uint<32> RegistersBank[32];

    // Methods
    void control();
    void memory_load();
    void memory_write();
    void print_registers();

    SC_CTOR(Registers_Bank)
    {
        SC_METHOD(control);
        sensitive << LoadAddress1 << LoadAddress2 << WriteAddress << WriteData << RegWrite << MemWrite;
    }
};

void Registers_Bank::control()
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

void Registers_Bank::memory_load()
{
    int addr = WriteAddress.read();
    if (addr >= 0 && addr < 32)
    {
        RegistersBank[addr] = WriteData.read();
    }
    else
    {
        std::cerr << "Error: WriteAddress out of range" << std::endl;
    }
}

void Registers_Bank::memory_write()
{
    int addr = LoadAddress1.read();
    if (addr >= 0 && addr < 32)
    {
        DataOut1.write(RegistersBank[addr]);
    }
    else
    {
        std::cerr << "Error: LoadAddress1 out of range" << std::endl;
    }
}

void Registers_Bank::print_registers()
{
    std::cout << "======= Registers Bank =======\n";
    for (size_t i = 0; i < 32; i++)
    {
        std::cout << "[" << i << "] - (" << RegistersBank[i] << ")\n";
    }
    std::cout << std::endl;
}

#endif
