#pragma once

#include "types.h"

int loader ( int debut, module mod, casemem *mem );
// loader prend comme arguments la position de chargement en ram , le tableau
// des modules à charger et la liste des emplacements memoires , il retourne la
// premiere position qui suit les deux segments du module chargé. ou
// SYMBOL_BEGIN_UNDEFINED s'il n'y a pas de symbol SYMBOL_BEGIN (rien n'est fait
// dans ce cas-là ).
