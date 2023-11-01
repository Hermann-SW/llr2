/*
Please feel free to comment out printf statements with: //

Nomenclature:

m mpz
w woltman
g giants
z gerbicz

r result
b base
e exponent
n modulus
c check
q prime
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include <math.h>
#include "giants.h"
#include "gwnum.h"
#include "gwthread.h"
#include "gwcommon.h"
#include "gw_utility.h"

#define THREADS 16
#define INI_FFT_SIZE 0 // Increase if trouble
#define MAX_STRING_LENGTH 32000000
#define GW_THRESHOLD 8192
#define SAFE 64 // Careful loops for large base loop.
#define ROUND_OFF_LIMIT 0.44
#define ZQ 17586324677945679473ULL // 64 bit prime for Gerbicz EDAC
#define w2m(wx,gx,mx) gwtogiant(gwdata,wx,gx);gtompz(gx,mx);

/*****************************************************************************/

void small_base_exp ( mpz_ptr mr, double db, mpz_srcptr me, mpz_srcptr mn )
{
    int fft_size = INI_FFT_SIZE;
    int string_length;
    int g_length;
    int j;
    int zstep;
    unsigned zb = db;
    char *string = (char*)malloc(MAX_STRING_LENGTH);
    mpz_t zr, zn, zc, zrq;
    gwnum wr, wc;
    gwhandle *gwdata;
    giant gr, gn;
    mpz_init ( zr );
    mpz_init ( zn );
    mpz_init ( zc );
    mpz_init ( zrq );
    mpz_mul_ui ( zn, mn, ZQ );
    mpz_get_str ( string, 10, zn );
    string_length = strlen ( string );
    zstep = ceil ( sqrt ( string_length ) ) + 1;
    g_length = ( string_length >> 2 ) + 16;
    gr  = newgiant ( g_length );
    gn  = newgiant ( g_length );
    ctog ( string, gn );
    gwdata = (gwhandle*) malloc ( sizeof ( gwhandle ) );
    gwinit ( gwdata );
    gwset_num_threads ( gwdata, THREADS );
    gwset_maxmulbyconst ( gwdata, (long) db );
    gwset_larger_fftlen_count ( gwdata, fft_size );
    gwsetup_general_mod_giant ( gwdata, gn );
    gwsetmulbyconst ( gwdata, (long) db );
    wr = gwalloc ( gwdata );
    wc = gwalloc ( gwdata );
    gwerror_checking ( gwdata, 1 );
    dbltogw ( gwdata, db, wr );
    mpz_set_ui ( zr, zb );
    for ( int i = mpz_sizeinbase ( me, 2 ) - 2; i >= 0; )
    {
        mpz_set ( zc, zr );
        gwcopy ( gwdata, wr, wc );
        for ( j = 0; j < zstep && i >= 0; j++, i-- )
        {
            if ( mpz_tstbit ( me, i ) )
            {
                gwsquare2 ( gwdata, wr, wr, GWMUL_MULBYCONST ); // From gwnum.h
                mpz_mul ( zr, zr, zr );
                mpz_mul_ui ( zr, zr, zb );
                mpz_mod_ui ( zr, zr, ZQ );
            }
            else
            {
                gwsquare2 ( gwdata, wr, wr, 0x0000 );
                mpz_mul ( zr, zr, zr );
                mpz_mod_ui ( zr, zr, ZQ );
            }
        }
        w2m ( wr, gr, mr );
        mpz_mod_ui ( zrq, mr, ZQ );
        if ( gw_get_maxerr ( gwdata ) > ROUND_OFF_LIMIT || mpz_cmp ( zrq, zr ) != 0 )
        {
            printf ( "*** (sm) Upping FFT size due to round off error or Gerbicz error before bit %d.\n", i );
            gwtogiant ( gwdata, wc, gr );
            gwfreeall ( gwdata );
            gwinit ( gwdata );
	    gwset_num_threads ( gwdata, THREADS );
            gwset_maxmulbyconst ( gwdata, (long) db );
            gwset_larger_fftlen_count ( gwdata, ++fft_size );
            gwsetup_general_mod_giant ( gwdata, gn );
            gwsetmulbyconst ( gwdata, (long) db );
            wr = gwalloc ( gwdata );
            wc = gwalloc ( gwdata );
            gwerror_checking ( gwdata, 1 );
            gianttogw ( gwdata, gr, wr );
            mpz_set ( zr, zc );
            i += j;
        }
    }
    w2m ( wr, gr, mr );
    gwdone ( gwdata );
    mpz_mod ( mr, mr, mn );
    mpz_clear ( zr );
    mpz_clear ( zn );
    mpz_clear ( zc );
    mpz_clear ( zrq );
    free ( gn );
    free ( gr );
}

