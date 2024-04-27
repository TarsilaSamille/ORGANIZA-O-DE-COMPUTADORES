// MIPS_Processor.h
#ifndef MIPS_PROCESSOR_H
#define MIPS_PROCESSOR_H

#include <systemc.h>
#include "mips_memory.h"
#include "ula.h"
#include "register_bank.h"
#include "pipeline_register.h"
#include "instruction_decoder.h"

SC_MODULE(MIPS_Processor)
{
    // Componentes do processador
    MIPS_Memory memory;
    ULA ula;
    Registers_Bank reg_bank;
    Pipeline_Register pipeline_reg;
    Instruction_Decoder decoder;

    // Construtor
    SC_CTOR(MIPS_Processor) : memory("Memory"), ula("ULA"), reg_bank("RegisterBank"), pipeline_reg("PipelineRegister"), decoder("InstructionDecoder")
    {
        // Conectar sinais entre os componentes
        memory.address(address);
        memory.data_in(data_in);
        memory.write_enable(write_enable);
        memory.read_enable(read_enable);
        memory.data_out(memory_data_out);

        ula.opA(opA);
        ula.opB(opB);
        ula.aluOp(aluOp);
        ula.result(ula_result);
        ula.zero(zero);

        reg_bank.LoadAddress1(LoadAddress1);
        reg_bank.LoadAddress2(LoadAddress2);
        reg_bank.WriteAddress(WriteAddress);
        reg_bank.WriteData(WriteData);
        reg_bank.RegWrite(RegWrite);
        reg_bank.MemWrite(MemWrite);
        reg_bank.clock(clock);

        pipeline_reg.clock(clock);
        pipeline_reg.reset(reset);
        pipeline_reg.if_id_instr_in(if_id_instr);
        pipeline_reg.id_ex_instr_in(id_ex_instr);
        pipeline_reg.ex_mem_instr_in(ex_mem_instr);
        pipeline_reg.mem_wb_instr_in(mem_wb_instr);
        pipeline_reg.wb_result_in(wb_result);

        decoder.instruction(if_id_instr);
        decoder.opcode(opcode);
        decoder.rs(rs);
        decoder.rt(rt);
        decoder.rd(rd);
        decoder.immediate(immediate);
        decoder.jump_address(jump_address);
        decoder.control_signals(control_signals);
    }

    // Entradas
    sc_in<bool> clock;
    sc_in<bool> reset;
    sc_in<sc_uint<32>> address;
    sc_in<sc_uint<32>> data_in;
    sc_in<bool> write_enable;
    sc_in<bool> read_enable;
    sc_in<sc_uint<32>> opA;
    sc_in<sc_uint<32>> opB;
    sc_in<sc_uint<4>> aluOp;
    sc_in<sc_uint<32>> LoadAddress1;
    sc_in<sc_uint<32>> LoadAddress2;
    sc_in<sc_uint<5>> WriteAddress;
    sc_in<sc_uint<32>> WriteData;
    sc_in<bool> RegWrite;
    sc_in<bool> MemWrite;

    // Saídas
    sc_out<sc_uint<32>> memory_data_out;
    sc_out<sc_uint<32>> ula_result;
    sc_out<bool> zero;
    sc_out<sc_uint<32>> if_id_instr;
    sc_out<sc_uint<32>> id_ex_instr;
    sc_out<sc_uint<32>> ex_mem_instr;
    sc_out<sc_uint<32>> mem_wb_instr;
    sc_out<sc_uint<32>> wb_result;
    sc_out<sc_uint<6>> opcode;
    sc_out<sc_uint<5>> rs;
    sc_out<sc_uint<5>> rt;
    sc_out<sc_uint<5>> rd;
    sc_out<sc_uint<16>> immediate;
    sc_out<sc_uint<32>> jump_address;
    sc_out<sc_uint<6>> control_signals;
};

#endif // MIPS_PROCESSOR_H
