#include "systemc.h"
#include "sysc/utils/sc_vector.h"

SC_MODULE(register_pipeline_1)
{
    sc_in<sc_uint<32>> WbIn;
    sc_in<sc_uint<32>> MIn;
    sc_in<sc_uint<32>> ExIn;
    sc_in<sc_uint<32>> AdderIn;
    sc_in<sc_uint<32>> ReadDataIn;
    sc_in<sc_uint<32>> ReadData2In;
    sc_in<sc_uint<32>> SignExtIn;
    sc_in<sc_uint<5>> InstructionIn;
    sc_in<sc_uint<5>> Instruction2In;
    sc_in_clk clock;

    sc_out<sc_uint<32>> MOut;
    sc_out<sc_uint<32>> ExOut;
    sc_out<sc_uint<32>> AdderOut;
    sc_out<sc_uint<32>> ReadDataOut;
    sc_out<sc_uint<32>> ReadData2Out;
    sc_out<sc_uint<32>> SignExtOut;
    sc_out<sc_uint<5>> InstructionOut;
    sc_out<sc_uint<5>> Instruction2Out;
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
    MOut.write(MIn.read());
    ExOut.write(ExIn.read());
    AdderOut.write(AdderIn.read());
    ReadDataOut.write(ReadDataIn.read());
    ReadData2Out.write(ReadData2In.read());
    SignExtOut.write(SignExtIn.read());
    InstructionOut.write(InstructionInt.read());
    Instruction2Out.write(Instruction2Int.read());
    
}