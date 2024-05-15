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

    // Method to decode the MIPS instruction
    void decode_instruction()
    {
uint32_t instr_word = instruction.read().to_uint64(); // Convert to standard C++ type for bit manipulation
     std::cout << "Opcode: " << instr_word << std::endl;

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

    // Print out decoded values
    std::cout << "Opcode: " << op << std::endl;
    std::cout << "RS Index: " << rs_index << std::endl;
    std::cout << "RT Index: " << rt_index << std::endl;
    std::cout << "RD Index: " << rd_index << std::endl;
    std::cout << "Immediate: " << imm << std::endl;
    std::cout << "Jump Address: " << j_addr << std::endl;

    // Control signals
    switch (op.to_int())
        {
            // R-type instructions
            case 0: // R-type
                control_signals.write(0b000000);
                break;
            // I-type instructions
            case 8: // Addi
                control_signals.write(0b001000);
                break;
            case 9: // Addiu
                control_signals.write(0b001001);
                break;
            case 10: // Slti
                control_signals.write(0b001010);
                break;
            case 11: // Sltiu
                control_signals.write(0b001011);
                break;
            case 12: // Andi
                control_signals.write(0b001100);
                break;
            case 13: // Ori
                control_signals.write(0b001101);
                break;
            case 14: // Xori
                control_signals.write(0b001110);
                break;
            case 15: // Lui
                control_signals.write(0b001111);
                break;
            case 32: // Lb
                control_signals.write(0b100000);
                break;
            case 33: // Lh
                control_signals.write(0b100001);
                break;
            case 34: // Lwl
                control_signals.write(0b100010);
                break;
            case 35: // Lw
                control_signals.write(0b100011);
                break;
            case 36: // Lbu
                control_signals.write(0b100100);
                break;
            case 37: // Lhu
                control_signals.write(0b100101);
                break;
            case 38: // Lwr
                control_signals.write(0b100110);
                break;
            case 40: // Sb
                control_signals.write(0b101000);
                break;
            case 41: // Sh
                control_signals.write(0b101001);
                break;
            case 42: // Swl
                control_signals.write(0b101010);
                break;
            case 43: // Sw
                control_signals.write(0b101011);
                break;
            case 44: // Swr
                control_signals.write(0b101100);
                break;
            case 46: // Lwc1
                control_signals.write(0b110000);
                break;
            case 47: // Lwc2
                control_signals.write(0b110001);
                break;
            case 50: // Swc1
                control_signals.write(0b110100);
                break;
            case 51: // Swc2
                control_signals.write(0b110101);
                break;
            // J-type instructions
            case 2: // J
                control_signals.write(0b000010);
                break;
            case 3: // Jal
                control_signals.write(0b000011);
                break;
            case 4: // Beq
                control_signals.write(0b000100);
                break;
            case 5: // Bne
                control_signals.write(0b000101);
                break;
            case 6: // Blez
                control_signals.write(0b000110);
                break;
            case 7: // Bgtz
                control_signals.write(0b000111);
                break;
            default:
                std::cout << "Unsupported opcode: " << op << std::endl;
                control_signals.write(0b000000); // Set default control signals
                break;
        }
    }
};

#endif // INSTRUCTION_DECODER_H
