#include <stdlib.h>
#include <string.h>

#include "utils.h"

void argument_print ( argument arg ) {
  switch ( arg.type ) {
  case ARG_INT:
    printf ( "%-4d ", arg.val.n );
    break;
  case ARG_REGISTER:
    printf ( "R%-3d ", arg.val.n );
    break;
  case ARG_ADDR_ABSOLUTE:
    printf ( "@%-3d ", arg.val.n );
    break;
  case ARG_ADDR_REL_SYMBOL:
    printf ( "%-3s  ", arg.val.name );
    break;
  case ARG_ADDR_REL_INT:
    printf ( "M%-3d ", arg.val.n );
    break;
  default:
    printf ( "--%d-- ", arg.type );
    break;
  }
}

void print_instr ( casemem instr, int i ) {
  printf ( "      %3d |  ", i );
  printf ( "%4s ", instr.symbol );
  printf ( "%6s ", instr.name );
  for ( int j = 0; j < instr.nbarg; j++ ) {
    if ( instr.args [ j ].type == ARG_ADDR_REL_SYMBOL ) {
      printf ( "( %d , %3s ) ", instr.args [ j ].type, instr.args [ j ].val.name );
    } else {
      printf ( "( %d , %3d ) ", instr.args [ j ].type, instr.args [ j ].val.n );
    }
  }
  for ( int j = instr.nbarg; j < 2; j++ ) {
    printf ( "            " );
  }
  printf ( "  | " );
  printf ( "%3d | ", i );
  printf ( "%4s ", instr.symbol );
  printf ( "%6s ", instr.name );
  for ( int j = 0; j < instr.nbarg; j++ ) {
    argument_print ( instr.args [ j ] );
  }
  for ( int j = instr.nbarg; j < 2; j++ ) {
    printf ( "     " );
  }
  printf ( " |\n" );
}

void data_print_cell ( casemem don, int i ) {
  printf ( "      %3d | ", i );
  printf ( "%6s", don.symbol );
  if ( don.nbarg == 0 ) {
    printf ( " %5d", 0 );
  } else {
    printf ( " %5d", don.args [ 0 ].val.n );
  }
  printf ( "   |\n" );
}

void print_ram_cell ( casemem casemem, int i ) {
  printf ( "       %3d |  ", i );
  if ( strcmp ( casemem.name, "mem" ) == 0 ) {
    printf ( "    %5d            |\n", casemem.args [ 0 ].val.n );
  } else {
    if ( casemem.name != NAME_EMPTY ) {
      printf ( "%6s  ", casemem.name );
    } else {
      printf ( "        " );
    }
    for ( int j = 0; j < casemem.nbarg; j++ ) {
      argument_print ( casemem.args [ j ] );
    }
    for ( int j = casemem.nbarg; j < 2; j++ ) {
      printf ( "     " );
    }
    printf ( "   |\n" );
  }
}

void print_line_dash () { puts ( "      -----------------------------------------------------------------------------" ); }

void print_code ( int code_length, casemem *linstr ) {
  // affichage dans le terminal d'une liste d'instruction, la premiere colonne donne la representation memoire utilisee ici, la seconde est de type langage d'assemblage

  //  line_star ();
  printf ( "           ################ Segment de code ################\n" );
  print_line_dash ();
  for ( int i = code_length - 1; i >= 0; i-- ) {
    print_instr ( linstr [ i ], i );
  }
  print_line_dash ();
  puts ( "" );
}

void print_data ( int code_length, int data_size, casemem *mem ) {
  // affichage dans le terminal d'une zone mémoire avec références éventuelles
  printf ( "           ################ Segment de données ################\n" );
  print_line_dash ();
  for ( int i = data_size - 1; i >= 0; i-- ) {
    data_print_cell ( mem [ i ], code_length + i );
  }
  print_line_dash ();
  puts ( "" );
}

void print_ram ( int ramsize, casemem *mem ) {
  // affichage dans le terminal d'une partie de la ram
  printf ( "           ################ Mémoire centrale ################\n" );
  print_line_dash ();
  for ( int i = ramsize - 1; i >= 0; i-- ) {
    print_ram_cell ( mem [ i ], i );
  }
  print_line_dash ();
  puts ( "" );
}

argument argument_create ( char type,
                           value val ) { // creation d'un element de type argument d'une instruction
  argument arg = { type, val };
  return arg;
}

casemem casemem_empty () {
  casemem c;
  c.symbol = SYMBOL_NONE;
  c.name = NAME_MEM;
  c.nbarg = 0;
  return c;
}

