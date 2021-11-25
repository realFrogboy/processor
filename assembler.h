#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

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

const int NUM_OF_LABELS   = 15;
const int LABEL_NAME_SIZE = 20;

#ifndef ASSERT
#define ASSERT(statement, text) do {                                                                                    \
                                     if (statement) {                                                                   \
                                        printf (" %s:%d, IN FUNCTION %s:\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
                                        printf (#text);                                                                 \
                                    }                                                                                   \
                                } while (0)
#endif 

#ifndef MEMORY_ACCESS
#define MEMORY_ACCESS res = res | (1u << 7);
#endif

#ifndef DEF_FUNC
#define DEF_FUNC(name, num) do {                                                \
                                if (strncmp(#name, func, strlen(func)) == 0)    \
                                    return num;                                 \
                            } while (0)
#endif

#ifndef LINE_BREAK
#define LINE_BREAK  int nlines = countStrings(nSmbl, str);              \
                    line *astr =(line*)calloc(nlines, sizeof(line));    \
                    lineCtor(astr, nlines);                             \
                    scanFile(nSmbl, astr, str);   
#endif   

#ifndef RECORD 
#define RECORD do {                                                           \
                    res = AssFuncDef (func);                                  \
                    ASSERT(res == 666, "There is no such function\n");        \
                                                                              \
                    printf("%c\n", reg);                                      \
                    if (isnan(value) == 0)                                    \
                        res = res | (1u << 5);                                \
                                                                              \
                    int regNum = 0;                                           \
                    if (reg != '\0')                                             \
                        regNum = placeReg (reg, &res);                        \
                                                                              \
                    fprintf(output, "%d", res);                               \
                    if (reg != '\0')                                             \
                        fprintf(output, " %d", regNum);                       \
                    if (isnan(value) == 0)                                    \
                        fprintf(output, " %lf", value);                       \
                    fprintf(output, "\n");                                    \
                                                                              \
                    return 0;                                                 \
                } while (0)
#endif     

struct Labels
{
    char name[LABEL_NAME_SIZE];
    int position;
};

int LabelsCtor (Labels *label);
int LabelsDtor (Labels *label);

int make_code(char *str, int nSmbl);
int scanLine(const char *str, FILE *output);
int AssFuncDef (const char *func);
int placeReg (const char reg, int *res);

#endif