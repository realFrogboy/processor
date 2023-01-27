#include "proc.h"

int DEBUG_LEVEL = 3; // for stack

int main() {
    FILE *code = fopen("code.txt", "rb");
    ASSERT(code == NULL, "Void ptr on file");

    struct CPU processor;
    CPUCtor(&processor);

    int nSmbl = countSymbol(code);
    char *str = read_f(code);

    arrayCtor (&processor, str, nSmbl);

    while ((int)processor.code[processor.ip])
    {
        DOFunc (&processor);
        /*printf ("================\n");
        prinStack (processor.stk_for_call);
        prinStack (processor.stk);
        printf ("----------------\n");
        printf ("%d\n", processor.ip);
        printf ("&&&&&&&&&&&&&&&&\n");
        for (int num = 0; num < 5; num++)
        {
            printf ("%d,  ", processor.regs[num]);
            printf ("\n");
        }
        printf ("================\n");*/
    }
    
    CPUDtor(&processor);

    free(str);
    fclose(code);

    return 0;
}