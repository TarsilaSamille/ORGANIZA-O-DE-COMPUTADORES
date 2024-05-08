#ifndef IFU_H
#define IFU_H

#include <systemc.h>
#include "mips_memory.h"

SC_MODULE(IFU) {
    sc_in<bool> clock;
    sc_in<bool> reset;
    sc_out<sc_uint<32>> instruction;
    sc_out<sc_uint<32>> pc;

    sc_signal<sc_uint<32>> pc_reg;
    sc_signal<sc_uint<32>> instruction_reg;

    sc_out<sc_uint<32>> address;
    sc_out<bool> read_enable;
    sc_in<sc_uint<32>> data_out;

    MIPS_Memory *memory;

    SC_CTOR(IFU, MIPS_Memory *m) : memory(m) {
        SC_THREAD(fetch_instruction);
        sensitive << clock.pos();
    }

    void fetch_instruction() {
        if (reset.read()) {
            pc_reg.write(0x0); // Reset PC to 0
        } else {
            pc_reg.write(pc_reg.read() + 4); // Increment PC by 4
        }

        // Send PC to memory module
        address.write(pc_reg.read());
        read_enable.write(true);

        // Wait for memory to respond
        wait();

        // Fetch instruction from memory
        instruction_reg.write(data_out.read());

        // Update output signals
        pc.write(pc_reg.read());
        instruction.write(instruction_reg.read());
    }
};

#endif // IFU_H