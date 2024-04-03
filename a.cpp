#include <systemc.h>
#include <iostream>

// Definindo as instruções suportadas
enum Instructions
{
    AND,
    OR,
    XOR,
    NOT,
    CMP,
    ADD,
    SUB,
    LD,
    ST,
    J,
    JN,
    JZ
};

// Definindo o tipo de dado para os registradores
typedef sc_uint<32> RegType;

SC_MODULE(Memory)
{
    sc_in_clk clk;
    sc_in<bool> rst;

    // Portas de entrada/saída
    sc_in<bool> read_en, write_en;
    sc_in<sc_uint<32>> address, data_in;
    sc_out<sc_uint<32>> data_out;

    // Memória interna
    sc_uint<32> mem[1024]; // Tamanho arbitrário de memória

    // Construtor do módulo
    SC_CTOR(Memory)
    {
        SC_CTHREAD(read_write, clk.pos());
        reset_signal_is(rst, true);
    }

    // Método para ler e escrever dados na memória
    void read_write()
    {
        while (true)
        {
            wait(); // Aguarda o sinal de clock

            if (!rst.read())
            {
                // Verifica se a leitura na memória está habilitada
                if (read_en.read())
                {
                    // Lê os dados da memória no endereço fornecido
                    data_out.write(mem[address.read()]);
                }
                // Verifica se a escrita na memória está habilitada
                else if (write_en.read())
                {
                    // Escreve os dados na memória no endereço fornecido
                    mem[address.read()] = data_in.read();
                }
            }
        }
    }
};

