## DIM0129 - ORGANIZAÇÃO DE COMPUTADORES - Trabalho 1

Este código em SystemC implementa um simulador básico de processador com pipeline de cinco estágios. Ele inclui um módulo de memória e um módulo de processador. Aqui está uma breve explicação do código:

Memória (Memory): Este módulo simula uma memória simples que pode ser lida e escrita. Ele possui portas de entrada/saída para sinal de clock, sinal de reset, sinal de leitura (read_en), sinal de escrita (write_en), endereço (address), dados de entrada (data_in) e dados de saída (data_out). A memória é implementada como uma matriz de 1024 elementos.

Processador (Processor): Este módulo simula um processador com pipeline de cinco estágios: fetch, decode, execute, memory e writeback. Ele possui portas de entrada/saída para sinal de clock, sinal de reset, instrução de entrada (instr_in), operandos de entrada (operand1_in e operand2_in) e resultado de saída (result_out). Além disso, ele inclui sinais internos para os estágios do pipeline, registros de controle e registradores de status.

O código principal (sc_main) instancia o processador, conecta os sinais e inicia a simulação. Durante a simulação, o código define uma instrução e operandos iniciais, executa a simulação por um curto período de tempo, altera a instrução e operandos e executa a simulação novamente. Por fim, exibe o resultado da operação.

Arquitetura do Processador:

Decisões de projeto:
1- **Tamanho da palavra do processador:**

- O tamanho da palavra do processador é definido como 32 bits (`sc_uint<32>`), o que é comum em arquiteturas de processadores RISC modernas.

2- **Formato da palavra de instrução:**

- Cada instrução é representada por um enum `Instructions`, o que simplifica o formato da palavra de instrução para apenas um campo, facilitando a decodificação.

3- **Modos de endereçamento de operandos:**

- Os operandos são endereçados diretamente como registradores. Isso significa que os operandos são lidos diretamente dos registradores especificados pela instrução.

4- **Tamanho do banco de registradores:**

- ?
  5- **Tamanho das memórias de instruções e de dados:**
- ?
  6- **Número e tipos de barramentos da parte operativa:**
- ?

7- **Organização do pipeline:**

- O pipeline foi implementado com cinco estágios:
  1.  **Fetch:** Busca a próxima instrução.
  2.  **Decode:** Decodifica a instrução e lê os operandos.
  3.  **Execute:** Executa a instrução.
  4.  **Memory:** Acesso à memória (leitura ou escrita).
  5.  **Writeback:** Escreve o resultado de volta nos registradores e realiza salto, se necessário.

TO-DO

- **Resultados de Simulações**:

  - Executar simulações da execução de instruções de pelo menos três algoritmos na arquitetura do processador.
  - Apresentar os resultados das simulações em forma de diagramas de forma de onda ou textualmente.
  - Incluir os resultados das simulações no relatório e também compartilhá-los em sala de aula.

- **Implementação**:

  - Terminar decisões de projeto
  - Certificar-se de implementar tanto a parte de controle quanto a parte operativa do processador conforme descrito nas decisões de projeto.
  - Implementar os seguintes componentes do processador qu faltam:
    - Registradores
    - Barramentos
    - Memória de instruções
    - Barramento de controle
    - Outros componentes necessários para completar o processador RISC, conforme as decisões de projeto.

- **Testes e Simulações**:

  - Após implementar todos os componentes do processador, realizar testes e simulações para verificar se o processador está funcionando conforme o esperado.
  - Executar instruções de teste em diferentes cenários para garantir que o processador esteja operando corretamente.
  - Documentar os testes realizados e os resultados obtidos para referência futura.

- **Relatório:**

- **Apresentação em Sala de Aula:**
  - Preparar slides ou material visual para mostrar os diagramas de bloco do processador.

## Outros

mac

- https://www.youtube.com/watch?v=Oo96VxAqV2E&ab_channel=DillonHuff

- https://www.learnsystemc.com/basic/trace

## To run

clang++ a.cpp -std=c++17 -I/systemc-2.3/include -lsystemc -L/systemc-2.3/lib-macosx386 -o a
./a

clang++ ab.cpp -std=c++17 -I/systemc-2.3/include -lsystemc -L/systemc-2.3/lib-macosx386 -o ab
./ab

git add .
git commit -m "."
git push -u origin main

# ORGANIZA-O-DE-COMPUTADORES

clang++ ./ULA/testbench_ula.cpp -std=c++17 -I/systemc-2.3/include -lsystemc -L/systemc-2.3/lib-macosx386 -o ./ULA/testbench_ula
./ULA/testbench_ula

clang++ ./Registry_Bank/register_bank_tests.cpp -std=c++17 -I/systemc-2.3/include -lsystemc -L/systemc-2.3/lib-macosx386 -o ./Registry_Bank/register_bank_tests
./Registry_Bank/register_bank_tests

