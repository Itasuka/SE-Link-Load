#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loader.h"
#include "utils.h"

int loader(int debut, module mod, casemem *ram) {
    int exist_deb = -1;
    for (int i = 1; i <= mod.code_length; ++i) {
        if (strcmp(mod.code[i - 1].symbol, SYMBOL_BEGIN) == 0) {
            exist_deb = 1;
            ram[debut] = casemem_copy(&mod.code[i - 1]);
            ram[debut].args[0].val.n += debut;
        }
        ram[i + debut] = casemem_copy(&mod.code[i - 1]);
        for (int j = 0; j < mod.code[i - 1].nbarg; ++j)
            if (mod.code[i - 1].args[j].type == 4)
                ram[i + debut].args[j].val.n += debut + 1;
    }
    for (int i = 1 + mod.code_length; i <= mod.code_length + mod.data_size; ++i)
        ram[i + debut] = casemem_copy(&mod.data[i - 1 - mod.code_length]);
    if (exist_deb < 0)
        return SYMBOL_BEGIN_UNDEFINED;
    return debut + mod.code_length + mod.data_size + 1;
}
