#ifndef PIPELINE_REGISTER_H
#define PIPELINE_REGISTER_H

#include <systemc.h>

SC_MODULE(Pipeline_Register)
{
    // Inputs
    sc_in<bool> clock;
    sc_in<bool> reset;
    sc_in<sc_uint<32>> if_id_instr_in;
    sc_in<sc_uint<32>> id_ex_instr_in;
    sc_in<sc_uint<32>> ex_mem_instr_in;
    sc_in<sc_uint<32>> mem_wb_instr_in;
    sc_in<sc_uint<32>> wb_result_in;
    sc_in<sc_uint<6>> id_ex_opcode;
    sc_in<sc_uint<6>> id_ex_reg_number1;
    sc_in<sc_uint<6>> id_ex_reg_number2;
    sc_in<sc_uint<6>> id_ex_reg_number;
    sc_in<sc_uint<32>> id_ex_immediate;
    sc_in<sc_uint<26>> id_ex_jump_address;
    sc_in<sc_uint<5>> id_ex_control_signals;
    sc_in<bool> id_ex_reg_write;
    sc_in<bool> id_ex_mem_read;
    sc_in<bool> id_ex_mem_write;
    sc_in<sc_uint<32>> ex_mem_alu_result;
    sc_in<bool> ex_mem_zero;

    sc_in<bool> ex_mem_reg_write;
    sc_in<sc_uint<6>> ex_mem_reg_number1;
    sc_in<sc_uint<32>> mem_wb_data_memory_out;
    sc_in<bool> mem_wb_reg_write;
    sc_in<sc_uint<32>> mem_wb_result;
    sc_in<sc_uint<32>> mem_wb_pc;

    // Outputs
    sc_out<sc_uint<32>> if_id_instr_out;
    sc_out<sc_uint<32>> id_ex_instr_out;
    sc_out<sc_uint<32>> ex_mem_instr_out;
    sc_out<sc_uint<32>> mem_wb_instr_out;
    sc_out<sc_uint<32>> wb_result_out;
    sc_out<sc_uint<6>> id_ex_opcode_out;
    sc_out<sc_uint<6>> id_ex_reg_number1_out;
    sc_out<sc_uint<6>> id_ex_reg_number2_out;
    sc_out<sc_uint<6>> id_ex_reg_number_out;
    sc_out<sc_uint<32>> id_ex_immediate_out;
    sc_out<sc_uint<26>> id_ex_jump_address_out;
    sc_out<sc_uint<5>> id_ex_control_signals_out;
    sc_out<bool> id_ex_reg_write_out;
    sc_out<bool> id_ex_mem_read_out;
    sc_out<bool> id_ex_mem_write_out;
    sc_out<sc_uint<32>> ex_mem_alu_result_out;
    sc_out<bool> ex_mem_zero_out;
    sc_out<bool> ex_mem_reg_write_out;
    sc_out<sc_uint<6>> ex_mem_reg_number1_out;
    sc_out<sc_uint<32>> mem_wb_data_memory_out_out;
    sc_out<bool> mem_wb_reg_write_out;
    sc_out<sc_uint<32>> mem_wb_result_out;
    sc_out<sc_uint<32>> mem_wb_pc_out;

    // Inputs
    sc_in<sc_uint<32>> id_ex_reg_data1;
    sc_in<sc_uint<32>> id_ex_reg_data2;
    sc_in<bool> ex_mem_mem_read;

    sc_in<sc_uint<32>> if_id_pc_in;
    sc_in<sc_uint<32>> id_ex_pc_in;
    sc_in<sc_uint<32>> ex_mem_pc_in;
    sc_in<sc_uint<32>> mem_wb_pc_in;
    sc_in<sc_uint<32>> wb_result_pc_in;

    // Outputs
    sc_out<sc_uint<32>> if_id_pc_out;
    sc_out<sc_uint<32>> id_ex_pc_out;
    sc_out<sc_uint<32>> ex_mem_pc_out;
    sc_out<sc_uint<32>> mem_wb_pc_out;
    sc_out<sc_uint<32>> wb_result_pc_out;

    // Constructor
    SC_CTOR(Pipeline_Register)
    {
        SC_METHOD(register_update);
        sensitive << clock.pos() << reset;
    }

    // Method to update pipeline registers
    void register_update();
};

void Pipeline_Register::register_update()
{
    if (reset.read())
    {
        // Reset all registers to zero
        if_id_instr_out.write(0);
        id_ex_instr_out.write(0);
        ex_mem_instr_out.write(0);
        mem_wb_instr_out.write(0);
        wb_result_out.write(0);
        id_ex_opcode_out.write(0);
        id_ex_reg_number1_out.write(0);
        id_ex_reg_number2_out.write(0);
        id_ex_reg_number_out.write(0);
        id_ex_immediate_out.write(0);
        id_ex_jump_address_out.write(0);
        id_ex_control_signals_out.write(0);
        id_ex_reg_write_out.write(false);
        id_ex_mem_read_out.write(false);
        id_ex_mem_write_out.write(false);
        ex_mem_alu_result_out.write(0);
        ex_mem_reg_write_out.write(false);
        ex_mem_reg_number1_out.write(0);
        mem_wb_data_memory_out_out.write(0);
        mem_wb_reg_write_out.write(false);
        mem_wb_result_out.write(0);
        mem_wb_pc_out.write(0);

        if_id_pc_out.write(0);
        id_ex_pc_out.write(0);
        ex_mem_pc_out.write(0);
        mem_wb_pc_out.write(0);
        wb_result_pc_out.write(0);
    }
    else
    {
        // Update registers with input values
        if_id_instr_out.write(if_id_instr_in.read());
        id_ex_instr_out.write(id_ex_instr_in.read());
        ex_mem_instr_out.write(ex_mem_instr_in.read());
        mem_wb_instr_out.write(mem_wb_instr_in.read());
        wb_result_out.write(wb_result_in.read());
        id_ex_opcode_out.write(id_ex_opcode.read());
        id_ex_reg_number1_out.write(id_ex_reg_number1.read());
        id_ex_reg_number2_out.write(id_ex_reg_number2.read());
        id_ex_reg_number_out.write(id_ex_reg_number.read());
        id_ex_immediate_out.write(id_ex_immediate.read());
        id_ex_jump_address_out.write(id_ex_jump_address.read());
        id_ex_control_signals_out.write(id_ex_control_signals.read());
        id_ex_reg_write_out.write(id_ex_reg_write.read());
        id_ex_mem_read_out.write(id_ex_mem_read.read());
        id_ex_mem_write_out.write(id_ex_mem_write.read());
        ex_mem_alu_result_out.write(ex_mem_alu_result.read());
        ex_mem_reg_write_out.write(ex_mem_reg_write.read());
        ex_mem_reg_number1_out.write(ex_mem_reg_number1.read());
        mem_wb_data_memory_out_out.write(mem_wb_data_memory_out.read());
        mem_wb_reg_write_out.write(mem_wb_reg_write.read());
        mem_wb_result_out.write(mem_wb_result.read());
        mem_wb_pc_out.write(mem_wb_pc.read());

        if_id_pc_out.write(if_id_pc.read());
        id_ex_pc_out.write(id_ex_pc.read());
        ex_mem_pc_out.write(ex_mem_pc.read());
        mem_wb_pc_out.write(mem_wb_pc.read());
        wb_result_pc_out.write(wb_result_pc.read());
    }
}

#endif // PIPELINE_REGISTER_H