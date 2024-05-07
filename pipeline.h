#ifndef PIPELINE_H
#define PIPELINE_H

#include <systemc.h>
#include "instruction_decoder.h"
#include "mips_memory.h"
#include "register_bank.h"
#include "ula.h"

enum PipelineState
{
    STATE_IF,
    STATE_ID,
    STATE_EX,
    STATE_MEM,
    STATE_WB,
    STATE_DONE
};

SC_MODULE(Pipeline)
{
    sc_in<bool> clock;
    sc_in<bool> reset;
    sc_out<PipelineState> current_state;

    sc_signal<sc_uint<32>> instruction;
    sc_signal<sc_uint<6>> opcode;
    sc_signal<sc_uint<5>> rs;
    sc_signal<sc_uint<5>> rt;
    sc_signal<sc_uint<5>> rd;
    sc_signal<sc_uint<16>> immediate;
    sc_signal<sc_uint<32>> jump_address;
    sc_signal<sc_uint<6>> control_signals;

    sc_signal<sc_uint<32>> opA, opB;
    sc_signal<sc_uint<4>> aluOp;
    sc_signal<sc_uint<32>> result;
    sc_signal<bool> zero;

    sc_signal<sc_uint<32>> address;
    sc_signal<sc_uint<32>> data_in;
    sc_signal<bool> write_enable_memory;
    sc_signal<bool> read_enable;
    sc_signal<sc_uint<32>> data_out;

    sc_signal<sc_uint<5>> read_reg1;
    sc_signal<sc_uint<5>> read_reg2;
    sc_signal<sc_uint<5>> write_reg;
    sc_signal<sc_uint<32>> write_data;
    sc_signal<bool> write_enable;

    sc_signal<sc_uint<32>> decoder_instruction;
    sc_signal<sc_uint<32>> ula_opA;
    sc_signal<sc_uint<32>> ula_opB;
    sc_signal<sc_uint<4>> ula_aluOp;
    sc_signal<sc_uint<32>> ula_result;
    sc_signal<bool> ula_zero;
    sc_signal<sc_uint<32>> memory_address;
    sc_signal<sc_uint<32>> memory_data_in;
    sc_signal<bool> memory_write_enable;
    sc_signal<bool> memory_read_enable;
    sc_signal<sc_uint<5>> register_bank_write_reg;
    sc_signal<sc_uint<32>> register_bank_write_data;
    sc_signal<bool> register_bank_write_enable;

    // Variáveis de controle do pipeline
    PipelineState state;
    bool stall;

    Instruction_Decoder *decoder;
    MIPS_Memory *memory;
    Register_Bank *register_bank;
    ULA *ula;

SC_CTOR(Pipeline, Instruction_Decoder *d, MIPS_Memory *m, Register_Bank *r, ULA *u) : decoder(d), memory(m), register_bank(r), ula(u)

{
    SC_THREAD(transition);
    sensitive << clock.pos();
}


    // Função de transição de estado
    void transition()
    {
        while (true) {
            wait(); // Wait for clock edge

            // Transition to next state
            if (reset.read()) {
                current_state.write(STATE_IF); // Reset to IF state
            } else {
                switch (current_state.read()) {
                    case STATE_IF:
                        // Fetch instruction
                        instruction.write(memory->data_out.read());
                        current_state.write(STATE_ID);
                        break;
                    case STATE_ID:
                        // Decode instruction
                        decoder_instruction.write(instruction.read());
                        opcode.write(decoder->opcode.read());
                        rs.write(decoder->rs.read());
                        rt.write(decoder->rt.read());
                        rd.write(decoder->rd.read());
                        immediate.write(decoder->immediate.read());
                        jump_address.write(decoder->jump_address.read());
                        control_signals.write(decoder->control_signals.read());
                        current_state.write(STATE_EX);
                        break;
                    case STATE_EX:
                        opA.write(register_bank->read_data1.read());
                        opB.write(register_bank->read_data2.read());
                        aluOp.write(control_signals.read());
                        break;
                    case STATE_MEM:
                        // Access memory
                        address.write(result.read());
                        write_enable_memory.write(control_signals.read() == 0x20); // Store
                        read_enable.write(control_signals.read() == 0x23); // Load
                        memory_address.write(address.read());
                        memory_data_in.write(write_data.read());
                        memory_write_enable.write(write_enable_memory.read());
                        memory_read_enable.write(read_enable.read());
                        data_out.write(memory->data_out.read());
                        current_state.write(STATE_WB);
                        break;
                    case STATE_WB:
                        // Write back
                        write_reg.write(rd.read());
                        write_data.write(result.read());
                        write_enable.write(true);
                        register_bank_write_reg.write(write_reg.read());
                        register_bank_write_data.write(write_data.read());
                        register_bank_write_enable.write(write_enable.read());
                        current_state.write(STATE_IF);
                        break;
                    default:
                        current_state.write(STATE_IF);
                        break;
                }
            }
        }
    }
};

#endif // PIPELINE_H