#include <systemc.h>
#include "mips_processor.h"
#include "pipeline_register.h"

SC_MODULE(MIPS_Testbench)
{
    // Instância do processador MIPS
    MIPS_Processor processor;

    // Sinais de controle
    sc_signal<bool> clock;
    sc_signal<bool> reset;

    // Sinais para conectar ao processador
    // Estes são apenas exemplos, você deve ajustar de acordo com a implementação real do seu processador
    sc_signal<sc_uint<32>> address;
    sc_signal<sc_uint<32>> data_in;
    sc_signal<bool> write_enable;
    sc_signal<bool> read_enable;
    sc_signal<sc_uint<32>> opA;
    sc_signal<sc_uint<32>> opB;
    sc_signal<sc_uint<4>> aluOp;
    sc_signal<sc_uint<5>> WriteAddress;
    sc_signal<sc_uint<32>> WriteData;
    sc_signal<bool> RegWrite;
    sc_signal<bool> MemWrite;

    // Construtor
    SC_CTOR(MIPS_Testbench) : processor("MIPS_Processor")
    {
        // Conectar sinais de controle
        processor.clock(clock);
        processor.reset(reset);

        // Conectar sinais ao processador
        // Aqui você deve conectar os sinais ao processador de acordo com a implementação real do seu processador
        // Este é apenas um exemplo, você deve ajustar de acordo com a implementação real do seu processador
        processor.address(address);
        processor.data_in(data_in);
        processor.write_enable(write_enable);
        processor.read_enable(read_enable);
        processor.opA(opA);
        processor.opB(opB);
        processor.aluOp(aluOp);
        processor.WriteAddress(WriteAddress);
        processor.WriteData(WriteData);
        processor.RegWrite(RegWrite);
        processor.MemWrite(MemWrite);

        // Iniciar o gerador de clock e o gerador de reset
        SC_THREAD(clock_generator);
        SC_THREAD(reset_generator);

        // Iniciar o teste
        SC_THREAD(run_test);
    }

    // Método para gerar sinal de clock
    void clock_generator()
    {
        while (true)
        {
            clock.write(true);
            wait(5, SC_NS);
            clock.write(false);
            wait(5, SC_NS);
        }
    }

    // Método para gerar sinal de reset
    void reset_generator()
    {
        reset.write(true);
        wait(10, SC_NS);
        reset.write(false);
    }

    // Método para executar o teste
    void run_test()
    {
        // Esperar o reset ser concluído
        wait(15, SC_NS);

        // Aqui você pode adicionar o código para testar o processador MIPS
        // Por exemplo, você pode escrever instruções de teste nas entradas do processador
        // e verificar os resultados nas saídas do processador.

        // Fim do teste
        sc_stop();
    }
};

int sc_main(int argc, char *argv[])
{
    MIPS_Testbench testbench("MIPS_Testbench");

    // Iniciar a simulação
    cout << "Iniciando simulação..." << endl;
    sc_start();

    cout << "Simulação concluída." << endl;

    return 0;
}