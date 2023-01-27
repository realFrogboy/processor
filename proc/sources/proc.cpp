#include "proc.h"

int CPUCtor (CPU *processor)
{
    ASSERT(processor == NULL, "Void ptr on processor\n");

    processor->stk_for_call = (Stack*) calloc (1, sizeof (Stack));
    ASSERT(processor->stk_for_call == NULL,  "Can't alloc memory\n");

    processor->stk = (Stack*) calloc (1, sizeof (Stack));
    ERROR_INFO(processor->stk == NULL,  "Can't alloc memory\n");

    stackCtor (processor->stk);
    stackCtor (processor->stk_for_call);
    
    ARRAY_INITIALIZATION(SIZE_OF_RAM, RAM);
    ARRAY_INITIALIZATION(SIZE_OF_CODE, code);
    ARRAY_INITIALIZATION(SIZE_OF_REGS, regs);
    ARRAY_INITIALIZATION(NUM_OF_LABELS, label);

    processor->ip = 0;

    return 0;
}

int CPUDtor (CPU *processor)
{
    ASSERT(processor == NULL, "Void ptr on processor\n");
    ASSERT(processor->ip == -1, "Repeated CPUDtor\n");

    stackDtor (processor->stk);
    stackDtor (processor->stk_for_call);

    free (processor->stk); 
    free (processor->stk_for_call);

    processor->ip = -1;

    return 0;    
}


//-----------------------------------------------------------------------------


int arrayCtor (CPU *processor, char *str, int nSmbl) {
    ASSERT(processor == NULL, "Void ptr on processor\n");
    ASSERT(str == NULL, "Void ptr on string\n");

    LINE_BREAK;

    for (int num = 0; num < nlines; num++) {
        CPUFuncDef(processor, astr[num].refonBegin);
    }

    /*for (int num = 0; num < processor->ip; num++) 
        printf ("%d\n", processor->code[num]);

    for (int num = 0; num < NUM_OF_LABELS; num++)
        printf ("%d,  ", processor->label[num]);

    printf ("\n");*/
    processor->ip = 0;

    free(astr);

    return 0;
}


int CPUFuncDef (CPU *processor, const char *str) {
    ASSERT(processor == NULL, "Void ptr on processor\n");
    ASSERT(str == NULL, "Void ptr on string\n");

    int func = 0, ver = 0;

    sscanf (str, ":%d%n", &func, &ver);
    if (ver) {
        for (int num = 1; num < NUM_OF_LABELS; num++) {
            if (num == func)
                processor->label[num] = processor->ip;
        }

        return 0;
    }

    sscanf (str, "%d", &func);

    int regNum = 0;
    double val = 0;

    if (CONSTANT_FL && REGISTER_FL) {    
        sscanf (str, "%d %d %lf", &func, &regNum, &val);

        SET_FUNC_IN_CODE;
        SET_REG_IN_CODE;
        SET_CONST_IN_CODE(5);

        processor->ip += 13;
    }

    else if (REGISTER_FL) {
        sscanf (str, "%d %d", &func, &regNum);

        SET_FUNC_IN_CODE;
        SET_REG_IN_CODE;

        processor->ip += 5;
    }

    else if (CONSTANT_FL) {
        sscanf (str, "%d %lf", &func, &val);

        SET_FUNC_IN_CODE;
        SET_CONST_IN_CODE(1);

        processor->ip += 9;   
    }

    else {
        SET_FUNC_IN_CODE;
        processor->ip += 1;
    }

    return 0;
}

int FuncInfoCtor (FuncInfo *strc, CPU *processor) {
    ASSERT(strc == NULL, "Void ptr");

    strc->processor = processor;
    strc->funcNum = 0;
    strc->value = 0;
    strc->ram_access = 0;

    return 0;
}


//-----------------------------------------------------------------------------


