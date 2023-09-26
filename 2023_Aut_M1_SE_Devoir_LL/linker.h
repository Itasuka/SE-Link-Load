#pragma once

#include "types.h"

module linker ( int nbmods, module *mods );
// linker rassemble les modules en mettant les adresses en coherence : les
// adresses du second module sont decalees de la taille du premier , etc...
// linker remplit la table des symboles avec :
//-les refs de tous les modules : nom sans adresse
//-les defs de tous les modules : nom et adresse
// puis fait la resolution en parcourant tous les arguments de toutes les
// instructions pour remplacer les refs par les adresses obtenues dans la table
// des symboles le module construit est retourné par la fonction
