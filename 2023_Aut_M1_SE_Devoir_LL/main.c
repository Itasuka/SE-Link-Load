#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linker.h"
#include "loader.h"
#include "types.h"
#include "utils.h"

int main ( int argc, char **argv ) {

  if ( argc != 4 ) {
    fprintf ( stderr, "Usage : %s step module1 module2\n", argv [ 0 ] );
    exit ( 1 );
  }

  int step = atoi ( argv [ 1 ] );

  if ( step < 0 || 2 < step ) {
    fprintf ( stderr, "step 0: charge les modules\n" );
    fprintf ( stderr, "step 1: fait l'édition de liens\n" );
    fprintf ( stderr, "step 2: fait le chargement en mémoire\n" );
    exit ( 1 );
  }

  // lecture des 2 modules depuis les fichiers de description
  module m0 = module_read ( argv [ 2 ] );
  module m1 = module_read ( argv [ 3 ] );
  module modules [ 2 ] = { m0, m1 };

  printf ( "\nModules avant édition de liens :\n\n" );
  print_code ( modules [ 0 ].code_length, modules [ 0 ].code );
  print_data ( modules [ 0 ].code_length, modules [ 0 ].data_size, modules [ 0 ].data );
  print_code ( modules [ 1 ].code_length, modules [ 1 ].code );
  print_data ( modules [ 1 ].code_length, modules [ 1 ].data_size, modules [ 1 ].data );

  if ( 1 <= step ) {

    // édition de liens, les 2 modules deviennent un seul
    printf ( "\nÉdition de liens :\n\n" );
    module modlinked = linker ( 2, modules );
    printf ( "\nModule après édition de liens :\n\n" );
    print_code ( modlinked.code_length, modlinked.code );
    print_data ( modlinked.code_length, modlinked.data_size, modlinked.data );

    if ( 2 <= step ) {
      // création de la mémoire
      int ramsize = 45;
      casemem *ram = ( casemem * ) malloc ( ramsize * sizeof ( casemem ) );
      int pos = 4;
      casemem vide = { SYMBOL_NONE, NAME_EMPTY, 0, NULL };

      for ( int i = 0; i < ramsize; i++ ) {
        ram [ i ] = vide;
      }

      // chargement
      loader ( pos, modlinked, ram );
      printf ( "\nMémoire après chargement :\n\n" );
      print_ram ( ramsize, ram );
      casemem_array_free ( ram, ramsize );
      free ( ram );
    }
    module_free ( &modlinked );
  }

  module_free ( &m1 );
  module_free ( &m0 );

  return 0;
}
