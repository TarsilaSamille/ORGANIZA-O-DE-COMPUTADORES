#ifndef PIPELINE_H
#define PIPELINE_H

#include <systemc.h>
#include "instruction_decoder.h"
#include "mips_memory.h"
#include "register_bank.h"
#include "ula.h"
#include "ifu.h"

enum PipelineState
{
  STATE_IF,
  STATE_ID,
  STATE_EX,
  STATE_MEM,
  STATE_WB,
  STATE_DONE
};

enum HazardType
{
  NO_HAZARD,
  DATA_HAZARD,
  CONTROL_HAZARD
};

SC_MODULE (Pipeline)
{
  sc_in < bool >clock;
  sc_in < bool >reset;
  sc_out < PipelineState > current_state;

  sc_signal < sc_uint < 32 >> instruction;
  sc_signal < sc_uint < 6 >> opcode;
  sc_signal < sc_uint < 5 >> rs;
  sc_signal < sc_uint < 5 >> rt;
  sc_signal < sc_uint < 5 >> rd;
  sc_signal < sc_uint < 16 >> immediate;
  sc_signal < sc_uint < 32 >> jump_address;
  sc_signal < sc_uint < 6 >> control_signals;

  sc_signal < sc_uint < 32 >> opA, opB;
  sc_signal < sc_uint < 4 >> aluOp;
  sc_signal < sc_uint < 32 >> result;
  sc_signal < bool >zero;

  sc_signal < sc_uint < 32 >> address;
  sc_signal < sc_uint < 32 >> data_in;
  sc_signal < bool >write_enable_memory;
  sc_signal < bool >read_enable;
  sc_signal < sc_uint < 32 >> data_out;

  sc_signal < sc_uint < 5 >> read_reg1;
  sc_signal < sc_uint < 5 >> read_reg2;
  sc_signal < sc_uint < 5 >> write_reg;
  sc_signal < sc_uint < 32 >> write_data;
  sc_signal < bool >write_enable;

  sc_signal < sc_uint < 32 >> decoder_instruction;
  sc_signal < sc_uint < 32 >> ula_opA;
  sc_signal < sc_uint < 32 >> ula_opB;
  sc_signal < sc_uint < 4 >> ula_aluOp;
  sc_signal < sc_uint < 32 >> ula_result;
  sc_signal < bool >ula_zero;
  sc_signal < sc_uint < 32 >> memory_address;
  sc_signal < sc_uint < 32 >> memory_data_in;
  sc_signal < bool >memory_write_enable;
  sc_signal < bool >memory_read_enable;
  sc_signal < sc_uint < 5 >> register_bank_write_reg;
  sc_signal < sc_uint < 32 >> register_bank_write_data;
  sc_signal < bool >register_bank_write_enable;

  sc_signal < sc_uint < 32 >> read_data1, read_data2;

  sc_signal < sc_uint < 32 >> pc;
  sc_signal < sc_uint < 32 >> address_ifu;
  sc_signal < bool >read_enable_ifu;

  // Hazard detection unit
  sc_signal < bool >stall;
  sc_signal < HazardType > hazard;

  Instruction_Decoder *decoder;
  MIPS_Memory *memory;
  Register_Bank *register_bank;
  ULA *ula;
  IFU *ifu;

  SC_CTOR (Pipeline)
  {
	SC_THREAD (transition);
	sensitive << clock.pos ();
	decoder = new Instruction_Decoder ("decoder");
	memory = new MIPS_Memory ("memory");
	register_bank = new Register_Bank ("register_bank");
	ula = new ULA ("ula");
	ifu = new IFU ("ifu", memory);

	// Connect signals
	connect_signals ();
  }

  void connect_signals ()
  {
	// Connect signals
	decoder->clk (clock);
	decoder->reset (reset);
	decoder->opcode (opcode);
	decoder->rs (rs);
	decoder->rt (rt);
	decoder->rd (rd);
	decoder->immediate (immediate);
	decoder->jump_address (jump_address);
	decoder->control_signals (control_signals);

	ula->clk (clock);
	ula->reset (reset);
	ula->opA (opA);
	ula->opB (opB);
	ula->aluOp (aluOp);
	ula->result (result);
	ula->zero (zero);

	memory->clk (clock);
	memory->reset (reset);
	memory->data_in (data_in);
	memory->write_enable (write_enable_memory);
	memory->data_out (data_out);

	register_bank->clk (clock);
	register_bank->reset (reset);
	register_bank->read_reg1 (read_reg1);
	register_bank->read_reg2 (read_reg2);
	register_bank->write_reg (write_reg);
	register_bank->write_data (write_data);
	register_bank->write_enable (write_enable);
	register_bank->read_data1 (read_data1);
	register_bank->read_data2 (read_data2);

	ifu->clock (clock);
	ifu->reset (reset);
	ifu->pc (pc);
	ifu->address (address_ifu);
	ifu->read_enable (read_enable_ifu);
	ifu->instruction (instruction);

	decoder_instruction = instruction;
	ula_opA = opA;
	ula_opB = opB;
	ula_aluOp = aluOp;
	ula_result = result;
	ula_zero = zero;
	memory_address = address;
	memory_data_in = data_in;
	memory_write_enable = write_enable_memory;
	memory_read_enable = read_enable;
	register_bank_write_reg = write_reg;
	register_bank_write_data = write_data;
	register_bank_write_enable = write_enable;

	memory->address.bind (address_ifu);
	memory->read_enable.bind (read_enable_ifu);
	ifu->data_out.bind (memory->data_out);
	decoder->instruction.bind (instruction);
  }

  void forwarding_unit ()
  {
	if (rs.read () == write_reg.read () && write_enable.read ())
	  {
		opA.write (write_data.read ());
	  }
	else if (rs.read () == register_bank_write_reg.read ()
			 && register_bank_write_enable.read ())
	  {
		opA.write (data_out.read ());
	  }
	else
	  {
		opA.write (register_bank->read_data1.read ());
	  }

	if (rt.read () == write_reg.read () && write_enable.read ())
	  {
		opB.write (write_data.read ());
	  }
	else if (rt.read () == register_bank_write_reg.read ()
			 && register_bank_write_enable.read ())
	  {
		opB.write (data_out.read ());
	  }
	else
	  {
		opB.write (register_bank->read_data2.read ());
	  }
  }
  sc_uint < 4 > determine_alu_op (sc_uint < 6 > opcode,
								  sc_uint < 3 > control_signals)
  {
	sc_uint < 4 > alu_op = 0;

	switch (opcode)
	  {
	  case 0x20:				// ADD
		alu_op = 0;				// ADD
		break;
	  case 0x22:				// SUB
		alu_op = 1;				// SUB
		break;
	  case 0x24:				// AND
		alu_op = 2;				// AND
		break;
	  case 0x25:				// OR
		alu_op = 3;				// OR
		break;
	  case 0x2A:				// SLT
		alu_op = 4;				// SLT
		break;
	  default:
		if (control_signals[0])
		  {						// Check if it's a load/store instruction
			alu_op = 5;			// ADD (for load/store addressing)
		  }
		else
		  {
			alu_op = 0;			// Default to ADD
		  }
		break;
	  }

	return alu_op;
  }

  void transition ()
  {
	while (true)
	  {
		wait ();				// Wait for clock edge

		if (reset.read ())
		  {
			current_state.write (STATE_IF);	// Reset to IF state
		  }
		else
		  {
			switch (current_state.read ())
			  {
			  case STATE_IF:
				std::cout << "STATE_IF" << std::endl;
				// Fetch instruction
				instruction.write (ifu->instruction.read ());
				std::cout << "Fetched Instruction: " << instruction.
				  read () << std::endl;
				current_state.write (STATE_ID);

				break;

			  case STATE_ID:
				std::cout << "STATE_ID" << std::endl;
				std::cout << "Decoded Instruction - Opcode: " << opcode.
				  read () << ", RS: " << rs.read () << ", RT: " << rt.
				  read () << ", RD: " << rd.
				  read () << ", Immediate: " << immediate.
				  read () << ", Jump Address: " << jump_address.
				  read () << std::endl;
				read_reg1.write (rs.read ());
				read_reg2.write (rt.read ());
				current_state.write (STATE_EX);
				break;

			  case STATE_EX:
				std::cout << "STATE_EX" << std::endl;
				// Execute instruction
				if (control_signals.read ()[0])
				  {				// Check if ALU operation is required
					aluOp.write (determine_alu_op (opcode.read (), control_signals.read ()));	// Determine ALU op based on opcode and control signals
					forwarding_unit ();
					ula->compute ();	// Execute ULA operation

					ula_result.write (result.read ());
					ula_zero.write (zero.read ());

					std::cout << "Executed ALU - Result: " << result.read ()
					  << ", Zero: " << zero.read () << std::endl;
				  }
				else
				  {
					// No ALU operation required, skip to next state
					current_state.write (STATE_MEM);
				  }
				break;
			  case STATE_MEM:
				std::cout << "STATE_MEM" << std::endl;
				// Access memory
				address.write (result.read ());
				write_enable_memory.write (control_signals.read ()[0]);
				read_enable.write (control_signals.read ()[1]);
				data_in.write (opB.read ());
				std::cout << "Memory Access - Address: " << address.read ()
				  << ", Data In: " << data_in.read ()
				  << ", Write Enable: " << write_enable_memory.read ()
				  << ", Read Enable: " << read_enable.read () << std::endl;
				current_state.write (STATE_WB);
				break;

			  case STATE_WB:
				std::cout << "STATE_WB" << std::endl;
				// Write-back to register file
				if (control_signals.read ()[2])
				  {				// Check if it's a write-back instruction
					write_reg.write (rd.read ());
					write_data.write (result.read ());
					write_enable.write (true);
					std::cout << "Write Back - Register: " << write_reg.
					  read () << ", Data: " << write_data.
					  read () << ", Write Enable: " << write_enable.
					  read () << std::endl;
				  }
				else
				  {
					write_enable.write (false);
				  }
				current_state.write (STATE_IF);

				register_bank->print_registers ();
				break;

			  default:
				current_state.write (STATE_DONE);
				break;
			  }
		  }
	  }
  }


};

#endif
