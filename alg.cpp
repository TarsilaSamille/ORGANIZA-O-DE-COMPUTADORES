#include "pipeline.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// MIPS processor state
struct MIPS_State {
    int PC; // program counter
    int registers[32]; // registers
};
// Function to read instructions from file
void readInstructions(string filename, vector<string> &instructions)
{
    ifstream file(filename);
    string line;
    while (getline(file, line))
    {
        instructions.push_back(line);
    }
    file.close();
}

// Function to convert string to binary instruction
string stringToBinary(string str)
{
    string binary;
    for (char c : str)
    {
        if (c == '0')
        {
            binary += "0000";
        }
        else if (c == '1')
        {
            binary += "0001";
        }
        else
        {
            cerr << "Error: invalid character in instruction: " << c << endl;
            return "";
        }
    }
    return binary;
}

// Function to execute instructions
void executeInstructions(vector<string> &instructions, MIPS_State &state)
{
    for (string instr : instructions)
    {
        string binaryInstr = stringToBinary(instr);
        // Decode instruction
        int opcode = stoi(binaryInstr.substr(0, 6), 0, 2);
        int rs = stoi(binaryInstr.substr(6, 5), 0, 2);
        int rt = stoi(binaryInstr.substr(11, 5), 0, 2);
        int rd = stoi(binaryInstr.substr(16, 5), 0, 2);
        int immediate = stoi(binaryInstr.substr(21, 16), 0, 2);

        // Execute instruction
        switch (opcode)
        {
        case 0x00: // add
            state.registers[rd] = state.registers[rs] + state.registers[rt];
            break;
        case 0x01: // sub
            state.registers[rd] = state.registers[rs] - state.registers[rt];
            break;
        case 0x02: // load
            state.registers[rt] = immediate;
            break;
        case 0x03: // store
            // store data from register into memory
            break;
        case 0x04: // jump
            state.PC = immediate;
            break;
        default:
            cerr << "Error: invalid opcode: " << opcode << endl;
            return;
        }

        // Update PC
        state.PC += 4;
    }
}

int main()
{
    vector<string> instructions;
    readInstructions("instructions.txt", instructions);

    MIPS_State state;
    state.PC = 0;
    for (int i = 0; i < 32; i++)
    {
        state.registers[i] = 0;
    }

    executeInstructions(instructions, state);

    // Print final processor state
    cout << "Final processor state:" << endl;
    cout << "PC: " << state.PC << endl;
    for (int i = 0; i < 32; i++)
    {
        cout << "R" << i << ": " << state.registers[i] << endl;
    }

    return 0;
}
