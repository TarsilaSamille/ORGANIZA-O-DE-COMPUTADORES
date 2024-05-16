#include "pipeline.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <queue>
#include <bitset>

// Define constants for the pipeline states (for example)
#define STATE_IF 1
#define STATE_ID 2
#define STATE_EX 3
#define STATE_MEM 4
#define STATE_WB 0
#define STATE_DONE 5

int sc_main(int argc, char * argv[]) {
  // Declaration of signals
  sc_clock clock("clock", 10, SC_NS);
  sc_signal < bool > reset;
  sc_signal < PipelineState > current_state;
  sc_signal < sc_uint < 32 >> instruction;

  Pipeline * pipeline = new Pipeline("pipeline");

  pipeline -> clock(clock);
  pipeline -> reset(reset);
  pipeline -> current_state(current_state);

  std::queue < sc_uint < 32 >> instruction_queue;
  sc_signal < bool > instruction_ready;

  // Load instructions from file
  std::ifstream file("instructions.bin", std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Error: unable to open file 'instructions.bin'" << std::endl;
    return 1;
  }

  char buffer[4];
  while (file.read(buffer, 4)) {
    sc_uint < 32 > instr = 0;
    for (int i = 0; i < 4; i++) {
      instr.range((i + 1) * 8 - 1, i * 8) = static_cast < unsigned char > (buffer[i]);
    }

    instruction_queue.push(instr);
  }

  file.close();
  // Run simulation
  reset = true;
  sc_start(10, SC_NS);
  reset = false;

  int num_cycles = 50;
  int instruction_count = 0;
  while (instruction_count < instruction_queue.size() && num_cycles > 0) {
    pipeline -> ifu -> instruction.write(instruction_queue.front());
    instruction_queue.pop();
    instruction_count++;

    for (int cycle_count = 0; cycle_count < 5; cycle_count++) {
      sc_start(10, SC_NS);

      std::cout << "Cycle " << cycle_count << ":" << std::endl;
      std::cout << "  Current State = " << current_state.read() << std::endl;

      if (current_state.read() == STATE_DONE) {
        break;
      }

      if (current_state.read() == STATE_IF) {
        instruction_ready.write(true);
      }
    }

    num_cycles -= 10;
  }

  return 0;
}