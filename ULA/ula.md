# Documentação: Implementação da ULA em SystemC

## Introdução

Este documento descreve a implementação de uma Unidade Lógica e Aritmética (ULA) em SystemC, juntamente com um teste bench para verificar sua funcionalidade. A ULA é uma parte fundamental de qualquer processador, responsável por executar operações lógicas e aritméticas em dados.

## ULA em SystemC

A ULA foi implementada como um módulo SystemC chamado `ULA`. Este módulo inclui entradas para os operandos (opA e opB), uma entrada para o código de operação da ULA (aluOp), uma saída para o resultado da operação (result) e uma saída para o sinal de zero (zero).

### Funcionalidades Implementadas:

1. **Operações Aritméticas**: Adição (ADD) e Subtração (SUB).
2. **Operações Lógicas**: AND, OR e XOR.
3. **Operações de Shift**: Shift Left Logical (SLL), Shift Right Logical (SRL) e Shift Right Arithmetic (SRA).
4. **Multiplicação (MULT)**: Multiplica dois números inteiros de 32 bits.
5. **Divisão (DIV)**: Divide dois números inteiros de 32 bits.

### Estrutura do Código Fonte:

O código fonte da ULA está dividido em dois arquivos:

- `ula.h`: Contém a declaração do módulo `ULA` em SystemC.
- `testbench.cpp`: Implementa um teste bench para verificar o funcionamento da ULA.

## Teste Bench

O teste bench foi desenvolvido para testar as diferentes operações da ULA e verificar se os resultados estão corretos.

### Casos de Teste:

O teste bench inclui casos de teste para todas as operações suportadas pela ULA. Cada caso de teste inclui a configuração dos operandos e do código de operação, seguido pela verificação do resultado e do sinal de zero.

## Compilação e Execução

Para compilar e executar o código, siga estas etapas:

1. Certifique-se de ter o SystemC instalado e configurado em seu sistema.
2. Abra um terminal e navegue até o diretório onde os arquivos estão localizados.
3. Compile os arquivos usando um compilador C++ e inclua as bibliotecas do SystemC.
4. Execute o teste bench compilado.

Exemplo de compilação e execução:

```bash
clang++ ./ULA/testbench_ula.cpp -std=c++17 -I/systemc-2.3/include -lsystemc -L/systemc-2.3/lib-macosx386 -o ./ULA/testbench_ula
./ULA/testbench_ula
```

## Conclusão

Com este documento, espera-se fornecer uma visão geral clara da implementação da ULA em SystemC, juntamente com um teste bench abrangente para verificar sua funcionalidade. Qualquer pessoa interessada em aprender ou entender a ULA em nível de hardware pode usar este código como referência.