int DOFunc (CPU *processor) {
    ASSERT(processor == NULL, "Void ptr on processor\n");

    int func = *(int*)(processor->code + processor->ip);

    struct FuncInfo info;
    FuncInfoCtor(&info, processor);

    if (RAM_FL) info.ram_access = -1;
    
    for (int num = 4; num >= 0; num--)
        info.funcNum += ((func >> num) & 1u) * pow_mod (2, num);
    
    if (CONSTANT_FL && REGISTER_FL) {    
        GET_REG;
        GET_CONST;

        if (!RAM_FL) info.ram_access = regNum;
        info.value = processor->regs[regNum] + val;

        int tmp = RealizeFunc (&info);
        ASSERT(tmp == 666, "There is no such function\n");

        processor->ip += 13;

        return 0;
    }

    else if (REGISTER_FL) {
        GET_REG;

        if (!RAM_FL) info.ram_access = regNum;
        info.value = processor->regs[regNum];

        int tmp = RealizeFunc (&info);
        ASSERT(tmp == 666, "There is no such function\n");

        processor->ip += 5;

        return 0; 
    }

    else if (CONSTANT_FL) {
        GET_CONST;

        info.value = val;

        int tmp = RealizeFunc (&info);
        ASSERT(tmp == 666, "There is no such function\n");

        processor->ip += 9;

        return 0;
    }
    
    else {    
        info.value = 0;

        int tmp = RealizeFunc (&info);
        ASSERT(tmp == 666, "There is no such function\n");

        processor->ip += 1;
    
        return 0;
    }
}

int RealizeFunc (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr");

    switch (strc->funcNum) {
        DEF_FUNC(HLT, hlt);
        DEF_FUNC(PUSH, push);
        DEF_FUNC(ADD, add);
        DEF_FUNC(SUB, sub);
        DEF_FUNC(MUL, mul);
        DEF_FUNC(DIV, div);
        DEF_FUNC(OUT, out);
        DEF_FUNC(POP, pop);
        DEF_FUNC(JMP, jmp);
        DEF_FUNC(JA, ja);
        DEF_FUNC(JAE, jae);
        DEF_FUNC(JB, jb);
        DEF_FUNC(JBE, jbe);
        DEF_FUNC(JE, je);
        DEF_FUNC(JNE, jne);
        DEF_FUNC(CALL, call);
        DEF_FUNC(RET, ret);
        DEF_FUNC(SQRT, sqrt_);
    }

    return 666;
}


//-----------------------------------------------------------------------------


int hlt (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr");
    return 0;
}

int push (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr");

    if (strc->ram_access == -1) 
        stackPush (strc->processor->stk, strc->processor->RAM[(int)strc->value]);
    else 
        stackPush (strc->processor->stk, strc->value);

    return 0;
}

int add (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr");
    OPERATION(+);
}

int sub (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr");
    OPERATION(-);
}

int mul (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr");
    OPERATION(*);
}

int div (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr");
    ASSERT(strc->processor->stk->data[strc->processor->stk->Size - 1] == 0, "Division by 0!\n");
    OPERATION(/);
}

int out (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr");
    printf ("%5.2f\n", strc->processor->stk->data[strc->processor->stk->Size - 1]); 
    stackPop (strc->processor->stk);

    return 0;   
}

int pop (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr on processor\n");

    if (strc->ram_access == -1)
        strc->processor->RAM[(int)strc->value] = strc->processor->stk->data[strc->processor->stk->Size - 1];
    else    
        strc->processor->regs[strc->ram_access] = strc->processor->stk->data[strc->processor->stk->Size - 1];
    
    stackPop (strc->processor->stk);

    return 0;
}

int jmp (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr on processor\n");

    strc->processor->ip = strc->processor->label[(int)strc->value] - 9;

    return 0;
}

