#include <systemc.h>
#include "mips_processor.h"

int sc_main(int argc, char *argv[])
{
    sc_clock clock("clock", 1, SC_NS);
    sc_signal<bool> reset;
    sc_signal<sc_uint<32>> address, data_in, memory_data_out, ula_result, if_id_instr, id_ex_instr, ex_mem_instr, mem_wb_instr, wb_result;
    sc_signal<sc_uint<4>> aluOp;
    sc_signal<sc_uint<6>> opcode, control_signals;
    sc_signal<sc_uint<5>> rs, rt, rd;
    sc_signal<sc_uint<16>> immediate;
    sc_signal<sc_uint<32>> jump_address;
    sc_signal<bool> zero;
    sc_signal<sc_uint<32>> LoadAddress1, LoadAddress2, WriteData;
    sc_signal<sc_uint<5>> WriteAddress;
    sc_signal<bool> RegWrite, MemWrite, write_enable, read_enable;

    MIPS_Processor processor("processor");
    processor.clock(clock);
    processor.reset(reset);
    processor.address(address);
    processor.data_in(data_in);
    processor.memory_data_out(memory_data_out);
    processor.ula_result(ula_result);
    processor.if_id_instr(if_id_instr);
    processor.id_ex_instr(id_ex_instr);
    processor.ex_mem_instr(ex_mem_instr);
    processor.mem_wb_instr(mem_wb_instr);
    processor.wb_result(wb_result);
    processor.opcode(opcode);
    processor.rs(rs);
    processor.rt(rt);
    processor.rd(rd);
    processor.immediate(immediate);
    processor.jump_address(jump_address);
    processor.control_signals(control_signals);
    processor.opA(ula_result);
    processor.opB(data_in);
    processor.aluOp(aluOp);
    processor.LoadAddress1(LoadAddress1);
    processor.LoadAddress2(LoadAddress2);
    processor.WriteData(WriteData);
    processor.WriteAddress(WriteAddress);
    processor.RegWrite(RegWrite);
    processor.MemWrite(MemWrite);
    processor.write_enable(write_enable);
    processor.read_enable(read_enable);

    // Define signals and clock
    sc_clock clock("clock", 1, SC_NS);
    sc_signal<bool> reset;
    // Define other signals...

    // Instantiate MIPS_Processor
    MIPS_Processor processor("processor");
    processor.clock(clock);
    processor.reset(reset);
    // Connect other signals to MIPS_Processor ports...

    // Initialize reset to 1 for some time, then set it to 0
    reset = 1;
    sc_start(10, SC_NS); // Wait for 10 ns
    reset = 0;

    // You can add more stim
    // Start simulation
    sc_start(); // Run simulation

    return 0;
}
