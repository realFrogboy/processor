#include "oneggin.h"
#include "assembler.h"

struct Labels label[NUM_OF_LABELS];
int THE_SECOND_PASS = 0;
int count = 1;

int make_code(char *str, int nSmbl) {
    LabelsCtor(label);

    LINE_BREAK;

    FILE *output = fopen("code.txt", "wb");
    ASSERT(output == NULL, "Void ptr on file");

    for (int num = 0; num < nlines; num++) {
        //puts(astr[num].refonBegin);
        scanLine(astr[num].refonBegin, output);
    }

    if (THE_SECOND_PASS) {
        count = 1;
        freopen ("code.txt", "wb", output);

        for (int num = 0; num < nlines; num++) {
            //puts(astr[num].refonBegin);
            scanLine(astr[num].refonBegin, output);
        }        
    }

    lineDtor(astr, nlines);
    LabelsDtor(label);

    free(astr);
    fclose(output);

    return 0;
}



int scanLine(const char *str, FILE *output) {
    ASSERT(str == NULL, "Void ptr on string");
    ASSERT(output == NULL, "Void ptr on file");

    char func[20] = "";
    int ver = 0, res = 0;
    char reg = '\0';
    double value = NAN;

    sscanf (str, ": %s%n", func, &ver);
    if (ver) {
        strcpy (label[count].name, func);
        fprintf (output, ":%d\n", count++);

        return 0;
    }

    char lab[LABEL_NAME_SIZE] = "";
    sscanf (str, "%s :: %s%n", func, lab, &ver);
    if (ver)
    {
        double value = 0;
        for (int num = 1; num < NUM_OF_LABELS; num++)
        {
            if (strncmp (lab, label[num].name, strlen(lab)) == 0)
                value = num;
        }

        if (value == 0)
            THE_SECOND_PASS = 1; // do assembler 2 times

        RECORD;
    }


    sscanf (str, "%s [%cx + %lf]%n", func, &reg, &value, &ver);
    if (ver) {
        MEMORY_ACCESS;
        RECORD;
    }

    sscanf (str, "%s %cx + %lf%n", func, &reg, &value, &ver);
    if (ver) {
        RECORD;
    }

    sscanf (str, "%s [%cx]%n", func, &reg, &ver);
    if (ver) {
        MEMORY_ACCESS;
        RECORD;
    }

    sscanf (str, "%s [%lf]%n", func, &value, &ver);
    if (ver) {
        reg = '\0';
        MEMORY_ACCESS;
        RECORD;
    }

    sscanf (str, "%s %cx%n", func, &reg, &ver);
    if (ver) {
        RECORD;
    }

    sscanf (str, "%s %lf%n", func, &value, &ver);
    if (ver) {
        reg = '\0';
        RECORD;
    }

    sscanf (str, "%s%n", func, &ver);
    if (ver) {
        reg = '\0';
        RECORD;
    }

    return 1;
}



int placeReg (const char reg, int *res) {
    ASSERT(res == NULL, "Void ptr\n");

    int regNum = 0;

    if ((reg >= 'a') && (reg <= 'd')) {
        *res = *res | (1u << 6);
        regNum = (int)reg - (int)('a') + 1;
    }
    else if (reg != '\0')
        ASSERT(reg != '\0', "There is no such register\n");
    
    return regNum;
}



int AssFuncDef (const char *func) {
    ASSERT(func == NULL, "Void ptr\n");

    int res = 666;

    DEF_FUNC(hlt, HLT);
    DEF_FUNC(push, PUSH);
    DEF_FUNC(add, ADD);
    DEF_FUNC(sub, SUB);
    DEF_FUNC(mul, MUL);
    DEF_FUNC(div, DIV);
    DEF_FUNC(out, OUT);
    DEF_FUNC(pop, POP);
    DEF_FUNC(jmp, JMP);
    DEF_FUNC(ja, JA);
    DEF_FUNC(jae, JAE);
    DEF_FUNC(jb, JB);
    DEF_FUNC(jbe, JBE);
    DEF_FUNC(je, JE);
    DEF_FUNC(jne, JNE);
    DEF_FUNC(call, CALL);
    DEF_FUNC(ret, RET);
    DEF_FUNC(sqrt, SQRT);

    return res;    
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