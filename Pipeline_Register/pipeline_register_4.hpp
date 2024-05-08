#include "systemc.h"
#include "sysc/utils/sc_vector.h"

SC_MODULE(register_pipeline_1)
{
    sc_in<sc_uint<32>> WbIn;
    sc_in<sc_uint<32>> ReadDataIn;
    sc_in<sc_uint<32>> MenIn;
    sc_in<sc_uint<32>> MuxoutIn;
    sc_in_clk clock;

    sc_out<sc_uint<32>> WbOut;
    sc_out<sc_uint<32>> ReadDataOut;
    sc_out<sc_uint<32>> MenOut;
    sc_out<sc_uint<32>> MuxoutOut;
    // methods
    void pass_instruction();

    SC_CTOR(register_pipeline_1)
    {
        SC_METHOD(pass_instruction);
        sensitive << clock.pos();
    }
};

void register_pipeline_1::pass_instruction()
{
    WbOut.write(WbIn.read());
    MenOut.write(MenIn.read());
    ReadDataOut.write(ReadDataIn.read());
    MuxoutOut.write(MuxoutIn.read());
    
}