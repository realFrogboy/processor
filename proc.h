#ifndef PROC_H_
#define PROC_H_

#include "stack.h"
#include "oneggin.h"

const int SIZE_OF_CODE    = 1000;
const int NUM_OF_LABELS   = 15;
const int LABEL_NAME_SIZE = 20;
const int SIZE_OF_RAM     = 50;
const int SIZE_OF_REGS    = 5; 

enum FUNCTIONS {
    HLT = 0,
    PUSH,
    ADD,
    SUB,
    MUL,
    DIV,
    OUT,
    POP,
    JMP,
    JA,
    JAE,
    JB,
    JBE,
    JE,
    JNE,
    CALL,
    RET,
    SQRT
};

#ifndef CONSTANT_FL
#define CONSTANT_FL (((func >> 5) & 1u) == 1)
#endif 

#ifndef REGISTER_FL
#define REGISTER_FL (((func >> 6) & 1u) == 1)
#endif 

#ifndef RAM_FL
#define RAM_FL (((func >> 7) & 1u) == 1)
#endif 

#ifndef SET_FUNC_IN_CODE
#define SET_FUNC_IN_CODE processor->code[processor->ip] = (char) func
#endif

#ifndef SET_REG_IN_CODE
#define SET_REG_IN_CODE *(int*)(processor->code + processor->ip + 1) = regNum
#endif

#ifndef SET_CONST_IN_CODE
#define SET_CONST_IN_CODE(pos) *(double*)(processor->code + processor->ip + pos) = val
#endif

#ifndef GET_CONST
#define GET_CONST int val = *(double*)(processor->code + processor->ip + 1)
#endif

#ifndef GET_REG
#define GET_REG int regNum = *(int*)(processor->code + processor->ip + 1);
#endif

#ifndef ASSERT
#define ASSERT(statement, text) do {                                                                                    \
                                     if (statement) {                                                                   \
                                        printf (" %s:%d, IN FUNCTION %s:\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
                                        printf (#text);                                                                 \
                                    }                                                                                   \
                                } while (0)
#endif

#ifndef DEF_FUNC
#define DEF_FUNC(num, func) do {                    \
                                case num: {         \
                                    func (strc);    \
                                    return 0;       \
                                }                   \
                            } while (0)
#endif

#ifndef ARRAY_INITIALIZATION
#define ARRAY_INITIALIZATION(quantity, arr) do {                                        \
                                            for (int num = 0; num < quantity; num++)    \
                                                processor->arr[num] = 0;                \
                                            } while (0)
#endif        

#ifndef LINE_BREAK
#define LINE_BREAK  int nlines = countStrings(nSmbl, str);              \
                    line *astr =(line*)calloc(nlines, sizeof(line));    \
                    lineCtor(astr, nlines);                             \
                    scanFile(nSmbl, astr, str);   
#endif   

#ifndef OPERATION
#define OPERATION(sign) do{                                                                                                                                             \
                            double res = strc->processor->stk->data[strc->processor->stk->Size - 2] sign strc->processor->stk->data[strc->processor->stk->Size - 1];    \
                            stackPop (strc->processor->stk); stackPop (strc->processor->stk);                                                                           \
                            stackPush (strc->processor->stk, res);                                                                                                      \
                            return 0;                                                                                                                                   \
                        } while (0)
#endif

struct CPU {
    struct Stack *stk;
    struct Stack *stk_for_call;
    int label[NUM_OF_LABELS];
    char code[SIZE_OF_CODE];
    int RAM[SIZE_OF_RAM];
    int regs[SIZE_OF_REGS];
    int ip;
};

struct FuncInfo {
    struct CPU *processor;
    int funcNum;
    double value;
    int ram_access;
};

int FuncInfoCtor (FuncInfo *strc);

int CPUCtor (CPU *processor);
int CPUDtor (CPU *processor);

int CPUFuncDef (CPU *processor, const char *str);
int arrayCtor (CPU *processor, char *str, int nSmbl);
int DOFunc (CPU *processor);
int RealizeFunc (FuncInfo *strc);

int hlt (FuncInfo *strc);
int push (FuncInfo *strc);
int add (FuncInfo *strc);
int sub (FuncInfo *strc);
int mul (FuncInfo *strc);
int div (FuncInfo *strc);
int out (FuncInfo *strc);
int pop (FuncInfo *strc);
int jmp (FuncInfo *strc);
int ja (FuncInfo *strc);
int jae (FuncInfo *strc);
int jb (FuncInfo *strc);
int jbe (FuncInfo *strc);
int je (FuncInfo *strc);
int jne (FuncInfo *strc);
int call (FuncInfo *strc);
int ret (FuncInfo *strc);
int sqrt_ (FuncInfo *strc);

unsigned long long pow_mod (unsigned long long n, unsigned long long k);
unsigned long long mult_mod (unsigned long long n, unsigned long long k);
bool isequal(double a, double b);

#endif