/*****************************************************************************/

void medium_base_exp ( mpz_ptr mr, double db, mpz_srcptr me, mpz_srcptr mn )
{
    int fft_size = INI_FFT_SIZE;
    int string_length;
    int g_length;
    int zstep;
    int j;
    unsigned zb = db;
    char *string = (char*)malloc(MAX_STRING_LENGTH);
    mpz_t zr, zn, zc, zrq;
    giant gr, gn;
    gwnum wr, wc;
    gwhandle *gwdata;
    mpz_init ( zr );
    mpz_init ( zn );
    mpz_init ( zc );
    mpz_init ( zrq );
    mpz_mul_ui ( zn, mn, ZQ );
    mpz_get_str ( string, 10, zn );
    string_length = strlen ( string );
    zstep = ceil ( sqrt ( string_length ) ) + 1;
    g_length = ( string_length >> 2 ) + 16;
    gr = newgiant ( g_length );
    gn = newgiant ( g_length );
    ctog ( string, gn );
    gwdata = (gwhandle*) malloc ( sizeof ( gwhandle ) );
    gwinit ( gwdata );
    gwset_num_threads ( gwdata, THREADS );
    gwset_larger_fftlen_count ( gwdata, fft_size );
    gwsetup_general_mod_giant ( gwdata, gn );
    wr = gwalloc ( gwdata );
    wc = gwalloc ( gwdata );
    gwerror_checking ( gwdata, 1 );
    dbltogw ( gwdata, db, wr );
    mpz_set_ui ( zr, zb );
    for ( int i = mpz_sizeinbase ( me, 2 ) - 2 ; i >= 0; )
    {
        gwcopy ( gwdata, wr, wc );
        mpz_set ( zc, zr );
        for ( j = 0; j < zstep && i >= 0; j++, i-- )
        {
            gwsquare2 ( gwdata, wr, wr, 0x0000 );
            mpz_mul ( zr, zr, zr );
            mpz_mod_ui ( zr, zr, ZQ );
            if ( mpz_tstbit ( me, i ) )
            {
                gwsmallmul ( gwdata, db, wr );
                mpz_mul_ui ( zr, zr, zb );
                mpz_mod_ui ( zr, zr, ZQ );
            }
        }
        w2m ( wr, gr, mr );
        mpz_mod_ui ( zrq, mr, ZQ );
        if ( gw_get_maxerr ( gwdata ) > ROUND_OFF_LIMIT || mpz_cmp ( zrq, zr ) != 0 )
        {
 //           printf ( "*** (md) Upping FFT size due to round off error or Gerbicz error before bit %d.\n", i );
            gwtogiant ( gwdata, wc, gr );
            gwfreeall ( gwdata );
            gwinit ( gwdata );
	    gwset_num_threads ( gwdata, THREADS );
            gwset_larger_fftlen_count ( gwdata, ++fft_size );
            gwsetup_general_mod_giant ( gwdata, gn );
            wr = gwalloc ( gwdata );
            wc = gwalloc ( gwdata );
            gwerror_checking ( gwdata, 1 );
            gianttogw ( gwdata, gr, wr );
            mpz_set ( zr, zc );
            i += j;
        }
    }
    w2m ( wr, gr, mr );
    gwdone ( gwdata );
    mpz_mod ( mr, mr, mn );
    mpz_clear ( zr );
    mpz_clear ( zn );
    mpz_clear ( zc );
    mpz_clear ( zrq );
    free ( gn );
    free ( gr );
}

