#ifndef INSTRUCTION_DECODER_H
#define INSTRUCTION_DECODER_H

#include <systemc.h>


SC_MODULE(Instruction_Decoder)
{
    // Inputs
    sc_in<bool> clk;
    sc_in<bool> reset;
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
        dont_initialize();
    }
void decode_instruction()
{
    uint32_t instr_word = instruction.read().to_uint64(); // Convert to standard C++ type for bit manipulation

    // Extract fields from the instruction word
    sc_uint<6> op = instr_word >> 26;
    sc_uint<5> rs_index = (instr_word >> 21) & 0x1F;
    sc_uint<5> rt_index = (instr_word >> 16) & 0x1F;
    sc_uint<5> rd_index = (instr_word >> 11) & 0x1F;
    sc_uint<16> imm = instr_word & 0xFFFF;
    sc_uint<32> j_addr = (instr_word >> 2) & 0x3FFFFFF;

    // Update output ports with decoded values
    opcode.write(op);
    rs.write(rs_index);
    rt.write(rt_index);
    rd.write(rd_index);
    immediate.write(imm);
    jump_address.write(j_addr);

    // Control signals
    switch (op.to_int())
    {
        // R-type instructions
        case 0: // R-type
            control_signals.write(0b000000);
            break;
        // I-type instructions
        case 8: // Addi
        case 9: // Addiu
        case 10: // Slti
        case 11: // Sltiu
        case 12: // Andi
        case 13: // Ori
        case 14: // Xori
        case 15: // Lui
            control_signals.write(0b001000); // Send opcode to ULA for ALU operation
            break;
        case 32: // Lb
        case 33: // Lh
        case 34: // Lwl
        case 35: // Lw
        case 36: // Lbu
        case 37: // Lhu
        case 38: // Lwr
            control_signals.write(0b000100); // Send opcode to ULA for load operation
            break;
        case 40: // Sb
        case 41: // Sh
        case 42: // Swl
        case 43: // Sw
        case 44: // Swr
            control_signals.write(0b000101); // Send opcode to ULA for store operation
            break;
        case 46: // Lwc1
        case 47: // Lwc2
            control_signals.write(0b000110); // Send opcode to ULA for load coprocessor operation
            break;
        case 50: // Swc1
        case 51: // Swc2
            control_signals.write(0b000111); // Send opcode to ULA for store coprocessor operation
            break;
        // J-type instructions
        case 2: // J
            control_signals.write(0b000010); // Send opcode to ULA for jump operation
            break;
        case 3: // Jal
            control_signals.write(0b000011); // Send opcode to ULA for jump and link operation
            break;
        case 4: // Beq
        case 5: // Bne
        case 6: // Blez
        case 7: // Bgtz
            control_signals.write(0b000110); // Send opcode to ULA for branch operation
            break;
        default:
            control_signals.write(0b000000); // Set default control signals
            break;
    }
}


};

#endif // INSTRUCTION_DECODER_H
