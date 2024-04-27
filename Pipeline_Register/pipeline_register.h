// Pipeline_Register.h
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

    // Outputs
    sc_out<sc_uint<32>> if_id_instr_out;
    sc_out<sc_uint<32>> id_ex_instr_out;
    sc_out<sc_uint<32>> ex_mem_instr_out;
    sc_out<sc_uint<32>> mem_wb_instr_out;
    sc_out<sc_uint<32>> wb_result_out;

    // Construtor
    SC_CTOR(Pipeline_Register)
    {
        SC_METHOD(register_update);
        sensitive << clock.pos() << reset;
    }

    // Método para atualizar os registradores de pipeline
    void register_update()
    {
        if (reset.read())
        {
            // Reset all registers to zero
            if_id_instr_out.write(0);
            id_ex_instr_out.write(0);
            ex_mem_instr_out.write(0);
            mem_wb_instr_out.write(0);
            wb_result_out.write(0);
        }
        else
        {
            // Update registers with input values
            if_id_instr_out.write(if_id_instr_in.read());
            id_ex_instr_out.write(id_ex_instr_in.read());
            ex_mem_instr_out.write(ex_mem_instr_in.read());
            mem_wb_instr_out.write(mem_wb_instr_in.read());
            wb_result_out.write(wb_result_in.read());
        }
    }
};

#endif // PIPELINE_REGISTER_H
