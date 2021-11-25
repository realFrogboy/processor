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

#ifndef CONSTANT_FL
#define CONSTANT_FL (((func >> 5) & 1u) == 1)
#endif 

#ifndef REGISTER_FL
#define REGISTER_FL (((func >> 6) & 1u) == 1)
#endif 

#ifndef RAM_FL
#define RAM_FL (((func >> 7) & 1u) == 1)
#endif 

#ifndef GET_REG
#define GET_REG char reg = (char)(regNum + (int)('a') - 1);
#endif 

#ifndef ASSERT
#define ASSERT(statement, text) do {                                                                                    \
                                     if (statement) {                                                                   \
                                        printf (" %s:%d, IN FUNCTION %s:\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
                                        printf (#text);                                                                 \
                                    }                                                                                   \
                                } while (0)
#endif

#ifndef COPY_FUNC_NAME
#define COPY_FUNC_NAME(func, num)   case num: {                                         \
                                        strncpy(name_of_func, #func, strlen(#func) + 1);\
                                        return 0;                                       \
                                    }          
#endif


#ifndef PUSH_LABELS
#define PUSH_LABELS char name[10] = "";                         \
                    int pos = 0;                                \
                                                                \
                    sscanf (ptr_line, "%s - %d", name, &pos);   \
                    printf ("%s,,,,,%d\n", name, pos);          \
                                                                \
                    if (strcmp (name, "-"))                     \
                    strcpy (strc[pos].name, name);  
#endif    

#ifndef LINE_BREAK
#define LINE_BREAK  int nlines = countStrings(nSmbl, str);              \
                    line *astr =(line*)calloc(nlines, sizeof(line));    \
                    lineCtor(astr, nlines);                             \
                    scanFile(nSmbl, astr, str);   
#endif                  

struct Labels
{
    char name[LABEL_NAME_SIZE];
    int position;
};

int LabelsCtor (Labels *label);
int LabelsDtor (Labels *label);

int getLabel(const char *str, Labels *strc);

unsigned long long pow_mod (unsigned long long n, unsigned long long k);
unsigned long long mult_mod (unsigned long long n, unsigned long long k);

int make_commands(char *str, int nSmbl);
int defineName (int funcNum, char *name_of_func);
int scanLine(const char *str, FILE *output);
int fill_labels (Labels *strc);