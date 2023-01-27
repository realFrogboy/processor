#include "oneggin.h"
#include "disassembler.h"

int main() {
    FILE *input = fopen("code.txt", "rb");
    ASSERT(input == NULL, "Void ptr on file");

    int nSmbl = countSymbol(input);
    char *str = read_f(input);

    make_commands(str, nSmbl);

    free(str);
    fclose(input);

    return 0;
}