casemem casemem_create_detail ( char const *symbol, char const *name, char nbarg, char type0, value val0, char type1, value val1 ) { // creation d'un element de type instruction
  casemem c = casemem_empty ();
  c.symbol = symbol == SYMBOL_NONE ? SYMBOL_NONE : strdup ( symbol );
  c.name = name == NAME_MEM ? NAME_MEM : strdup ( name );
  c.nbarg = nbarg;
  c.args = ( argument * ) malloc ( c.nbarg * sizeof ( argument ) );
  c.args [ 0 ] = argument_create ( type0, val0 );
  if ( nbarg == 2 ) {
    c.args [ 1 ] = argument_create ( type1, val1 );
  }
  return ( c );
}

argument argument_create_str ( char const *arg_str ) {
  argument arg;
  if ( '0' <= arg_str [ 0 ] && arg_str [ 0 ] <= '9' ) {
    arg.type = ARG_INT;
    arg.val.n = atoi ( arg_str );
  } else if ( '0' <= arg_str [ 1 ] && arg_str [ 1 ] <= '9' ) {
    if ( arg_str [ 0 ] == 'r' || arg_str [ 0 ] == 'R' ) {
      arg.type = ARG_REGISTER;
      arg.val.n = atoi ( arg_str + 1 );
    } else if ( arg_str [ 0 ] == '@' ) {
      arg.type = ARG_ADDR_ABSOLUTE;
      arg.val.n = atoi ( arg_str + 1 );
    } else if ( arg_str [ 0 ] == 'm' || arg_str [ 0 ] == 'M' ) {
      arg.type = ARG_ADDR_REL_INT;
      arg.val.n = atoi ( arg_str + 1 );
    } else {
      arg.type = ARG_ERREUR;
      arg.val.n = 0;
    }
  } else {
    arg.type = ARG_ADDR_REL_SYMBOL;
    arg.val.n = 0; // ensure full initialisation
    strncpy ( arg.val.name, arg_str, 3 );
  }
  return arg;
}

casemem casemem_create_arg ( char const *symbol, char const *name, char nbarg, char const *arg_0,
                             char const *arg_1 ) { // creation d'un element de type instruction
  casemem c = casemem_empty ();
  c.symbol = symbol == SYMBOL_NONE ? SYMBOL_NONE : strdup ( symbol );
  c.name = name == NAME_MEM ? NAME_MEM : strdup ( name );
  c.nbarg = nbarg;
  c.args = ( argument * ) malloc ( c.nbarg * sizeof ( argument ) );
  c.args [ 0 ] = argument_create_str ( arg_0 );
  if ( nbarg == 2 ) {
    c.args [ 1 ] = argument_create_str ( arg_1 );
  }
  return ( c );
}

casemem caseme_create_instruction ( char const *symbol, char const *comm, char const *arg1, char const *arg2 ) {
  if ( 0 == strcmp ( symbol, SYMBOL_NONE ) ) {
    symbol = SYMBOL_NONE;
  }
  if ( strcmp ( comm, "push" ) == 0 ) {
    return casemem_create_arg ( symbol, comm, 1, arg1, "" );
  }
  if ( strcmp ( comm, "pop" ) == 0 ) {
    return casemem_create_arg ( symbol, comm, 1, arg1, "" );
  }
  if ( strcmp ( comm, "call" ) == 0 ) {
    return casemem_create_arg ( symbol, comm, 1, arg1, "" );
  }
  if ( strcmp ( comm, "add" ) == 0 ) {
    return casemem_create_arg ( symbol, comm, 2, arg1, arg2 );
  }
  if ( strcmp ( comm, "jeq" ) == 0 ) {
    return casemem_create_arg ( symbol, comm, 1, arg1, "" );
  }
  if ( strcmp ( comm, "jse" ) == 0 ) {
    return casemem_create_arg ( symbol, comm, 1, arg1, "" );
  }
  if ( strcmp ( comm, "jmp" ) == 0 ) {
    return casemem_create_arg ( symbol, comm, 1, arg1, "" );
  }
  if ( strcmp ( comm, "print" ) == 0 ) {
    return casemem_create_arg ( symbol, comm, 1, arg1, "" );
  }
  if ( strcmp ( comm, "rdmem" ) == 0 ) {
    return casemem_create_arg ( symbol, comm, 2, arg1, arg2 );
  }
  if ( strcmp ( comm, "cmp" ) == 0 ) {
    return casemem_create_arg ( symbol, comm, 2, arg1, arg2 );
  }
  if ( strcmp ( comm, "inc" ) == 0 ) {
    return casemem_create_arg ( symbol, comm, 1, arg1, "" );
  }
  if ( strcmp ( comm, "dec" ) == 0 ) {
    return casemem_create_arg ( symbol, comm, 1, arg1, "" );
  }
  if ( strcmp ( comm, "ret" ) == 0 ) {
    return casemem_create_arg ( symbol, comm, 1, arg1, "" );
  }
  fprintf ( stderr, "Instruction inconnue : %s", comm );
  exit ( 1 );
}

