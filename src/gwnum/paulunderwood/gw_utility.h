/*
Nomenclature:
m mpz
w woltman
g giants
*/

#include <gmp.h>

void small_base_exp ( mpz_ptr mr, double db, mpz_srcptr me, mpz_srcptr mn );
void medium_base_exp ( mpz_ptr mr, double db, mpz_srcptr me, mpz_srcptr mn );
void large_base_exp ( mpz_ptr mr, mpz_srcptr mb, mpz_srcptr me, mpz_srcptr mn );
int gw_prp ( mpz_srcptr mn );
void gw_powm ( mpz_ptr mr, mpz_srcptr mb_inp, mpz_srcptr me, mpz_srcptr mn );

/*****************************************************************************/
/*****************************************************************************/
