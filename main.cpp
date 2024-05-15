#include "pipeline.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <queue>
#include <bitset>

// Define constants for the pipeline states (for example)
#define STATE_IF 1
#define STATE_ID 2
#define STATE_EX 3
#define STATE_MEM 4
#define STATE_WB 0
#define STATE_DONE 5

int sc_main(int argc, char *argv[])
{
    // Declaration of signals
    sc_clock clock("clock", 10, SC_NS);
    sc_signal<bool> reset;
    sc_signal<PipelineState> current_state;
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
    sc_signal<sc_uint<32>> read_data1, read_data2;

    sc_signal<sc_uint<32>> pc;
    sc_signal<sc_uint<32>> address_ifu;
    sc_signal<bool> read_enable_ifu;

    // Instantiate modules
    Instruction_Decoder *decoder = new Instruction_Decoder("decoder");
    MIPS_Memory *memory = new MIPS_Memory("memory");
    Register_Bank *register_bank = new Register_Bank("register_bank");
    ULA *ula = new ULA("ula");
    IFU *ifu = new IFU("ifu", memory);
    Pipeline *pipeline = new Pipeline("pipeline", decoder, memory, register_bank, ula, ifu);

    // Connect signals
    decoder->clk(clock);
    decoder->reset(reset);
    decoder->opcode(opcode);
    decoder->rs(rs);
    decoder->rt(rt);
    decoder->rd(rd);
    decoder->immediate(immediate);
    decoder->jump_address(jump_address);
    decoder->control_signals(control_signals);

    ula->clk(clock);
    ula->reset(reset);
    ula->opA(opA);
    ula->opB(opB);
    ula->aluOp(aluOp);
    ula->result(result);
    ula->zero(zero);

    memory->clk(clock);
    memory->reset(reset);
    memory->data_in(data_in);
    memory->write_enable(write_enable_memory);
    memory->data_out(data_out);

    register_bank->clk(clock);
    register_bank->reset(reset);
    register_bank->read_reg1(read_reg1);
    register_bank->read_reg2(read_reg2);
    register_bank->write_reg(write_reg);
    register_bank->write_data(write_data);
    register_bank->write_enable(write_enable);
    register_bank->read_data1(read_data1);
    register_bank->read_data2(read_data2);

    ifu->clock(clock);
    ifu->reset(reset);
    ifu->pc(pc);
    ifu->address(address_ifu);
    ifu->read_enable(read_enable_ifu);
    ifu->instruction(instruction);

    memory->address.bind(address_ifu);
    memory->read_enable.bind(read_enable_ifu);
    ifu->data_out.bind(memory->data_out);
    decoder->instruction.bind(instruction);

    pipeline->clock(clock);
    pipeline->reset(reset);
    pipeline->current_state(current_state);

    // Tracing
    sc_trace_file *tf = sc_create_vcd_trace_file("pipeline_trace");
    sc_trace(tf, clock, "clock");
    sc_trace(tf, reset, "reset");
    sc_trace(tf, current_state, "current_state");
    sc_trace(tf, instruction, "instruction");
    sc_trace(tf, opcode, "opcode");
    sc_trace(tf, rs, "rs");
    sc_trace(tf, rt, "rt");
    sc_trace(tf, rd, "rd");
    sc_trace(tf, immediate, "immediate");
    sc_trace(tf, jump_address, "jump_address");
    sc_trace(tf, control_signals, "control_signals");
    sc_trace(tf, opA, "opA");
    sc_trace(tf, opB, "opB");
    sc_trace(tf, aluOp, "aluOp");
    sc_trace(tf, result, "result");
    sc_trace(tf, zero, "zero");
    sc_trace(tf, address, "address");
    sc_trace(tf, data_in, "data_in");
    sc_trace(tf, write_enable_memory, "write_enable_memory");
    sc_trace(tf, read_enable, "read_enable");
    sc_trace(tf, data_out, "data_out");
    sc_trace(tf, read_reg1, "read_reg1");
    sc_trace(tf, read_reg2, "read_reg2");
    sc_trace(tf, write_reg, "write_reg");
    sc_trace(tf, write_data, "write_data");
    sc_trace(tf, write_enable, "write_enable");
    sc_trace(tf, pc, "pc");
    sc_trace(tf, address_ifu, "address_ifu");
    sc_trace(tf, read_enable_ifu, "read_enable_ifu");

std::queue<sc_uint<32>> instruction_queue;
sc_signal<bool> instruction_ready;

// Load instructions from file
std::ifstream file("instructions.bin", std::ios::binary);
if (!file.is_open()) {
    std::cerr << "Error: unable to open file 'instructions.bin'" << std::endl;
    return 1;
}

char buffer[4];
while (file.read(buffer, 4)) {
    sc_uint<32> instr = 0;
    for (int i = 0; i < 4; i++) {
        instr.range((i+1)*8-1, i*8) = static_cast<unsigned char>(buffer[i]);
    }

    instruction_queue.push(instr);
}

file.close();
    // Run simulation
    reset = true;
    sc_start(10, SC_NS);
    reset = false;

int num_cycles = 50;
int instruction_count = 0;
while (instruction_count < instruction_queue.size() && num_cycles > 0) {
    ifu->instruction.write(instruction_queue.front());
    instruction_queue.pop();
    instruction_count++;

    for (int cycle_count = 0; cycle_count < 5; cycle_count++) {
        sc_start(10, SC_NS);

        std::cout << "Cycle " << cycle_count << ":" << std::endl;
        std::cout << "  Current State = " << current_state.read() << std::endl;
        std::cout << "  Instruction: " << ifu->instruction.read() << std::endl;
        std::cout << "  Opcode: " << opcode.read() << std::endl;
        std::cout << "  RS: " << rs.read() << std::endl;
        std::cout << "  RT: " << rt.read() << std::endl;
        std::cout << "  RD: " << rd.read() << std::endl;
        std::cout << "  Immediate: " << immediate.read() << std::endl;
        std::cout << "  Jump address: " << jump_address.read() << std::endl;
        std::cout << "  Control signals: " << control_signals.read() << std::endl;

// std::cout << "Initial values:" << std::endl;
// std::cout << "ula->clk: " << ula->clk.read() << std::endl;
// std::cout << "ula->reset: " << ula->reset.read() << std::endl;
// std::cout << "ula->opA: " << ula->opA.read() << std::endl;
// std::cout << "ula->opB: " << ula->opB.read() << std::endl;
// std::cout << "ula->aluOp: " << ula->aluOp.read() << std::endl;
// std::cout << "ula->result: " << ula->result.read() << std::endl;
// std::cout << "ula->zero: " << ula->zero.read() << std::endl;

        if (current_state.read() == STATE_DONE) {
            break;
        }

        if (current_state.read() == STATE_IF) {
            instruction_ready.write(true);
        }
  }

    num_cycles -= 10;
}

    sc_close_vcd_trace_file(tf);
    return 0;
}
