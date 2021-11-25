#include "oneggin.h"
#include "disassembler.h"

Labels label[NUM_OF_LABELS];

int make_commands(char *str, int nSmbl) {
    ASSERT(str == NULL, "Void ptr");
    
    LINE_BREAK;

    LabelsCtor(label);
    fill_labels(label);

    FILE *output = fopen("commands_d.txt", "wb");
    ASSERT(output == NULL, "Void ptr on file");

    for (int num = 0; num < nlines; num++) {
        //puts(astr[num].refonBegin);
        scanLine(astr[num].refonBegin, output);\
        fprintf(output, "\n");
    }

    lineDtor(astr, nlines);
    LabelsDtor(label);

    free(astr);

    return 0;
}


int scanLine(const char *str, FILE *output) {
    ASSERT(str == NULL, "Void ptr");
    ASSERT(output == NULL, "Void ptr");

    int func = 0,  ver = 0;

    sscanf (str, ":%d%n", &func, &ver);
    if (ver) {
        for (int num = 0; num < NUM_OF_LABELS; num++) {
            if (num == func)
                fprintf (output, ": %s", label[num].name);
        }

        return 0;
    }

    sscanf (str, "%d", &func);

    int funcNum = 0;
    for (int num = 4; num >= 0; num--)
        funcNum += ((func >> num) & 1u) * pow_mod (2, num);

    char name_of_func[10] = "";
    int tmp = defineName (funcNum, name_of_func);
    ASSERT(tmp == 1, "There is no such function");

    double val = 0;
    int regNum = 0;
    if (CONSTANT_FL && REGISTER_FL) {    
        sscanf (str, "%d %d %lf", &func, &regNum, &val);
        GET_REG;
        RAM_FL ? fprintf (output, "%s [%cx + %lf]", name_of_func, reg, val) : fprintf (output, "%s %cx + %lf", name_of_func, reg, val);
    }

    else if (REGISTER_FL)
    {
        sscanf (str, "%d %d", &func, &regNum);
        GET_REG
        RAM_FL ? fprintf (output, "%s [%cx]", name_of_func, reg) : fprintf (output, "%s %cx", name_of_func, reg); 
    }

    else if (CONSTANT_FL)
    {
        sscanf (str, "%d %lf", &func, &val);

        if ((funcNum >= 8) && (funcNum <=15)) {                                 // FOR FUNCTIONS WITH LABELS
            for (int num = 0; num < NUM_OF_LABELS; num++)                       //
                if (num == (int)val) {                                          // 
                    fprintf (output, "%s :: %s", name_of_func, label[num].name);// FOR FUNCTIONS WITH LABELS                                
                    return 0;                                                   //
                }                                                               //
            }                                                                   // FOR FUNCTIONS WITH LABELS

        RAM_FL ? fprintf (output, "%s [%lf]", name_of_func, val) : fprintf (output, "%s %lf", name_of_func, val); 
    }

    else
        fprintf (output, "%s", name_of_func);

    return 0;
}


int defineName (int funcNum, char *name_of_func) {
    ASSERT(name_of_func == NULL, "Void ptr on string\n");

    switch (funcNum) {
        COPY_FUNC_NAME(hlt, HLT);
        COPY_FUNC_NAME(push, PUSH);
        COPY_FUNC_NAME(add, ADD);
        COPY_FUNC_NAME(sub, SUB);
        COPY_FUNC_NAME(mul, MUL);
        COPY_FUNC_NAME(div, DIV);
        COPY_FUNC_NAME(out, OUT);
        COPY_FUNC_NAME(pop, POP);
        COPY_FUNC_NAME(jmp, JMP);
        COPY_FUNC_NAME(ja, JA);
        COPY_FUNC_NAME(jae, JAE);
        COPY_FUNC_NAME(jb, JB);
        COPY_FUNC_NAME(jbe, JBE);
        COPY_FUNC_NAME(je, JE);
        COPY_FUNC_NAME(jne, JNE);
        COPY_FUNC_NAME(call, CALL);
        COPY_FUNC_NAME(ret, RET);
        COPY_FUNC_NAME(sqtr, SQRT);
    }

    return 1;
}


//-----------------------------------------------------------------------------


int fill_labels (Labels *strc) {
    ASSERT(strc == NULL, "Void ptr");

    FILE *label_file = fopen ("label.txt", "rb");
    ASSERT(label_file == NULL, "Can't open file\n");

    int nSmbl = countSymbol(label_file);
    char *str = read_f(label_file);

    LINE_BREAK;

    for (int num = 0; num < nlines; num++) {
        getLabel(astr[num].refonBegin, strc);
    }

    free (str);
    fclose (label_file);

    return 0;
}


int getLabel(const char *str, Labels *strc) {
    ASSERT(str == NULL, "Void ptr");
    ASSERT(strc == NULL, "Void ptr");
    
    char name[LABEL_NAME_SIZE] = "";                         
    int pos = 0;                                
    
    sscanf (str, "%s - %d", name, &pos);   
    printf ("%s,,,,,%d\n", name, pos);          
                                                                
    if (strcmp (name, "-"))                     
        strcpy (strc[pos].name, name);  
    
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


//-----------------------------------------------------------------------------


int LabelsCtor (Labels *label) {
    ASSERT(label == NULL, "Void ptr on ptr");
    
    for (int num = 0; num < NUM_OF_LABELS; num++) {
        //label[num].name = ""; ??????
        label[num].position = 0;
    }

    return 0;
}

int LabelsDtor (Labels *label) {
    ASSERT(label == NULL, "Void ptr on label\n");
    ASSERT(label[1].position == -1, "Repeated LabelsDtor\n");

    FILE* label_file = fopen ("label.txt", "wb");
    ASSERT(label_file == NULL, "Can't open file\n");

    for (int num = 0; num < NUM_OF_LABELS; num++) {
        fprintf (label_file, "%s - %d\n", label[num].name, num);
        label[num].position = -1;
    }

    fclose (label_file);

    return 0;
}