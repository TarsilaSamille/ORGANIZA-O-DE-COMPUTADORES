#ifndef REGISTER_BANK_H
#define REGISTER_BANK_H

#include <systemc.h>

SC_MODULE(Register_Bank)
{
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<sc_uint<5>> read_reg1;
    sc_in<sc_uint<5>> read_reg2;
    sc_in<sc_uint<5>> write_reg;
    sc_in<sc_uint<32>> write_data;
    sc_in<bool> write_enable;

    sc_out<sc_uint<32>> read_data1;
    sc_out<sc_uint<32>> read_data2;

    SC_CTOR(Register_Bank)
    {
        SC_METHOD(register_access);
        sensitive << clk.pos();
    }

    void register_access()
    {
        if (reset.read())
        {
            // Initialize registers to 0
            for (int i = 0; i < 32; i++)
            {
                registers[i] = 0;
            }
        }
        else
        {
            if (write_enable.read())
            {
                registers[write_reg.read()] = write_data.read();
            }

            read_data1.write(registers[read_reg1.read()]);
            read_data2.write(registers[read_reg2.read()]);
        }
    }
    void print_registers() const
    {
        std::cout << "Register contents:" << std::endl;
        for (int i = 0; i < 32; ++i)
        {
            std::cout << "R" << i << ": " << registers[i] << std::endl;
        }
    }

private:
    sc_uint<32> registers[32]; // 32 registers of 32 bits each
};

#endif // REGISTER_BANK_H