#include <systemc.h>
#include "register_bank.h"

int sc_main(int argc, char *argv[])
{
    // Instanciando módulo de banco de registradores
    Registers_Bank reg_bank("RegisterBank");

    // Definindo sinais
    sc_signal<sc_uint<32>> LoadAddress1, LoadAddress2;
    sc_signal<sc_uint<5>> WriteAddress;
    sc_signal<sc_uint<32>> WriteData;
    sc_signal<bool> RegWrite, MemWrite;
    sc_signal<sc_uint<32>> DataOut1, DataOut2;
    sc_clock clock("clock", 1, SC_NS);

    // Conectando sinais ao módulo de banco de registradores
    reg_bank.LoadAddress1(LoadAddress1);
    reg_bank.LoadAddress2(LoadAddress2);
    reg_bank.WriteAddress(WriteAddress);
    reg_bank.WriteData(WriteData);
    reg_bank.RegWrite(RegWrite);
    reg_bank.MemWrite(MemWrite);
    reg_bank.DataOut1(DataOut1);
    reg_bank.DataOut2(DataOut2);
    reg_bank.clock(clock);

    // Caso de teste 1: Escrever valores diferentes em todos os registradores.
    for (int i = 0; i < 32; ++i)
    {
        WriteAddress = i;
        WriteData = i * 10 + 5;
        RegWrite = true;
        MemWrite = true;
        sc_start(1, SC_NS);
    }

    // Caso de teste 2: Ler os valores de todos os registradores após a escrita.
    for (int i = 0; i < 32; ++i)
    {
        LoadAddress1 = i;
        sc_start(1, SC_NS);
        cout << "Valor do registrador " << i << ": " << DataOut1.read() << endl;
    }

    // Caso de teste 3: Escrever em um registrador e verificar se o valor foi escrito corretamente.
    WriteAddress = 10;
    WriteData = 12345;
    RegWrite = true;
    MemWrite = true;
    sc_start(1, SC_NS);

    LoadAddress1 = 10;
    sc_start(1, SC_NS);
    cout << "Valor do registrador 10: " << DataOut1.read() << endl;

    // Caso de teste 4: Tentar escrever em um endereço de registrador inválido e verificar se um erro é gerado.
    WriteAddress = 50; // Endereço inválido
    WriteData = 999;
    RegWrite = true;
    MemWrite = true;
    sc_start(1, SC_NS);

    // Caso de teste 5: Tentar ler de um endereço de registrador inválido e verificar se um erro é gerado.
    LoadAddress1 = 50; // Endereço inválido
    sc_start(1, SC_NS);

    // Caso de teste 6: Escrever em um registrador e ler o valor de outro registrador para garantir que não houve interferência.
    WriteAddress = 20;
    WriteData = 555;
    RegWrite = true;
    MemWrite = true;
    sc_start(1, SC_NS);

    LoadAddress1 = 10; // Tentando ler de um registrador diferente
    sc_start(1, SC_NS);
    cout << "Valor do registrador 10: " << DataOut1.read() << endl;

    // Caso de teste 7: Testar a leitura de valores quando o sinal de leitura (MemWrite) está desativado.
    MemWrite = false;
    LoadAddress1 = 5;
    sc_start(1, SC_NS);
    cout << "Valor do registrador 5 (com MemWrite desativado): " << DataOut1.read() << endl;

    return 0;
}
