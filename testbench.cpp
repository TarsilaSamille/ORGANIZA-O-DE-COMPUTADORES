#include <systemc.h>
#include "pipeline.h"

SC_MODULE(Testbench)
{
    // Signals
    sc_clock clk;
    sc_signal<bool> reset;
    // Other signals...
    // Connect signals...

    // Modules
    InstructionFetch if_stage;
    InstructionDecode id_stage;
    Execute ex_stage;
    MemoryAccess mem_stage;
    WriteBack wb_stage;
    ControlUnit control_unit;

    // Constructor
    SC_CTOR(Testbench) : clk("clk", 10, SC_NS),
                         if_stage("IF_Stage"),
                         id_stage("ID_Stage"),
                         ex_stage("EX_Stage"),
                         mem_stage("MEM_Stage"),
                         wb_stage("WB_Stage"),
                         control_unit("ControlUnit")
    {
        // Connect signals
        if_stage.clk(clk);
        if_stage.reset(reset);

        id_stage.clk(clk);
        id_stage.reset(reset);

        ex_stage.clk(clk);
        ex_stage.reset(reset);

        mem_stage.clk(clk);
        mem_stage.reset(reset);

        wb_stage.clk(clk);
        wb_stage.reset(reset);

        control_unit.clk(clk);
        control_unit.reset(reset);
    }

    // Other methods...
};

int sc_main(int argc, char *argv[])
{
    // Instantiate the testbench
    Testbench testbench("Testbench");

    // Start simulation
    sc_start();

    return 0;
}
