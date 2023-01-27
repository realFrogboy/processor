# Procesor
4th task from Huawei course, 1st grade.<br />

The project consists from 3 parts: assembler, processor and diasassembler.

## Assembler
This program gets file with commands and converts them to the "executable code". So it generates two files: code.txt and label.txt. The last one consists names of labels and their numbers.

## Processor
The program gets the "executable code" and executes it.<br />
There are 18 different functions:<br />
1. **ADD**
2. **SUB**
3. **MUL**
4. **DIV**
5. **PUSH**
6. **POP**
7. **JMP**
8. **JA**
9. **JAE**
10. **JB**
11. **JBE**
12. **JE**
13. **JNE**
14. **CALL**
15. **RET**
16. **OUT**
17. **HLT**
18. **SQRT**

## Disassembler
This program gets the "executable code" and file with labels (label.txt) and try to regenerate source code. 

## Tests
There are programs that solves square equation and count factorial(recursive).
