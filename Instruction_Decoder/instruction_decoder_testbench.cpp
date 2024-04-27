#include <systemc.h>
#include "instruction_decoder.h"

// Testbench module
SC_MODULE(Testbench)
{
    // Signals
    sc_signal<sc_uint<32>> instruction;
    sc_signal<sc_uint<6>> opcode;
    sc_signal<sc_uint<5>> rs;
    sc_signal<sc_uint<5>> rt;
    sc_signal<sc_uint<5>> rd;
    sc_signal<sc_uint<16>> immediate;
    sc_signal<sc_uint<32>> jump_address;
    sc_signal<sc_uint<6>> control_signals;

    // Instruction_Decoder instance
    Instruction_Decoder decoder;

    // Constructor
    SC_CTOR(Testbench) : decoder("Decoder")
    {
        // Connect signals
        decoder.instruction(instruction);
        decoder.opcode(opcode);
        decoder.rs(rs);
        decoder.rt(rt);
        decoder.rd(rd);
        decoder.immediate(immediate);
        decoder.jump_address(jump_address);
        decoder.control_signals(control_signals);

        // Process to generate test instructions
        SC_THREAD(generate_instructions);
    }

    // Method to generate test instructions
    void generate_instructions()
    {
        wait(1, SC_NS); // Wait for 1 ns to start simulation

        // Test instruction: Addi
        instruction.write(0x2083000A); // Addi $3, $4, 10
        wait(1, SC_NS);                // Wait for decoder to process instruction

        // Test instruction: LW
        instruction.write(0x8C420000); // LW $2, 0($2)
        wait(1, SC_NS);                // Wait for decoder to process instruction

        // Test instruction: SW
        instruction.write(0xAC820000); // SW $2, 0($4)
        wait(1, SC_NS);                // Wait for decoder to process instruction

        // Test instruction: J
        instruction.write(0x0800000A); // J 0x0000000A
        wait(1, SC_NS);                // Wait for decoder to process instruction

        // Test instruction: Jal
        instruction.write(0x0C000000); // Jal 0x00000000
        wait(1, SC_NS);                // Wait for decoder to process instruction

        // Test instruction: Unsupported opcode
        instruction.write(0xFFFFFFFF); // Invalid instruction
        wait(1, SC_NS);                // Wait for decoder to process instruction

        // End of simulation
        sc_stop();
    }
};

int sc_main(int argc, char *argv[])
{
    // Instantiate testbench module
    Testbench testbench("Testbench");

    // Trace signals
    sc_trace_file *tf = sc_create_vcd_trace_file("instruction_decoder");
    sc_trace(tf, testbench.instruction, "instruction");
    sc_trace(tf, testbench.opcode, "opcode");
    sc_trace(tf, testbench.rs, "rs");
    sc_trace(tf, testbench.rt, "rt");
    sc_trace(tf, testbench.rd, "rd");
    sc_trace(tf, testbench.immediate, "immediate");
    sc_trace(tf, testbench.jump_address, "jump_address");
    sc_trace(tf, testbench.control_signals, "control_signals");

    // Start simulation
    sc_start();

    // Close trace file
    sc_close_vcd_trace_file(tf);

    return 0;
}