int ja (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr on processor\n");

    if (strc->processor->stk->data[strc->processor->stk->Size - 2] > strc->processor->stk->data[strc->processor->stk->Size - 1])
        strc->processor->ip = strc->processor->label[(int)strc->value] - 9;
    
    stackPop (strc->processor->stk); stackPop (strc->processor->stk);

    return 0;
}

int jae (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr on processor\n");

    if (strc->processor->stk->data[strc->processor->stk->Size - 2] >= strc->processor->stk->data[strc->processor->stk->Size - 1])
        strc->processor->ip = strc->processor->label[(int)strc->value] - 9;
    
    stackPop (strc->processor->stk); stackPop (strc->processor->stk);

    return 0;
}

int jb (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr on processor\n");

    if (strc->processor->stk->data[strc->processor->stk->Size - 2] < strc->processor->stk->data[strc->processor->stk->Size - 1])
        strc->processor->ip = strc->processor->label[(int)strc->value] - 9;
    
    stackPop (strc->processor->stk); stackPop (strc->processor->stk);

    return 0;
}

int jbe (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr on processor\n");

    if (strc->processor->stk->data[strc->processor->stk->Size - 2] <= strc->processor->stk->data[strc->processor->stk->Size - 1])
        strc->processor->ip = strc->processor->label[(int)strc->value] - 9;
    
    stackPop (strc->processor->stk); stackPop (strc->processor->stk);

    return 0;
}

int je (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr on processor\n");

    printf ("==============%f, %f\n", strc->processor->stk->data[strc->processor->stk->Size - 2], strc->processor->stk->data[strc->processor->stk->Size - 1]);
    if (isequal (strc->processor->stk->data[strc->processor->stk->Size - 2], strc->processor->stk->data[strc->processor->stk->Size - 1]))
        strc->processor->ip = strc->processor->label[(int)strc->value] - 9;
    
    stackPop (strc->processor->stk); stackPop (strc->processor->stk);

    return 0;
}

int jne (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr on processor\n");

    if (strc->processor->stk->data[strc->processor->stk->Size - 2] != strc->processor->stk->data[strc->processor->stk->Size - 1])
        strc->processor->ip = strc->processor->label[(int)strc->value] - 9;
    
    stackPop (strc->processor->stk); stackPop (strc->processor->stk);

    return 0;
}

int call (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr on processor\n");

    stackPush (strc->processor->stk_for_call, strc->processor->ip + 9);
    strc->processor->ip = strc->processor->label[(int)strc->value] - 9;

    return 0;
}

int ret (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr on processor\n");

    strc->processor->ip = strc->processor->stk_for_call->data[strc->processor->stk_for_call->Size - 1] - 1;
    stackPop (strc->processor->stk_for_call);

    return 0;
}

int sqrt_ (FuncInfo *strc) {
    ASSERT(strc == NULL, "Void ptr on processor\n");

    double value = strc->processor->stk->data[strc->processor->stk->Size - 1];
    stackPop (strc->processor->stk);
    double res = sqrt (value);
    stackPush (strc->processor->stk, res); 

    return 0;
}


//-----------------------------------------------------------------------------


unsigned long long pow_mod (unsigned long long n, unsigned long long k) {
    unsigned long long mult = n;
    unsigned long long prod = 1;
    while (k > 0) {
        if ((k % 2) == 1) {
            prod = mult_mod (prod, mult); k = k - 1;
        }
        
        mult = mult_mod (mult, mult); k = k / 2;
    }
    
    return prod;
}


unsigned long long mult_mod (unsigned long long n, unsigned long long k) {
    unsigned long long mult = n;
    unsigned long long prod = 0;
    while (k > 0) {
        if ((k % 2) == 1) {
            prod = prod + mult; 
            k = k - 1;
        }
    
        mult = mult + mult; 
        k = k / 2;
    }
    
    return prod;
}


bool isequal(double a, double b) {
    const double EPSILON = 0.0001; //measurement error
    
    if (fabs(a - b) < EPSILON)
        return 1;
    else 
        return 0;
}