clang++ ./Memory/mips_memory_testbench.cpp -std=c++17 -I/systemc-2.3/include -lsystemc -L/systemc-2.3/lib-macosx386 -o ./Memory/mips_memory_testbench
./Memory/mips_memory_testbench

clang++ ./Instruction_Decoder/instruction_decoder_testbench.cpp -std=c++17 -I/systemc-2.3/include -lsystemc -L/systemc-2.3/lib-macosx386 -o ./Instruction_Decoder/instruction_decoder_testbench
./Instruction_Decoder/instruction_decoder_testbench

clang++ ./Pipeline_Register/pipeline_register_testbench
.cpp -std=c++17 -I/systemc-2.3/include -lsystemc -L/systemc-2.3/lib-macosx386 -o ./Pipeline_Register/pipeline_register_testbench
./Pipeline_Register/pipeline_register_testbench

clang++ ./mips_testbench.cpp -std=c++17 -I./Memory -I./Pipeline_Register -I./Instruction_Decoder -I./Registry_Bank -I./ULA -I/systemc-2.3/include -lsystemc -L/systemc-2.3/lib-macosx386 -o ./mips_testbench
./mips_testbench

clang++ ./pipeline.cpp -std=c++17 -I./Memory -I./Pipeline_Register -I./Instruction_Decoder -I./Registry_Bank -I./ULA -I./Mips_Instructions -I/systemc-2.3/include -lsystemc -L/systemc-2.3/lib-macosx386 -o ./pipeline
./pipeline

clang++ ./mips_example.cpp -std=c++17 -I/systemc-2.3/include -lsystemc -L/systemc-2.3/lib-macosx386 -o ./mips_example
./mips_example

clang++ ./pipeline.cpp -std=c++17 \
 -I./Memory \
 -I./Pipeline_Register \
 -I./Instruction_Decoder \
 -I./Registry_Bank \
 -I./ULA \
 -I./Mips_Instructions \
 -I/systemc-2.3/include \
 -lsystemc -L/systemc-2.3/lib-macosx386 \
 -o ./pipeline
./pipeline

clang++ ./pipeline.cpp -std=c++17 \
 -I./new \
 -I/systemc-2.3/include \
 -lsystemc -L/systemc-2.3/lib-macosx386 \
 -o ./pipeline
./pipeline

clang++ -c pipeline.cpp -std=c++17 -I./Memory -I./Pipeline_Register -I./Instruction_Decoder -I./Registry_Bank -I./ULA -I./Mips_Instructions -I/systemc-2.3/include

clang++ ./main.cpp -std=c++17 \
 -I./Memory \
 -I./Pipeline_Register \
 -I./Instruction_Decoder \
 -I./Registry_Bank \
 -I./ULA \
 -I./Mips_Instructions \
 -I./systemc-2.3/include \
 -lsystemc -L./systemc-2.3/lib-macosx386 \
 -o ./main
./main

clang++ ./alg.cpp ./pipeline.cpp -std=c++17 \
 -I./Memory \
 -I./Pipeline_Register \
 -I./Instruction_Decoder \
 -I./Registry_Bank \
 -I./ULA \
 -I./Mips_Instructions \
 -I./systemc-2.3/include \
 -lsystemc -L./systemc-2.3/lib-macosx386 \
 -o ./alg
./alg

Instruction Fetch Unit (IFU): You need to implement the IFU, which fetches instructions from memory and stores them in the instruction signal. You can use a simple counter to increment the program counter (PC) and fetch the next instruction.
Register Bank connections: You need to connect the read_reg1 and read_reg2 signals from the Register_Bank module to the opA and opB signals, respectively. This will allow the register bank to provide the operands for the ALU.
ULA connections: You need to connect the result signal from the ULA module to the write_data signal, which will store the result of the ALU operation in the register bank.
Memory connections: You need to connect the address signal from the Pipeline module to the address signal of the MIPS_Memory module. This will allow the pipeline to access memory.
Hazard detection and stall logic: You need to implement the hazard detection unit and stall logic to handle data hazards and control hazards. This will involve checking for dependencies between instructions and stalling the pipeline when necessary.
Forwarding unit: You need to implement the forwarding unit, which will forward the result of an instruction to the next instruction that uses it as an operand.
Testbench: You need to create a testbench to verify the correctness of your pipeline simulator. This can involve writing test cases to exercise different instructions and scenarios.

clang++ ./main.cpp -std=c++17 \
 -I./Memory \
 -I./Pipeline_Register \
 -I./Instruction_Decoder \
 -I./Registry_Bank \
 -I./ULA \
 -I./IFU \
 -I./Mips_Instructions \
 -I./systemc-2.3/include \
 -lsystemc -L./systemc-2.3/lib-macosx386 \
 -o ./main
 ./main