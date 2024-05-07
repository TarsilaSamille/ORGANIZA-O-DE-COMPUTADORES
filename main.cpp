#include "pipeline.h"

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

    // Instantiate modules
    Instruction_Decoder *decoder = new Instruction_Decoder("decoder");
    MIPS_Memory *memory = new MIPS_Memory("memory");
    Register_Bank *register_bank = new Register_Bank("register_bank");
    ULA *ula = new ULA("ula");
    Pipeline *pipeline = new Pipeline("pipeline", decoder, memory, register_bank, ula);

    // Connect signals
    decoder->clk(clock);
    decoder->reset(reset);
    decoder->instruction(instruction);
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
    memory->address(address);
    memory->data_in(data_in);
    memory->write_enable(write_enable_memory);
    memory->read_enable(read_enable);
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


    pipeline->clock(clock);
    pipeline->reset(reset);
    pipeline->current_state(current_state);

    // Load instructions from file
    std::ifstream file("instructions.bin", std::ios::binary);
    if (file.is_open()) {
        char buffer[4];
        while (file.read(buffer, 4)) {
            sc_uint<32> instr;
            for (int i = 0; i < 4; i++) {
                instr[i*8 + 7] = (buffer[i] >> 7) & 1;
                instr[i*8 + 6] = (buffer[i] >> 6) & 1;
                instr[i*8 + 5] = (buffer[i] >> 5) & 1;
                instr[i*8 + 4] = (buffer[i] >> 4) & 1;
                instr[i*8 + 3] = (buffer[i] >> 3) & 1;
                instr[i*8 + 2] = (buffer[i] >> 2) & 1;
                instr[i*8 + 1] = (buffer[i] >> 1) & 1;
                instr[i*8 + 0] = (buffer[i] >> 0) & 1;
            }
            instruction.write(instr);
            sc_start(10, SC_NS);
        }
        file.close();
    } else {
        std::cerr << "Error: unable to open file 'instructions.bin'" << std::endl;
    }

    // Run simulation
    reset = true;
    sc_start(10, SC_NS);
    reset = false;
    int num_cycles = 100; // Stop simulation after 100 cycles
    int cycle_count = 0;
    while (true) {
        if (current_state.read() == STATE_DONE) {
            break;
        }

        // Print statements
        std::cout << "Current state: " << current_state.read() << std::endl;
        std::cout << "Instruction: " << instruction.read() << std::endl;
        std::cout << "Opcode: " << opcode.read() << std::endl;
        std::cout << "RS: " << rs.read() << std::endl;
        std::cout << "RT: " << rt.read() << std::endl;
        std::cout << "RD: " << rd.read() << std::endl;
        std::cout << "Immediate: " << immediate.read() << std::endl;
        std::cout << "Jump address: " << jump_address.read() << std::endl;
        std::cout << "Control signals: " << control_signals.read() << std::endl;

        cycle_count++;

        if (cycle_count >= num_cycles || current_state.read() == STATE_DONE) {
            break;
        }

        sc_start(10, SC_NS);
    }

    return 0;
}