casemem casemem_create_memory ( char const *symbol, char const *arg1 ) {
  value empty;
  value a1;
  a1.n = atoi ( arg1 );
  if ( 0 == strcmp ( symbol, SYMBOL_NONE ) ) {
    symbol = SYMBOL_NONE;
  }
  return casemem_create_detail ( symbol, NAME_MEM, 1, 0, a1, 0, empty );
}

casemem casemem_copy ( casemem *ci ) {
  casemem c;
  c.symbol = ci->symbol == SYMBOL_NONE ? SYMBOL_NONE : strdup ( ci->symbol );
  c.name = ci->name == NAME_MEM ? NAME_MEM : strdup ( ci->name );
  c.nbarg = ci->nbarg;
  c.args = ( argument * ) malloc ( c.nbarg * sizeof ( argument ) );
  for ( int i = 0; i < c.nbarg; i++ ) {
    c.args [ i ] = ci->args [ i ];
  }
  return c;
}

void casemem_free ( casemem *cm ) {
  if ( cm->symbol != SYMBOL_NONE ) {
    free ( ( void * ) ( cm->symbol ) );
  }
  if ( cm->name != NAME_MEM && cm->name != NAME_EMPTY ) {
    free ( ( void * ) ( cm->name ) );
  }
  free ( cm->args );
}

void casemem_array_free ( casemem *cm, int length ) {
  for ( int i = 0; i < length; i++ ) {
    casemem_free ( cm + i );
  }
}

module module_create ( int code_length, casemem *code, int data_size, casemem *data ) {
  module mod;
  mod.code_length = code_length;
  mod.code = code;
  mod.data_size = data_size;
  mod.data = data;
  return mod;
}

module module_read ( char *file_name ) { // lecture dans un fichier donné en paramètre des segments de code et de données, puis construction du module associé
  FILE *pFile;
  int code_length = 0;
  int data_size = 0;
  char *args [ 4 ];
  args [ 2 ] = "";
  int nbarg = 0;
  int num = 0;
  char *lg = NULL;
  char sep [] = ", \t\n";
  size_t len = 0;
  pFile = fopen ( file_name, "r" );
  getline ( &lg, &len, pFile );
  char *mot = strtok ( lg, sep );
  if ( mot != NULL ) {
    code_length = atoi ( mot );
  }
  free ( lg );
  lg = NULL;
  casemem *code = ( casemem * ) malloc ( code_length * sizeof ( casemem ) );
  int i = 0;
  while ( i < code_length ) {
    getline ( &lg, &len, pFile );
    mot = strtok ( lg, sep );
    nbarg = 0;
    while ( mot != NULL ) {
      args [ nbarg ] = mot;
      nbarg = nbarg + 1;
      mot = strtok ( NULL, sep );
    }
    code [ i ] = caseme_create_instruction ( args [ 0 ], args [ 1 ], args [ 2 ], args [ 3 ] );
    i = i + 1;
  }
  getline ( &lg, &len, pFile );
  mot = strtok ( lg, sep );
  if ( mot != NULL ) {
    data_size = atoi ( mot );
  }
  casemem *mem = ( casemem * ) malloc ( data_size * sizeof ( casemem ) );
  i = 0;
  getline ( &lg, &len, pFile );
  while ( strlen ( lg ) != 1 ) {
    mot = strtok ( lg, sep );
    num = 0;
    while ( mot != NULL ) {
      args [ num ] = mot;
      num = num + 1;
      mot = strtok ( NULL, sep );
    }
    mem [ i ] = casemem_create_memory ( args [ 0 ], args [ 1 ] );
    i = i + 1;
    getline ( &lg, &len, pFile );
  }
  while ( i < data_size ) {
    mem [ i++ ] = casemem_empty ();
  }

  free ( lg );
  lg = NULL;
  fclose ( pFile );
  module x = module_create ( code_length, code, data_size, mem );
  return x;
}

void module_free ( module *m ) {
  casemem_array_free ( m->code, m->code_length );
  free ( m->code );
  casemem_array_free ( m->data, m->data_size );
  free ( m->data );
}
