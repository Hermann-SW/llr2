#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "gw_utility.h"

int main ( int argc, char * argv[] ) {

	mpz_t r, b, e, n;
	mpz_init ( r );
	mpz_init ( b );
	mpz_init ( e );
	mpz_init ( n );

	mpz_inp_str ( n, NULL, 10 );
	mpz_sub_ui ( e, n, 2 );
	mpz_set_ui ( b, 3 );
	gw_powm ( r, b, e, n );
	mpz_out_str ( NULL, 10, r ); printf ( "\n" );
}

