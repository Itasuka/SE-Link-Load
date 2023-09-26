#pragma once

#include <stdio.h>
#include <sys/types.h>

#include "types.h"

void print_line_dash ();
// Print_ une ligne de tirets

void print_code ( int nbinstr, casemem *linstr );
// affichage dans le terminal d'une liste d'instruction , la premiere colonne
// donne la representation memoire utilisee ici , la seconde est de type langage
// d'assemblage

void print_data ( int nbinstr, int memsize, casemem *mem );
// affichage dans le terminal d'une zone mémoire avec références éventuelles

void print_ram ( int ramsize, casemem *ram );
// affichage dans le terminal d'une partie de la ram , la ligne de début est en
// rouge

argument argument_create ( char type, value val );
// creation d'un element de type argument d'une instruction

casemem casemem_create_detail ( char const *symbol, char const *name, char nbarg, char type0, value val0, char type1, value val1 );
// creation d'un element de type instruction

casemem casemem_copy ( casemem *ci );
// renvoie une copie profonde d'une casemem

void casemem_free ( casemem *cm );
// libère la mémoire occupée par un casemem

void casemem_array_free ( casemem *cm, int length );
// libère la mémoire occupée par un tableau de casemem

void module_free ( module *m );
// libère la mémoire occupée par un module

module module_create ( int code_length, casemem *code, int data_size, casemem *data );
// creation d'un element de type module

module module_read ( char *file_name );
// lecture de la description d'un module dans le fichier et création du module

#define ARG_INT 0
#define ARG_REGISTER 1
#define ARG_ADDR_ABSOLUTE 2
#define ARG_ADDR_REL_SYMBOL 3
#define ARG_ADDR_REL_INT 4
#define ARG_ERREUR -1
