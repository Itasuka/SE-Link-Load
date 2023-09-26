#pragma once

typedef struct {
  char name [ 3 ];
  int adr;
} symbole; // élément de base pour la table des symboles

typedef union value {
  int n;
  char name [ 3 ];
} value;

typedef struct {
  char type; // 0 si entier, 1 si numéro de registre, 2 si adr absolue, 3 si adr
             // relative par référence, 4 si adresse relative entière
  value val; // soit un entier, soit une référence
} argument;

typedef struct {
  char const *symbol;
  char const *name;
  char nbarg; // nombre d'arguments (1 ou 2)
  argument *args;
} casemem;

typedef struct {
  int code_length; // en nombre d'instructions
  casemem *code;
  int data_size;
  casemem *data;
} module;

#define SYMBOL_BEGIN_UNDEFINED -1

extern char const * const SYMBOL_BEGIN;
extern char const * const SYMBOL_NONE;
extern char const * const NAME_MEM;
extern char const * const NAME_EMPTY;
