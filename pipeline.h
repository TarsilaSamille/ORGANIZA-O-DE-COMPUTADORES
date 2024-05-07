#ifndef PIPELINE_H
#define PIPELINE_H

#include <systemc.h>
#include "instruction_decoder.h"
#include "mips_memory.h"
#include "register_bank.h"
#include "ula.h"

enum PipelineState
{
    STATE_IF,
    STATE_ID,
    STATE_EX,
    STATE_MEM,
    STATE_WB,
    STATE_DONE
};

SC_MODULE(Pipeline)
{
    sc_in<bool> clock;
    sc_in<bool> reset;
    sc_out<PipelineState> current_state;

    // Variáveis de controle do pipeline
    PipelineState state;
    bool stall;

    // Instruction Decoder module
    Instruction_Decoder *decoder;

    // MIPS Memory module
    MIPS_Memory *memory;

    // Register Bank module
    Register_Bank *register_bank;

    // ULA module
    ULA *ula;

    // Função de transição de estado
    void transition();

    SC_CTOR(Pipeline, Instruction_Decoder *decoder, MIPS_Memory *memory, Register_Bank *register_bank, ULA *ula);
};

#endif // PIPELINE_H