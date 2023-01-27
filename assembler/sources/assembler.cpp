#include "oneggin.h"
#include "assembler.h"

int main() {
    FILE *input = fopen("commands.txt", "rb");
    ASSERT(input == NULL, "Void ptr on file");

    int nSmbl = countSymbol(input);
    char *str = read_f(input);

    make_code(str, nSmbl);

    free(str);
    fclose(input);

    return 0;
}