/*****************************************************************************/

void large_base_exp ( mpz_ptr mr, mpz_srcptr mb, mpz_srcptr me, mpz_srcptr mn )
{
    int fft_size = INI_FFT_SIZE;
    int string_length;
    int g_length;
    int j = mpz_sizeinbase ( me, 2 ) - 2;
    int k = j - SAFE;
    int zstep;
    char *string = (char*)malloc(MAX_STRING_LENGTH);
    mpz_t zr, zb, zn, zc, zrq;
    giant gr, gb, gn;
    gwnum wr, wb, wc;
    gwhandle *gwdata;
    mpz_init ( zr );
    mpz_init ( zn );
    mpz_init_set ( zb, mb );
    mpz_init ( zc );
    mpz_init ( zrq );
    mpz_mod_ui ( zb, zb, ZQ );
    mpz_mul_ui ( zn, mn, ZQ );
    mpz_get_str ( string, 10, zn );
    string_length = strlen ( string );
    zstep = ceil ( sqrt ( string_length ) ) + 1;
    g_length = ( string_length >> 2 ) + 16;
    gr = newgiant ( g_length );
    gb = newgiant ( g_length );
    gn = newgiant ( g_length );
    ctog ( string, gn );
    gwdata = (gwhandle*) malloc ( sizeof ( gwhandle ) );
    gwinit ( gwdata );
    gwset_num_threads ( gwdata, THREADS );
    gwset_larger_fftlen_count ( gwdata, fft_size );
    gwsetup_general_mod_giant ( gwdata, gn );
    wr = gwalloc ( gwdata );
    wb = gwalloc ( gwdata );
    wc = gwalloc ( gwdata );
    gwerror_checking ( gwdata, 1 );
    mpztog ( mb, gb );
    gianttogw ( gwdata, gb, wb );
    gwcopy ( gwdata, wb, wr );
    mpz_set ( zr, zb );
    for ( int i = j; i > k; i-- )
    {
        gwmul3_carefully ( gwdata, wr, wr, wr, 0x0000 );
        mpz_mul ( zr, zr, zr );
        mpz_mod_ui ( zr, zr, ZQ );
        if ( mpz_tstbit ( me, i ) )
        {
            gwmul_carefully ( gwdata, wb, wr );
            mpz_mul ( zr, zr, zb );
            mpz_mod_ui ( zr, zr, ZQ );
        }
    }
    for ( int i = k ; i > SAFE; )
    {
        gwcopy ( gwdata, wr, wc );
        mpz_set ( zc, zr );
        for ( j = 0; j < zstep && i > SAFE; j++, i-- )
        {
            gwsquare2 ( gwdata, wr, wr, 0x0000 );
            mpz_mul ( zr, zr, zr );
            mpz_mod_ui ( zr, zr, ZQ );
            if ( mpz_tstbit ( me, i ) )
            {
                gwmul3 ( gwdata, wb, wr, wr, 0x0002 );
                mpz_mul (zr, zr, zb );
                mpz_mod_ui ( zr, zr, ZQ );
            }
        }
        w2m ( wr, gr, mr );
        mpz_mod_ui ( zrq, mr, ZQ );
        if ( gw_get_maxerr ( gwdata ) > ROUND_OFF_LIMIT || mpz_cmp ( zrq, zr ) != 0 )
        {
//            printf ( "*** (lg) Upping FFT size due to round off error or Gerbicz error before bit %d.\n", i );
            gwtogiant ( gwdata, wc, gr );
            gwfreeall ( gwdata );
            gwinit ( gwdata );
	    gwset_num_threads ( gwdata, THREADS );
            gwset_larger_fftlen_count ( gwdata, ++fft_size );
            gwsetup_general_mod_giant ( gwdata, gn );
            wr = gwalloc ( gwdata );
            wb = gwalloc ( gwdata );
            wc = gwalloc ( gwdata );
            gwerror_checking ( gwdata, 1 );
            gianttogw ( gwdata, gb, wb );
            gianttogw ( gwdata, gr, wr );
            mpz_set ( zr, zc );
            i += j;
        }
    }
    for ( int i = SAFE; i >= 0; i-- )
    {
        gwmul3_carefully ( gwdata, wr, wr, wr, 0x0000 );
        mpz_mul ( zr, zr, zr );
        mpz_mod_ui ( zr, zr, ZQ );
        if ( mpz_tstbit ( me, i ) )
        {
            gwmul_carefully ( gwdata, wb, wr );
            mpz_mul ( zr, zr, zb );
	    mpz_mod_ui ( zr, zr, ZQ );
        }
    }
    w2m ( wr, gr, mr );
    mpz_mod_ui ( zrq, mr, ZQ );
    if ( mpz_cmp ( zrq, zr ) != 0 )
    {
	printf ( "Uh oh!!\n" );
	exit ( -1 );
    }
    gwdone ( gwdata );
    mpz_mod ( mr, mr, mn );
    mpz_clear ( zr );
    mpz_clear ( zn );
    mpz_clear ( zc );
    mpz_clear ( zrq );
    mpz_clear ( zb );
    free ( gn );
    free ( gr );
}

