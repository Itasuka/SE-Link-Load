#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linker.h"
#include "utils.h"


module linker(int nbmods, module *mods) {

    module mod;

    // passe 1, créer un module unique pour les regrouper tous en décalant les adresses relatives dans chaque module

    int code_length = 0;
    int data_size = 0;
    casemem *code;
    casemem *data;

    for (int i = 0; i < nbmods; ++i) {
        code_length += mods[i].code_length;
        data_size += mods[i].data_size;
    }

    code = malloc(code_length * sizeof(casemem));
    data = malloc(data_size * sizeof(casemem));

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

    mod = module_create(code_length, code, data_size, data);

    for (int i = 0; i < mod.code_length; ++i){
        for (int j = 0; j < mod.code[i].nbarg; ++j){
            if (mod.code[i].args[j].type == 4){
                //TODO
            }
            if (mod.code[i].args[j].type == 3){
                //TODO
            }
        }
    }

    // passe 2, remplir la table des symboles


    // afficher la table des symboles

    // passe 3, remplacer pour chaque référence le nom par l'adresse de la définition

    return mod;
}
