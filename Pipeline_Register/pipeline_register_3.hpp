#include "systemc.h"
#include "sysc/utils/sc_vector.h"

SC_MODULE(register_pipeline_1)
{
    sc_in<sc_uint<32>> WbIn;
    sc_in<sc_uint<32>> MIn;
    sc_in<sc_uint<32>> AddIn;
    sc_in<sc_uint<32>> ZenIn;
    sc_in<sc_uint<32>> UlaIn;
    sc_in<sc_uint<32>> MuxIn;
    sc_in<sc_uint<32>> Mux2In;
    sc_in_clk clock;

    sc_out<sc_uint<32>> WbOut;
    sc_out<sc_uint<32>> MOut;
    sc_out<sc_uint<32>> AddOut;
    sc_out<sc_uint<32>> ZenOut;
    sc_out<sc_uint<32>> UlaOut;
    sc_out<sc_uint<32>> MuxOut;
    sc_out<sc_uint<32>> Mux2Out;
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
    WbIn.write(WbOut.read());
    MIn.write(MOut.read());
    AddIn.write(AddOut.read());
    ZenIn.write(ZenOut.read());
    UlaIn.write(UlaOut.read());
    MuxIn.write(MuxOut.read());
    Mux2In.write(Mux2Out.read());

}