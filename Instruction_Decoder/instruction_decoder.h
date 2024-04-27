// Instruction_Decoder.h
#ifndef INSTRUCTION_DECODER_H
#define INSTRUCTION_DECODER_H

#include <systemc.h>

SC_MODULE(Instruction_Decoder)
{
    // Inputs
    sc_in<sc_uint<32>> instruction;

    // Outputs
    sc_out<sc_uint<6>> opcode;
    sc_out<sc_uint<5>> rs;
    sc_out<sc_uint<5>> rt;
    sc_out<sc_uint<5>> rd;
    sc_out<sc_uint<16>> immediate;
    sc_out<sc_uint<32>> jump_address;
    sc_out<sc_uint<6>> control_signals;

    // Constructor
    SC_CTOR(Instruction_Decoder)
    {
        SC_METHOD(decode_instruction);
        sensitive << instruction;
    }

    // Method to decode the MIPS instruction
    void decode_instruction()
    {
        sc_uint<6> op = instruction.read().range(31, 26);
        sc_uint<5> rs_index = instruction.read().range(25, 21);
        sc_uint<5> rt_index = instruction.read().range(20, 16);
        sc_uint<5> rd_index = instruction.read().range(15, 11);
        sc_uint<16> imm = instruction.read().range(15, 0);
        sc_uint<32> j_addr = instruction.read().range(25, 0) << 2;

        // Outputs
        opcode.write(op);
        rs.write(rs_index);
        rt.write(rt_index);
        rd.write(rd_index);
        immediate.write(imm);
        jump_address.write(j_addr);

        // Control signals
        switch (op)
        {
        // R-type instructions
        case 0x00: // R-type
            control_signals.write(0b000000);
            break;
        // I-type instructions
        case 0x08: // Addi
            control_signals.write(0b001000);
            break;
        case 0x23: // LW
            control_signals.write(0b100011);
            break;
        case 0x2B: // SW
            control_signals.write(0b101011);
            break;
        // J-type instructions
        case 0x02: // J
            control_signals.write(0b000010);
            break;
        case 0x03: // Jal
            control_signals.write(0b000011);
            break;
        // Default for unsupported opcodes
        default:
            control_signals.write(0b000000);
            break;
        }
    }
};

#endif // INSTRUCTION_DECODER_H
