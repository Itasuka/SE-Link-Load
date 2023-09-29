#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linker.h"
#include "utils.h"


module linker(int nbmods, module *mods) {

    module mod;

    // passe 1, créer un module unique pour les regrouper tous en décalant les adresses relatives dans chaque module

    int *code_length = malloc((nbmods + 1) * sizeof(int));
    int *data_size = malloc((nbmods + 1) * sizeof(int));
    casemem *code;
    casemem *data;

    code_length[0] = mods[0].code_length;
    data_size[0] = mods[0].data_size;
    for (int i = 1; i <= (nbmods); ++i) {
        code_length[i] = mods[i].code_length + code_length[i - 1];
        data_size[i] = mods[i].data_size + data_size[i - 1];
    }

    code = malloc(code_length[(nbmods - 1)] * sizeof(casemem));
    data = malloc(data_size[(nbmods - 1)] * sizeof(casemem));

    int decalage_code = 0;
    int decalage_data = 0;
    for (int i = 0; i < nbmods; ++i) {
        for (int j = 0; j < mods[i].code_length; ++j) {
            code[j + decalage_code] = casemem_copy(&mods[i].code[j]);
        }
        for (int j = 0; j < mods[i].data_size; ++j) {
            data[j + decalage_data] = casemem_copy(&mods[i].data[j]);
        }
        decalage_code += mods[i].code_length;
        decalage_data += mods[i].data_size;
    }

    mod = module_create(code_length[nbmods - 1], code, data_size[nbmods - 1], data);

    for (int i = 0; i < mod.code_length; ++i) {
        for (int j = 0; j < mod.code[i].nbarg; ++j) {
            if (mod.code[i].args[j].type == 4) {
                int c = 0; // emplacement code
                int d = 0; // emplacement data
                int my_k = 0; // numéro du module
                for (int k = 0; k <= nbmods; ++k) {
                    if (i < code_length[k]) {
                        if (k != 0) {
                            c = code_length[k - 1];
                            d = data_size[k - 1];
                        }
                        my_k = k;
                        break;
                    }
                }
                //printf("%d\n", d);
                int taille_code = code_length[my_k] - c;
                if (mod.code[i].args[j].val.n >= taille_code) // l'adresse se trouve dans les data
                    mod.code[i].args[j].val.n += code_length[nbmods - 1] + d - taille_code;
                else // l'adresse se trouve dans le code
                    mod.code[i].args[j].val.n += c;
            }
        }
    }

    free(code_length);
    free(data_size);
    // passe 2, remplir la table des symboles


    // afficher la table des symboles

    // passe 3, remplacer pour chaque référence le nom par l'adresse de la définition

    for (int i = 0; i < mod.code_length; ++i) {
        for (int j = 0; j < mod.code[i].nbarg; ++j) {
            if (mod.code[i].args[j].type == 3) {
                mod.code[i].args[j].type = 4;
                mod.code[i].args[j].val.n = chercherTable(t, mod.code[i].args[j].val.name);
            }
        }
    }

    libererTable(t);

    return
            mod;
}