SC_MODULE(Processor)
{
    // Portas de entrada/saída
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_in<Instructions> instr_in;
    sc_in<RegType> operand1_in, operand2_in;
    sc_out<RegType> result_out;

    // Registradores internos
    sc_signal<Instructions> instr;
    sc_signal<RegType> operand1, operand2, result;

    // Pipeline estágios
    sc_signal<Instructions> instr_fetch, instr_decode, instr_execute, instr_memory, instr_writeback;

    // Registrador para o endereço do salto
    sc_signal<sc_uint<32>> jump_address;

    // Registrador de status
    sc_signal<bool> zero, negative;

    // Registrador para dados da memória
    sc_signal<RegType> memory_data;

    // Flags de controle para leitura e escrita na memória
    sc_signal<bool> memory_read_en, memory_write_en;

    // Endereço e dados da memória
    sc_signal<sc_uint<32>> memory_address;

    // Construtor do módulo
    SC_CTOR(Processor)
    {
        // Módulos para os estágios do pipeline
        SC_CTHREAD(fetch, clk.pos());
        reset_signal_is(rst, true);

        SC_CTHREAD(decode, clk.pos());
        reset_signal_is(rst, true);

        SC_CTHREAD(execute, clk.pos());
        reset_signal_is(rst, true);

        SC_CTHREAD(memory, clk.pos());
        reset_signal_is(rst, true);

        SC_CTHREAD(writeback, clk.pos());
        reset_signal_is(rst, true);
    }

    // Método para buscar a próxima instrução
    void fetch()
    {
        while (true)
        {
            wait(); // Aguarda o sinal de clock

            if (!rst.read())
            {
                instr_fetch = instr_in.read();
            }
        }
    }

    // Método para decodificar a instrução
    void decode()
    {
        while (true)
        {
            wait(); // Aguarda o sinal de clock

            if (!rst.read())
            {
                instr_decode = instr_fetch.read();
                operand1 = operand1_in.read();
                operand2 = operand2_in.read();
            }
        }
    }

    // Método para executar a instrução
    void execute()
    {
        while (true)
        {
            wait(); // Aguarda o sinal de clock

            if (!rst.read())
            {
                // Executa a instrução atual
                switch (instr_decode.read())
                {
                case AND:
                    result = operand1.read() & operand2.read();
                    break;
                case OR:
                    result = operand1.read() | operand2.read();
                    break;
                case XOR:
                    result = operand1.read() ^ operand2.read();
                    break;
                case NOT:
                    result = ~operand1.read();
                    break;
                case CMP:
                    result = operand1.read() - operand2.read();
                    // Define os sinais de status
                    zero = (result.read() == 0);
                    negative = (result.read() < 0);
                    break;
                case ADD:
                    result = operand1.read() + operand2.read();
                    break;
                case SUB:
                    result = operand1.read() - operand2.read();
                    break;
                case LD:
                    instr_memory = instr_decode.read();
                    break;
                case ST:
                    instr_memory = instr_decode.read();
                    break;
                case J:
                    // Ativa o sinal de jump_address para realizar o salto
                    jump_address.write(operand1.read());
                    break;
                case JN:
                    if (negative.read())
                    {
                        // Ativa o sinal de jump_address para realizar o salto
                        jump_address.write(operand1.read());
                    }
                    break;
                case JZ:
                    if (zero.read())
                    {
                        // Ativa o sinal de jump_address para realizar o salto
                        jump_address.write(operand1.read());
                    }
                    break;
                default:
                    break;
                }
                instr_execute = instr_decode.read();
            }
        }
    }

    // Método para acessar a memória
    void memory()
    {
        while (true)
        {
            wait(); // Aguarda o sinal de clock

            if (!rst.read())
            {
                // Verifica se a instrução atual requer acesso à memória
                if (instr_memory.read() == LD)
                {
                    // Ativa o sinal de leitura na memória
                    memory_read_en.write(true);
                    // Define o endereço de leitura na memória
                    memory_address.write(operand2.read());
                }
                else if (instr_memory.read() == ST)
                {
                    // Ativa o sinal de escrita na memória
                    memory_write_en.write(true);
                    // Define o endereço de escrita na memória
                    memory_address.write(operand2.read());
                    // Define os dados a serem escritos na memória
                    memory_data.write(operand1.read());
                }
                else
                {
                    // Desativa tanto a leitura quanto a escrita na memória
                    memory_read_en.write(false);
                    memory_write_en.write(false);
                }
            }
        }
    }

    // Método para escrever o resultado
    void writeback()
    {
        while (true)
        {
            wait(); // Aguarda o sinal de clock

            if (!rst.read())
            {
                // Escreve o resultado de volta no estágio de writeback
                result_out.write(result.read());
                // Realiza o salto se necessário
                if (instr_execute.read() == J || instr_execute.read() == JN || instr_execute.read() == JZ)
                {
                    instr_fetch = J;
                }
            }
        }
    }
};
int sc_main(int argc, char *argv[])
{
    // Criação do clock e do sinal de reset
    sc_clock clk("clk", 10, SC_NS); // Clock de 10 ns
    sc_signal<bool> rst;

    // Criação dos sinais de entrada para a instrução e operandos
    sc_signal<Instructions> instr;
    sc_signal<RegType> operand1, operand2;

    // Criação do sinal de saída para o resultado
    sc_signal<RegType> result;

    // Instanciação do processador
    Processor processor("processor");
    processor.clk(clk);
    processor.rst(rst);
    processor.instr_in(instr);
    processor.operand1_in(operand1);
    processor.operand2_in(operand2);
    processor.result_out(result);

    // Inicialização dos sinais de entrada
    instr = AND;  // Definindo a instrução como AND
    operand1 = 5; // Definindo o operando 1 como 5 (por exemplo)
    operand2 = 3; // Definindo o operando 2 como 3 (por exemplo)
    rst = 0;      // Mantendo o sinal de reset baixo

    // Simulação de execução do algoritmo
    sc_start(10, SC_NS); // Aguarda 10 ns
    instr = ADD;         // Alterando a instrução para ADD
    operand1 = 7;        // Alterando o operando 1 para 7 (por exemplo)
    operand2 = 2;        // Alterando o operando 2 para 2 (por exemplo)
    sc_start(10, SC_NS); // Aguarda mais 10 ns

    // Exibindo o resultado após a simulação
    cout << "Resultado da operação: " << result.read() << endl;

    // Encerrando a simulação
    sc_stop();

    return 0;
}
