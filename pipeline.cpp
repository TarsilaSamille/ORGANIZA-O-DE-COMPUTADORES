#include "pipeline.h"

void Pipeline::transition()
{
   while (true)
    {
        wait(); // Wait for clock edge

        // Transition to next state
        if (reset.read())
        {
            state = STATE_IF; // Reset to IF state
        }
        else
        {
            switch (state)
            {
            case STATE_IF:
                state = STATE_ID;
                break;
            case STATE_ID:
                state = STATE_EX;
                break;
            case STATE_EX:
                state = STATE_MEM;
                break;
            case STATE_MEM:
                state = STATE_WB;
                break;
            case STATE_WB:
                state = STATE_IF;
                break;
            default:
                state = STATE_IF;
                break;
            }
        }

        // Update current state
        current_state.write(state);
    }
}

Pipeline::SC_CTOR(Pipeline, Instruction_Decoder *decoder, MIPS_Memory *memory, Register_Bank *register_bank, ULA *ula)
{
    // Inicialização do estado
    state = STATE_IF;
    stall = false;

    // Instanciação do Instruction Decoder
    this->decoder = decoder;

    // Instanciação do MIPS Memory
    this->memory = memory;

    // Instanciação do Register Bank
    this->register_bank = register_bank;

    // Instanciação do ULA
    this->ula = ula;

    // Processo de transição de estado
    SC_THREAD(transition);
    sensitive << clock.pos();
}