/*****************************************************************************/

int gw_prp ( mpz_srcptr mn )
{ // base 3 euler
    if ( mpz_sizeinbase ( mn, 2 ) < GW_THRESHOLD )
    {
        return ( mpz_probab_prime_p ( mn, 0) > 0 );
    }
    int res = 0;
    mpz_t mr;
    mpz_t me;
    mpz_init ( mr );
    mpz_init ( me );
    mpz_sub_ui ( me, mn, 1 );
    mpz_tdiv_q_2exp ( me, me, 1 );
    small_base_exp ( mr, 3.0, me, mn );
    if ( mpz_cmp_ui ( mr, 1 ) == 0 )
    {
        res = 1;
    }
    else
    {
        mpz_add_ui ( mr, mr, 1 );
        mpz_mod (mr, mr, mn );
        if ( mpz_cmp_ui ( mr, 0 ) == 0 )
        {
            res = 1;
        }
    }
    mpz_clear ( mr );
    mpz_clear ( me );
    return ( res );
}

/*****************************************************************************/

void gw_powm ( mpz_ptr mr, mpz_srcptr mb_inp, mpz_srcptr me, mpz_srcptr mn )
{
    if ( mpz_sizeinbase ( me, 2 ) < GW_THRESHOLD )
    {
        mpz_powm ( mr, mb_inp, me, mn );
        return;
    }
    int base_sign = mpz_sgn ( mb_inp );
    int base_size = mpz_sizeinbase ( mb_inp, 2 );
    double db;
    mpz_t mb;
    mpz_init_set ( mb, mb_inp );
    if ( base_sign == -1 )
    {
        mpz_neg ( mb, mb );
    }
    if ( base_size < 26 )
    {
        db = (double) mpz_get_ui ( mb );
        if ( base_size < 9 )
        {
            small_base_exp ( mr, db, me, mn );
        }
        else
        {
            medium_base_exp ( mr, db, me, mn );
        }
    }
    else
    {
        large_base_exp ( mr, mb, me, mn );
    }
    if ( base_sign == -1 && mpz_odd_p ( me ) )
    {
        mpz_neg ( mr, mr );
        mpz_mod ( mr, mr, mn );
    }
    mpz_clear ( mb );

}

/*****************************************************************************/
/*****************************************************************************/

