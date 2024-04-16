#include <systemc.h>
#include "register_bank.h" // Include only the register bank module header file

int sc_main(int argc, char *argv[])
{
    // Signals for Register Bank
    sc_signal<sc_uint<32>> LoadAddress1, LoadAddress2;
    sc_signal<sc_uint<5>> WriteAddress;
    sc_signal<sc_uint<32>> WriteData;
    sc_signal<bool> RegWrite, MemWrite;
    sc_signal<sc_uint<32>> DataOut1, DataOut2;
    sc_clock clock("clock", 1, SC_NS); // Declare clock signal

    // Instantiate register bank module
    registers_bank reg_bank("RegisterBank");

    // Connect Register Bank signals
    reg_bank.LoadAddress1(LoadAddress1);
    reg_bank.LoadAddress2(LoadAddress2);
    reg_bank.WriteAddress(WriteAddress);
    reg_bank.WriteData(WriteData);
    reg_bank.RegWrite(RegWrite);
    reg_bank.MemWrite(MemWrite);
    reg_bank.DataOut1(DataOut1);
    reg_bank.DataOut2(DataOut2);
    reg_bank.clock(clock); // Connect clock signal to register bank

    // Set up test cases
    sc_start(1, SC_NS); // Start simulation to initialize signals

    // Test register bank operations
    WriteAddress = 10;
    WriteData = 50;
    RegWrite = true;
    MemWrite = true;
    sc_start(1, SC_NS);
    reg_bank.print_registers(); // Print registers after write operation

    LoadAddress1 = 10;
    sc_start(1, SC_NS);
    cout << "Data read from register 10: " << DataOut1.read() << endl;

    // Add more test cases as needed...

    return 0;
}
