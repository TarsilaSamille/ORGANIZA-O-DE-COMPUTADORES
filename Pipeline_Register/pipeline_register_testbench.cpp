#include <systemc.h>
#include "pipeline_register.h"

SC_MODULE(Testbench)
{
    sc_clock clock;
    sc_signal<bool> reset;
    sc_signal<sc_uint<32>> if_id_instr_in;
    sc_signal<sc_uint<32>> id_ex_instr_in;
    sc_signal<sc_uint<32>> ex_mem_instr_in;
    sc_signal<sc_uint<32>> mem_wb_instr_in;
    sc_signal<sc_uint<32>> wb_result_in;
    sc_signal<sc_uint<32>> if_id_instr_out;
    sc_signal<sc_uint<32>> id_ex_instr_out;
    sc_signal<sc_uint<32>> ex_mem_instr_out;
    sc_signal<sc_uint<32>> mem_wb_instr_out;
    sc_signal<sc_uint<32>> wb_result_out;

    Pipeline_Register pipeline_reg;

    SC_CTOR(Testbench) : clock("clock", 10, SC_NS), pipeline_reg("PipelineRegister")
    {
        pipeline_reg.clock(clock);
        pipeline_reg.reset(reset);
        pipeline_reg.if_id_instr_in(if_id_instr_in);
        pipeline_reg.id_ex_instr_in(id_ex_instr_in);
        pipeline_reg.ex_mem_instr_in(ex_mem_instr_in);
        pipeline_reg.mem_wb_instr_in(mem_wb_instr_in);
        pipeline_reg.wb_result_in(wb_result_in);
        pipeline_reg.if_id_instr_out(if_id_instr_out);
        pipeline_reg.id_ex_instr_out(id_ex_instr_out);
        pipeline_reg.ex_mem_instr_out(ex_mem_instr_out);
        pipeline_reg.mem_wb_instr_out(mem_wb_instr_out);
        pipeline_reg.wb_result_out(wb_result_out);

        SC_THREAD(generate_reset);

        SC_THREAD(simulate_instructions);
        sensitive << clock.posedge_event();
    }

    void generate_reset()
    {
        reset.write(true);
        wait(5, SC_NS);
        reset.write(false);
    }

    void simulate_instructions()
    {
        wait(10, SC_NS);

        if_id_instr_in.write(1);
        wait(10, SC_NS);
        if_id_instr_in.write(0);

        for (int i = 0; i < 5; ++i)
        {
            if_id_instr_in.write(i + 1);
            wait(10, SC_NS);
            if_id_instr_in.write(0);
            print_output();
        }
    }

    void print_output()
    {
        cout << "[TESTBENCH] Output signals:" << endl;
        cout << "if_id_instr_out: " << if_id_instr_out.read() << endl;
        cout << "id_ex_instr_out: " << id_ex_instr_out.read() << endl;
        cout << "ex_mem_instr_out: " << ex_mem_instr_out.read() << endl;
        cout << "mem_wb_instr_out: " << mem_wb_instr_out.read() << endl;
        cout << "wb_result_out: " << wb_result_out.read() << endl;
    }
};

int sc_main(int argc, char *argv[])
{
    Testbench testbench("Testbench");
    cout << "[MAIN] Starting simulation..." << endl;
    sc_start(100, SC_NS);
    testbench.print_output();
    cout << "[MAIN] Simulation completed." << endl;

    return 0;
}
